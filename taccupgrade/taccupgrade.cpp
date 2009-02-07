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
    if (!schemaValid()) {
        upgradeDatabase();
        fprintf(stderr, "\nDone\n");
    } else {
        fprintf(stderr, "The TACC schema version is up to date.\n");
    }
}

/**
 * schemaVersion()
 *
 * Gets the current schema version from the database.
 */
int schemaVersion()
{
    QSqlDbPool  dbp;

    QSqlQuery   q(dbp.qsqldb());

    if (!q.exec("select SchemaVersion from SchemaVersion")) {
        // If we can't query it, return false.
        return 0;
    };
    if (q.size() < 1) {
        // No records or an error, return false
        return 0;
    }
    q.next();
    if (q.value(0).toInt() < SCHEMA_VERSION_REQUIRED) {
        // We need a newer schema
        return 0;
    }

    // Schema version is okay, return true
    return q.value(0).toInt();
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
        
    }
}

