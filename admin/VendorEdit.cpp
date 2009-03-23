/**
 * VendorEdit.h - Class file for the Vendor Edit widget.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qlabel.h>
#include <qlayout.h>
#include <qgroupbox.h>

#include <ADB.h>
#include <BlargDB.h>

#include <VendorEdit.h>

VendorEdit::VendorEdit(QWidget* parent, const char* name, int vID) :
    TAAWidget(parent, name)
{
	myVendorID = vID;
	
    QLabel *companyNameLabel = new QLabel(this);
    companyNameLabel->setAlignment(AlignRight|AlignVCenter);
    companyNameLabel->setText("Company name:");

    companyName = new QLineEdit(this, "companyName");
    companyName->setMaxLength(60);

    QLabel *contactNameLabel = new QLabel(this);
    contactNameLabel->setAlignment(AlignRight|AlignVCenter);
    contactNameLabel->setText("Contact name:");

    contactName = new QLineEdit(this, "contactName");
    contactName->setMaxLength(60);

    QLabel *vendorIDLabel = new QLabel(this);
    vendorIDLabel->setAlignment(AlignRight|AlignVCenter);
    vendorIDLabel->setText("Vendor ID:");

    vendorID = new QLineEdit(this, "vendorID");
    vendorID->setMaxLength(60);
    vendorID->setEnabled(false);

    isActive = new QCheckBox(this, "isActive");
    isActive->setText("Active");

    QLabel *altContactLabel = new QLabel(this);
    altContactLabel->setAlignment(AlignRight|AlignVCenter);
    altContactLabel->setText("Alt Contact:");

    altContact = new QLineEdit(this, "altContact");
    altContact->setMaxLength(60);


    // Now things get a bit more complicated.  I like the 
    // frame around the QGroupBox, but since we'll have a semi-complex
    // widget layout, we need to create the groupbox, then the one widget
    // to put in it, and inherit all the rest from that one widget.
    // The final layout for the Address section (and similarly the phone
    // and account info sections) will look something like this:
    //
    // +-- Address ------------------------------------------------------+
    // |  Address Line 1: _____________________________________________  |
    // |  Address Line 2: _____________________________________________  |
    // | City, State ZIP: ______________________ _______ ______________  |
    // +-----------------------------------------------------------------+
    //
    // I may change all this up later and create a customer-like contact
    // and address section, but for now, one address and one phone for
    // vendors.
    QGroupBox   *addrGB = new QGroupBox(1, Horizontal, "Company Address", this, "addrGB");

    // Now the widget for the address group box.
    TAAWidget   *addrWidget = new TAAWidget(addrGB, "addrWidget");

    QLabel *address1Label = new QLabel(addrWidget);
    address1Label->setAlignment(AlignRight|AlignVCenter);
    address1Label->setText("Address Line 1:");
    
    address1 = new QLineEdit(addrWidget, "address1");
    address1->setMaxLength(60);

    QLabel *address2Label = new QLabel(addrWidget);
    address2Label->setAlignment(AlignRight|AlignVCenter);
    address2Label->setText("Address Line 2:");
    
    address2 = new QLineEdit(addrWidget, "address2");
    address2->setMaxLength(60);

    QLabel *cityStateZIPLabel = new QLabel(addrWidget);
    cityStateZIPLabel->setAlignment(AlignRight|AlignVCenter);
    cityStateZIPLabel->setText("City, State, ZIP:");

    city = new QLineEdit(addrWidget, "city");
    city->setMaxLength(60);

    state = new QLineEdit(addrWidget, "state");
    state->setMaxLength(2);

    zip = new QLineEdit(addrWidget, "zip");
    zip->setMaxLength(10);

    // Now create the layouts needed for these widgets.
    QGridLayout *addressLayout = new QGridLayout(addrWidget, 3, 2, 2);
    QBoxLayout  *cityStateZIPLayout = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    cityStateZIPLayout->addWidget(city, 1);
    cityStateZIPLayout->addWidget(state, 0);
    cityStateZIPLayout->addWidget(zip, 0);
    int curRow = 0;
    addressLayout->addWidget(address1Label,         curRow, 0);
    addressLayout->addWidget(address1,              curRow, 1);
    addressLayout->setRowStretch(curRow, 0);

    curRow++;
    addressLayout->addWidget(address2Label,         curRow, 0);
    addressLayout->addWidget(address2,              curRow, 1);
    addressLayout->setRowStretch(curRow, 0);
    
    curRow++;
    addressLayout->addWidget(cityStateZIPLabel,     curRow, 0);
    addressLayout->addLayout(cityStateZIPLayout,    curRow, 1);
    addressLayout->setRowStretch(curRow, 0);
    addressLayout->setColStretch(0, 0);
    addressLayout->setColStretch(1, 1);
    
    // Phone numbers...
    QGroupBox   *phoneGB = new QGroupBox(1, Horizontal, "Phone Numbers", this, "phoneGB");

    // Now the widget for the address group box.
    TAAWidget   *phoneWidget = new TAAWidget(phoneGB, "phoneWidget");

    QLabel *phoneLabel = new QLabel(phoneWidget);
    phoneLabel->setAlignment(AlignRight|AlignVCenter);
    phoneLabel->setText("Primary Phone:");

    phone = new QLineEdit(phoneWidget, "phone");
    phone->setMaxLength(16);

    QLabel *altPhoneLabel = new QLabel(phoneWidget);
    altPhoneLabel->setAlignment(AlignRight|AlignVCenter);
    altPhoneLabel->setText("Alternate Phone:");

    altPhone = new QLineEdit(phoneWidget, "altPhone");
    altPhone->setMaxLength(16);

    QLabel *faxLabel = new QLabel(phoneWidget);
    faxLabel->setAlignment(AlignRight|AlignVCenter);
    faxLabel->setText("Fax:");

    fax = new QLineEdit(phoneWidget, "fax");
    fax->setMaxLength(16);

    // Now create the layouts needed for the phone widgets.
    QGridLayout *phoneLayout = new QGridLayout(phoneWidget, 3, 2);
    curRow = 0;
    phoneLayout->addWidget(phoneLabel,      curRow, 0);
    phoneLayout->addWidget(phone,           curRow, 1);
    phoneLayout->setRowStretch(curRow, 0);

    curRow++;
    phoneLayout->addWidget(altPhoneLabel,   curRow, 0);
    phoneLayout->addWidget(altPhone,        curRow, 1);
    phoneLayout->setRowStretch(curRow, 0);
    
    curRow++;
    phoneLayout->addWidget(faxLabel,        curRow, 0);
    phoneLayout->addWidget(fax,             curRow, 1);
    phoneLayout->setRowStretch(curRow, 0);
    phoneLayout->setColStretch(0, 0);
    phoneLayout->setColStretch(1, 1);

    // Account Information
    QGroupBox   *acctGB = new QGroupBox(1, Horizontal, "Account Information", this, "acctGB");

    // Now the widget for the address group box.
    TAAWidget   *acctWidget = new TAAWidget(acctGB, "acctWidget");

    QLabel *checkNameLabel = new QLabel(acctWidget);
    checkNameLabel->setAlignment(AlignRight|AlignVCenter);
    checkNameLabel->setText("Print check as:");

    checkName = new QLineEdit(acctWidget, "checkName");
    checkName->setMaxLength(60);

    QLabel *accountNoLabel = new QLabel(acctWidget);
    accountNoLabel->setAlignment(AlignRight|AlignVCenter);
    accountNoLabel->setText("Acct. Number:");

    accountNo = new QLineEdit(acctWidget, "accountNo");
    accountNo->setMaxLength(60);

    QLabel *vendorTypeLabel = new QLabel(acctWidget);
    vendorTypeLabel->setAlignment(AlignRight|AlignVCenter);
    vendorTypeLabel->setText("Vendor Type:");

    vendorType = new QComboBox(false, acctWidget, "vendorType");

    QLabel *termsLabel = new QLabel(acctWidget);
    termsLabel->setAlignment(AlignRight|AlignVCenter);
    termsLabel->setText("Terms:");

    terms = new QComboBox(false, acctWidget, "terms");

    QLabel *taxIDLabel = new QLabel(acctWidget);
    taxIDLabel->setAlignment(AlignRight|AlignVCenter);
    taxIDLabel->setText("Tax ID:");

    taxID = new QLineEdit(acctWidget, "taxID");
    taxID->setMaxLength(16);

    use1099 = new QCheckBox(acctWidget, "use1099");
    use1099->setText("Needs 1099");

    QLabel *creditLimitLabel = new QLabel(acctWidget);
    creditLimitLabel->setAlignment(AlignRight|AlignVCenter);
    creditLimitLabel->setText("Credit Limit:");

    creditLimit = new QLineEdit(acctWidget, "creditLimit");
    creditLimit->setMaxLength(16);

    QLabel *balanceLabel = new QLabel(acctWidget);
    balanceLabel->setAlignment(AlignRight|AlignVCenter);
    balanceLabel->setText("Balance:");

    balance = new QLabel(acctWidget, "balance");

    // Now create the layouts needed for the account info widgets.
    QGridLayout *acctLayout = new QGridLayout(acctWidget, 3, 3);
    curRow = 0;
    acctLayout->addWidget(checkNameLabel,          curRow, 0);
    acctLayout->addMultiCellWidget(checkName,      curRow, curRow, 1, 2);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(accountNoLabel,          curRow, 0);
    acctLayout->addMultiCellWidget(accountNo,      curRow, curRow, 1, 2);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(vendorTypeLabel,         curRow, 0);
    acctLayout->addMultiCellWidget(vendorType,     curRow, curRow, 1, 2);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(termsLabel,              curRow, 0);
    acctLayout->addMultiCellWidget(terms,          curRow, curRow, 1, 2);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(taxIDLabel,              curRow, 0);
    acctLayout->addWidget(taxID,                   curRow, 1);
    acctLayout->addWidget(use1099,                 curRow, 2);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(creditLimitLabel,        curRow, 0);
    acctLayout->addWidget(creditLimit,             curRow, 1);
    acctLayout->setRowStretch(curRow, 0);

    curRow++;
    acctLayout->addWidget(balanceLabel,            curRow, 0);
    acctLayout->addWidget(balance,                 curRow, 1);
    acctLayout->setRowStretch(curRow, 0);
    acctLayout->setColStretch(0, 0);
    acctLayout->setColStretch(1, 1);
    acctLayout->setColStretch(2, 0);

    // Create our buttons, too.
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveVendor()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelVendor()));

    // Our main layout
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    // Vendor ID and Active share a grid cell.
    // So they need a layout.
    QBoxLayout *idactLayout = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    idactLayout->addWidget(vendorID, 1);
    idactLayout->addSpacing(20);
    idactLayout->addWidget(isActive, 0);

    // Top grid - company, vendor id, etc.
    QGridLayout *gl = new QGridLayout(4, 4, 3);
    curRow = 0;
    gl->addWidget(companyNameLabel,     curRow, 0);
    gl->addWidget(companyName,          curRow, 1);
    gl->addWidget(vendorIDLabel,        curRow, 2);
    gl->addLayout(idactLayout,          curRow, 3);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(contactNameLabel,     curRow, 0);
    gl->addWidget(contactName,          curRow, 1);
    gl->addWidget(altContactLabel,      curRow, 2);
    gl->addWidget(altContact,           curRow, 3);
    gl->setRowStretch(curRow, 0);

    // Add our address group box as a multi cell widget.
    curRow++;
    gl->addMultiCellWidget(addrGB,      curRow, curRow, 0, 1);
    gl->addMultiCellWidget(phoneGB,     curRow+1, curRow+1, 0, 1);
    gl->addMultiCellWidget(acctGB,      curRow, curRow+1, 2, 3);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    // Finally, our button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);


	VendorsDB *vendor = new VendorsDB;
	
	if (myVendorID) {
		setCaption("Edit Vendor");
		vendor->get(myVendorID);
	} else {
		setCaption("New Vendor");
	}

	companyName->setText(vendor->getStr("CompanyName"));
	contactName->setText(vendor->getStr("ContactName"));
	altContact->setText(vendor->getStr("AltContact"));
	address1->setText(vendor->getStr("Address1"));
	address2->setText(vendor->getStr("Address2"));
	city->setText(vendor->getStr("City"));
	state->setText(vendor->getStr("State"));
	zip->setText(vendor->getStr("ZIP"));
	checkName->setText(vendor->getStr("CheckName"));
	accountNo->setText(vendor->getStr("AccountNo"));
	taxID->setText(vendor->getStr("TaxID"));
	use1099->setChecked(vendor->getInt("Use1099"));
	creditLimit->setText(vendor->getStr("CreditLimit"));
	isActive->setChecked(vendor->getInt("Active"));

/*
	companyName->setText(vendor->CompanyName);
	contactName->setText(vendor->ContactName);
	altContact->setText(vendor->AltContact);
	address1->setText(vendor->Address1);
	address2->setText(vendor->Address2);
	city->setText(vendor->City);
	state->setText(vendor->State);
	zip->setText(vendor->ZIP);
	checkName->setText(vendor->CheckName);
	accountNo->setText(vendor->AccountNo);
	taxID->setText(vendor->TaxID);
	use1099->setChecked(vendor->Use1099.toInt());
	creditLimit->setText(vendor->CreditLimit);
	active->setChecked(vendor->Active.toInt());
*/
}


VendorEdit::~VendorEdit()
{
	//delete vendor;
}



/*
** saveVendor - The slot for saveing.  Saves the entry and cleans up 
**				the memory used.
*/ 

void VendorEdit::saveVendor()
{
    VendorsDB   vendor;
	vendor.setValue("CompanyName", companyName->text());
	vendor.setValue("ContactName", contactName->text());
	vendor.setValue("AltContact", altContact->text());
	vendor.setValue("Address1", address1->text());
	vendor.setValue("Address2", address2->text());
	vendor.setValue("City", city->text());
	vendor.setValue("State", state->text());
	vendor.setValue("ZIP", zip->text());
	vendor.setValue("CheckName", checkName->text());
	vendor.setValue("AccountNo", accountNo->text());
	vendor.setValue("TaxID", taxID->text());
	vendor.setValue("Use1099", use1099->isChecked());
	vendor.setValue("CreditLimit", creditLimit->text());
	vendor.setValue("Active", isActive->isChecked());

	if (myVendorID) { 
		vendor.upd();
	} else {
		vendor.ins();
	}
	emit refresh(1);
	close();
}

/**
 ** cancelVendor - The slot for cancelling.  Cleans up the memory used.
 **/ 

void VendorEdit::cancelVendor()
{
	close();
}

