/*
** $Id$
**
***************************************************************************
**
** FileName     - ChangeRatePlan.cpp
**
**                Allows the user to change a customers rate plan.
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
** $Log: ChangeRatePlan.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <qapp.h>
#include <qcursor.h>
#include "ChangeRatePlan.h"
#include "BlargDB.h"
#include "BString.h"
#include <TAATools.h>

#define Inherited ChangeRatePlanData

ChangeRatePlan::ChangeRatePlan
(
	QWidget* parent,
	const char* name,
	long CustID
)
	:
	Inherited( parent, name )
{
	setCaption( "Edit Customer Rate Plan" );
	
	if (!CustID) return;
	
	myCustID    = CustID;
	
	// Load the list of rate plans.
	ADB         DB;
	CustomersDB CDB;
	QDate       tmpDate;
	char        theDate[16];
	char        tmpStr[1024];
	
	tmpDate = QDate::currentDate();
	
	QDatetomyDate(theDate, tmpDate);
	
	CDB.get(CustID);
	
	sprintf(tmpStr, "%s (%ld)", (const char *) CDB.getStr("FullName"), myCustID);
	customerLabel->setText(tmpStr);
	
	DB.query("select PlanTag,InternalID from RatePlans order by PlanTag");
	while (DB.getrow()) {
	    ratePlanList->insertItem(DB.curRow["PlanTag"]);
	    if (atol(DB.curRow["InternalID"]) == CDB.getLong("RatePlan")) {
	        ratePlanList->setCurrentItem(ratePlanList->count()-1);
	    }
	}
	
	effectiveDate->setText(theDate);
	
    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
}


ChangeRatePlan::~ChangeRatePlan()
{
}

/*
** saveRatePlanChange - Saves the changes...
*/

void ChangeRatePlan::saveRatePlanChange()
{
    CustomersDB CDB;
    ADB         DB;
    
    QApplication::setOverrideCursor(waitCursor);
    CDB.get(myCustID);

    DB.query("select InternalID from RatePlans where PlanTag = '%s'", (const char *) ratePlanList->text(ratePlanList->currentItem()));
    DB.getrow();

    CDB.setValue("RatePlanDate", effectiveDate->text());
    CDB.setValue("RatePlan", atol(DB.curRow["InternalID"]));
    CDB.upd();
    QApplication::restoreOverrideCursor();
    emit(customerChanged(myCustID));
    close();
}

/*
** cancelRatePlanChange - Closes the window...
*/

void ChangeRatePlan::cancelRatePlanChange()
{
    close();
}



