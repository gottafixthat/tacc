/*
** $Id$
**
***************************************************************************
**
** ReceivePayment - Creates the Receive payment window.
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
** $Log: ReceivePayment.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <BlargDB.h>
#include <Cfg.h>
#include <qtooltip.h>
#include <AcctsRecv.h>
#include <qdatetm.h>
#include <qapp.h>
#include "ReceivePayment.h"
#include <qmessagebox.h>
#include <qlayout.h>

#include <TAATools.h>

ReceivePayment::ReceivePayment
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name, WType_Dialog )
{
    myCustID = CustID;
    if (!myCustID) return;

    QApplication::setOverrideCursor(waitCursor);
	setCaption( "Receive Payment" );

    // Create our widgets.
    QLabel  *customerIDHeader = new QLabel(this, "Customer ID Header");
    customerIDHeader->setAlignment(AlignRight|AlignVCenter);
    customerIDHeader->setText("Customer ID:");

    customerIDLabel = new QLabel(this, "Customer ID");
    customerIDLabel->setAlignment(AlignLeft|AlignVCenter);

    QLabel *customerNameHeader = new QLabel(this, "Customer Name Header");
    customerNameHeader->setAlignment(AlignRight|AlignVCenter);
    customerNameHeader->setText("Customer Name:");
    
    customerNameLabel = new QLabel(this, "Customer Name");
    customerNameLabel->setAlignment(AlignLeft|AlignVCenter);

    QLabel *addressHeader = new QLabel(this, "Customer Addr Header");
    addressHeader->setAlignment(AlignRight|AlignVCenter);
    addressHeader->setText("Address:");
    
    addressLabel1 = new QLabel(this, "Customer Address");
    addressLabel1->setAlignment(AlignLeft|AlignVCenter);

    addressLabel2 = new QLabel(this, "Customer Address");
    addressLabel2->setAlignment(AlignLeft|AlignVCenter);
    
    QLabel *phoneHeader = new QLabel(this, "Customer Phone Header");
    phoneHeader->setAlignment(AlignRight|AlignVCenter);
    phoneHeader->setText("Phone:");
    
    phoneLabel = new QLabel(this, "Customer Phone");
    phoneLabel->setAlignment(AlignLeft|AlignVCenter);

    QLabel *dateHeader = new QLabel(this, "Trans Date Header");
    dateHeader->setAlignment(AlignRight|AlignVCenter);
    dateHeader->setText("Date:");
    
    dateLabel = new QLabel(this, "Trans Date");
    dateLabel->setAlignment(AlignLeft|AlignVCenter);

    QLabel *balanceHeader = new QLabel(this, "Balance Header");
    balanceHeader->setAlignment(AlignRight|AlignVCenter);
    balanceHeader->setText("Current Balance:");
    
    balanceLabel = new QLabel(this, "Current Balance");
    balanceLabel->setAlignment(AlignLeft|AlignVCenter);
    
    QLabel *checkNumberHeader = new QLabel(this, "Check Number Header");
    checkNumberHeader->setAlignment(AlignRight|AlignVCenter);
    checkNumberHeader->setText("Check Number:");
    
    checkNumber = new QLineEdit(this, "Check Number");
    checkNumber->setMaxLength(20);
    connect(checkNumber, SIGNAL(returnPressed()), this, SLOT(savePayment()));
    
    QLabel *amountHeader = new QLabel(this, "Check Amount Header");
    amountHeader->setAlignment(AlignRight|AlignVCenter);
    amountHeader->setText("Check Amount:");

    amount = new QLineEdit(this, "Check Amount");
    amount->setMaxLength(20);
    connect(amount, SIGNAL(returnPressed()), this, SLOT(savePayment()));
    
    chargeList = new QListView(this, "Charge List");
    chargeList->setRootIsDecorated(false);
    chargeList->setAllColumnsShowFocus(true);
    chargeList->setMultiSelection(false);
    chargeList->addColumn("Date");
    chargeList->addColumn("Username");
    chargeList->addColumn("Description");
    chargeList->addColumn("Due Date");
    chargeList->addColumn("Charge");
    chargeList->addColumn("Paid");
    chargeList->addColumn("Total");
    for (int i = 4 ; i < 6; i++) chargeList->setColumnAlignment(i, AlignRight);

    saveButton = new QPushButton(this, "Save Button");
    saveButton->setText("&Save");
    saveButton->setAutoDefault(true);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savePayment()));

    cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));

    // Create our layout.  Somewhat complex.  We have the main layout, which
    // is a box, then a grid, then the list, then another box.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout(5, 4, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);
    int curRow = 0;

    gl->addWidget(customerIDHeader,         curRow, 0);
    gl->addWidget(customerIDLabel,          curRow, 1);
    gl->addWidget(dateHeader,               curRow, 2);
    gl->addWidget(dateLabel,                curRow, 3);
    curRow++;

    gl->addWidget(customerNameHeader,       curRow, 0);
    gl->addWidget(customerNameLabel,        curRow, 1);
    curRow++;

    gl->addWidget(addressHeader,            curRow, 0);
    gl->addWidget(addressLabel1,            curRow, 1);
    gl->addWidget(balanceHeader,            curRow, 2);
    gl->addWidget(balanceLabel,             curRow, 3);
    curRow++;

    gl->addWidget(addressLabel2,            curRow, 1);
    gl->addWidget(checkNumberHeader,        curRow, 2);
    gl->addWidget(checkNumber,              curRow, 3);
    curRow++;

    gl->addWidget(phoneHeader,              curRow, 0);
    gl->addWidget(phoneLabel,               curRow, 1);
    gl->addWidget(amountHeader,             curRow, 2);
    gl->addWidget(amount,                   curRow, 3);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addWidget(chargeList, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    fillUncleared();
    loadCustInfo();

    checkNumber->setFocus();
    
    saveButton->setDefault(TRUE);
    
    QApplication::restoreOverrideCursor();

}


ReceivePayment::~ReceivePayment()
{
}

/*
** loadCustInfo - Loads the customer information from the database.
*/

void ReceivePayment::loadCustInfo()
{
    // Now, fill in the header information.  A bit backwards, but thats okay.
    ADB             DB;
    CustomersDB     CDB;
    AddressesDB     addrDB;
    QDate           theDate;
    char            tmpstr[1024];
    
    theDate = QDate::currentDate();
    
    CDB.get(myCustID);
    customerIDLabel->setText(CDB.getStr("CustomerID"));
    customerNameLabel->setText(CDB.getStr("FullName"));
    balanceLabel->setText(CDB.getStr("CurrentBalance"));
    balanceDue = CDB.getFloat("CurrentBalance");
    dateLabel->setText(theDate.toString());
    
    // Get the address for the customer.
    strcpy(tmpstr, CDB.getStr("BillingAddress"));
    addrDB.get(REF_CUSTOMER, myCustID, tmpstr);
    sprintf(tmpstr, "%s  %s", (const char *) addrDB.Address1, (const char *) addrDB.Address2);
    addressLabel1->setText(tmpstr);
    sprintf(tmpstr, "%s, %s %s",
      (const char *) addrDB.City,
      (const char *) addrDB.State,
      (const char *) addrDB.ZIP
    );
    addressLabel2->setText(tmpstr);

	// There is no default phone number, so get the first one from the DB.
	DB.query("select PhoneNumber, Tag from CustomerContacts where CustomerID = %ld", myCustID);
	if (DB.rowCount) {
		DB.getrow();
		sprintf(tmpstr, "%s (%s)", DB.curRow["PhoneNumber"], DB.curRow["Tag"]);
		phoneLabel->setText(tmpstr);
	}
	
}

/*
** fillUncleared  - Fills the table with uncleared charges.
**
*/

void ReceivePayment::fillUncleared(void)
{
    // First, delete all of the rows except the header rows.
    chargeList->clear();
    
    // Now, load all of the customer's uncleared transactions from their
    // AR.
    ADB         DB;
    char        charge[1024];
    char        paid[1024];
    char        total[1024];
    int         rowNum;
    float       runningTotal = 0.00;
    
    DB.query("select TransDate, LoginID, Memo, DueDate, Amount, ClearedAmount from AcctsRecv where CustomerID = %ld and Cleared = 0 and Amount > 0.00 order by TransDate, InternalID", myCustID);
    rowNum  = 0;
    while (DB.getrow()) {
        // Amount
        sprintf(charge, "%10.2f", atof(DB.curRow["Amount"]));

        // ClearedAmount
        sprintf(paid, "%10.2f", atof(DB.curRow["ClearedAmount"]));

        // Running Total
        runningTotal += (atof(DB.curRow["Amount"]) - atof(DB.curRow["ClearedAmount"]));
        sprintf(total, "%10.2f", runningTotal);
        
        (void) new QListViewItem(chargeList, 
          DB.curRow["TransDate"],               // Trans Date
          DB.curRow["LoginID"],                 // Login ID
          DB.curRow["Memo"],                    // Description
          DB.curRow["DueDate"],                 // Due Date
          charge,
          paid,
          total
        );

    }
    chargeList->setSorting(6);  // Sort on the total...
}

/*
** savePayment  - Validates the (heh, two) items and saves the payment
**                into the database.
*/

void ReceivePayment::savePayment()
{
    char    tmpstr[1024];
    int     undepostedFundsAcct;
    float   amtEntered;
    
    strcpy(tmpstr, cfgVal("UndepositedFundsAcct"));
    undepostedFundsAcct = atoi(tmpstr);
    
    if (!undepostedFundsAcct) {
        QMessageBox::critical(this, "No Account", "No undeposited funds account.\n\nContact the administratoNo undeposited funds account.\n\nContact the administrator.");
        return;
    }
    
    // Validate the check number
    strcpy(tmpstr, checkNumber->text());
    if (!strlen(tmpstr)) {
        QMessageBox::critical(this, "Invalid Check Number", "All payments must have an\nassociated check or payment number.");
        checkNumber->setFocus();
        return;
    }
    
    // Validate the amount.
    amtEntered = atof(amount->text());
    if (!(amtEntered > 0.00)) {
        QMessageBox::critical(this, "Invalid Amount", "Payments must have a dollar amount\nhigher than $0.00.  Please correct.");
        amount->setFocus();
        return;
    }
    
    // For some reason, my float comparison didn't seem to work...
    if (amtEntered != balanceDue) {
        sprintf(tmpstr, "The amount due ($%2.2f) is different than the amount entered ($%2.2f).  Do you wish to continue?", balanceDue, amtEntered);
        if (QMessageBox::warning(this, "Differing Amounts", tmpstr, "&Yes", "&No", 0, 1) == 1) {
        // if (!yn.answer("Differing Amounts", tmpstr, 1)) {
            amount->setFocus();
            return;
        }
    }
    
    // If we've made it this far, we want to save.
    CustomersDB CDB;
    AcctsRecv   AR;
    char        dateStr[64];
    QDate       theDate;
    char        memoStr[512];
    
    theDate = QDate::currentDate();
    
    sprintf(dateStr, "%04d-%02d-%02d", theDate.year(), theDate.month(), theDate.day());
    
    sprintf(memoStr, "Payment Received (Check #%s)", (const char *) checkNumber->text());
    
    CDB.get(myCustID);
    AR.ARDB->setValue("TransDate", dateStr);
    AR.ARDB->setValue("CustomerID", myCustID);
    AR.ARDB->setValue("RefNo", checkNumber->text());
    AR.ARDB->setValue("TransType", TRANSTYPE_PAYMENT);
    AR.ARDB->setValue("Price", (float) (atof(amount->text()) * -1.0));
    AR.ARDB->setValue("Amount", (float) (atof(amount->text()) * -1.0));
    AR.ARDB->setValue("Memo", memoStr);
    AR.SaveTrans();

    emit customerUpdated(myCustID);
    close();
    
    
}


/*
** cancelPressed - The slot that is selected when the user hits the cancel
**                 button.  It just closes the window.
*/

void ReceivePayment::cancelPressed()
{
    close();
}

void ReceivePayment::refreshCustomer(long custID)
{
    if (myCustID == custID) {
        loadCustInfo();
        fillUncleared();
    }
}
