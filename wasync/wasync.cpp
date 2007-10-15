/*
** wasync - Syncs up the remote WebAcct database with the local TAA
**          database.
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// Qt Includes
#include <qlist.h>
#include <qintdict.h>
#include <qdatetm.h>

// Blarg includes
#include <ADB.h>
#include <bdes.h>
#include <Cfg.h>


// Some constants.  We may want to change these to use config files for
// the release version.
#define STAT_NOCHANGE   0
#define STAT_DELETE     1
#define STAT_INSERT     2
#define STAT_UPDATE     3

#define REG_DAYS       90

#define TACT_KEYFILE "/usr/local/lib/tact.key"
#define CONFIGFILE   "/usr/local/etc/wasync.cf"

char REMOTEHOST  [1024];
char REMOTEDBASE [1024];
char LOCALHOST   [1024];
char LOCALDBASE  [1024];

char REMOTEUSER  [1024];
char REMOTEPASS  [1024];
char LOCALUSER   [1024];
char LOCALPASS   [1024];

// Our data structures

typedef struct RemoteCustomer {
    long    CustomerID;
    char    PrimaryLogin[20];
    float   CurrentBalance;
    int     AutoPayment;
    char    AutoExpires[12];
    char    AutoLastFour[12];
    int     PendingPayment;
    float   PendingAmount;
    char    PendingDate[12];
    char    GraceDate[12];
    float   PrevBalance;
    char    PrevBalanceDate[12];
    char    LastModified[24];
    int     MailboxesAllowed;
    
    // Special, not part of the record itself.
    int     Status;
};

typedef struct RemoteLogin {
    long    InternalID;
    long    CustomerID;
    char    LoginID[20];
    long    LoginType;
    int     Active;
    
    // Special, not part of the record itself.
    int     Status;
};

typedef struct RemoteDomain {
    long    InternalID;
    long    CustomerID;
    char    LoginID[20];
    char    DomainName[32];
    int     DomainType;
    int     HasSSL;         // Not used yet...
    
    // Special, not part of the record itself.
    int     Status;
};

typedef struct AutoPayments {
    long    InternalID;
    long    CustomerID;
    char    TransType;
    char    ExpDate[12];
    char    LastFour[12];
};

typedef struct PendingPayments {
    long    CustomerID;
    int     CardType;
    float   Amount;
    char    TransDate[12];
};

typedef struct RemoteRegister {
    long    InternalID;
    long    CustomerID;
    char    LoginID[20];
    float   Quantity;
    float   Price;
    float   Amount;
    long    StatementNo;
    char    TransDate[12];
    char    BilledDate[12];
    char    DueDate[12];
    char    StartDate[12];
    char    EndDate[12];
    char    *Memo;
    
    int     Status;
};

typedef struct RemoteLoginType {
    long    InternalID;
    char    LoginType[40];
    char    Description[1024];
    int     DiskSpace;
    int     Active;
    char    LoginClass[40];
    char    Flags[4096];

    int     Status;
};

typedef struct ProgramKeyStruct {
    char    CompanyName[32];
    char    CompanyAddr1[32];
    char    CompanyAddr2[32];
    char    CompanyCity[32];
    char    CompanyState[32];
    char    CompanyPostal[32];
    char    CompanyCountry[32];
    char    CompanyPhone[32];
    char    CompanyFax[32];
    char    PurchaseDate[32];
    char    ExpDate[32];
    long    MaxUsers;
    char    CipherKey[32];
};

static struct ProgramKeyStruct ProgramKey;

QDateTime                StartTime;
QDateTime                EndTime;


QList<RemoteCustomer>    RCustList;
QIntDict<RemoteCustomer> RCustListDict;
QList<RemoteCustomer>    LCustList;
QIntDict<RemoteCustomer> LCustListDict;
QList<RemoteLogin>       RLoginList;
QList<RemoteLogin>       LLoginList;
QList<RemoteLoginType>   RLoginTypeList;
QList<RemoteLoginType>   LLoginTypeList;
QList<RemoteDomain>      RDomainList;
QList<RemoteDomain>      LDomainList;
QList<AutoPayments>      AutoPayList;
QList<PendingPayments>   PendingList;
QList<RemoteRegister>    LocalRegList;
QIntDict<RemoteRegister> LocalRegListDict;
QList<RemoteRegister>    RemoteRegList;
QIntDict<RemoteRegister> RemoteRegListDict;


// Function prototypes
void loadConfig(void);
int loginCount(long CustID);
AutoPayments *getAutoPayment(long CustID);
PendingPayments *getPendingPayment(long CustID);
int loadRemoteLogins(void);
int loadLocalLogins(void);
int loadRemoteLoginTypes(void);
int loadLocalLoginTypes(void);
int loadRemoteDomains(void);
int loadLocalDomains(void);
int loadRemoteCustomers(void);
int loadLocalCustomers(void);
int loadAutoPayments(void);
int loadPendingPayments(void);
int loadLocalRegister(void);
int loadRemoteRegister(void);
int checkRemoteCustomers(void);
int checkLocalCustomers(void);
int checkRemoteLogins(void);
int checkLocalLogins(void);
int checkRemoteLoginTypes(void);
int checkLocalLoginTypes(void);
int checkRemoteDomains(void);
int checkLocalDomains(void);
int checkRemoteRegister(void);
int checkLocalRegister(void);
int updateRemoteCustomers(void);
int updateRemoteLogins(void);
int updateRemoteLoginTypes(void);
int updateRemoteDomains(void);
int updateRemoteRegister(void);
int main(int, char **);

void loadKey(void);

int main(int, char **)
{
    StartTime   = QDateTime::currentDateTime();
    
    loadConfig();
    loadKey();
    if (loadAutoPayments() < 0) exit(-1);
    if (loadPendingPayments() < 0) exit(-1);
    if (loadRemoteLogins() < 0) exit(-1);
    if (loadLocalLogins() < 0) exit(-1);
    if (loadRemoteLoginTypes() < 0) exit(-1);
    if (loadLocalLoginTypes() < 0) exit(-1);
    if (loadRemoteDomains() < 0) exit(-1);
    if (loadLocalDomains() < 0) exit(-1);
    if (loadRemoteCustomers() < 0) exit(-1);
    if (loadLocalCustomers() < 0) exit(-1);
    if (loadLocalRegister() < 0) exit(-1);
    if (loadRemoteRegister() < 0) exit(-1);
    if (checkRemoteCustomers() < 0) exit(-1);
    if (checkLocalCustomers() < 0) exit(-1);
    if (checkRemoteLogins() < 0) exit(-1);
    if (checkLocalLogins() < 0) exit(-1);
    if (checkRemoteLoginTypes() < 0) exit(-1);
    if (checkLocalLoginTypes() < 0) exit(-1);
    if (checkRemoteDomains() < 0) exit(-1);
    if (checkLocalDomains() < 0) exit(-1);
    if (checkRemoteRegister() < 0) exit(-1);
    if (checkLocalRegister() < 0) exit(-1);
    if (updateRemoteCustomers() < 0) exit(-1);
    if (updateRemoteLogins() < 0) exit(-1);
    if (updateRemoteLoginTypes() < 0) exit(-1);
    if (updateRemoteDomains() < 0) exit(-1);
    if (updateRemoteRegister() < 0) exit(-1);
    
    EndTime = QDateTime::currentDateTime();
    syslog(LOG_INFO, "Finished syncing remote database.  Elapsed time was %d seconds.", StartTime.secsTo(EndTime));
}

/*
** loginCount  - Given a customer ID, it returns the number of active
**               logins that are associated with this customer ID.
*/

int loginCount(long CustID)
{
    int RetVal = 0;
    RemoteLogin *curItem;
    
    for (curItem = LLoginList.first(); curItem != 0; curItem = LLoginList.next()) {
        if (curItem->CustomerID == CustID) RetVal++;
    }
    
    return RetVal;
}

/*
** getAutoPayment  - Given a customer ID, it returns an auto payment record
**                   for the customer or NULL if none was found.
*/

AutoPayments *getAutoPayment(long CustID)
{
    AutoPayments *RetVal = NULL;
    AutoPayments *curItem;
    
    for (curItem = AutoPayList.first(); curItem != 0; curItem = AutoPayList.next()) {
        if (curItem->CustomerID == CustID) RetVal = curItem;
    }
    
    return RetVal;
}

/*
** getPendingPayment  - Given a customer ID, it returns a pending payment
**                      record for the customer or NULL if none was found.
*/

PendingPayments *getPendingPayment(long CustID)
{
    PendingPayments *RetVal = NULL;
    PendingPayments *curItem;
    
    for (curItem = PendingList.first(); curItem != 0; curItem = PendingList.next()) {
        if (curItem->CustomerID == CustID) RetVal = curItem;
    }
    
    return RetVal;
}

/*
** loadLocalLogins - Connects to the local database and loads the
**                   contents of the login records into memory.
*/

int loadLocalLogins(void)
{
    int         RetVal = 0;
    RemoteLogin *curItem;
    
    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select * from Logins where Active <> 0 order by CustomerID");

    if (ldb.rowCount) while (ldb.getrow()) {
        curItem = new RemoteLogin;
        curItem->InternalID     = atol(ldb.curRow["InternalID"]);
        curItem->CustomerID     = atol(ldb.curRow["CustomerID"]);
        curItem->LoginType      = atol(ldb.curRow["LoginType"]);
        strcpy(curItem->LoginID, ldb.curRow["LoginID"]);
        curItem->Active         = atoi(ldb.curRow["Active"]);

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        LLoginList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", LLoginList.count(), LOCALDBASE, "Logins", LOCALHOST);
    
    return RetVal;
}


/*
** loadRemoteLogins - Connects to the remote database and loads the
**                    contents of the login records into memory.
*/

int loadRemoteLogins(void)
{
    int         RetVal = 0;
    RemoteLogin *curItem;
    
    ADB         rdb(REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    //ADBTable    rldb("Logins", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);

    rdb.query("select * from Logins order by CustomerID");

    if (rdb.rowCount) while (rdb.getrow()) {
        // rldb.get(atol(rdb.curRow["InternalID"]));

        curItem = new RemoteLogin;
        curItem->InternalID     = atol(rdb.curRow["InternalID"]); //rldb.getLong("InternalID");
        curItem->CustomerID     = atol(rdb.curRow["CustomerID"]); //rldb.getLong("CustomerID");
        curItem->LoginType      = atol(rdb.curRow["LoginType"]); //rldb.getLong("LoginType");
        //strcpy(curItem->LoginID, rldb.getStr("LoginID"));
        strcpy(curItem->LoginID, rdb.curRow["LoginID"]);
        curItem->Active         = atoi(rdb.curRow["Active"]); //rldb.getInt("Active");

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        RLoginList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", RLoginList.count(), REMOTEDBASE, "Logins", REMOTEHOST);
    
    return RetVal;
}

/*
** loadRemoteLoginTypes - Connects to the remote database and loads the
**                        contents of the login types table into memory.
*/

int loadRemoteLoginTypes(void)
{
    int             RetVal = 0;
    RemoteLoginType *curItem;
    
    ADB         rdb(REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);

    rdb.query("select * from LoginTypes order by InternalID");

    if (rdb.rowCount) while (rdb.getrow()) {
        curItem = new RemoteLoginType;
        curItem->InternalID     = atol(rdb.curRow["InternalID"]);
        curItem->DiskSpace      = atoi(rdb.curRow["DiskSpace"]);
        curItem->Active         = atoi(rdb.curRow["Active"]);
        strcpy(curItem->LoginType,   rdb.curRow["LoginType"]);
        strcpy(curItem->Description, rdb.curRow["Description"]);
        strcpy(curItem->LoginClass,  rdb.curRow["LoginClass"]);
        strcpy(curItem->Flags,       rdb.curRow["LoginFlags"]);

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        RLoginTypeList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", RLoginTypeList.count(), REMOTEDBASE, "LoginTypes", REMOTEHOST);
    
    return RetVal;
}

/*
** loadLocalLoginTypes - Connects to the remote database and loads the
**                       contents of the login types table into memory.
*/

int loadLocalLoginTypes(void)
{
    int             RetVal = 0;
    RemoteLoginType *curItem;
    char            flagStr[4096];
    
    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    ADB         fdb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select * from LoginTypes order by InternalID");

    if (ldb.rowCount) while (ldb.getrow()) {
        //fprintf(stderr, "Examining login type '%ld'\n", atol(ldb.curRow["InternalID"]));
        curItem = new RemoteLoginType;
        curItem->InternalID     = atol(ldb.curRow["InternalID"]);
        curItem->DiskSpace      = atoi(ldb.curRow["DiskSpace"]);
        curItem->Active         = atoi(ldb.curRow["Active"]);
        strcpy(curItem->LoginType,   ldb.curRow["LoginType"]);
        strcpy(curItem->Description, ldb.curRow["Description"]);
        strcpy(curItem->LoginClass,  ldb.curRow["LoginClass"]);
        strcpy(curItem->Flags,       "");
        // Get the flags for them now.
        strcpy(flagStr, "");
        fdb.query("select LoginTypeFlags.Tag, LoginTypeFlags.Value, LoginFlags.IsBool from LoginTypeFlags, LoginFlags where LoginTypeFlags.Tag = LoginFlags.LoginFlag and LoginTypeFlags.LoginTypeID = %ld order by LoginTypeFlags.Tag", curItem->InternalID);
        if (fdb.rowCount) {
            while (fdb.getrow()) {
                if (strlen(flagStr)) strcat(flagStr, ",");
                strcat(flagStr, fdb.curRow["Tag"]);
                if (!atoi(fdb.curRow["IsBool"])) {
                    strcat(flagStr, "=");
                    strcat(flagStr, fdb.curRow["Value"]);
                }
            }
        }
        strcpy(curItem->Flags, flagStr);

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        LLoginTypeList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", LLoginTypeList.count(), LOCALDBASE, "LoginTypes", LOCALHOST);
    
    return RetVal;
}


/*
** loadLocalDomains - Connects to the local database and loads the
**                    contents of the domain records into memory.
*/

int loadLocalDomains(void)
{
    int          RetVal = 0;
    RemoteDomain *curItem;
    
    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    ADBTable    lddb("Domains", LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select InternalID, CustomerID, DomainType, HasSSL, LoginID, DomainName from Domains where Active <> 0 order by CustomerID");

    if (ldb.rowCount) while (ldb.getrow()) {
        // lddb.get(atol(ldb.curRow["InternalID"]));

        curItem = new RemoteDomain;
        curItem->InternalID       = atol(ldb.curRow["InternalID"]); //lddb.getLong("InternalID");
        curItem->CustomerID       = atol(ldb.curRow["CustomerID"]); //lddb.getLong("CustomerID");
        curItem->DomainType       = atoi(ldb.curRow["DomainType"]); //lddb.getInt("DomainType");
        curItem->HasSSL           = atoi(ldb.curRow["HasSSL"]); // lddb.getInt("HasSSL");
        //strcpy(curItem->LoginID,    lddb.getStr("LoginID"));
        //strcpy(curItem->DomainName, lddb.getStr("DomainName"));
        strcpy(curItem->LoginID,    ldb.curRow["LoginID"]);
        strcpy(curItem->DomainName, ldb.curRow["DomainName"]);

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        LDomainList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", LDomainList.count(), LOCALDBASE, "Domains", LOCALHOST);
    
    return RetVal;
}


/*
** loadRemoteDomains - Connects to the remote database and loads the
**                     contents of the domain records into memory.
*/

int loadRemoteDomains(void)
{
    int         RetVal = 0;
    RemoteDomain *curItem;
    
    ADB         rdb(REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    ADBTable    rddb("Domains", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);

    rdb.query("select InternalID, CustomerID, DomainType, HasSSL, LoginID, DomainName from Domains order by CustomerID");

    if (rdb.rowCount) while (rdb.getrow()) {
        // rddb.get(atol(rdb.curRow["InternalID"]));

        curItem = new RemoteDomain;
        curItem->InternalID       = atol(rdb.curRow["InternalID"]); //rddb.getLong("InternalID");
        curItem->CustomerID       = atol(rdb.curRow["CustomerID"]); //rddb.getLong("CustomerID");
        curItem->DomainType       = atoi(rdb.curRow["DomainType"]); //rddb.getInt("DomainType");
        curItem->HasSSL           = atoi(rdb.curRow["HasSSL"]); // rddb.getInt("HasSSL");
        //strcpy(curItem->LoginID,    rddb.getStr("LoginID"));
        //strcpy(curItem->DomainName, rddb.getStr("DomainName"));
        strcpy(curItem->LoginID,    rdb.curRow["LoginID"]);
        strcpy(curItem->DomainName, rdb.curRow["DomainName"]);

        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        RDomainList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", RDomainList.count(), REMOTEDBASE, "Domains", REMOTEHOST);
    
    return RetVal;
}


/*
** loadRemoteCustomers - Connects to the remote database and loads the
**                       contents of the customer records into memory.
*/

int loadRemoteCustomers(void)
{
    int             RetVal = 0;
    RemoteCustomer  *curItem;
    
    ADB         rdb(REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    // ADBTable    rcdb("Customers", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);

    rdb.query("select * from Customers order by CustomerID");

    if (rdb.rowCount) while (rdb.getrow()) {
        // rcdb.get(atol(rdb.curRow["CustomerID"]));
        curItem = new RemoteCustomer;
        curItem->CustomerID     = atol(rdb.curRow["CustomerID"]);
        curItem->CurrentBalance = atof(rdb.curRow["CurrentBalance"]);
        curItem->AutoPayment    = atoi(rdb.curRow["AutoPayment"]);
        curItem->PendingPayment = atoi(rdb.curRow["PendingPayment"]);
        curItem->PendingAmount  = atof(rdb.curRow["PendingAmount"]);

        strcpy(curItem->PrimaryLogin, rdb.curRow["PrimaryLogin"]);
        strcpy(curItem->AutoExpires,  rdb.curRow["AutoExpires"]);
        strcpy(curItem->AutoLastFour, rdb.curRow["AutoLastFour"]);
        strcpy(curItem->PendingDate,  rdb.curRow["PendingDate"]);
        strcpy(curItem->GraceDate,    rdb.curRow["GraceDate"]);
        strcpy(curItem->LastModified, rdb.curRow["LastModified"]);

        curItem->MailboxesAllowed = atoi(rdb.curRow["MailboxesAllowed"]);
        
        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
        
        RCustList.append(curItem);
        RCustListDict.insert(curItem->CustomerID, curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", RCustList.count(), REMOTEDBASE, "Customers", REMOTEHOST);
    
    return RetVal;
}

/*
** loadLocalCustomers - Connects to the local database and loads the
**                      contents of the customer records into memory.
*/

int loadLocalCustomers(void)
{
    int             RetVal = 0;
    RemoteCustomer  *curItem;
    AutoPayments    *aPay = NULL;
    PendingPayments *ccTrans = NULL;
    
    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    // ADBTable    lcdb("Customers", LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select * from Customers order by CustomerID");

    if (ldb.rowCount) while (ldb.getrow()) {
        // lcdb.get(atol(ldb.curRow["CustomerID"]));
        
        // Make sure this customer has active logins...
        if (loginCount(atol(ldb.curRow["CustomerID"]))) {
	        curItem = new RemoteCustomer;
	        curItem->CustomerID     = atol(ldb.curRow["CustomerID"]);
	        curItem->CurrentBalance = atof(ldb.curRow["CurrentBalance"]);
	        
	        aPay = getAutoPayment(curItem->CustomerID);
	        if (aPay != NULL) {
	            curItem->AutoPayment    = aPay->TransType + 1;
	            strcpy(curItem->AutoExpires, aPay->ExpDate);
	            strcpy(curItem->AutoLastFour, aPay->LastFour);
	        } else {
    	        curItem->AutoPayment    = 0;
    	        strcpy(curItem->AutoExpires,  "0000-00-00");
    	        strcpy(curItem->AutoLastFour,  "");
	        }
	        
	        ccTrans = getPendingPayment(curItem->CustomerID);
	        if (ccTrans != NULL) {
	            curItem->PendingPayment = 1;
	            curItem->PendingAmount = ccTrans->Amount;
	            strcpy(curItem->PendingDate, ccTrans->TransDate);
	        } else {
	            curItem->PendingPayment = 0;
	            curItem->PendingAmount  = 0.00;
	            strcpy(curItem->PendingDate, "0000-00-00");
	        }

            curItem->PrevBalance = 0.00;
	        
	        strcpy(curItem->PrimaryLogin, ldb.curRow["PrimaryLogin"]);
	        strcpy(curItem->GraceDate,    ldb.curRow["GraceDate"]);
	        strcpy(curItem->LastModified, ldb.curRow["LastModified"]);

            curItem->MailboxesAllowed = atoi(ldb.curRow["MailboxesAllowed"]);
	        
	        curItem->Status = STAT_NOCHANGE;    // No change/update/delete/etc.
	        
	        LCustList.append(curItem);
	        LCustListDict.insert(curItem->CustomerID, curItem);
        }
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", LCustList.count(), LOCALDBASE, "Customers", LOCALHOST);
    
    return RetVal;
}

/*
** loadAutoPayments - Connects to the local database and loads the
**                    contents of the AutoPayments table into memory.
*/

int loadAutoPayments(void)
{
    int             RetVal = 0;
    AutoPayments    *curItem;
    QString         tmpQSt;
    

    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    ADBTable    lapdb("AutoPayments", LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    lapdb.setEncryptedColumn("CardType");
    lapdb.setEncryptedColumn("AcctNo");

    ldb.query("select InternalID from AutoPayments where Active <> 0");

    if (ldb.rowCount) while (ldb.getrow()) {
        lapdb.get(atol(ldb.curRow["InternalID"]));
        
        // Before this will work, we'll need to load the TAA encryption
        // key and use the legacy decryption routines on the CardType.
        // Manual decryption would probably be best for now...
        
        curItem = new AutoPayments;
        curItem->InternalID     = lapdb.getLong("InternalID");
        curItem->CustomerID     = lapdb.getLong("CustomerID");
        curItem->TransType      = atoi(lapdb.getStr("CardType"));
        strcpy(curItem->ExpDate, lapdb.getStr("ExpDate"));
        tmpQSt = lapdb.getStr("AcctNo");
        strcpy(curItem->LastFour, (const char *) tmpQSt.right(4));
	        
        AutoPayList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", AutoPayList.count(), LOCALDBASE, "AutoPayments", LOCALHOST);
    
    return RetVal;
}

/*
** loadPendingPayments - Connects to the local database and loads the
**                       contents of the CCTrans table into memory.
*/

int loadPendingPayments(void)
{
    int             RetVal = 0;
    PendingPayments *curItem;
    
    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    ADBTable    lccdb("CCTrans", LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select InternalID from CCTrans where BPSExported = 0");

    if (ldb.rowCount) while (ldb.getrow()) {
        lccdb.get(atol(ldb.curRow["InternalID"]));
        
        curItem = new PendingPayments;
        curItem->CustomerID     = lccdb.getLong("CustomerID");
        curItem->CardType       = atoi(lccdb.getStr("CardType"));
        curItem->Amount         = lccdb.getFloat("Amount");
        strcpy(curItem->TransDate, lccdb.getStr("TransDate"));
	        
        PendingList.append(curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", PendingList.count(), LOCALDBASE, "CCTrans", LOCALHOST);
    
    return RetVal;
}


/*
** loadLocalRegister - Connects to the local database and loads the
**                     contents of customer registers into memory.
**
**                     This updates the PrevBalance and PrevBalanceDate
**                     of the customers, so it should be called _after_
**                     the local customers have been loaded.
*/

int loadLocalRegister(void)
{
    int             RetVal = 0;
    RemoteRegister  *curItem;
    QDate           oldestDate;
    QDate           transDate;
    RemoteCustomer  *curCust = NULL;
    
    
    transDate   = QDate::currentDate();
    oldestDate  = transDate.addDays(-REG_DAYS);
    // LocalRegListDict.resize(25000);

    ADB         ldb(LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);
    //ADBTable    lardb("AcctsRecv", LOCALDBASE, LOCALUSER, LOCALPASS, LOCALHOST);

    ldb.query("select AcctsRecv.InternalID, Customers.CustomerID, AcctsRecv.Quantity, AcctsRecv.Price, AcctsRecv.Amount, AcctsRecv.StatementNo, AcctsRecv.LoginID, AcctsRecv.TransDate, AcctsRecv.BilledDate, AcctsRecv.DueDate, AcctsRecv.StartDate, AcctsRecv.EndDate, AcctsRecv.Memo from AcctsRecv, Customers where Customers.Active > 0 and AcctsRecv.CustomerID = Customers.CustomerID order by Customers.CustomerID");

    if (ldb.rowCount) while (ldb.getrow()) {
        // lardb.get(atol(ldb.curRow["InternalID"]));

        if (ldb.curRow.col("TransDate")->toQDate() < oldestDate) {
            // Too old to be included in their register.  
            // Add this transaction to the customer's previous balance
            // instead.
            curCust = LCustListDict[atol(ldb.curRow["CustomerID"])];
            if (curCust != 0) {
                sprintf(curCust->PrevBalanceDate, "%04d-%02d-%02d", 
                  oldestDate.year(),
                  oldestDate.month(),
                  oldestDate.day()
                );
                // Add this amount to the customer's previous balance
                curCust->PrevBalance += atof(ldb.curRow["Amount"]);
                if (curCust->Status == STAT_NOCHANGE) {
                  curCust->Status = STAT_UPDATE;
                }
            }
            
        } else {
            curItem = new RemoteRegister;
            
            curItem->InternalID     = atol(ldb.curRow["InternalID"]); 
            curItem->CustomerID     = atol(ldb.curRow["CustomerID"]);
            curItem->Quantity       = atof(ldb.curRow["Quantity"]);
            curItem->Price          = atof(ldb.curRow["Price"]);
            curItem->Amount         = atof(ldb.curRow["Amount"]);
            curItem->StatementNo    = atol(ldb.curRow["StatementNo"]);
            
            strcpy(curItem->LoginID,    ldb.curRow["LoginID"]);
            strcpy(curItem->TransDate,  ldb.curRow["TransDate"]);
            strcpy(curItem->BilledDate, ldb.curRow["BilledDate"]);
            strcpy(curItem->DueDate,    ldb.curRow["DueDate"]);
            strcpy(curItem->StartDate,  ldb.curRow["StartDate"]);
            strcpy(curItem->EndDate,    ldb.curRow["EndDate"]);
            
            curItem->Memo = new char[strlen(ldb.curRow["Memo"]) + 2];
            strcpy(curItem->Memo, ldb.curRow["Memo"]);
            
            curItem->Status = STAT_NOCHANGE;
            
            LocalRegList.append(curItem);
            
            LocalRegListDict.insert(curItem->InternalID, curItem);
        }
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", LocalRegList.count(), LOCALDBASE, "AcctsRecv", LOCALHOST);
    
    return RetVal;
}


/*
** loadRemoteRegister - Connects to the remote database and loads the
**                      contents of customer registers into memory.
**
*/

int loadRemoteRegister(void)
{
    int             RetVal = 0;
    RemoteRegister  *curItem;
    
    
    ADB         rdb(REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    // ADBTable    rrdb("Register", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);

    rdb.query("select * from Register order by InternalID");

    // RemoteRegListDict.resize(25000);

    if (rdb.rowCount) while (rdb.getrow()) {
        // rrdb.get(atol(rdb.curRow["InternalID"]));

        curItem = new RemoteRegister;
        
        curItem->InternalID     = atol(rdb.curRow["InternalID"]);
        curItem->CustomerID     = atol(rdb.curRow["CustomerID"]);
        curItem->Quantity       = atof(rdb.curRow["Quantity"]);
        curItem->Price          = atof(rdb.curRow["Price"]);
        curItem->Amount         = atof(rdb.curRow["Amount"]);
        curItem->StatementNo    = atol(rdb.curRow["StatementNo"]);
        
        strcpy(curItem->LoginID,    rdb.curRow["LoginID"]);
        strcpy(curItem->TransDate,  rdb.curRow["TransDate"]);
        strcpy(curItem->BilledDate, rdb.curRow["BilledDate"]);
        strcpy(curItem->DueDate,    rdb.curRow["DueDate"]);
        strcpy(curItem->StartDate,  rdb.curRow["StartDate"]);
        strcpy(curItem->EndDate,    rdb.curRow["EndDate"]);
        
        curItem->Memo = new char[strlen(rdb.curRow["Memo"]) + 2];
        strcpy(curItem->Memo, rdb.curRow["Memo"]);
        
        // We default to delete, and then update the no-change items later.
        curItem->Status = STAT_NOCHANGE;
        
        RemoteRegList.append(curItem);
        
        RemoteRegListDict.insert(curItem->InternalID, curItem);
    }
    
    syslog(LOG_INFO, "Loaded %d records from %s:%s on %s\n", RemoteRegList.count(), REMOTEDBASE, "Register", REMOTEHOST);
    
    return RetVal;
}






/*
** checkRemoteCustomers - The first pass in comparing.  Steps through the
**                        remote data and looks for any updates or deletes.
*/

int checkRemoteCustomers(void)
{
    int             RetVal = 0;
    RemoteCustomer  *curItem;
    RemoteCustomer  *curLocal;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = RCustList.first(); curItem != 0; curItem = RCustList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        curLocal = LCustListDict[curItem->CustomerID];
        if (curLocal != 0) {
        // for (curLocal = LCustList.first(); curLocal != 0; curLocal = LCustList.next()) {
            if (curItem->CustomerID == curLocal->CustomerID) {
                foundIt = 1;
                // Found the item.  Check for any updates.
                
                // Check the primary login ID.
                if (strcmp(curItem->PrimaryLogin, curLocal->PrimaryLogin)) {
                    strcpy(curItem->PrimaryLogin, curLocal->PrimaryLogin);
                    curItem->Status = STAT_UPDATE;
                }

                // Check how many mailboxes
                if (curItem->MailboxesAllowed != curLocal->MailboxesAllowed) {
                    curItem->MailboxesAllowed = curLocal->MailboxesAllowed;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the current balance
                if (curItem->CurrentBalance != curLocal->CurrentBalance) {
                    curItem->CurrentBalance = curLocal->CurrentBalance;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the previous balance
                if (curItem->PrevBalance != curLocal->PrevBalance) {
                    curItem->PrevBalance = curLocal->PrevBalance;
                    curItem->Status = STAT_UPDATE;
                }

                // Check AutoPayment status
                if (curItem->AutoPayment != curLocal->AutoPayment) {
                    curItem->AutoPayment = curLocal->AutoPayment;
                    curItem->Status = STAT_UPDATE;
                }

                // When does the automatic payment expire?
                if (strcmp(curItem->AutoExpires, curLocal->AutoExpires)) {
                    strcpy(curItem->AutoExpires, curLocal->AutoExpires);
                    curItem->Status = STAT_UPDATE;
                }

                if (strcmp(curItem->AutoLastFour, curLocal->AutoLastFour)) {
                    strcpy(curItem->AutoLastFour, curLocal->AutoLastFour);
                    curItem->Status = STAT_UPDATE;
                }

                // Check PendingPayment status
                if (curItem->PendingPayment != curLocal->PendingPayment) {
                    curItem->PendingPayment = curLocal->PendingPayment;
                    curItem->Status = STAT_UPDATE;
                }

                // Check PendingAmount status
                if (curItem->PendingAmount != curLocal->PendingAmount) {
                    curItem->PendingAmount = curLocal->PendingAmount;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the date of the pending amount
                if (strcmp(curItem->PendingDate, curLocal->PendingDate)) {
                    strcpy(curItem->PendingDate, curLocal->PendingDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the grace date on the account
                if (strcmp(curItem->GraceDate, curLocal->GraceDate)) {
                    strcpy(curItem->GraceDate, curLocal->GraceDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the previous balance date on the account
                if (strcmp(curItem->PrevBalanceDate, curLocal->PrevBalanceDate)) {
                    strcpy(curItem->PrevBalanceDate, curLocal->PrevBalanceDate);
                    curItem->Status = STAT_UPDATE;
                }
            }
        }
        // If we didn't find the record, mark it for deletion.
        if (!foundIt) curItem->Status = STAT_DELETE;
    }

    return RetVal;    
}


/*
** checkLocalCustomers - The second pass in comparing.  Steps through the
**                       local data and looks for any inserts.
*/

int checkLocalCustomers(void)
{
    int             RetVal = 0;
    RemoteCustomer  *curItem;
    RemoteCustomer  *curRemote;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = LCustList.first(); curItem != 0; curItem = LCustList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        curRemote = RCustListDict[curItem->CustomerID];
        if (curRemote != 0) {
        // for (curRemote = RCustList.first(); curRemote != 0; curRemote = RCustList.next()) {
            if (curItem->CustomerID == curRemote->CustomerID) {
                foundIt = 1;
            }
        }
        // If we didn't find the record, add it in.
        if (!foundIt) {
	        curRemote = new RemoteCustomer;
	        curRemote->CustomerID       = curItem->CustomerID;
	        curRemote->CurrentBalance   = curItem->CurrentBalance;
	        curRemote->AutoPayment      = curItem->AutoPayment;
	        curRemote->PendingPayment   = curItem->PendingPayment;
	        curRemote->PendingAmount    = curItem->PendingAmount;
	        curRemote->PrevBalance      = curItem->PrevBalance;
            curRemote->MailboxesAllowed = curItem->MailboxesAllowed;

	        strcpy(curRemote->PrimaryLogin, curItem->PrimaryLogin);
	        strcpy(curRemote->AutoExpires,  curItem->AutoExpires);
	        strcpy(curRemote->AutoLastFour, curItem->AutoLastFour);
	        strcpy(curRemote->PendingDate,  curItem->PendingDate);
	        strcpy(curRemote->GraceDate,    curItem->GraceDate);
	        strcpy(curRemote->LastModified, curItem->LastModified);
	        
	        curRemote->Status = STAT_INSERT;    // No change/update/delete/etc.
	        
	        RCustList.append(curRemote);
        }
    }

    return RetVal;    
}


/*
** checkRemoteLogins - The first pass in comparing.  Steps through the
**                     remote data and looks for any updates or deletes.
*/

int checkRemoteLogins(void)
{
    int         RetVal = 0;
    RemoteLogin *curItem;
    RemoteLogin *curLocal;
    int         foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = RLoginList.first(); curItem != 0; curItem = RLoginList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curLocal = LLoginList.first(); curLocal != 0; curLocal = LLoginList.next()) {
            if (curItem->InternalID == curLocal->InternalID) {
                foundIt = 1;
                // Found the item.  Check for any updates.
                
                // Check the Customer ID
                if (curItem->CustomerID != curLocal->CustomerID) {
                    curItem->CustomerID = curLocal->CustomerID;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the login ID
                if (strcmp(curItem->LoginID, curLocal->LoginID)) {
                    strcpy(curItem->LoginID, curLocal->LoginID);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the Login type
                if (curItem->LoginType != curLocal->LoginType) {
                    curItem->LoginType = curLocal->LoginType;
                    curItem->Status = STAT_UPDATE;
                }

                if (curItem->Active != curLocal->Active) {
                    curItem->Active = curLocal->Active;
                    curItem->Status = STAT_UPDATE;
                }
            }
        }
        // If we didn't find the record, mark it for deletion.
        if (!foundIt) curItem->Status = STAT_DELETE;
    }

    return RetVal;    
}

/*
** checkLocalLogins - The second pass in comparing logins.  Steps through the
**                    local data and looks for any inserts.
*/

int checkLocalLogins(void)
{
    int         RetVal = 0;
    RemoteLogin *curItem;
    RemoteLogin *curRemote;
    int         foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = LLoginList.first(); curItem != 0; curItem = LLoginList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curRemote = RLoginList.first(); curRemote != 0; curRemote = RLoginList.next()) {
            if (curItem->InternalID == curRemote->InternalID) {
                foundIt = 1;
            }
        }
        // If we didn't find the record, add it in.
        if (!foundIt) {
	        curRemote = new RemoteLogin;
	        curRemote->InternalID     = curItem->InternalID;
	        curRemote->CustomerID     = curItem->CustomerID;
	        curRemote->LoginType      = curItem->LoginType;
            curRemote->Active         = curItem->Active;

	        strcpy(curRemote->LoginID, curItem->LoginID);
	        
	        curRemote->Status = STAT_INSERT;    // No change/update/delete/etc.
	        
	        RLoginList.append(curRemote);
        }
    }

    return RetVal;    
}

/*
** checkRemoteLoginTypes - The first pass in comparing.  Steps through the
**                         remote data and looks for any updates or deletes.
*/

int checkRemoteLoginTypes(void)
{
    int             RetVal = 0;
    RemoteLoginType *curItem;
    RemoteLoginType *curLocal;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = RLoginTypeList.first(); curItem != 0; curItem = RLoginTypeList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curLocal = LLoginTypeList.first(); curLocal != 0; curLocal = LLoginTypeList.next()) {
            if (curItem->InternalID == curLocal->InternalID) {
                foundIt = 1;
                // Found the item.  Check for any updates.
                
                // Check the login type
                if (strcmp(curItem->LoginType, curLocal->LoginType)) {
                    strcpy(curItem->LoginType, curLocal->LoginType);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the description
                if (strcmp(curItem->Description, curLocal->Description)) {
                    strcpy(curItem->Description, curLocal->Description);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the class
                if (strcmp(curItem->LoginClass, curLocal->LoginClass)) {
                    strcpy(curItem->LoginClass, curLocal->LoginClass);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the disk space
                if (curItem->DiskSpace != curLocal->DiskSpace) {
                    curItem->DiskSpace = curLocal->DiskSpace;
                    curItem->Status = STAT_UPDATE;
                }

                // And whether or not it was active
                if (curItem->Active != curLocal->Active) {
                    curItem->Active = curLocal->Active;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the flags
                if (strcmp(curItem->Flags, curLocal->Flags)) {
                    strcpy(curItem->Flags, curLocal->Flags);
                    curItem->Status = STAT_UPDATE;
                }

            }
        }
        // If we didn't find the record, mark it for deletion.
        if (!foundIt) curItem->Status = STAT_DELETE;
    }

    return RetVal;    
}

/*
** checkLocalLoginTypes - The second pass in comparing login types.  Steps 
**                        through the local data and looks for any inserts.
*/

int checkLocalLoginTypes(void)
{
    int             RetVal = 0;
    RemoteLoginType *curItem;
    RemoteLoginType *curRemote;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = LLoginTypeList.first(); curItem != 0; curItem = LLoginTypeList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curRemote = RLoginTypeList.first(); curRemote != 0; curRemote = RLoginTypeList.next()) {
            if (curItem->InternalID == curRemote->InternalID) {
                foundIt = 1;
            }
        }
        // If we didn't find the record, add it in.
        if (!foundIt) {
	        curRemote = new RemoteLoginType;
	        curRemote->InternalID     = curItem->InternalID;
	        curRemote->DiskSpace      = curItem->DiskSpace;
	        curRemote->Active         = curItem->Active;
            strcpy(curRemote->LoginType,    curItem->LoginType);
            strcpy(curRemote->Description,  curItem->Description);
            strcpy(curRemote->LoginClass,   curItem->LoginClass);
            strcpy(curRemote->Flags,        curItem->Flags);
	        
	        curRemote->Status = STAT_INSERT;    // No change/update/delete/etc.
	        
	        RLoginTypeList.append(curRemote);
        }
    }

    return RetVal;    
}


/*
** checkRemoteDomains - The first pass in comparing.  Steps through the
**                      remote data and looks for any updates or deletes.
*/

int checkRemoteDomains(void)
{
    int             RetVal = 0;
    RemoteDomain    *curItem;
    RemoteDomain    *curLocal;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = RDomainList.first(); curItem != 0; curItem = RDomainList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curLocal = LDomainList.first(); curLocal != 0; curLocal = LDomainList.next()) {
            if (curItem->InternalID == curLocal->InternalID) {
                foundIt = 1;
                // Found the item.  Check for any updates.
                
                // Check the Customer ID
                if (curItem->CustomerID != curLocal->CustomerID) {
                    curItem->CustomerID = curLocal->CustomerID;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the login ID
                if (strcmp(curItem->LoginID, curLocal->LoginID)) {
                    strcpy(curItem->LoginID, curLocal->LoginID);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the Login type
                if (curItem->DomainType != curLocal->DomainType) {
                    curItem->DomainType = curLocal->DomainType;
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check the Domain Name
                if (strcmp(curItem->DomainName, curLocal->DomainName)) {
                    strcpy(curItem->DomainName, curLocal->DomainName);
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check for SSL
                if (curItem->HasSSL != curLocal->HasSSL) {
                    curItem->HasSSL = curLocal->HasSSL;
                    curItem->Status = STAT_UPDATE;
                }
                
            }
        }
        // If we didn't find the record, mark it for deletion.
        if (!foundIt) curItem->Status = STAT_DELETE;
    }

    return RetVal;    
}

/*
** checkLocalDomains - The second pass in comparing domains.  Steps through the
**                     local data and looks for any inserts.
*/

int checkLocalDomains(void)
{
    int             RetVal = 0;
    RemoteDomain    *curItem;
    RemoteDomain    *curRemote;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = LDomainList.first(); curItem != 0; curItem = LDomainList.next()) {
        foundIt = 0;
        // And our inner loop, which actually does the checking.
        for (curRemote = RDomainList.first(); curRemote != 0; curRemote = RDomainList.next()) {
            if (curItem->InternalID == curRemote->InternalID) {
                foundIt = 1;
            }
        }
        // If we didn't find the record, add it in.
        if (!foundIt) {
	        curRemote = new RemoteDomain;
	        curRemote->InternalID     = curItem->InternalID;
	        curRemote->CustomerID     = curItem->CustomerID;
	        curRemote->DomainType     = curItem->DomainType;
            curRemote->HasSSL         = curItem->HasSSL;
	        strcpy(curRemote->LoginID, curItem->LoginID);
	        strcpy(curRemote->DomainName, curItem->DomainName);
	        
	        curRemote->Status = STAT_INSERT;    // No change/update/delete/etc.
	        
	        RDomainList.append(curRemote);
        }
    }

    return RetVal;    
}


/*
** checkRemoteRegister - The first pass in comparing.  Steps through the
**                       remote data and looks for any updates or deletes.
*/

int checkRemoteRegister(void)
{
    int             RetVal = 0;
    RemoteRegister  *curItem;
    RemoteRegister  *curLocal;
    
    // Lets just jump right into it...
    for (curItem = RemoteRegList.first(); curItem != 0; curItem = RemoteRegList.next()) {
        curLocal = LocalRegListDict[curItem->InternalID];
        if (curLocal != 0) {
        // And our inner loop, which actually does the checking.
        // for (curLocal = LocalRegList.first(); curLocal != 0; curLocal = RemoteRegList.next()) {
            if (curItem->InternalID == curLocal->InternalID) {
                // Found the item.  Check for any updates.
                // curItem->Status = STAT_NOCHANGE;
                
                // Check the Customer ID
                if (curItem->CustomerID != curLocal->CustomerID) {
                    curItem->CustomerID = curLocal->CustomerID;
                    curItem->Status = STAT_UPDATE;
                }

                // Check the login ID
                if (strcmp(curItem->LoginID, curLocal->LoginID)) {
                    strcpy(curItem->LoginID, curLocal->LoginID);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the Quantity
                if (curItem->Quantity != curLocal->Quantity) {
                    curItem->Quantity = curLocal->Quantity;
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check the Price
                if (curItem->Price != curLocal->Price) {
                    curItem->Price = curLocal->Price;
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check the Amount
                if (curItem->Amount != curLocal->Amount) {
                    curItem->Amount = curLocal->Amount;
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check the StatementNo
                if (curItem->StatementNo != curLocal->StatementNo) {
                    curItem->StatementNo = curLocal->StatementNo;
                    curItem->Status = STAT_UPDATE;
                }
                
                // Check the TransDate
                if (strcmp(curItem->TransDate, curLocal->TransDate)) {
                    strcpy(curItem->TransDate, curLocal->TransDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the BilledDate
                if (strcmp(curItem->BilledDate, curLocal->BilledDate)) {
                    strcpy(curItem->BilledDate, curLocal->BilledDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the DueDate
                if (strcmp(curItem->DueDate, curLocal->DueDate)) {
                    strcpy(curItem->DueDate, curLocal->DueDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the StartDate
                if (strcmp(curItem->StartDate, curLocal->StartDate)) {
                    strcpy(curItem->StartDate, curLocal->StartDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the EndDate
                if (strcmp(curItem->EndDate, curLocal->EndDate)) {
                    strcpy(curItem->EndDate, curLocal->EndDate);
                    curItem->Status = STAT_UPDATE;
                }

                // Check the Memo
                if (strcmp(curItem->Memo, curLocal->Memo)) {
                    delete curItem->Memo;
                    curItem->Memo = new char[strlen(curLocal->Memo)+2];
                    strcpy(curItem->Memo, curLocal->Memo);
                    curItem->Status = STAT_UPDATE;
                }
                curLocal = LocalRegList.last();
            }
        } else {
            curItem->Status = STAT_DELETE;
        }
    }

    return RetVal;    
}

/*
** checkLocalRegister - The second pass in comparing registers.  
**                      Steps through the local data and looks for 
**                      any inserts.
*/

int checkLocalRegister(void)
{
    int             RetVal = 0;
    RemoteRegister  *curItem;
    RemoteRegister  *curRemote;
    int             foundIt = 0;
    
    // Lets just jump right into it...
    for (curItem = LocalRegList.first(); curItem != 0; curItem = LocalRegList.next()) {
        foundIt = 0;
        curRemote = RemoteRegListDict[curItem->InternalID];
        if (curRemote == 0) {
            // We didn't find the record, add it in.
	        curRemote = new RemoteRegister;

	        curRemote->InternalID     = curItem->InternalID;
	        curRemote->CustomerID     = curItem->CustomerID;
	        curRemote->Quantity       = curItem->Quantity;
	        curRemote->Price          = curItem->Price;
	        curRemote->Amount         = curItem->Amount;
	        curRemote->StatementNo    = curItem->StatementNo;
	        
	        strcpy(curRemote->LoginID,    curItem->LoginID);
	        strcpy(curRemote->TransDate,  curItem->TransDate);
	        strcpy(curRemote->BilledDate, curItem->BilledDate);
	        strcpy(curRemote->DueDate,    curItem->DueDate);
	        strcpy(curRemote->StartDate,  curItem->StartDate);
	        strcpy(curRemote->EndDate,    curItem->EndDate);
	        
	        curRemote->Memo = new char[strlen(curItem->Memo) + 2];
	        strcpy(curRemote->Memo, curItem->Memo);
	        
	        curRemote->Status = STAT_INSERT;
	        
	        RemoteRegList.append(curRemote);
        }
    }

    return RetVal;    
}





/*
** updateRemoteCustomers - Steps through the remote customer list and 
**                         makes any necessary modifications to the remote
**                         database.
*/

int updateRemoteCustomers(void)
{
    int             RetVal = 0;
    RemoteCustomer  *curItem;
    ADBTable RCDB("Customers", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    
    long            inscount = 0;
    long            updcount = 0;
    long            delcount = 0;
    long            nochange = 0;
    
    for (curItem = RCustList.first(); curItem != 0; curItem = RCustList.next()) {
        switch (curItem->Status) {
            case STAT_DELETE:
                RCDB.del(curItem->CustomerID);
                delcount++;
                break;
            
            case STAT_UPDATE:
            case STAT_INSERT:
                if (curItem->Status == STAT_UPDATE) RCDB.get(curItem->CustomerID);
                RCDB.setValue("CustomerID",       curItem->CustomerID);
                RCDB.setValue("PrimaryLogin",     curItem->PrimaryLogin);
                RCDB.setValue("CurrentBalance",   curItem->CurrentBalance);
                RCDB.setValue("AutoPayment",      curItem->AutoPayment);
                RCDB.setValue("AutoExpires",      curItem->AutoExpires);
                RCDB.setValue("AutoLastFour",     curItem->AutoLastFour);
                RCDB.setValue("PendingPayment",   curItem->PendingPayment);
                RCDB.setValue("PendingAmount",    curItem->PendingAmount);
                RCDB.setValue("PendingDate",      curItem->PendingDate);
                RCDB.setValue("GraceDate",        curItem->GraceDate);
                RCDB.setValue("PrevBalance",      curItem->PrevBalance);
                RCDB.setValue("PrevBalanceDate",  curItem->PrevBalanceDate);
                RCDB.setValue("MailboxesAllowed", curItem->MailboxesAllowed);
                if (curItem->Status == STAT_UPDATE) {
                    RCDB.upd();
                    updcount++;
                } else {
                    RCDB.ins();
                    inscount++;
                }
                break;
            
            default:
                nochange++;
                break;
        }
    }
    
    syslog(LOG_INFO, "Remote customers:  Del %ld, Upd %ld, Ins %ld, NoCh %ld\n", delcount, updcount, inscount, nochange);
    
    return RetVal;
}


/*
** updateRemoteLogins - Steps through the remote login list and 
**                      makes any necessary modifications to the remote
**                      database.
*/

int updateRemoteLogins(void)
{
    int         RetVal = 0;
    RemoteLogin *curItem;
    ADBTable    RLDB("Logins", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    
    long        inscount = 0;
    long        updcount = 0;
    long        delcount = 0;
    long        nochange = 0;
    
    for (curItem = RLoginList.first(); curItem != 0; curItem = RLoginList.next()) {
        switch (curItem->Status) {
            case STAT_DELETE:
                RLDB.del(curItem->InternalID);
                delcount++;
                break;
            
            case STAT_UPDATE:
                RLDB.get(curItem->InternalID);
                RLDB.setValue("CustomerID", curItem->CustomerID);
                RLDB.setValue("LoginID",    curItem->LoginID);
                RLDB.setValue("LoginType",  curItem->LoginType);
                RLDB.setValue("Active",     curItem->Active);
                RLDB.upd();
                updcount++;
                break;
            
            case STAT_INSERT:
                RLDB.setValue("InternalID", curItem->InternalID);
                RLDB.setValue("CustomerID", curItem->CustomerID);
                RLDB.setValue("LoginID",    curItem->LoginID);
                RLDB.setValue("LoginType",  curItem->LoginType);
                RLDB.setValue("Active",     curItem->Active);
                RLDB.ins();
                inscount++;
                break;
            
            default:
                nochange++;
                break;
        }
    }
    
    syslog(LOG_INFO, "Remote logins:  Del %ld, Upd %ld, Ins %ld, NoCh %ld\n", delcount, updcount, inscount, nochange);
    
    return RetVal;
}

/*
** updateRemoteLoginTypes - Steps through the remote login type list and 
**                          makes any necessary modifications to the remote
**                          database.
*/

int updateRemoteLoginTypes(void)
{
    int             RetVal = 0;
    RemoteLoginType *curItem;
    ADBTable        RLDB("LoginTypes", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    
    long        inscount = 0;
    long        updcount = 0;
    long        delcount = 0;
    long        nochange = 0;
    
    for (curItem = RLoginTypeList.first(); curItem != 0; curItem = RLoginTypeList.next()) {
        switch (curItem->Status) {
            case STAT_DELETE:
                RLDB.del(curItem->InternalID);
                delcount++;
                break;
            
            case STAT_UPDATE:
                RLDB.get(curItem->InternalID);
                RLDB.setValue("LoginType",      curItem->LoginType);
                RLDB.setValue("Description",    curItem->Description);
                RLDB.setValue("DiskSpace",      curItem->DiskSpace);
                RLDB.setValue("LoginClass",     curItem->LoginClass);
                RLDB.setValue("Active",         curItem->Active);
                RLDB.setValue("LoginFlags",     curItem->Flags);
                RLDB.upd();
                updcount++;
                break;
            
            case STAT_INSERT:
                RLDB.setValue("InternalID",     curItem->InternalID);
                RLDB.setValue("LoginType",      curItem->LoginType);
                RLDB.setValue("Description",    curItem->Description);
                RLDB.setValue("DiskSpace",      curItem->DiskSpace);
                RLDB.setValue("LoginClass",     curItem->LoginClass);
                RLDB.setValue("Active",         curItem->Active);
                RLDB.setValue("LoginFlags",     curItem->Flags);
                RLDB.ins();
                inscount++;
                break;
            
            default:
                nochange++;
                break;
        }
    }
    
    syslog(LOG_INFO, "Remote login types:  Del %ld, Upd %ld, Ins %ld, NoCh %ld\n", delcount, updcount, inscount, nochange);
    
    return RetVal;
}
/*
** updateRemoteDomains - Steps through the remote domain list and 
**                       makes any necessary modifications to the remote
**                       database.
*/

int updateRemoteDomains(void)
{
    int          RetVal = 0;
    RemoteDomain *curItem;
    ADBTable     RDDB("Domains", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    
    long        inscount = 0;
    long        updcount = 0;
    long        delcount = 0;
    long        nochange = 0;
    
    for (curItem = RDomainList.first(); curItem != 0; curItem = RDomainList.next()) {
        switch (curItem->Status) {
            case STAT_DELETE:
                RDDB.del(curItem->InternalID);
                delcount++;
                break;
            
            case STAT_UPDATE:
                RDDB.get(curItem->InternalID);
                RDDB.setValue("CustomerID",  curItem->CustomerID);
                RDDB.setValue("LoginID",     curItem->LoginID);
                RDDB.setValue("DomainName",  curItem->DomainName);
                RDDB.setValue("DomainType",  curItem->DomainType);
                RDDB.setValue("HasSSL",      curItem->HasSSL);
                RDDB.upd();
                updcount++;
                break;
            
            case STAT_INSERT:
                RDDB.setValue("InternalID", curItem->InternalID);
                RDDB.setValue("CustomerID", curItem->CustomerID);
                RDDB.setValue("LoginID",    curItem->LoginID);
                RDDB.setValue("DomainName", curItem->DomainName);
                RDDB.setValue("DomainType", curItem->DomainType);
                RDDB.setValue("HasSSL",     curItem->HasSSL);
                RDDB.ins();
                inscount++;
                break;
            
            default:
                nochange++;
                break;
        }
    }
    
    syslog(LOG_INFO, "Remote domains:  Del %ld, Upd %ld, Ins %ld, NoCh %ld\n", delcount, updcount, inscount, nochange);
    
    return RetVal;
}
/*
** updateRemoteRegister - Steps through the remote login list and 
**                      makes any necessary modifications to the remote
**                      database.
*/

int updateRemoteRegister(void)
{
    int             RetVal = 0;
    RemoteRegister  *curItem;
    ADBTable        RRDB("Register", REMOTEDBASE, REMOTEUSER, REMOTEPASS, REMOTEHOST);
    
    long        inscount = 0;
    long        updcount = 0;
    long        delcount = 0;
    long        nochange = 0;
    
    for (curItem = RemoteRegList.first(); curItem != 0; curItem = RemoteRegList.next()) {
        switch (curItem->Status) {
            case STAT_DELETE:
                RRDB.del(curItem->InternalID);
                delcount++;
                break;
            
            case STAT_UPDATE:
            case STAT_INSERT:
                // if (curItem->Status == STAT_INSERT) RRDB.setValue("InternalID", curItem->InternalID);
                if (curItem->Status == STAT_UPDATE) RRDB.get(curItem->InternalID);
                else RRDB.setValue("InternalID", curItem->InternalID);
                RRDB.setValue("CustomerID",     curItem->CustomerID);
                RRDB.setValue("LoginID",        curItem->LoginID);
                RRDB.setValue("Quantity",       curItem->Quantity);
                RRDB.setValue("Price",          curItem->Price);
                RRDB.setValue("Amount",         curItem->Amount);
                RRDB.setValue("StatementNo",    curItem->StatementNo);
                RRDB.setValue("TransDate",      curItem->TransDate);
                RRDB.setValue("BilledDate",     curItem->BilledDate);
                RRDB.setValue("DueDate",        curItem->DueDate);
                RRDB.setValue("StartDate",      curItem->StartDate);
                RRDB.setValue("EndDate",        curItem->EndDate);
                RRDB.setValue("Memo",           curItem->Memo);
                
                if (curItem->Status == STAT_UPDATE) {
                    RRDB.upd();
                    updcount++;
                } else {
                    RRDB.ins();
                    inscount++;
                }
                break;
            
            default:
                nochange++;
                break;
        }
    }
    
    syslog(LOG_INFO, "Remote register:  Del %ld, Upd %ld, Ins %ld, NoCh %ld\n", delcount, updcount, inscount, nochange);
    
    return RetVal;
}













/*
** loadKey - Loads the program key and stuffs it into memory.
**           The key must be called '/usr/local/lib/tact.key'
**           It also sets up the user key which is used by the
**           encrypt_string and decrypt_string functions.
**           If the key is invalid, it aborts the program.
*/

void loadKey(void)
{
    FILE        *fp;
    int         Success = 1;
    int         LineNo  = 0;
    unsigned    char *src;
    unsigned    char *dst;
    // extern      ProgramKeyStruct ProgramKey;
    // extern      unsigned char cbc_user[8];

    // Clear out the program key information.
    memset(&ProgramKey, 0, sizeof(ProgramKey));
    
    src = (unsigned char *) calloc(1024, sizeof(char));
    dst = (unsigned char *) calloc(1024, sizeof(char));
    
    fp = fopen(TACT_KEYFILE, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to load key file '%s'.  Exiting.\n", TACT_KEYFILE);
        exit(-1);
    }

    while (fgets((char *)src, 1000, fp) != NULL) {
        LineNo++;
        if (Success) {
            if (!decrypt_string(src, dst, 0)) {
                switch (LineNo) {
                    case  1:
                        strcpy(ProgramKey.CompanyName, (const char *) dst);
                        break;

                    case  2:
                        strcpy(ProgramKey.CompanyAddr1, (const char *) dst);
                        break;

                    case  3:
                        strcpy(ProgramKey.CompanyAddr2, (const char *) dst);
                        break;
                    
                    case  4:
                        strcpy(ProgramKey.CompanyCity, (const char *) dst);
                        break;
                        
                    case  5:
                        strcpy(ProgramKey.CompanyState, (const char *) dst);
                        break;

                    case  6:
                        strcpy(ProgramKey.CompanyPostal, (const char *) dst);
                        break;

                    case  7:
                        strcpy(ProgramKey.CompanyCountry, (const char *) dst);
                        break;

                    case  8:
                        strcpy(ProgramKey.CompanyPhone, (const char *) dst);
                        break;

                    case  9:
                        strcpy(ProgramKey.CompanyFax, (const char *) dst);
                        break;

                    case 10:
                        strcpy(ProgramKey.PurchaseDate, (const char *) dst);
                        break;

                    case 11:
                        strcpy(ProgramKey.ExpDate, (const char *) dst);
                        break;

                    case 12:
                        ProgramKey.MaxUsers = atol((const char *) dst);
                        break;

                    case 13:
                        strcpy(ProgramKey.CipherKey, (const char *) dst);
                        break;

                    default:
                        Success = 0;
                        break;
                } 
            } else {
                Success = 0;
            }
        }
        // memset(src, 0, 1024);
        // memset(dst, 0, 1024);
    }
    fclose(fp);
    
    // Check the line count.
    if (LineNo != 13) {
        Success = 0;
    }

    // Check the expiration date of the key.    
/*    printf("Checking the date.\n");
    fflush(stdout);
    if (Success) {
        QString tmpQstr;
        QDate   keyDate;
        tmpQstr.setStr(ProgramKey.ExpDate);

        #ifdef DBDEBUG
        fprintf(stderr, "myDatetoQDate: Source date is %s\n", myDate);
        #endif

	    keyDate.setYMD(
    	  atoi((const char *) tmpQstr.mid(0,4)),
	      atoi((const char *) tmpQstr.mid(5,2)),
	      atoi((const char *) tmpQstr.mid(8,2))
	    );
	    
	    if (keyDate < QDate::currentDate()) {
	        printf("Your program key has expired.\n");
	        exit(-1);
	    }
    }
*/

    // Assign the user Cipher Key
    set_user_key((unsigned char *) ProgramKey.CipherKey);
    
    if (!Success) {
        fprintf(stderr, "\nError with key file.\n\n");
        exit(-1);
    }
}


/*
** loadConfig   - Loads the configuration values into memory.
*/

void loadConfig(void)
{
	strcpy(REMOTEHOST , "");
	strcpy(REMOTEDBASE, "");
	strcpy(LOCALHOST  , "");
	strcpy(LOCALDBASE , "");

	strcpy(REMOTEUSER , "");
	strcpy(REMOTEPASS , "");
	strcpy(LOCALUSER  , "");
	strcpy(LOCALPASS  , "");

    if (!loadCfg("./wasync.cf")) {
        if (!loadCfg(CONFIGFILE)) {
            fprintf(stderr, "Unable to load config file.  Aborting.");
            exit(-1);
        }
    }
    strcpy(REMOTEHOST,  cfgVal("RemoteHost"));
	strcpy(REMOTEDBASE, cfgVal("RemoteDBase"));
	strcpy(LOCALHOST,   cfgVal("LocalHost"));
	strcpy(LOCALDBASE,  cfgVal("LocalDBase"));

	strcpy(REMOTEUSER,  cfgVal("RemoteUser"));
	strcpy(REMOTEPASS,  cfgVal("RemotePass"));
	strcpy(LOCALUSER,   cfgVal("LocalUser"));
	strcpy(LOCALPASS,   cfgVal("LocalPass"));

}
