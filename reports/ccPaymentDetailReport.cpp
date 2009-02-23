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

#include <qlabel.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>


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
    opts = new ccPaymentDetailOptions();
    connect(opts, SIGNAL(optionsUpdated()), this, SLOT(refreshReport()));
    opts->hide();
}


/**
 * ~ccPaymentDetailReport()
 * 
 * Destructor.
 */
ccPaymentDetailReport::~ccPaymentDetailReport()
{
    delete opts;
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
    QString     cardTypePart;
    QString     query;
    QString     extraq;
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
    if (opts->cardTypeSet().length()) {
        cardTypePart = "CCTrans.CardType in (";
        cardTypePart += opts->cardTypeSet();
        cardTypePart += ")";
    }

    extraq = "";
    if (opts->cardTypeSet().length()) {
        if (extraq.length()) extraq += " and ";
        extraq += "CCTrans.CardType in (";
        extraq += opts->cardTypeSet();
        extraq += ")";
    }
    if (opts->dispositionSet().length()) {
        if (extraq.length()) extraq += " and ";
        extraq += "CCTrans.MCVE_Status in (";
        extraq += opts->dispositionSet();
        extraq += ")";
    }
    query = "select CCTrans.TransDate, CCTrans.Amount, CCTrans.CustomerID, Customers.FullName, Customers.ContactName, CCTrans.CardType, CCTrans.MCVE_Status from CCTrans, Customers where ";
    if (extraq.length()) {
        query += extraq;
        query += " and ";
    }
    query += "CCTrans.TransDate >= :startDate and CCTrans.TransDate <= :endDate and Customers.CustomerID = CCTrans.CustomerID order by CCTrans.InternalID";
    q.prepare(query);
    q.bindValue(":cardTypes",   cardTypePart);
    q.bindValue(":startDate",   startDate().toString("yyyy-MM-dd"));
    q.bindValue(":endDate",     endDate().toString("yyyy-MM-dd"));
    if (q.exec()) {
        debug(1, "Last Query: '%s'\n", q.lastQuery().ascii());
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
    debug(1, "Last Query: '%s'\n", q.lastQuery().ascii());

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

/**
 * editFilters()
 *
 * Shows the ccPaymentDetailOptions window.
 */
void ccPaymentDetailReport::editFilters()
{
    opts->show();
}


/**
 * ccPaymentDetailOptions()
 *
 * Constructor.
 */
ccPaymentDetailOptions::ccPaymentDetailOptions(QWidget *parent, const char *name) :
    TAAWidget(parent, name, 0)
{
    setCaption("Credit Card Payment Detail Report Options");

    QLabel  *cardTypeLabel = new QLabel(this, "cardTypeLabel");
    cardTypeLabel->setText("Card Types:");
    cardTypeLabel->setAlignment(Qt::AlignRight);

    allCardTypes = new QCheckBox("All Card Types", this, "allCardTypes");
    allCardTypes->setChecked(true);
    connect(allCardTypes, SIGNAL(stateChanged(int)), this, SLOT(allCardTypesChanged(int)));

    cardTypeList = new QListBox(this, "cardTypeList");
    cardTypeList->setSelectionMode(QListBox::Multi);
    cardTypeList->insertItem("Mastercard");
    cardTypeList->insertItem("Visa");
    cardTypeList->insertItem("American Express");
    cardTypeList->insertItem("Discover");
    cardTypeList->insertItem("Other");

    QLabel  *dispTypeLabel = new QLabel(this, "dispTypeLabel");
    dispTypeLabel->setText("Disposition:");
    dispTypeLabel->setAlignment(Qt::AlignRight);

    allDispTypes = new QCheckBox("All Transaction Results", this, "allDispTypes");
    allDispTypes->setChecked(true);
    connect(allDispTypes, SIGNAL(stateChanged(int)), this, SLOT(allDispTypesChanged(int)));

    dispList = new QListBox(this, "dispList");
    dispList->setSelectionMode(QListBox::Multi);
    dispList->insertItem("Failure");
    dispList->insertItem("Success");
    dispList->insertItem("Auth");
    dispList->insertItem("Deny");
    dispList->insertItem("Call");
    dispList->insertItem("Duplicate");
    dispList->insertItem("Pick Up");
    dispList->insertItem("Retry");
    dispList->insertItem("Setup");
    dispList->insertItem("Timeout");

    QPushButton *updateButton = new QPushButton(this, "updateButton");
    updateButton->setText("&Update");
    connect(updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));

    QPushButton *closeButton = new QPushButton(this, "closebutton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    
    // Grid for the main widgets
    QGridLayout *gl = new QGridLayout(2, 4);
    int curRow = 0;
    gl->addWidget(cardTypeLabel,        curRow, 0);
    gl->addWidget(allCardTypes,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(cardTypeList,         curRow, 1);
    gl->setRowStretch(curRow, 1);
    curRow++;

    gl->addWidget(dispTypeLabel,        curRow, 0);
    gl->addWidget(allDispTypes,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(dispList,             curRow, 1);
    gl->setRowStretch(curRow, 1);
    curRow++;

    ml->addLayout(gl, 1);
    // Our buttons
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(updateButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    allCardTypesChanged(0);
    allDispTypesChanged(0);
}

/**
 * ~ccPaymentDetailOptions()
 *
 * Destructor.
 */
ccPaymentDetailOptions::~ccPaymentDetailOptions()
{
}

/**
 * cardTypeSet()
 *
 * Returns a numeric list of ID's for the report to select
 * the credit card type from.  This list is appropriate for 
 * inserting into the query, i.e. "1,2,4", so the query would
 * look like "where CardType in (1,2,4)".
 */
const QString ccPaymentDetailOptions::cardTypeSet()
{
    QString     retVal = "";

    // I'm sure there is a better way to do this, but I don't want to
    // think about this too much.
    if (!allCardTypes->isChecked()) {
        for (uint i = 0; i < cardTypeList->count(); i++) {
            if (cardTypeList->isSelected(i)) {
                if (retVal.length()) retVal += ",";
                retVal += QString("%1") . arg(i);
            }
        }
    }
    return retVal;
}

/**
 * dispositionSet()
 *
 * Returns a numeric list of MCVE transaction types for the report
 * to list.
 * This list is appropriate for inserting into the query, i.e. 
 * "1,2,4", so the query would look like "where MCVEStatus in (1,2,4)".
 */
const QString ccPaymentDetailOptions::dispositionSet()
{
    QString     retVal = "";

    // I'm sure there is a better way to do this, but I don't want to
    // think about this too much.
    if (!allDispTypes->isChecked()) {
        for (uint i = 0; i < dispList->count(); i++) {
            if (dispList->isSelected(i)) {
                if (retVal.length()) retVal += ",";
                retVal += QString("%1") . arg(i);
            }
        }
    }
    return retVal;
}

/**
 * updateClicked()
 *
 * Gets called when the user hits the "Update" button.
 * It emits the optionsUpdated signal and hides itself.
 */
void ccPaymentDetailOptions::updateClicked()
{
    emit(optionsUpdated());
    hide();
}

/**
 * closeClicked()
 *
 * Gets called when the user hits the "Close" button.
 * It hides the window.
 */
void ccPaymentDetailOptions::closeClicked()
{
    hide();
}

/**
 * allCardTypesChanged()
 *
 * Gets called when the allCardTypes checkbox is changed.
 */
void ccPaymentDetailOptions::allCardTypesChanged(int)
{
    if (allCardTypes->isChecked()) {
        cardTypeList->setEnabled(false);
    } else {
        cardTypeList->setEnabled(true);
    }
}

/**
 * allDispTypesChanged()
 *
 * Gets called when the allDispTypes checkbox is changed.
 */
void ccPaymentDetailOptions::allDispTypesChanged(int)
{
    if (allDispTypes->isChecked()) {
        dispList->setEnabled(false);
    } else {
        dispList->setEnabled(true);
    }
}
