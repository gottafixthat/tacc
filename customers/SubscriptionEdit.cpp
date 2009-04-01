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
#include <time.h>
#include <sys/timeb.h>

#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/q3strlist.h>

#include <ADB.h>
#include <BlargDB.h>
#include "SubscriptionEdit.h"

using namespace Qt;

SubscriptionEdit::SubscriptionEdit
(
	QWidget* parent,
	const char* name,
	long CustID,
	long SubscrID
) : TAAWidget( parent, name )
{
    // Create all of our widgets.  This one has quite a few widgets,
    // it will be a box layout from top to bottom, and then a grid for
    // the information.  A standard box layout accross the bottom for
    // the save and cancel buttons.


    // First row, Customer Name and Primary Login ID
    QLabel  *customerNameLabel = new QLabel(this);
    customerNameLabel->setText("Customer:");
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);

    customerName     = new QLabel(this);
    customerName->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *primaryLoginLabel = new QLabel(this);
    primaryLoginLabel->setText("Primary Login:");
    primaryLoginLabel->setAlignment(AlignRight|AlignVCenter);

    primaryLogin     = new QLabel(this);
    primaryLogin->setAlignment(AlignLeft|AlignVCenter);

    
    // Next row, Rate Plan and Billing Cycle
    QLabel  *ratePlanLabel = new QLabel(this);
    ratePlanLabel->setText("Rate Plan:");
    ratePlanLabel->setAlignment(AlignRight|AlignVCenter);

    ratePlan     = new QLabel(this);
    ratePlan->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *billingCycleLabel = new QLabel(this);
    billingCycleLabel->setText("Billing Cycle:");
    billingCycleLabel->setAlignment(AlignRight|AlignVCenter);

    billingCycle     = new QLabel(this);
    billingCycle->setAlignment(AlignLeft|AlignVCenter);

    // Next row, Item ID, and Login ID
    QLabel  *itemIDLabel = new QLabel(this);
    itemIDLabel->setText("Item ID:");
    itemIDLabel->setAlignment(AlignRight|AlignVCenter);

    itemID = new QComboBox(false, this, "Item ID");
    connect(itemID, SIGNAL(activated(int)), this, SLOT(itemIDChanged(int)));
    connect(itemID, SIGNAL(highlighted(int)), this, SLOT(itemIDChanged(int)));

    QLabel  *loginIDLabel = new QLabel(this);
    loginIDLabel->setText("Login ID:");
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);

    loginID = new QComboBox(false, this, "loginID");
    connect(loginID, SIGNAL(highlighted(const QString &)), this, SLOT(loginIDChanged(const QString &)));

    // Next row, Quantity and Price.
    QLabel  *quantityLabel = new QLabel(this);
    quantityLabel->setText("Quantity:");
    quantityLabel->setAlignment(AlignRight|AlignVCenter);

    quantity = new QLineEdit(this, "Quantity");
    quantity->setMaxLength(10);

    priceLabel = new QLabel(this);
    priceLabel->setText("Price:");
    priceLabel->setAlignment(AlignRight|AlignVCenter);

    price = new QLineEdit(this, "Price");
    price->setMaxLength(10);

    
    // Next row, Last Date and Ends On.
    QLabel  *lastDateLabel = new QLabel(this);
    lastDateLabel->setText("Last Date:");
    lastDateLabel->setAlignment(AlignRight|AlignVCenter);

    lastDate = new DateInput(this, myLastDate);
    lastDate->setMinimumHeight(30);

    QLabel  *endDateLabel = new QLabel(this);
    endDateLabel->setText("Last Date:");
    endDateLabel->setAlignment(AlignRight|AlignVCenter);

    endDate = new DateInput(this, myEndDate);
    endDate->setMinimumHeight(30);

    // Next row, Auto Renew checkbox, Active Checkbox
    autoRenew = new QCheckBox(this, "Auto Renew");
    autoRenew->setText("Auto Renew");
    autoRenew->setChecked(false);

    active = new QCheckBox(this, "Active");
    active->setText("Active");
    active->setChecked(false);

    // Next row, Follow Item, Setup Charged
    followItem = new QCheckBox(this, "followItem");
    followItem->setText("Follow Item");
    followItem->setChecked(false);

    setupCharged = new QCheckBox(this, "setupCharged");
    setupCharged->setText("Setup Charged");
    setupCharged->setChecked(false);

    // Next, and final row, the description.
    descriptionLabel = new QLabel(this);
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(AlignRight|AlignTop);

    description = new Q3MultiLineEdit(this, "Description");
    description->setWordWrap(Q3MultiLineEdit::WidgetWidth);

    // Finally, our action buttons.
    QPushButton *saveButton = new QPushButton(this, "Save");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSubscription()));

    QPushButton *cancelButton = new QPushButton(this, "Cancel");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelSubscription()));

    // Now, do our layout.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    
    Q3GridLayout *gl = new Q3GridLayout(8, 4);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);
    for (int i = 0; i < 6; i++) gl->setRowStretch(i, 0);
    gl->setRowStretch(7, 1);
    int curRow = 0;
    gl->addWidget(customerNameLabel,        curRow, 0);
    gl->addWidget(customerName,             curRow, 1);
    gl->addWidget(primaryLoginLabel,        curRow, 2);
    gl->addWidget(primaryLogin,             curRow, 3);
    curRow++;

    gl->addWidget(ratePlanLabel,            curRow, 0);
    gl->addWidget(ratePlan,                 curRow, 1);
    gl->addWidget(billingCycleLabel,        curRow, 2);
    gl->addWidget(billingCycle,             curRow, 3);
    curRow++;
    
    gl->addWidget(itemIDLabel,              curRow, 0);
    gl->addWidget(itemID,                   curRow, 1);
    gl->addWidget(loginIDLabel,             curRow, 2);
    gl->addWidget(loginID,                  curRow, 3);
    curRow++;
    
    gl->addWidget(quantityLabel,            curRow, 0);
    gl->addWidget(quantity,                 curRow, 1);
    gl->addWidget(priceLabel,               curRow, 2);
    gl->addWidget(price,                    curRow, 3);
    curRow++;

    gl->addWidget(lastDateLabel,            curRow, 0);
    gl->addWidget(lastDate,                 curRow, 1);
    gl->addWidget(endDateLabel,             curRow, 2);
    gl->addWidget(endDate,                  curRow, 3);
    curRow++;

    gl->addWidget(autoRenew,                curRow, 1);
    gl->addWidget(active,                   curRow, 3);
    curRow++;
    
    gl->addWidget(followItem,               curRow, 1);
    gl->addWidget(setupCharged,             curRow, 3);
    curRow++;

    gl->addWidget(descriptionLabel,         curRow, 0);
    gl->addMultiCellWidget(description,     curRow, curRow, 1, 3);

    ml->addLayout(gl);

    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    abl->addStretch(1);
    abl->addWidget(saveButton, 0);
    abl->addWidget(cancelButton, 0);

    ml->addLayout(abl);

    // Done with widget creation and layout...



    
    ADB 		    DB;
    BillablesDataDB	BDB;
    LoginsDB        LDB;
    char            tmpstr[256];
    long            tmpItemNo = 0;
    char            tmploginid[64];
    QString         qtmpstr = "";
    SubscriptionsDB subscrDB;

    if (CustID == 0) {
        close();
        return;
    }

    myCustID   = CustID;
    mySubscrID = SubscrID;    
    isParent   = 0;
    
    // Load the customer record from the database.
    CustomersDB custRec;
    custRec.get(CustID);
    
    // If we're editing, load our record from the database
    if (SubscrID != 0) {
        subscrDB.get(mySubscrID);
        tmpItemNo = subscrDB.getLong("ItemNumber");
        strcpy(tmploginid, subscrDB.getStr("LoginID"));
        LDB.get(myCustID, (const char *) subscrDB.getStr("LoginID"));
        if (!LDB.getInt("Active")) {
            active->setEnabled(0);
            loginID->setEnabled(0);
        }
	}
    
    customerName->setText(custRec.getStr("FullName"));
    primaryLogin->setText(custRec.getStr("PrimaryLogin"));
/*    mainPhone->setText(custRec.getStr("DayPhone")); */
    billingCycle->setText(custRec.getStr("BillingCycle"));
    
    strcpy(myCycleID, custRec.getStr("BillingCycle"));
    strcpy(myParentID, custRec.getStr("PrimaryLogin"));
    
    autoRenew->setChecked(TRUE);
    active->setChecked(TRUE);
    setupCharged->setChecked(FALSE);
    followItem->setChecked(TRUE);
    updateFollowItem(TRUE);
    
    quantity->setText("1");
    
    connect(followItem, SIGNAL(toggled(bool)), SLOT(updateFollowItem(bool)));
    
    itemIndex  = NULL;
    myItemPrice = NULL;
    myItemChildPrice = NULL;
    loginIndex = new Q3StrList(TRUE);
    myItemDescr = new Q3StrList(TRUE);
    myItemID    = new Q3StrList(TRUE);
    
    // Fill in our combo box with the Subscription Item types.
    strcpy(tmpstr, custRec.getStr("BillingCycle"));
    DB.query("select ItemNumber, ItemID, Description from Billables where ItemType = 1 order by ItemID");
    itemIndex        = new(long[DB.rowCount + 1]);
    myItemPrice      = new(float[DB.rowCount + 1]);
    myItemChildPrice = new(float[DB.rowCount + 1]);
    int idxPtr = 0;
    while (DB.getrow()) {
    	BDB.getItem(atol(DB.curRow["ItemNumber"]), custRec.getLong("RatePlan"), custRec.getLong("BillingCycle"));
        itemIndex[idxPtr] = atol(DB.curRow["ItemNumber"]);
        itemID->insertItem(DB.curRow["ItemID"]);
        myItemID->append(DB.curRow["ItemID"]);
        myItemDescr->append(DB.curRow["Description"]);
        myItemPrice[idxPtr] = BDB.getFloat("Price");
        myItemChildPrice[idxPtr] = BDB.getFloat("SecondaryPrice");
		if (mySubscrID) {
			if (tmpItemNo == itemIndex[idxPtr]) {
		        itemID->setCurrentItem(idxPtr);
		    }
		}
        idxPtr++;
    }
    // Now, fill in the Description and price.
	itemIDChanged(itemID->currentItem());
	
    
    // Fill in our combo box with the Logins for this customer.
    DB.query("select LoginID from Logins where CustomerID = %ld and Active = 1 order by LoginID", myCustID);
    idxPtr = 0;
    strcpy(tmpstr, custRec.getStr("PrimaryLogin"));
    while (DB.getrow()) {
        loginIndex->append(DB.curRow["LoginID"]);
        loginID->insertItem(DB.curRow["LoginID"]);
        if (mySubscrID) {
            if (!strcmp (tmploginid, DB.curRow["LoginID"])) {
                loginID->setCurrentItem(idxPtr);
                if (!strcmp(tmpstr, DB.curRow["LoginID"])) {
                    isParent = 1;
                }
            }
        } else {
            if (!strcmp (DB.curRow["LoginID"], tmpstr)) {
                loginID->setCurrentItem(idxPtr);
                isParent = 1;
            }
        }
        idxPtr++;
    }

    
    if (SubscrID != 0) {
		setCaption( "Edit Subscription" );
		active->setChecked(subscrDB.getInt("Active"));
		quantity->setText(subscrDB.getStr("Quantity"));
		description->setText(subscrDB.getStr("ItemDesc"));
		price->setText(subscrDB.getStr("Price"));
        lastDate->setDate(subscrDB.getDate("LastDate"));
        endDate->setDate(subscrDB.getDate("EndsOn"));
		autoRenew->setChecked(subscrDB.getInt("AutoRenew"));
		followItem->setChecked(subscrDB.getInt("AutoPrice"));
		setupCharged->setChecked(subscrDB.getInt("SetupCharged"));
		updateFollowItem(subscrDB.getInt("AutoPrice"));
		
		// Okay, testing here.  Lets set the Main Phone to a linked
		// record if we have one.
		if (subscrDB.getInt("LinkedTo")) {
		    sprintf(tmpstr, "%d - %ld - %s", subscrDB.getInt("LinkedTo"), subscrDB.getLong("LinkID"), (const char *) subscrDB.getStr("LinkStr"));
		    customerName->setText(tmpstr);
		}
		
	} else {
        QDate   tmpDate = QDate::currentDate();
        lastDate->setDate(tmpDate);
        endDate->setDate(tmpDate);
		setCaption( "New Subscription" );
	}
	
	
	
	
	show();
}


SubscriptionEdit::~SubscriptionEdit()
{
    if (itemIndex != NULL) {
        delete(itemIndex);
    }
}

// The user clicked on the close button

void SubscriptionEdit::cancelSubscription()
{
    close();
}

// The user clicked on the save button

void SubscriptionEdit::saveSubscription()
{
    SubscriptionsDB subscrDB;
    
    subscrDB.setValue("CustomerID", myCustID);
    subscrDB.setValue("LoginID", loginIndex->at(loginID->currentItem()));
    subscrDB.setValue("Active", (int) active->isChecked());
    subscrDB.setValue("ItemNumber", itemIndex[itemID->currentItem()]); // myItemID->at(itemID->currentItem());
    subscrDB.setValue("Quantity", atof(quantity->text()));
    subscrDB.setValue("ItemDesc", description->text());
    subscrDB.setValue("Price", atof(price->text()));
    subscrDB.setValue("LastDate", lastDate->getQDate());
    subscrDB.setValue("EndsOn", endDate->getQDate());
    subscrDB.setValue("AutoRenew", (int) autoRenew->isChecked());
    subscrDB.setValue("AutoPrice", (int) followItem->isChecked());
    subscrDB.setValue("SetupCharged", (int) setupCharged->isChecked());
    
    if (mySubscrID) {
    	// Update an edited one.
    	subscrDB.setValue("InternalID", mySubscrID);
    	subscrDB.upd();
    } else {
    	// Insert a new one
    	subscrDB.ins();
    }
    
    emit customerUpdated(myCustID);
    close();
}

// When an Item ID is selected from the list, this slot is called.
// It will update the Description and price from the database for the 
// item.

void SubscriptionEdit::itemIDChanged(int newItemID)
{
    char    tmpstr[128];
    QString qtmpstr = "";
    
    if (isParent) {
        qtmpstr.sprintf("%-12.2f", myItemPrice[newItemID]);
    } else {
        qtmpstr.sprintf("%-12.2f", myItemChildPrice[newItemID]);
    }
    qtmpstr.stripWhiteSpace();
    strcpy(tmpstr, qtmpstr);
    price->setText(tmpstr);
    description->setText(myItemDescr->at(newItemID));
}

// When a Login ID is selected from the list, this slot is called.
// It will update the Description and price from the database for the 
// item, based on whether or not the login ID is the parent.

void SubscriptionEdit::loginIDChanged(const QString &newLoginID)
{
    if (!strcmp(newLoginID, myParentID)) {
        isParent = 1;
    } else {
        isParent = 0;
    }
    
    itemIDChanged(itemID->currentItem());
}

void SubscriptionEdit::updateFollowItem(bool checked)
{
    descriptionLabel->setEnabled(!checked);
    description->setEnabled(!checked);
    priceLabel->setEnabled(!checked);
    price->setEnabled(!checked);
}

// vim: expandtab
