/**
 * taccupgrade.cpp
 *
 * Checks the database schema and upgrades the database to the
 * latest version.
 */

#include <TAATools.h>
#include <QtCore/QCoreApplication>
#include <QSqlDbPool.h>
#include <Qt3Support/q3sqlcursor.h>
#include <QtSql/QSqlRecord>
#include <QtCore/QList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <ADB.h>
#include <Cfg.h>

#include "taccupgrade.h"


int main( int argc, char ** argv )
{
    // Load the main config file and setup our QApplication.
    loadTAAConfig();
    QCoreApplication    a( argc, argv );

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

    // Import the login types and billable items.
    //importLoginTypes();
    upgradeDatabase();
    /*
    if (!schemaValid()) {
        upgradeDatabase();
        fprintf(stderr, "\nDone\n");
    } else {
        fprintf(stderr, "The TACC schema version is up to date.\n");
    }
    */
}

/**
 * upgradeError()
 *
 * Aborts the execution of the program if there is an error.
 */
void upgradeError(const QString &err, const QString &sql)
{
    fprintf(stderr, "\nError performing upgrade.  Perhaps a db permission problem?\n");
    fprintf(stderr, "Last Error:  '%s'\n\n", err.ascii());
    fprintf(stderr, "Last Query:  '%s'\n\n", sql.ascii());
    exit(-1);
}

/**
 * upgradeDatabase()
 *
 * Upgrades the database based on the schema version it finds.
 */
void upgradeDatabase()
{
    QSqlDbPool  dbp;
    ADB         db;

    int     vers = schemaVersion();
    printf("Current schema version is %d, upgrading to version %d...\n", vers, SCHEMA_VERSION_REQUIRED);

    if (vers < 1) {
        printf("Creating SchemaVersion table and upgrading to version 1...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;
        sql = "create table if not exists SchemaVersion (SchemaVersion int NOT NULL DEFAULT '0', PRIMARY KEY(SchemaVersion))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlags add column BaseType int NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlags add column DefaultValue blob NOT NULL";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlags add column UserDefined int NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "CREATE TABLE LoginFlagValues (LoginID varchar(32) NOT NULL DEFAULT '', LoginFlag varchar(32) NOT NULL DEFAULT '', FlagValue text NOT NULL DEFAULT '', PRIMARY KEY(LoginID, LoginFlag))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column FullName FullName varchar(80) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column ContactName ContactName varchar(80) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AltContact AltContact varchar(80) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column BillingAddress BillingAddress varchar(32) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column RatePlanDate RatePlanDate date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column BillingCycleDate BillingCycleDate date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column Terms Terms int(11) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column CreditLimit CreditLimit float(8,2) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column LastBilled LastBilled date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column LastStatementNo LastStatementNo bigint(21) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AccountExpires AccountExpires date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AgencyVendorNo AgencyVendorNo int(11) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AddedBy AddedBy int(11) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column SalesRep SalesRep int(11) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column ReferredBy ReferredBy varchar(80) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AccountOpened AccountOpened date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AccountClosed AccountClosed date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column AccountReOpened AccountReOpened date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column GraceDate GraceDate date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column WelcomeLetterSent WelcomeLetterSent int(11) NOT NULL DEFAULT '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column PromotionGiven PromotionGiven date NOT NULL DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "delete from SchemaVersion";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());
        
        sql = "insert into SchemaVersion values(1)";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());
        
        vers = 1;
    }

    if (vers < 2) {
        printf("Updating to Schema Version 2...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table Logins change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Subscriptions change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table AcctsRecv change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table CallLog change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers change column PrimaryLogin PrimaryLogin varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table ModemUsage change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table TrafficUsage change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Notes change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table UserNotes change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table VoiceMail change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table UserActivities change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlags change column LoginFlag LoginFlag varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlagValues change column LoginID LoginID varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table LoginFlagValues change column LoginFlag LoginFlag varchar(64) NOT NULL DEFAULT ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 2";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());
        
        vers = 2;
    }

    if (vers < 3) {
        printf("Updating to Schema Version 3...\n");
        QSqlDbPool  dbp2;
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "drop table if exists Contacts";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "drop table if exists CustomerContacts";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "create table CustomerContacts (ContactID bigint(21) NOT NULL auto_increment, CustomerID  bigint(21) NOT NULL DEFAULT '0', Tag varchar(64) DEFAULT '', Name varchar(64) DEFAULT '', Access varchar(64) DEFAULT '', International tinyint(4) DEFAULT '0', PhoneNumber varchar(32) DEFAULT '', EmailAddress varchar(64) DEFAULT '', Active tinyint(4) DEFAULT '1', Flags int(11) DEFAULT '0', LastModifiedBy varchar(64) DEFAULT '', LastModified timestamp DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY(ContactID), INDEX CustomerIDIDX (CustomerID))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        // Import the current phone numbers into the new Contacts table.
        Q3SqlCursor  contacts("CustomerContacts", true, dbp2.qsqldb());
        QSqlRecord  *buf;
        sql = "select RefID, Tag, International, PhoneNumber, Active, LastModifiedBy, LastModified from PhoneNumbers where RefFrom = 0";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());
        if (q.size()) while (q.next()) {
            buf = contacts.primeInsert();
            buf->setValue("ContactID",      0);
            buf->setValue("CustomerID",     q.value(0).toInt());
            buf->setValue("Tag",            q.value(1));
            buf->setValue("International",  q.value(2).toInt());
            buf->setValue("PhoneNumber",    q.value(3));
            buf->setValue("Active",         q.value(4).toInt());
            buf->setValue("LastModifiedBy", q.value(5));
            buf->setValue("LastModified",   q.value(6));
            contacts.insert();
        }

        sql = "update SchemaVersion set SchemaVersion = 3";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());
        
        vers = 3;
    }

    if (vers < 4) {
        printf("Updating to Schema Version 4...\n");
        QSqlDbPool  dbp2;
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table BillingCycles add column CycleType enum('Day','Anniversary') DEFAULT 'Day'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table BillingCycles add column AnniversaryPeriod int(11) DEFAULT '1'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 4";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 4;
    }

    if (vers < 5) {
        printf("Updating to Schema Version 5...\n");
        QSqlDbPool  dbp2;
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table Customers add column CycleStartDate date default '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers add column CycleEndDate date default '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers add column NextStatementDate date DEFAULT '0000-00-00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 5";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 5;
    }

    if (vers < 6) {
        printf("Updating to Schema Version 6...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table CustomerContacts add column SendStatements int(11) default '0'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 6";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 6;
    }

    if (vers < 7) {
        printf("Updating to Schema Version 7...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "drop table `Triggers`";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "drop table `TAProcInfo`";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Customers add column RegNum varchar(255) default ''";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table CCTrans add index DateTypeStatusIDX(TransDate,CardType,MCVE_Status)";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 7";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 7;
    }

    if (vers < 8) {
        printf("Updating to Schema Version 8...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;
        int         ufAcct = atoi(cfgVal("UndepositedFundsAccount")) * 10;
        int         coAcct = atoi(cfgVal("CollectionsAccount")) * 10;
        int         acctNoList[8];
        int         acctMap[16];
        acctNoList[0]   = 1000;     // Asset
        acctNoList[1]   = 2000;     // Liability
        acctNoList[2]   = 3000;     // Equity
        acctNoList[3]   = 4000;     // Income
        acctNoList[4]   = 5000;     // Cost of Goods Sold
        acctNoList[5]   = 6000;     // Expense
        acctNoList[6]   = 7000;     // Other Income
        acctNoList[7]   = 8000;     // Other Expense

        acctMap[0]      = 0;    // Bank -> Asset
        acctMap[1]      = 0;    // Accts Recv -> Asset
        acctMap[2]      = 0;    // Other Current Asset -> Asset
        acctMap[3]      = 0;    // Fixed Asset -> Asset
        acctMap[4]      = 0;    // Other Asset -> Asset
        acctMap[5]      = 1;    // Accounts Payable
        acctMap[6]      = 1;    // Credit Card
        acctMap[7]      = 1;    // Other Current Liability
        acctMap[8]      = 1;    // Long Term Liability
        acctMap[9]      = 2;    // Capital/Equity
        acctMap[10]     = 3;    // Income
        acctMap[11]     = 4;    // Cost of Goods Sold
        acctMap[12]     = 5;    // Expense
        acctMap[13]     = 6;    // Other Income
        acctMap[14]     = 7;    // Other Expense

        printf("Updating General Ledger account types...\n");
        sql = "update GL set AccountNo = AccountNo * 10";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update Accounts set AccountNo = AccountNo * 10";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        // Now, get all of the accounts in the chart of accounts.
        sql = "select AccountNo, AcctType from Accounts";
        if (!q.exec(sql) || !q.size()) upgradeError(q.lastError().databaseText(), q.lastQuery());
        else while (q.next()) {
            int newAcctType = acctMap[q.value(1).toInt()];
            int newAcctNo = acctNoList[newAcctType];
            acctNoList[newAcctType]++;
            
            db.dbcmd("update Accounts set AccountNo = %d, AcctType = %d where AccountNo = %d", newAcctNo, newAcctType, q.value(0).toInt());
            db.dbcmd("update GL set AccountNo = %d where AccountNo = %d", newAcctNo, q.value(0).toInt());
            // Find our undeposited funds account
            if (q.value(0).toInt() == ufAcct) {
                ufAcct = newAcctNo;
            }
            // Find our undeposited funds account
            if (q.value(0).toInt() == coAcct) {
                coAcct = newAcctNo;
            }
        }

        // Update our settings with the new undeposted funds and collections accounts.
        db.dbcmd("delete from Settings where Token in ('CollectionsAccount', 'UndepositedFundsAcct')");
        db.dbcmd("insert into Settings values(0, 0, 0, 'CollectionsAccount', '%d')", coAcct);
        db.dbcmd("insert into Settings values(0, 0, 0, 'UndepositedFundsAcct', '%d')", ufAcct);

        sql = "update SchemaVersion set SchemaVersion = 8";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 8;
    }

    if (vers < 9) {
        printf("Updating to Schema Version 9...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table Accounts add primary key(AccountNo)";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 9";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 9;
    }

    if (vers < 10) {
        printf("Updating to Schema Version 10...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;
        ADB         db2;

        db.dbcmd("rename table Accounts to AccountsOld");

        db.dbcmd("drop table if exists Accounts");
        db.dbcmd("create table Accounts (IntAccountNo int(11) not null auto_increment, AccountNo varchar(64) not null, AcctName varchar(80) not null, ParentID int(11) default '0', AccountType int(11) not null, ProviderAccountNo varchar(80), TaxLine varchar(250), Balance decimal(20,2) default '0.00', TransCount bigint(21) default '0', PRIMARY KEY (IntAccountNo))");
        ADBTable acctsDB("Accounts");
        db.dbcmd("alter table GL change column AccountNo IntAccountNo int(11) not null");
        db.dbcmd("alter table GL change column Amount Amount decimal(20,2) not null");
        db.dbcmd("alter table Billables change column AccountNo IntAccountNo int(11) not null");
        db.dbcmd("alter table Packages add column IntAccountNo int(11) not null default '0'");

        // Load the old accounts table and move them to the new accounts table
        // and update the GL at the same time.
        int intAcctNo = 1;
        db.query("select * from AccountsOld");
        while(db.getrow()) {
            acctsDB.clearData();
            acctsDB.setValue("IntAccountNo",        intAcctNo);
            acctsDB.setValue("AccountNo",           db.curRow["AccountNo"]);
            acctsDB.setValue("AcctName",            db.curRow["AcctName"]);
            acctsDB.setValue("AccountType",         db.curRow["AcctType"]);
            acctsDB.setValue("ParentID",            db.curRow["SubAcctOf"]);
            acctsDB.setValue("ProviderAccountNo",   db.curRow["AcctNumber"]);
            acctsDB.setValue("TaxLine",             db.curRow["TaxLine"]);
            acctsDB.setValue("Balance",             db.curRow["Balance"]);
            acctsDB.setValue("TransCount",          db.curRow["TransCount"]);
            acctsDB.ins();
            db2.dbcmd("update GL set IntAccountNo = %d where IntAccountNo = %s", intAcctNo, db.curRow["AccountNo"]);
            db2.dbcmd("update Billables set IntAccountNo = %d where IntAccountNo = %s", intAcctNo, db.curRow["AccountNo"]);
            db2.dbcmd("update Accounts set Balance = (select sum(Amount) from GL where IntAccountNo = %d) where IntAccountNo = %d", intAcctNo, intAcctNo);
            db2.dbcmd("update Accounts set TransCount = (select count(InternalID) from GL where IntAccountNo = %d) where IntAccountNo = %d", intAcctNo, intAcctNo);
            intAcctNo++;
        }
        db.dbcmd("drop table if exists AccountsOld");

        // Create the new GL Account Types
        db.dbcmd("create table GLAccountTypes (AccountType int(11) not null auto_increment, Description varchar(80), PRIMARY KEY (AccountType))");
        db.dbcmd("insert into GLAccountTypes values(1,'Asset')");
        db.dbcmd("insert into GLAccountTypes values(2,'Liabilites & Equity')");
        db.dbcmd("insert into GLAccountTypes values(3,'Income')");
        db.dbcmd("insert into GLAccountTypes values(4,'Cost of Goods Sold')");
        db.dbcmd("insert into GLAccountTypes values(5,'Selling Expenses')");
        db.dbcmd("insert into GLAccountTypes values(6,'Administrative Expenses')");
        db.dbcmd("insert into GLAccountTypes values(7,'Other Income and Expense')");

        // Drop a couple of old tables while we're at it.
        db.dbcmd("drop table if exists Register");
        db.dbcmd("drop table if exists RegisterSplits");
        db.dbcmd("drop table if exists QBRegister");
        db.dbcmd("drop table if exists IQCart");

        sql = "update SchemaVersion set SchemaVersion = 10";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 10;
    }

    if (vers < 11) {
        printf("Creating SavedReports table and upgrading to version 11...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;
        sql = "create table if not exists SavedReports (SavedReportID   bigint(21) NOT NULL auto_increment, LoginID         varchar(64) NOT NULL, ReportName      varchar(64) NOT NULL, SaveName        varchar(255) NOT NULL, ReportData      text NOT NULL, PRIMARY KEY(SavedReportID), INDEX LoginNameIDX(LoginID, ReportName), INDEX SaveNameIDX(LoginID, ReportName, SaveName))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 11";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 11;
    }

    if (vers < 12) {
        printf("Updating Statements table...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "alter table Statements add column TotalOverdue decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Statements add column CurrentDue decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Statements add column Overdue decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Statements add column Overdue30 decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Statements add column Overdue60 decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "alter table Statements add column Overdue90 decimal(20,2) default '0.00'";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "update SchemaVersion set SchemaVersion = 12";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 12;
    }

    // Call Rates
    /*
    if (vers < 13) {
        printf("Creating VoIPCarrierRates table...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;

        sql = "create table VoIPCarriers (CarrierID bigint(21) not null auto_increment, CarrierName varchar(255) not null, termvers bigint(21) not null, origvers bigint(21) not null, tforigvers bigint(21) not null, primary key (CarrierID))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        sql = "create table VoIPCarrierRates (CarrierID bigint(21) NOT NULL, RateType enum('term','orig','tforig'), RatePlanVersion bigint(21) not null, npanxx varchar(10) not null, npa varchar(10) not null, nxx varchar(10) not null, locstate varchar(255) not null, lata varchar(255) not null, lataocn varchar(255) not null, lataname varchar(255) not null, ocn varchar(255) not null, ocnname varchar(255) not null, ocntype varchar(255) not null, interlatarate decimal (20,6) not null default '0.000000', intralatarate decimal(20,6) not null default '0.000000', lastupdated datetime not null, primary key(CarrierID, RateType, RatePlanVersion, npa, nxx, lata, lataocn))";
        if (!q.exec(sql)) upgradeError(q.lastError().databaseText(), q.lastQuery());

        vers = 13;
    }
    */

}

