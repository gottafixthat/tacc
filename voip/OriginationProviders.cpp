/*
***************************************************************************
**
** OriginatonProviders - An interface that allows an admin to manage the
** origination providers for VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <Cfg.h>

#include <TAA.h>
#include "OriginationProviders.h"
//#include "LogVoiceMail.h"

OriginationProviders::OriginationProviders
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Origination Providers" );

    opList = new QListView(this, "Origination Providers");
    opList->setAllColumnsShowFocus(true);
    opList->setRootIsDecorated(true);
    opList->addColumn("Vendor/Tag");
    opList->addColumn("Active");
    opList->addColumn("Avail");
    opList->addColumn("Total");
    opList->addColumn("Type");

    activeColumn    = 1;
    availColumn     = 2;
    totalColumn     = 3;
    idColumn        = 5;

    opList->setColumnAlignment(activeColumn, Qt::AlignRight);
    opList->setColumnAlignment(availColumn,  Qt::AlignRight);
    opList->setColumnAlignment(totalColumn,  Qt::AlignRight);


    addButton = new QPushButton(this, "Add Button");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    editButton = new QPushButton(this, "Edit Button");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editClicked()));

    deleteButton = new QPushButton(this, "Delete Button");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    deleteButton->setEnabled(false);

    closeButton = new QPushButton(this, "Close Button");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(opList, 1);

    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

OriginationProviders::~OriginationProviders()
{
}

/**
  * refreshList - Refreshes the origination provider list.
  */

void OriginationProviders::refreshList()
{
    ADB     myDB;

    QApplication::setOverrideCursor(Qt::waitCursor);
    opList->clear();
    // Get the list of vendors that we have in the VoIP origination list.
    myDB.query("select Vendors.CompanyName, VoIP_Origination.VendorID from Vendors, VoIP_Origination where Vendors.VendorID = VoIP_Origination.VendorID group by VoIP_Origination.VendorID");
    if (myDB.rowCount) {
        ADB     myDB2;
        ADB     myDB3;
        // Walk through the list of vendors we've got and populate it.
        while(myDB.getrow()) {
            // We'll populate the DID counts from the children
            long    inUse = 0;
            long    avail = 0;
            char    inUseStr[1024];
            char    availStr[1024];
            char    totalStr[1024];

            QListViewItem   *parentItem = new QListViewItem(opList, myDB.curRow["CompanyName"]);

            // Now get the items for this vendor
            myDB2.query("select OriginationID, VendorID, Tag, DeliveryMethod from VoIP_Origination where VendorID = %ld", atoi(myDB.curRow["VendorID"]));
            if (myDB2.rowCount) {
                while(myDB2.getrow()) {
                    long    dInUse = 0;
                    long    dAvail = 0;
                    char    typeStr[1024];


                    sprintf(inUseStr, "%ld", dInUse);
                    sprintf(availStr, "%ld", dAvail);
                    sprintf(totalStr, "%ld", dInUse + dAvail);
                    if (!atoi(myDB2.curRow["DeliveryMethod"])) strcpy(typeStr, "IP");
                    else strcpy(typeStr, "PRI");

                    QListViewItem   *curItem = new QListViewItem(parentItem, myDB2.curRow["Tag"], inUseStr, availStr, totalStr, typeStr, myDB2.curRow["OriginationID"]);

                }
            }


            sprintf(inUseStr, "%ld", inUse);
            sprintf(availStr, "%ld", avail);
            sprintf(totalStr, "%ld", inUse + avail);
            parentItem->setText(activeColumn, inUseStr);
            parentItem->setText(availColumn,  availStr);
            parentItem->setText(totalColumn,  totalStr);
        }
    }

    QApplication::restoreOverrideCursor();
}

/** refreshProvider - A slot that connects to the provider editor so the
  * provider that was affected can be refreshed after an edit.
  */
void OriginationProviders::refreshProvider(long providerID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void OriginationProviders::addClicked()
{
    OriginationProviderEditor   *ope;
    ope = new OriginationProviderEditor();
    ope->show();
    connect(ope, SIGNAL(providerSaved(long)), this, SLOT(refreshProvider(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void OriginationProviders::editClicked()
{
    QListViewItem   *curItem;
    curItem = opList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                OriginationProviderEditor   *ope;
                ope = new OriginationProviderEditor();
                ope->setProviderID(tmpID);
                ope->show();
                connect(ope, SIGNAL(providerSaved(long)), this, SLOT(refreshProvider(long)));
            }
        }
    }
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void OriginationProviders::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void OriginationProviders::closeClicked()
{
    delete this;
}


/**
  * OriginationProviderEditor - Allows the user to add or edit an
  * origination provider.
  */

OriginationProviderEditor::OriginationProviderEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Origination Provider Add" );

    originationID = 0;

    ADB     DB;

    QLabel  *vendorListLabel = new QLabel(this, "vendorListLabel");
    vendorListLabel->setText("Vendor:");
    vendorListLabel->setAlignment(Qt::AlignRight);

    vendorList = new QComboBox(this, "vendorList");
    // Fill the vendor list.
    DB.query("select CompanyName from Vendors order by CompanyName");
    if (DB.rowCount) while (DB.getrow()) {
        vendorList->insertItem(DB.curRow["CompanyName"]);
    }

    QLabel *tagLineLabel = new QLabel(this, "tagLineLabel");
    tagLineLabel->setText("Identifier:");
    tagLineLabel->setAlignment(Qt::AlignRight);

    tagLine = new QLineEdit(this, "tagLine");

    QLabel *deliveryMethodLabel = new QLabel(this, "deliveryMethodLabel");
    deliveryMethodLabel->setText("Delivery Method:");
    deliveryMethodLabel->setAlignment(Qt::AlignRight);

    deliveryMethod = new QComboBox(this, "deliveryMethod");
    deliveryMethod->insertItem("SIP");
    deliveryMethod->insertItem("PRI");

    QLabel *databaseHostLabel = new QLabel(this, "databaseHostLabel");
    databaseHostLabel->setText("Database Host:");
    databaseHostLabel->setAlignment(Qt::AlignRight);

    databaseHost = new QLineEdit(this, "databaseHost");

    QLabel *databaseNameLabel = new QLabel(this, "databaseNameLabel");
    databaseNameLabel->setText("Database Name:");
    databaseNameLabel->setAlignment(Qt::AlignRight);

    databaseName = new QLineEdit(this, "databaseName");

    QLabel *databaseUserLabel = new QLabel(this, "databaseUserLabel");
    databaseUserLabel->setText("Database User:");
    databaseUserLabel->setAlignment(Qt::AlignRight);

    databaseUser = new QLineEdit(this, "databaseUser");

    QLabel *databasePass1Label = new QLabel(this, "databasePass1Label");
    databasePass1Label->setText("Database Password:");
    databasePass1Label->setAlignment(Qt::AlignRight);

    databasePass1 = new QLineEdit(this, "databasePass1");
    databasePass1->setEchoMode(QLineEdit::Password);

    QLabel *databasePass2Label = new QLabel(this, "databasePass2Label");
    databasePass2Label->setText("Verify Password:");
    databasePass2Label->setAlignment(Qt::AlignRight);

    databasePass2 = new QLineEdit(this, "databasePass2");
    databasePass2->setEchoMode(QLineEdit::Password);

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(vendorListLabel,          curRow, 0);
    gl->addWidget(vendorList,               curRow, 1);
    curRow++;
    gl->addWidget(tagLineLabel,             curRow, 0);
    gl->addWidget(tagLine,                  curRow, 1);
    curRow++;
    gl->addWidget(deliveryMethodLabel,      curRow, 0);
    gl->addWidget(deliveryMethod,           curRow, 1);
    curRow++;
    gl->addWidget(databaseHostLabel,        curRow, 0);
    gl->addWidget(databaseHost,             curRow, 1);
    curRow++;
    gl->addWidget(databaseNameLabel,        curRow, 0);
    gl->addWidget(databaseName,             curRow, 1);
    curRow++;
    gl->addWidget(databaseUserLabel,        curRow, 0);
    gl->addWidget(databaseUser,             curRow, 1);
    curRow++;
    gl->addWidget(databasePass1Label,       curRow, 0);
    gl->addWidget(databasePass1,            curRow, 1);
    curRow++;
    gl->addWidget(databasePass2Label,       curRow, 0);
    gl->addWidget(databasePass2,            curRow, 1);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~OriginationProviderEditor - Destructor.
  */
OriginationProviderEditor::~OriginationProviderEditor()
{
}

/**
  * setProviderID - Activates the "edit" mode of the editor.
  * Loads up the specified provider ID and puts its values into
  * the form.
  * Returns true if the operation was successful, otherwise false.
  */
int OriginationProviderEditor::setProviderID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("VoIP_Origination");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        myDB.query("select CompanyName from Vendors where VendorID = '%ld'", DB.getLong("VendorID"));
        if (myDB.rowCount) {
            myDB.getrow();
            vendorList->setCurrentText(myDB.curRow["CompanyName"]);
        }

        tagLine->setText(DB.getStr("Tag"));
        deliveryMethod->setCurrentItem(atoi(DB.getStr("DeliveryMethod")));
        databaseHost->setText(DB.getStr("DBHost"));
        databaseName->setText(DB.getStr("DBName"));
        databaseUser->setText(DB.getStr("DBUser"));
        databasePass1->setText(DB.getStr("DBPass"));
        databasePass2->setText(DB.getStr("DBPass"));

        retVal = 1;
        originationID = newID;
    }

    setCaption( "Origination Provider Edit" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void OriginationProviderEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;
    long    vendorID = 0;

    // Get the vendor
    DB.query("select VendorID from Vendors where CompanyName = '%s'", (const char *)vendorList->currentText());
    if (!DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Provider", "No vendor could be found for this provider.");
        return;
    }
    DB.getrow();
    vendorID = atol(DB.curRow["VendorID"]);

    // Validate the tag line.
    if (tagLine->text().length() < 2 || tagLine->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Provider", "Tag line must be between 2 and 64 characters long.");
        return;
    }
    DB.query("select OriginationID from VoIP_Origination where Tag = '%s' and OriginationID <> %ld", (const char *)tagLine->text(), originationID);
    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Provider", "Tag lines must be unique.");
        return;
    }

    // Validate the database host
    if (databaseHost->text().length() < 2 || databaseHost->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Provider", "Database host name must be between 2 and 64 characters long.");
        return;
    }

    if (databaseName->text().length() < 2 || databaseName->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Provider", "Database name must be between 2 and 64 characters long.");
        return;
    }

    if (databaseUser->text().length() < 2 || databaseUser->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Provider", "Database user must be between 2 and 64 characters long.");
        return;
    }

    if (databasePass1->text().length() < 2 || databasePass1->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Provider", "Database password must be between 2 and 64 characters long.");
        return;
    }

    if (strcmp(databasePass1->text(), databasePass2->text())) {
        QMessageBox::critical(this, "Unable to save Provider", "Database passwords do not match.");
        return;
    }

    // Try connecting to the database they specified, and if we can't, give them a warning
    // but save anyway.
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     tmpDB((const char *)databaseName->text(), (const char *)databaseUser->text(), (const char *)databasePass1->text(), (const char *)databaseHost->text());
    QApplication::restoreOverrideCursor();
    if (!tmpDB.Connected()) {
        QMessageBox::warning(this, "Database Connection Problem", "Warning:  Unable to connect to the specified database.");
    }

    // Now check to see if we are updating or adding.
    ADBTable    opDB("VoIP_Origination");
    if (originationID) {
        opDB.get(originationID);
    }

    opDB.setValue("VendorID",       vendorID);
    opDB.setValue("Tag",            (const char *)tagLine->text());
    opDB.setValue("DeliveryMethod", deliveryMethod->currentItem());
    opDB.setValue("DBHost",         (const char *)databaseHost->text());
    opDB.setValue("DBName",         (const char *)databaseName->text());
    opDB.setValue("DBUser",         (const char *)databaseUser->text());
    opDB.setValue("DBPass",         (const char *)databasePass1->text());

    if (originationID) {
        opDB.upd();
    } else {
        opDB.ins();
        originationID = opDB.getLong("OriginationID");
    }

    emit(providerSaved(originationID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void OriginationProviderEditor::cancelClicked()
{
    delete this;
}

