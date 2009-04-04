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

#include <QtGui/QPixmap>
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include "Tab_ContactInfo.h"
#include "BlargDB.h"
#include "AddressEditor.h"
#include <TAAPixmaps.h>
#include <TAATools.h>
#include <ADB.h>
#include <TAAStructures.h>
#include <CustomerContactsDB.h>
#include <CustomerContactEditor.h>

using namespace Qt;

Tab_ContactInfo::Tab_ContactInfo
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget(parent)
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

    // RegNum (labels only)
    QLabel      *regNumLabel = new QLabel(this, "regNumLabel");
    regNumLabel->setText("RegNum:");
    regNumLabel->setAlignment(AlignRight|AlignVCenter);

    regNum = new QLabel(this, "regNum");
    regNum->setAlignment(AlignLeft|AlignVCenter);

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

    // Pixmaps
    QPixmap     clipCopy(edit_paste_xpm);
    QPushButton *copyButton = new QPushButton(this, "copyButton");
    copyButton->setPixmap(clipCopy);
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copyContactInfoToClipboard()));

    // Now, the contact list.
    contactList = new Q3ListView(this, "contactList");
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
    addressList = new Q3ListView(this, "Address List");
    addressList->addColumn("Street");
    addressList->addColumn("Apt");
    addressList->addColumn("City");
    addressList->addColumn("State");
    addressList->addColumn("ZIP");
    addressList->addColumn("Active");
    addressList->addColumn("Tag");
    addressList->setAllColumnsShowFocus(true);
    connect(addressList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(editCurrentAddressItem(Q3ListViewItem *)));

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
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // Create a grid layout for our name and contact names
    Q3GridLayout *cngl = new Q3GridLayout(3,2);
    int curRow = 0;
    cngl->addWidget(nameLabel,          curRow, 0);
    cngl->addWidget(fullName,           curRow, 1);
    curRow++;

    cngl->addWidget(contactNameLabel,   curRow, 0);
    cngl->addWidget(contactName,        curRow, 1);
    curRow++;

    cngl->addWidget(altContactLabel,    curRow, 0);
    cngl->addWidget(altContact,         curRow, 1);
    curRow++;
    cngl->setColStretch(0,0);
    cngl->setColStretch(1,1);

    // Now one for our text/informational fields.
    Q3GridLayout *igl = new Q3GridLayout(4,2);
    curRow = 0;
    igl->addWidget(customerIDLabel,     curRow, 0);
    igl->addWidget(customerID,          curRow, 1);
    curRow++;

    igl->addWidget(regNumLabel,         curRow, 0);
    igl->addWidget(regNum,              curRow, 1);
    curRow++;

    igl->addWidget(primaryLoginLabel,   curRow, 0);
    igl->addWidget(primaryLogin,        curRow, 1);
    curRow++;

    igl->addWidget(lastModifiedLabel,   curRow, 0);
    igl->addWidget(lastModified,        curRow, 1);
    curRow++;
    igl->setColStretch(0,0);
    igl->setColStretch(1,1);

    // And now a left to right box layout to hold the two grids.
    Q3BoxLayout  *hbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight);
    hbl->addLayout(cngl, 1);
    hbl->addLayout(igl,  1);

    // Our "special" layout for action buttons.
    Q3BoxLayout *specl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
    specl->addWidget(copyButton, 0);
    specl->addStretch(1);
    hbl->addLayout(specl,0);

    // And add the hbl to the main layout
    ml->addLayout(hbl, 0);

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
    
    Q3BoxLayout *cpbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    
    // Our contact list
    cpbl->addWidget(contactList, 1);
    Q3BoxLayout *cabl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
    cabl->addStretch(1);
    cabl->addWidget(addContactButton, 0);
    cabl->addWidget(editContactButton, 0);
    cabl->addWidget(deleteContactButton, 0);
    cpbl->addLayout(cabl, 0);


    ml->addLayout(cpbl, 1);
    
    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    abl->addWidget(addressList, 1);
    Q3BoxLayout *babl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
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
    for (int i = 0; i < contacts.count(); i++) {
        customerContactRecord ctc = contacts.at(i);
        QString tmpSt = QString::number(ctc.contactID);
        QString tmpFlags = "";
        if (ctc.sendStatements) tmpFlags += "Statements";
        (void) new Q3ListViewItem(contactList, ctc.tag, ctc.name, ctc.phoneNumber, ctc.emailAddress, ctc.access, tmpFlags, tmpSt);
    }
    debug(5, "Loaded %d contacts...\n", contacts.count());
	cdb.get(myCustID);
	
	fullName->setText(cdb.getStr("FullName"));
	contactName->setText(cdb.getStr("ContactName"));
	customerID->setText(cdb.getStr("CustomerID"));
	regNum->setText(cdb.getStr("RegNum"));
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
    Q3ListViewItem *curItem = addressList->currentItem();
    if (curItem) {
        AddressEditor	*AE;
        AE = new(AddressEditor);
        AE->editAddress(REF_CUSTOMER, myCustID, (const char *) curItem->key(6,0));
    }
}

void Tab_ContactInfo::editCurrentAddressItem(Q3ListViewItem *curItem)
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
    Q3ListViewItem   *curItem = addressList->currentItem();
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

        (void) new Q3ListViewItem(addressList, 
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
    Q3ListViewItem   *curItem = contactList->currentItem();
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
    Q3ListViewItem   *curItem = contactList->currentItem();
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

/**
 * copyContactInfoToClipboard()
 *
 * Copies various information from a qstring to the global 
 * clipboard.
 */
void Tab_ContactInfo::copyContactInfoToClipboard()
{
    QClipboard *cb = QApplication::clipboard();

    QString tmpStr;
    tmpStr = "Full Name: ";
    tmpStr += fullName->text();
    if (contactName->text().length()) {
        tmpStr += "\nContact Name: ";
        tmpStr += contactName->text();
    }
    if (contactList->currentItem()) {
        tmpStr += "\nPhone Number: ";
        tmpStr += contactList->currentItem()->key(2,0);
    }
    tmpStr += "\nRegNum: ";
    tmpStr += regNum->text();

    cb->setText(tmpStr, QClipboard::Clipboard);
        
}

// vim: expandtab

