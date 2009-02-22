/*
** $Id$
**
***************************************************************************
**
** Tab_ContactInfo - Displays and allows the editing of the customers
**                   contact information.
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
** $Log: Tab_ContactInfo.cpp,v $
** Revision 1.2  2003/12/30 01:53:02  marc
** Fixed the font and stretch issues between the contact list and phone list.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "Tab_ContactInfo.h"
#include "BlargDB.h"
#include "AddressEditor.h"
#include <stdio.h>
#include <qapp.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <TAATools.h>
#include <ADB.h>
#include <qmessagebox.h>
#include <TAAStructures.h>
#include <CustomerContactsDB.h>
#include <CustomerContactEditor.h>

Tab_ContactInfo::Tab_ContactInfo
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
	if (!CustID) return;

    // Create all of our widgets and our layout.

    // Customer full name
    QLabel      *nameLabel = new QLabel(this);
    nameLabel->setText("Name:");
    nameLabel->setAlignment(AlignRight|AlignVCenter);

    fullName = new QLineEdit(this, "FullName");
    fullName->setMaxLength(60);

    // Customer ID (labels only)
    QLabel      *customerIDLabel = new QLabel(this);
    customerIDLabel->setText("Customer ID:");
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);

    customerID = new QLabel(this);
    customerID->setAlignment(AlignLeft|AlignVCenter);

    // Contact Name
    QLabel      *contactNameLabel = new QLabel(this);
    contactNameLabel->setText("Contact Name:");
    contactNameLabel->setAlignment(AlignRight|AlignVCenter);

    contactName = new QLineEdit(this, "ContactName");
    contactName->setMaxLength(60);

    // Primary login (labels only)
    QLabel      *primaryLoginLabel = new QLabel(this);
    primaryLoginLabel->setText("Primary Login:");
    primaryLoginLabel->setAlignment(AlignRight|AlignVCenter);

    primaryLogin = new QLabel(this);
    primaryLogin->setAlignment(AlignLeft|AlignVCenter);

    // Alternate contact
    QLabel      *altContactLabel = new QLabel(this);
    altContactLabel->setText("Alt. Contact:");
    altContactLabel->setAlignment(AlignRight|AlignVCenter);

    altContact = new QLineEdit(this, "Alternate Contact");
    altContact->setMaxLength(60);

    // Last Modified (labels only)
    QLabel      *lastModifiedLabel = new QLabel(this);
    lastModifiedLabel->setText("Last Modified:");
    lastModifiedLabel->setAlignment(AlignRight|AlignVCenter);

    lastModified = new QLabel(this);
    lastModified->setAlignment(AlignLeft|AlignVCenter);

    // Now, the contact list.
    contactList = new QListView(this, "contactList");
    contactList->addColumn("Tag");
    contactList->addColumn("Name");
    contactList->addColumn("Phone");
    contactList->addColumn("Email");
    contactList->addColumn("Access");
    contactList->addColumn("Flags");
    contactList->setAllColumnsShowFocus(true);
    contactIDCol = 6;

    QPushButton *addContactButton = new QPushButton(this, "AddContactButton");
    addContactButton->setText("Add");
    connect(addContactButton, SIGNAL(clicked()), this, SLOT(addNewContact()));

    editContactButton = new QPushButton(this, "EditContactButton");
    editContactButton->setText("Edit");
    connect(editContactButton, SIGNAL(clicked()), this, SLOT(editCurrentContact()));

    deleteContactButton = new QPushButton(this, "DeleteContactButton");
    deleteContactButton->setText("Delete");
    connect(deleteContactButton, SIGNAL(clicked()), this, SLOT(deleteCurrentContact()));

    // Now, do the contact list.
    
    // Do the address list.
    addressList = new QListView(this, "Address List");
    addressList->addColumn("Street");
    addressList->addColumn("Apt");
    addressList->addColumn("City");
    addressList->addColumn("State");
    addressList->addColumn("ZIP");
    addressList->addColumn("Active");
    addressList->addColumn("Tag");
    addressList->setAllColumnsShowFocus(true);
    connect(addressList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(editCurrentAddressItem(QListViewItem *)));

    QPushButton *addAddrButton = new QPushButton(this, "AddAddressButton");
    addAddrButton->setText("Add");
    connect(addAddrButton, SIGNAL(clicked()), this, SLOT(addNewAddress()));

    editAddrButton = new QPushButton(this, "EditAddressButton");
    editAddrButton->setText("Edit");
    connect(editAddrButton, SIGNAL(clicked()), this, SLOT(editCurrentAddress()));

    deleteAddrButton = new QPushButton(this, "DeleteAddressButton");
    deleteAddrButton->setText("Delete");
    connect(deleteAddrButton, SIGNAL(clicked()), this, SLOT(deleteCurrentAddress()));



    // Thats it, create the (rather complex) layout.  We'll need several
    // grids within our main box.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // Create a grid for our contact information and our other misc info
    QGridLayout *cil = new QGridLayout(3,4);
    cil->addWidget(nameLabel,           0, 0);
    cil->addWidget(fullName,            0, 1);
    cil->addWidget(customerIDLabel,     0, 2);
    cil->addWidget(customerID,          0, 3);

    cil->addWidget(contactNameLabel,    1, 0);
    cil->addWidget(contactName,         1, 1);
    cil->addWidget(primaryLoginLabel,   1, 2);
    cil->addWidget(primaryLogin,        1, 3);
    
    cil->addWidget(altContactLabel,     2, 0);
    cil->addWidget(altContact,          2, 1);
    cil->addWidget(lastModifiedLabel,   2, 2);
    cil->addWidget(lastModified,        2, 3);

    cil->setColStretch(0, 0);
    cil->setColStretch(1, 1);
    cil->setColStretch(2, 0);
    cil->setColStretch(3, 1);

    ml->addLayout(cil, 0);          // Don't stretch the grid

    // The lists are much more complex.  They require two box layouts in
    // them:
    //
    //    +-Box Layout (Left to Right)------------------------+
    //    | +-----------------------------+ +---------------+ |
    //    | | QListView                   | | Box Layout    | |
    //    | |                             | |  +----------+ | |
    //    | |                             | |  | Button   | | |
    //    | |                             | |  +----------+ | |
    //    | |                             | |  +----------+ | |
    //    | |                             | |  | Button   | | |
    //    | |                             | |  +----------+ | |
    //    | +-----------------------------+ +---------------+ |
    //    +---------------------------------------------------+
    
    QBoxLayout *cpbl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    
    // Our contact list
    cpbl->addWidget(contactList, 1);
    QBoxLayout *cabl = new QBoxLayout(QBoxLayout::TopToBottom, 0);
    cabl->addStretch(1);
    cabl->addWidget(addContactButton, 0);
    cabl->addWidget(editContactButton, 0);
    cabl->addWidget(deleteContactButton, 0);
    cpbl->addLayout(cabl, 0);


    ml->addLayout(cpbl, 1);
    
    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    abl->addWidget(addressList, 1);
    QBoxLayout *babl = new QBoxLayout(QBoxLayout::TopToBottom, 0);
    babl->addStretch(1);
    babl->addWidget(addAddrButton, 0);
    babl->addWidget(editAddrButton, 0);
    babl->addWidget(deleteAddrButton, 0);
    abl->addLayout(babl, 0);

    ml->addLayout(abl, 1);          // This one gets stretch


	myCustID = CustID;
	
	setCaption( "Contact Info" );

    loadCustInfo();
	
	if (!isManager()) {
		deleteAddrButton->hide();
	}

    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
    connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
}


Tab_ContactInfo::~Tab_ContactInfo()
{
}

/*
** loadCustInfo  - Loads our customer information and displays it in the
**                 widget.
*/

void Tab_ContactInfo::loadCustInfo()
{
	CustomersDB cdb;
	AddressesDB adb;
    customerContactList contacts;
	
    contacts = CustomerContactsDB::allCustomerContacts((uint)myCustID);
    contactList->clear();
    for (uint i = 0; i < contacts.count(); i++) {
        customerContactRecord *ctc = contacts.at(i);
        QString tmpSt = QString::number(ctc->contactID);
        QString tmpFlags = "";
        if (ctc->sendStatements) tmpFlags += "Statements";
        (void) new QListViewItem(contactList, ctc->tag, ctc->name, ctc->phoneNumber, ctc->emailAddress, ctc->access, tmpFlags, tmpSt);
    }
    debug(5, "Loaded %d contacts...\n", contacts.count());
	cdb.get(myCustID);
	
	fullName->setText(cdb.getStr("FullName"));
	contactName->setText(cdb.getStr("ContactName"));
	customerID->setText(cdb.getStr("CustomerID"));
	altContact->setText(cdb.getStr("AltContact"));
	strcpy(myBillingTag, cdb.getStr("BillingAddress"));
	
	// Load the addresses for the customers into a list that the
	// user can select.
	refreshAddrList();

	primaryLogin->setText(cdb.getStr("PrimaryLogin"));
	lastModified->setText(cdb.getDateTime("LastModified").toString());


}

/*
** saveContactInfo - 	Saves any changed data back into the database.
**
*/

bool Tab_ContactInfo::saveContactInfo(void)
{
	CustomersDB cdb;
	
	cdb.get(myCustID);

	cdb.setValue("FullName", fullName->text());
	cdb.setValue("ContactName", contactName->text());
	cdb.setValue("AltContact", altContact->text());
	cdb.setValue("BillingAddress", myBillingTag);

	cdb.upd();
    emit(customerChanged(myCustID));
	return(TRUE);
}



void Tab_ContactInfo::editCurrentAddress()
{
    QListViewItem *curItem = addressList->currentItem();
    if (curItem) {
        AddressEditor	*AE;
        AE = new(AddressEditor);
        AE->editAddress(REF_CUSTOMER, myCustID, (const char *) curItem->key(6,0));
    }
}

void Tab_ContactInfo::editCurrentAddressItem(QListViewItem *curItem)
{
    if (curItem) {
        AddressEditor	*AE;
        AE = new(AddressEditor);
        AE->editAddress(REF_CUSTOMER, myCustID, (const char *) curItem->key(6,0));
    }
}

void Tab_ContactInfo::editCurrentAddressL(const QString &tag)
{
	AddressEditor	*AE;
	AE = new(AddressEditor);
	AE->editAddress(REF_CUSTOMER, myCustID, (const char *) tag);
}


void Tab_ContactInfo::addNewAddress()
{
	AddressEditor	*AE;
	AE = new(AddressEditor);
	AE->newAddress(REF_CUSTOMER, myCustID);
}


void Tab_ContactInfo::deleteCurrentAddress()
{
    QListViewItem   *curItem = addressList->currentItem();
    if (curItem) {
        ADB		    DB;
        
        if (QMessageBox::warning(this, "Delete Address", "Are you sure you wish to delete\nthe currently selected address?", "&Yes", "&No", 0, 1) == 0) {
            DB.dbcmd("delete from Addresses where RefFrom = %d and RefID = %ld and Tag = '%s'",
              REF_CUSTOMER,
              myCustID,
              (const char *) curItem->key(6,0)
            );
            emit(customerChanged(myCustID));
        }
    }
}


/*
** refreshAddrList - Refreshes the Address Listing.
**
*/

void Tab_ContactInfo::refreshAddrList(void)
{
    ADB         DB;
    char        tmpAct[1024];

    addressList->clear();

    DB.query("select * from Addresses where RefFrom = %d and RefID = %ld", REF_CUSTOMER, myCustID);
    if (DB.rowCount) while (DB.getrow()) {
        if (DB.curRow.col("Active")->toInt()) strcpy(tmpAct, "Yes");
        else strcpy(tmpAct, "No");

        (void) new QListViewItem(addressList, 
                                 DB.curRow["Address1"],
                                 DB.curRow["Address2"],
                                 DB.curRow["City"],
                                 DB.curRow["State"],
                                 DB.curRow["ZIP"],
                                 tmpAct,
                                 DB.curRow["Tag"],
                                 DB.curRow["InternalID"]
                                );
    }
}

/*
** addrRefreshSlot - A slot for connecting to the address editor so we know
**					 when an address has been updated.
**
*/

void Tab_ContactInfo::addrRefreshSlot(int RefFrom, long RefID)
{
	if ((RefFrom == REF_CUSTOMER) && (RefID == myCustID)) {
		refreshAddrList();
	}
}




// Contact routines...

void Tab_ContactInfo::editCurrentContact()
{
    QListViewItem   *curItem = contactList->currentItem();
    if (curItem) {
        CustomerContactEditor *CE;
        CE = new CustomerContactEditor();
        CE->setContactID(curItem->key(contactIDCol, 0).toInt());
        CE->show();
    }
}

void Tab_ContactInfo::addNewContact()
{
	CustomerContactEditor *CE;
	CE = new CustomerContactEditor();
    CE->setCustomerID(myCustID);
    CE->show();

	//connect(PE, SIGNAL(phoneUpdated(int, long)), SLOT(phoneRefreshSlot(int, long)));
}

void Tab_ContactInfo::deleteCurrentContact()
{
    QListViewItem   *curItem = contactList->currentItem();
    if (curItem) {
        CustomerContactsDB  CDB;
        
        if (QMessageBox::warning(this, "Delete Contact", "Are you sure you want to delete\nthe currently selected contact?", "&Yes", "&No", 0, 1) == 0) {
            if (CDB.loadContact(curItem->key(contactIDCol,0).toInt()) == curItem->key(contactIDCol,0).toInt()) {
                CDB.del();
                emit(customerChanged(myCustID));
            }
        }
    }
}


/*
** DeactiveAddress - A placeholder right now...
*/

void Tab_ContactInfo::DeactiveAddress()
{
}

/*
** customerChanged - Updates the customer data on the screen when called.
*/

void Tab_ContactInfo::refreshCustomer(long custID)
{
    if (custID == myCustID) {
        loadCustInfo();
    }
}

