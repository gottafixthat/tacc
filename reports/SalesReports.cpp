/**
 * SalesReports.h - Reports for Billable Items and packages.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#include <stdio.h>
#include <stdlib.h>
#include <q3listview.h>
#include <qapplication.h>
#include <q3dict.h>

#include <BlargDB.h>
#include <BString.h>
#include <TAATools.h>

#include "SalesReports.h"

salesByServiceReport::salesByServiceReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Sales By Service" );
	setTitle("Sales By Service");
	
    int c = 0;
	repBody->setColumnText(0, "Service");  repBody->setColumnAlignment(c++, AlignLeft);
    //repBody->addColumn("Qty");             repBody->setColumnAlignment(c++, AlignRight);
    repBody->addColumn("Amount");          repBody->setColumnAlignment(c++, AlignRight);
	
    // When starting out, do this month.
    allowDates(REP_ALLDATES);
    setDateRange(d_thisMonth);

    allowFilters(0);
	refreshReport();
}


salesByServiceReport::~salesByServiceReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void salesByServiceReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    QDate   sDate = this->startDateCal->date();
    QDate   eDate = this->endDateCal->date();
    char    tmpStr[1024];
    float   grandTotal = 0.0;

    QApplication::setOverrideCursor(waitCursor);
    repBody->clear();

    // Two queries, one for standalone billables, one for packages.
    DB.query("select AcctsRecv.ItemID, Billables.ItemID, sum(AcctsRecv.Amount) from AcctsRecv, Billables where Billables.ItemNumber = AcctsRecv.ItemID and AcctsRecv.TransType = 0 and AcctsRecv.ItemID > 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' group by AcctsRecv.ItemID order by Billables.ItemID", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        if (atof(DB.curRow[2]) > 0.0) {
            new Q3ListViewItem(repBody, 
                    DB.curRow[1], 
                    DB.curRow[2],
                    DB.curRow[0],    // The Item ID
                    "0"              // isPackage
                    );
            grandTotal += atof(DB.curRow[2]);
        }
    }
    
    // The seocnd query for packages.
    DB.query("select AcctsRecv.PackageItem, Packages.PackageTag, sum(AcctsRecv.Amount) from AcctsRecv, Packages where Packages.InternalID = AcctsRecv.PackageItem and AcctsRecv.TransType = 0 and AcctsRecv.ItemID = 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' group by Packages.InternalID order by Packages.PackageTag", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        sprintf(tmpStr, "%s - Package", DB.curRow[1]);
        if (atof(DB.curRow[2]) > 0.0) {
            new Q3ListViewItem(repBody, 
                    tmpStr,
                    DB.curRow[2],
                    DB.curRow["PackageItem"],   // The Package ID
                    "1"                         // isPackage
                    );
            grandTotal += atof(DB.curRow[2]);
        }
    }
    // One last query to get the stuff in the database prior to 5/1/06
    DB.query("select sum(AcctsRecv.Amount) from AcctsRecv where AcctsRecv.TransType = 0 and AcctsRecv.ItemID = 0 and AcctsRecv.PackageItem = 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d'", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        //sprintf(tmpStr, "%s - Package", DB.curRow[1]);
        if (atof(DB.curRow[0]) > 0.0) {
            Q3ListViewItem *curItem = new Q3ListViewItem(repBody, 
                    "[Unknown Items]",
                    DB.curRow[0]
                    );
            grandTotal += atof(DB.curRow[0]);
        }
    }

    sprintf(tmpStr, "%.2f", grandTotal);
    new Q3ListViewItem(repBody, "Total", tmpStr);

    sprintf(tmpStr, "Sales by Service\n%02d-%02d-%04d through %02d-%02d-%04d, $%.2f", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day(), grandTotal);
	setTitle(tmpStr);

    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void salesByServiceReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        if (curItem->key(2,0).length()) {
            salesByServiceDetailReport *detRep = new salesByServiceDetailReport();
            detRep->setStartDate(startDate());
            detRep->setEndDate(endDate());
            detRep->setBillableItem(atoi(curItem->key(2,0)), atoi(curItem->key(3,0)));
            detRep->show();
        }
    }
}

/**
 * salesByServiceDetailReport()
 *
 * Constructor.
 */
salesByServiceDetailReport::salesByServiceDetailReport(QWidget *parent, const char *name)
	: Report( parent, name )
{
	setCaption( "Sales Detail Report" );
	setTitle("Sales Detail Report");
	
    int c = 0;
	repBody->setColumnText(0, "Date");      repBody->setColumnAlignment(c++, AlignLeft);
    repBody->addColumn("Amount");           repBody->setColumnAlignment(c++, AlignRight);
    repBody->addColumn("Cust ID");          repBody->setColumnAlignment(c++, AlignLeft);
    custIDCol = c - 1;
	repBody->addColumn("Customer Name");    repBody->setColumnAlignment(c++, AlignLeft);
	repBody->addColumn("Company");          repBody->setColumnAlignment(c++, AlignLeft);
	repBody->addColumn("Statement");        repBody->setColumnAlignment(c++, AlignLeft);
	
    // When starting out, do this month.
    allowDates(REP_ALLDATES);
    setDateRange(d_thisMonth);
    myBillableID = 0;

    allowFilters(0);
}

/**
 * ~salesByServiceDetailReport
 *
 * Destructor.
 */
salesByServiceDetailReport::~salesByServiceDetailReport()
{
}

/**
 * setBillableItem()
 *
 * Sets the billable item ID that we will display.
 */
void salesByServiceDetailReport::setBillableItem(long billableID, int isPackage)
{
    myBillableID = billableID;
    myIsPackage = isPackage;
    if (myBillableID) refreshReport();
}

/**
 * refreshReport()
 *
 * Fills the report body with data.
 */
void salesByServiceDetailReport::refreshReport()
{
    repBody->clear();
    if (!myBillableID) return;

    ADB     DB;
    QDate   sDate = this->startDateCal->date();
    QDate   eDate = this->endDateCal->date();
    QString companyName;
    QString customerName;
    QString tmpStr;
    double  grandTotal = 0.00;

    QApplication::setOverrideCursor(waitCursor);

    if (myIsPackage) {
        PackagesDB  PDB;
        PDB.get(myBillableID);
        tmpStr = tmpStr.sprintf("Sales Detail Report\n%s (Package)", PDB.getStr("Description"));
        setTitle(tmpStr);

        //debug(1,"select AcctsRecv.TransDate, AcctsRecv.PackageItem, Packages.InternalID, Packages.PackageTag, AcctsRecv.Amount, Customers.CustomerID, Customers.FullName, Customers.ContactName from AcctsRecv, Packages, Customers where Packages.InternalID = AcctsRecv.PackageItem and AcctsRecv.TransType = 0 and AcctsRecv.ItemID = %d and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' and Customers.CustomerID = AcctsRecv.CustomerID order by Customers.CustomerID\n", myBillableID, sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
        DB.query("select AcctsRecv.TransDate, AcctsRecv.PackageItem, Packages.InternalID, Packages.PackageTag, AcctsRecv.Amount, Customers.CustomerID, Customers.FullName, Customers.ContactName from AcctsRecv, Packages, Customers where Packages.InternalID = AcctsRecv.PackageItem and AcctsRecv.TransType = 0 and AcctsRecv.PackageItem = %d and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' and Customers.CustomerID = AcctsRecv.CustomerID order by Customers.CustomerID", myBillableID, sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());

    } else {
        // Get the title.
        BillablesDB BDB;
        BDB.get(myBillableID);
        tmpStr = tmpStr.sprintf("Sales Detail Report\n%s", BDB.getStr("Description"));
        setTitle(tmpStr);

        // Two queries, one for standalone billables, one for packages.
        DB.query("select AcctsRecv.TransDate, AcctsRecv.ItemID, Billables.ItemID, AcctsRecv.Amount, AcctsRecv.StatementNo, Customers.CustomerID, Customers.FullName, Customers.ContactName from AcctsRecv, Billables, Customers where Billables.ItemNumber = AcctsRecv.ItemID and AcctsRecv.TransType = 0 and AcctsRecv.ItemID = %d and AcctsRecv.PackageItem = 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' and Customers.CustomerID = AcctsRecv.CustomerID order by Customers.CustomerID", myBillableID, sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    }
    // Both queries return compatible result sets for a single pass
    // through the returned rows.
    if (DB.rowCount) while (DB.getrow()) {
        customerName = "";
        companyName  = "";
        // Check for a company name.
        if (strlen(DB.curRow["ContactName"])) {
            customerName = DB.curRow["ContactName"];
            companyName  = DB.curRow["FullName"];
        } else {
            customerName = DB.curRow["FullName"];
        }
        new Q3ListViewItem(repBody, 
                DB.curRow["TransDate"],
                DB.curRow["Amount"], 
                DB.curRow["CustomerID"],
                customerName,
                companyName,
                DB.curRow["StatementNo"]
                );
        grandTotal += atof(DB.curRow["Amount"]);
    }
    tmpStr = tmpStr.sprintf("%.2f", grandTotal);
    new Q3ListViewItem(repBody, "Total", tmpStr);

    QApplication::restoreOverrideCursor();
}

/**
 * listItemSelected()
 *
 * Gets called when the user double clicks an item in the report.
 */
void salesByServiceDetailReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (!curItem) return;
    QString custID;
    custID = curItem->key(custIDCol, 0);
    if (custID.length()) emit(openCustomer(custID.toInt()));
}


