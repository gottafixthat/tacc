/*
***************************************************************************
**
** VoIPServiceTypes - An interface that allows an admin to manage the
** VoIP Service types.
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
#include <VoIPCommon.h>
#include "VoIPServiceTypes.h"

VoIPServiceTypes::VoIPServiceTypes
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    setCaption( "VoIP Services" );

    vstList = new Q3ListView(this, "VoIP Services");
    vstList->setAllColumnsShowFocus(true);
    vstList->setRootIsDecorated(true);
    vstList->addColumn("Service Name/Flags");
    vstList->addColumn("Type");
    vstList->addColumn("Chan");
    connect(vstList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));
    connect(vstList, SIGNAL(returnPressed(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));

    idColumn        = 3;

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
    ml->addWidget(vstList, 1);

    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

VoIPServiceTypes::~VoIPServiceTypes()
{
}

/**
  * refreshList - Refreshes the origination provider list.
  */

void VoIPServiceTypes::refreshList()
{
    ADB     myDB;

    QApplication::setOverrideCursor(Qt::waitCursor);
    vstList->clear();
    // Get the list of vendors that we have in the VoIP origination list.
    myDB.query("select VoIPServiceID, ServiceName, ServiceType, BaseChannels from VoIP_Service_Types");
    if (myDB.rowCount) {
        ADB     myDB2;
        // Walk through the list of services we've got and populate it.
        while(myDB.getrow()) {
            char    typeStr[1024];

            switch(atoi(myDB.curRow["ServiceType"])) {
                case 0:
                    strcpy(typeStr, "SIP");
                    break;
                case 1:
                    strcpy(typeStr, "IAX");
                    break;
                default:
                    strcpy(typeStr, "Unknown");
                    break;
            }
                    
            Q3ListViewItem   *parentItem = new Q3ListViewItem(vstList, myDB.curRow["ServiceName"], typeStr, myDB.curRow["BaseChannels"], myDB.curRow["VoIPServiceID"]);

            // Now get the flags for this service type
            myDB2.query("select VoIPServiceItemID, ServiceFlag from VoIP_Service_Items where VoIPServiceID = %ld", atoi(myDB.curRow["VoIPServiceID"]));
            if (myDB2.rowCount) {
                while(myDB2.getrow()) {
                    char    flagStr[1024];

                    strcpy(flagStr, voipServiceFlagStr(atoi(myDB2.curRow["ServiceFlag"])));

                    Q3ListViewItem   *curItem = new Q3ListViewItem(parentItem, flagStr, "", "", myDB.curRow["VoIPServiceID"]);

                }
            }
        }
    }

    QApplication::restoreOverrideCursor();
}

/** refreshServiceItem - A slot that connects to the provider editor so the
  * provider that was affected can be refreshed after an edit.
  */
void VoIPServiceTypes::refreshServiceItem(long providerID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void VoIPServiceTypes::addClicked()
{
    VoIPServiceTypeEditor   *ope;
    ope = new VoIPServiceTypeEditor();
    ope->show();
    connect(ope, SIGNAL(serviceItemSaved(long)), this, SLOT(refreshServiceItem(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void VoIPServiceTypes::editClicked()
{
    Q3ListViewItem   *curItem;
    curItem = vstList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                VoIPServiceTypeEditor   *ope;
                ope = new VoIPServiceTypeEditor();
                ope->setServiceID(tmpID);
                ope->show();
                connect(ope, SIGNAL(serviceItemSaved(long)), this, SLOT(refreshServiceItem(long)));
            }
        }
    }
}

/**
  * itemDoubleClicked - Gets called when the user double clicks an item.
  */

void VoIPServiceTypes::itemDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) editClicked();
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void VoIPServiceTypes::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void VoIPServiceTypes::closeClicked()
{
    delete this;
}


/**
  * VoIPServiceTypeEditor - Allows the user to add or edit an
  * origination provider.
  */

VoIPServiceTypeEditor::VoIPServiceTypeEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    setCaption( "VoIP Service Type Add" );

    voipServiceID = 0;

    ADB     DB;

    QLabel *serviceNameLabel = new QLabel(this, "serviceNameLabel");
    serviceNameLabel->setText("Service Name:");
    serviceNameLabel->setAlignment(Qt::AlignRight);

    serviceName = new QLineEdit(this, "serviceName");

    QLabel  *serviceTypeLabel = new QLabel(this, "serviceTypeLabel");
    serviceTypeLabel->setText("Service Type:");
    serviceTypeLabel->setAlignment(Qt::AlignRight);

    serviceType = new QComboBox(this, "serviceType");
    serviceType->insertItem("SIP");
    serviceType->insertItem("IAX");

    QLabel *billableItemLabel = new QLabel(this, "billableItemLabel");
    billableItemLabel->setText("Billable:");
    billableItemLabel->setAlignment(Qt::AlignRight);
    
    billableItem = new QComboBox(this, "billableItem");

    QLabel *baseChannelsLabel = new QLabel(this, "baseChannelsLabel");
    baseChannelsLabel->setText("Base Channels:");
    baseChannelsLabel->setAlignment(Qt::AlignRight);
    
    baseChannels = new QSpinBox(1,9999,1,this, "baseChannels");

    QLabel *channelBillableLabel = new QLabel(this, "channelBillableLabel");
    channelBillableLabel->setText("Channel Billable:");
    channelBillableLabel->setAlignment(Qt::AlignRight);
    
    channelBillable = new QComboBox(this, "channelBillable");

    DB.query("select ItemID from Billables where ItemType = 1 order by ItemID");
    if (DB.rowCount) while (DB.getrow()) {
        billableItem->insertItem(DB.curRow["ItemID"]);
        channelBillable->insertItem(DB.curRow["ItemID"]);
    }

    flagsAvailable = new Q3ListView(this, "flagsAvailable");
    flagsAvailable->addColumn("Available Flags");
    connect(flagsAvailable, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(availableFlagDoubleClicked(Q3ListViewItem *)));

    int fCount = 0;
    char sfStr[1024];
    while (strlen(voipServiceFlagText[fCount])) {
        sprintf(sfStr, "%d", fCount+1);
        Q3ListViewItem *curItem = new Q3ListViewItem(flagsAvailable, voipServiceFlagText[fCount], sfStr);
        fCount++;
    }

    QPushButton *addFlagButton = new QPushButton(this, "addFlagButton");
    addFlagButton->setText("&Add");
    connect(addFlagButton, SIGNAL(clicked()), this, SLOT(addFlagClicked()));
    
    QPushButton *rmFlagButton = new QPushButton(this, "rmFlagButton");
    rmFlagButton->setText("&Remove");
    connect(rmFlagButton, SIGNAL(clicked()), this, SLOT(rmFlagClicked()));

    flagsAssigned = new Q3ListView(this, "flagsAssigned");
    flagsAssigned->addColumn("Assigned Flags");
    connect(flagsAssigned, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(assignedFlagDoubleClicked(Q3ListViewItem *)));

    // Server groups.
    serverGroups = new ServerGroupSelector(this, "serverGroupSelector");
    serverGroups->setAddButtonText("A&dd");
    serverGroups->setRemoveButtonText("R&emove");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3GridLayout *gl = new Q3GridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(serviceNameLabel,         curRow, 0);
    gl->addWidget(serviceName,              curRow, 1);
    curRow++;
    gl->addWidget(serviceTypeLabel,         curRow, 0);
    gl->addWidget(serviceType,              curRow, 1);
    curRow++;
    gl->addWidget(billableItemLabel,        curRow, 0);
    gl->addWidget(billableItem,             curRow, 1);
    curRow++;
    gl->addWidget(baseChannelsLabel,        curRow, 0);
    gl->addWidget(baseChannels,             curRow, 1);
    curRow++;
    gl->addWidget(channelBillableLabel,     curRow, 0);
    gl->addWidget(channelBillable,          curRow, 1);
    curRow++;

    HorizLine *hline1 = new HorizLine(this);
    gl->addMultiCellWidget(hline1, curRow, curRow, 0, 1);
    curRow++;

    // Create a layout for the flags
    Q3BoxLayout *fl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    fl->addWidget(flagsAvailable, 1);

    Q3BoxLayout *fbl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 3);
    fbl->addStretch(1);
    fbl->addWidget(addFlagButton, 0);
    fbl->addWidget(rmFlagButton, 0);
    fbl->addStretch(1);

    fl->addLayout(fbl, 0);

    fl->addWidget(flagsAssigned, 1);

    gl->addMultiCellLayout(fl,curRow, curRow, 0, 1);
    curRow++;

    HorizLine *hline2 = new HorizLine(this);
    gl->addMultiCellWidget(hline2, curRow, curRow, 0, 1);
    curRow++;

    // Create a layout for the server groups
    gl->addMultiCellWidget(serverGroups,curRow, curRow, 0, 1);
    curRow++;

    HorizLine *hline3 = new HorizLine(this);
    gl->addMultiCellWidget(hline3, curRow, curRow, 0, 1);
    curRow++;

    ml->addLayout(gl, 0);
    //ml->addStretch(1);
    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~VoIPServiceTypeEditor - Destructor.
  */
VoIPServiceTypeEditor::~VoIPServiceTypeEditor()
{
}

/**
  * setServiceID - Activates the "edit" mode of the editor.
  * Loads up the specified VoipServiceID and puts its values into
  * the form.
  * Returns true if the operation was successful, otherwise false.
  */
int VoIPServiceTypeEditor::setServiceID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("VoIP_Service_Types");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        serviceName->setText(DB.getStr("ServiceName"));
        serviceType->setCurrentItem(DB.getInt("ServiceType"));
        baseChannels->setValue(DB.getInt("BaseChannels"));

        // Get the billable items
        myDB.query("select ItemID from Billables where ItemNumber = %ld", DB.getLong("BillableItemID"));
        if (myDB.rowCount) {
            myDB.getrow();
            billableItem->setCurrentText(myDB.curRow["ItemID"]);
        }

        myDB.query("select ItemID from Billables where ItemNumber = %ld", DB.getLong("ChannelBillable"));
        if (myDB.rowCount) {
            myDB.getrow();
            channelBillable->setCurrentText(myDB.curRow["ItemID"]);
        }

        // Now set the flags
        Q3ListViewItem   *curItem = flagsAssigned->firstChild();
        if (curItem) {
            while(curItem) {
                Q3ListViewItem *tmpItem = new Q3ListViewItem(flagsAvailable, curItem->key(0,0), curItem->key(1,0));
                flagsAvailable->setCurrentItem(tmpItem);
                flagsAssigned->removeItem(curItem);
                curItem = curItem->itemBelow();
            }
            flagsAssigned->clear();
        }

        myDB.query("select ServiceFlag from VoIP_Service_Items where VoIPServiceID = %ld", newID);
        if (myDB.rowCount) while (myDB.getrow()) {
            // Find the item in the list.
            curItem = flagsAvailable->firstChild();
            while(curItem) {
                if (atoi(myDB.curRow["ServiceFlag"]) == atoi(curItem->key(1,0))) {
                    Q3ListViewItem *tmpItem = new Q3ListViewItem(flagsAssigned, curItem->key(0,0), curItem->key(1,0));
                    flagsAvailable->removeItem(curItem);
                    curItem = 0;
                } else {
                    curItem = curItem->itemBelow();
                }
            }
        }

        // Finally, set the server groups.
        serverGroups->reset();
        myDB.query("select AssociationID, ServerGroupID from ServerGroupAssociations where AssociatedFrom = %d and AssociatedID = %ld", (TACCModules)VoIPServiceTypes, newID);
        if (myDB.rowCount) while (myDB.getrow()) {
            serverGroups->assign(atol(myDB.curRow["ServerGroupID"]));
        }


        retVal = 1;
        voipServiceID = newID;
    }

    setCaption( "VoIP Service Type Edit" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void VoIPServiceTypeEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;
    long    billableItemInt = 0;
    long    chanBillableInt = 0;

    // Validate the service name line.
    if (serviceName->text().length() < 2 || serviceName->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Service", "Service name must be between 2 and 64 characters long.");
        return;
    }

    DB.query("select VoIPServiceID from VoIP_Service_Types where ServiceName = '%s' and VoIPServiceID <> %ld", (const char *)serviceName->text(), voipServiceID);
    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Service", "Service names must be unique.");
        return;
    }

    // The rest doesn't need to be validates since its combo boxes and lists only.
    
    // Get the billable item ID's for the base billable and the channel billable
    DB.query("select ItemNumber from Billables where ItemID = '%s'", (const char *)billableItem->currentText());
    if (DB.rowCount) {
        DB.getrow();
        billableItemInt = atol(DB.curRow["ItemNumber"]);
    }
    DB.query("select ItemNumber from Billables where ItemID = '%s'", (const char *)channelBillable->currentText());
    if (DB.rowCount) {
        DB.getrow();
        chanBillableInt = atol(DB.curRow["ItemNumber"]);
    }

    // Now check to see if we are updating or adding.
    ADBTable    vstDB("VoIP_Service_Types");
    if (voipServiceID) {
        vstDB.get(voipServiceID);
    }

    vstDB.setValue("ServiceName",       (const char *)serviceName->text());
    vstDB.setValue("ServiceType",       serviceType->currentItem());
    vstDB.setValue("BillableItemID",    billableItemInt);
    vstDB.setValue("BaseChannels",      baseChannels->value());
    vstDB.setValue("ChannelBillable",   chanBillableInt);

    if (voipServiceID) {
        vstDB.upd();
        DB.dbcmd("delete from VoIP_Service_Items where VoIPServiceID = %ld", voipServiceID);
    } else {
        vstDB.ins();
        voipServiceID = vstDB.getLong("VoIPServiceID");
    }

    // Now insert the Service Items
    Q3ListViewItem *curItem = flagsAssigned->firstChild();
    if (curItem) {
        while(curItem) {
            Q3ListViewItem *tmpItem = new Q3ListViewItem(flagsAvailable, curItem->key(0,0), curItem->key(1,0));
            DB.dbcmd("insert into VoIP_Service_Items (VoIPServiceID, ServiceFlag) values (%ld, %d)", voipServiceID, atoi(curItem->key(1,0)));
            curItem = curItem->itemBelow();
        }
        flagsAssigned->clear();
    }

    // And add the Server Groups
    if (voipServiceID) DB.dbcmd("delete from ServerGroupAssociations where AssociatedFrom = %d and AssociatedID = %ld", (TACCModules) VoIPServiceTypes, voipServiceID);
    long    *sGroups = serverGroups->getAssigned();
    int i = 0;
    while (sGroups[i]) {
        DB.dbcmd("insert into ServerGroupAssociations (AssociationID, AssociatedFrom, AssociatedID, ServerGroupID) values (0, %d, %d, %ld)",
                (TACCModules) VoIPServiceTypes, voipServiceID, sGroups[i]);
        i++;
    }

    emit(serviceItemSaved(voipServiceID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void VoIPServiceTypeEditor::cancelClicked()
{
    delete this;
}

/**
  * addFlagClicked - Moves the selected flag from the Available flags list
  * to the Assigned flags list.
  */
void VoIPServiceTypeEditor::addFlagClicked()
{
    Q3ListViewItem   *curItem = flagsAvailable->currentItem();
    if (curItem) {
        Q3ListViewItem *tmpItem = new Q3ListViewItem(flagsAssigned, curItem->key(0,0), curItem->key(1,0));
        flagsAssigned->setCurrentItem(tmpItem);
        flagsAvailable->removeItem(curItem);
    }
}

/**
  * rmFlagClicked - Moves the selected flag from the Assigned flags list
  * to the Available flags list.
  */
void VoIPServiceTypeEditor::rmFlagClicked()
{
    Q3ListViewItem   *curItem = flagsAssigned->currentItem();
    if (curItem) {
        Q3ListViewItem *tmpItem = new Q3ListViewItem(flagsAvailable, curItem->key(0,0), curItem->key(1,0));
        flagsAvailable->setCurrentItem(tmpItem);
        flagsAssigned->removeItem(curItem);
    }
}

/**
  * availableFlagDoubleClicked - When the user double clicks on a flag,
  * this is called.
  */
void VoIPServiceTypeEditor::availableFlagDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) addFlagClicked();
}

/**
  * assignedFlagDoubleClicked - When the user double clicks on a flag,
  * this is called.
  */
void VoIPServiceTypeEditor::assignedFlagDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) rmFlagClicked();
}

