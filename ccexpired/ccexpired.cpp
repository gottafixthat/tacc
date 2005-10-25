/* $Id$
**
** ccexpired  - Scans through the database looking for expired credit cards.
**              When it finds one, it does the following:
**
**                  - Deactivate the card (set Active = 0)
**                  - Find the primary login for the account, 
**                    and send them an email message letting them know
**                    that their card expired.
**                      - It should include the type of Card and 
**                        the date it expired.
**                      - It should include how they can update their
**                        payment information
**                  - Insert a note into their account showing that we 
**                    deactivated the card. 
**
** This should be run every day during nightly TAA maintenance, just
** before we sync the CCC database with wasync.
**
** Configuration file.  The default configuration file for this is located
** /usr/local/etc/ccexpired.cf.  It will need to contain the following
** configuration entries for the program to work correctly:
**
**
**
** # TAAPath points to the path that holds MessageText
** TAAPath       = /usr/local/lib/taa
** # MessageTest is the name of the file that will be mailed to the user
** # after being parsed.  The avilalble variables are:
** #   {CompanyName}, {ContactName}, {LoginID}, {CurrentDate}, {ExpDate},
** #   {DomainName}
** MessageText   = ccexpired.txt
** # The number of days in advance to process cards, the query will be
** # laid out select ... where ExpDate <= CurrentDate.addDays(DaysInAdvance)
** DaysInAdvance = 10
**
** # This is the domain name that we deliver mail from and to.
** DomainName    = nwfirst.com
**
** # This is the path to put the file where taamail will pick it up
** # and mail it to the user.
** MailSpool     = /var/spool/taamail
**
** # The database connection info.
** DBHost        = localhost
** DBName        = bacct
** DBUser        = marc
**
** # A password generated by "ecenv"
** DBPass        = d146cf491f33ebf655e49137056bc5840a49ba03b2
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// Qt Includes
#include <qlist.h>
#include <qdatetm.h>
#include <qregexp.h>
#include <qstrlist.h>
#include <qstring.h>

// Blarg includes
#include <ADB.h>
#include <Cfg.h>
#include <bdes.h>
#include <FParse.h>


#define TACT_KEYFILE "/usr/local/lib/tact.key"
#define CONFIGFILE   "/usr/local/etc/ccexpired.cf"
#define PROGNAME     "ccexpired"


// Function prototypes
int  dbSetup(void);
void findCards(void);
void parseFile(const char *SrcFile, const char *DstFile, long CustomerID, const char *LoginID, const char *ExpDate);
void addNote(long CustomerID, const char *LoginID, const char *ExpDate, int isActive);



main(int, char **)
{
    openlog(PROGNAME, LOG_PID|LOG_NDELAY, LOG_INFO);
    loadCfg(CONFIGFILE);
    if (dbSetup()) {
        findCards();
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
    ADB::setDefaultHost(cfgVal("DBHost"));
    ADB::setDefaultDBase(cfgVal("DBName"));
    ADB::setDefaultUser(cfgVal("DBUser"));
    ADB::setDefaultPass(cfgVal("DBPass"));
    return RetVal;
}


/*
** findCards    - Finds the expired credit cards in the database
**                and processes them.
*/

void findCards(void)
{
    ADB         DB;
    ADB         DB2;
    ADBTable    APDB("AutoPayments");
    ADBTable    CDB("Customers");
    ADBTable    LDB("Logins");
    
    char        *srcFile = new char[strlen(cfgVal("TAAPath")) + strlen(cfgVal("MessageText")) + 10];
    char        *dstFile = new char[strlen(cfgVal("MailSpool")) + 128];
            
    sprintf(srcFile, "%s/%s", cfgVal("TAAPath"), cfgVal("MessageText"));

    QDate   theDate = QDate::currentDate();
    QDate   tmpDate = QDate::currentDate();
    char    dateStr[64];
    
    theDate = tmpDate.addDays(atoi(cfgVal("DaysInAdvance")));
    sprintf(dateStr, "%04d-%02d-%02d", theDate.year(), theDate.month(), theDate.day());

    
    DB.query("select AutoPayments.InternalID, Customers.CustomerID, Logins.LoginID from AutoPayments, Customers, Logins where AutoPayments.ExpDate <= '%s' and AutoPayments.Active = 1 and Logins.LoginID = Customers.PrimaryLogin and Logins.Active = 1 and AutoPayments.CustomerID = Customers.CustomerID", dateStr);
    if (DB.rowCount) while (DB.getrow()) {
        APDB.get(atol(DB.curRow["InternalID"]));
        
        // Get the customer and login information.
        CDB.get(APDB.getLong("CustomerID"));
        DB2.query("select InternalID from Logins where CustomerID = %ld and LoginID = '%s'", CDB.getLong("CustomerID"), (const char *) CDB.getStr("PrimaryLogin"));
        if (DB2.rowCount) {
            DB2.getrow();
            LDB.get(atol(DB2.curRow["InternalID"]));
            
            if (LDB.getInt("Active")) {
                // Send them the mail
                sprintf(dstFile, "%s/ccexpired-%ld-%s-%s", (const char *) cfgVal("MailSpool"), CDB.getLong("CustomerID"), (const char *) LDB.getStr("LoginID"), (const char *) APDB.getStr("ExpDate"));
                tmpDate = APDB.getDate("ExpDate");
                parseFile(srcFile, dstFile, CDB.getLong("CustomerID"), LDB.getStr("LoginID"), (const char *) tmpDate.toString());
                addNote(CDB.getLong("CustomerID"), LDB.getStr("LoginID"), APDB.getStr("ExpDate"), 1);
            } else {
                addNote(CDB.getLong("CustomerID"), LDB.getStr("LoginID"), APDB.getStr("ExpDate"), 0);
            }
        }
        
        APDB.setValue("Active", 0);
        APDB.upd();
    }
    syslog(LOG_INFO, "Expired active %ld credit cards from the automatic payment database", DB.rowCount);
    DB.query("update AutoPayments set Active = 0 where ExpDate <= '%s' and Active = 1", dateStr);
}

/*
** parseFile   - The only function for it.  It reads SrcFile, and spits
**               out DstFile.
*/

void    parseFile(const char *SrcFile, const char *DstFile, long CustomerID, const char *LoginID, const char *ExpDate)
{
    FParser     parser;
    ADB         DB;
    ADBTable    custDB("Customers");
	FILE     *dfp;

    custDB.get(CustomerID);

    parser.set("CustomerID",    custDB.getStr("CustomerID"));
    parser.set("FullName",      custDB.getStr("FullName"));
    parser.set("ContactName",   custDB.getStr("ContactName"));
    parser.set("PrimaryLogin",  custDB.getStr("PrimaryLogin"));
    parser.set("LoginID",       LoginID);
    parser.set("DomainName",    "blarg.net");
    parser.set("ExpDate",       ExpDate);


    // Open our destination file
    dfp = fopen(DstFile, "w");
    parser.parseFile(SrcFile, dfp);
    fclose(dfp);
/* 
    
	char 	 tmpstr[1024];
	QStrList tmplist(TRUE);
	QCString qst;
	QCString tmpqstr2;
	long     lineCount = 0;
	char     contactName[256];
	
	char     theDate[64];
	QDate    theQDate;
	
	theQDate    = QDate::currentDate();
	strcpy(theDate, theQDate.toString());
	
	
	ADB             DB;
	ADBTable        CDB("Customers");
	ADBTable        LTDB;
	
	CDB.get(CustomerID);
	
	// fprintf(stderr, "parseFile: Reading File '%s'\n", SrcFile);
	
	
	// Check to see if we have a contact name.
	if (strlen((const char *)CDB.getStr("ContactName"))) {
	    strcpy(contactName, CDB.getStr("ContactName"));
	} else {
	    strcpy(contactName, CDB.getStr("FullName"));
	}
	

    // Read in the entire source file.
	fp = fopen(SrcFile, "r");
	if (fp != NULL) {
	    dfp = fopen(DstFile, "w");
		while (fgets(tmpstr, sizeof(tmpstr), fp) != NULL) {
		    lineCount++;
		    // Here is where we'll want to do the actual parsing...
		    // With qstrings, its quite easy, we just use the replace() 
		    // function
		    qst.setStr(tmpstr);
		    
		    // Go through all of our tokens and replace them.
		    qst.replace(QRegExp("{CompanyName}"),   CDB.getStr("FullName"));
		    qst.replace(QRegExp("{ContactName}"),   contactName);
		    qst.replace(QRegExp("{LoginID}"),       LoginID);
		    qst.replace(QRegExp("{CurrentDate}"),   theDate);
		    qst.replace(QRegExp("{ExpDate}"),       ExpDate);
            qst.replace(QRegExp("{DomainName}"),    (const char *) cfgVal("DomainName"));

            // Now, put the qstring back into a char buffer and write it.
            strcpy(tmpstr, qst);		    
		    fprintf(dfp, "%s", tmpstr);
		}
		fclose(fp);
		fclose(dfp);
	}
*/
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

