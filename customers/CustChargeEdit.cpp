/*
** $Id: CustChargeEdit.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
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

#define Inherited CustChargeEditData

CustChargeEdit::CustChargeEdit
(
	QWidget* parent,
	const char* name,
	const long CustID,
	const long TransID
)
	:
	Inherited( parent, name )
{
	char	tmpstr[128];
	char	separator[4] = "";
    QDate   curDate = QDate::currentDate();
	ADB     DB;
	CustomersDB CDB;
	
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
	AR.ARDB->setValue("TransDate", chargeDate->getQDate());
    AR.ARDB->setValue("StartDate", startDate->getQDate());
    AR.ARDB->setValue("EndDate",  stopDate->getQDate());
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

