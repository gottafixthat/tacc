/*
** $Id$
**
***************************************************************************
**
** Vendors - Vendor List
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
** $Log: Vendors.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "Vendors.h"
#include "BlargDB.h"
#include "VendorEdit.h"
#include <qprogbar.h>
#include <qkeycode.h>
#include <qstring.h>
#include <qapp.h>
#include <qpainter.h>
#include <qlistview.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qlayout.h>
#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>
#include <TAAWidget.h>

Vendors::Vendors
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    // Create our widgets and our layout.
    menu = new QMenuBar(this);

    QPushButton *newButton  = new QPushButton(this, "New Button");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newVendor()));

    QPushButton *editButton = new QPushButton(this, "Edit Button");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editVendor()));

    QPushButton *deleteButton = new QPushButton(this, "Delete Button");
    deleteButton->setText("&Delete");
    deleteButton->setEnabled(false);

    list = new QListView(this, "Vendor List");
    list->addColumn("Name");
    list->addColumn("Vendor ID");
    list->addColumn("Balance");
    list->setColumnAlignment(2, AlignRight);
    list->setAllColumnsShowFocus(true);

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 0, 3);
    ml->addWidget(menu, 0);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);

    ml->addLayout(bl, 0);
    ml->addWidget(list, 1);
    
    // Set up the menu...
    QPopupMenu * options = new QPopupMenu();
    options->insertItem("New", this, SLOT(newVendor()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editVendor()), CTRL+Key_E);
    int delitem = options->insertItem("Delete", this, SLOT(deleteVendor()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(close()), CTRL+Key_C);
    options->setItemEnabled(delitem, false);


    
    menu->insertItem("&Options", options);
    
    list->setFocus();
    
	setCaption( "Vendor List" );
}


Vendors::~Vendors()
{
}



//
// refreshList  - Refreshes the list of Customers.
//

void Vendors::refreshList(int)
{
    ADB     DB;
    int     rowcnt;
    QString tmpsub;
    tmpsub = "";

    list->clear();

    
    QApplication::setOverrideCursor(waitCursor);

    DB.query("select InternalID, VendorID, CompanyName, Balance from Vendors order by CompanyName");
    rowcnt = DB.rowCount;
    
    while(DB.getrow()) {
        (void) new QListViewItem(list,
                                 DB.curRow["CompanyName"],
                                 DB.curRow["VendorID"],
                                 DB.curRow["Balance"],
                                 DB.curRow["InternalID"]
                                 );
    }
    

    QApplication::restoreOverrideCursor();

}

/**
 ** newVendor - The slot for adding a new Vendor.  Brings up the
 **               New Vendor Dialog box and exits.
 **/ 

void Vendors::newVendor()
{
    VendorEdit *vendEdit;
    vendEdit = new VendorEdit(0, "", 0);
    vendEdit->show();
    connect(vendEdit, SIGNAL(refresh(int)), SLOT(refreshList(int)));
}

/**
 ** editVendor - The slot for adding a new Vendor.  Brings up the
 **               New Vendor Dialog box and exits.
 **/ 

void Vendors::editVendor()
{
    QListViewItem   *curItem = list->currentItem();
	if (curItem) {
	    VendorEdit *vendEdit;
    	vendEdit = new VendorEdit(0, "", atoi(curItem->key(3,0)));
	    vendEdit->show();
    	connect(vendEdit, SIGNAL(refresh(int)), SLOT(refreshList(int)));
    }
}


