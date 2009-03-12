/**
 * AgingReport.cpp - Finds all customers with an open balance that is overdue.
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
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>
#include <qdatetime.h>

#include <BlargDB.h>
#include <BString.h>
#include <Cfg.h>
#include <TAATools.h>

#include <AgingReport.h>

/**
 * AgingReport()
 *
 * Constructor.
 */
AgingReport::AgingReport(QWidget* parent, const char* name) : 
    Report( parent, name )
{
	setCaption( "Aging Report" );
	setTitle("Aging Report");
	
	repBody->setColumnText(0, "Cust ID");       repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Full Name");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Contact");              repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Current");              repBody->setColumnAlignment(3, AlignRight);
	repBody->addColumn("0-30");                 repBody->setColumnAlignment(4, AlignRight);
	repBody->addColumn("31-60");                repBody->setColumnAlignment(5, AlignRight);
	repBody->addColumn("60-90");                repBody->setColumnAlignment(6, AlignRight);
	repBody->addColumn("90+");                  repBody->setColumnAlignment(7, AlignRight);
	repBody->addColumn("Total");                repBody->setColumnAlignment(8, AlignRight);
	
    custIDCol = 0;

    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}

/**
 * ~AgingReport()
 *
 * Destructor.
 */
AgingReport::~AgingReport()
{
}


/**
 * refreshReport()
 *
 * Fills the report with the overdue customers.
 */
void AgingReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);
    emit(setStatus("Generating aging report..."));
    repBody->clear();
    ADB     DB;
    ADB     DB2;
    QDate   today = QDate::currentDate();
    QDate   dueDate = QDate::currentDate();
    QString dueDateSt;
    int     curPos = 0;
    double  totcur         = 0.00;
    double  totoverdue     = 0.00;
    double  totoverdue30   = 0.00;
    double  totoverdue60   = 0.00;
    double  totoverdue90   = 0.00;
    double  baltotal       = 0.00;
    QString curSt;
    QString overdueSt;
    QString overdueSt30;
    QString overdueSt60;
    QString overdueSt90;
    QString baltotalSt;

    // First, get the list of customers that have a balance.
    DB.query("select CustomerID, FullName, ContactName, CurrentBalance from Customers where CurrentBalance > 0.00");
    while (DB.getrow()) {
        emit(setProgress(++curPos, DB.rowCount));
        bool    addEm       = false;
        double  amount      = 0.00;
        double  cleared     = 0.00;
        double  diff        = 0.00;
        double  cur         = 0.00;
        double  overdue     = 0.00;
        double  overdue30   = 0.00;
        double  overdue60   = 0.00;
        double  overdue90   = 0.00;
        int     days        = 0;

        // Now, extract the data from the AcctsRecv table
        DB2.query("select Amount, ClearedAmount, DueDate from AcctsRecv where CustomerID = %s and DueDate <> '0000-00-00' and ClearedAmount <> Amount", DB.curRow["CustomerID"]);
        if (DB2.rowCount) while (DB2.getrow()) {
            dueDateSt = DB2.curRow["DueDate"];
            myDateToQDate(dueDateSt.ascii(), dueDate);
            amount  = atof(DB2.curRow["Amount"]);
            cleared = atof(DB2.curRow["ClearedAmount"]);
            diff = amount - cleared;
            days = dueDate.daysTo(today);

            if (days >= 90) {
                overdue90 += diff;
                totoverdue90 += diff;
                addEm = true;
            } else if (days >= 60) {
                overdue60 += diff;
                totoverdue60 += diff;
                addEm = true;
            } else if (days >= 30) {
                overdue30 += diff;
                totoverdue30 += diff;
                addEm = true;
            } else if (days >= 1) {
                overdue += diff;
                totoverdue += diff;
                addEm = true;
            } else {
                // else its not overdue yet, don't necessarily add them
                cur += diff;
                totcur += diff;
            }
        }

        // Now, add them into the list.
        if (addEm) {
            curSt       = curSt.sprintf("%.2f", cur);
            overdueSt   = overdueSt.sprintf("%.2f", overdue);
            overdueSt30 = overdueSt30.sprintf("%.2f", overdue30);
            overdueSt60 = overdueSt60.sprintf("%.2f", overdue60);
            overdueSt90 = overdueSt90.sprintf("%.2f", overdue90);
            QListViewItem *curItem = new QListViewItem(repBody,
                    DB.curRow["CustomerID"],
                    DB.curRow["FullName"],
                    DB.curRow["ContactName"],
                    curSt,
                    overdueSt,
                    overdueSt30,
                    overdueSt60,
                    overdueSt90);
            curItem->setText(8, DB.curRow["CurrentBalance"]);
            baltotal += atof(DB.curRow["CurrentBalance"]);
        }
    }

    // Put in a total line
    curSt       = curSt.sprintf("%.2f", totcur);
    overdueSt   = overdueSt.sprintf("%.2f", totoverdue);
    overdueSt30 = overdueSt30.sprintf("%.2f", totoverdue30);
    overdueSt60 = overdueSt60.sprintf("%.2f", totoverdue60);
    overdueSt90 = overdueSt90.sprintf("%.2f", totoverdue90);
    baltotalSt  = baltotalSt.sprintf("%.2f", baltotal);
    QListViewItem *curItem = new QListViewItem(repBody,
            "Total",
            "",
            "",
            curSt,
            overdueSt,
            overdueSt30,
            overdueSt60,
            overdueSt90);
    curItem->setText(8, baltotalSt);
    
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
}

/**
 * listItemSelected()
 *
 * Called when the user double clicks an entry in the report.
 * Opens the customer window.
 */
void AgingReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        if (curItem->key(custIDCol,0).toInt()) {
            emit(openCustomer(curItem->key(custIDCol,0).toInt()));
        }
    }
}


