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

#include "CustChargeEdit.h"
#include "BlargDB.h"
#include "AcctsRecv.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <qdatetm.h>
#include <ADB.h>
#include <qlayout.h>

CustChargeEdit::CustChargeEdit
(
	QWidget* parent,
	const char* name,
	const long CustID,
	const long TransID
) : TAAWidget( parent, name )
{
	char	tmpstr[128];
	char	separator[4] = "";
    QDate   curDate = QDate::currentDate();
	ADB     DB;
	CustomersDB CDB;


    // Create our Widgets
    custNameLabel   = new QLabel(this, "Customer Name Label");

    QLabel *loginListLabel = new QLabel(this, "LoginListLabel");
    loginListLabel->setText("Login ID:");

    loginList = new QComboBox(false, this, "Login List");

    QLabel *chargeDateLabel = new QLabel(this, "chargeDateLabel");
    chargeDateLabel->setText("Charge Date:");
    chargeDate = new QDateEdit(QDate::currentDate(), this);

    QLabel *itemListLabel = new QLabel(this, "itemListLabel");
    itemListLabel->setText("Item:");
    itemList  = new QComboBox(false, this, "ItemList");

    QLabel *startDateLabel = new QLabel(this, "startDateLabel");
    startDateLabel->setText("Start Date:");
    startDate = new QDateEdit(QDate::currentDate(), this);
    QLabel *stopDateLabel = new QLabel(this, "stopDateLabel");
    stopDateLabel->setText("End Date:");
    stopDate   = new QDateEdit(QDate::currentDate(), this);

    QLabel *quantityLabel = new QLabel(this, "quantityLabel");
    quantityLabel->setText("End Date:");
    quantity = new QLineEdit(this, "Quantity");
    taxable  = new QCheckBox(this, "Taxable");
    taxable->setText("Taxable");
    QLabel *priceLabel = new QLabel(this, "priceLabel");
    priceLabel->setText("Price:");
    price    = new QLineEdit(this, "Price");
    QLabel *unitsLabel = new QLabel(this, "unitsLabel");
    unitsLabel->setText("Units:");
    units    = new QLineEdit(this, "Units");
    QLabel *memoLabel = new QLabel(this, "memoLabel");
    memoLabel->setText("Memo:");
    memo     = new QMultiLineEdit(this, "Memo");

    QLabel *totalChargeLabel = new QLabel(this, "totalChargeLabel");
    totalChargeLabel->setText("Total Charge:");
    totalCharge     = new QLabel(this, "Total Charge");
    ratePlanLabel   = new QLabel(this, "Rate Plan");
    cycleLabel      = new QLabel(this, "Billing Cycle");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    cancelButton = new QPushButton(this, "saveButton");
    cancelButton->setText("&Cancel");
	
    // Create the layout now.
    // The main layout first, a box layout from top to bottom.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    ml->addWidget(custNameLabel, 0);

    // A grid to hold all of our input fields.
    QGridLayout *gl = new QGridLayout(7, 4);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);

    int row = 0;
    gl->addWidget(ratePlanLabel,                    row, 0);
    gl->addWidget(cycleLabel,                       row, 2);
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
    gl->addWidget(taxable,                          row, 2);
    gl->addWidget(price,                            row, 3);
    row++;

    gl->addWidget(unitsLabel,                       row, 0);
    gl->addWidget(units,                            row, 1);
    gl->addWidget(totalChargeLabel,                 row, 2);
    gl->addWidget(totalCharge,                      row, 3);
    row++;

    gl->addWidget(memoLabel,                        row, 0);
    gl->addMultiCellWidget(memo,                    row, row, 1, 3);
    gl->setRowStretch(row, 1);
    row++;

    ml->addLayout(gl, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
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
	
	if (myTransID) setCaption("Edit Customer Charge");
	else setCaption( "New Customer Charge" );

	// Setup the labels at the top, complete with the customer name,
	// rate plan, and billing cycle.
	if (strlen((const char *)CDB.getStr("ContactName"))) strcpy(separator, "/");
	sprintf(tmpstr, "%ld, %s%s%s", myCustID, (const char *) CDB.getStr("FullName"), separator, (const char *) CDB.getStr("ContactName"));
	custNameLabel->setText(tmpstr);
	
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
	AR.ARDB->setValue("LoginID", loginList->currentText());
	AR.ARDB->setValue("ItemID", itemIDX[itemList->currentItem()]);
	AR.ARDB->setValue("Quantity", (float)atof(quantity->text()));
	AR.ARDB->setValue("Price", (float) atof(price->text()));
	AR.ARDB->setValue("Amount", (float) (atof(quantity->text()) * atof(price->text())));
	AR.ARDB->setValue("TransDate", chargeDate->date());
    AR.ARDB->setValue("StartDate", startDate->date());
    AR.ARDB->setValue("EndDate",  stopDate->date());
	AR.ARDB->setValue("Memo", memo->text());
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

