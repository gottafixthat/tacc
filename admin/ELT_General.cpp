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

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <ADB.h>
#include <BlargDB.h>

#include "ELT_General.h"

using namespace Qt;

ELT_General::ELT_General
(
	QWidget* parent,
	const char* name
)
	: TAAWidget(parent, name)
{
    // We start out by disabling all of our buttons and fields.
    // By setting the Login Type ID to zero.
    
    bListIDX       = NULL;
    intItemName    = NULL;
    intItemDesc    = NULL;
    strcpy(intLoginClass, "");
    intItemActive  = 0;
    intItemSpace   = 0;
    intItemChan    = 0;
    
    // Create our widgets.
    QLabel  *itemNameLabel = new QLabel(this, "itemNameLabel");
    itemNameLabel->setText("Name:");
    itemNameLabel->setAlignment(AlignRight|AlignVCenter);

    itemName = new QLineEdit(this, "itemName");
    itemName->setMaxLength(32);
    connect(itemName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));

    QLabel  *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(AlignRight|AlignTop);

    description = new Q3MultiLineEdit(this, "description");
    connect(description, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    QLabel  *diskSpaceLabel = new QLabel(this, "diskSpaceLabel");
    diskSpaceLabel->setText("Disk Space:");
    diskSpaceLabel->setAlignment(AlignRight|AlignVCenter);

    diskSpace = new QSpinBox(this, "diskSpace");
    diskSpace->setRange(1,100);
    diskSpace->setSuffix("MB");
    connect(diskSpace, SIGNAL(valueChanged(int)), this, SLOT(diskSpaceChanged(int)));

    QLabel  *diskBillableLabel = new QLabel(this, "diskBillableLabel");
    diskBillableLabel->setText("Billable:");
    diskBillableLabel->setAlignment(AlignRight|AlignVCenter);

    diskBillableList = new QComboBox(false, this, "diskBillableList");
    connect(diskBillableList, SIGNAL(activated(int)), this, SLOT(diskBillableChanged(int)));
    
    QLabel  *dialupChannelsLabel = new QLabel(this, "dialupChannelsLabel");
    dialupChannelsLabel->setText("Dialup Channels:");
    dialupChannelsLabel->setAlignment(AlignRight|AlignVCenter);
    
    dialupChannels = new QSpinBox(this, "dialupChannels");
    dialupChannels->setRange(0,100);
    dialupChannels->setSuffix("Channels");
    connect(dialupChannels, SIGNAL(valueChanged(int)), this, SLOT(dialupChannelsChanged(int)));

    QLabel  *channelBillableLabel = new QLabel(this, "channelBillableLabel");
    channelBillableLabel->setText("Billable:");
    channelBillableLabel->setAlignment(AlignRight|AlignVCenter);

    channelBillableList = new QComboBox(false, this, "channelBillableList");
    connect(channelBillableList, SIGNAL(activated(int)), this, SLOT(channelBillableChanged(int)));
    
    QLabel  *additionalMailboxesLabel = new QLabel(this, "additionalMailboxesLabel");
    additionalMailboxesLabel->setText("Additional Mailboxes:");
    additionalMailboxesLabel->setAlignment(AlignRight|AlignVCenter);
    
    additionalMailboxes = new QSpinBox(this, "additionalMailboxes");
    additionalMailboxes->setRange(0,100);
    connect(additionalMailboxes, SIGNAL(valueChanged(int)), this, SLOT(additionalMailboxesChanged(int)));

    additionalMailboxesLabel->hide();
    additionalMailboxes->hide();

    QLabel  *loginClassLabel = new QLabel(this, "loginClassLabel");
    loginClassLabel->setText("Class:");
    loginClassLabel->setAlignment(AlignRight|AlignVCenter);

    loginClassList = new QComboBox(true, this, "loginClassList");
    connect(loginClassList, SIGNAL(activated(int)), this, SLOT(loginClassChanged(int)));
    
    isActive = new QCheckBox(this, "isActive");
    isActive->setText("&Active");
    connect(isActive, SIGNAL(toggled(bool)), this, SLOT(activeChanged(bool)));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Setup our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // The main elements will go best in a grid.
    Q3GridLayout *gl = new Q3GridLayout(4, 4);
    gl->setColStretch(0, false);
    gl->setColStretch(1, true);
    gl->setColStretch(2, false);
    gl->setColStretch(3, true);
    int curRow = 0;
    gl->addWidget(itemNameLabel,        curRow, 0);
    gl->addMultiCellWidget(itemName,    curRow, curRow, 1, 3);
    gl->setRowStretch(curRow, false);
    curRow++;
    gl->addWidget(descriptionLabel,    curRow, 0);
    gl->addMultiCellWidget(description, curRow, curRow, 1, 3);
    gl->setRowStretch(curRow, true);
    curRow++;
    gl->addWidget(diskSpaceLabel,       curRow, 0);
    gl->addWidget(diskSpace,            curRow, 1);
    gl->addWidget(diskBillableLabel,    curRow, 2);
    gl->addWidget(diskBillableList,     curRow, 3);
    gl->setRowStretch(curRow, false);
    curRow++;
    gl->addWidget(dialupChannelsLabel,  curRow, 0);
    gl->addWidget(dialupChannels,       curRow, 1);
    gl->addWidget(channelBillableLabel, curRow, 2);
    gl->addWidget(channelBillableList,  curRow, 3);
    gl->setRowStretch(curRow, false);
    curRow++;
    gl->addWidget(additionalMailboxesLabel, curRow, 0);
    gl->addWidget(additionalMailboxes,      curRow, 1);
    curRow++;
    gl->addWidget(loginClassLabel,      curRow, 0);
    gl->addWidget(loginClassList,       curRow, 1);

    ml->addLayout(gl, 1);

    // buttons
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(isActive, 0);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    loadBillableLists();
    
    setLoginTypeID((long) 0);
}


ELT_General::~ELT_General()
{
}


/*
** setLoginTypeID - Sets the current login type ID.
**                  Loads the information from the database.
*/

void ELT_General::setLoginTypeID(long newLoginTypeID)
{
    if (newLoginTypeID) {
        ADBTable    LTDB;
        LTDB.setTableName("LoginTypes");
        if (LTDB.get(newLoginTypeID)) {
            myLoginTypeID = newLoginTypeID;
	        itemName->setText(LTDB.getStr("LoginType"));
	        itemName->setEnabled(TRUE);
	        description->setText(LTDB.getStr("Description"));
	        description->setEnabled(TRUE);
	        isActive->setChecked(LTDB.getInt("Active"));
	        isActive->setEnabled(TRUE);
	        diskSpace->setValue(LTDB.getInt("DiskSpace"));
	        diskSpace->setEnabled(TRUE);
	        dialupChannels->setValue(LTDB.getInt("DialupChannels"));
	        dialupChannels->setEnabled(true);
	        additionalMailboxes->setValue(LTDB.getInt("AdditionalMailboxes"));
	        additionalMailboxes->setEnabled(true);
            diskBillableList->setEnabled(true);
            channelBillableList->setEnabled(true);
            loginClassList->setEnabled(true);
            
            saveButton->setEnabled(FALSE);
            
            // Now, store the values in memory for comparison.
            if (intItemName != NULL) delete intItemName;
            if (intItemDesc != NULL) delete intItemDesc;
            
            intItemName = new char[strlen(LTDB.getStr("LoginType"))+2];
            strcpy(intItemName, LTDB.getStr("LoginType"));
            intItemDesc = new char[strlen(LTDB.getStr("Description"))+2];
            strcpy(intItemDesc, LTDB.getStr("Description"));
            strcpy(intLoginClass, LTDB.getStr("LoginClass"));
            intItemActive = isActive->isChecked();
            intItemSpace  = LTDB.getInt("DiskSpace");
            intItemChan   = LTDB.getInt("DialupChannels");
            intDiskBill   = LTDB.getLong("DiskSpaceBillable");
            intChanBill   = LTDB.getLong("DialupChannelsBillable");
            intAddMail    = LTDB.getInt("AdditionalMailboxes");
            
            // Find the disk space billable and the dialup channel billables
            // in the lists.
            int     tmpPos = 0;
            while (tmpPos >= 0) {
                if (bListIDX[tmpPos] == intDiskBill) {
                    diskBillableList->setCurrentItem(tmpPos);
                    tmpPos = -1;
                } else {
                    tmpPos++;
                }
            }

            tmpPos = 0;
            while (tmpPos >= 0) {
                if (bListIDX[tmpPos] == intChanBill) {
                    channelBillableList->setCurrentItem(tmpPos);
                    tmpPos = -1;
                } else {
                    tmpPos++;
                }
            }

            // Find the login class in the loginClassList
            loginClassList->setCurrentText(LTDB.getStr("LoginClass"));
            strcpy(intLoginClass, LTDB.getStr("LoginClass"));
            
        } else {
            myLoginTypeID = 0;
        }
    } else {
        myLoginTypeID = 0;
    }
    
    if (!myLoginTypeID) {
        itemName->setText("");
        itemName->setEnabled(FALSE);
        description->setText("");
        description->setEnabled(FALSE);
        isActive->setChecked(FALSE);
        isActive->setEnabled(FALSE);
        diskSpace->setValue(1);
        diskSpace->setEnabled(FALSE);
        dialupChannels->setValue(0);
        dialupChannels->setEnabled(FALSE);
        additionalMailboxes->setValue(0);
        additionalMailboxes->setEnabled(FALSE);
        diskBillableList->setEnabled(FALSE);
        channelBillableList->setEnabled(FALSE);
        loginClassList->setEnabled(false);
        saveButton->setEnabled(FALSE);

        // Clear our internal variables
        if (intItemName != NULL) delete intItemName;
        if (intItemDesc != NULL) delete intItemDesc;
        intItemName    = NULL;
        intItemDesc    = NULL;
        strcpy(intLoginClass, "");

        intItemActive  = 0;
        intItemSpace   = 0;
        intItemChan    = 0;
        intAddMail     = 0;
        

    }
}


/*
** checkForSave - Compares the edited values for our selections with
**                the ones in memory to see if we are going to allow
**                the user to save.
*/

void ELT_General::checkForSave(void)
{
    int     allowSave = 0;
    
    if (intItemName != NULL) {
        if (strcmp(intItemName, itemName->text())) allowSave++;
    }
    
    if (intItemDesc != NULL) {
        if (strcmp(intItemDesc, description->text())) allowSave++;
    }
    
    if (strcmp(intLoginClass, loginClassList->currentText())) allowSave++;

    if (intItemActive != isActive->isChecked()) allowSave++;
    
    if (intItemSpace != diskSpace->cleanText().toInt()) allowSave++;
    
    if (intItemChan  != dialupChannels->cleanText().toInt()) allowSave++;
    
    if (intAddMail   != additionalMailboxes->cleanText().toInt()) allowSave++;

    if (bListIDX[diskBillableList->currentItem()] != intDiskBill) allowSave++;
    if (bListIDX[channelBillableList->currentItem()] != intChanBill) allowSave++;
    
    
    saveButton->setEnabled(allowSave);
    
}


/*
** save  - Saves the entries in the general tab to the database.
*/

void ELT_General::save()
{
    QApplication::setOverrideCursor(waitCursor);

	ADBTable    LTDB;
	LTDB.setTableName("LoginTypes");

    LTDB.get(myLoginTypeID);
    LTDB.setValue("LoginType", itemName->text());
    LTDB.setValue("Description", description->text());
    LTDB.setValue("Active", (int)isActive->isChecked());
    LTDB.setValue("DiskSpace", diskSpace->cleanText().toInt());
    LTDB.setValue("DialupChannels", dialupChannels->cleanText().toInt());
    LTDB.setValue("DiskSpaceBillable", bListIDX[diskBillableList->currentItem()]);
    LTDB.setValue("DialupChannelsBillable", bListIDX[channelBillableList->currentItem()]);
    LTDB.setValue("LoginClass", loginClassList->currentText());
    LTDB.setValue("AdditionalMailboxes", additionalMailboxes->cleanText().toInt());
    LTDB.upd();
    
    setLoginTypeID(myLoginTypeID);
    
    emit(loginTypeChanged());
    
    QApplication::restoreOverrideCursor();
}


/*
** loadBillableLists - Loads the list of available billable items
**                     into the diskBillableList and the channelBillableList
**                     as well as into an index for easy conversion.
*/

void ELT_General::loadBillableLists()
{
    ADB         DB;
    int         tmpCount = 0;
    
    if (bListIDX != NULL) delete bListIDX;

    diskBillableList->clear();
    channelBillableList->clear();
    
    DB.query("select ItemNumber, ItemID from Billables order by ItemID");
    bListIDX = new long[DB.rowCount + 2];
    bListIDX[tmpCount++] = 0;
    diskBillableList->insertItem("None");
    channelBillableList->insertItem("None");
    
    if (DB.rowCount) while (DB.getrow()) {
        bListIDX[tmpCount++] = atol(DB.curRow["ItemNumber"]);
        diskBillableList->insertItem(DB.curRow["ItemID"]);
        channelBillableList->insertItem(DB.curRow["ItemID"]);
    }

    // While we're here, go ahead and get the login class as well.
    DB.query("select distinct LoginClass from LoginTypes order by LoginClass");
    if (DB.rowCount) while (DB.getrow()) {
        loginClassList->insertItem(DB.curRow["LoginClass"]);
    }
}


// vim: expandtab
