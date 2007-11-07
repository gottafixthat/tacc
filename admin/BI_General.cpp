/*
** $Id$
**
***************************************************************************
**
** BI_General - Billable Items editing
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
** $Log: BI_General.cpp,v $
** Revision 1.4  2004/02/27 23:11:18  marc
** Fixed the way we're doing mailbox counts.  It now keys off of subscriptions.
**
** Revision 1.3  2003/12/27 21:37:29  marc
** Switched a couple char variables to static sizes.
**
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "BI_General.h"

#include <stdio.h>
#include <stdlib.h>

#include "BlargDB.h"
#include <ADB.h>
#include <TAAWidget.h>
#include <qlabel.h>
#include <qlayout.h>

BI_General::BI_General
(
    QWidget* parent,
    const char* name
) : TAAWidget( parent, name )
{
    // Setup our layout.
    QLabel  *itemNameLabel = new QLabel(this, "itemNameLabel");
    itemNameLabel->setText("Name:");
    itemNameLabel->setAlignment(AlignRight|AlignVCenter);

    itemName = new QLineEdit(this, "itemName");
    itemName->setMaxLength(32);
    connect(itemName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));

    QLabel  *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Base Description:");
    descriptionLabel->setAlignment(AlignRight|AlignTop);

    description = new QMultiLineEdit(this, "description");
    connect(description, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    QLabel  *itemTypeListLabel = new QLabel(this, "itemTypeListLabel");
    itemTypeListLabel->setText("Item Type:");
    itemTypeListLabel->setAlignment(AlignRight|AlignVCenter);

    itemTypeList = new QComboBox(false, this, "itemTypeList");
    connect(itemTypeList, SIGNAL(activated(int)), this, SLOT(itemTypeChanged(int)));

    QLabel  *accountListLabel = new QLabel(this, "accountListLabel");
    accountListLabel->setText("Account:");
    accountListLabel->setAlignment(AlignRight|AlignVCenter);

    accountList = new QComboBox(false, this, "accountList");
    connect(accountList, SIGNAL(activated(int)), this, SLOT(accountChanged(int)));

    QLabel  *setupItemListLabel = new QLabel(this, "setupItemListLabel");
    setupItemListLabel->setText("Setup Item:");
    setupItemListLabel->setAlignment(AlignRight|AlignVCenter);

    setupItemList = new QComboBox(false, this, "setupItemList");
    connect(setupItemList, SIGNAL(activated(int)), this, SLOT(setupItemChanged(int)));

    QLabel  *prioritySpinLabel = new QLabel(this, "prioritySpinLabel");
    prioritySpinLabel->setText("Priority:");
    prioritySpinLabel->setAlignment(AlignRight|AlignVCenter);

    QLabel  *prioritySpinLabel2 = new QLabel(this, "prioritySpinLabel2");
    prioritySpinLabel2->setText("(Lower number = higher priority)");
    prioritySpinLabel2->setAlignment(AlignLeft|AlignVCenter);

    prioritySpin = new QSpinBox(this, "prioritySpin");
    prioritySpin->setRange(0, 100);
    connect(prioritySpin, SIGNAL(valueChanged(int)), this, SLOT(priorityChanged(int)));

    QLabel  *mailboxesSpinLabel = new QLabel(this, "mailboxesSpinLabel");
    mailboxesSpinLabel->setText("Total Mailboxes:");
    mailboxesSpinLabel->setAlignment(AlignRight|AlignVCenter);

    mailboxesSpin = new QSpinBox(this, "mailboxesSpin");
    mailboxesSpin->setRange(0, 100);
    connect(mailboxesSpin, SIGNAL(valueChanged(int)), this, SLOT(mailboxesChanged(int)));

    isActive = new QCheckBox(this, "isActive");
    isActive->setText("Active");
    connect(isActive, SIGNAL(toggled(bool)), this, SLOT(activeChanged(bool)));

    isTaxable = new QCheckBox(this, "isTaxable");
    isTaxable->setText("Taxable");
    connect(isTaxable, SIGNAL(toggled(bool)), this, SLOT(taxableChanged(bool)));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Okay, now create our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // The bulk of the widgets will go into this grid.
    QGridLayout *gl = new QGridLayout(7, 3);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    int curRow = 0;

    gl->addWidget(itemNameLabel,        curRow, 0);
    gl->addMultiCellWidget(itemName,    curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(descriptionLabel,     curRow, 0);
    gl->addMultiCellWidget(description, curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 1);

    gl->addWidget(itemTypeListLabel,        curRow, 0);
    gl->addMultiCellWidget(itemTypeList,    curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(accountListLabel,         curRow, 0);
    gl->addMultiCellWidget(accountList,     curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(setupItemListLabel,       curRow, 0);
    gl->addMultiCellWidget(setupItemList,   curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(prioritySpinLabel,        curRow, 0);
    gl->addWidget(prioritySpin,             curRow, 1);
    gl->addWidget(prioritySpinLabel2,       curRow, 2);
    gl->setRowStretch(curRow++, 0);
    
    gl->addWidget(mailboxesSpinLabel,       curRow, 0);
    gl->addMultiCellWidget(mailboxesSpin,  curRow, curRow, 1, 2);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(isActive,                 curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(isTaxable,                curRow, 1);
    gl->setRowStretch(curRow++, 0);

    ml->addLayout(gl, 1);

    // Our row of button.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    acctIDX = NULL;
    setupIDX = NULL;

    strcpy(myItemName, "");
    strcpy(myItemDesc, "");
    myItemType      = 0;
    myAccount       = (long) 0;
    mySetupItem     = (long) 0;
    myActive        = 0;
    myTaxable       = 0;
    myPriority      = 0;
    myMailboxes     = 0;

    setItemNumber((long) 0);
    
    loadItemTypeList();
    loadAccountList();
    loadSetupItemList();
    
}


BI_General::~BI_General()
{
}

/*
** loadItemTypeList - Fills the itemTypeList with the various item types.
*/

void BI_General::loadItemTypeList()
{
    itemTypeList->clear();
    itemTypeList->insertItem("Normal");
    itemTypeList->insertItem("Subscription Item");
    itemTypeList->insertItem("Setup Item");
}

/*
** loadAccountList  - Loads the list of accounts that can be associated
**                    with this billable item.
*/

void BI_General::loadAccountList()
{
    ADB     DB;
    int     idxPtr = 0;
    
    if (acctIDX != NULL) delete acctIDX;

    DB.query("select AccountNo, AcctName from Accounts order by AcctType, AcctName");
    if (DB.rowCount) {
        acctIDX = new long[DB.rowCount+1];
        acctIDX[DB.rowCount] = -1;
        while (DB.getrow()) {
            acctIDX[idxPtr++] = atol(DB.curRow["AccountNo"]);
            accountList->insertItem(DB.curRow["AcctName"]);
        }
    }
    
}

/*
** loadSetupItemList - Loads the list of setup items that can be associated
**                     with this billable item.
*/

void BI_General::loadSetupItemList()
{
    ADB     DB;
    int     idxPtr = 0;
    
    if (setupIDX != NULL) delete setupIDX;
    setupIDX = NULL;
    
    DB.query("select ItemNumber, ItemID from Billables where ItemType >= 2 order by ItemID");
    if (DB.rowCount) {
        setupIDX = new long[DB.rowCount+2];
        setupIDX[DB.rowCount+1] = -1;
        setupIDX[idxPtr++] = (long) 0;
        setupItemList->insertItem("None");
        while (DB.getrow()) {
            setupIDX[idxPtr++] = atol(DB.curRow["ItemNumber"]);
            setupItemList->insertItem(DB.curRow["ItemID"]);
        }
    }
}

/*
** setItemNumber - Sets the item number and loads the variables from the
**                 database.
*/

void BI_General::setItemNumber(long itemNumber)
{
    myItemNumber = itemNumber;
    if (myItemNumber) {
        ADBTable    BIDB;
        BIDB.setTableName("Billables");
        if (BIDB.get(myItemNumber)) {
            itemName->setText(BIDB.getStr("ItemID"));
            itemName->setEnabled(TRUE);
            strcpy(myItemName, BIDB.getStr("ItemID"));

            description->setText(BIDB.getStr("Description"));
            description->setEnabled(TRUE);
            strcpy(myItemDesc, BIDB.getStr("Description"));

            itemTypeList->setCurrentItem(BIDB.getInt("ItemType"));
            itemTypeList->setEnabled(TRUE);
            myItemType = BIDB.getInt("ItemType");

            // Now, find the account in the account list.
            if (acctIDX != NULL) {
                int     idxPos = 0;
                while (acctIDX[idxPos] >= 0) {
                    if (acctIDX[idxPos] == BIDB.getLong("AccountNo")) {
                        accountList->setCurrentItem(idxPos);
                    }
                    idxPos++;
                }
                accountList->setEnabled(TRUE);
                myAccount = acctIDX[accountList->currentItem()];
            } else {
                myAccount = 0;
            }
            
            // Now, find the setup item in the setupItemList
            if (setupIDX != NULL) {
                int     idxPos = 0;
                while (setupIDX[idxPos] >= 0) {
                    if (setupIDX[idxPos] == BIDB.getLong("SetupItem")) {
                        setupItemList->setCurrentItem(idxPos);
                    }
                    idxPos++;
                }
                setupItemList->setEnabled(TRUE);
                mySetupItem = setupIDX[setupItemList->currentItem()];
            } else {
                mySetupItem = 0;
            }

            prioritySpin->setValue(BIDB.getInt("Priority"));
            prioritySpin->setEnabled(TRUE);
            myPriority = BIDB.getInt("Priority");

            mailboxesSpin->setValue(BIDB.getInt("NumMailboxes"));
            mailboxesSpin->setEnabled(TRUE);
            myMailboxes = BIDB.getInt("NumMailboxes");

            isActive->setChecked(BIDB.getInt("Active"));
            isActive->setEnabled(TRUE);
            myActive = BIDB.getInt("Active");

            isTaxable->setChecked(BIDB.getInt("Taxable"));
            isTaxable->setEnabled(TRUE);
            myTaxable = BIDB.getInt("Taxable");
        } else {
            myItemNumber = (long) 0;
        }
    }
    
    if (!myItemNumber) {
        itemName->setText("");
        itemName->setEnabled(FALSE);
        description->setText("");
        description->setEnabled(FALSE);
        itemTypeList->setEnabled(FALSE);
        accountList->setEnabled(FALSE);
        setupItemList->setEnabled(FALSE);
        prioritySpin->setValue(0);
        prioritySpin->setEnabled(FALSE);
        mailboxesSpin->setValue(0);
        mailboxesSpin->setEnabled(FALSE);
        isActive->setChecked(FALSE);
        isActive->setEnabled(FALSE);
        isTaxable->setChecked(FALSE);
        isTaxable->setEnabled(FALSE);
        strcpy(myItemName, "");
        strcpy(myItemDesc, "");
        myItemType  = 0;
        myAccount   = (long) 0;
        mySetupItem = (long) 0;
        myPriority  = 0;
        myMailboxes = 0;
        myActive    = 0;
        myTaxable   = 0;
        saveButton->setEnabled(true);
    }
    
    checkForSave();
}

/*
** checkForSave - Scans through the various my* variables and checks to see
**                if any values have changed.  If they have, the save button
**                is enabled.
*/

void BI_General::checkForSave()
{
    int     needSave = 0;
    
    if (!myItemNumber) {
        saveButton->setEnabled(FALSE);
        return;
    } else {
        saveButton->setEnabled(true);
        return;
    }
    
    // First, check the strings.  Do a NULL check first, then a strcmp.
    if (strcmp(myItemName, itemName->text())) needSave ++;
    if (strcmp(myItemDesc, description->text())) needSave++;
    
    // Now do the more simple number comparisons.
    if (myItemType  != itemTypeList->currentItem()) needSave++;
    if (myAccount   != acctIDX[accountList->currentItem()]) needSave++;
    if (mySetupItem != setupIDX[setupItemList->currentItem()]) needSave++;
    if (myPriority  != prioritySpin->cleanText().toInt()) needSave++;
    if (myMailboxes != mailboxesSpin->cleanText().toInt()) needSave++;
    if (myActive    != isActive->isChecked()) needSave++;
    if (myTaxable   != isTaxable->isChecked()) needSave++;
    
    saveButton->setEnabled(needSave);
}

/*
** save  - Saves the current edits to the database.
*/

void BI_General::save()
{
    if (!myItemNumber) return;
    
    ADBTable    BIDB;
    BIDB.setTableName("Billables");

    int myAcct = 0;
    int mySetup = 0;

    if (accountList->currentItem())   myAcct  = acctIDX[accountList->currentItem()];
    if (setupItemList->currentItem()) mySetup = setupIDX[setupItemList->currentItem()];
    
    BIDB.get(myItemNumber);
    BIDB.setValue("ItemID", itemName->text());
    BIDB.setValue("Description", description->text());
    BIDB.setValue("ItemType", itemTypeList->currentItem());
    BIDB.setValue("AccountNo", myAcct);
    BIDB.setValue("SetupItem", mySetup);
    BIDB.setValue("Active", (int) isActive->isChecked());
    BIDB.setValue("Taxable", (int) isTaxable->isChecked());
    BIDB.setValue("Priority", prioritySpin->cleanText().toInt());
    BIDB.setValue("NumMailboxes", mailboxesSpin->cleanText().toInt());
    BIDB.upd();
    
    setItemNumber(myItemNumber);
    emit(itemChanged());
}



