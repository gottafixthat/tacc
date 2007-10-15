/*
** radimport.cpp - This program connects to the radius accounting server and
**                 imports the accounting records from the UserRecords DB.
**                 it then stores them into the bacct database ModemUsage and
**                 marks the remote record as "Exported".  When its all done
**                 it deletes all of the "Exported" records that are more than
**                 a week old from the remote database to conserve space.
**
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
//using namespace std;
#include <map>
#include <ext/hash_map>
#include <list>
#include <string>

// Our local includes and libraries
#include <Cfg.h>
#include <ADB.h>

#define MAXIMPORTROWS 50000

#define configFile   "/usr/local/etc/radimport.cf"
#define lockFileName "/var/run/radimport.pid"


struct smapeq
{
    bool operator()(const char *s1, const char *s2) const 
    {
        return strcmp(s1, s2) == 0;
    };
};

struct imapeq
{
    bool operator()(int v1, int v2) const
    {
        return (v1 == v2);
    };
};

typedef __gnu_cxx::hash_map<const char *, int, __gnu_cxx::hash<const char *>, smapeq> StringMap;
StringMap   strxlt;

// Globals.  Mostly just used to make it shut up.
int		QuietMode = 0;
int     DebugMode = 0;
char	Today[64];

static char termxlt[20][128];
static char portxlt[20][128];


// Function prototypes
void getRemoteDB(void);
void deleteExported(void);
void loadDict(void);
int  addToDict(const char * newStr);
long ipstr2long(const char *ip_str);

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
    openlog("radimport", LOG_PID, LOG_DAEMON);
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

	


    syslog(LOG_NOTICE, "Connecting to remote database to import records");
    // Convert the customers...
    getRemoteDB();
    deleteExported();
    syslog(LOG_NOTICE, "Finished importing.");
}

/*
** getRemoteDB - Converts the customer data.
*/

void getRemoteDB(void)
{


    // Load the dictionary for the translations...
    loadDict();

    if (DebugMode) fprintf(stderr, "Connecting to RADIUS database %s@%s as %s\n", cfgVal("RadiusDB"), cfgVal("RadiusHost"), cfgVal("RadiusUser"));
    ADB         RadDB(cfgVal("RadiusDB"), cfgVal("RadiusUser"), cfgVal("RadiusPass"), cfgVal("RadiusHost"));
    ADB         RadDB2(cfgVal("RadiusDB"), cfgVal("RadiusUser"), cfgVal("RadiusPass"), cfgVal("RadiusHost"));
    if (DebugMode) fprintf(stderr, "Connecting to Accounting database %s@%s as %s\n", cfgVal("AcctDB"), cfgVal("AcctHost"), cfgVal("AcctUser"));
    ADB         AcctDB(cfgVal("AcctDB"), cfgVal("AcctUser"), cfgVal("AcctPass"), cfgVal("AcctHost"));
    ADBTable    AcctTable(cfgVal("AcctTable"), cfgVal("AcctDB"), cfgVal("AcctUser"), cfgVal("AcctPass"), cfgVal("AcctHost"));

    long        impCount = 0;

    std::string      startDateTime;
    std::string      stopDateTime;
    char        startDateStr[1024];
    char        startTimeStr[1024];
    char        stopDateStr[1024];
    char        stopTimeStr[1024];

    // Hostname lookup vars
    struct hostent  *hp;
    static char     hstname[128];
    unsigned int    n_ipaddr;
    std::string          hostonly;

    // Load all of the stop records.  They contain everything we need.
    RadDB.query("select * from %s where AcctStopTime <> '0000-00-00 00:00:00' and exported = 0 limit %ld", cfgVal("RadiusTable"), MAXIMPORTROWS);
    syslog(LOG_NOTICE, "Found %ld remote radius accounting records", RadDB.rowCount);
    if (DebugMode) printf("Found %ld radius accounting records.\n", RadDB.rowCount);
    
    if (RadDB.rowCount) {
        // if (!QuietMode) printf("Reading and inserting...\n");
        if (DebugMode) {
            printf("%-8s %-10s %-16s %-3s %-20s %-10s %-10s %-8s\n",
              "Int ID",
              "Login ID",
              "NAS ID",
              "Prt",
              "Connect Info",
              "Out Bytes",
              "In Bytes",
              "Seconds"
            );
        }
        while (RadDB.getrow()) {
            AcctTable.clearData();
            if (DebugMode) {
                printf("%-8s %-10s %-16s %-3s %-20s %-10s %-10s %-8s\n",
                  RadDB.curRow["RadAcctId"],
                  RadDB.curRow["UserName"],
                  RadDB.curRow["NASIPAddress"],
                  RadDB.curRow["NASPortID"],
                  RadDB.curRow["ConnectInfo_start"],
                  RadDB.curRow["AcctOutputOctets"],
                  RadDB.curRow["AcctInputOctets"],
                  RadDB.curRow["AcctSessionTime"]
                );
            }

            // Get the start and stop times and turn them into something
            // that MySQL likes
            startDateTime.assign(RadDB.curRow["AcctStartTime"]);
            strcpy(startDateStr, startDateTime.substr(0,10).c_str());
            strcpy(startTimeStr, startDateTime.substr(11,8).c_str());

            stopDateTime.assign(RadDB.curRow["AcctStopTime"]);
            strcpy(stopDateStr, stopDateTime.substr(0,10).c_str());
            strcpy(stopTimeStr, stopDateTime.substr(11,8).c_str());

            // Do the translation from the remote radius database to the
            // local accounting database.  The fields are totally different.
            
            AcctTable.setValue("LoginID",       RadDB.curRow["UserName"]);
            AcctTable.setValue("StartDate",     startDateStr);
            AcctTable.setValue("StartTime",     startTimeStr);
            AcctTable.setValue("StopDate",      stopDateStr);
            AcctTable.setValue("StopTime",      stopTimeStr);
            AcctTable.setValue("SessionLength", RadDB.curRow.col("AcctSessionTime")->toLong());
            AcctTable.setValue("Port",          RadDB.curRow.col("NASPortId")->toInt());
            AcctTable.setValue("Speed",         "");
            AcctTable.setValue("InBytes",       RadDB.curRow.col("AcctInputOctets")->toLong());
            AcctTable.setValue("OutBytes",      RadDB.curRow.col("AcctOutputOctets")->toLong());
            AcctTable.setValue("CallerID",      RadDB.curRow["CallingStationId"]);
            AcctTable.setValue("CalledNumber",  RadDB.curRow["CalledStationId"]);

            // get the host name by address.
            n_ipaddr = htonl(ipstr2long(RadDB.curRow["NASIPAddress"]));
            hp = gethostbyaddr((char *) &n_ipaddr, sizeof(struct in_addr), AF_INET);
            strcpy(hstname, hp->h_name);

            // If a DomainName was specified in the configuration file, 
            // strip it off of this host name.
            if (strlen(cfgVal("DomainName"))) {
                hostonly.assign(hstname);
                if (hostonly.rfind(cfgVal("DomainName")) != std::string::npos) {
                    // Found it, strip it.  Strip out one extra for the "."
                    hostonly.replace(hostonly.rfind(cfgVal("DomainName"))-1, strlen(cfgVal("DomainName")) + 1, "");
                    strcpy(hstname, hostonly.c_str());
                }
            }

            // Get the translations for the lookup entries.
            if (!strxlt[hstname]) {
                addToDict(hstname);
            }
            AcctTable.setValue("Host", strxlt[hstname]);

            // Add the terminate cause.
            if (!strxlt[RadDB.curRow["AcctTerminateCause"]]) {
                addToDict(RadDB.curRow["AcctTerminateCause"]);
            }
            AcctTable.setValue("TermCause", strxlt[RadDB.curRow["AcctTerminateCause"]]);
            
            // Add the session type
            if (!strxlt[RadDB.curRow["NASPortType"]]) {
                addToDict(RadDB.curRow["NASPortType"]);
            }
            AcctTable.setValue("SessionType", strxlt[RadDB.curRow["NASPortType"]]);
            AcctTable.ins();
            
            // And update the Exported flag in the remote system.
            RadDB2.dbcmd("update %s set exported = 1 where RadAcctId = %d", cfgVal("RadiusTable"), atoi(RadDB.curRow["RadAcctId"]));
            
            impCount++;

            if (impCount == MAXIMPORTROWS) {
                syslog(LOG_NOTICE, "Imported maximum number of records (%ld) stopping", impCount);
                return;
            }

            if ((impCount % 1000) == 0) {
                syslog(LOG_NOTICE, "Checkpoint: Imported %ld remote RADIUS records", impCount);
            }
        }
    }

    syslog(LOG_NOTICE, "Imported %ld remote RADIUS records", impCount);
}

/*
** deleteExported - Connects to the remote database and deletes all rows
**                  that are marked as exported that are also over 1 week old.
*/

void deleteExported(void)
{
    if (DebugMode) fprintf(stderr, "Connecting to RADIUS database %s@%s as %s to delete exported records\n", cfgVal("RadiusDB"), cfgVal("RadiusHost"), cfgVal("RadiusUser"));
    ADB         RadDB(cfgVal("RadiusDB"), cfgVal("RadiusUser"), cfgVal("RadiusPass"), cfgVal("RadiusHost"));
    RadDB.dbcmd("delete from radacct where exported = 1");
}



/*
** loadDict - Loads the string dictionary for converting space consuming
**            strings into integer keys.
*/

void loadDict(void)
{
    
    if (DebugMode) fprintf(stderr, "Connecting to Accounting database %s@%s as %s to load ModemUsageStrings\n", cfgVal("AcctDB"), cfgVal("AcctHost"), cfgVal("AcctUser"));
    int     xlateCount = 0;
    ADB     DB(cfgVal("AcctDB"), cfgVal("AcctUser"), cfgVal("AcctPass"), cfgVal("AcctHost"));

    DB.query("select * from ModemUsageStrings order by TextStr");
    if (DB.rowCount) while (DB.getrow()) {
        xlateCount++;
        char    *tmpStr = new char[strlen(DB.curRow["TextStr"]) + 16];
        strcpy(tmpStr, DB.curRow["TextStr"]);
        strxlt[tmpStr] = DB.curRow.col("TextID")->toInt();
    }
    if (DebugMode) fprintf(stderr, "Loaded %d dictionary entries\n", xlateCount);

    if (DebugMode) fprintf(stderr, "Setting up RADIUS dictionary entries for terminate causes and session types\n");
    
    strcpy(termxlt[0] , "Unknown");
    strcpy(termxlt[1] , "User-Request");
    strcpy(termxlt[2] , "Lost-Carrier");
    strcpy(termxlt[3] , "Lost-Service");
    strcpy(termxlt[4] , "Idle-Timeout");
    strcpy(termxlt[5] , "Session-Timeout");
    strcpy(termxlt[6] , "Admin-Reset");
    strcpy(termxlt[7] , "Admin-Reboot");
    strcpy(termxlt[8] , "Port-Error");
    strcpy(termxlt[9] , "NAS-Error");
    strcpy(termxlt[10], "NAS-Request");
    strcpy(termxlt[11], "NAS-Reboot");
    strcpy(termxlt[12], "Port-Unneeded");
    strcpy(termxlt[13], "Port-Preempted");
    strcpy(termxlt[14], "Port-Suspended");
    strcpy(termxlt[15], "Service-Unavailable");
    strcpy(termxlt[16], "Callback");
    strcpy(termxlt[17], "User-Error");
    strcpy(termxlt[18], "Host-Request");

    strcpy(portxlt[0], "Async");
    strcpy(portxlt[1], "Sync");
    strcpy(portxlt[2], "ISDN");
    strcpy(portxlt[3], "ISDN-V120");
    strcpy(portxlt[4], "ISDN-V110");

}

/*
** addToDict - Adds a dictionary entry that is not found.
**
**             Returns the new entry after reloading the dictionary.
*/

int addToDict(const char * newStr)
{
    int retVal = 0;
    if (DebugMode) fprintf(stderr, "Connecting to Accounting database %s@%s as %s to load ModemUsageStrings\n", cfgVal("AcctDB"), cfgVal("AcctHost"), cfgVal("AcctUser"));
    ADBTable DB("ModemUsageStrings", cfgVal("AcctDB"), cfgVal("AcctUser"), cfgVal("AcctPass"), cfgVal("AcctHost"));

    char    *tmpStr = new char[strlen(newStr) + 16];
    strcpy(tmpStr, newStr);

    DB.setValue("TextStr",  newStr);
    retVal = DB.ins();
    strxlt[tmpStr] = DB.getInt("TextID");

    return retVal;
}



/*
** ipstr2long - Converts a string IP address into ip long notation.
*/

long ipstr2long(const char *ip_str)
{
	char	buf[6];
	char	*ptr;
	int	i;
	int	count;
	long ipaddr;
	int	cur_byte;

	ipaddr = (long)0;
	for(i = 0;i < 4;i++) {
		ptr = buf;
		count = 0;
		*ptr = '\0';
		while(*ip_str != '.' && *ip_str != '\0' && count < 4) {
			if(!isdigit(*ip_str)) {
				return((long)0);
			}
			*ptr++ = *ip_str++;
			count++;
		}
		if(count >= 4 || count == 0) {
			return((long)0);
		}
		*ptr = '\0';
		cur_byte = atoi(buf);
		if(cur_byte < 0 || cur_byte > 255) {
			return((long)0);
		}
		ip_str++;
		ipaddr = ipaddr << 8 | (long)cur_byte;
	}
	return(ipaddr);
}

