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

#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QDateTime>
#include <QtGui/QToolTip>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <BlargDB.h>
#include <BString.h>
#include <AcctsRecv.h>
#include <CCTools.h>
#include <ConfirmBox.h>
#include <TAATools.h>
#include <ADB.h>

#include "CCEntry.h"

using namespace Qt;

CCEntry::CCEntry
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget(parent)
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
    
    QLabel *cardListLabel = new QLabel(this, "cardListLabel");
    cardListLabel->setAlignment(AlignRight|AlignVCenter);
    cardListLabel->setText("Card to charge:");

    // Our list of cards.
    cards = getCreditCardList(CustID);
    cardList = new QComboBox(false, this, "cardList");
    cardList->insertItem("Charge New Card");
    CreditCardRecord    *tmpCard;
    for (tmpCard = cards.first(); tmpCard; tmpCard = cards.next()) {
        cardList->insertItem(tmpCard->listText, tmpCard->idx+1);
    }
    connect(cardList, SIGNAL(activated(int)), this, SLOT(cardListChanged(int)));
    
    QLabel  *chargeAmountLabel  = new QLabel(this);
    chargeAmountLabel->setAlignment(AlignRight|AlignVCenter);
    chargeAmountLabel->setText("Charge Amount:");

    chargeAmount = new QLineEdit(this);
    chargeAmount->setMaxLength(10);
    
    cardTypeLabel  = new QLabel(this);
    cardTypeLabel->setAlignment(AlignRight|AlignVCenter);
    cardTypeLabel->setText("Card Type:");

    cardType = new QComboBox(false, this);
    cardType->insertItem("Mastercard");
    cardType->insertItem("Visa");
    cardType->insertItem("American Express");

    cardNumberLabel  = new QLabel(this);
    cardNumberLabel->setAlignment(AlignRight|AlignVCenter);
    cardNumberLabel->setText("Card Number:");

    cardNumber = new QLineEdit(this);
    cardNumber->setMaxLength(18);

    expDateLabel  = new QLabel(this);
    expDateLabel->setAlignment(AlignRight|AlignVCenter);
    expDateLabel->setText("Expiration Date:");

    expDate = new QLineEdit(this);
    expDate->setMaxLength(18);

    cvInfoLabel  = new QLabel(this);
    cvInfoLabel->setAlignment(AlignRight|AlignVCenter);
    cvInfoLabel->setText("CV Security Code:");

    cvInfo = new QLineEdit(this);
    cvInfo->setMaxLength(4);

    autoPay = new QCheckBox(this, "AutoPay");
    autoPay->setText("Use as automatic payment method");
    connect(autoPay, SIGNAL(toggled(bool)), this, SLOT(autoPayChanged(bool)));

    chargeDayLabel = new QLabel(this, "chargeDayLabel");
    chargeDayLabel->setAlignment(AlignRight|AlignVCenter);
    chargeDayLabel->setText("Charge Day:");

    chargeDay = new QSpinBox(0, 28, 1, this, "chargeDay");
    chargeDay->setEnabled(false);


    QLabel *balanceLabel = new QLabel(this);
    balanceLabel->setAlignment(AlignRight);
    balanceLabel->setText("Current Balance:");

    balance = new QLabel(this);
    balance->setAlignment(AlignRight);

    saveButton = new QPushButton(this);
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCCard()));
    
    cancelButton = new QPushButton(this);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelCCard()));


    // Setup the layout for this window.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // The entry widgets go into a grid 4 columns wide.
    Q3GridLayout *ewl = new Q3GridLayout(4, 5, 2);
    ewl->setColStretch(0, 0);
    ewl->setColStretch(1, 1);
    ewl->setColStretch(2, 0);
    ewl->setColStretch(3, 1);
    
    int curRow = -1;
    int curCol = 0;

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(custNameLabel,       curRow, curCol++);
    ewl->addWidget(custName,            curRow, curCol++);
    ewl->addWidget(balanceLabel,        curRow, curCol++);
    ewl->addWidget(balance,             curRow, curCol++);

    curRow++; curCol = 0;
    ewl->setRowStretch(curRow, 0);
    ewl->addWidget(addr1Label,          curRow, curCol++);
    ewl->addWidget(addr1,               curRow, curCol++);
    ewl->addWidget(cardListLabel,       curRow, curCol++);
    ewl->addWidget(cardList,            curRow, curCol++);

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
    ewl->addWidget(chargeDayLabel,      curRow, curCol++);
    ewl->addWidget(chargeDay,           curRow, curCol++);

    
    ml->addLayout(ewl,0);
    ml->addStretch(1);

    // Do a layout for the buttons
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl);
    
	setCaption( "Credit Card Entry" );

	myCustID = CustID;
	CustomersDB		CDB;
    CDB.get(myCustID);
	
    fillContactInfo();
	
	
	QToolTip::add(custName, "The full name exactly as it\nappears on the card.");
	
	QToolTip::add(addr1, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	QToolTip::add(addr2, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	QToolTip::add(city, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	QToolTip::add(state, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	QToolTip::add(zip, "The address _MUST_ be the address\nwhere the credit card statements\nare sent to.");
	
	QToolTip::add(expDate, "The Expiration date on the card in the form MMYY.");
	QToolTip::add(cvInfo,  "The security code for the card (optional but desired)\nThis is a three digit number following the card number on Visa or\nMastercard located above the signature on the back of the card.\nOn American Express cards, it is a four digit number located\non the front of the card above the card number.");

    QToolTip::add(chargeAmount, "Leave this blank or set to 0.00 if you are\nonly processing an automatic payment method.");
	QToolTip::add(chargeDay, "The number of days from the time\na statement is processed until\nthe card is charged.");

	balance->setText(CDB.getStr("CurrentBalance"));
	
    custName->setFocus();

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
    if (cardList->currentItem()) {
        // Using an existing card.
        CreditCardRecord    *card;
        for (card = cards.first(); card; card = cards.next()) {
            if (card->idx + 1 == cardList->currentItem()) {
                CCDB.setValue("CardType",       card->cardType);
                CCDB.setValue("ExpDate",        card->expDate.ascii());
                CCDB.setValue("CardNo",         card->cardNo.ascii());
                CCDB.setValue("SecurityCode",   card->ccv.ascii());
            }
        }
    } else {
        // Using a new card
        CCDB.setValue("CardType",     cardType->currentItem());
        CCDB.setValue("ExpDate",      (const char *) expDate->text());
        CCDB.setValue("CardNo",       (const char *) cardNumber->text());
        CCDB.setValue("SecurityCode", (const char *) cvInfo->text());
    }
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

        // One last check.  Confirm the amounts.
        ConfirmBox  *cb = new ConfirmBox(this);
        cb->setTitle("Confirm Charge");
        QString tmpText = "Confirm that you wish to charge $";
        tmpText += chargeAmount->text();
        tmpText += "\nfor ";
        tmpText += custName->text();
        cb->setText(tmpText);
        cb->setConfirmText("Check this box to confirm the transaction");
        if (cb->exec() == QDialog::Rejected) {
            return;
        }

        
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
            APDB.setValue("ChargeDay",      chargeDay->value());
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
        char errStr[1024];
        sprintf(errStr, "Something wasn't right with the card.\nPlease validate the entry and try again.\nError code %d", CCDB.isValid());
        //QMessageBox::critical(this, "Invalid Entry", "Something wasn't right with the credit card entry.\nPlease validate the entry and try again.", "&Ok");
        QMessageBox::critical(this, "Invalid Entry", errStr);
        return;
    }
    
    if (transIns || autoIns) {
        emit(customerUpdated(myCustID));
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

/**
 * autoPayChanged()
 *
 * Gets called when the user changes the use automatic payment
 * checkbox.  Enables/disables our charge day field.
 */
void CCEntry::autoPayChanged(bool check)
{

    chargeDay->setEnabled(check);

}

/**
 * fillContactInfo()
 *
 * Grabs the default contact info for this customer and shoves
 * it into the edit fields.
 */
void CCEntry::fillContactInfo()
{
    ADB             DB;
    CustomersDB     CDB;
	AddressesDB		addrDB;
    QString         tmpStr;

	// There is no default phone number, so get the first one from the DB.
	DB.query("select PhoneNumber from CustomerContacts where Active > 0 and CustomerID = %ld", myCustID);
	if (DB.rowCount) {
		DB.getrow();
		phone->setText(DB.curRow["PhoneNumber"]);
	}

	CDB.get(myCustID);
	custName->setText(CDB.getStr("FullName"));

    tmpStr = CDB.getStr("BillingAddress");

	addrDB.get(REF_CUSTOMER, myCustID, tmpStr.ascii());

	addr1->setText(addrDB.Address1);
	addr2->setText(addrDB.Address2);
	city->setText(addrDB.City);
	state->setText(addrDB.State);
	zip->setText(addrDB.ZIP);
}


/**
 * cardListChanged()
 *
 * Gets called when the user selects an item in the card list.
 * We'll either show all of the other input fields or hide them
 * depending on whether or not they have selected to charge a new
 * card or an existing card.
 */
void CCEntry::cardListChanged(int idx)
{
    bool    useExisting = true;
    if (!idx) useExisting = false;

    cardTypeLabel->setHidden(useExisting);
    cardType->setHidden(useExisting);
    cardNumberLabel->setHidden(useExisting);
    cardNumber->setHidden(useExisting);
    expDateLabel->setHidden(useExisting);
    expDate->setHidden(useExisting);
    cvInfoLabel->setHidden(useExisting);
    cvInfo->setHidden(useExisting);
    autoPay->setHidden(useExisting);
    chargeDayLabel->setHidden(useExisting);
    chargeDay->setHidden(useExisting);
    
    custName->setEnabled(!useExisting);
    addr1->setEnabled(!useExisting);
    addr2->setEnabled(!useExisting);
    city->setEnabled(!useExisting);
    state->setEnabled(!useExisting);
    zip->setEnabled(!useExisting);
    phone->setEnabled(!useExisting);

    if (useExisting) {
        // Set the address information from the card data.
        CreditCardRecord *tmpCard;
        for (tmpCard = cards.first(); tmpCard; tmpCard = cards.next()) {
            if (tmpCard->idx == idx-1) {
                custName->setText(tmpCard->cardholder);
                addr1->setText(tmpCard->addr);
                addr2->setText("");
                city->setText("");
                state->setText("");
                zip->setText(tmpCard->zip);
                phone->setText("");
            }
        }
        // Make sure auto pay is NOT checked.
        autoPay->setChecked(false);
    } else {
        // Grab the data from the customers database again.
        fillContactInfo();
    }
}

// vim: expandtab
