/*
** $Id$
**
***************************************************************************
**
** RunSubscriptions - Runs subscriptions for all customers that need them.
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
** $Log: RunSubscriptions.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "RunSubscriptions.h"

#include <qdatetm.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include "BlargDB.h"
#include "BString.h"
#include <ADB.h>

#define Inherited RunSubscriptionsData

RunSubscriptions::RunSubscriptions
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Run Subscriptions" );
	
	ADB DB;
	
	// Set up our list.
	list->addColumn("Cust ID");
	list->addColumn("Customer Name");
	list->addColumn("Primary Login");
	list->addColumn("Logins");
	list->addColumn("Subscriptions");

    totSubscriptions = 0;
    totActiveLogins  = 0;
    totCharged       = 0.00;

	show();

    // Get the AR Account
    ARAcct  = 0;
    DB.query("select AccountNo from Accounts where AcctType = 1");
    DB.getrow();
    ARAcct  = atoi(DB.curRow["AccountNo"]);

    fillList();	

}


RunSubscriptions::~RunSubscriptions()
{
}

/*
** fillList     - Fills the list with information.
*/

void RunSubscriptions::fillList(void)
{
    char        tmpStr[1024];
    ADB         DB;
    ADB         DB2;
    CustomersDB CDB;
    QDate       today;
    char        sToday[64];
    int         totSteps = 0;
    int         curPos   = 0;
    char        tmpTotLogins[64];
    char        tmpTotSubs[64];
    
    today   = QDate::currentDate();
    QDatetomyDate(sToday, today);
    

    QApplication::setOverrideCursor(waitCursor);
    
    // Get the list of customer ID's that we'll be working with.
    DB.query("select distinct CustomerID from Subscriptions where Active <> 0 and EndsOn < '%s'", sToday);
    totSteps    = DB.rowCount;
    progress->setTotalSteps(totSteps);
    progress->show();
    if (DB.rowCount) {
	    while (DB.getrow()) {
	        progress->setProgress(++curPos);
	        CDB.get(atol(DB.curRow["CustomerID"]));
	        
	        DB2.query("select InternalID from Logins where CustomerID = %ld and Active <> 0", atol(DB.curRow["CustomerID"]));
	        sprintf(tmpTotLogins, "%ld", DB2.rowCount);
	        totActiveLogins += DB2.rowCount;
	        
	        DB2.query("select InternalID from Subscriptions where CustomerID = %ld and Active <> 0 and EndsOn < '%s'", atol(DB.curRow["CustomerID"]), sToday);
	        sprintf(tmpTotSubs, "%ld", DB2.rowCount);
	        totSubscriptions += DB2.rowCount;
	        
	        (void) new QListViewItem(list, DB.curRow["CustomerID"], CDB.getStr("FullName"), CDB.getStr("PrimaryLogin"), tmpTotLogins, tmpTotSubs);
	    }
	    
	    sprintf(tmpTotSubs, "%ld", totSubscriptions);
	    totalSubscriptions->setText(tmpTotSubs);
	    
	    progress->hide();
	    progressLabel->hide();
	    
	    // While our cursor is an hourglass, get the starting AR amount.
	    DB.query("select Balance from Accounts where AccountNo = %d", ARAcct);
	    DB.getrow();
	    startingAR  = atof(DB.curRow["Balance"]);
	    sprintf(tmpStr, "$%.2f", startingAR);
	    accountsReceivable->setText(tmpStr);
	    sprintf(tmpStr, "$0.00");
	    amountCharged->setText(tmpStr);
    } else {
	    beginButton->hide();
	    cancelButton->setText("&Done");
        QMessageBox::information(this, "Run Subscriptions", "There were no pending subcrptions found.");
    }    
    QApplication::restoreOverrideCursor();

    
}

/*
** processSelections - Scans through the list and does a subscription
**                     run for each customer ID in the list.
*/

void RunSubscriptions::processSelections()
{
    QListViewItem   *curItem;
    long            curCount = 0;
    CustomersDB     CDB;
    ADB             DB;
    char            tmpStr[1024];
    
    float           tmpAR;

    QApplication::setOverrideCursor(waitCursor);
    progress->reset();
    progress->show();
    
    beginButton->hide();
    cancelButton->setEnabled(FALSE);

    curItem = list->firstChild();
    while (curItem != NULL) {
        progress->setProgress(++curCount);
        list->setCurrentItem(curItem);
        list->ensureItemVisible(curItem);
        list->repaintItem(curItem);
        
        CDB.get(atol(curItem->key(0,1)));
        CDB.doSubscriptions();
        
        // Now, refresh our AR amounts.
	    DB.query("select Balance from Accounts where AccountNo = %d", ARAcct);
	    DB.getrow();
	    tmpAR  = atof(DB.curRow["Balance"]);
	    sprintf(tmpStr, "$%.2f", tmpAR);
	    accountsReceivable->setText(tmpStr);
	    sprintf(tmpStr, "$%.2f", tmpAR - startingAR);
	    amountCharged->setText(tmpStr);
        
        curItem = curItem->itemBelow();
    }

    progress->hide();
    
    cancelButton->setEnabled(TRUE);
    cancelButton->setText("&Done");
    
        

    QApplication::restoreOverrideCursor();
}


/*
** cancelAction - Closes the window.
*/

void RunSubscriptions::cancelAction()
{
    close();
}
