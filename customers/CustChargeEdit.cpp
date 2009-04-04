/*
** $Id$
**
***************************************************************************
**
** CustChargEdit.cpp - Allows the user to enter a new charge into the
**                     customers register.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CustChargeEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <ADB.h>
#include <BlargDB.h>
#include <AcctsRecv.h>
#include "CustChargeEdit.h"

using namespace Qt;

CustChargeEdit::CustChargeEdit
(
	QWidget* parent,
	const char* name,
	const long CustID,
	const long TransID
) : TAAWidget(parent)
{
	char	tmpstr[128];
	char	separator[4] = "";
    QDate   curDate = QDate::currentDate();
	ADB     DB;
	CustomersDB CDB;


    // Create our Widgets
    custNameDispLabel   = new QLabel(this, "Customer Name Label");
    custNameDispLabel->setAlignment(AlignRight|AlignVCenter);
    custNameDispLabel->setText("Customer Name:");
    custNameDisp        = new QLabel(this, "Customer Name");
    custNameDisp->setAlignment(AlignLeft|AlignVCenter);

    custIDDispLabel   = new QLabel(this, "Customer ID Label");
    custIDDispLabel->setAlignment(AlignRight|AlignVCenter);
    custIDDispLabel->setText("Customer ID:");
    custIDDisp        = new QLabel(this, "Customer ID");
    custIDDisp->setAlignment(AlignLeft|AlignVCenter);

    QLabel *loginListLabel = new QLabel(this, "LoginListLabel");
    loginListLabel->setText("Login ID:");
    loginListLabel->setAlignment(AlignRight|AlignVCenter);

    loginList = new QComboBox(false, this, "Login List");

    QLabel *chargeDateLabel = new QLabel(this, "chargeDateLabel");
    chargeDateLabel->setText("Charge Date:");
    chargeDateLabel->setAlignment(AlignRight|AlignVCenter);
    chargeDate = new Q3DateEdit(QDate::currentDate(), this);

    QLabel *itemListLabel = new QLabel(this, "itemListLabel");
    itemListLabel->setText("Item:");
    itemListLabel->setAlignment(AlignRight|AlignVCenter);
    itemList  = new QComboBox(false, this, "ItemList");
    connect(itemList, SIGNAL(activated(int)), this, SLOT(itemChanged(int)));

    QLabel *startDateLabel = new QLabel(this, "startDateLabel");
    startDateLabel->setText("Start Date:");
    startDateLabel->setAlignment(AlignRight|AlignVCenter);
    startDate = new Q3DateEdit(QDate::currentDate(), this);
    connect(startDate, SIGNAL(valueChanged(const QDate &)), this, SLOT(periodChanged(const QDate &)));
    QLabel *stopDateLabel = new QLabel(this, "stopDateLabel");
    stopDateLabel->setText("End Date:");
    stopDateLabel->setAlignment(AlignRight|AlignVCenter);
    stopDate   = new Q3DateEdit(QDate::currentDate(), this);
    connect(stopDate, SIGNAL(valueChanged(const QDate &)), this, SLOT(periodChanged(const QDate &)));

    QLabel *quantityLabel = new QLabel(this, "quantityLabel");
    quantityLabel->setText("Quantity:");
    quantityLabel->setAlignment(AlignRight|AlignVCenter);
    quantity = new QLineEdit(this, "Quantity");
    connect(quantity, SIGNAL(textChanged(const QString&)), this, SLOT(quantityChanged(const QString &)));

    QLabel *priceLabel = new QLabel(this, "priceLabel");
    priceLabel->setText("Price:");
    priceLabel->setAlignment(AlignRight|AlignVCenter);
    price    = new QLineEdit(this, "Price");
    connect(price, SIGNAL(textChanged(const QString&)), this, SLOT(priceChanged(const QString&)));

    QLabel *unitsLabel = new QLabel(this, "unitsLabel");
    unitsLabel->setText("Units:");
    unitsLabel->setAlignment(AlignRight|AlignVCenter);
    units    = new QLineEdit(this, "Units");

    taxable  = new QCheckBox(this, "Taxable");
    taxable->setText("Taxable");
    
    QLabel *memoLabel = new QLabel(this, "memoLabel");
    memoLabel->setText("Memo:");
    memoLabel->setAlignment(AlignRight|AlignTop);
    memo     = new Q3MultiLineEdit(this, "Memo");

    QLabel *totalChargeLabel = new QLabel(this, "totalChargeLabel");
    totalChargeLabel->setText("Total Charge:");
    totalChargeLabel->setAlignment(AlignRight|AlignVCenter);
    totalCharge     = new QLabel(this, "Total Charge");
    ratePlanDispLabel = new QLabel(this, "ratePlanDispLabel");
    ratePlanDispLabel->setText("Rate Plan:");
    ratePlanDispLabel->setAlignment(AlignRight|AlignVCenter);
    ratePlanLabel   = new QLabel(this, "Rate Plan");
    cycleDispLabel = new QLabel(this, "cycleDispLabel");
    cycleDispLabel->setText("Billing Cycle:");
    cycleDispLabel->setAlignment(AlignRight|AlignVCenter);
    cycleLabel      = new QLabel(this, "Billing Cycle");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCharge()));

    cancelButton = new QPushButton(this, "saveButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
	
    // Create the layout now.
    // The main layout first, a box layout from top to bottom.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);


    // A grid to hold all of our input fields.
    Q3GridLayout *gl = new Q3GridLayout(7, 4);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);

    int row = 0;
    gl->addWidget(custNameDispLabel,                row, 0);
    gl->addWidget(custNameDisp,                     row, 1);
    gl->addWidget(custIDDispLabel,                  row, 2);
    gl->addWidget(custIDDisp,                       row, 3);
    row++;

    gl->addWidget(ratePlanDispLabel,                row, 0);
    gl->addWidget(ratePlanLabel,                    row, 1);
    gl->addWidget(cycleDispLabel,                   row, 2);
    gl->addWidget(cycleLabel,                       row, 3);
    row++;

    gl->addWidget(loginListLabel,                   row, 0);
    gl->addWidget(loginList,                        row, 1);
    row++;
    
    gl->addWidget(chargeDateLabel,                  row, 0);
    gl->addWidget(chargeDate,                       row, 1);
    gl->addWidget(itemListLabel,                    row, 2);
    gl->addWidget(itemList,                         row, 3);
    row++;

    gl->addWidget(startDateLabel,                   row, 0);
    gl->addWidget(startDate,                        row, 1);
    gl->addWidget(stopDateLabel,                    row, 2);
    gl->addWidget(stopDate,                         row, 3);
    row++;
    
    gl->addWidget(quantityLabel,                    row, 0);
    gl->addWidget(quantity,                         row, 1);
    gl->addWidget(priceLabel,                       row, 2);
    gl->addWidget(price,                            row, 3);
    row++;

    gl->addWidget(unitsLabel,                       row, 0);
    gl->addWidget(units,                            row, 1);
    gl->addWidget(taxable,                          row, 3);
    row++;

    gl->addWidget(totalChargeLabel,                 row, 2);
    gl->addWidget(totalCharge,                      row, 3);
    row++;

    gl->addWidget(memoLabel,                        row, 0);
    gl->addMultiCellWidget(memo,                    row, row, 1, 3);
    gl->setRowStretch(row, 1);
    row++;

    ml->addLayout(gl, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    // Done with the layout


    
	myCustID = CustID;
	myTransID = TransID;
	myGL = new GenLedger;
	
	if (!myCustID) return;
	CDB.get(myCustID);
	
	if (myTransID) {
        if (strlen((const char *)CDB.getStr("ContactName"))) strcpy(separator, "/");
        sprintf(tmpstr, "Edit Charge - %s%s%s (%ld)", (const char *) CDB.getStr("FullName"), separator, (const char *) CDB.getStr("ContactName"), myCustID);
        setCaption(tmpstr);
    } else {
        if (strlen((const char *)CDB.getStr("ContactName"))) strcpy(separator, "/");
        sprintf(tmpstr, "New Charge - %s%s%s (%ld)", (const char *) CDB.getStr("FullName"), separator, (const char *) CDB.getStr("ContactName"), myCustID);
        setCaption(tmpstr);
    }

	// Setup the labels at the top, complete with the customer name,
	// rate plan, and billing cycle.
	if (strlen((const char *)CDB.getStr("ContactName"))) strcpy(separator, "/");
	sprintf(tmpstr, "%s%s%s", (const char *) CDB.getStr("FullName"), separator, (const char *) CDB.getStr("ContactName"));
	custNameDisp->setText(tmpstr);
	sprintf(tmpstr, "%ld", myCustID);
    custIDDisp->setText(tmpstr);

	DB.query("select PlanTag from RatePlans where InternalID = %ld", CDB.getLong("RatePlan"));
	if (DB.rowCount) {
		DB.getrow();
		ratePlanLabel->setText(DB.curRow["PlanTag"]);
	} else {
		ratePlanLabel->setText("Unkown!");
	}

	DB.query("select CycleID from BillingCycles where InternalID = %ld", CDB.getLong("BillingCycle"));
	if (DB.rowCount) {
		DB.getrow();
		cycleLabel->setText(DB.curRow["CycleID"]);
	} else {
		cycleLabel->setText("Unknown!");
	}
	
	myRatePlan = CDB.getLong("RatePlan");
	myCycleID  = CDB.getLong("BillingCycle");

	// Fill the Login ID combo box...
	loginList->insertItem("None");
	DB.query("select LoginID from Logins where CustomerID = %ld order by LoginID", myCustID);
	while (DB.getrow()) {
		loginList->insertItem(DB.curRow["LoginID"]);
	}
	
	// Fill the Billable Item combo-boxe
	DB.query("select ItemNumber, ItemID from Billables order by ItemID");
	itemIDX = new(long[DB.rowCount + 1]);
	long idxPtr = 0;
	while (DB.getrow()) {
		itemIDX[idxPtr++] = atol(DB.curRow["ItemNumber"]);
		itemList->insertItem(DB.curRow["ItemID"]);
	}

    // Set up the charge date and date ranges.
    chargeDate->setDate(QDate::currentDate());

    QDate   tmpQDate;
    tmpQDate.setYMD(curDate.year(), curDate.month(), 1);
    startDate->setDate(tmpQDate);
    tmpQDate.setYMD(curDate.year(), curDate.month(), curDate.daysInMonth());
    stopDate->setDate(tmpQDate);
	
	// Also insert the quantity.
	quantity->setText("1");
	
	itemChanged(itemList->currentItem());
}


CustChargeEdit::~CustChargeEdit()
{
}

void CustChargeEdit::saveCharge()
{
	ADB		    DB;
	AcctsRecv 	AR;
	
	AR.ARDB->setValue("CustomerID", myCustID);
	AR.ARDB->setValue("LoginID", loginList->currentText().ascii());
	AR.ARDB->setValue("ItemID", itemIDX[itemList->currentItem()]);
	AR.ARDB->setValue("Quantity", (float)atof(quantity->text()));
	AR.ARDB->setValue("Price", (float) atof(price->text()));
	AR.ARDB->setValue("Amount", (float) (atof(quantity->text()) * atof(price->text())));
	AR.ARDB->setValue("TransDate", chargeDate->date());
    AR.ARDB->setValue("StartDate", startDate->date());
    AR.ARDB->setValue("EndDate",  stopDate->date());
	AR.ARDB->setValue("Memo", memo->text().ascii());
	AR.SaveTrans();
	
	emit(chargeSaved(1));
	
	close();
}

void CustChargeEdit::cancelPressed()
{
	close();
}
/*
** itemChanged - When the user selects a new item from the list, this slot
**               will be called.  It should plug data into the Price, Units,
**               and description fields.
*/

void CustChargeEdit::itemChanged(int newItemNo)
{
	BillablesDB	BDB;
	BillablesDataDB BDDB;
	
	BDB.get(itemIDX[newItemNo]);
	BDDB.getItem(itemIDX[newItemNo], myRatePlan, myCycleID);
	price->setText(BDDB.getStr("Price"));
	units->setText(BDDB.getStr("Units"));
	memo->setText(BDB.getStr("Description"));
}

/*
** quantityChanged - Updates the total charge whenever the quantity changes.
**
*/

void CustChargeEdit::quantityChanged(const QString &newVal)
{
	float	tmpQty;
	float	tmpPrice;
	float	newTotal;
	char	tmpstr[64];
	
	tmpQty = newVal.toFloat();
	tmpPrice = atof((const char *) price->text());
	newTotal = tmpQty * tmpPrice;
	sprintf(tmpstr, "%12.2f", newTotal);
	totalCharge->setText(tmpstr);

}

/*
** priceChanged - Updates the total charge whenever the quantity changes.
**
*/

void CustChargeEdit::priceChanged(const QString &newVal)
{
	float	tmpQty;
	float	tmpPrice;
	float	newTotal;
	char	tmpstr[64];
	
	
	tmpPrice = newVal.toFloat();
	tmpQty = atof((const char *) quantity->text());
	newTotal = tmpQty * tmpPrice;
	sprintf(tmpstr, "%12.2f", newTotal);
	totalCharge->setText(tmpstr);

}

/*
** periodChanged - Gets called whenever the start/stop date changes.  
**                 If the quantity has not been updated manually
**                 it recalculates the quantity based on the
**                 start and stop dates.
*/

void CustChargeEdit::periodChanged(const QDate &)
{
    calculateQuantity();
}

/*
** calculateQuantity - Calculates the quantity based on the start
**                     and stop dates.
*/

void CustChargeEdit::calculateQuantity()
{
    float   baseQty = 0.0;
    QDate   myStartDate = startDate->date();
    QDate   myStopDate  = stopDate->date();

    // Only proceed if the start date is less than the stop date
    if (myStartDate > myStopDate) {
        stopDate->setDate(myStartDate);
        quantity->setText("0");
        return;
    }

    // Okay, the stopDate is greater than the startDate.

    // Count all of the months and partial months 
    // from the start date until it reaches the
    // stopDate's month and year.

    QDate tmpEndDate;
    tmpEndDate.setYMD(myStopDate.year(), myStopDate.month(), 1);
    while (myStartDate < tmpEndDate) {
        if (myStartDate.day() != 1) {
            // Partial month.
            float tmpAmt = (float) (myStartDate.daysInMonth() - myStartDate.day() + 1) / myStartDate.daysInMonth();
            baseQty += tmpAmt;
            myStartDate.setYMD(myStartDate.year(), myStartDate.month(), 1);
        } else {
            // Whole month.
            baseQty += 1.0;
        }
        myStartDate = myStartDate.addMonths(1);
    }

    // Check to see if we're doing a partial quantity in the current month
    if ((myStartDate.day() == 1) && (myStopDate.day() == myStopDate.daysInMonth())) {
        // Full month.
        baseQty += 1.0;
    } else {
        // Partial month
        float tmpAmt = (float) (myStopDate.day() - myStartDate.day() + 1) / myStartDate.daysInMonth();
        baseQty += tmpAmt;

    }

    char tmpstr[1024];
	sprintf(tmpstr, "%.2f", baseQty);
	quantity->setText(tmpstr);
}


