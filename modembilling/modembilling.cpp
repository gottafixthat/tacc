/*
** modembilling.cpp - This program connects to various databases and bills
**                    customers according to their modem usage.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
#include <map>
#include <ext/hash_map>

// Our local includes and libraries
#include <Cfg.h>
#include <ADB.h>

#include "common.h"
#include "AcctsRecv.h"

#define MAXIMPORTROWS 25000

#define configFile   "/usr/local/etc/modembilling.cf"
#define lockFileName "/var/run/modembilling.pid"


// Globals.  Mostly just used to make it shut up.
int		QuietMode = 0;
int     DebugMode = 0;


// Function prototypes
void processModemRecords(void);

int main (int argc, char **argv)
{
    int pid = 0;
	int opt;
    int sleepTime = 300;
    
    // Parse our arguments
    while((opt = getopt(argc, argv, "qd")) != EOF) {
		switch(opt) {
			case 'q':
				QuietMode = 1;
				break;
				
			case 'd':
				DebugMode = 1;
				break;
				
			default:
				exit(1);
				break;
		}
	}

    // Setup syslog and read in our configuration files
    openlog("modembilling", LOG_PID, LOG_DAEMON);
    loadCfg(configFile);
    if (atoi(cfgVal("SleepTime"))) sleepTime = atoi(cfgVal("SleepTime"));

    // Enter main loop.
    for (;;) {
        if ((pid = fork()) == 0) {
            // We successfully spawned
            break;
        } else {
            // Tha parent will continue on here.
            if (pid > 0) {
                // Wait for the child process to exit
                wait(NULL);
            } else {
                syslog(LOG_NOTICE, "Error forking child process");
            }
            sleep (sleepTime);
        }
    }

    loadCfg(configFile);

    ADB::setDefaultHost(cfgVal("AcctHost"));
    ADB::setDefaultDBase(cfgVal("AcctDB"));
    ADB::setDefaultUser(cfgVal("AcctUser"));
    ADB::setDefaultPass(cfgVal("AcctPass"));

    syslog(LOG_NOTICE, "Processing billable modem connections");
    // Convert the customers...
    processModemRecords();
    syslog(LOG_NOTICE, "Finished importing.");
}

/*
** processModemRecords - Scans the modem usage records and bills
**                       customers for it.
*/

void processModemRecords(void)
{
    ADB     muDB(cfgVal("ModemUsageDB"), cfgVal("ModemUsageUser"), cfgVal("ModemUsagePass"), cfgVal("ModemUsageHost"));
    ADB     muDB2(cfgVal("ModemUsageDB"), cfgVal("ModemUsageUser"), cfgVal("ModemUsagePass"), cfgVal("ModemUsageHost"));
    ADB     acDB;       // We set the defaults above

    muDB.query("select * from %s where CalledNumber in (%s) and Billed = 0", cfgVal("ModemUsageTable"), cfgVal("BillableNumbers"));
    if (muDB.rowCount) {
        syslog(LOG_NOTICE, "Found %ld calls to bill", muDB.rowCount);
        while(muDB.getrow()) {
            AcctsRecv   AR;
            float       qty;
            // Get the necessary information out of the database.
            // This is a seriously harsh query, but it works.
            acDB.query("select Billables.ItemNumber, Billables.Description, BillablesData.Price, Customers.CustomerID from Billables, BillablesData, Customers, Logins where Billables.ItemID = '%s' and BillablesData.ItemNumber = Billables.ItemNumber and BillablesData.RatePlanID = Customers.RatePlan and BillablesData.CycleID = Customers.BillingCycle and Customers.CustomerID = Logins.CustomerID and Logins.LoginID = '%s'", cfgVal("BillableName"), muDB.curRow["LoginID"]);
            if (acDB.rowCount) {
                // Got the info.
                std::string memoStr;
                char   tmpLen[1024];
                acDB.getrow();
                syslog(LOG_NOTICE, "Found entry for CustomerID %s, rate '%s'", acDB.curRow["CustomerID"], acDB.curRow["Price"]);
                // We have enough to process the billable now.
                // Since we charge by the minute, divide the users session time
                // by 60 to get the quantity
                qty = atoi(muDB.curRow["SessionLength"]) / 60.0;
                AR.ARDB->setValue("TransDate", muDB.curRow["StartDate"]);
                AR.ARDB->setValue("CustomerID", atol(acDB.curRow["CustomerID"]));
                AR.ARDB->setValue("LoginID", muDB.curRow["LoginID"]);
                AR.ARDB->setValue("ItemID", acDB.curRow["ItemNumber"]);
                AR.ARDB->setValue("RefNo", muDB.curRow["InternalID"]);
                AR.ARDB->setValue("TransType", TRANSTYPE_CUSTCHARGE);
                AR.ARDB->setValue("Quantity", qty);
                AR.ARDB->setValue("StartDate", muDB.curRow["StartDate"]);
                AR.ARDB->setValue("EndDate", muDB.curRow["StopDate"]);
                AR.ARDB->setValue("Price", atof(acDB.curRow["Price"]));
                AR.ARDB->setValue("Amount", qty * atof(acDB.curRow["Price"]));
                memoStr = acDB.curRow["Description"];
                sprintf(tmpLen, "%.2f", qty);
                while (memoStr.find("{CalledFrom}") != string::npos) memoStr.replace(memoStr.find("{CalledFrom}"), strlen("{CalledFrom}"), muDB.curRow["CallerID"]);
                while (memoStr.find("{CallLength}") != string::npos) memoStr.replace(memoStr.find("{CallLength}"), strlen("{CallLength}"), tmpLen);
                while (memoStr.find("{CallStartTime}") != string::npos) memoStr.replace(memoStr.find("{CallStartTime}"), strlen("{CallStartTime}"), muDB.curRow["StartTime"]);
                while (memoStr.find("{BillableRate}") != string::npos) memoStr.replace(memoStr.find("{BillableRate}"), strlen("{BillableRate}"), acDB.curRow["Price"]);

                AR.ARDB->setValue("Memo", memoStr.c_str());
                syslog(LOG_NOTICE, "Saving AcctsRecv entry");
                AR.SaveTrans();

                // Now, make sure we don't bill for this one again.
                muDB2.dbcmd("update ModemUsage set Billed = 1 where InternalID = %s", muDB.curRow["InternalID"]);
            } else {
                syslog(LOG_NOTICE, "Could not find Billable for login '%s'", muDB.curRow["LoginID"]);
            }
        }
    }
}


