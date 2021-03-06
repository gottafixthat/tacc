/*
** ccget  - This program ties the CCC credit card payment functions and
**          Total AccountAbility together.  It reads the posted charges
**          from the remote CCC database, inserts them into the local TAA
**          database and then removes them from the remote CCC database.
**
**          It should be installed and run as a cron job every 30 minutes
**          or so.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <syslog.h>

/*#include <qstrlist.h>*/

#include <ADB.h>
/*#include <BString.h>*/
#include <StrTools.h>
#include <CCValidate.h>
#include <Cfg.h>
#include <loadKey.h>

#include <bdes.h>



// Function Prototypes
int  main(int, char **);
void cfgInit();
int  processRemoteCCards(void);

void loadKey(void);

/*
** main     - Program entry point.
*/

int main(int, char **)
{
    //ADB::setDebugLevel(9);
    cfgInit();
    loadKey();
    processRemoteCCards();
}

/*
** cfgInit - Loads the configuration file from /etc/ldapsync.cf,
**           /usr/local/etc/ldapsync.cf or ./ldapsync.cf, in that order.
*/

void cfgInit()
{
    if (!loadCfg("/etc/ccget.cf")) {
        if (!loadCfg("/usr/local/etc/ccget.cf")) {
            if (!loadCfg("ccget.cf")) {
                fprintf(stderr, "Unable to find a configuration file.  Aborting.\n");
                exit(-1);
            }
        }
    }

    if (!strlen(cfgVal("REMOTEHOST")))   setCfgVal("REMOTEHOST",   "guysmiley.blarg.net");
    if (!strlen(cfgVal("REMOTEDBASE")))  setCfgVal("REMOTEDBASE",  "CCC");
    if (!strlen(cfgVal("LOCALHOST")))    setCfgVal("LOCALHOST",    "localhost");
    if (!strlen(cfgVal("LOCALDBASE")))   setCfgVal("LOCALDBASE",   "bacct");
    if (!strlen(cfgVal("REMOTEUSER")))   setCfgVal("REMOTEUSER",   "nobody");
    if (!strlen(cfgVal("REMOTEPASS")))   setCfgVal("REMOTEPASS",   "3H8a35J0");
    if (!strlen(cfgVal("LOCALUSER")))    setCfgVal("LOCALUSER",    "marc");
    if (!strlen(cfgVal("LOCALPASS")))    setCfgVal("LOCALPASS",    "808Frm!3");
}
    
/*
** processRemoteCCards - Opens two database connections, one remote and one
**                       local.  It copies the remote CCards to the local
**                       database and then deletes them from the remote
**                       database if the local insert was successful.
*/

int processRemoteCCards()
{
    int RetVal = 0;
    //QStrList    tmpList;
    __gnu_cxx::StrSplitMap tmpList;
    char        *encStr = new char[65536];
    char        *decStr = new char[65536];
    char        cardType[1024];
    int         transCount = 0;
    long        insSuccess;
    
    ADB         RemCCDB(cfgVal("REMOTEDBASE"), cfgVal("REMOTEUSER"), cfgVal("REMOTEPASS"), cfgVal("REMOTEHOST"));
    ADB         RemCCDB2(cfgVal("REMOTEDBASE"), cfgVal("REMOTEUSER"), cfgVal("REMOTEPASS"), cfgVal("REMOTEHOST"));
    ADBTable    LocCCDB("CCTrans", cfgVal("LOCALDBASE"), cfgVal("LOCALUSER"), cfgVal("LOCALPASS"), cfgVal("LOCALHOST"));
    ADB         LocTmpDB(cfgVal("LOCALDBASE"), cfgVal("LOCALUSER"), cfgVal("LOCALPASS"), cfgVal("LOCALHOST"));

    /*
    if (!RemCCDB.connected) return(-1);
    if (!LocCCDB.connected) return(-1);
    */

    LocCCDB.setEncryptedColumn("CardNo");    

    //RemCCDB.query("select InternalID, CustomerID, LoginID, DateEntered, TimeEntered, WantsAutomatic, DECODE(CCData, '%s'), CCCVersion, NeedsExport from CCPayments where NeedsExport = 1 order by CustomerID", cfgVal("ENCODEKEY"));
    RemCCDB.query("select DECODE(CCPayments.CCData, '%s'), CCPayments.* from CCPayments where NeedsExport = 1 order by CustomerID", cfgVal("ENCODEKEY"));
    //fprintf(stderr, "Query was successful\n");

    if (RemCCDB.rowCount) {
        //ADB::setDebugLevel(99);
        while (RemCCDB.getrow()) {
            //fprintf(stderr, "Encode Key = '%s', CCData = '%s'\n", cfgVal("ENCODEKEY"), RemCCDB.curRow[0]);
            // Check to see what encryption/encoding version we are using
            // on the CCC Transaction data.
            if (RemCCDB.curRow.col("CCCVersion")->toInt() == 1) {
                // It was "encoded" with the MySQL encode function and a 
                // shared key.  Decode it.
                //LocTmpDB.query("select DECODE('%s', '%s')", RemCCDB.curRow["CCData"], cfgVal("ENCODEKEY"));
                //LocTmpDB.getrow();
                strcpy(decStr, RemCCDB.curRow[0]);
            } else {
                strcpy(encStr, RemCCDB.curRow["CCData"]);
                decrypt_string2((unsigned char *) encStr, (unsigned char *) decStr);
            }
            //fprintf(stderr, "Decoded data = '%s'\n", decStr);
            
            //tmpList.clear();
            StrSplit(decStr, "\t", tmpList);
            //splitString(decStr, '\t', tmpList, 0);
            
            // Okay, insert the new one.
            LocCCDB.setValue("InternalID", (long) 0);
            LocCCDB.setValue("CustomerID", RemCCDB.curRow.col("CustomerID")->toLong());
            LocCCDB.setValue("EnteredBy",  RemCCDB.curRow["LoginID"]);
            LocCCDB.setValue("TransDate",  RemCCDB.curRow["DateEntered"]);
            LocCCDB.setValue("Name",       tmpList[0].c_str());
            LocCCDB.setValue("Address",    tmpList[1].c_str());
            LocCCDB.setValue("ZIP",        tmpList[4].c_str());
            LocCCDB.setValue("BPSExported", 0);
            LocCCDB.setValue("BPSEDate",   "");
            LocCCDB.setValue("ASExported", "0");
            LocCCDB.setValue("ASEDate",    "");
            LocCCDB.setValue("Confirmation", (long) 0);
            LocCCDB.setValue("Amount",     atof(tmpList[8].c_str()));
            
            // WRONG!!!
            if (!strcasecmp("visa", tmpList[5].c_str())) {
                sprintf(cardType, "%d", CCTYPE_VISA);
            } else if (!strcasecmp("mastercard", tmpList[5].c_str())) {
                sprintf(cardType, "%d", CCTYPE_MC);
            } else if (!strcasecmp("amex", tmpList[5].c_str())) {
                sprintf(cardType, "%d", CCTYPE_AMEX);
            } else {
                sprintf(cardType, "%d", CCTYPE_UNKNOWN);
            } 
            LocCCDB.setValue("CardType", cardType);
            LocCCDB.setValue("ExpDate",    tmpList[7].c_str());
            LocCCDB.setValue("CardNo",     tmpList[6].c_str());

            insSuccess = 0;
            // Check to see if this is an actual payment, or if they are
            // just updating their automatic payment.
            if (atof(tmpList[8].c_str()) > 0.99) {
                //fprintf(stderr, "Inserting transaction...\n");
                insSuccess = LocCCDB.ins();
            } else {
                // We don't want to insert the transaction, but we 
                // do want to indicate success.
                insSuccess = 1;
            }


            if (insSuccess) {
                // We inserted okay.
                // Did they want automatic?  If so, process that, too.
                //fprintf(stderr, "Checking for automatic...\n");
                if (RemCCDB.curRow.col("WantsAutomatic")->toInt()) {
                    ADB         DB(cfgVal("LOCALDBASE"), cfgVal("LOCALUSER"), cfgVal("LOCALPASS"), cfgVal("LOCALHOST"));
                    ADBTable    AutoDB("AutoPayments", cfgVal("LOCALDBASE"), cfgVal("LOCALUSER"), cfgVal("LOCALPASS"), cfgVal("LOCALHOST"));
                    char        *dbcmd = new char[65536];
				    AutoDB.setEncryptedColumn("Name");
				    AutoDB.setEncryptedColumn("Address");
				    AutoDB.setEncryptedColumn("CityState");
				    AutoDB.setEncryptedColumn("ZIP");
				    AutoDB.setEncryptedColumn("Phone");
				    AutoDB.setEncryptedColumn("AuthName");
				    AutoDB.setEncryptedColumn("BankName");
				    AutoDB.setEncryptedColumn("BankCity");
				    AutoDB.setEncryptedColumn("BankState");
				    AutoDB.setEncryptedColumn("CardType");
				    AutoDB.setEncryptedColumn("AcctNo");
                    
                    // First, make sure that they only have one active 
                    // automatic payment -- this one.
                    //fprintf(stderr, "Removing old automatic payments...\n");
                    sprintf(dbcmd, "update AutoPayments set Active = 0 where CustomerID = %ld", RemCCDB.curRow.col("CustomerID")->toLong());
                    DB.dbcmd(dbcmd);
                    //fprintf(stderr, "Old automatic payments removed...\n");
                    
                    // Okay, set up the insert.
                    AutoDB.setValue("InternalID", (long) 0);
                    AutoDB.setValue("CustomerID",       RemCCDB.curRow.col("CustomerID")->toLong());
                    AutoDB.setValue("AuthorizedOn",     RemCCDB.curRow["DateEntered"]);
                    AutoDB.setValue("AuthorizedBy",     RemCCDB.curRow["LoginID"]);
                    AutoDB.setValue("EnteredBy",        RemCCDB.curRow["LoginID"]);
                    AutoDB.setValue("Active",           1);
                    AutoDB.setValue("TransDate",        RemCCDB.curRow["DateEntered"]);
                    AutoDB.setValue("ChargeDay",        7);
                    AutoDB.setValue("Name",             tmpList[0].c_str());
                    AutoDB.setValue("Address",          tmpList[1].c_str());
                    AutoDB.setValue("ZIP",              tmpList[4].c_str());
                    AutoDB.setValue("AuthName",         tmpList[0].c_str());
                    AutoDB.setValue("PaymentType",      1);
                    AutoDB.setValue("CardType",         cardType);
                    
                    //fprintf(stderr, "Fixing up the expiration date...\n");
                    // Fix the expiration date.
                    int     month, year;
                    string  tmpQStr;
                    //QCString tmpQStr;
                    //QDate   tmpDate1;
                    //char    tmpDate1[1024];
                    char    tmpDate[512];
                    int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
                    month = atoi(tmpList[7].substr(0,2).c_str());
                    //fprintf(stderr, "    month = %d...\n", month);

                    year  = atoi(tmpList[7].substr(2,2).c_str());
                    //fprintf(stderr, "    year= %d...\n", year);

                    //tmpQStr.setStr(tmpList[7].c_str());
                    //month   = atoi(tmpQStr.left(2));
                    //year    = atoi(tmpQStr.right(2));
                    // Check for 2000
                    if (year < 95) year += 2000;
                    else year += 1900;
                    sprintf(tmpDate, "%04d-%02d-%02d", year, month, monthDays[month-1]);
                    //tmpDate1.setYMD(year, month, 1);
                    //tmpDate1.setYMD(year, month, tmpDate1.daysInMonth());
                    //QDatetomyDate(tmpDate, tmpDate1);
                    
                    //fprintf(stderr, "Setting the Expiration Date...\n");
                    AutoDB.setValue("ExpDate",          tmpDate);
                    //fprintf(stderr, "Setting the card number...\n");
                    AutoDB.setValue("AcctNo",           tmpList[6].c_str());
                    
                    // All done...Now insert it...
                    //fprintf(stderr, "Inserting new automatic payment...\n");
                    AutoDB.ins();
                }

                // All finished with this record.  Delete it from
                // the remote server.
                //RemCCDB2.dbcmd("delete from CCPayments where InternalID = %ld", RemCCDB.curRow.col("InternalID")->toLong());
                RemCCDB2.dbcmd("update CCPayments set NeedsExport = 0 where InternalID = %ld", RemCCDB.curRow.col("InternalID")->toLong());
                transCount++;
            } else {
                // There was a problem.  Log it.
                syslog(LOG_INFO, "Error inserting CC payment.  Remote ID = %ld.  Skipping.", RemCCDB.curRow.col("InternalID")->toLong());
            }
            
        }
        syslog(LOG_INFO, "Imported %d credit card transactions", transCount);
    } else {
        syslog(LOG_INFO, "No transactions found on remote system");
    }
    
    
    return RetVal;
}


