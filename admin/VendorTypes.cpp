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
#include <qkeycode.h>
#include <qmessagebox.h>
#include <ADB.h>

#define Inherited VendorTypesData

VendorTypes::VendorTypes
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption("Vendor Types");

    // Set up the menu...
    
    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newVendorType()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editVendorType()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteVendorType()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);
    
    list->setFocus();

    intIDIndex = NULL;    
    indexPtr   = 0;
}


VendorTypes::~VendorTypes()
{
}

void VendorTypes::Hide()
{
    hide();
}

// addToList   - Adds to the Vendor List.  This function is actually called
//               recursivly by the refreshList routine, and should not be
//               called directly.
//
//   Args      - IntID should be the one we're getting sub types for.
//

void VendorTypes::addToList(int IntID, int Level)
{
    ADB     DB;
    char    lvl[128] = ""; 
    char    tmpstr[128];
    
    DB.query("select * from VendorTypes where SubTypeOf = %d order by VendorType", IntID);

    while(DB.getrow()) {
        indexPtr++;
        QString tmpStr1 = DB.curRow["InternalID"];
        intIDIndex[indexPtr] = tmpStr1.toInt();
        strncpy(lvl, "                                        ", Level * 2);
        sprintf (tmpstr, "%s%-60s", lvl, DB.curRow["VendorType"]);
        list->insertItem(tmpstr);
        if (atoi(DB.curRow["HasSubTypes"])) {
            addToList(intIDIndex[indexPtr], Level + 1);
        }
    }
    
}


// refreshList    - Connects to the database and refreshes the vendor types
//                  list

void VendorTypes::refreshList(int)
{
    ADB     DB;
    char    tmpstr[128]; 
    int     tmpTop, tmpCur;
    int     rowcnt;
    
    // Save the state of the list.
    tmpTop = list->topItem();
    tmpCur = list->currentItem();
    
    list->setAutoUpdate(FALSE);
    list->clear();
    DB.query("select * from VendorTypes order by VendorType");
    if (intIDIndex != NULL) {
        delete(intIDIndex);
    }
    intIDIndex = new(int[DB.rowCount + 1]);
    indexPtr = -1;
    rowcnt = DB.rowCount;
    DB.query("select * from VendorTypes where SubTypeOf = 0 order by VendorType");
    while(DB.getrow()) {
        indexPtr++;
        QString tmpStr1 = DB.curRow["InternalID"];
        intIDIndex[indexPtr] = tmpStr1.toInt(); //atoi(DB.curRow[0]);
        sprintf (tmpstr, "%-60s", DB.curRow["VendorType"]);
        list->insertItem(tmpstr);
        if (atoi(DB.curRow["HasSubTypes"])) {
            addToList(intIDIndex[indexPtr], 1);
        }
    }
    list->setAutoUpdate(TRUE);
    list->repaint();
    
    rowcnt = DB.rowCount - 1;
    // Restore the state of the list.
    if (tmpTop > -1) {
        while(tmpTop > rowcnt) tmpTop--;
        while(tmpCur > rowcnt) tmpCur--;
        list->setCurrentItem(tmpCur);
        list->setTopItem(tmpTop);
    }

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
    int itemNo = -1;
    VendorTypeEdit * newVendType;
    itemNo = list->currentItem();
    if (itemNo > -1) {
        newVendType = new VendorTypeEdit(0, "", intIDIndex[itemNo]);
        newVendType->show();
        connect(newVendType, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
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
}

//
// IntIDfromList  - Gets a VendorType InternalID from a list entry.
//

int VendorTypes::IntIDfromList(int ListNum)
{
    int Ret = 0;

    QString tmpStr1;
    QString tmpStr2;    
    
    if (ListNum > -1) {
        tmpStr1 = list->currentItem();
    }
    return (Ret);
}
