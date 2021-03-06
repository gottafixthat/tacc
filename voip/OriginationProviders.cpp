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

#include <QtCore/QDateTime>
#include <QtGui/QLayout>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <QtGui/QLabel>
#include <Cfg.h>

#include <TAA.h>
#include "OriginationProviders.h"
//#include "LogVoiceMail.h"

OriginationProviders::OriginationProviders
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    setCaption( "Origination Providers" );

    opList = new Q3ListView(this, "Origination Providers");
    opList->setAllColumnsShowFocus(true);
    opList->setRootIsDecorated(true);
    opList->addColumn("Vendor/Tag");
    opList->addColumn("Active");
    opList->addColumn("Avail");
    opList->addColumn("Total");
    opList->addColumn("Type");
    connect(opList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));

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

    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(opList, 1);

    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
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

            Q3ListViewItem   *parentItem = new Q3ListViewItem(opList, myDB.curRow["CompanyName"]);

            // Now get the items for this vendor
            myDB2.query("select OriginationID, VendorID, Tag, DeliveryMethod from VoIP_Origination where VendorID = %ld", atoi(myDB.curRow["VendorID"]));
            if (myDB2.rowCount) {
                while(myDB2.getrow()) {
                    long    dInUse = 0;
                    long    dAvail = 0;
                    char    typeStr[1024];

                    myDB3.query("select count(DID) from DID_Inventory where CustomerID > 0 and OriginationID = %s", myDB2.curRow["OriginationID"]);
                    myDB3.getrow();
                    dInUse = atol(myDB3.curRow[0]);

                    myDB3.query("select count(DID) from DID_Inventory where CustomerID = 0 and OriginationID = %s", myDB2.curRow["OriginationID"]);
                    myDB3.getrow();
                    dAvail = atol(myDB3.curRow[0]);
                    inUse += dInUse;
                    avail += dAvail;

                    sprintf(inUseStr, "%ld", dInUse);
                    sprintf(availStr, "%ld", dAvail);
                    sprintf(totalStr, "%ld", dInUse + dAvail);
                    if (!atoi(myDB2.curRow["DeliveryMethod"])) strcpy(typeStr, "IP");
                    else strcpy(typeStr, "PRI");

                    Q3ListViewItem   *curItem = new Q3ListViewItem(parentItem, myDB2.curRow["Tag"], inUseStr, availStr, totalStr, typeStr, myDB2.curRow["OriginationID"]);

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
    Q3ListViewItem   *curItem;
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
  * itemDoubleClicked - When the user double clicks on an origination provider
  * this opens the edit window.
  */
void OriginationProviders::itemDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) editClicked();
}

/**
  * OriginationProviderEditor - Allows the user to add or edit an
  * origination provider.
  */

OriginationProviderEditor::OriginationProviderEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
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

    QLabel *serverGroupLabel = new QLabel(this, "serverGroupLabel");
    serverGroupLabel->setText("Server Group:");
    serverGroupLabel->setAlignment(Qt::AlignRight);

    serverGroup = new QComboBox(this, "serverGroup");
    DB.query("select ServerGroup from ServerGroups order by ServerGroup");
    if (DB.rowCount) while (DB.getrow()) {
        serverGroup->insertItem(DB.curRow["ServerGroup"]);
    }

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3GridLayout *gl = new Q3GridLayout(2, 2);
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
    gl->addWidget(serverGroupLabel,        curRow, 0);
    gl->addWidget(serverGroup,             curRow, 1);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
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

        // Get the server group
        myDB.query("select ServerGroup from ServerGroups where ServerGroupID = %ld", DB.getLong("ServerGroupID"));
        if (myDB.rowCount) {
            myDB.getrow();
            serverGroup->setCurrentText(myDB.curRow["ServerGroup"]);
        }

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
    long    serverGroupID = 0;

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

    // Get the server group, and make sure its valid.
    DB.query("select ServerGroupID from ServerGroups where ServerGroup = '%s'", (const char *)serverGroup->currentText());
    if (!DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Provider", "No server group could be found for this provider.");
        return;
    }
    DB.getrow();
    serverGroupID = atol(DB.curRow["ServerGroupID"]);

    // Now check to see if we are updating or adding.
    ADBTable    opDB("VoIP_Origination");
    if (originationID) {
        opDB.get(originationID);
    }

    opDB.setValue("VendorID",       vendorID);
    opDB.setValue("Tag",            (const char *)tagLine->text());
    opDB.setValue("DeliveryMethod", deliveryMethod->currentItem());
    opDB.setValue("ServerGroupID",  serverGroupID);

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

