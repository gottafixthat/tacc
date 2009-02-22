/**
 * ccPaymentDetailReport.cpp - Credit Card Payments detail report
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>

#include <ccPaymentDetailReport.h>
#include <QSqlDbPool.h>
#include <CCValidate.h>
#include <mcve.h>
#include <TAATools.h>
#include <BString.h>

/**
 * ccPaymentDetailReport()
 *
 * Constructor.
 */
ccPaymentDetailReport::ccPaymentDetailReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption("Credit Card Transaction Detail Report");
	setTitle("Credit Card Transaction Detail Report");
	
	list->setColumnText(0, "Date");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Amount");       list->setColumnAlignment(1, AlignRight);
	list->addColumn("Cust ID");      list->setColumnAlignment(2, AlignLeft);
	list->addColumn("Customer Name");list->setColumnAlignment(3, AlignLeft);
	list->addColumn("Company");      list->setColumnAlignment(4, AlignLeft);
	list->addColumn("CC Type");      list->setColumnAlignment(5, AlignLeft);
	list->addColumn("Disposition");  list->setColumnAlignment(6, AlignLeft);
	
    allowDates(REP_ALLDATES);
    allowFilters(1);
}


/**
 * ~ccPaymentDetailReport()
 * 
 * Destructor.
 */
ccPaymentDetailReport::~ccPaymentDetailReport()
{
}

/**
 * refreshReport()
 *
 * A virtual function that gets called when the user changes
 * one of the dates or otherwise needs to refresh the report.
 */
void ccPaymentDetailReport::refreshReport()
{
    QSqlDbPool  dbpool;
    QSqlQuery   q(dbpool.qsqldb());
    QString     customerName;
    QString     companyName;
    QString     ccType;
    QString     disposition;
    QString     amount;
    double      total = 0.00;

    QApplication::setOverrideCursor(waitCursor);
    list->clear();

    // Create our query.
    int transDateCol    = 0;
    int amountCol       = 1;
    int customerIDCol   = 2;
    int fullNameCol     = 3;
    int contactNameCol  = 4;
    int ccTypeCol       = 5;
    int mcveStatusCol   = 6;
    q.prepare("select CCTrans.TransDate, CCTrans.Amount, CCTrans.CustomerID, Customers.FullName, Customers.ContactName, CCTrans.CardType, CCTrans.MCVE_Status from CCTrans, Customers where CCTrans.TransDate >= :startDate and CCTrans.TransDate <= :endDate and Customers.CustomerID = CCTrans.CustomerID order by CCTrans.InternalID");
    q.bindValue(":startDate",   startDate().toString("yyyy-MM-dd"));
    q.bindValue(":endDate",     endDate().toString("yyyy-MM-dd"));
    if (q.exec()) {
        while(q.next()) {
            total += q.value(amountCol).toDouble();
            amount = q.value(amountCol).toString();
            amount = amount.sprintf("%.2f", amount.toDouble());

            customerName = "";
            companyName  = "";
            // Check for a company name.
            if (q.value(contactNameCol).toString().length()) {
                customerName = q.value(contactNameCol).toString();
                companyName  = q.value(fullNameCol).toString();
            } else {
                customerName = q.value(fullNameCol).toString();
            }

            switch(q.value(ccTypeCol).toInt()) {
                case CCTYPE_VISA:
                    ccType = "Visa";
                    break;
                case CCTYPE_MC:
                    ccType = "Mastercard";
                    break;
                case CCTYPE_AMEX:
                    ccType = "AmEx";
                    break;
                case CCTYPE_DISC:
                    ccType = "Discover";
                    break;
                default:
                    ccType = "Other";
                    break;
            }

            switch(q.value(mcveStatusCol).toInt()) {
                case MCVE_ERROR:
                    disposition = "MCVE Error";
                    break;
                case MCVE_FAIL:
                    disposition = "Fail";
                    break;
                case MCVE_SUCCESS:
                    disposition = "Success";
                    break;
                case MCVE_AUTH:
                    disposition = "Auth";
                    break;
                case MCVE_DENY:
                    disposition = "Deny";
                    break;
                case MCVE_CALL:
                    disposition = "Call";
                    break;
                case MCVE_DUPL:
                    disposition = "Duplicate";
                    break;
                case MCVE_PKUP:
                    disposition = "Pickup";
                    break;
                case MCVE_RETRY:
                    disposition = "Retry";
                    break;
                case MCVE_TIMEOUT:
                    disposition = "Timeout";
                    break;
                default:
                    disposition = "Uknown";
                    break;
            }


            (void) new QListViewItem(list, 
                    q.value(transDateCol).toString(),
                    amount,
                    q.value(customerIDCol).toString(),
                    customerName,
                    companyName,
                    ccType,
                    disposition);
        }
        // Add the total line
        amount = amount.sprintf("%.2f", total);
        (void) new QListViewItem(list, 
                "Total",
                amount);
    }

    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report.
 */
void ccPaymentDetailReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        // Open the customer window
        QString tmpStr = curItem->key(2,0);
        if (tmpStr.toInt() > 0) {
            emit(openCustomer(tmpStr.toInt()));
        }
    }
}


