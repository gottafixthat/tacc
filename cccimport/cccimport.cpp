/* $Id$
**
** cccimport  - Called from cron at lest 4 times an hour, it connects to 
**              the CCC's database and retrieves any log entries and new 
**              user records.  It then adds them to the appropriate customer
**              record.
**
** Configuration file.  The default configuration file for this is located
** /usr/local/etc/cccimport.cf.  It will need to contain the following
** configuration entries for the program to work correctly:
**
** # The database connection info.
** DBHost        = localhost
** DBName        = bacct
** DBUser        = marc
** DBPass        = password
**
** CCCDBHost     = beaker.blarg.net
** CCCDBName     = CCC
** CCCDBUser     = taa
** CCCDBPass     = password
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// Qt Includes
//#include <qlist.h>
//#include <qdatetm.h>
//#include <qregexp.h>
//#include <qstrlist.h>
//#include <qstring.h>

// Blarg includes
#include <ADB.h>
#include <Cfg.h>
//#include <bdes.h>
#include <FParse.h>
#include <BlargDB.h>
#include <TAATools.h>


#define CONFIGFILE   "/usr/local/etc/cccimport.cf"
#define PROGNAME     "cccimport"


// Function prototypes
int  dbSetup(void);
void importLogins(void);
void importLogs(void);
void parseFile(long CustomerID, const char *LoginID, const char *parentID, const char *Action);
void addNote(long CustomerID, const char *LoginID, const char *ExpDate, int isActive);



main(int, char **)
{
    openlog(PROGNAME, LOG_PID|LOG_NDELAY, LOG_INFO);
    loadCfg(CONFIGFILE);
    if (dbSetup()) {
        importLogins();
        importLogs();
    } else {
        syslog(LOG_INFO, "Unable to connect to the database - Aborting");
    }
}


/*
** dbSetup       - Checks the configuration information to make sure that
**                 our database connection information has been properly
**                 configured.
**
**                 It also sets the ADB default information for connection.
**
**                 Returns 0 on failure, 1 on success.
*/

int dbSetup(void)
{
    int     RetVal = 1;         // Assume success
    // Set up our database connection.
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));
    return RetVal;
}


/*
** importLogins - Gets new logins and adds them to customer records.  It
**                also adds the subscription items for them.
*/

void importLogins(void)
{
    ADB         cccDB(cfgVal("CCCDBName"), cfgVal("CCCDBUser"), cfgVal("CCCDBPass"), cfgVal("CCCDBHost"));
    ADB         cccDB2(cfgVal("CCCDBName"), cfgVal("CCCDBUser"), cfgVal("CCCDBPass"), cfgVal("CCCDBHost"));
    LoginsDB    loginsDB;
    ADB         DB;

    cccDB.query("select * from MailboxActions order by InternalID");
    if (!cccDB.rowCount) {
        syslog(LOG_INFO, "No new logins found to import");
        return;
    }

    syslog(LOG_INFO, "Found %ld new logins to import", cccDB.rowCount);
    while (cccDB.getrow()) {
        syslog(LOG_INFO, "%s %s for customer %ld by %s", cccDB.curRow["Action"], cccDB.curRow["LoginID"], atol(cccDB.curRow["CustomerID"]), cccDB.curRow["PrimaryLogin"]);
        loginsDB.clearData();
        // Are we suspending or creating?
        if (!strcasecmp("Create", cccDB.curRow["Action"])) {
            // Creating.
            // Get a couple of fields from the local LoginTypes DB.
            DB.query("select * from LoginTypes where InternalID = %d", atoi(cccDB.curRow["LoginType"]));
            DB.getrow();

            loginsDB.setValue("CustomerID",     atol(cccDB.curRow["CustomerID"]));
            loginsDB.setValue("LoginID",        cccDB.curRow["LoginID"]);
            loginsDB.setValue("LoginType",      atoi(cccDB.curRow["LoginType"]));
            loginsDB.setValue("Opened",         timeToStr(rightNow(), YYYY_MM_DD));
            loginsDB.setValue("DiskSpace",      atoi(DB.curRow["DiskSpace"]));
            loginsDB.setValue("DialChannels",   atoi(DB.curRow["DialupChannels"]));
            loginsDB.setValue("LastModified",   timeToStr(rightNow(),  YYYY_MM_DD_HH_MM_SS));
            loginsDB.setValue("Active",         1);
            loginsDB.ins();
        } else {
            // Suspending.
            loginsDB.get(atol(cccDB.curRow["CustomerID"]), cccDB.curRow["LoginID"]);
            loginsDB.setValue("Active", 0);
            loginsDB.setValue("Closed", timeToStr(rightNow(), YYYY_MM_DD));
            loginsDB.setValue("LastModified",   timeToStr(rightNow(),  YYYY_MM_DD_HH_MM_SS));
            loginsDB.upd();
            // And thats all there is to a suspension
        }

        // Send email.
        parseFile(atol(cccDB.curRow["CustomerID"]), cccDB.curRow["LoginID"], cccDB.curRow["PrimaryLogin"], cccDB.curRow["Action"]);

        // Now, remove the entry so we don't process it again.
        cccDB2.dbcmd("delete from MailboxActions where InternalID = %ld", atol(cccDB.curRow["InternalID"]));
    }

}

/*
** importLogs - Gets log entries and imports them.
*/

void importLogs(void)
{
    ADB         cccDB(cfgVal("CCCDBName"), cfgVal("CCCDBUser"), cfgVal("CCCDBPass"), cfgVal("CCCDBHost"));
    ADB         cccDB2(cfgVal("CCCDBName"), cfgVal("CCCDBUser"), cfgVal("CCCDBPass"), cfgVal("CCCDBHost"));
    ADBTable    notesDB("Notes");

    cccDB.query("select * from TAANotes order by InternalID");
    if (!cccDB.rowCount) {
        syslog(LOG_INFO, "No new notes found to import");
        return;
    }

    syslog(LOG_INFO, "Found %ld new notes to import", cccDB.rowCount);
    while (cccDB.getrow()) {
        syslog(LOG_INFO, "Customer %ld %s - %s", atol(cccDB.curRow["CustomerID"]), cccDB.curRow["LoginID"], cccDB.curRow["Subject"]);
        notesDB.clearData();

        notesDB.setValue("LoginID",     cccDB.curRow["LoginID"]);
        notesDB.setValue("NoteDate",    cccDB.curRow["NoteDate"]);
        notesDB.setValue("CustomerID",  atol(cccDB.curRow["CustomerID"]));
        notesDB.setValue("NoteType",    cccDB.curRow["NoteType"]);
        notesDB.setValue("Category",    cccDB.curRow["Category"]);
        notesDB.setValue("SubCategory", cccDB.curRow["SubCategory"]);
        notesDB.setValue("Subject",     cccDB.curRow["Subject"]);
        notesDB.setValue("NoteText",    cccDB.curRow["NoteText"]);
        notesDB.ins();
        // Now, remove the entry so we don't process it again.
        cccDB2.dbcmd("delete from TAANotes where InternalID = %ld", atol(cccDB.curRow["InternalID"]));
    }

}

/*
** parseFile   - The only function for it.  It reads SrcFile, and spits
**               out DstFile.
*/

void    parseFile(long CustomerID, const char *LoginID, const char *parentID, const char *Action)
{
    FParser     parser;
    ADB         DB;
    ADBTable    custDB("Customers");
    LoginsDB    loginsDB;
    char        dstFile[2048];
	FILE     *dfp;

    custDB.get(CustomerID);
    loginsDB.get(CustomerID, LoginID);

    DB.query("select LoginType, Description from LoginTypes where InternalID = %ld", loginsDB.getInt("LoginType"));
    DB.getrow();
    parser.set("LoginType",     DB.curRow["LoginType"]);
    parser.set("LoginTypeD",    DB.curRow["Description"]);
    parser.set("Action",        Action);
    parser.set("CustomerID",    custDB.getStr("CustomerID"));
    parser.set("LoginID",       LoginID);
    parser.set("ParentID",      parentID);
    parser.set("CompanyName",   custDB.getStr("FullName"));
    parser.set("ContactName",   loginsDB.getStr("ContactName"));


    // Open our destination file
    sprintf(dstFile, "/var/spool/taamail/%06ld-%s-XXXXXX", CustomerID, LoginID);
    mkstemp(dstFile);
    dfp = fopen(dstFile, "w");
    parser.parseFile(cfgVal("EmailTemplate"), dfp);
    fclose(dfp);
}

/*
** addNote  - Adds a note into the customers account notes stating that
**            we expired the card and sent them mail about it.
*/

void addNote(long CustomerID, const char *LoginID, const char *ExpDate, int isActive)
{
    ADBTable    NDB("Notes");
    char        dateStamp[128];
    char        *noteText = new char[16384];
    QDateTime   theDate = QDateTime::currentDateTime();
    
    sprintf(dateStamp, "%04d-%02d-%02d %02d:%02d:%02d", 
      theDate.date().year(),
      theDate.date().month(),
      theDate.date().day(),
      theDate.time().hour(),
      theDate.time().minute(),
      theDate.time().second()
    );
    
    NDB.setValue("NoteID", (long) 0);
    NDB.setValue("ParentNoteID", (long) 0);
    NDB.setValue("LoginID",    LoginID);
    NDB.setValue("CustomerID", CustomerID);
    NDB.setValue("NoteDate",  dateStamp);
    NDB.setValue("NoteType",   "Accounting");
    NDB.setValue("AddedBy",    "Accounting");
    NDB.setValue("Subject",    "Expired Credit Card");
    
    if (isActive) {
        sprintf(noteText, "Deactivated automatic credit card payment (card expiration date is %s).  Sent email notification to %s", ExpDate, LoginID);
    } else {
        sprintf(noteText, "Deactivated automatic credit card payment (card expiration date is %s).  %s was NOT emailed (inactive account)", ExpDate, LoginID);
    }
    NDB.setValue("NoteText", noteText);
    NDB.ins();

}

