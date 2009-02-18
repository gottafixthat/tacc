/**
 * taccupgrade.cpp
 *
 * Checks the database schema and upgrades the database to the
 * latest version.
 */

#include <TAATools.h>
#include <qapplication.h>
#include <QSqlDbPool.h>
#include <qsqlcursor.h>
#include <qsqlrecord.h>
#include <Cfg.h>

#include "taccupgrade.h"


int main( int argc, char ** argv )
{
    // Load the main config file and setup our QApplication.
    loadTAAConfig();
    QApplication    a( argc, argv );

    // Setup the database pool
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

    int     vers = schemaVersion();
    printf("Current schema version is %d, upgrading to version %d...\n", vers, SCHEMA_VERSION_REQUIRED);

    if (vers < 1) {
        printf("Creating SchemaVersion table and upgrading to version 1...\n");
        QSqlQuery   q(dbp.qsqldb());
        QString     sql;
        sql = "create table if not exists SchemaVersion (SchemaVersion int NOT NULL DEFAULT '0', PRIMARY KEY(SchemaVersion))";
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
        QSqlCursor  contacts("CustomerContacts", true, dbp2.qsqldb());
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
}

