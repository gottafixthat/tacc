/*
** $Id$
**
***************************************************************************
**
** BillingCycles - Billing Cycle list.
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
** $Log: BillingCycles.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "BlargDB.h"
#include "BillingCycles.h"
#include "BillingCycleEdit.h"

#include <qkeycode.h>
#include <qstrlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <stdio.h>
#include <ADB.h>
#include <qmessagebox.h>


BillingCycles::BillingCycles
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Create our widgets.
    list = new QListView(this, "Cycle List");
    list->addColumn("Cycle ID");
    list->addColumn("Description");
    list->setAllColumnsShowFocus(true);

    // Create our buttons.
    QPushButton *newButton = new QPushButton(this, "New");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newCycle()));

    QPushButton *editButton = new QPushButton(this, "Edit");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editCycle()));

    QPushButton *deleteButton = new QPushButton(this, "Delete");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCycle()));

    QPushButton *closeButton = new QPushButton(this, "Close");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    // Our layout.  Very simple.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(list, 1);

    // Buttons.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);


    // Create our layout.


	setCaption( "Billing Cycles" );

}


BillingCycles::~BillingCycles()
{
}


void BillingCycles::Hide()
{
    hide();
}


//
// refreshList  - Refreshes the list of Billing Cycles.
//

void BillingCycles::refreshList(int)
{
    ADB             DB;
    QListViewItem   *curItem = NULL;
    
    // Save the state of the list.
    list->clear();

    DB.query("select CycleID, Description from BillingCycles order by CycleID");
    if (DB.rowCount) while(DB.getrow()) {
        curItem = new QListViewItem(list, DB.curRow["CycleID"], DB.curRow["Description"]);
    }
    list->repaint();
}


//
// newCycle  - Allows the user to create a new Billing Cycle
//

void BillingCycles::newCycle()
{
    BillingCycleEdit * newBillingCycle;
    newBillingCycle = new BillingCycleEdit();
    newBillingCycle->show();
    connect(newBillingCycle, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
}

//
// editCycle  - Allows the user to edit a Billing Cycle
//

void BillingCycles::editCycle()
{
    char tmpCycle[128];
    BillingCycleEdit * newBillingCycle;
    QListViewItem   *curItem = NULL;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
        strcpy(tmpCycle, curItem->key(0,0));
        if (strlen(tmpCycle)) {
            newBillingCycle = new BillingCycleEdit(0,"",tmpCycle);
            newBillingCycle->show();
            connect(newBillingCycle, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
        }
    }
}

//
// editCycleL  - Allows the user to edit a Billing Cycle from the list
//

void BillingCycles::editCycleL(int msg)
{
    msg = 0;
    editCycle();
}

//
// deleteCycle  - Allows the user to delete a Billing Cycle
//

void BillingCycles::deleteCycle()
{
    char            tmpstr[256];
    QListViewItem   *curItem = NULL;
    
    char tmpCycle[128];
    curItem = list->currentItem();
    if (curItem != NULL) {
	    strcpy(tmpCycle, curItem->key(0,0));

	    if (strlen(tmpCycle)) {
	        sprintf(tmpstr, "Are you sure you wish to delete the Billing\nCycle '%s'?", tmpCycle);
            if (QMessageBox::warning(this, "Delete Billing Cycle", tmpstr, "&Yes", "&No", 0, 1) == 0) {
	            BillingCyclesDB BCDB;
	            if (BCDB.getByCycleID(tmpCycle)) {
		            if (BCDB.del(BCDB.InternalID)) {
                        QMessageBox::critical(this, "Delete Billing Cycle", "Unable to delete Billing Cycle.\nIt has been assigned to one or more Customers.");
	            	} else {
	                	refreshList(1);
		            }
		        } else {
                    QMessageBox::critical(this, "Delete Billing Cycle", "Unable to delete Billing Cycle.\nIt was not found in the database.");
		        }
	        }
	    }
    }
}

