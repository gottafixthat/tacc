/*
** $Id$
**
***************************************************************************
**
** VendorTypes - The list of vendor types.
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
** $Log: VendorTypes.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "VendorTypes.h"
#include "VendorTypeEdit.h"
#include "BlargDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <qnamespace.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <Q3PopupMenu>
#include <ADB.h>


VendorTypes::VendorTypes
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("Vendor Types");

    // Create our widgets
    list = new Q3ListView(this, "vendorTypesList");
    list->addColumn("Vendor Type");
    
    menu = new QMenuBar(this, "menuBar");

    // Create some buttons.
    QPushButton *newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newVendorType()));

    QPushButton *editButton = new QPushButton(this, "editButton");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editVendorType()));

    QPushButton *deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteVendorType()));


    // Create our layout.
    // Main layout is a box from top to bottom.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // Menu goes first.
    ml->addWidget(menu, 0);

    // Button bar
    Q3BoxLayout *bb = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bb->addWidget(newButton, 0);
    bb->addWidget(editButton, 0);
    bb->addWidget(deleteButton, 0);
    bb->addStretch(1);
    ml->addLayout(bb, 0);

    // The  list gets the rest of our space
    ml->addWidget(list, true);


    // Set up the menu...
    
    Q3PopupMenu * options = new Q3PopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newVendorType()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editVendorType()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteVendorType()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);
    
    list->setFocus();

    refreshList(0);
}


VendorTypes::~VendorTypes()
{
}

void VendorTypes::Hide()
{
    delete this;
}

/* addToList - Adds vendor types to the list.  This function is called recursively
 *             by the refreshList routine and should not be called directly.
 *
 * @param   parentID - The parent ID to run the query on
 * @param   parent   - The QListViewItem that we should add to.
 */
void VendorTypes::addToList(long parentID, Q3ListViewItem *parent)
{
    ADB     DB;

    DB.query("select * from VendorTypes where SubTypeOf = %ld order by VendorType", parentID);
    if (DB.rowCount) while(DB.getrow()) {
        Q3ListViewItem *curItem = new Q3ListViewItem(parent, DB.curRow["VendorType"], DB.curRow["InternalID"]);
        if (atoi(DB.curRow["HasSubTypes"])) addToList(atoi(DB.curRow["InternalID"]), curItem);
    }
}

// refreshList    - Connects to the database and refreshes the vendor types
//                  list

void VendorTypes::refreshList(int)
{
    ADB     DB;
    char    tmpstr[128]; 
    int     rowcnt;

    list->clear();
    list->setRootIsDecorated(false);
    DB.query("select * from VendorTypes where SubTypeOf = 0 order by VendorType");
    if (DB.rowCount) while(DB.getrow()) {
        Q3ListViewItem *curItem = new Q3ListViewItem(list, DB.curRow["VendorType"], DB.curRow["InternalID"]);
        if (atoi(DB.curRow["HasSubTypes"])) {
            list->setRootIsDecorated(true);
            addToList(atoi(DB.curRow["InternalID"]), curItem);
        }
    }

    /*
    
    // Save the state of the list.
    //tmpTop = list->topItem();
    //tmpCur = list->currentItem();
    
    //list->setAutoUpdate(FALSE);
    //list->clear();
    DB.query("select * from VendorTypes order by VendorType");
    //intIDIndex = new(int[DB.rowCount + 1]);
    //indexPtr = -1;
    rowcnt = DB.rowCount;
    DB.query("select * from VendorTypes where SubTypeOf = 0 order by VendorType");
    while(DB.getrow()) {
        //indexPtr++;
        QString tmpStr1 = DB.curRow["InternalID"];
        //intIDIndex[indexPtr] = tmpStr1.toInt(); //atoi(DB.curRow[0]);
        sprintf (tmpstr, "%-60s", DB.curRow["VendorType"]);
        //list->insertItem(tmpstr);
        if (atoi(DB.curRow["HasSubTypes"])) {
            //addToList(intIDIndex[indexPtr], 1);
        }
    }
    //list->setAutoUpdate(TRUE);
    //list->repaint();
    
    rowcnt = DB.rowCount - 1;
    */

}

//
// newVendorType  - Allows the user to create a new VendorType
//

void VendorTypes::newVendorType()
{
    VendorTypeEdit * newVendType;
    newVendType = new VendorTypeEdit();
    newVendType->show();
    connect(newVendType, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    
}

//
// editVendorType  - Allows the user to edit a VendorType
//

void VendorTypes::editVendorType()
{
    /*
    int itemNo = -1;
    VendorTypeEdit * newVendType;
    itemNo = list->currentItem();
    if (itemNo > -1) {
        newVendType = new VendorTypeEdit(0, "", intIDIndex[itemNo]);
        newVendType->show();
        connect(newVendType, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
    */
}

//
// editVendorTypeL  - Allows the user to edit a new VendorType from the list
//

void VendorTypes::editVendorTypeL(int msg)
{
    msg = 0;
    editVendorType();
}

//
// deleteVendorType  - Allows the user to delete a new VendorType
//

void VendorTypes::deleteVendorType()
{
    /*
    int    CurItem;
    char   tmpstr[256];
    ADB    DB;
    int    subof = 0;
    int    hasSubs = 0;
    
    CurItem = list->currentItem();
    if (CurItem > -1) {
        DB.query("select VendorType,SubTypeOf,HasSubTypes from VendorTypes where InternalID = %d", intIDIndex[CurItem]);
        DB.getrow();
        subof   = atoi(DB.curRow["SubTypeOf"]);
        hasSubs = atoi(DB.curRow["HasSubTypes"]);
        sprintf(tmpstr, "Are you sure you wish to delete the vendor\ntype '%s'?", DB.curRow["VendorType"]);
        if (QMessageBox::warning(this, "Delete Vendor Type", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            VendorTypesDB VTDB;
            if (VTDB.del(intIDIndex[CurItem])) {
                QMessageBox::critical(this, "Delete Vendor Type", "Unable to delete Vendor Type.\nIt has been assigned to one or more Vendors.");
            } else {
                refreshList(1);
            }
        };
    }
    */
}

