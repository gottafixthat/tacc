/**
 * seanetimport.cpp
 *
 * One time program that will take the seanet data and migrate it
 * to tacc.
 */

#include <TAATools.h>
#include <CSVParser.h>
#include <qapplication.h>
#include <QSqlDbPool.h>
#include <qsqlcursor.h>
#include <qsqlrecord.h>
#include <Cfg.h>
#include <BlargDB.h>
#include <CCValidate.h>
#include <AcctsRecv.h>
#include <qregexp.h>
#include <getopt.h>
#include <CustomerContactsDB.h>
#include <loadKey.h>

#include "seanetimport.h"

// Globals
QPtrList<customerRecord> customerList;
QPtrList<domainRecord> domainListFull;
QPtrList<loginRecord> loginListFull;
QPtrList<billingCycleRecord> billingCycleList;
int doHistoricalData;

int main( int argc, char ** argv )
{
    // Load the main config file and setup our QApplication.
    loadTAAConfig();
    QApplication    a( argc, argv );
    bool            scrubDB = true;
    doHistoricalData = 1;

    fprintf(stderr, "AcctsRecvAccount = '%s'\n", cfgVal("AcctsRecvAccount"));

    // Setup the database pool
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));

    QSqlDbPool::setDefaultHost(cfgVal("TAAMySQLHost"));
    QSqlDbPool::setDefaultName(cfgVal("TAAMySQLDB"));
    QSqlDbPool::setDefaultUser(cfgVal("TAAMySQLUser"));
    QSqlDbPool::setDefaultPass(cfgVal("TAAMySQLPass"));
    QSqlDbPool::setDefaultDriver(cfgVal("TAASQLDriver"));

    loadKey();

    // Transfer our CSV files into tables
    /*
    csvImport("PLANS_AccessSet.csv", "import_access_set");
    csvImport("PLANS_dialup_NONstatic.csv", "import_dialup_nonstatic");
    csvImport("PLANS_dialup_static.csv", "import_dialup_static");
    csvImport("PLANS_main_billable.csv", "import_main_billable");
    csvImport("PLANS_nailedset1.csv", "import_nailedset1");
    csvImport("PLANS_nailedset_2.csv", "import_nailedset2");
    csvImport("PLANS_virtmail_1.csv", "import_virtmail1");
    csvImport("PLANS_virtmail_2.csv", "import_virtmail2");
    csvImport("PLANS_websets.csv", "import_websets");
    */

    int c;
    while(1) {
        c = getopt(argc,argv,"sh");
        if (c == -1) break;
        switch(c) {
            case    's':
                scrubDB = false;
                break;
            case    'h':
                doHistoricalData = 0;
                break;
            default:
                break;
        }
    }

    if (scrubDB) cleanDatabase();

    // Import the login types and billable items.
    
    loadAccountTypes();
    loadChartOfAccounts();
    fprintf(stderr, "AcctsRecvAccount = '%s'\n", cfgVal("AcctsRecvAccount"));

    loadBillingCycles();
    importLoginTypes();
    loadDomains();
    loadWebSet();
    loadVirtualSet();
    loadDialupStatic();
    loadDialupDynamic();
    loadDSLAccessSet();
    loadNailedSet();
    loadCustomers();
    importCustomers();

    if (doHistoricalData) {
        loadCustomerCharges();
        loadCustomerPayments();
    }

}

/**
 * csvImport()
 *
 * Given a CSV file and a table name, this migrates all of the columns
 * found in the CSV into a SQL database.
 */
void csvImport(const char *csvFile, const char *tableName, char delim)
{
    CSVParser   parser;
    QChar       tmpDelim(delim);
    parser.setDelimiter(tmpDelim);
    // Open our CSV file
    if (!parser.openFile(csvFile, true)) {
        fprintf(stderr, "Unable to open '%s', aborting\n", csvFile);
        exit(-1);
    }

    QSqlDbPool  dbpool;
    QSqlQuery   q(dbpool.qsqldb());
    QString     qstr;

    fprintf(stderr, "\e[KLoading table '%s' from '%s'\r", tableName, csvFile);
    qstr = "drop table if exists ";
    qstr += tableName;
    if (!q.exec(qstr)) {
        fprintf(stderr, "Error dropping table '%s'\n", tableName);
        fprintf(stderr, "Query: '%s'\n", q.lastQuery().ascii());
        exit(-1);
    }

    qstr = "create table ";
    qstr += tableName;
    qstr += "(id bigint(21) not null auto_increment, processed int(11) not null default '0', ";

    for (uint i = 0; i < parser.header().count(); i++) {
        qstr += parser.header()[i];
        qstr += " varchar(255) NOT NULL default '', ";
    }
    qstr += " PRIMARY KEY(id))";

    if (!q.exec(qstr)) {
        fprintf(stderr, "Error creating table '%s'\n", tableName);
        fprintf(stderr, "Query: '%s'\n", q.lastQuery().ascii());
        exit(-1);
    }

    int rowCount = 0;
    while(parser.loadRecord()) {
        rowCount++;
        if (!(rowCount % 10)) fprintf(stderr, "\e[KLoading table '%s' from '%s'...Row %d...\r", tableName, csvFile, rowCount);
        QSqlQuery   iq(dbpool.qsqldb());
        QString tmpStr;
        qstr = "insert into ";
        qstr += tableName;
        qstr += " values(0, 0, ";
        for (uint i = 0; i < parser.row().count(); i++) {
            qstr += "'";
            tmpStr = parser.row()[i];
            tmpStr.replace("\\", "");
            tmpStr.replace(QRegExp("'"), "\\'");
            qstr += tmpStr;
            qstr += "'";
            if (i < parser.row().count() -1) qstr += ", ";
        }
        qstr += ")";
        if (!iq.exec(qstr)) {
            fprintf(stderr, "Error inserting row.\n");
            fprintf(stderr, "Query: '%s'\n", iq.lastQuery().ascii());
            exit(-1);
        }
    }
    fprintf(stderr, "\e[KFinished loading table '%s' from '%s'...%d Rows...\n", tableName, csvFile, rowCount);
}

/**
 * cleanDatabase()
 *
 * Scrubs all of the tables needed for a clean import.
 */
void cleanDatabase()
{
    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());
    QStringList tables;
    tables  += "Accounts";
    tables  += "AcctsRecv";
    tables  += "Addresses";
    tables  += "AutoPayments";
    tables  += "Billables";
    tables  += "BillablesData";
    tables  += "BillingCycles";
    tables  += "CCTrans";
    tables  += "Contacts";
    tables  += "Customers";
    tables  += "CustomerContacts";
    tables  += "DomainTypeBillables";
    tables  += "DomainTypes";
    tables  += "Domains";
    tables  += "GL";
    tables  += "GLIndex";
    tables  += "GLAccountTypes";
    tables  += "LoginFlagValues";
    tables  += "LoginTypeBillables";
    tables  += "LoginTypeFlags";
    tables  += "LoginTypes";
    tables  += "Logins";
    tables  += "Notes";
    tables  += "PackageContents";
    tables  += "Packages";
    tables  += "PackagesData";
    tables  += "RegisterSplits";
    tables  += "Statements";
    tables  += "StatementsData";
    tables  += "Subscriptions";

    QString tmpStr;
    for ( QStringList::Iterator it = tables.begin(); it != tables.end(); ++it ) {
        tmpStr = *it;
        fprintf(stderr, "\e[KScrubbing table %s...\r", tmpStr.ascii());

        tmpStr = "delete from ";
        tmpStr += *it;
        q.exec(tmpStr);
    }
    fprintf(stderr, "\e[KFinished scrubbing %d tables...\n", tables.count());
}

/**
 * loadGLAccountTypes()
 *
 * Loads the GLAccountTypes from a files.
 */
void loadAccountTypes()
{
    CSVParser   parser;
    QChar       delim('\t');
    parser.setDelimiter(delim);
    // Open our CSV file
    if (!parser.openFile("GLAccountTypes.tsv", true)) {
        fprintf(stderr, "Unable to open GLAccountTypes.tsv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int acctTypeCol     = parser.header().findIndex("AccountType");
    int descCol         = parser.header().findIndex("Description");

    ADBTable    atDB("GLAccountTypes");

    while(parser.loadRecord()) {
        atDB.clearData();
        atDB.setValue("AccountType",    parser.row()[acctTypeCol].ascii());
        atDB.setValue("Description",    parser.row()[descCol].ascii());
        atDB.ins();
    }
}

/**
 * loadChartOfAccounts()
 *
 * Loads the Accounts table from a file.
 */
void loadChartOfAccounts()
{
    CSVParser   parser;
    QChar       delim('\t');
    parser.setDelimiter(delim);
    // Open our CSV file
    if (!parser.openFile("Accounts.tsv", true)) {
        fprintf(stderr, "Unable to open Accounts.tsv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int intAcctNoCol    = parser.header().findIndex("IntAccountNo");
    int acctNoCol       = parser.header().findIndex("AccountNo");
    int acctNameCol     = parser.header().findIndex("AcctName");
    int parentIDCol     = parser.header().findIndex("ParentID");
    int acctTypeCol     = parser.header().findIndex("AccountType");
    int provAcctCol     = parser.header().findIndex("ProviderAccountNo");
    int taxLineCol      = parser.header().findIndex("TaxLine");

    ADBTable    acctDB("Accounts");

    while(parser.loadRecord()) {
        acctDB.clearData();
        acctDB.setValue("IntAccountNo",   parser.row()[intAcctNoCol].ascii());
        acctDB.setValue("AccountNo",      parser.row()[acctNoCol].ascii());
        acctDB.setValue("AcctName",       parser.row()[acctNameCol].ascii());
        acctDB.setValue("ParentID",       parser.row()[parentIDCol].ascii());
        acctDB.setValue("AccountType",    parser.row()[acctTypeCol].ascii());
        acctDB.setValue("ProviderAccountNo", parser.row()[provAcctCol].ascii());
        acctDB.setValue("TaxLine",        parser.row()[taxLineCol].ascii());
        acctDB.setValue("Balance",        (float) 0.0);
        acctDB.setValue("TransCount",     0);
        acctDB.ins();
    }

    // Now set our AR account to be the one defined as ARACCOUNT
    ADB DB;
    DB.query("select IntAccountNo from Accounts where AccountNo = '%s'", ARACCOUNT);
    if (!DB.rowCount) {
        fprintf(stderr, "Unable to locate AR Account '%s'.  Aborting.\n", ARACCOUNT);
        exit(-1);
    }
    DB.getrow();
    updateCfgVal("AcctsRecvAccount", DB.curRow["IntAccountNo"]);

    // Now do the same for undeposited funds.
    DB.query("select IntAccountNo from Accounts where AccountNo = '%s'", SALESACCOUNT);
    if (!DB.rowCount) {
        fprintf(stderr, "Unable to locate Account '%s'.  Aborting.\n", SALESACCOUNT);
        exit(-1);
    }
    DB.getrow();
    updateCfgVal("UndepositedFundsAcct", DB.curRow["IntAccountNo"]);
}

/**
 * loadBillingCycles()
 *
 * Loads the billing cycles into memory.  All it really loads is
 * the InternalID and Period for billing cycles with type Anniversary.
 */
void loadBillingCycles()
{
    QSqlDbPool  dbpool;
    QSqlQuery   q(dbpool.qsqldb());

    if (!q.exec("select InternalID, AnniversaryPeriod from BillingCycles where CycleType = 'Anniversary'")) {
        fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
        exit(-1);
    }
    if (q.size()) {
        while(q.next()) {
            billingCycleRecord  *rec = new billingCycleRecord;
            rec->cycleID = q.value(0).toInt();
            rec->period  = q.value(1).toInt();
            billingCycleList.append(rec);
        }
    } else {
        // Define our billing cycles.
        ADBTable    bcDB("BillingCycles");
        bcDB.clearData();
        bcDB.setValue("CycleID",        "Monthly");
        bcDB.setValue("Description",    "Monthly");
        bcDB.setValue("DefaultCycle",   1);
        bcDB.setValue("CycleType",      "Anniversary");
        bcDB.setValue("AnniversaryPeriod", 1);
        bcDB.ins();

        bcDB.clearData();
        bcDB.setValue("CycleID",        "Quarterly");
        bcDB.setValue("Description",    "Quarterly");
        bcDB.setValue("DefaultCycle",   0);
        bcDB.setValue("CycleType",      "Anniversary");
        bcDB.setValue("AnniversaryPeriod", 3);
        bcDB.ins();

        bcDB.clearData();
        bcDB.setValue("CycleID",        "Semi-Annual");
        bcDB.setValue("Description",    "Semi-Annual");
        bcDB.setValue("DefaultCycle",   0);
        bcDB.setValue("CycleType",      "Anniversary");
        bcDB.setValue("AnniversaryPeriod", 6);
        bcDB.ins();

        bcDB.clearData();
        bcDB.setValue("CycleID",        "Annual");
        bcDB.setValue("Description",    "Annual");
        bcDB.setValue("DefaultCycle",   0);
        bcDB.setValue("CycleType",      "Anniversary");
        bcDB.setValue("AnniversaryPeriod", 12);
        bcDB.ins();

        // And reload our cycles.
        loadBillingCycles();
    }
}

/**
 * getBillingCycleID()
 *
 * Given a billing period, this will return the InternalID of the billing
 * cycle or zero if it wasn't found.
 */
long getBillingCycleID(int period)
{
    long    retVal = 0;
    billingCycleRecord  *rec;
    for (unsigned int c = 0; c < billingCycleList.count(); c++) {
        rec = billingCycleList.at(c);
        if (rec->period == period) {
            retVal = rec->cycleID;
        }
    }
    
    return retVal;
}

/**
 * loadDomains()
 *
 * Loads the domain records from the CSV file into memory.
 */
void loadDomains()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_domainSet.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_domainSet.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("RegNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int domainNameCol       = parser.header().findIndex("ItemVal1");
    int spamFilterCol       = parser.header().findIndex("ItemVal2");
    int mailTypeCol         = parser.header().findIndex("ItemVal3");

    char    userName[1024];

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        recCount++;
        sprintf(userName, "domainset%05d", recCount);
        loginRecord   *loginRec = new loginRecord;
        loginRec->regNumber     = parser.row()[regNumberCol];
        loginRec->planStatus    = parser.row()[planStatusCol].toInt();
        loginRec->serviceType   = parser.row()[serviceTypeCol];
        loginRec->serviceStart  = parser.row()[serviceStartCol];
        loginRec->virtDomain    = parser.row()[domainNameCol];
        loginRec->spamFilter    = parser.row()[spamFilterCol];
        loginRec->mailType      = parser.row()[mailTypeCol];
        loginRec->userName      = userName;

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        fprintf(stderr, "\e[KLoaded domain %s...\r", loginRec->virtDomain.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d domains into memory.\n", recCount);
}

/**
 * loadWebSet()
 *
 * Loads the web set records from the CSV file into memory.
 */
void loadWebSet()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_websets.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_websets.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("accountNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int webSvcTypeCol       = parser.header().findIndex("webServiceTypeName");
    int hostingUserCol      = parser.header().findIndex("username");
    int hostingPassCol      = parser.header().findIndex("password");
    int domain1Col          = parser.header().findIndex("domain1");
    int domain2Col          = parser.header().findIndex("domain2");

    char    userName[1024];

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        recCount++;
        sprintf(userName, "webset%05d", recCount);
        loginRecord   *loginRec = new loginRecord;
        loginRec->regNumber     = parser.row()[regNumberCol];
        loginRec->userName      = userName;
        loginRec->planStatus    = parser.row()[planStatusCol].toInt();
        loginRec->serviceType   = parser.row()[serviceTypeCol];
        loginRec->webSvcType    = parser.row()[webSvcTypeCol];
        loginRec->hostingUserName = parser.row()[hostingUserCol];
        loginRec->password      = parser.row()[hostingPassCol];
        loginRec->virtDomain    = parser.row()[domain1Col];
        loginRec->virtDomain2   = parser.row()[domain2Col];

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        fprintf(stderr, "\e[KLoaded webset domain %s...\r", loginRec->virtDomain.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d webset domains into memory.\n", recCount);
}

/**
 * loadVirtualSet()
 *
 * Loads the domain records from the CSV file into memory.
 */
void loadVirtualSet()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_virtmail_2.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_virtmail_2.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("RegNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int virtUserNameCol     = parser.header().findIndex("ItemVal1");
    int domainNameCol       = parser.header().findIndex("ItemVal2");
    int virtDestCol         = parser.header().findIndex("ItemVal3");

    char    userName[1024];

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        recCount++;
        sprintf(userName, "virtualset%05d", recCount);
        loginRecord   *loginRec = new loginRecord;
        loginRec->regNumber     = parser.row()[regNumberCol];
        loginRec->userName      = userName;
        loginRec->planStatus    = parser.row()[planStatusCol].toInt();
        loginRec->serviceType   = parser.row()[serviceTypeCol];
        loginRec->serviceStart  = parser.row()[serviceStartCol];
        loginRec->virtDomain    = parser.row()[domainNameCol];
        loginRec->virtUser      = parser.row()[virtUserNameCol];
        loginRec->virtDest      = parser.row()[virtDestCol];

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        fprintf(stderr, "\e[KLoaded virtual domain %s...\r", loginRec->virtDomain.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d virtual domains into memory.\n", recCount);
}
/**
 * loadDialupStatic()
 *
 * Loads the static dialup records from the CSV file into memory.
 */
void loadDialupStatic()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_dialup_statics.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_dialup_statics.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("RegNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int userNameCol         = parser.header().findIndex("username");
    int passwordCol         = parser.header().findIndex("password");
    int virtDomainCol       = parser.header().findIndex("domainName");
    int autoAssignCol       = parser.header().findIndex("autoAssign");
    int dateAssignedCol     = parser.header().findIndex("dateAssigned");
    int netmaskCol          = parser.header().findIndex("netmask");
    int ipAddrCol           = parser.header().findIndex("startIPAddress");
    int mailTypeCol         = parser.header().findIndex("mailTypeName");


    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        loginRecord    *loginRec  = new loginRecord;
        loginRec->regNumber        = parser.row()[regNumberCol];
        loginRec->planStatus       = parser.row()[planStatusCol].toInt();
        loginRec->serviceStart     = parser.row()[serviceStartCol];
        loginRec->serviceType      = parser.row()[serviceTypeCol];
        loginRec->userName         = parser.row()[userNameCol];
        loginRec->password         = parser.row()[passwordCol];
        loginRec->virtDomain       = parser.row()[virtDomainCol];
        loginRec->autoAssign       = parser.row()[autoAssignCol];
        loginRec->dateAssigned     = parser.row()[dateAssignedCol];
        loginRec->netmask          = parser.row()[netmaskCol];
        loginRec->ipAddr           = parser.row()[ipAddrCol];
        loginRec->mailType         = parser.row()[mailTypeCol];
        loginRec->foundMatch       = 0;

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }
        loginListFull.append(loginRec);

        recCount++;
        fprintf(stderr, "\e[KLoaded static dialup %s...\r", loginRec->userName.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d static dialups into memory.\n", recCount);
}

/**
 * loadDialupDynamic()
 *
 * Loads the non-static dialup records from the CSV file into memory.
 */
void loadDialupDynamic()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_dialup_NONstatic.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_dialup_NONstatic.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("RegNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int userNameCol         = parser.header().findIndex("username");
    int passwordCol         = parser.header().findIndex("password");
    int virtDomainCol       = parser.header().findIndex("domainName");
    int mailTypeCol         = parser.header().findIndex("mailTypeName");

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        loginRecord    *loginRec  = new loginRecord;
        loginRec->regNumber        = parser.row()[regNumberCol];
        loginRec->planStatus       = parser.row()[planStatusCol].toInt();
        loginRec->serviceStart     = parser.row()[serviceStartCol];
        loginRec->serviceType      = parser.row()[serviceTypeCol];
        loginRec->userName         = parser.row()[userNameCol];
        loginRec->password         = parser.row()[passwordCol];
        loginRec->virtDomain       = parser.row()[virtDomainCol];
        loginRec->mailType         = parser.row()[mailTypeCol];
        loginRec->foundMatch       = 0;
        loginRec->loginTypeID      = 0;

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        recCount++;
        fprintf(stderr, "\e[KLoaded dynamic dialup %s...\r", loginRec->userName.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d dynamic dialups into memory.\n", recCount);
}

/**
 * loadDSLAccessSet()
 *
 * Loads the non-static dialup records from the CSV file into memory.
 */
void loadDSLAccessSet()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_AccessSet_dsl.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_AccessSet_dsl.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("RegNumber");
    int planStatusCol       = parser.header().findIndex("Status");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int providerCol         = parser.header().findIndex("Expr1");
    int circuitIDCol        = parser.header().findIndex("customerCircuitID");
    int vpiCol              = parser.header().findIndex("vpi");
    int vciCol              = parser.header().findIndex("vci");
    int cpeCol              = parser.header().findIndex("Expr2");
    int requirePVCCol       = parser.header().findIndex("requiresPVC");
    char    userName[1024];

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        recCount++;
        sprintf(userName, "dslset%05d", recCount);
        loginRecord    *loginRec  = new loginRecord;
        loginRec->regNumber        = parser.row()[regNumberCol];
        loginRec->planStatus       = parser.row()[planStatusCol].toInt();
        loginRec->serviceStart     = parser.row()[serviceStartCol];
        loginRec->serviceType      = parser.row()[serviceTypeCol];
        loginRec->userName         = userName;
        loginRec->provider          = parser.row()[providerCol];
        loginRec->circuitID         = parser.row()[circuitIDCol];
        loginRec->vpi               = parser.row()[vpiCol];
        loginRec->vci               = parser.row()[vciCol];
        loginRec->cpeName           = parser.row()[cpeCol];
        loginRec->requirePVC        = parser.row()[requirePVCCol];
        loginRec->foundMatch       = 0;
        loginRec->loginTypeID      = 0;

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        fprintf(stderr, "\e[KLoaded DSLset %s...\r", loginRec->userName.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d DSLset entries into memory.\n", recCount);
}

/**
 * loadNailedSet()
 *
 * Loads the nailed set records from the CSV file into the logins list.
 */
void loadNailedSet()
{
    CSVParser   parser;
    // Open our CSV file
    if (!parser.openFile("PLANS_nailedset1.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_nailedset1.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("accountNumber");
    int planStatusCol       = parser.header().findIndex("PlanStatus");
    int serviceStartCol     = parser.header().findIndex("ServiceStart");
    int serviceTypeCol      = parser.header().findIndex("Detail");
    int nailedTypeCol       = parser.header().findIndex("componentSetName");
    int routeSwitchCol      = parser.header().findIndex("routerOrSwitch");
    int portDLCICol         = parser.header().findIndex("portOrDLCI");
    int ipAddrCol           = parser.header().findIndex("startIPAddress");
    int netmaskCol          = parser.header().findIndex("netmask");
    char    userName[1024];

    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());

    int recCount = 0;
    while(parser.loadRecord()) {
        recCount++;
        sprintf(userName, "nailedset%05d", recCount);
        loginRecord    *loginRec  = new loginRecord;
        loginRec->regNumber        = parser.row()[regNumberCol];
        loginRec->planStatus       = parser.row()[planStatusCol].toInt();
        loginRec->serviceStart     = parser.row()[serviceStartCol];
        loginRec->serviceType      = parser.row()[serviceTypeCol];
        loginRec->userName         = userName;
        loginRec->nailedType        = parser.row()[nailedTypeCol];
        loginRec->routeSwitch       = parser.row()[routeSwitchCol];
        loginRec->ipAddr            = parser.row()[ipAddrCol];
        loginRec->netmask           = parser.row()[netmaskCol];
        loginRec->portDLCI          = parser.row()[portDLCICol];
        loginRec->foundMatch       = 0;
        loginRec->loginTypeID      = 0;

        // Get the loginTypeID
        q.prepare("select InternalID, LoginType, Description from LoginTypes where LoginType LIKE :svctype");
        q.bindValue(":svctype", parser.row()[serviceTypeCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            loginRec->loginTypeID = q.value(0).toInt();
        }

        loginListFull.append(loginRec);
        fprintf(stderr, "\e[KLoaded NailedSet %s...\r", loginRec->userName.ascii());
    }
    fprintf(stderr, "\e[KLoaded %d NailedSet entries into memory.\n", recCount);
}

/**
 * addLoginType - Given the login type, this creates an entry for it.
 */
void addLoginType(const char *loginType)
{
    QSqlDbPool  pool;
    QSqlCursor  logins("LoginTypes", true, pool.qsqldb());
    QSqlRecord  *buf;

    buf = logins.primeInsert();
    buf->setValue("InternalID",             0);
    buf->setValue("LoginType",              loginType);
    buf->setValue("Description",            loginType);
    buf->setValue("DiskSpace",              0);
    buf->setValue("DiskSpaceBillable",      0);
    buf->setValue("DialupChannels",         0);
    buf->setValue("DialupChannelsBillable", 0);
    buf->setValue("Active",                 1);
    buf->setValue("AdditionalMailboxes",    0);
    buf->setValue("LoginClass",             loginType);
    logins.insert();
}

/**
 * addLoginTypeFlag - Given the login type, and a flag name, this associates
 * the specified flag with the specified login type.
 * If the flag doesn't exist, it is added.
 */
void addLoginTypeFlag(const char *loginType, const char *flagName)
{
    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());
    QSqlCursor  logins("LoginTypes", true, pool.qsqldb());
    QSqlCursor  loginTypeFlags("LoginTypeFlags", true, pool.qsqldb());
    QSqlRecord  *buf;
    int         loginTypeID;
    int         flagID;

    // First things first, get the login type ID
    q.prepare("select InternalID from LoginTypes where LoginType = :loginType");
    q.bindValue(":loginType",   loginType);
    if (!q.exec()) {
        fprintf(stderr, "addLoginTypeFlag()\n");
        fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
        exit(-1);
    }
    if (!q.size()) {
        fprintf(stderr, "addLoginTypeFlag() - No matching login type '%s'\n", loginType);
        exit(-1);
    }
    q.next();
    loginTypeID = q.value(0).toInt();

    // Check to see if the login flag has already been defined or not.
    q.prepare("select InternalID from LoginFlags where LoginFlag = :loginFlag");
    q.bindValue(":loginFlag",   flagName);
    if (!q.exec()) {
        fprintf(stderr, "addLoginTypeFlag()\n");
        fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
        exit(-1);
    }
    if (!q.size()) {
        // Not found, insert it.
        QSqlCursor  lFlags("LoginFlags", true, pool.qsqldb());
        buf = lFlags.primeInsert();
        buf->setValue("InternalID",         0);
        buf->setValue("LoginFlag",          flagName);
        buf->setValue("Description",        flagName);
        buf->setValue("IsBool",             0);
        buf->setValue("BaseType",           0);
        buf->setValue("DefaultValue",       " ");
        buf->setValue("UserDefined",        1);
        lFlags.insert();
        // Get the flag ID
        q.prepare("select InternalID from LoginFlags where LoginFlag = :loginFlag");
        q.bindValue(":loginFlag",   flagName);
        if (!q.exec()) {
            fprintf(stderr, "addLoginTypeFlag()\n");
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (!q.size()) {
            fprintf(stderr, "addLoginTypeFlag() - Error adding flag '%s'\n", flagName);
            exit(-1);
        }
        // We'll grab the value after we exit this nesting level
    }

    q.next();
    flagID = q.value(0).toInt();

    buf = loginTypeFlags.primeInsert();
    buf->setValue("InternalID",             0);
    buf->setValue("LoginTypeID",            loginTypeID);
    buf->setValue("Tag",                    flagName);
    buf->setValue("Value",                  " ");
    loginTypeFlags.insert();
}

/**
 * setLoginFlag()
 *
 * Given a username, flag name and value, this sets the flag.
 */
void setLoginFlag(const QString userName, const char *flagName, const QString val)
{
    QSqlDbPool  pool;
    QSqlQuery   q(pool.qsqldb());
    q.prepare("replace into LoginFlagValues values(:loginID, :loginFlag, :flagValue)");
    q.bindValue(":loginID",     userName);
    q.bindValue(":loginFlag",   flagName);
    q.bindValue(":flagValue",   val);
    q.exec();
}

/**
 * importLoginTypes()
 *
 * Imports the login types and billable items from the SeanetBillables.csv
 * file.
 */
void importLoginTypes()
{
    CSVParser   parser;
    int         *ratePlans = NULL;
    int         planCount = 0;
    
    QSqlDbPool      dbconn1;
    QSqlDbPool      dbconn2;
    QSqlDatabase    *db1 = dbconn1.qsqldb();
    QSqlDatabase    *db2 = dbconn2.qsqldb();
    if (!db1->isOpen() || !db2->isOpen()) {
        fprintf(stderr, "The database connection is not open\n");
        exit(-1);
    } /*else {
        QStringList tables = db1->tables();
        for (unsigned int i = 0; i < tables.count(); i++) {
            QString tmpStr = tables[i];
            printf("Table: %s\n", tmpStr.ascii());
        }
    }*/


    // Get the list of rate plans
    QSqlQuery   q(db1);
    if (!q.exec("select InternalID from RatePlans")) {
        fprintf(stderr, "Error loading rate plan list\n");
        exit(-1);
    }
    if (q.size() < 1) {
        fprintf(stderr, "No rate plans have been defined.\n");
        exit(-1);
    }
    planCount = q.size();
    ratePlans = new int[planCount+1];
    int curPlan = 0;
    while(q.next()) {
        ratePlans[curPlan] = q.value(0).toInt();
        curPlan++;
    }
    
    // Get the list of billing cycles
    if (!billingCycleList.count()) {
        fprintf(stderr, "No billing cycles have been defined.\n");
        exit(-1);
    }
    
    // Open our CSV file
    if (!parser.openFile("SeanetBillables.csv", true)) {
        fprintf(stderr, "Unable to open SeanetBillables.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    //int loginTypeCol    = parser.header().findIndex("ServiceType");
    int billableCol     = parser.header().findIndex("BillableItem");
    int descriptionCol  = parser.header().findIndex("PlanName");
    //int classCol        = parser.header().findIndex("Category");
    int priceCol        = parser.header().findIndex("monthlyFee");

    //int loginTypeIns = 0;
    int billablesIns = 0;
    int billablePricesIns = 0;

    QSqlCursor  logins("LoginTypes", true, db1);
    QSqlCursor  billables("Billables", true, db1);
    QSqlCursor  billablesData("BillablesData", true, db1);
    QSqlRecord  *buf;
    while(parser.loadRecord()) {
        /*
        if (parser.row()[loginTypeCol] != NULL) {
            buf = logins.primeInsert();
            buf->setValue("InternalID",     0);
            buf->setValue("LoginType",      parser.row()[loginTypeCol]);
            buf->setValue("Description",    parser.row()[descriptionCol]);
            buf->setValue("LoginClass",     parser.row()[classCol]);
            loginTypeIns += logins.insert();
        }
        */

        // Prepare for a new billable to be inserted.
        if (parser.row()[billableCol] != NULL) {
            buf = billables.primeInsert();
            buf->setValue("ItemNumber",     0);
            buf->setValue("ItemID",         parser.row()[billableCol]);
            buf->setValue("Description",    parser.row()[descriptionCol]);
            buf->setValue("ItemType",       1);
            buf->setValue("IntAccountNo",   17);
            buf->setValue("Priority",       10);
            buf->setValue("Active",         1);
            billablesIns += billables.insert();
            // Do another query to get the last insert id
            q.exec("select last_insert_id()");
            q.next();
            int itemNo = q.value(0).toInt();
            //fprintf(stderr, "Inserted billable item number %ld\n", itemNo);

            // Now, walk through each of the rate plans and billing
            // cycles and add the pricing data.
            for (int r = 0; r < planCount; r++) {
                billingCycleRecord  *cycle;
                for (uint b = 0; b < billingCycleList.count(); b++) {
                    cycle = billingCycleList.at(b);
                    int rowCount = 0;
                    QString tmpPrice = parser.row()[priceCol];
                    float   actPrice;
                    tmpPrice.replace('$', "");
                    actPrice = tmpPrice.toFloat();
                    actPrice = actPrice * cycle->period;
                    QString units = "Month";
                    if (cycle->period == 3) units = "Quarter";
                    if (cycle->period == 6) units = "Semi-Annual";
                    if (cycle->period == 12) units = "Annual";
                    buf = billablesData.primeInsert();
                    buf->setValue("InternalID",     0);
                    buf->setValue("ItemNumber",     itemNo);
                    buf->setValue("RatePlanID",     ratePlans[r]);
                    buf->setValue("CycleID",        (uint)cycle->cycleID);
                    buf->setValue("Description",    "");
                    buf->setValue("Price",          actPrice);
                    buf->setValue("SecondaryPrice", actPrice);
                    buf->setValue("Units",          units);
                    rowCount = billablesData.insert();
                    billablePricesIns += rowCount;
                    //fprintf(stderr, "r[%d] = %d, b[%d] = %d, rowCount = %d\n", r, ratePlans[r], b, billingCycles[b], rowCount);
                    if (!rowCount) {
                        QSqlError lErr = db1->lastError();
                        fprintf(stderr, "Error inserting billable item pricing\n");
                        fprintf(stderr, "driver: '%s'\n", lErr.driverText().ascii());
                        fprintf(stderr, "database: '%s'\n", lErr.databaseText().ascii());
                        fprintf(stderr, "last query: '%s'\n", billablesData.lastQuery().ascii());
                    }
                }
            }
                
        }

        char colTitle[1024];
        //fprintf(stderr, "Loaded a record with %d columns...\n", parser.row().count());
        for (unsigned int i = 0; i < parser.row().count(); i++) {
            if (i <= parser.header().count()) {
                QStringList tmpHeaders = parser.header();
                QString tmpHeader = tmpHeaders[i];
                strcpy(colTitle, tmpHeader.ascii());
            } else {
                sprintf(colTitle, "Column %d", i);
            }
            //printf("%20s: %s\n", colTitle, parser.row()[i].ascii());
        }
        //printf("\n");
    }

    // Now, walk through the login types we've just added and create
    // a billable item for each one we find.
    /*
     * We don't want to associate billables with login types anymore.
     * Severing that link will make it so the billable items that we insert
     * won't insert billable records for this customer, which is the only
     * way to make the seanet import "match" what was in the old Admiral
     * system.
    QSqlQuery   subq(dbconn2.qsqldb());
    q.exec("select InternalID, LoginType from LoginTypes");
    while (q.next()) {
        subq.prepare("select * from LoginTypeBillables where LoginTypeID = :loginTypeID");
        subq.bindValue(":loginTypeID", q.value(0).toInt());
        subq.exec();
        if (subq.size() == 0) {
            subq.prepare("insert into LoginTypeBillables values(0, :loginTypeID, (select ItemNumber from Billables where ItemID = :loginTypeDesc))");
            subq.bindValue(":loginTypeID", q.value(0).toInt());
            subq.bindValue(":loginTypeDesc", q.value(1));
            subq.exec();
        }
    }
    */

    // Login types are statically defined
    addLoginType("DialupStatic");
    addLoginTypeFlag("DialupStatic",    "Username");
    addLoginTypeFlag("DialupStatic",    "Password");
    addLoginTypeFlag("DialupStatic",    "VirtDomain");
    addLoginTypeFlag("DialupStatic",    "AutoAssign");
    addLoginTypeFlag("DialupStatic",    "DateAssigned");
    addLoginTypeFlag("DialupStatic",    "IPaddress");
    addLoginTypeFlag("DialupStatic",    "Netmask");
    addLoginTypeFlag("DialupStatic",    "MailType");

    addLoginType("DialupDynamic");
    addLoginTypeFlag("DialupDynamic",    "Username");
    addLoginTypeFlag("DialupDynamic",    "Password");
    addLoginTypeFlag("DialupDynamic",    "VirtDomain");
    addLoginTypeFlag("DialupDynamic",    "MailType");

    addLoginType("POPaccount");
    addLoginTypeFlag("POPaccount",    "Popname");
    addLoginTypeFlag("POPaccount",    "Password");
    addLoginTypeFlag("POPaccount",    "VirtDomain");
    addLoginTypeFlag("POPaccount",    "MailType");

    addLoginType("DomainSet");
    addLoginTypeFlag("DomainSet",    "SpamFilter");
    addLoginTypeFlag("DomainSet",    "VirtDomain");
    addLoginTypeFlag("DomainSet",    "MailType");

    addLoginType("VirtualSet");
    addLoginTypeFlag("VirtualSet",    "VirtUser");
    addLoginTypeFlag("VirtualSet",    "VirtDomain");
    addLoginTypeFlag("VirtualSet",    "VirtDest");

    addLoginType("DSLset");
    addLoginTypeFlag("DSLset",    "Provider");
    addLoginTypeFlag("DSLset",    "CircuitID");
    addLoginTypeFlag("DSLset",    "VPI");
    addLoginTypeFlag("DSLset",    "VCI");
    addLoginTypeFlag("DSLset",    "CPEname");
    addLoginTypeFlag("DSLset",    "RequirePVC");

    addLoginType("NailedSet");
    addLoginTypeFlag("NailedSet",    "NailedType");
    addLoginTypeFlag("NailedSet",    "ActivationDate");
    addLoginTypeFlag("NailedSet",    "RouteSwitch");
    addLoginTypeFlag("NailedSet",    "PortDLCI");
    addLoginTypeFlag("NailedSet",    "IPaddress");
    addLoginTypeFlag("NailedSet",    "Netmask");

    addLoginType("WebSet");
    addLoginTypeFlag("WebSet",    "WebServiceType");
    addLoginTypeFlag("WebSet",    "HostingUsername");
    addLoginTypeFlag("WebSet",    "HostingPassword");
    addLoginTypeFlag("WebSet",    "VirtDomain");
    addLoginTypeFlag("WebSet",    "VirtDomain2");

    printf("Imported %d BillableItems and %d Billable Prices for %d Rate Plans and %d Billing Cycles\n", billablesIns, billablePricesIns, planCount, billingCycleList.count());
}

/**
 * loadCustomers()
 *
 * Loads customer records from the SeanetPlansMain.csv file
 */
void loadCustomers()
{
    CSVParser   parser;
    
    customerRecord  *cust = NULL;

    QSqlDbPool      dbconn1;
    QSqlDbPool      dbconn2;
    QSqlDatabase    *db1 = dbconn1.qsqldb();
    QSqlDatabase    *db2 = dbconn2.qsqldb();
    if (!db1->isOpen() || !db2->isOpen()) {
        fprintf(stderr, "The database connection is not open\n");
        exit(-1);
    }

    // Open the CSV file
    if (!parser.openFile("PLANS_mainBillable.csv", true)) {
        fprintf(stderr, "Unable to open PLANS_mainBillable.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int regNumCol       = parser.header().findIndex("Regnum");
    int firstNameCol    = parser.header().findIndex("firstName");
    int middleNameCol   = parser.header().findIndex("middleName");
    int lastNameCol     = parser.header().findIndex("lastName");
    int companyCol      = parser.header().findIndex("Company");
    int addressCol      = parser.header().findIndex("Address");
    int cityCol         = parser.header().findIndex("cityName");
    int stateCol        = parser.header().findIndex("stateName");
    int zipCol          = parser.header().findIndex("zip");
    int countryCol      = parser.header().findIndex("countryName");
    int homePhoneCol    = parser.header().findIndex("HomePhone");
    int busPhoneCol     = parser.header().findIndex("BusPhone");
    int faxPhoneCol     = parser.header().findIndex("FaxPhone");
    int emailCol        = parser.header().findIndex("Email");
    int invoiceCol      = parser.header().findIndex("Invoice");
    //int statementCol    = parser.header().findIndex("Statement");
    int passwordCol     = parser.header().findIndex("Password");
    int debitCol        = parser.header().findIndex("DebitSum");
    int creditCol       = parser.header().findIndex("CreditSum");
    int paymentTypeCol  = parser.header().findIndex("PaymentType");
    int creditCardCol   = parser.header().findIndex("cardNumber");
    int expMonthCol     = parser.header().findIndex("ExpM");
    int expYearCol      = parser.header().findIndex("ExpYY");
    int svcPlanCol      = parser.header().findIndex("ServicePlan");
    int billPeriodCol   = parser.header().findIndex("BillingPeriod");
    int serviceStartCol = parser.header().findIndex("ServiceStart");
    int nextBillDateCol = parser.header().findIndex("NextBillDate");
    int closeDateCol    = parser.header().findIndex("SchClose");

    // Set our starting information
    QString lastRegnum = "";
    int     numCusts = 0;
    int     numLines = 0;
    int     subActive = 1;

    // Now walk through the rows.
    while (parser.loadRecord()) {
        numLines++;
        //if (!(numLines % 10)) fprintf(stderr, "\rProcessing line %d...", numLines);
        if (lastRegnum.compare(parser.row()[regNumCol])) {
            if (cust) {
                customerList.append(cust);
            }
            cust = new customerRecord;
            // We should save the current customer data here
            lastRegnum = parser.row()[regNumCol];
            numCusts++;

            // Prime the customer record
            cust->regNumber = parser.row()[regNumCol];
            cust->customerID = numCusts + STARTING_CUSTOMERID;

            // Set the contact name
            cust->contactName = "";
            if (parser.row()[firstNameCol].length()) {
                if (cust->contactName.length()) cust->contactName += " ";
                cust->contactName += parser.row()[firstNameCol];
            }
            if (parser.row()[middleNameCol].length()) {
                if (cust->contactName.length()) cust->contactName += " ";
                cust->contactName += parser.row()[middleNameCol];
            }
            if (parser.row()[lastNameCol].length()) {
                if (cust->contactName.length()) cust->contactName += " ";
                cust->contactName += parser.row()[lastNameCol];
            }

            fprintf(stderr, "\e[KLoading customer %s...\r", cust->contactName.ascii());

            // Set the fullname/company name
            cust->fullName = "";
            if (parser.row()[companyCol].length()) {
                if (cust->fullName.length()) cust->fullName += " ";
                cust->fullName += parser.row()[companyCol];
            }

            // Email address
            cust->email     = parser.row()[emailCol];

            // Set the address
            cust->street    = parser.row()[addressCol];
            cust->city      = parser.row()[cityCol];
            cust->state     = parser.row()[stateCol];
            cust->zip       = parser.row()[zipCol];
            cust->country   = parser.row()[countryCol];

            // Fill in their phone numbers
            cust->homePhone = parser.row()[homePhoneCol];
            cust->workPhone = parser.row()[busPhoneCol];
            cust->faxPhone  = parser.row()[faxPhoneCol];

            // Statement type
            cust->statementType = parser.row()[invoiceCol].toInt();

            // Password
            cust->password      = parser.row()[passwordCol];

            // Opening balance
            cust->currentBalance = parser.row()[debitCol].toFloat() - parser.row()[creditCol].toFloat();

            // Payment info
            cust->paymentType   = parser.row()[paymentTypeCol].toInt();
            // If they're a credit card customer, get that info too
            if (cust->paymentType == 3) {
                cust->creditCard    = parser.row()[creditCardCol];
                cust->creditCard.replace(QRegExp("[^0-9]"), "");
                cust->cardExpMonth  = parser.row()[expMonthCol].toInt();
                cust->cardExpYear   = parser.row()[expYearCol].toInt();
            }

            // A few dates.
            subActive = 1;
            QString tmpNextBill = parser.row()[nextBillDateCol];
            if (!tmpNextBill.length()) {
                tmpNextBill = QDate::currentDate().toString("MM/dd/yy");
                subActive = 0;
            }
            
            cust->accountOpened     = dateConvert(parser.row()[serviceStartCol]);
            cust->nextBillDate      = dateConvert(parser.row()[nextBillDateCol]);
            cust->billingPeriod     = parser.row()[billPeriodCol].toInt();
            cust->billingCycleID    = getBillingCycleID(cust->billingPeriod);
            if (!cust->billingCycleID) {
                fprintf(stderr, "\e[KUnable to find a billing cycle for %d month period.  Aborting.\n", cust->billingPeriod);
                exit(-1);
            }

            //fprintf(stderr, "Customer ID %ld %-50s\r", cust->customerID, cust->contactName.ascii());

            // Add the logins for this new customer record, if any.
            for (unsigned int i = 0; i < loginListFull.count(); i++) {
                loginRecord *login = loginListFull.at(i);
                if (login->regNumber == cust->regNumber) {
                    cust->loginList.append(login);
                }
            }
        }

        // At this point, regardless of whether or not we are on a
        // new customer, we should have a customer record.
        // Find the service we're looking for.
        QSqlQuery   q(db1);
        q.prepare("select ItemNumber, Description from Billables where Description LIKE :svcplan");
        q.bindValue(":svcplan", parser.row()[svcPlanCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            // Got one.  Create a new billable record.
            billableRecord  *billable = new billableRecord;
            billable->billableItemID = q.value(0).toInt();

            // Parse the date.
            billable->subscriptionActive = 1;
            QString tmpNextBill = parser.row()[nextBillDateCol];
            if (!tmpNextBill.length()) {
                tmpNextBill = QDate::currentDate().toString("MM/dd/yy");
                billable->subscriptionActive = 0;
            }
            billable->endsOn = tmpNextBill;

            billable->endsOnDate = dateConvert(billable->endsOn);
            billable->endsOnDate = billable->endsOnDate.addDays(-1);
            billable->lastDate = billable->endsOnDate.addMonths(-1 * cust->billingPeriod);
            billable->lastDate = billable->lastDate.addDays(1);
            billable->closeDate = parser.row()[closeDateCol];
            // Add the description since they won't be AutoPrice
            billable->description = parser.row()[svcPlanCol];
            cust->billableList.append(billable);
        } else {
            fprintf(stderr, "Unable to find match for service plan '%s' on line %d.  Skipping.\n", parser.row()[svcPlanCol].ascii(), numLines);
        }
        
    }
    // Save the last one we were working on.
    customerList.append(cust);

    fprintf(stderr, "\e[KParsed %d lines, loaded %d customer records into memory.\n", numLines, customerList.count());

    //printf("Customers:  Parsed %d lines, found %d customers.\n", numLines, numCusts);


}

/**
 * importCustomers()
 *
 * Walks through the in-memory customer list, assigns logins and
 * domains, and then saves them to the database by calling 
 * saveCustomer().
 */
void importCustomers()
{

    // Walk through the customer list and process the services they have,
    // replacing any placeholders with entries from the other lists we have
    // imported.
    for (unsigned int c = 0; c < customerList.count(); c++) {
        customerRecord  *cust = customerList.at(c);
        fprintf(stderr, "\e[KProcessing customer ID %d - '%s'...\r", cust->customerID, cust->contactName.ascii());
        // Save the customer in the database now.
        saveCustomer(cust);
    }
    fprintf(stderr, "\e[KProcessed %d customers.\n", customerList.count());

    /* FIXME:  Uncomment this block when ready to save to the database.
    QSqlDbPool  pooldb;
    //fprintf(stderr, "\rProcessing line %d...\n", numLines);
    fprintf(stderr, "Fixing dates...\r");
    QSqlQuery   sql(pooldb.qsqldb());
    sql.exec("update Subscriptions set EndsOn = '2009-02-28', LastDate = '2009-02-01'");
    sql.exec("update Customers set RatePlanDate = '2009-02-01', BillingCycleDate = '2009-02-01', LastBilled = '0000-00-00', LastStatementNo = '0000-00-00', AccountExpires = '0000-00-00', AccountOpened = '0000-00-00', AccountClosed = '0000-00-00', AccountReOpened = '0000-00-00', GraceDate = '0000-00-00', PromotionGiven = '0000-00-00'");
    */
}


/**
 * saveCustomer()
 *
 * Saves the data in a customerRecord into the database.
 */
void saveCustomer(customerRecord *cust)
{
    //fprintf(stderr, "\e[KSaving Customer ID %d %s\r", cust->customerID, cust->contactName.ascii());

    QSqlDbPool  custPool;
    QSqlDbPool  addrPool;
    
    QSqlCursor  customer("Customers", true, custPool.qsqldb());
    QSqlCursor  addr("Addresses", true, addrPool.qsqldb());
    QSqlRecord  *buf;

    QString     fullName = "";
    QString     contactName = "";
    QDate       cycleStartDate;
    QDate       cycleEndDate;
    if (!cust->altContact) cust->altContact = "";

    if (cust->fullName.length()) {
        fullName = cust->fullName;
        contactName = cust->contactName;
    } else {
        fullName = cust->contactName;
    }
    
    // Insert the customer record
    buf = customer.primeInsert();
    buf->setValue("CustomerID",         cust->customerID);
    buf->setValue("FullName",           fullName);
    buf->setValue("ContactName",        contactName);
    buf->setValue("AltContact",         cust->altContact);
    buf->setValue("PrimaryLogin",       "");
    buf->setValue("BillingAddress",     "Billing");
    buf->setValue("LastStatementNo",    0);
    buf->setValue("AccountExpires",     "1900-01-01");
    buf->setValue("SentToCollection",   0);
    buf->setValue("AgencyVendorNo",     0);
    buf->setValue("AddedBy",            0);
    buf->setValue("SalesRep",           0);
    buf->setValue("ReferredBy",         "");
    buf->setValue("AccountOpened",      cust->accountOpened.toString("yyyy-MM-dd").ascii());
    buf->setValue("AccountClosed",      cust->accountOpened.toString("yyyy-MM-dd").ascii());
    buf->setValue("AccountReOpened",    cust->accountOpened.toString("yyyy-MM-dd").ascii());
    buf->setValue("GraceDate",          timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("PromotionGiven",     timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("CurrentBalance",     cust->currentBalance);
    buf->setValue("CreditLimit",        cust->currentBalance);
    buf->setValue("RegNum",             cust->regNumber);
    if (cust->statementType == 3 || cust->statementType == 5) buf->setValue("PrintedStatement", 1);

    // Set the rate plan, billing cycle and terms
    buf->setValue("Terms",              1);     // FIXME
    buf->setValue("RatePlan",           1);     // FIXME
    buf->setValue("RatePlanDate",       cust->accountOpened.toString("yyyy-MM-dd").ascii());
    buf->setValue("BillingCycle",       (uint)cust->billingCycleID);
    buf->setValue("BillingCycleDate",   cust->accountOpened.toString("yyyy-MM-dd").ascii());
    buf->setValue("LastBilled",         timeToStr(rightNow(), YYYY_MM_DD)); // FIXME
    // Setup our billing dates.
    cycleStartDate = cust->nextBillDate;
    cycleStartDate = cycleStartDate.addMonths(-1 * cust->billingPeriod);
    cust->nextBillDate = cust->nextBillDate.addDays(1);
    buf->setValue("NextStatementDate",  cust->nextBillDate.toString("yyyy-MM-dd").ascii());
    // Subtract 2 days from the next statement date and we'll have the end date.
    // subscriptions get run on the day before statements.
    cycleEndDate  = cust->nextBillDate.addDays(-2);
    buf->setValue("CycleStartDate", cycleStartDate.toString("yyyy-MM-dd").ascii());
    buf->setValue("CycleEndDate",   cycleEndDate.toString("yyyy-MM-dd").ascii());

    
    // Finally, insert the customer
    if (!customer.insert()) {
        QSqlError lErr = custPool.qsqldb()->lastError();
        fprintf(stderr, "Error inserting customer\n");
        fprintf(stderr, "driver: '%s'\n", lErr.driverText().ascii());
        fprintf(stderr, "database: '%s'\n", lErr.databaseText().ascii());
        fprintf(stderr, "last query: '%s'\n", customer.lastQuery().ascii());
    }

    // Add their billing address
    buf = addr.primeInsert();
    buf->setValue("InternalID",     0);
    buf->setValue("RefFrom",        0);
    buf->setValue("RefID",          (uint)cust->customerID);
    buf->setValue("Tag",            "Billing");
    buf->setValue("International",  0);
    buf->setValue("Address1",       cust->street);
    buf->setValue("Address2",       "");
    buf->setValue("City",           cust->city);
    buf->setValue("State",          cust->state);
    buf->setValue("ZIP",            cust->zip);
    buf->setValue("Country",        "");
    buf->setValue("PostalCode",     "");
    buf->setValue("LastModifiedBy", "IMPORT");
    buf->setValue("Active",         1);
    addr.insert();

    CustomerContactsDB  contact;

    if (cust->homePhone.length() > 0) {
        contact.clear();
        contact.setTag("Home");
        contact.setName(fullName);
        contact.setPhoneNumber(cust->homePhone);
        contact.setCustomerID(cust->customerID);
        contact.setEmailAddress(cust->email);
        contact.setSendStatements(1);
        contact.setActive(1);
        contact.insert();
    }
    if (cust->workPhone.length() > 0) {
        contact.clear();
        contact.setTag("Work");
        contact.setName(fullName);
        contact.setPhoneNumber(cust->workPhone);
        contact.setCustomerID(cust->customerID);
        contact.setEmailAddress(cust->email);
        contact.setSendStatements(1);
        contact.setActive(1);
        contact.insert();
    }
    if (cust->faxPhone.length() > 0) {
        contact.clear();
        contact.setTag("Fax");
        contact.setName(fullName);
        contact.setPhoneNumber(cust->faxPhone);
        contact.setCustomerID(cust->customerID);
        contact.setEmailAddress(cust->email);
        contact.setSendStatements(1);
        contact.setActive(1);
        contact.insert();
    }
    
    CustomersDB CDB;
    // Add the billables
    for (uint i = 0; i < cust->billableList.count(); i++) {
        billableRecord  *billRec = cust->billableList.at(i);
        CDB.get((long int)cust->customerID);
        SubscriptionsDB    sdb;
        sdb.setValue("InternalID",      0);
        sdb.setValue("CustomerID",      (long int)cust->customerID);
        sdb.setValue("LoginID",         CDB.getStr("PrimaryLogin"));
        sdb.setValue("Active",          billRec->subscriptionActive);
        sdb.setValue("PackageNo",       0);
        sdb.setValue("ItemNumber",      billRec->billableItemID);
        sdb.setValue("ItemDesc",        billRec->description.ascii());
        sdb.setValue("ParentID",        0);
        sdb.setValue("Quantity",        1.0);
        sdb.setValue("AutoPrice",       1);
        sdb.setValue("AutoRenew",       1);
        sdb.setValue("SetupCharged",    1);
        sdb.setValue("EndsOn",          billRec->endsOnDate.toString("yyyy-MM-dd").ascii());
        sdb.setValue("LastDate",        billRec->lastDate.toString("yyyy-MM-dd").ascii());
        sdb.ins();
        //fprintf(stderr, "Subscription ends on date = '%s'\n", billRec->endsOnDate.toString("YYYY-MM-DD").ascii());
    }

    // If they have an opening balance, create an AR charge for them.
    if (cust->currentBalance > 0.0 && !doHistoricalData) {
        AcctsRecv   AR;
        AR.setGLAccount(SALESACCOUNT);
        CDB.get((long int)cust->customerID);
        AR.ARDB->setValue("CustomerID",     (long int) cust->customerID);
        AR.ARDB->setValue("LoginID",        CDB.getStr("PrimaryLogin"));
        AR.ARDB->setValue("ItemID",         0);
        AR.ARDB->setValue("Quantity",       1.0);
        AR.ARDB->setValue("Price",          cust->currentBalance);
        AR.ARDB->setValue("Amount",         cust->currentBalance);
        AR.ARDB->setValue("TransDate",      timeToStr(rightNow(), YYYY_MM_DD));
        AR.ARDB->setValue("StartDate",      timeToStr(rightNow(), YYYY_MM_DD));
        AR.ARDB->setValue("EndDate",        timeToStr(rightNow(), YYYY_MM_DD));
        AR.ARDB->setValue("Memo",           "Transferred balance");
        AR.SaveTrans();
    }

    // If they have a credit card, create the AutoPayments entry
    if (cust->creditCard.length()) {
        AutoPaymentsDB      APDB;
        APDB.setValue("CustomerID",         (long)cust->customerID);
        APDB.setValue("AuthorizedOn",       timeToStr(rightNow(), YYYY_MM_DD));
        APDB.setValue("AuthorizedBy",       "IMPORT");
        APDB.setValue("EnteredBy",          "IMPORT");
        APDB.setValue("Active",             1);
        APDB.setValue("TransDate",          timeToStr(rightNow(), YYYY_MM_DD));
        APDB.setValue("ChargeDay",          0); // The day of the statement
        APDB.setValue("Name",               fullName);
        APDB.setValue("Address",            cust->street);
        APDB.setValue("ZIP",                cust->zip);
        APDB.setValue("AuthName",           fullName);
        APDB.setValue("PaymentType",        1);
        int cardType = CCTYPE_UNKNOWN;
        QString tmpCard;
        tmpCard = cust->creditCard.left(2);
        if (tmpCard.toInt() >= 51 && tmpCard.toInt() <= 55) {
            cardType = CCTYPE_MC;
        }

        tmpCard = cust->creditCard.left(1);
        if (tmpCard.toInt() == 4) {
            cardType = CCTYPE_VISA;
        }

        tmpCard = cust->creditCard.left(2);
        if (tmpCard.toInt() == 34 || tmpCard.toInt() == 37) {
            cardType = CCTYPE_MC;
        }

        tmpCard = cust->creditCard.left(4);
        if (tmpCard.toInt() == 6011) {
            cardType = CCTYPE_DISC;
        }

        APDB.setValue("CardType",       cardType);
        
        if (cardType == CCTYPE_UNKNOWN) {
            fprintf(stderr, "\nUnknown credit card type for customer ID %d\n", cust->customerID);
        }

        // Fix the date.
        QDate   tmpDate;
        tmpDate.setYMD(cust->cardExpYear, cust->cardExpMonth, 1);
        tmpDate.setYMD(cust->cardExpYear, cust->cardExpMonth, tmpDate.daysInMonth());
        
        APDB.setValue("ExpDate",    tmpDate.toString("yyyy-MM-dd"));
        APDB.setValue("AcctNo",     cust->creditCard);
        APDB.ins();
    
    }

    // Add the logins
    for (uint i = 0; i < cust->loginList.count(); i++) {
        loginRecord   *loginRec = cust->loginList.at(i);
        LoginsDB    ldb;
        ldb.setValue("CustomerID",      (long int)cust->customerID);
        ldb.setValue("LoginID",         loginRec->userName);
        ldb.setValue("LoginType",       loginRec->loginTypeID);
        ldb.setValue("ContactName",     fullName.ascii());
        ldb.setValue("DiskSpace",       0);
        ldb.setValue("DialupChannels",  0);
        ldb.setValue("Active",          1);
        ldb.setValue("LastModified",    timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
        ldb.ins();
        ldb.setValue("Active",          1);
        ldb.upd(0);
        CDB.get((long int)cust->customerID);
        if (!strlen((const char *) CDB.getStr("PrimaryLogin")) ||
                !strncmp(CDB.getStr("PrimaryLogin"), "W", 1)) {
            CDB.setValue("PrimaryLogin", loginRec->userName);
            CDB.upd();
        }
        // Set the flags for this user
        // MARC
        if (loginRec->serviceType == "DialupDynamic") {
            setLoginFlag(loginRec->userName, "Username",     loginRec->userName);
            setLoginFlag(loginRec->userName, "Password",     loginRec->password);
            setLoginFlag(loginRec->userName, "VirtDomain",   loginRec->virtDomain);
            setLoginFlag(loginRec->userName, "MailType",     loginRec->mailType);
        } else if (loginRec->serviceType == "DialupStatic") {
            setLoginFlag(loginRec->userName, "Username",     loginRec->userName);
            setLoginFlag(loginRec->userName, "Password",     loginRec->password);
            setLoginFlag(loginRec->userName, "VirtDomain",   loginRec->virtDomain);
            setLoginFlag(loginRec->userName, "MailType",     loginRec->mailType);
            setLoginFlag(loginRec->userName, "AutoAssign",   loginRec->autoAssign);
            setLoginFlag(loginRec->userName, "DateAssigned", loginRec->dateAssigned);
            setLoginFlag(loginRec->userName, "IPaddress",    loginRec->ipAddr);
            setLoginFlag(loginRec->userName, "Netmask",      loginRec->netmask);
        } else if (loginRec->serviceType == "DSLset") {
            setLoginFlag(loginRec->userName, "Provider",     loginRec->provider);
            setLoginFlag(loginRec->userName, "CircuitID",    loginRec->circuitID);
            setLoginFlag(loginRec->userName, "VPI",          loginRec->vpi);
            setLoginFlag(loginRec->userName, "VCI",          loginRec->vci);
            setLoginFlag(loginRec->userName, "CPEname",      loginRec->cpeName);
            setLoginFlag(loginRec->userName, "RequirePVC",   loginRec->requirePVC);
        } else if (loginRec->serviceType == "NailedSet") {
            setLoginFlag(loginRec->userName, "NailedType",   loginRec->nailedType);
            setLoginFlag(loginRec->userName, "ActivationDate", loginRec->serviceStart);
            setLoginFlag(loginRec->userName, "RouteSwitch",  loginRec->routeSwitch);
            setLoginFlag(loginRec->userName, "PortDLCI",     loginRec->portDLCI);
            setLoginFlag(loginRec->userName, "IPaddress",    loginRec->ipAddr);
            setLoginFlag(loginRec->userName, "Netmask",      loginRec->netmask);
        } else if (loginRec->serviceType == "DomainSet") {
            setLoginFlag(loginRec->userName, "VirtDomain",   loginRec->virtDomain);
            setLoginFlag(loginRec->userName, "SpamFilter",   loginRec->spamFilter);
            setLoginFlag(loginRec->userName, "MailType",     loginRec->mailType);
        } else if (loginRec->serviceType == "VirtualSet") {
            setLoginFlag(loginRec->userName, "VirtDomain",   loginRec->virtDomain);
            setLoginFlag(loginRec->userName, "VirtUser",     loginRec->virtUser);
            setLoginFlag(loginRec->userName, "VirtDest",     loginRec->virtDest);
        } else if (loginRec->serviceType == "WebSet") {
            setLoginFlag(loginRec->userName, "WebServiceType",  loginRec->webSvcType);
            setLoginFlag(loginRec->userName, "HostingUsername", loginRec->hostingUserName);
            setLoginFlag(loginRec->userName, "HostingPassword", loginRec->password);
            setLoginFlag(loginRec->userName, "VirtDomain",      loginRec->virtDomain);
            if (loginRec->virtDomain2.length() > 0) setLoginFlag(loginRec->userName, "VirtDomain2",      loginRec->virtDomain2);
        } else if (loginRec->serviceType == "POPaccount") {
            setLoginFlag(loginRec->userName, "Popname",      loginRec->userName);
            setLoginFlag(loginRec->userName, "Password",     loginRec->password);
            setLoginFlag(loginRec->userName, "VirtDomain",   loginRec->virtDomain);
            setLoginFlag(loginRec->userName, "MailType",     loginRec->mailType);
        }
        //CDB.doSubscriptions();
        //ldb.addSubscriptions();
    }
}

/**
 * dateConvert()
 *
 * Converts a date in m/d/yyyy format to a QDate object.
*/

const QDate dateConvert(const QString src)
{
    QString     delim = "/";
    QStringList parts;
    int         y = 0, m = 0, d = 0;
    char        workStr[1024];

    parts = QStringList::split(delim, src);
    m = parts[0].toInt();
    d = parts[1].toInt();
    y = parts[2].toInt();
    
    sprintf(workStr, "%04d-%02d-%02d", y, m, d);
    return QDate::fromString(workStr, Qt::ISODate);
}

/**
 * loadCustomerCharges()
 *
 * Loads the charges for the customer from seanet_charges.csv
 * into Accounts Receivables.
 */
void loadCustomerCharges()
{
    CSVParser   parser;
    if (!parser.openFile("seanet_charges.csv", true)) {
        fprintf(stderr, "Unable to open seanet_charges.csv, skipping.\n");
        return;
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("Regnum");
    int amountCol           = parser.header().findIndex("amount");
    int startDateCol        = parser.header().findIndex("dateFrom");
    int endDateCol          = parser.header().findIndex("dateTo");
    int idInvoiceCol        = parser.header().findIndex("idInvoice");
    int memoCol             = parser.header().findIndex("serviceTypeName");

    QString         lastRegnum = "";
    int             numLines = 0;
    long            custID = 0;
    ADB             DB;
    QStringList     tmpParts;
    QDate           startDate;
    QDate           endDate;
    float           curProg = 0.00;

    // Now walk through the rows.
    fprintf(stderr, "Processing customer charges...\n");
    while (parser.loadRecord()) {
        numLines++;
        //if (!(numLines % 10)) fprintf(stderr, "\rProcessing line %d...", numLines);
        if (lastRegnum.compare(lastRegnum, parser.row()[regNumberCol])) {
            lastRegnum = parser.row()[regNumberCol];
            DB.query("select CustomerID from Customers where RegNum = '%s'", parser.row()[regNumberCol].ascii());
            if (DB.rowCount) {
                if (custID) {
                    fprintf(stderr, "\rProcessing charges line %d...Customer ID %ld - %.2f...\e[K", numLines, custID, curProg);
                }
                DB.getrow();
                custID = atoi(DB.curRow["CustomerID"]);
                curProg = 0.00;
            } else {
                custID = 0;
                curProg = 0.00;
            }
        }

        if (custID) {
            curProg += parser.row()[amountCol].toFloat();
            // Parse the dates.
            if (parser.row()[startDateCol].length()) {
                tmpParts = tmpParts.split(" ", parser.row()[startDateCol]);
                myDateToQDate(tmpParts[0].ascii(), startDate);
            }
            if (parser.row()[endDateCol].length()) {
                tmpParts = tmpParts.split(" ", parser.row()[endDateCol]);
                myDateToQDate(tmpParts[0].ascii(), endDate);
            }

            AcctsRecv   AR;
            AR.setGLAccount(SALESACCOUNT);
            AR.ARDB->setValue("CustomerID",     custID);
            AR.ARDB->setValue("LoginID",        "");
            AR.ARDB->setValue("ItemID",         0);
            AR.ARDB->setValue("Quantity",       1.0);
            AR.ARDB->setValue("Price",          parser.row()[amountCol].toFloat());
            AR.ARDB->setValue("Amount",         parser.row()[amountCol].toFloat());
            AR.ARDB->setValue("TransDate",      startDate.toString("yyyy-MM-dd").ascii());
            AR.ARDB->setValue("StartDate",      startDate.toString("yyyy-MM-dd").ascii());
            AR.ARDB->setValue("EndDate",        endDate.toString("yyyy-MM-dd").ascii());
            AR.ARDB->setValue("StatementNo",    parser.row()[idInvoiceCol].toInt());
            AR.ARDB->setValue("Memo",           parser.row()[memoCol].ascii());
            AR.SaveTrans();
        }
    }
}

/**
 * loadCustomerPayments()
 *
 * Loads the payments for the customer from seanet_payments.csv
 * into Accounts Receivables.
 */
void loadCustomerPayments()
{
    CSVParser   parser;
    if (!parser.openFile("seanet_payments.csv", true)) {
        fprintf(stderr, "Unable to open seanet_payments.csv, skipping.\n");
        return;
    }

    // Get the index of certain columns from our header row.
    int regNumberCol        = parser.header().findIndex("Regnum");
    int amountCol           = parser.header().findIndex("Expr1");
    int recvDateCol         = parser.header().findIndex("receiveDate");
    int refNumCol           = parser.header().findIndex("refNumber");
    int methodCol           = parser.header().findIndex("PaymentType");

    QString         lastRegnum = "";
    int             numLines = 0;
    long            custID = 0;
    ADB             DB;
    QStringList     tmpParts;
    QDate           paymentDate;
    QString         memo;
    long            statementNo = 0;
    float           curProg = 0.00;

    // Now walk through the rows.
    fprintf(stderr, "Processing customer payments...\n");
    while (parser.loadRecord()) {
        numLines++;
        //if (!(numLines % 10)) fprintf(stderr, "\rProcessing line %d...", numLines);
        if (lastRegnum.compare(lastRegnum, parser.row()[regNumberCol])) {
            lastRegnum = parser.row()[regNumberCol];
            DB.query("select CustomerID from Customers where RegNum = '%s'", parser.row()[regNumberCol].ascii());
            if (DB.rowCount) {
                if (custID) {
                    fprintf(stderr, "\rProcessing payments line %d...Customer ID %ld - %.2f...\e[K", numLines, custID, curProg);
                }
                DB.getrow();
                custID = atoi(DB.curRow["CustomerID"]);
                curProg = 0.00;
            } else {
                custID = 0;
                curProg = 0.00;
            }
        }

        if (custID) {
            curProg += parser.row()[amountCol].toFloat();
            statementNo = 0;
            // Parse the dates.
            if (parser.row()[recvDateCol].length()) {
                tmpParts = tmpParts.split(" ", parser.row()[recvDateCol]);
                myDateToQDate(tmpParts[0].ascii(), paymentDate);
                // Now grab the date from the database of the first
                // billable with a statement that follows this the date
                // of this transaction.
                DB.query("select StatementNo from AcctsRecv where CustomerID = %ld and TransType <> 2 and TransDate > '%s' limit 1", custID, paymentDate.toString("yyyy-MM-dd").ascii());
                if (DB.rowCount) {
                    DB.getrow();
                    statementNo = atol(DB.curRow["StatementNo"]);
                }
            }

            memo = "Payment Received";
            if (parser.row()[methodCol].length()) {
                if (parser.row()[methodCol].compare(parser.row()[methodCol], "N/A")) {
                    memo += " (";
                    memo += parser.row()[methodCol];
                    if (parser.row()[methodCol].compare(parser.row()[methodCol], "Credit Card")) {
                        memo += " #";
                        memo += parser.row()[refNumCol];
                    };
                    memo += ")";
                }
            }
            AcctsRecv   AR;
            AR.setGLAccount(SALESACCOUNT);
            AR.ARDB->setValue("CustomerID",     custID);
            AR.ARDB->setValue("TransType",      TRANSTYPE_PAYMENT);
            AR.ARDB->setValue("Price",          parser.row()[amountCol].toFloat() * -1.0);
            AR.ARDB->setValue("Amount",         parser.row()[amountCol].toFloat() * -1.0);
            AR.ARDB->setValue("TransDate",      paymentDate.toString("yyyy-MM-dd").ascii());
            AR.ARDB->setValue("StatementNo",    statementNo);
            AR.ARDB->setValue("Memo",           memo.ascii());
            AR.SaveTrans();
        }
    }
}

