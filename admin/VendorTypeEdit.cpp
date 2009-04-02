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

#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <BlargDB.h>
#include <ADB.h>

#include <VendorTypeEdit.h>

using namespace Qt;

VendorTypeEdit::VendorTypeEdit(QWidget* parent, const char* name, int InternalID) :
	TAAWidget( parent, name )
{
    // Create our layout
    setCaption("New Vendor Type");

    QLabel  *vendorTypeLabel = new QLabel(this, "vendorTypeLabel");
    vendorTypeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    vendorTypeLabel->setText("Vendor Type:");

    vendorType = new QLineEdit(this, "vendorType");
    vendorType->setMaxLength(60);

    QLabel *subTypeListLabel = new QLabel(this, "subTypeListLabel");
    subTypeListLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    subTypeListLabel->setText("Sub type of:");

    subTypeList = new QComboBox(false, this, "subTypeList");

    // Our buttons
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveVendType()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelVendType()));

    // Create our layout now.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);

    // A grid for our main widgets.
    Q3GridLayout *gl = new Q3GridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(vendorTypeLabel,      curRow, 0);
    gl->addWidget(vendorType,           curRow, 1);
    gl->setRowStretch(curRow, 0);
    
    curRow++;
    gl->addWidget(subTypeListLabel,     curRow, 0);
    gl->addWidget(subTypeList,          curRow, 1);
    gl->setRowStretch(curRow, 0);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 1);

    // A button layout
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addStretch(1);
    ml->addLayout(bl, 0);


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
        vendorType->setText(VTDB.VendorType);
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
    
    vendorType->setFocus();
    
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
    strcpy(vendtype, vendorType->text());
    tmpID = intIDIndex[subTypeList->currentItem()];
    
    VendorTypesDB VTDB;
    
    if (!IntID) {
        // New record...
        VTDB.VendorType = vendorType->text();
        VTDB.SubTypeOf.setNum(tmpID);
        VTDB.ins();
    } else {
        // Updating a record...
        VTDB.get(IntID);
        VTDB.VendorType = vendorType->text();
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


// vim: expandtab
