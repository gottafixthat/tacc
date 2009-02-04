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

int main( int argc, char ** argv );


int main( int argc, char ** argv )
{
    // Load the main config file.
    loadTAAConfig();
    CSVParser       parser;
    QApplication    a( argc, argv );
    int *ratePlans = NULL;
    int planCount = 0;
    int *billingCycles = NULL;
    int cycleCount = 0;
    
    // Setup the database pool
    QSqlDbPool::setDefaultHost(cfgVal("TAAMySQLHost"));
    QSqlDbPool::setDefaultName(cfgVal("TAAMySQLDB"));
    QSqlDbPool::setDefaultUser(cfgVal("TAAMySQLUser"));
    QSqlDbPool::setDefaultPass(cfgVal("TAAMySQLPass"));
    QSqlDbPool::setDefaultDriver(cfgVal("TAASQLDriver"));

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
    if (!q.exec("select InternalID from BillingCycles")) {
        fprintf(stderr, "Error loading billing cycle list\n");
        exit(-1);
    }
    if (q.size() < 1) {
        fprintf(stderr, "No billing cycles have been defined.\n");
        exit(-1);
    }
    cycleCount = q.size();
    billingCycles = new int[cycleCount+1];
    int curCycle = 0;
    while(q.next()) {
        billingCycles[curCycle] = q.value(0).toInt();
        curCycle++;
    }
    
    // Open our CSV file
    if (!parser.openFile("SeanetBillables.csv", true)) {
        fprintf(stderr, "Unable to open SeanetBillables.csv, aborting\n");
        exit(-1);
    }

    // Get the index of certain columns from our header row.
    int loginTypeCol    = parser.header().findIndex("ServiceType");
    int billableCol     = parser.header().findIndex("BillableItem");
    int descriptionCol  = parser.header().findIndex("PlanName");
    int classCol        = parser.header().findIndex("Category");
    int priceCol        = parser.header().findIndex("monthlyFee");

    int loginTypeIns = 0;
    int billablesIns = 0;
    int billablePricesIns = 0;

    QSqlCursor  logins("LoginTypes", true, db1);
    QSqlCursor  billables("Billables", true, db1);
    QSqlCursor  billablesData("BillablesData", true, db1);
    QSqlRecord  *buf;
    while(parser.loadRecord()) {
        if (parser.row()[loginTypeCol] != NULL) {
            buf = logins.primeInsert();
            buf->setValue("InternalID",     0);
            buf->setValue("LoginType",      parser.row()[loginTypeCol]);
            buf->setValue("Description",    parser.row()[descriptionCol]);
            buf->setValue("LoginClass",     parser.row()[classCol]);
            loginTypeIns += logins.insert();
        }

        // Prepare for a new billable to be inserted.
        if (parser.row()[billableCol] != NULL) {
            buf = billables.primeInsert();
            buf->setValue("ItemNumber",     0);
            buf->setValue("ItemID",         parser.row()[billableCol]);
            buf->setValue("Description",    parser.row()[descriptionCol]);
            buf->setValue("ItemType",       1);
            buf->setValue("AccountNo",      9100);
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
                for (int b = 0; b < cycleCount; b++) {
                    int rowCount = 0;
                    QString tmpPrice = parser.row()[priceCol];
                    tmpPrice.replace('$', "");
                    buf = billablesData.primeInsert();
                    buf->setValue("InternalID",     0);
                    buf->setValue("ItemNumber",     itemNo);
                    buf->setValue("RatePlanID",     ratePlans[r]);
                    buf->setValue("CycleID",        billingCycles[b]);
                    buf->setValue("Description",    "");
                    buf->setValue("Price",          tmpPrice);
                    buf->setValue("SecondaryPrice", tmpPrice);
                    buf->setValue("Units",          "Month");
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

    printf("Imported %d LoginTypes, %d BillableItems and %d Billable Prices for %d Rate Plans and %d Billing Cycles\n", loginTypeIns, billablesIns, billablePricesIns, planCount, cycleCount);
}


