/*
** $Id$
**
***************************************************************************
**
** ChangeBillingCycle - Allows the user to change the billing cycle of
**                      a customer.
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
** $Log: ChangeBillingCycle.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "ChangeBillingCycle.h"
#include <stdio.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <qcursor.h>
#include <qapp.h>
#include <qmessagebox.h>
#include "BlargDB.h"
#include "BString.h"

#include <TAATools.h>
#include <ADB.h>

#define Inherited ChangeBillingCycleData

ChangeBillingCycle::ChangeBillingCycle
(
	QWidget* parent,
	const char* name,
	long CustID
)
	:
	Inherited( parent, name )
{
	setCaption( "Edit Customer Billing Cycle" );

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
	
	DB.query("select CycleID,InternalID from BillingCycles order by CycleID");
	while (DB.getrow()) {
	    cycleList->insertItem(DB.curRow["CycleID"]);
	    if (atol(DB.curRow["InternalID"]) == CDB.getLong("BillingCycle")) {
	        cycleList->setCurrentItem(cycleList->count()-1);
	    }
	}
	
	effectiveDate->setText(theDate);

    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
}


ChangeBillingCycle::~ChangeBillingCycle()
{
}

/*
** saveBillingCycleChange - Saves the changes...
*/

void ChangeBillingCycle::saveBillingCycleChange()
{
    CustomersDB CDB;
    ADB         DB;
    
    QApplication::setOverrideCursor(waitCursor);
    CDB.get(myCustID);

    DB.query("select InternalID from BillingCycles where CycleID = '%s'", (const char *) cycleList->text(cycleList->currentItem()));
    DB.getrow();

    CDB.setValue("BillingCycleDate", effectiveDate->text());
    CDB.setValue("BillingCycle", atol(DB.curRow["InternalID"]));
    CDB.upd();
    QApplication::restoreOverrideCursor();
    emit(customerChanged(myCustID));
    
    QMessageBox::information(this, "Rerate warning", "Note that billing cycle changes are not\nrerated automatically.  You will need to\nmake any necessary adjustments to the\ncustomers register manually.");
    
    close();
}

/*
** cancelBillingCycleChange - Closes the window...
*/

void ChangeBillingCycle::cancelBillingCycleChange()
{
    close();
}



