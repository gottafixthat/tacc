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
#include <ADB.h>

#define Inherited RatePlansData

RatePlans::RatePlans
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Rate Plans" );

    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newRatePlan()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editRatePlan()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteRatePlan()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);

    list->setFocus();
    
	planIndex = NULL;
}


RatePlans::~RatePlans()
{
}


void RatePlans::Hide()
{
    hide();
}


//
// refreshList  - Refreshes the list of Billing Cycles.
//

void RatePlans::refreshList(int)
{
    ADB     DB;
    char    tmpstr[128]; 
    char    tmpstr2[128];
    int     tmpTop, tmpCur;
    int     rowcnt;
    
    // Save the state of the list.
    tmpTop = list->topItem();
    tmpCur = list->currentItem();
    
    list->setAutoUpdate(FALSE);
    list->clear();

	// If we've got an index already, clear it.
	if (planIndex != NULL) {
		delete(planIndex);
	}

    DB.query("select InternalID, PlanTag, Description from RatePlans order by PlanTag");
    rowcnt = DB.rowCount;
    planIndex = new(int[DB.rowCount + 1]);
    int indexPtr = 0;
    while(DB.getrow()) {
    	planIndex[indexPtr++] = atoi(DB.curRow["InternalID"]);
        QString tmpStr1 = DB.curRow["Description"];
        tmpStr1.truncate(53);
        strcpy(tmpstr, tmpStr1);
        sprintf (tmpstr2, "%-16s     %-53s", DB.curRow["PlanTag"], tmpstr);
        list->insertItem(tmpstr2);
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
// newRatePlan  - Allows the user to create a new Billing RatePlan
//

void RatePlans::newRatePlan()
{
	RatePlanEdit *newPlan;
	newPlan = new RatePlanEdit();
	newPlan->show();
}

//
// editRatePlan  - Allows the user to edit a Billing RatePlan
//

void RatePlans::editRatePlan()
{
	RatePlanEdit *editPlan;
	if (list->currentItem() >= 0) {
		editPlan = new RatePlanEdit(0, "", planIndex[list->currentItem()]);
		editPlan->show();
	}
}

//
// editRatePlanL  - Allows the user to edit a Billing RatePlan from the list
//

void RatePlans::editRatePlanL(int msg)
{
    msg = 0;
    editRatePlan();
}

//
// deleteRatePlan  - Allows the user to delete a Billing RatePlan
//

void RatePlans::deleteRatePlan()
{
	ADB     DB;
	char	tmpstr[1024];
	int		itemNo = list->currentItem();
	int		count = 0;
		
	if (itemNo > -1) {
		// First, we must ensure that the item is not in use by anything else
		// Bad Things(tm) would happen if we were to blindly delete an item
		// that was in use in a customer record.  Like we would lose money.
		
		DB.query("select CustomerID from Customers where RatePlan = %d", planIndex[itemNo]);
		count += DB.rowCount;
		
		if (count) {
			sprintf(tmpstr, "Unable to delete Rate Plan.\n\nIt has been used %d times in other tables.", count);
            QMessageBox::critical(this, "Unable to Delete Rate Plan", tmpstr);
		} else {
            if (QMessageBox::warning(this, "Delete Current Rate Plan", "Are you sure you wish to delete\nthe current Rate Plan?", "&Yes", "&No", 0, 1) == 0) {
				RatePlansDB	RPDB;
				RPDB.del(planIndex[itemNo]);
				refreshList(1);
			}
		}
	}
}

