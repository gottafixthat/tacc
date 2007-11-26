/*
** wasync - Syncs up the remote WebAcct database with the local TAA
**          database.
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <getopt.h>
#include <pwd.h>

// Qt Includes
#include <qlist.h>
#include <qdatetm.h>

// Blarg includes
#include <ADB.h>
#include <bdes.h>
#include <Cfg.h>

#define TACT_KEYFILE "/usr/local/lib/tact.key"

// Function prototypes
int  main(int, char **);
void showSubscriptionTotals(float &totMonthly);
long showLoginCountsByClass();
void showSubscriptionDetails();

main(int argc, char **argv)
{
    passwd  *pent;
    uid_t   myUID;
    char    homeCfg[1024];

    myUID = getuid();
    pent = getpwuid(myUID);
    if (pent == NULL) {
        fprintf(stderr, "\nUnable to get user information.\n\n");
        exit(-1);
    }

    strcpy(homeCfg, pent->pw_dir);
    strcat(homeCfg, "/.taa/taa.cf");
    
    // load the file configuration first.  These settings can be
    // overridden by the database settings.
    if (!loadCfg(homeCfg)) {
        if (!loadCfg("/etc/taa.cf")) {
            if (!loadCfg("/usr/local/etc/taa.cf")) {
                if (!loadCfg("/usr/local/lib/taa.cf")) {
                    fprintf(stderr, "\nUnable to find a configuration file.\n");
                    exit(-1);
                }
            }
        }
    }
    
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));

    float   monthly    = 0.00;
    long    custs      = 0;
    float   annual     = 0.00;
    float   custannual = 0.00;
    bool    quick      = false;
    int     c;

    // Get our command line options.
    while(1) {
        c = getopt(argc, argv, "q");
        if (c == -1) break;     // All out of options.
        switch(c) {
            case    'q':
                quick = true;
                break;

            default:
                break;
        }
    }

    showSubscriptionTotals(monthly);
    custs = showLoginCountsByClass();

    printf("\n");

    annual     = monthly * 12.0;
    custannual = annual / custs;

    printf("\nGross per account valuation.\n");
    printf("%-30s $%9.2f\n", "Per Customer:", custannual);
    printf("%-30s $%9.2f\n", "Estimated Annual Revenue:", annual);

    printf("\nApplying 10%% uncollectible/churn factor.\n");
    printf("%-30s $%9.2f\n", "Per Customer:", custannual * 0.9);
    printf("%-30s $%9.2f\n", "Estimated Annual Revenue:", annual * 0.9);

    /*
    printf("\nApplying 50%% valuation factor.\n");
    printf("%-30s $%9.2f\n", "Per Customer value:", (custannual * 0.9) * 0.5);
    printf("%-30s $%9.2f\n", "Customer Base Value:", (annual * 0.9) * 0.5);
    */

    printf("\n");

    if (!quick) showSubscriptionDetails();
}

/*
** showSubscriptionTotals - Calculates the total amount billed monthly for
**                          all active users from their subscriptions.
*/

void showSubscriptionTotals(float &totMonthly)
{
    ADB     DB;
    float   total   = 0.00;
    float   totNorm = 0.00;
    float   totSet  = 0.00;
    float   totPkg  = 0.00;

    
    // First, get the total number of subscriptions that are billed out
    // without any packages, and that are not part of any packages.
    DB.query("select Subscriptions.CustomerID, Subscriptions.LoginID, Subscriptions.Quantity, Subscriptions.ItemNumber, Customers.BillingCycle, Customers.RatePlan, BillablesData.Price from Subscriptions, Customers, BillablesData where Subscriptions.Active = 1 and Subscriptions.ParentID = 0 and Subscriptions.PackageNo = 0 and Subscriptions.AutoPrice = 1 and Customers.CustomerID = Subscriptions.CustomerID and BillablesData.ItemNumber = Subscriptions.ItemNumber and BillablesData.RatePlanID = Customers.RatePlan and BillablesData.CycleID = Customers.BillingCycle");
    while (DB.getrow()) {
        switch (atoi(DB.curRow["BillingCycle"])) {
            case    2:      // Quarter A
            case    3:      // Quarter B
            case    4:      // Quarter C
                totNorm += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 3;
                break;

            case    5:      // Annual - March
                totNorm += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 12;
                break;

            case    1:      // Monthly
            default:
                totNorm += atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"]);
                break;
        }
    }
    //printf("Total normal subscriptions are $%.2f\n", totNorm);

    DB.query("select Subscriptions.CustomerID, Subscriptions.LoginID, Subscriptions.Quantity, Subscriptions.ItemNumber, Subscriptions.Price, Customers.BillingCycle, Customers.RatePlan from Subscriptions, Customers where Subscriptions.Active = 1 and Subscriptions.ParentID = 0 and Subscriptions.PackageNo = 0 and Subscriptions.AutoPrice = 0 and Customers.CustomerID = Subscriptions.CustomerID");
    while (DB.getrow()) {
        switch (atoi(DB.curRow["BillingCycle"])) {
            case    2:      // Quarter A
            case    3:      // Quarter B
            case    4:      // Quarter C
                totSet += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 3;
                break;

            case    5:      // Annual - March
                totSet += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 12;
                break;

            case    1:      // Monthly
            default:
                totSet += atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"]);
                break;
        }
    }
    //printf("Total set subscriptions are $%.2f\n", totSet);
    
    
    // Get the packages now.
    DB.query("select Subscriptions.CustomerID, Subscriptions.LoginID, Subscriptions.Quantity, Subscriptions.PackageNo, Customers.BillingCycle, Customers.RatePlan, PackagesData.Price from Subscriptions, Customers, PackagesData where Subscriptions.Active = 1 and Subscriptions.ParentID = 0 and Subscriptions.PackageNo <> 0 and Customers.CustomerID = Subscriptions.CustomerID and PackagesData.PackageID = Subscriptions.PackageNo and PackagesData.RatePlanID = Customers.RatePlan and PackagesData.CycleID = Customers.BillingCycle");
    while (DB.getrow()) {
        switch (atoi(DB.curRow["BillingCycle"])) {
            case    2:      // Quarter A
            case    3:      // Quarter B
            case    4:      // Quarter C
                totPkg += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 3;
                break;

            case    5:      // Annual - March
                totPkg += (atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"])) / 12;
                break;

            case    1:      // Monthly
            default:
                totPkg += atof(DB.curRow["Quantity"]) * atof(DB.curRow["Price"]);
                break;
        }
    }
    //printf("Total package subscriptions are $%.2f\n", totPkg);


    total = totNorm + totSet + totPkg;

    totMonthly = total;

    printf("%-30s $%9.2f\n", "Total monthly billables:", total);
}

/*
** showLoginCountsByClass - Displays the number of non-system logins by
**                          their class.  i.e. number of dialup, number of
**                          Qwest DSL, etc.
*/

long showLoginCountsByClass(void)
{
    ADB     classDB;
    ADB     DB;
    long    totCustomers = 0;
    long    totLogins = 0;
    char    tmpStr[1024];


    DB.query("select distinct(CustomerID) from Logins where Active <> 0 and CustomerID > 100");
    printf("%-30s  %9ld\n", "Total Active Customers:", DB.rowCount);
    totCustomers = DB.rowCount;
    
    // How many of those are on automatic payment?
    DB.query("select distinct Logins.CustomerID, AutoPayments.CustomerID from Logins, AutoPayments where Logins.Active > 0 and AutoPayments.CustomerID = Logins.CustomerID and AutoPayments.Active > 0");
    printf("%-30s  %9ld (%.1f%%)\n", "Auto Credit Card Customers:", DB.rowCount, (float) DB.rowCount / totCustomers * 100.0);

    classDB.query("select distinct LoginClass from LoginTypes order by LoginClass");
    while (classDB.getrow()) {
        DB.query("select Logins.LoginID, Logins.CustomerID, LoginTypes.LoginType from Logins, LoginTypes where Logins.CustomerID > 100 and Logins.Active > 0 and Logins.LoginType = LoginTypes.InternalID and LoginTypes.LoginClass = '%s'", classDB.curRow["LoginClass"]);
        sprintf(tmpStr, "%s accounts:", classDB.curRow["LoginClass"]);
        printf("%-30s  %9ld\n", tmpStr, DB.rowCount);
        totLogins += DB.rowCount;
    }
    printf("%-30s  %9ld\n", "Total Logins:", totLogins);

    return totCustomers;
}

/*
** showSubscriptionDetails - Walks through the packages and subscriptions
**                           for all customers and calculates how much
**                           we are making for each subscription type.
*/

void showSubscriptionDetails()
{
    ADB     sbDB;
    ADB     sumDB;
    ADB     tmpDB;
    char    tmpName[512];
    long    totCount = 0;
    float   tmpAmount;
    float   totMonthly = 0.0;
    float   tmpPrice;

    printf("\nDetailed breakdown of account types and monthly revenue based on\nsubscription items:\n");
    printf("%-62s %5s %9s\n", "Description", "Qty", "Amount");
    // First, we'll get all of the package numbers.  Then we will sum
    // up all of the active subscriptions for that package type.
    printf("\nPackaged billables:\n");
    sbDB.query("select distinct(Subscriptions.PackageNo), Packages.PackageTag, Packages.Description from Subscriptions, Packages where Subscriptions.PackageNo > 0 and Subscriptions.Active > 0 and Packages.InternalID = Subscriptions.PackageNo order by Packages.Description");
    if (sbDB.rowCount) while (sbDB.getrow()) {
        // Get the name of the package
        strcpy(tmpName, sbDB.curRow["Description"]);
        printf("%-16s %-62s ", sbDB.curRow["PackageTag"], tmpName);

        // Get the dollar amounts from the type
        tmpAmount = 0.0;
        sumDB.query("select Subscriptions.PackageNo, Subscriptions.Quantity, Customers.BillingCycle, Customers.RatePlan, PackagesData.Price from Subscriptions, Customers,PackagesData where Subscriptions.PackageNo = %ld and Subscriptions.Active > 0 and Customers.CustomerID = Subscriptions.CustomerID and PackagesData.PackageID = Subscriptions.PackageNo and PackagesData.CycleID = Customers.BillingCycle and PackagesData.RatePlanID = Customers.RatePlan", atol(sbDB.curRow["PackageNo"]));
        totCount = sumDB.rowCount;
        printf("%5ld ", sumDB.rowCount);
        if (sumDB.rowCount) while (sumDB.getrow()) {
            switch (atoi(sumDB.curRow["BillingCycle"])) {
                case    2:      // Quarter A
                case    3:      // Quarter B
                case    4:      // Quarter C
                    tmpAmount += (atof(sumDB.curRow["Quantity"]) * atof(sumDB.curRow["Price"])) / 3;
                    break;

                case    5:      // Annual - March
                    tmpAmount += (atof(sumDB.curRow["Quantity"]) * atof(sumDB.curRow["Price"])) / 12;
                    break;

                case    1:      // Monthly
                default:
                    tmpAmount += atof(sumDB.curRow["Quantity"]) * atof(sumDB.curRow["Price"]);
                    break;
            }

        }
        totMonthly += tmpAmount;
        printf("%9.2f ", tmpAmount);

        printf("\n");
    }

    // Go through the subscriptions for non-packaged items.
    printf("\nStandalone subscriptions:\n");
    sbDB.query("select distinct(Subscriptions.ItemNumber), Billables.ItemID, Billables.Description from Subscriptions, Billables where Subscriptions.PackageNo = 0 and Subscriptions.Active > 0 and Subscriptions.ParentID = 0 and Billables.ItemNumber = Subscriptions.ItemNumber order by Billables.Description");
    if (sbDB.rowCount) while (sbDB.getrow()) {
        // Get the name of the package
        strcpy(tmpName, sbDB.curRow["Description"]);
        printf("%-16s %-62s ", sbDB.curRow["ItemID"], tmpName);

        // Get the dollar amounts from the type
        tmpAmount = 0.0;
        sumDB.query("select Subscriptions.InternalID, Subscriptions.ItemNumber, Subscriptions.Quantity, Subscriptions.AutoPrice, Customers.BillingCycle, Customers.RatePlan, BillablesData.Price from Subscriptions, Customers, BillablesData where Subscriptions.ItemNumber = %ld and Subscriptions.Active > 0 and Subscriptions.ParentID = 0 and Customers.CustomerID = Subscriptions.CustomerID and BillablesData.ItemNumber = Subscriptions.ItemNumber and BillablesData.CycleID = Customers.BillingCycle and BillablesData.RatePlanID = Customers.RatePlan", atol(sbDB.curRow["ItemNumber"]));
        printf("%5ld ", sumDB.rowCount);
        totCount += sumDB.rowCount;
        if (sumDB.rowCount) while (sumDB.getrow()) {
            tmpPrice = atof(sumDB.curRow["Price"]);
            if (!atoi(sumDB.curRow["AutoPrice"])) {
                tmpDB.query("select Price from Subscriptions where InternalID = %s", sumDB.curRow["InternalID"]);
                tmpDB.getrow();
                tmpPrice = atof(tmpDB.curRow["Price"]);
            }
            switch (atoi(sumDB.curRow["BillingCycle"])) {
                case    2:      // Quarter A
                case    3:      // Quarter B
                case    4:      // Quarter C
                    tmpAmount += (atof(sumDB.curRow["Quantity"]) * tmpPrice) / 3;
                    break;

                case    5:      // Annual - March
                    tmpAmount += (atof(sumDB.curRow["Quantity"]) * tmpPrice) / 12;
                    break;

                case    1:      // Monthly
                default:
                    tmpAmount += atof(sumDB.curRow["Quantity"]) * tmpPrice;
                    break;
            }

        }
        totMonthly += tmpAmount;
        printf("%9.2f ", tmpAmount);


        printf("\n");
    }

    

    printf("%16s %62s %5ld $%8.2f\n\n", "", "Total", totCount, totMonthly);
}

