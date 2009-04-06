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

#include <ADB.h>
#include <TAATools.h>
#include <Cfg.h>
#include <CheckPaymentsReport.h>

using namespace Qt;

/**
 * CheckPaymentsReport()
 *
 * Constructor.
 */
CheckPaymentsReport::CheckPaymentsReport(QWidget* parent)
	: TACCReport(parent)
{
	setWindowTitle("Check Payments");
	setTitle("Check Payments");

    QStringList headers;
    headers += "Date";
    headers += "Amount";
    headers += "Cust ID";
    headers += "Customer Name";
    headers += "Company";
    headers += "Comments";
    repBody->setColumnCount(6);
	repBody->setHeaderLabels(headers);
	
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

            TACCReportItem *curItem = new TACCReportItem(repBody);
            curItem->setText(0, DB.curRow["TransDate"]);
            curItem->setText(1, amount);
            curItem->setText(2, DB.curRow["CustomerID"]);
            curItem->setText(3, customerName);
            curItem->setText(4, companyName);
            curItem->setText(5, DB.curRow["Memo"]);
            curItem->setSortType(1, TACCReportItem::rDouble);
            curItem->setTextAlignment(0, Qt::AlignLeft);
            curItem->setTextAlignment(1, Qt::AlignRight);
            curItem->setTextAlignment(2, Qt::AlignLeft);
            curItem->setTextAlignment(3, Qt::AlignLeft);
            curItem->setTextAlignment(4, Qt::AlignLeft);
            curItem->setTextAlignment(5, Qt::AlignLeft);
        }
        // Add the total line
        amount = amount.sprintf("%.2f", total);
        TACCReportItem *totItem = new TACCReportItem(repBody);
        totItem->setIsTotalLine(true);
        totItem->setText(0, "Total");
        totItem->setText(1, amount);
        totItem->setTextAlignment(0, Qt::AlignLeft);
        totItem->setTextAlignment(1, Qt::AlignRight);
        QFont tmpFont = totItem->font(0);
        tmpFont.setWeight(QFont::Bold);
        totItem->setFont(0, tmpFont);
        totItem->setFont(1, tmpFont);
    }

    for (int i = 0; i < repBody->columnCount(); i++) repBody->resizeColumnToContents(i);
    repBody->sortItems(0, Qt::AscendingOrder);
    repBody->setSortingEnabled(true);

    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report, opening a ccPaymentDetails report for the specified
 * day.
 */
void CheckPaymentsReport::listItemSelected(QTreeWidgetItem *curItem, int)
{
    if (curItem != NULL) {
        // If they double click, open the customer's window.
        int custID = curItem->text(2).toInt();
        if (custID) {
            emit openCustomer(custID);
        }
    }
}



// vim: expandtab
