/**
 * CustomerContactEditor.cpp - Customer Contact editor class.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <BlargDB.h>
#include <TAATools.h>
#include <CustomerContactsDB.h>
#include <CustomerContactEditor.h>

using namespace Qt;

/**
 * CustomerContactEditor()
 *
 * Constructor.  Sets up the main widget layout and structure.
 */
CustomerContactEditor::CustomerContactEditor(QWidget *parent, const char * name) 
    : TAAWidget(parent, name)
{
    setCaption("Customer Contact Editor");

    QLabel      *custNameLabel = new QLabel(this, "custNameLabel");
    custNameLabel->setText("Customer Name:");
    custNameLabel->setAlignment(Qt::AlignRight);

    custName = new QLabel(this, "custName");

    QLabel      *custIDLabel = new QLabel(this, "custIDLabel");
    custIDLabel->setText("Customer ID:");
    custIDLabel->setAlignment(Qt::AlignRight);

    custID = new QLabel(this, "custID");


    QLabel      *tagLabel = new QLabel(this, "tagLabel");
    tagLabel->setText("Tag:");
    tagLabel->setAlignment(Qt::AlignRight);

    Tag = new QComboBox(true, this);    // Make it read/write

    QLabel      *nameLabel = new QLabel(this, "nameLabel");
    nameLabel->setText("Name:");
    nameLabel->setAlignment(Qt::AlignRight);
    
    Name = new QLineEdit(this, "Name");
    Name->setMaxLength(64);

    QLabel      *accessLabel = new QLabel(this, "accessLabel");
    accessLabel->setText("Access:");
    accessLabel->setAlignment(Qt::AlignRight);
    
    Access = new QComboBox(true, this);    // Make it read/write

    International = new QCheckBox("International", this);

    QLabel      *phoneNumberLabel = new QLabel(this, "phoneNumberLabel");
    phoneNumberLabel->setText("Phone Number:");
    phoneNumberLabel->setAlignment(Qt::AlignRight);

    PhoneNumber = new QLineEdit(this, "PhoneNumber");
    PhoneNumber->setMaxLength(32);
    //PhoneNumber->setInputMask("(999) 999-9999");

    QLabel      *emailAddressLabel = new QLabel(this, "emailAddressLabel");
    emailAddressLabel->setText("Email Address:");
    emailAddressLabel->setAlignment(Qt::AlignRight);

    EmailAddress = new QLineEdit(this, "EmailAddress");
    EmailAddress->setMaxLength(64);

    BillingEmail = new QCheckBox("Email Statements", this);

    Active = new QCheckBox("Active", this);

    // Action buttons
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    // Create the layout now.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // Create a grid for our input data.
    Q3GridLayout *gl = new Q3GridLayout(3,5);
    int rowNum = 0;
    gl->addWidget(custNameLabel,            rowNum, 0);
    gl->addWidget(custName,                 rowNum, 1);
    rowNum++;
    gl->addWidget(custIDLabel,              rowNum, 0);
    gl->addWidget(custID,                   rowNum, 1);
    rowNum++;
    gl->addWidget(tagLabel,                 rowNum, 0);
    gl->addWidget(Tag,                      rowNum, 1);
    rowNum++;
    gl->addWidget(nameLabel,                rowNum, 0);
    gl->addWidget(Name,                     rowNum, 1);
    rowNum++;
    gl->addWidget(accessLabel,              rowNum, 0);
    gl->addWidget(Access,                   rowNum, 1);
    rowNum++;
    gl->addWidget(International,            rowNum, 1);
    rowNum++;
    gl->addWidget(phoneNumberLabel,         rowNum, 0);
    gl->addWidget(PhoneNumber,              rowNum, 1);
    rowNum++;
    gl->addWidget(emailAddressLabel,        rowNum, 0);
    gl->addWidget(EmailAddress,             rowNum, 1);
    rowNum++;
    gl->addWidget(BillingEmail,             rowNum, 1);
    rowNum++;
    gl->addWidget(Active,                   rowNum, 1);
    rowNum++;
    gl->setColStretch(0, 0);
    ml->addLayout(gl, 0);

    ml->addStretch(1);
    // Button layout
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    contactRec.contactID    = 0;
    contactRec.customerID   = 0;
    contactRec.flags        = 0;
    contactRec.sendStatements = 0;
    contactRec.active       = 0;
    contactRec.international= 0;
}

/**
 * ~CustomerContactEditor()
 *
 * Destructor
 */
CustomerContactEditor::~CustomerContactEditor()
{
}

/**
 * setContactID()
 *
 * Given a CustomerContactID, this loads the contact data from the database
 * fills in the fields and puts the editor into edit mode.
 */
void CustomerContactEditor::setContactID(uint id)
{
    CustomerContactsDB  cdb;
    if (cdb.loadContact(id) == id) {
        contactRec = cdb.getCustomerContact();
        Name->setText(contactRec.name);
        if (contactRec.international) International->setChecked(true);
        else International->setChecked(false);
        PhoneNumber->setText(contactRec.phoneNumber);
        EmailAddress->setText(contactRec.emailAddress);
        if (contactRec.sendStatements) BillingEmail->setChecked(true);
        else BillingEmail->setChecked(false);
        if (contactRec.active) Active->setChecked(true);
        else Active->setChecked(false);

        // Fill the combo box with all of the tags from this customer.
        customerContactList contacts;
        contacts = CustomerContactsDB::allCustomerContacts(contactRec.customerID);
        Tag->clear();
        for (int i = 0; i < contacts.count(); i++) {
            if (contacts.at(i).tag.length()) {
                Tag->insertItem(contacts.at(i).tag);
            }
        }
        Tag->setCurrentText(contactRec.tag);
        // And the same with Access
        Access->clear();
        for (int i = 0; i < contacts.count(); i++) {
            if (contacts.at(i).access.length()) {
                Access->insertItem(contacts.at(i).access);
            }
        }
        Access->setCurrentText(contactRec.access);

        myCustID = contactRec.customerID;
        updateCustomerFields();
    }
}

/**
 * setCustomerID()
 *
 * This needs to be called when adding a new customer contact.  It defines
 * the customer ID to save the record to.  Without it, it won't save.
 */
void CustomerContactEditor::setCustomerID(uint id)
{
    contactRec.customerID = id;
    myCustID = id;
    updateCustomerFields();

    // Fill the combo box with all of the tags from this customer.
    customerContactList contacts;
    contacts = CustomerContactsDB::allCustomerContacts(contactRec.customerID);
    Tag->clear();
    for (int i = 0; i < contacts.count(); i++) {
        if (contacts.at(i).tag.length()) {
            Tag->insertItem(contacts.at(i).tag);
        }
    }

    // And the same with Access
    Access->clear();
    for (int i = 0; i < contacts.count(); i++) {
        if (contacts.at(i).access.length()) {
            Access->insertItem(contacts.at(i).access);
        }
    }
}

/**
 * updateCustomerFields()
 *
 * Takes the current customer ID and updates the display fields.
 */
void CustomerContactEditor::updateCustomerFields()
{
    CustomersDB     CDB;

    CDB.get((long)myCustID);

    QString nameStr = CDB.getStr("FullName");
    QString idStr = QString("%1") . arg(myCustID);

    custName->setText(nameStr);
    custID->setText(idStr);

}

/**
 * saveClicked()
 *
 * Called when the user clicks the save button.  It validates the
 * data and either adds or inserts the record into the database.
 */
void CustomerContactEditor::saveClicked()
{
    CustomerContactsDB      cdb;
    if (contactRec.contactID) {
        cdb.loadContact(contactRec.contactID);
    }

    cdb.setCustomerID(contactRec.customerID);
    cdb.setTag(Tag->currentText());
    cdb.setName(Name->text());
    cdb.setAccess(Access->currentText());
    if (International->isChecked()) cdb.setInternational((uint)1);
    else cdb.setInternational((uint)0);
    cdb.setPhoneNumber(PhoneNumber->text());
    cdb.setEmailAddress(EmailAddress->text());
    cdb.setFlags(0);
    if (BillingEmail->isChecked()) cdb.setSendStatements((uint)1);
    else cdb.setSendStatements((uint)0);
    if (Active->isChecked()) cdb.setActive((uint)1);
    else cdb.setActive((uint)0);

    cdb.setLastModifiedBy(curUser().userName);
    if (contactRec.contactID) {
        cdb.update();
    } else {
        cdb.insert();
    }

    emit(customerUpdated(contactRec.customerID));
    delete this;
}

/**
 * cancelClicked()
 *
 * Closes the window and deletes the object aborting the edit.
 */
void CustomerContactEditor::cancelClicked()
{
    delete this;
}


