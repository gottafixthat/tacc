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
#include <AcctsRecv.h>

#include "seanetimport.h"


int main( int argc, char ** argv )
{
    // Load the main config file and setup our QApplication.
    loadTAAConfig();
    QApplication    a( argc, argv );

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
    importLoginTypes();
    importCustomers();

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
    int         *billingCycles = NULL;
    int         cycleCount = 0;
    
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

    // Now, walk through the login types we've just added and create
    // a billable item for each one we find.
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

    printf("Imported %d LoginTypes, %d BillableItems and %d Billable Prices for %d Rate Plans and %d Billing Cycles\n", loginTypeIns, billablesIns, billablePricesIns, planCount, cycleCount);
}

/**
 * importCustomers()
 *
 * Loads customer records from the SeanetPlansMain.csv file
 */
void importCustomers()
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
    if (!parser.openFile("SeanetPlansMain.csv", true)) {
        fprintf(stderr, "Unable to open SeanetPlansMain.csv, aborting\n");
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
    int creditCardCol   = parser.header().findIndex("CreditCard");
    int expMonthCol     = parser.header().findIndex("ExpM");
    int expYearCol      = parser.header().findIndex("ExpYY");
    int svcPlanCol      = parser.header().findIndex("ServicePlan");
    int periodCol       = parser.header().findIndex("BillingPeriod");
    int serviceStartCol = parser.header().findIndex("ServiceStart");
    int nextBillDateCol = parser.header().findIndex("NextBillDate");
    int closeDateCol    = parser.header().findIndex("SchClose");

    // Set our starting information
    QString lastRegnum = "";
    int     numCusts = 0;
    int     numLines = 0;

    // Now walk through the rows.
    while (parser.loadRecord()) {
        numLines++;
        //if (!(numLines % 10)) fprintf(stderr, "\rProcessing line %d...", numLines);
        if (lastRegnum.compare(parser.row()[regNumCol])) {
            if (cust) {
                saveCustomer(cust);
                delete cust;
            }
            cust = new customerRecord;
            // We should save the current customer data here
            lastRegnum = parser.row()[regNumCol];
            numCusts++;

            // Prime the customer record
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
                cust->cardExpMonth  = parser.row()[expMonthCol].toInt();
                cust->cardExpYear   = parser.row()[expYearCol].toInt();
            }

            //fprintf(stderr, "Customer ID %ld %-50s\r", cust->customerID, cust->contactName.ascii());
        }

        // At this point, regardless of whether or not we are on a
        // new customer, we should have a customer record.
        // Find the service we're looking for.
        QSqlQuery   q(db1);
        q.prepare("select InternalID from LoginTypes where Description LIKE :svcplan");
        q.bindValue(":svcplan", parser.row()[svcPlanCol]);
        if (!q.exec()) {
            fprintf(stderr, "Error executing query: '%s'\n", q.lastQuery().ascii());
            exit(-1);
        }
        if (q.size()) {
            q.next();
            // Create a new Login record for this line and append it
            // to the list stored in the customer record.
            serviceRecord   *svc = new serviceRecord;
            // Set the "Login ID"
            sprintf(svc->loginID, "sea%05d", numLines);
            // Set the loginTypeID
            svc->loginTypeID = q.value(0).toInt();
            svc->endsOn = parser.row()[nextBillDateCol];
            svc->closeDate = parser.row()[closeDateCol];
            cust->svcList.append(svc);
        } else {
            // No matching login type, search for a matching billable item instead.
            q.prepare("select ItemNumber from Billables where Description LIKE :svcplan");
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
                billable->endsOn = parser.row()[nextBillDateCol];
                billable->closeDate = parser.row()[closeDateCol];
                cust->billableList.append(billable);
            } else {
                fprintf(stderr, "Unable to find match for service plan '%s' on line %d.  Skipping.\n", parser.row()[svcPlanCol].ascii(), numLines);
            }
        }

        
        
    }
    // Save the last one we were working on.
    saveCustomer(cust);

    //fprintf(stderr, "\rProcessing line %d...\n", numLines);
    fprintf(stderr, "Fixing dates...\r");
    QSqlQuery   sql(db1);
    sql.exec("update Subscriptions set EndsOn = '2009-02-28', LastDate = '2009-02-01'");
    sql.exec("update Customers set RatePlanDate = '2009-02-01', BillingCycleDate = '2009-02-01', LastBilled = '0000-00-00', LastStatementNo = '0000-00-00', AccountExpires = '0000-00-00', AccountOpened = '0000-00-00', AccountClosed = '0000-00-00', AccountReOpened = '0000-00-00', GraceDate = '0000-00-00', PromotionGiven = '0000-00-00'");



    printf("Customers:  Parsed %d lines, found %d customers.\n", numLines, numCusts);


}

/**
 * saveCustomer()
 *
 * Saves the data in a customerRecord into the database.
 */
void saveCustomer(customerRecord *cust)
{
    fprintf(stderr, "\e[KSaving Customer ID %d %s\r", cust->customerID, cust->contactName.ascii());

    QSqlDbPool  custPool;
    QSqlDbPool  addrPool;
    
    QSqlCursor  customer("Customers", true, custPool.qsqldb());
    QSqlCursor  addr("Addresses", true, addrPool.qsqldb());
    QSqlRecord  *buf;

    QString     fullName = "";
    QString     contactName = "";
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
    buf->setValue("AccountOpened",      timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("AccountClosed",      timeToStr(rightNow(), YYYY_MM_DD));  // FIXME
    buf->setValue("AccountReOpened",    timeToStr(rightNow(), YYYY_MM_DD));  // FIXME
    buf->setValue("GraceDate",          timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("PromotionGiven",     timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("CurrentBalance",     cust->currentBalance);
    buf->setValue("CreditLimit",        cust->currentBalance);
    if (cust->statementType == 3) buf->setValue("PrintedStatement", 1);

    // Set the rate plan, billing cycle and terms
    buf->setValue("Terms",              1);     // FIXME
    buf->setValue("RatePlan",           1);     // FIXME
    buf->setValue("RatePlanDate",       timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("BillingCycle",       1);     // FIXME
    buf->setValue("BillingCycleDate",   timeToStr(rightNow(), YYYY_MM_DD));
    buf->setValue("LastBilled",         timeToStr(rightNow(), YYYY_MM_DD)); // FIXME


    
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

    
    CustomersDB CDB;
    // Add the "logins" in the service records
    for (uint i = 0; i < cust->svcList.count(); i++) {
        serviceRecord   *svcRec = cust->svcList.at(i);
        LoginsDB    ldb;
        ldb.setValue("CustomerID",      (long int)cust->customerID);
        ldb.setValue("LoginID",         svcRec->loginID);
        ldb.setValue("LoginType",       svcRec->loginTypeID);
        ldb.setValue("ContactName",     fullName.ascii());
        ldb.setValue("DiskSpace",       0);
        ldb.setValue("DialupChannels",  0);
        ldb.setValue("Active",          1);
        ldb.ins();
        ldb.setValue("Active",          1);
        ldb.upd(0);
        CDB.get((long int)cust->customerID);
        if (!strlen((const char *) CDB.getStr("PrimaryLogin")) ||
                !strncmp(CDB.getStr("PrimaryLogin"), "W", 1)) {
            CDB.setValue("PrimaryLogin", svcRec->loginID);
            CDB.upd();
        }
        //CDB.doSubscriptions();
        //ldb.addSubscriptions();
    }

    // Add any extra billables
    for (uint i = 0; i < cust->billableList.count(); i++) {
        billableRecord  *billRec = cust->billableList.at(i);
        CDB.get((long int)cust->customerID);
        SubscriptionsDB    sdb;
        sdb.setValue("InternalID",      0);
        sdb.setValue("CustomerID",      (long int)cust->customerID);
        sdb.setValue("LoginID",         CDB.getStr("PrimaryLogin"));
        sdb.setValue("Active",          1);
        sdb.setValue("PackageNo",       0);
        sdb.setValue("ItemNumber",      billRec->billableItemID);
        sdb.setValue("ParentID",        0);
        sdb.setValue("Quantity",        1.0);
        sdb.setValue("ItemDesc",        "");
        sdb.setValue("AutoPrice",       1);
        sdb.setValue("AutoRenew",       1);
        sdb.setValue("SetupCharged",    1);
        sdb.ins();
    }

    // If they have an opening balance, create an AR charge for them.
    if (cust->currentBalance > 0.0) {
        AcctsRecv   AR;
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
}

