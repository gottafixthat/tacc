/*
** $Id: VendorTypeEdit.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** VendorTypeEdit - Edits the vendor type.
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
** $Log: VendorTypeEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "VendorTypeEdit.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>

#define Inherited VendorTypeEditData

VendorTypeEdit::VendorTypeEdit
(
	QWidget* parent,
	const char* name,
	int InternalID
)
	:
	Inherited( parent, name )
{
    ADB     DB;
    char    tmpstr[128];
    int     indexPtr = 0;
    int     tmpSubOf = 0;
    
    IntID  = InternalID;
    
    if (InternalID) {
    	setCaption("Edit Vendor Type");
    } else {
        setCaption("New Vendor Type");
    }

    // Clear our mappings to internal ID's
    intIDIndex = NULL;
    
    // We're editing one, so get the data for it.
    if (IntID) {
        VendorTypesDB VTDB;
        VTDB.get(IntID);
        vendTypeEnt->setText(VTDB.VendorType);
        tmpSubOf = VTDB.SubTypeOf.toInt();
    }

    // Fill our combo box...
    DB.query("select * from VendorTypes where InternalID <> %d and SubTypeOf = 0 order by VendorType", IntID);
    intIDIndex = new(int[DB.rowCount+1]);
    intIDIndex[0] = 0;
    subTypeList->insertItem("None");
    while (DB.getrow()) {
        indexPtr++;
        QString tmpStr1 = DB.curRow["InternalID"];
        intIDIndex[indexPtr] = tmpStr1.toInt();
        sprintf(tmpstr, "%s", DB.curRow["VendorType"]);
        subTypeList->insertItem(tmpstr);
        if (intIDIndex[indexPtr] == tmpSubOf) {
            subTypeList->setCurrentItem(indexPtr);
        }
    }
    
    vendTypeEnt->setFocus();
    
}


VendorTypeEdit::~VendorTypeEdit()
{
    if (intIDIndex != NULL) {
        delete(intIDIndex);
    }
}

// The user clicked on the close button...

void VendorTypeEdit::cancelVendType()
{
    close();
}

// The user clicked on the save button...

void VendorTypeEdit::saveVendType()
{
    char    vendtype[256];
    int     tmpID;

    // Get the data from the dialog    
    strcpy(vendtype, vendTypeEnt->text());
    tmpID = intIDIndex[subTypeList->currentItem()];
    
    VendorTypesDB VTDB;
    
    if (!IntID) {
        // New record...
        VTDB.VendorType = vendTypeEnt->text();
        VTDB.SubTypeOf.setNum(tmpID);
        VTDB.ins();
    } else {
        // Updating a record...
        VTDB.get(IntID);
        VTDB.VendorType = vendTypeEnt->text();
        VTDB.SubTypeOf.setNum(tmpID);
        VTDB.upd();
    }
    
    emit refresh(1);
    close();
}

// A public slot for saving...

void VendorTypeEdit::refreshVendList(int)
{
}

