/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>

#include <mcve.h>
#include <ADB.h>
#include <CCValidate.h>
#include <TAATools.h>
#include <BString.h>
#include <Cfg.h>
#include <CheckPaymentsReport.h>

using namespace Qt;

/**
 * CheckPaymentsReport()
 *
 * Constructor.
 */
CheckPaymentsReport::CheckPaymentsReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption("Check Payments");
	setTitle("Check Payments");
	
	repBody->setColumnText(0, "Date");     repBody->setColumnAlignment(0, Qt::AlignLeft);
	repBody->addColumn("Amount");          repBody->setColumnAlignment(1, Qt::AlignRight);
	repBody->addColumn("CustomerID");      repBody->setColumnAlignment(2, Qt::AlignLeft);
	repBody->addColumn("Customer Name");   repBody->setColumnAlignment(3, Qt::AlignLeft);
	repBody->addColumn("Company");         repBody->setColumnAlignment(4, Qt::AlignLeft);
	repBody->addColumn("Comments");        repBody->setColumnAlignment(5, Qt::AlignLeft);
	
    //setStartDate(QDate(2007,6,1));
    //setEndDate(QDate(2007,6,30));
    allowDates(REP_ALLDATES);
    allowFilters(0);
	refreshReport();
}


/**
 * ~CheckPaymentsReport()
 * 
 * Destructor.
 */
CheckPaymentsReport::~CheckPaymentsReport()
{
}

/**
 * refreshReport()
 *
 * A virtual function that gets called when the user changes
 * one of the dates or otherwise needs to refresh the report.
 */
void CheckPaymentsReport::refreshReport()
{
    ADB         DB;
    QString     customerName;
    QString     companyName;
    QString     amount;
    QString     memo;
    QString     query;
    double      total = 0.00;

    QApplication::setOverrideCursor(Qt::waitCursor);
    repBody->clear();

    // Format our dates
    QString     sDate = startDate().toString("yyyy-MM-dd");
    QString     eDate = endDate().toString("yyyy-MM-dd");

    // Create our query.
    DB.query("select GL.InternalID, GL.TransDate, GL.Amount, GL.Memo, AcctsRecv.RefNo, AcctsRecv.CustomerID, Customers.FullName, Customers.ContactName from GL, AcctsRecv, Customers where GL.IntAccountNo = %d and GL.TransType = 2 and AcctsRecv.InternalID = GL.TransTypeLink and AcctsRecv.RefNo > 0 and Customers.CustomerID = AcctsRecv.CustomerID and GL.TransDate >= '%s' and GL.TransDate <= '%s'", atoi(cfgVal("UndepositedFundsAcct")), sDate.ascii(), eDate.ascii());

    if (DB.rowCount) {
        while(DB.getrow()) {
            amount = DB.curRow["Amount"];
            total += amount.toDouble();
            amount = amount.sprintf("%.2f", amount.toDouble());

            customerName = "";
            companyName  = "";
            // Check for a company name.
            if (strlen(DB.curRow["ContactName"])) {
                customerName = DB.curRow["ContactName"];
                companyName  = DB.curRow["FullName"];
            } else {
                customerName = DB.curRow["FullName"];
            }

            (void) new Q3ListViewItem(repBody, 
                    DB.curRow["TransDate"],
                    amount,
                    DB.curRow["CustomerID"],
                    customerName,
                    companyName,
                    DB.curRow["Memo"]);
        }
        // Add the total line
        amount = amount.sprintf("%.2f", total);
        (void) new Q3ListViewItem(repBody, 
                "Total",
                amount);
    }

    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report, opening a ccPaymentDetails report for the specified
 * day.
 */
void CheckPaymentsReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        // If they double click, open the customer's window.
        QString custID = curItem->key(2,0);
        if (custID.toInt()) {
            emit(openCustomer(custID.toInt()));
        }
    }
}



// vim: expandtab
