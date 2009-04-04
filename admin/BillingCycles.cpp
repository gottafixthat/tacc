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

#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3strlist.h>

#include <ADB.h>
#include <BlargDB.h>
#include "BillingCycles.h"
#include "BillingCycleEdit.h"

using namespace Qt;

BillingCycles::BillingCycles
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    // Create our widgets.
    list = new Q3ListView(this, "Cycle List");
    list->addColumn("Cycle ID");
    list->addColumn("Type");
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
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    // Our layout.  Very simple.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(list, 1);

    // Buttons.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
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

/**
 * closeClicked()
 *
 * Called when the user clicks the close button.
 */
void BillingCycles::closeClicked()
{
    delete this;
}


//
// refreshList  - Refreshes the list of Billing Cycles.
//

void BillingCycles::refreshList(int)
{
    ADB             DB;
    Q3ListViewItem   *curItem = NULL;
    
    // Save the state of the list.
    list->clear();

    DB.query("select CycleID, CycleType, Description from BillingCycles order by CycleID");
    if (DB.rowCount) while(DB.getrow()) {
        curItem = new Q3ListViewItem(list, DB.curRow["CycleID"], DB.curRow["CycleType"], DB.curRow["Description"]);
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
    Q3ListViewItem   *curItem = NULL;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
        strcpy(tmpCycle, curItem->key(0,0));
        if (strlen(tmpCycle)) {
            newBillingCycle = new BillingCycleEdit();
            newBillingCycle->setCycleID(tmpCycle);
            newBillingCycle->show();
            connect(newBillingCycle, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
        }
    }
}


//
// deleteCycle  - Allows the user to delete a Billing Cycle
//

void BillingCycles::deleteCycle()
{
    char            tmpstr[256];
    Q3ListViewItem   *curItem = NULL;
    
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


// vim: expandtab
