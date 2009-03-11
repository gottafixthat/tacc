/**
 * CustSubscrList.h - A widget that allows the user to view and edit
 * subscriptions.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include "CustSubscrList.h"
#include "BlargDB.h"
#include "BString.h"
#include "SubscriptionEdit.h"
#include "SelectionList.h"
#include <qapp.h>
#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qlayout.h>
#include <ADB.h>
#include <TAATools.h>
#include <qmessagebox.h>

CustSubscrList::CustSubscrList
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
	myCustID = CustID;
	setCaption( "Subscriptions" );
	setFocusPolicy(StrongFocus);

    // Create our widgets and our layout.
    list = new QListView(this);
    list->addColumn("LoginID");
    list->addColumn("Item ID");
    list->addColumn("Qty");         list->setColumnAlignment(2, AlignRight);
    list->addColumn("Price");       list->setColumnAlignment(3, AlignRight);
    list->addColumn("Last Date");
    list->addColumn("Ends On");
    list->addColumn("Act");         list->setColumnAlignment(6, AlignCenter);
    list->addColumn("Description");
    subscriptionIDCol = 8;
    list->setAllColumnsShowFocus(true);
    list->setMultiSelection(false);
    list->setRootIsDecorated(true);
    list->setItemMargin( 2 );

    // Create the action buttons.
    QPushButton *addPackageButton = new QPushButton(this);
    addPackageButton->setText("&Add Package");
    connect(addPackageButton, SIGNAL(clicked()), this, SLOT(addPackageItem()));

    newButton = new QPushButton(this);
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newSubscription()));

    editButton = new QPushButton(this);
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editSubscription()));

    deleteButton = new QPushButton(this);
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSubscription()));

    hideInactiveButton = new QCheckBox(this);
    hideInactiveButton->setText("Hide &Inactive");
    connect(hideInactiveButton, SIGNAL(clicked()), this, SLOT(hideInactiveChanged()));

    // Now, create the layout.  Standard box layout, top to bottom, with
    // a left to right layout for our buttons.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    ml->addWidget(list, 1);

    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    abl->addWidget(addPackageButton, 0);
    abl->addWidget(newButton, 0);
    abl->addWidget(editButton, 0);
    abl->addWidget(deleteButton, 0);
    abl->addStretch(1);
    abl->addWidget(hideInactiveButton, 0);

    ml->addLayout(abl, 0);

    // All done.

	
	hideInactiveButton->setChecked(1);
	
	if (!isManager()) {
		newButton->hide();
		editButton->hide();
		deleteButton->hide();
	}
    
    showInactive = 0;
	refreshCustomer(myCustID);
}


CustSubscrList::~CustSubscrList()
{
}


/*
** refreshCustomer - A public slot which will get called when a customer
**					 has changed.  If we are refreshing our own customer
**                   ID, we refresh.
**
*/

void CustSubscrList::refreshCustomer(long custID)
{
    // If this is the same customer, refresh, otherwise return.
    if (custID != myCustID) return;

    QApplication::setOverrideCursor(waitCursor);

    ADB             DB;
    ADB             DB2;
    BillablesDB	    BDB;
    PackagesDB	    PDB;
    SubscriptionsDB SDB;
    CustomersDB     CDB;
    char            tmpActive[32] = "";
    char            tmpPrice[32];
    int             showIt = 0;
    
    QListViewItem   *curItem;

    char            tmpItemID[1024];

    // Clear out our list and get our customer
    list->clear();
    
    CDB.get(myCustID);

    // First, load up any package items.
    DB.query("select InternalID from Subscriptions where CustomerID = %ld and PackageNo > 0", myCustID);
    if (DB.rowCount) while (DB.getrow()) {
        list->setRootIsDecorated(TRUE);
        // Load the subscription item itelf for inserting into the list.
        SDB.get(atol(DB.curRow["InternalID"]));
        
        // Check to see if we are going to show this one.
        showIt = 1;
        if (!SDB.getInt("Active")) showIt = showInactive;

        if (showIt) {
	        PDB.get(SDB.getLong("PackageNo"));
	        // Get the price for this package based on the customers
	        // rate plan and billing cycle.
	        DB2.query("select Price from PackagesData where PackageID = %ld and RatePlanID = %ld and CycleID = %ld",
	          PDB.getLong("InternalID"),
	          CDB.getLong("RatePlan"),
	          CDB.getLong("BillingCycle")
	        );
	        if (DB2.rowCount) {
	            DB2.getrow();
	            sprintf(tmpPrice, "%.2f", atof(DB2.curRow["Price"]));
	        } else {
	            strcpy(tmpPrice, "Undefined!");
	        }
	        
	        if (SDB.getInt("Active")) strcpy(tmpActive, "Y");
	        else strcpy(tmpActive, "N");
	        
	        sprintf(tmpItemID, "%s Package", (const char *) PDB.getStr("PackageTag"));
	        
	        curItem = new QListViewItem(list,
	          SDB.getStr("LoginID"),
	          tmpItemID,
	          SDB.getStr("Quantity"),
	          tmpPrice,
	          SDB.getStr("LastDate"),
	          SDB.getStr("EndsOn"),
	          tmpActive,
              PDB.getStr("Description")
	        );
            curItem->setText(subscriptionIDCol, SDB.getStr("InternalID"));

	        // Now that we have the package in our list, get the contents
	        // of the package.
	        DB2.query("select InternalID from Subscriptions where ParentID = %ld", SDB.getLong("InternalID"));
	        if (DB2.rowCount) while (DB2.getrow()) {
	            SDB.get(atol(DB2.curRow["InternalID"]));
	            // Get the billable for this subscription item.
	            BDB.get(SDB.getLong("ItemNumber"));
	            
	            if (SDB.getInt("Active")) strcpy(tmpActive, "Y");
	            else strcpy(tmpActive, "N");
	            
	            QListViewItem *tmpItem = new QListViewItem(curItem,
	              SDB.getStr("LoginID"),
	              BDB.getStr("ItemID"),
	              SDB.getStr("Quantity"),
	              "0.00",
	              SDB.getStr("LastDate"),
	              SDB.getStr("EndsOn"),
	              tmpActive,
                  BDB.getStr("Description")
	            );
                tmpItem->setText(subscriptionIDCol, SDB.getStr("InternalID"));
	        }
	        
	        // curItem->setOpen(TRUE);
        }        
    }  else {
        // we have no packages, so don't decorate the root window.
        list->setRootIsDecorated(FALSE);

    }

    // Now, get the remaining subscriptions that aren't a package or
    // part of a package.
    DB.query("select InternalID from Subscriptions where CustomerID = %ld and PackageNo = 0 and ParentID = 0", myCustID);
    if (DB.rowCount) while (DB.getrow()) {
        // Load the subscription item itelf for inserting into the list.
        SDB.get(atol(DB.curRow["InternalID"]));
        
        // Check to see if we are going to show this one.
        showIt = 1;
        if (!SDB.getInt("Active")) showIt = showInactive;

        if (showIt) {
	        // Get the billable info
	        BDB.get(SDB.getLong("ItemNumber"));
	        DB2.query("select Price from BillablesData where ItemNumber = %ld and RatePlanID = %ld and CycleID = %ld",
	          BDB.getLong("ItemNumber"),
	          CDB.getLong("RatePlan"),
	          CDB.getLong("BillingCycle")
	        );
	        
	        if (DB2.rowCount) {
	            DB2.getrow();
	            sprintf(tmpPrice, "%.2f", atof(DB2.curRow["Price"]));
	        } else {
	            strcpy(tmpPrice, "Undefined!");
	        }

            // Check for a price override.
            if (!SDB.getInt("AutoPrice")) {
                sprintf(tmpPrice, "*%.2f", SDB.getFloat("Price"));
            }

	        if (SDB.getInt("Active")) strcpy(tmpActive, "Y");
	        else strcpy(tmpActive, "N");
	    
	        QListViewItem *tmpItem = new QListViewItem(list,
	          SDB.getStr("LoginID"),
	          BDB.getStr("ItemID"),
	          SDB.getStr("Quantity"),
	          tmpPrice,
	          SDB.getStr("LastDate"),
	          SDB.getStr("EndsOn"),
	          tmpActive,
	          BDB.getStr("Description")
	        );
	        tmpItem->setText(subscriptionIDCol, SDB.getStr("InternalID"));
        }    
    }
    
    QApplication::restoreOverrideCursor();
}




/*
** newSubscription - A way for the user to manually add subscriptions
**					 to a customer's subscription list.
*/

void CustSubscrList::newSubscription()
{
	SubscriptionEdit	*newSub;
	QApplication::setOverrideCursor(waitCursor);
	newSub = new SubscriptionEdit(0, "", myCustID);
	// connect(newSub, SIGNAL(refresh(int)), SLOT(refreshSubscrList(int)));
	QApplication::restoreOverrideCursor();
	// newSub.show();
}

/**
 ** editSubscription - A slot for editing a new subscription item for a 
 **                    customer.
 **/
 
void CustSubscrList::editSubscription()
{
    QListViewItem   *tmpItem;
    tmpItem         = list->currentItem();

    if (tmpItem != NULL) {
        SubscriptionEdit *editSubscr;
        editSubscr = new SubscriptionEdit(0,"",myCustID, atol(tmpItem->key(subscriptionIDCol, 1)));
	    // connect(editSubscr, SIGNAL(refresh(int)), SLOT(refreshSubscrList(int)));
    }
}



/*
** deleteSubscription  - Allows the user to delete a Subscription Item
*/

void CustSubscrList::deleteSubscription()
{
    QListViewItem	*CurItem;
    char	        tmpstr[256];
    ADB	            DB;
    
    CurItem = list->currentItem();
    if (CurItem != NULL) {
        sprintf(tmpstr, "Are you sure you wish to delete this subscription item?");
        if (QMessageBox::warning(this, "Delete Subscription Item", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            SubscriptionsDB SBDB;
            // For the auto package scan to work we have to get the item first
            SBDB.get(atol(CurItem->key(subscriptionIDCol,1)));
            if (SBDB.del(atol(CurItem->key(subscriptionIDCol,1))) < 1) {
                QMessageBox::critical(0, "Delete Subscription Item", "Unable to delete Subscription Item.  It is in use by another table.");
            } else {
                refreshCustomer(myCustID);
                emit(customerUpdated(myCustID));
            }
        }
    }
}

/*
** addPackageItem   - Allows the user to add a package item to the customers
**                    subscriptions.
*/

void CustSubscrList::addPackageItem()
{
    PackageSelector *sList;
    char            tmpstr[1024];
    long            itemNo = 0;
    ADB             DB;
    QDate           todaysDate;
    QDate           packageDate;
    QListViewItem   *selItem;

    sList = new PackageSelector(this);
    
    todaysDate  = QDate::currentDate();
    
    // Set the title and the header info.
    sList->setTitle("Package Addition");
    sList->setHeader("Select a package to add to the customer's subscriptions.");
    sList->addColumn("Package");
    sList->addColumn("Description");
    
    // Add the various packages to the selection list.
    DB.query("select InternalID, PackageTag, Description, AutoConvert, PromoPackage, PromoEnds from Packages where Active <> 0 order by Description");
    while (DB.getrow()) {
        if (atoi(DB.curRow["AutoConvert"]) && atoi(DB.curRow["PromoPackage"])) {
            myDatetoQDate(DB.curRow["PromoEnds"], &packageDate);
            if (packageDate >= todaysDate) {
	            sprintf(tmpstr, "%-16s  %s", DB.curRow["PackageTag"], DB.curRow["Description"]);
	            sList->addItem(DB.curRow["PackageTag"], DB.curRow["Description"], DB.curRow["InternalID"]);
            }
        } else {
	        sList->addItem(DB.curRow["PackageTag"], DB.curRow["Description"], DB.curRow["InternalID"]);
        }
    }

    sList->setCustomerID(myCustID);
    
    selItem = sList->getSelection();
    if (selItem != NULL) {
        // The user picked one.
		SubscriptionsDB sdb;
		ADB  	        tmpDB;
		CustomersDB     CDB;
		
		itemNo  = atol(selItem->key(2,0));

        CDB.get(myCustID);

		tmpDB.query("select InternalID, Price from PackagesData where PackageID = %ld and RatePlanID = %ld and CycleID = %ld", 
		  itemNo,
		  CDB.getLong("RatePlan"),
		  CDB.getLong("BillingCycle")
		);
		if (tmpDB.rowCount) {
			tmpDB.getrow();
            sdb.setValue("InternalID", (long) 0);
			sdb.setValue("CustomerID", myCustID);
			sdb.setValue("LoginID", sList->getLoginID());
			sdb.setValue("Active", (int) 1);
			sdb.setValue("PackageNo", itemNo);
			sdb.setValue("Quantity", (float) 1.0);
			sdb.setValue("Price", atof(tmpDB.curRow["Price"]));
            
			sdb.ins();
		}
		CDB.doSubscriptions();
    }
    refreshCustomer(myCustID);
    emit(customerUpdated(myCustID));
    
    // fprintf(stderr, "addPackageItem clicked and selected item number %d.\n", itemNo);
}



/*
** showInactiveChanged - A slot that gets called whenever the "Hide Inactive"
**                       checkbox gets toggled.
*/

void CustSubscrList::hideInactiveChanged()
{
    showInactive = 1;
    if (hideInactiveButton->isChecked()) showInactive = 0;
    refreshCustomer(myCustID);
}



/*
** PackageSelector - Inherits selection list.
*/

PackageSelector::PackageSelector
(
	QWidget* parent,
	const char* name
) : SelectionList( parent, name )
{
    extraLabel->setText("Login ID:");
    extraLabel->show();
    //extra->show();
    //loginList = new QComboBox(extra);
    extraList->show();
}

PackageSelector::~PackageSelector()
{
}

/*
** setCustID - This is used to fill the Login List combo box.
*/

void PackageSelector::setCustomerID(long CustID)
{
    ADB     DB;
    char    priLogin[64];
    int     curEnt = 0;
    int     priEnt = 0;

    strcpy(priLogin, "");
    
    extraList->clear();

    DB.query("select PrimaryLogin from Customers where CustomerID = %ld", CustID);
    if (DB.rowCount) {
        DB.getrow();
        strcpy(priLogin, DB.curRow["PrimaryLogin"]);
    }
    
    DB.query("select LoginID from Logins where CustomerID = %ld order by LoginID", CustID);
    if (DB.rowCount) while (DB.getrow()) {
        extraList->insertItem(DB.curRow["LoginID"]);
        if (!strcmp(priLogin, DB.curRow["LoginID"])) priEnt = curEnt;
        curEnt++;
    }
    extraList->setCurrentItem(priEnt);
}

/*
** getLoginID - Gets the currently selected LoginID from the selection list.
*/

const char *PackageSelector::getLoginID(void)
{
    return extraList->text(extraList->currentItem());
}

