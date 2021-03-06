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

#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>
#include <BlargDB.h>

#include "LoginTypeBillablesAdd.h"
#include "ELT_Billables.h"

using namespace Qt;

ELT_Billables::ELT_Billables
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    // Create our widgets
    QLabel *billableListLabel  = new QLabel(this, "billableListLabel");
    billableListLabel->setText("Billables:");
    billableListLabel->setAlignment(AlignRight|AlignTop);

    billableList = new Q3ListView(this, "billableList");
    billableList->addColumn("Name");
    billableList->addColumn("Description");
    billableList->setAllColumnsShowFocus(TRUE);
    connect(billableList, SIGNAL(selectionChanged(Q3ListViewItem *)), this, SLOT(itemSelected(Q3ListViewItem *)));

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addBillable()));

    removeButton = new QPushButton(this, "removeButton");
    removeButton->setText("&Remove");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeBillable()));

    // Create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3GridLayout *gl = new Q3GridLayout(1, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setRowStretch(0, 1);
    gl->addWidget(billableListLabel,        0, 0);
    gl->addWidget(billableList,             0, 1);

    ml->addLayout(gl, 1);

    // Our buttons.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(removeButton, 0);

    ml->addLayout(bl, 0);

    // We start out by disabling all of our buttons and fields.
    // By setting the Login Type ID to zero.
    setLoginTypeID((long) 0);
}


ELT_Billables::~ELT_Billables()
{
}


/*
** setLoginTypeID - Sets the current login type ID.
**                  Loads the information from the database.
*/

void ELT_Billables::setLoginTypeID(long newLoginTypeID)
{
    myLoginTypeID = newLoginTypeID;
    billableList->clear();
    if (newLoginTypeID) {
        ADB         DB;
        BillablesDB BDB;
        DB.query("select InternalID, ItemNumber from LoginTypeBillables where LoginTypeID = %ld", newLoginTypeID);
        if (DB.rowCount) {
            // Enable/disable buttons and stuff here...
            while (DB.getrow()) { 
                BDB.get(atol(DB.curRow["ItemNumber"]));
                (void) new Q3ListViewItem(billableList, BDB.getStr("ItemID"), BDB.getStr("Description"), DB.curRow["InternalID"]);
            }
        } 
    }
    
    itemSelected(billableList->currentItem());
}

/*
** itemSelected - Gets called whenever the user hilights an item from the
**                list.
*/

void ELT_Billables::itemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        removeButton->setEnabled(TRUE);
    } else {
        removeButton->setEnabled(FALSE);
    }
    
    if (myLoginTypeID) addButton->setEnabled(TRUE);
    else addButton->setEnabled(FALSE);
}



/*
** removeBillable - Prompts the user to verify that they wish to remove
**                  an associated billable.  If they do, it removes it.
*/

void ELT_Billables::removeBillable()
{
    Q3ListViewItem   *curItem = billableList->currentItem();
    if (curItem == NULL) return;
    
    char    *tmpStr = new char[4096];
    ADB     DB;
    
    sprintf(tmpStr, "Are you sure you wish to\nremove the '%s' billable?", (const char *) curItem->key(0,0));
    if (QMessageBox::information(this, "Remove Billable", tmpStr, "&No", "&Yes", 0, 1)) {
        DB.dbcmd("delete from LoginTypeBillables where InternalID = %ld", atol(curItem->key(2,0)));
        setLoginTypeID(myLoginTypeID);
    }
    
}


/*
** addBillable - Brings up a window to select a new billable item to
**               associate with this login type.
*/

void ELT_Billables::addBillable()
{
    if (!myLoginTypeID) return;
    
    LoginTypeBillablesAdd   *ltba;
    
    ltba = new LoginTypeBillablesAdd(0, "", myLoginTypeID);
    ltba->exec();
    if (ltba->result()) setLoginTypeID(myLoginTypeID);
    delete ltba;
}



// vim: expandtab
