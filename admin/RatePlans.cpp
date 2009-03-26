/*
** $Id$
**
***************************************************************************
**
** RatePlans - Brings up a list of rate plans.
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
** $Log: RatePlans.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "RatePlans.h"
#include "BlargDB.h"
#include "RatePlanEdit.h"
#include <stdio.h>
#include <stdlib.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <ADB.h>

RatePlans::RatePlans
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Rate Plans" );

    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newRatePlan()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editRatePlan()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteRatePlan()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    // Create the menu.
    menu = new QMenuBar(this, "Menu Bar");
    menu->insertItem("&Options", options);

    // Create the buttons for the top of the window.
    QPushButton *newButton = new QPushButton(this, "NewButton");
    newButton->setText("New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newRatePlan()));

    QPushButton *editButton = new QPushButton(this, "EditButton");
    editButton->setText("Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editRatePlan()));

    QPushButton *deleteButton = new QPushButton(this, "DeleteButton");
    deleteButton->setText("Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRatePlan()));

    QPushButton *closeButton = new QPushButton(this, "CloseButton");
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(Hide()));

    list = new QListView(this, "RatePlanList");
    list->addColumn("Name");
    list->addColumn("Description");
    list->setFocus();
    list->setAllColumnsShowFocus(true);
    list->setRootIsDecorated(false);
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(editRatePlan(QListViewItem *)));
    internalIDColumn = 2;   // Change this if changing the list columns

    // Setup the layout and add our widgets
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(menu, 0);

    // The buttons need thier own layout
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);
    bl->addStretch(1);
    
    ml->addLayout(bl, 0);

    ml->addWidget(list, 1);

    refreshList(0);
    
}


RatePlans::~RatePlans()
{
}


void RatePlans::Hide()
{
    delete this;
}


//
// refreshList  - Refreshes the list of Billing Cycles.
//

void RatePlans::refreshList(int)
{
    ADB     DB;
    
    list->clear();

    DB.query("select InternalID, PlanTag, Description from RatePlans order by PlanTag");
    while(DB.getrow()) {

        (void) new QListViewItem(list, DB.curRow["PlanTag"], DB.curRow["Description"], DB.curRow["InternalID"]);
    }

}


//
// newRatePlan  - Allows the user to create a new Billing RatePlan
//

void RatePlans::newRatePlan()
{
	RatePlanEdit *newPlan;
	newPlan = new RatePlanEdit();
    connect(newPlan, SIGNAL(ratePlanSaved(int)), this, SLOT(refreshList(int)));
	newPlan->show();
}

//
// editRatePlan  - Allows the user to edit a Billing RatePlan
//

void RatePlans::editRatePlan()
{
    QListViewItem   *curItem;
	RatePlanEdit    *editPlan;

    curItem = list->currentItem();
	if (curItem != NULL) {
		editPlan = new RatePlanEdit(0, "", atoi(curItem->text(internalIDColumn)));
        connect(editPlan, SIGNAL(ratePlanSaved(int)), this, SLOT(refreshList(int)));
		editPlan->show();
	}
}

/*
 * editRatePlan - A protected slot that gets called when the user double
 *                clicks on an item in the list.
 */
void RatePlans::editRatePlan(QListViewItem *curItem)
{
    if (curItem == NULL) return;
    RatePlanEdit    *editPlan;
    editPlan = new RatePlanEdit(0, "", atoi(curItem->text(internalIDColumn)));
    connect(editPlan, SIGNAL(ratePlanSaved(int)), this, SLOT(refreshList(int)));
    editPlan->show();
}

//
// deleteRatePlan  - Allows the user to delete a Billing RatePlan
//

void RatePlans::deleteRatePlan()
{
	ADB     DB;
    QListViewItem   *curItem;
   
    curItem = list->currentItem();
    if (curItem == NULL) return;

	char	tmpstr[1024];
	long	itemNo = atoi(curItem->text(internalIDColumn));
	int		count = 0;
		
    // First, we must ensure that the item is not in use by anything else
    // Bad Things(tm) would happen if we were to blindly delete an item
    // that was in use in a customer record.  Like we would lose money.
    
    DB.query("select CustomerID from Customers where RatePlan = %ld", itemNo);
    count += DB.rowCount;
    
    if (count) {
        sprintf(tmpstr, "Unable to delete Rate Plan.\n\nIt has been used %d times in other tables.", count);
        QMessageBox::critical(this, "Unable to Delete Rate Plan", tmpstr);
    } else {
        if (QMessageBox::warning(this, "Delete Current Rate Plan", "Are you sure you wish to delete\nthe current Rate Plan?", "&Yes", "&No", 0, 1) == 0) {
            RatePlansDB	RPDB;
            RPDB.del(itemNo);
            refreshList(1);
        }
    }
}

