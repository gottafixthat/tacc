/*
** $Id$
**
***************************************************************************
**
** PKG_Billables - Package billables
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
** $Log: PKG_Billables.cpp,v $
** Revision 1.2  2003/12/30 01:38:28  marc
** Removed QTArch dialogs.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "PKG_Billables.h"

#include <stdio.h>
#include <stdlib.h>

#include <BlargDB.h>
#include <ADB.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlayout.h>

PKG_Billables::PKG_Billables
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Okay, create our widgets.
    billableList = new QListView(this, "billableList");
    int curCol = 0;
    billableList->addColumn("Item ID");
    billableList->setColumnAlignment(curCol++, AlignLeft);
    billableList->addColumn("Description");
    billableList->setColumnAlignment(curCol++, AlignLeft);
    billableList->addColumn("Qty");
    billableList->setColumnAlignment(curCol++, AlignRight);
    billableList->addColumn("Grp");
    billableList->setColumnAlignment(curCol++, AlignRight);
    connect(billableList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(billableSelected(QListViewItem *)));

    QLabel *itemListLabel = new QLabel(this, "itemListLabel");
    itemListLabel->setText("Item:");
    itemListLabel->setAlignment(AlignRight|AlignVCenter);

    itemList = new QComboBox(false, this, "itemList");
    connect(itemList, SIGNAL(activated(int)), this, SLOT(billableItemChanged(int)));

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(AlignRight|AlignVCenter);

    description = new QLineEdit(this, "description");
    description->setMaxLength(80);
    connect(description, SIGNAL(textChanged(const QString &)), this, SLOT(descriptionChanged(const QString &)));

    QLabel *qtySpinLabel = new QLabel(this, "qtySpinLabel");
    qtySpinLabel->setText("Quantity:");
    qtySpinLabel->setAlignment(AlignRight|AlignVCenter);

    qtySpin = new QSpinBox(this, "qtySpin");
    qtySpin->setRange(0, 999);
    connect(qtySpin, SIGNAL(valueChanged(int)), this, SLOT(quantityChanged(int)));

    QLabel *groupSpinLabel = new QLabel(this, "groupSpinLabel");
    groupSpinLabel->setText("Group:");
    groupSpinLabel->setAlignment(AlignRight|AlignVCenter);

    groupSpin = new QSpinBox(this, "groupSpin");
    groupSpin->setRange(0, 1);
    connect(groupSpin, SIGNAL(valueChanged(int)), this, SLOT(groupChanged(int)));

    removeButton = new QPushButton(this, "removeButton");
    removeButton->setText("&Remove");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeBillable()));

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addBillable()));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Setup our layout.  Pretty straightforward
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(billableList, 1);

    QGridLayout *gl = new QGridLayout(4,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 0);
    int curRow = 0;
    gl->addWidget(itemListLabel,            curRow, 0);
    gl->addWidget(itemList,                 curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(descriptionLabel,         curRow, 0);
    gl->addWidget(description,              curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(qtySpinLabel,             curRow, 0);
    gl->addWidget(qtySpin,                  curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(groupSpinLabel,           curRow, 0);
    gl->addWidget(groupSpin,                curRow, 1);
    gl->setRowStretch(curRow++, 0);

    ml->addLayout(gl, 0);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(removeButton, 0);
    bl->addWidget(addButton, 0);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // All done with the layout

    myPackageID = (long) 0;
    
    setPackageID(0);

    billItemIDX = NULL;
    intBillable = 0;
    intDescr    = NULL;
    intQty      = 0;
    intGroup    = 0;

    billableList->setAllColumnsShowFocus(TRUE);

    // Fill our setup item list.
    ADB         DB;
    int         tmpPos = 0;
    DB.query("select ItemNumber, ItemID from Billables order by ItemID");
    billItemIDX = new long[DB.rowCount+2];
    itemList->clear();
    itemList->insertItem("None Selected");
    billItemIDX[tmpPos++] = (long) 0;
    if (DB.rowCount) while (DB.getrow()) {
        billItemIDX[tmpPos++] = atol(DB.curRow["ItemNumber"]);
        itemList->insertItem(DB.curRow["ItemID"]);
    }

}


PKG_Billables::~PKG_Billables()
{
}

/*
** setPackageID - Sets the current package ID.
*/

void PKG_Billables::setPackageID(long newPackageID)
{
    myPackageID = newPackageID;
    billableList->clear();
    if (myPackageID) {
        // Load all of the billables for this package ID.
        ADB         DB;
        BillablesDB BDB;
        DB.query("select ItemNumber, Description, Quantity, LoginGroup, InternalID from PackageContents where PackageID = %ld", myPackageID);
        if (DB.rowCount) while (DB.getrow()) {
            BDB.get(atol(DB.curRow["ItemNumber"]));
            (void) new QListViewItem(billableList,
              BDB.getStr("ItemID"),
              DB.curRow["Description"],
              DB.curRow["Quantity"],
              DB.curRow["LoginGroup"],
              DB.curRow["InternalID"],
              DB.curRow["ItemNumber"]
            );
        }
        
    }
    
    if (myPackageID) addButton->setEnabled(TRUE);
    else addButton->setEnabled(FALSE);
    
    billableSelected(billableList->currentItem());
    
}

/*
** billableSelected - Sets up the edit widgets for the currently selected
**                    billable item.
*/

void PKG_Billables::billableSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        // Okay, we have a billable.  Set its values in our list.
        // First, get the billable ID.
        int finished = 0;
        int tmpPos = 0;
        long    billableItem = atol(curItem->key(5, 0));
        while (!finished) {
            if (billItemIDX[tmpPos] == billableItem) {
                itemList->setCurrentItem(tmpPos);
                finished++;
            } else {
                tmpPos++;
                if (tmpPos > itemList->count()) finished++;
            }
        }

        
        if (intDescr != NULL) delete intDescr;
        intDescr = new char[strlen(curItem->key(1,0)) + 2];
        strcpy(intDescr, curItem->key(1,0));
        intBillable = billableItem;
        intQty      = atoi(curItem->key(2,0));
        intGroup    = atoi(curItem->key(3,0));
        
        
        description->setText(intDescr);
        qtySpin->setValue(intQty);
        groupSpin->setValue(intGroup);
        
        itemList->setEnabled(TRUE);
        description->setEnabled(TRUE);
        qtySpin->setEnabled(TRUE);
        groupSpin->setEnabled(TRUE);
        removeButton->setEnabled(TRUE);
    } else {
        itemList->setCurrentItem(0);
        itemList->setEnabled(FALSE);
        description->setText("");
        description->setEnabled(FALSE);
        qtySpin->setValue(0);
        qtySpin->setEnabled(FALSE);
        groupSpin->setValue(0);
        groupSpin->setEnabled(FALSE);
        removeButton->setEnabled(FALSE);
    }
    checkForSave();
}



/*
** checkForSave - Checks to see if we need to save the current item.
*/

void PKG_Billables::checkForSave()
{
    int     allowSave = 0;
    
    if (myPackageID && billableList->currentItem() != NULL) {
	    
	    // Check to see if the currently selected billable is different than
	    // the one in memory.
	    
	    if (billItemIDX[itemList->currentItem()] != intBillable) allowSave++;
	    if (strcmp(intDescr, description->text())) allowSave++;
	    if (intQty != qtySpin->cleanText().toInt()) allowSave++;
	    if (intGroup != groupSpin->cleanText().toInt()) allowSave++;
    }
    
    saveButton->setEnabled(allowSave);
}


/*
** save - Saves the changes made in the edit widgets.
*/

void PKG_Billables::save()
{
    QListViewItem   *curItem = billableList->currentItem();
    if (myPackageID && curItem != NULL) {
        // Save the current item back into the database.
        // We have an internal ID so this will make things easier.
        ADBTable    PCDB;
        PCDB.setTableName("PackageContents");
        PCDB.get(atol(curItem->key(4,0)));
        PCDB.setValue("ItemNumber", billItemIDX[itemList->currentItem()]);
        PCDB.setValue("Description", description->text());
        PCDB.setValue("Quantity", qtySpin->cleanText().toFloat());
        PCDB.setValue("LoginGroup", groupSpin->cleanText().toInt());
        PCDB.upd();
        
        char    tmpStr[1024];
        
        sprintf(tmpStr, "%.2f", qtySpin->cleanText().toFloat());
        
        curItem->setText(0, itemList->text(itemList->currentItem()));
        curItem->setText(1, description->text());
        curItem->setText(2, tmpStr);
        curItem->setText(3, groupSpin->cleanText());
        sprintf(tmpStr, "%ld", billItemIDX[itemList->currentItem()]);
        curItem->setText(5,tmpStr);
        
        billableSelected(curItem);
    }
}


/*
** addBillable - Adds a new item into the billable list.
*/

void PKG_Billables::addBillable()
{
    // This is a very simple function.  It inserts a new, blank row into
    // PackageContents for the current Package ID and then sets it to be
    // the current entry.

    if (myPackageID) {
        QListViewItem   *curItem;
        ADBTable        PCDB;
        
        // Create and insert an empty record.
        PCDB.setTableName("PackageContents");
        PCDB.setValue("InternalID", 0);
        PCDB.setValue("PackageID", myPackageID);
        PCDB.setValue("Quantity", (float) 1.00);
        PCDB.setValue("ItemNumber", (long) 0);
        PCDB.setValue("Description", "");
        PCDB.setValue("LoginGroup", 0);
        PCDB.ins();
        
        // Okay, now insert this new one into our list.
        curItem = new QListViewItem(billableList,
          itemList->text(0),
          "",
          "1.00",
          "0",
          PCDB.getStr("InternalID"),
          "0"
        );
        
        billableList->setCurrentItem(curItem);
        billableList->setSelected(curItem, TRUE);
    }
}

/*
** removeBillable - Removes the currently selected billable from the list.
*/

void PKG_Billables::removeBillable()
{
    QListViewItem   *curItem = billableList->currentItem();
    if (myPackageID && curItem != NULL) {
        ADB     DB;
        
        switch(QMessageBox::information(this, "Remove Billable",
          "Are you sure you wish to remove\nthe currently selected billable?",
          "&No", "&Yes", "&Cancel", 0, 2)) {
          
            case 1:     // Remove it.
                QApplication::setOverrideCursor(waitCursor);
                DB.dbcmd("delete from PackageContents where InternalID = %ld", atol(curItem->key(4,0)));
                
                setPackageID(myPackageID);
                billableSelected(NULL);
                
                QApplication::restoreOverrideCursor();
            break;
            
            default:    // Don't remove it.
            break;
        }
    }
}
