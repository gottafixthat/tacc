/*
** $Id$
**
***************************************************************************
**
** CCEntry - Takes a credit card payment from the user.
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
** $Log: CCEntry.cpp,v $
** Revision 1.5  2004/04/15 17:57:22  marc
** Zero dollar amounts will no longer post a transaction.  Mantis bug ID 0000107.
**
** Revision 1.4  2004/01/19 20:57:13  marc
** Logic bug fix that made the CVV code required
**
** Revision 1.3  2004/01/13 21:30:02  marc
** Fixed bug that inserted zero dollar amount transactions.
**
** Revision 1.2  2004/01/13 01:15:58  marc
** Added CV/Security Code processing and storage.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CCEntry.h"
#include "BlargDB.h"
#include "BString.h"
#include <AcctsRecv.h>
#include <qdatetm.h>
#include <stdio.h>
#include <stdlib.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <qhbuttongroup.h>
#include <qlayout.h>
#include <qmessagebox.h>

#include <TAATools.h>
#include <ADB.h>

CCEntry::CCEntry
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
	if (!CustID) return;

    // Create our widgets.
    QLabel  *custNameLabel  = new QLabel(this);
    custNameLabel->setAlignment(AlignRight|AlignVCenter);
    custNameLabel->setText("Cardholder name:");

    custName = new QLineEdit(this);
    custName->setMaxLength(80);

    QLabel  *addr1Label  = new QLabel(this);
    addr1Label->setAlignment(AlignRight|AlignVCenter);
    addr1Label->setText("Address:");

    addr1 = new QLineEdit(this);
    addr1->setMaxLength(80);

    QLabel  *addr2Label  = new QLabel(this);
    addr2Label->setAlignment(AlignRight|AlignVCenter);
    addr2Label->setText("Address (cont):");

    addr2 = new QLineEdit(this);
    addr2->setMaxLength(80);

    QLabel  *cityLabel  = new QLabel(this);
    cityLabel->setAlignment(AlignRight|AlignVCenter);
    cityLabel->setText("City:");

    city = new QLineEdit(this);
    city->setMaxLength(80);

    QLabel  *stateLabel  = new QLabel(this);
    stateLabel->setAlignment(AlignRight|AlignVCenter);
    stateLabel->setText("State:");

    state = new QLineEdit(this);
    state->setMaxLength(2);
    
    QLabel  *zipLabel  = new QLabel(this);
    zipLabel->setAlignment(AlignRight|AlignVCenter);
    zipLabel->setText("ZIP:");

    zip = new QLineEdit(this);
    zip->setMaxLength(10);
    
    QLabel  *phoneLabel  = new QLabel(this);
    phoneLabel->setAlignment(AlignRight|AlignVCenter);
    phoneLabel->setText("Phone Number:");

    phone = new QLineEdit(this);
    phone->setMaxLength(15);
    
    QLabel  *chargeAmountLabel  = new QLabel(this);
    chargeAmountLabel->setAlignment(AlignRight|AlignVCenter);
    chargeAmountLabel->setText("Charge Amount:");

    chargeAmount = new QLineEdit(this);
    chargeAmount->setMaxLength(10);
    
    QLabel  *cardTypeLabel  = new QLabel(this);
    cardTypeLabel->setAlignment(AlignRight|AlignVCenter);
    cardTypeLabel->setText("Card Type:");

    cardType = new QComboBox(false, this);
    cardType->insertItem("Mastercard");
    cardType->insertItem("Visa");
    cardType->insertItem("American Express");

    QLabel  *cardNumberLabel  = new QLabel(this);
    cardNumberLabel->setAlignment(AlignRight|AlignVCenter);
    cardNumberLabel->setText("Card Number:");

    cardNumber = new QLineEdit(this);
    cardNumber->setMaxLength(18);

    QLabel  *expDateLabel  = new QLabel(this);
    expDateLabel->setAlignment(AlignRight|AlignVCenter);
    expDateLabel->setText("Expiration Date:");

    expDate = new QLineEdit(this);
    expDate->setMaxLength(18);

    QLabel  *cvInfoLabel  = new QLabel(this);
    cvInfoLabel->setAlignment(AlignRight|AlignVCenter);
    cvInfoLabel->setText("CV Security Code:");

    cvInfo = new QLineEdit(this);
    cvInfo->setMaxLength(4);

    autoPay = new QCheckBox(this, "AutoPay");
    autoPay->setText("Use as automatic payment method");


    QHButtonGroup   *chargeDateGroup = new QHButtonGroup(this, "ChargeDateGroup");
    chargeDateGroup->setTitle("Auto Charge Date");
    chargeDateGroup->setExclusive(true);

    chargeDate1 = new QRadioButton(chargeDateGroup);
    chargeDate1->setText("7th");
    chargeDate1->setChecked(true);

    chargeDate2 = new QRadioButton(chargeDateGroup);
    chargeDate2->setText("16th");
    chargeDate2->setChecked(false);

    QLabel *balanceLabel = new QLabel(this);
    balanceLabel->setAlignment(AlignRight);
    balanceLabel->setText("Current Balance:");

    balance = new QLabel(this);
    balance->setAlignment(AlignRight);

    dateLabel = new QLabel(this);
    dateLabel->setAlignment(AlignRight);

    saveButton = new QPushButton(this);
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCCard()));
    
    cancelButton = new QPushButton(this);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelCCard()));


    // Setup the layout for this window.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // The entry widgets go into a grid 4 columns wide.
    QGridLayout *ewl = new QGridLayout(4, 5, 2);
    ewl->setColStretch(0, 0);
    ewl->setColStretch(1, 1);
    ewl->setColStretch(2, 1);
    ewl->setColStretch(3, 1);
    
    int curRow = -1;
    int curCol = 0;

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(custNameLabel,       curRow, curCol++);
    ewl->addWidget(custName,            curRow, curCol++);
    curCol++;
    ewl->addWidget(dateLabel,           curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(addr1Label,          curRow, curCol++);
    ewl->addWidget(addr1,               curRow, curCol++);
    ewl->addWidget(balanceLabel,        curRow, curCol++);
    ewl->addWidget(balance,             curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(addr2Label,          curRow, curCol++);
    ewl->addWidget(addr2,               curRow, curCol++);
    ewl->addWidget(chargeAmountLabel,   curRow, curCol++);
    ewl->addWidget(chargeAmount,        curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(cityLabel,           curRow, curCol++);
    ewl->addWidget(city,                curRow, curCol++);
    ewl->addWidget(cardTypeLabel,       curRow, curCol++);
    ewl->addWidget(cardType,            curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(stateLabel,          curRow, curCol++);
    ewl->addWidget(state,               curRow, curCol++);
    ewl->addWidget(cardNumberLabel,     curRow, curCol++);
    ewl->addWidget(cardNumber,          curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(zipLabel,            curRow, curCol++);
    ewl->addWidget(zip,                 curRow, curCol++);
    ewl->addWidget(expDateLabel,        curRow, curCol++);
    ewl->addWidget(expDate,             curRow, curCol++);
    
    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(phoneLabel,          curRow, curCol++);
    ewl->addWidget(phone,               curRow, curCol++);
    ewl->addWidget(cvInfoLabel,         curRow, curCol++);
    ewl->addWidget(cvInfo,              curRow, curCol++);

    curRow++; curCol = 3;
    ewl->addWidget(autoPay,             curRow, curCol++);

    curRow++; curCol = 2;
    ewl->setRowStretch(curRow, 0);
    ewl->addMultiCellWidget(chargeDateGroup, curRow, curRow, curCol, ++curCol);

    
    ml->addLayout(ewl,0);
    ml->addStretch(1);

    // Do a layout for the buttons
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl);
    
	setCaption( "Credit Card Entry" );

	ADB			    DB;
	CustomersDB		CDB;
	AddressesDB		addrDB;
	PhoneNumbersDB	PDB;
	QDate			theDate;
	char			tmpstr[1024];

	
	myCustID = CustID;
	theDate	= QDate::currentDate();
	
	CDB.get(myCustID);
	strcpy(tmpstr, CDB.getStr("BillingAddress"));
	addrDB.get(REF_CUSTOMER, myCustID, tmpstr);
	
	// There is no default phone number, so get the first one from the DB.
	DB.query("select PhoneNumber from PhoneNumbers where RefFrom = %d and RefID = %ld", REF_CUSTOMER, myCustID);
	if (DB.rowCount) {
		DB.getrow();
		phone->setText(DB.curRow["PhoneNumber"]);
	}
	
	custName->setText(CDB.getStr("FullName"));
	QToolTip::add(custName, "The full name exactly as it\nappears on the card.");
	
	addr1->setText(addrDB.Address1);
	QToolTip::add(addr1, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	addr2->setText(addrDB.Address2);
	QToolTip::add(addr2, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	city->setText(addrDB.City);
	QToolTip::add(city, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	state->setText(addrDB.State);
	QToolTip::add(state, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	zip->setText(addrDB.ZIP);
	QToolTip::add(zip, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	dateLabel->setText(theDate.toString());
	
	QToolTip::add(expDate, "The Expiration date on the card in the form MMYY.");
	QToolTip::add(cvInfo,  "The security code for the card (optional but desired)\nThis is a three digit number following the card number on Visa or\nMastercard located above the signature on the back of the card.\nOn American Express cards, it is a four digit number located\non the front of the card above the card number.");

    QToolTip::add(chargeAmount, "Leave this blank or set to 0.00 if you are\nonly processing an automatic payment method.");

	balance->setText(CDB.getStr("CurrentBalance"));
	
    custName->setFocus();

    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
    // connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
}


CCEntry::~CCEntry()
{
}


/*
** saveCCard   - The slot that gets called when the user clicks the save
**               button.  Should do the validation of the card, etc.
**
*/

void CCEntry::saveCCard()
{
    CCTransDB   CCDB;
    QDate       theDate;
    char        today[64];
    char        tmpstr[1024];
    char        cvStr[1024];
    float       tmpAmt;
    int         transIns = 1;
    int         autoIns  = 0;
    
    theDate = QDate::currentDate();
    sprintf(today, "%04d-%02d-%02d", theDate.year(), theDate.month(), theDate.day());
    
    sprintf(tmpstr, "%s %s", (const char *)addr1->text(), (const char *)addr2->text());
    
    // First things first, lets copy the data into a CCTransDB entry,
    // and validate it.
    CCDB.setValue("CustomerID",   myCustID);
    CCDB.setValue("EnteredBy",    curUser().userName);
    CCDB.setValue("TransDate",    today);
    CCDB.setValue("Name",         (const char *) custName->text());
    CCDB.setValue("Address",      tmpstr);
    CCDB.setValue("ZIP",          (const char *) zip->text());
    CCDB.setValue("Amount",       (const char *) chargeAmount->text());
    CCDB.setValue("CardType",     cardType->currentItem());
    CCDB.setValue("ExpDate",      (const char *) expDate->text());
    CCDB.setValue("CardNo",       (const char *) cardNumber->text());
    CCDB.setValue("SecurityCode", (const char *) cvInfo->text());
    tmpAmt = CCDB.getFloat("Amount");
    
    if (CCDB.isValid() == CCARD_OK) {
        // Seems okay.  Check the dollar amount now.  If the amount is $0.00
        // then make sure that it is checked for automatic payment method.
        // If it is less then $1.00, then don't accept the charge.
        if (tmpAmt == 0.00) {
            if (autoPay->isChecked()) {
                QMessageBox::information(this, "Auto Payment Only", "No charge will be processed right now,\nhowever, this transaction will be saved\nas an automatic payment method.", "&Ok");
                autoIns = 1;
                transIns = 0;
            } else {
                transIns = 0;
                QMessageBox::critical(this, "Invalid Amount", "Invalid dollar amount specified.\nCredit card charges must be at least $1.00 if not\nestablishing an automatic payment method.", "&Ok");
            }
        } else {
            if (tmpAmt < 1.00) {
                if (autoPay->isChecked()) {
                    QMessageBox::information(this, "Auto Payment Only", "No charge will be processed right now,\nhowever, this transaction will be saved\nas an automatic payment method.\nThe amount has been adjusted to be $0.00.", "&Ok");
                    tmpAmt = 0.00;
                    CCDB.setValue("Amount", "0.00");
                    autoIns = 1;
                    transIns = 0;
                } else {
                    QMessageBox::critical(this, "Invalid Amount", "Invalid dollar amount specified.\nCredit card charges must be at least $1.00 if not\nestablishing an automatic payment method.", "&Ok");
                }
                transIns = 0;
            } else {
                transIns    = 1;
                autoIns     = autoPay->isChecked();
            }
        }

        // Check the security code in the cvInfo
        strcpy(cvStr, cvInfo->text());
        // Its optional, so only run the checks if one has been entered.
        if (strlen(cvStr)) {
            unsigned int ccvLen = 3;
            if (cardType->currentItem() == 2) ccvLen = 4;
            if (strlen(cvStr) == ccvLen) {
                if (atoi(cvStr) == 0) {
                    QMessageBox::critical(this, "Invalid Security Code", "Invalid security code specified.\nFor Visa and Mastercard the code is three\ndigits long.  For American Express, they are 4 digits long.", "&Ok");
                    transIns = 0;
                    autoIns  = 0;
                }
            } else {
                QMessageBox::critical(this, "Invalid Security Code", "Invalid security code specified.\nFor Visa and Mastercard the code is three\ndigits long.  For American Express, they are 4 digits long.", "&Ok");
                transIns = 0;
                autoIns  = 0;
            }
        }
        // All done with the security code.


        
        if (transIns) {
            CCDB.ins();
            
            /*
            // Now, we want to insert it into the customers register and
            // update their balance.
            AcctsRecv   AR;
            char        memoStr[512];
            
            sprintf(memoStr, "Payment Received (%s)", (const char *)cardType->text(cardType->currentItem()));
            
            AR.ARDB->setValue("TransDate", today);
            AR.ARDB->setValue("CustomerID", myCustID);
            AR.ARDB->setValue("TransType", TRANSTYPE_PAYMENT);
            AR.ARDB->setValue("Price", (float)(tmpAmt * -1.0));
            AR.ARDB->setValue("Amount", (float)(tmpAmt * -1.0));
            AR.ARDB->setValue("Memo", memoStr);
            AR.SaveTrans();
            */
        }
        
        // If the user has selected automatic payment, save that as well.
        if (autoIns) {
            // Before we go any further, update the AutoPayments to make sure
            // that this is the only active one we have.
            ADB     DB;
            DB.dbcmd("update AutoPayments set Active = 0 where CustomerID = %ld", myCustID);
            
            AutoPaymentsDB      APDB;
            
            APDB.setValue("CustomerID",     myCustID);
            APDB.setValue("AuthorizedOn",   today);
            APDB.setValue("AuthorizedBy",   custName->text());
            APDB.setValue("EnteredBy",      curUser().userName);
            APDB.setValue("Active",         1);
            APDB.setValue("TransDate",      today);

            // Default to charging 5 days after the statement date,
            // which would make it the 7th of the month.
            APDB.setValue("ChargeDay",      5);
            if (chargeDate2->isChecked()) {
                // They selected the alternate day, which is 15 days
                // after statements are processed, which would make it
                // the 17th of the month.
                APDB.setValue("ChargeDay",  15);
            }
            APDB.setValue("Name",           CCDB.getStr("Name"));
            APDB.setValue("Address",        CCDB.getStr("Address"));
            APDB.setValue("ZIP",            CCDB.getStr("ZIP"));
            APDB.setValue("AuthName",       custName->text());
            APDB.setValue("PaymentType",    1);
            APDB.setValue("CardType",       CCDB.getInt("CardType"));
            APDB.setValue("SecurityCode",   (const char *)cvInfo->text());
            
            // Fix the date
            int month, year;
            QString tmpQStr;
            QDate   tmpDate1;
            char    tmpDate[1024];
            tmpQStr = CCDB.getStr("ExpDate");
            
            // Extract the month and year.
            month = atoi(tmpQStr.left(2));
            year  = atoi(tmpQStr.right(2));
            // Check for 2000.
            if (year < 95) {
                year += 2000;
            } else {
                year += 1900;
            }
            tmpDate1.setYMD(year, month, 1);
            tmpDate1.setYMD(year, month, tmpDate1.daysInMonth());
            QDatetomyDate(tmpDate, tmpDate1);
            
            APDB.setValue("ExpDate",        tmpDate);
            APDB.setValue("AcctNo",         CCDB.getStr("CardNo"));

            APDB.ins();
        }
    } else {
        QMessageBox::critical(this, "Invalid Entry", "Something wasn't right with the credit card entry.\nPlease validate the entry and try again.", "&Ok");
    }
    
    if (transIns || autoIns) {
        emit(customerChanged(myCustID));
        close();
    }
}

/*
** cancelCCard   - The slot that gets called when the user clicks the cancel
**                 button.
**
*/

void CCEntry::cancelCCard()
{
    close();
}


