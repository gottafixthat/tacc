/**
 * RunSubscriptions.cpp - Class definition for the RunSubscriptions
 * widget.  RunSubscriptions brings up a window that gives a manager
 * the ability to run subscriptions for all customers that have
 * subscriptions pending.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdio.h>
#include <stdlib.h>

#include <qdatetime.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qmessagebox.h>

#include <BlargDB.h>
#include <ADB.h>
#include <Cfg.h>
#include "BString.h"
#include <TAATools.h>

#include "RunSubscriptions.h"


#define Inherited RunSubscriptionsData

RunSubscriptions::RunSubscriptions(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
	setCaption( "Run Subscriptions" );

    // Create our widgets
    subscrList = new QListView(this, "subscrList");
    subscrList->addColumn("Cust ID");
    subscrList->addColumn("Customer Name");
    subscrList->addColumn("Primary Login");
    subscrList->addColumn("Services");
    subscrList->addColumn("Subs");
    subscrList->setAllColumnsShowFocus(true);

    // A group box to hold our summary information.
    // We'll use the group box's automatic layout, so we need to 
    // create the widgets in order.
    QGroupBox   *summaryBox = new QGroupBox(2, Horizontal, "Summary Information", this, "summaryBox");

    QLabel  *totalSubscriptionsLabel = new QLabel(summaryBox, "totalSubscriptionsLabel");
    totalSubscriptionsLabel->setText("Total Subscriptions:");
    totalSubscriptionsLabel->setAlignment(AlignRight|AlignVCenter);

    totalSubscriptions = new QLabel(summaryBox, "totalSubscriptions");
    totalSubscriptionsLabel->setAlignment(AlignRight|AlignVCenter);

    QLabel *accountsReceivableLabel = new QLabel(summaryBox, "accountsReceivableLabel");
    accountsReceivableLabel->setText("Accounts Receivable:");
    accountsReceivableLabel->setAlignment(AlignRight|AlignVCenter);

    accountsReceivable = new QLabel(summaryBox, "accountsReceivable");
    accountsReceivableLabel->setAlignment(AlignRight|AlignVCenter);

    QLabel *amountChargedLabel = new QLabel(summaryBox, "amountChargedLabel");
    amountChargedLabel->setText("Amount Charged:");
    amountChargedLabel->setAlignment(AlignRight|AlignVCenter);

    amountCharged = new QLabel(summaryBox, "amountCharged");
    amountChargedLabel->setAlignment(AlignRight|AlignVCenter);

    // And now our buttons.
    beginButton = new QPushButton(this, "beginButton");
    beginButton->setText("&Begin");
    connect(beginButton, SIGNAL(clicked()), this, SLOT(processSelections()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));

    // Create our layout now.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);
    ml->addWidget(subscrList, 1);

    QBoxLayout *secLayout = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    secLayout->addWidget(summaryBox, 1);
    secLayout->addStretch(1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::QBoxLayout::TopToBottom, 3);
    bl->addStretch(1);
    bl->addWidget(beginButton, 0);
    bl->addWidget(cancelButton, 0);

    secLayout->addLayout(bl, 0);
    ml->addLayout(secLayout, 0);

    totSubscriptions = 0;
    totActiveLogins  = 0;
    totCharged = 0.00;
	
    // Get the AR Account
    ARAcct  = atoi(cfgVal("AcctsRecvAccount"));

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
    emit(setStatus("Searching for subscriptions..."));
    
    // Get the list of customer ID's that we'll be working with.
    DB.query("select distinct CustomerID from Subscriptions where Active <> 0 and EndsOn < '%s'", sToday);
    totSteps    = DB.rowCount;
    if (DB.rowCount) {
        emit(setStatus("Analyzing subscriptions..."));
	    while (DB.getrow()) {
            emit(setProgressRT(++curPos, totSteps));
	        CDB.get(atol(DB.curRow["CustomerID"]));
	        
	        DB2.query("select InternalID from Logins where CustomerID = %ld and Active <> 0", atol(DB.curRow["CustomerID"]));
	        sprintf(tmpTotLogins, "%ld", DB2.rowCount);
	        totActiveLogins += DB2.rowCount;
	        
	        DB2.query("select InternalID from Subscriptions where CustomerID = %ld and Active <> 0 and EndsOn < '%s'", atol(DB.curRow["CustomerID"]), sToday);
	        sprintf(tmpTotSubs, "%ld", DB2.rowCount);
	        totSubscriptions += DB2.rowCount;
	        
	        (void) new QListViewItem(subscrList, DB.curRow["CustomerID"], CDB.getStr("FullName"), CDB.getStr("PrimaryLogin"), tmpTotLogins, tmpTotSubs);
	    }
	    
	    sprintf(tmpTotSubs, "%d", totSubscriptions);
	    totalSubscriptions->setText(tmpTotSubs);
	    
	    // While our cursor is an hourglass, get the starting AR amount.
	    DB.query("select Balance from Accounts where IntAccountNo = %d", ARAcct);
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

    // Resize the window before we show it.
    updateGeometry();
    QSize   mainSize = sizeHint();
    QSize   listSize = subscrList->sizeHint();
    mainSize.setWidth(listSize.width()+25);
    resize(mainSize);
    show();
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
}

/*
** processSelections - Scans through the list and does a subscription
**                     run for each customer ID in the list.
*/

void RunSubscriptions::processSelections()
{
    QListViewItem   *curItem;
    int             curCount = 0;
    CustomersDB     CDB;
    ADB             DB;
    char            tmpStr[1024];
    
    float           tmpAR;

    QApplication::setOverrideCursor(waitCursor);
    emit(setStatus("Processing subscriptions..."));
    
    beginButton->hide();
    cancelButton->setEnabled(FALSE);

    curItem = subscrList->firstChild();
    emit(setProgressRT(0,1));
    emit(setProgressRT(1,1));
    while (curItem != NULL) {
        emit(setProgressRT(curCount, totSubscriptions));
        subscrList->setCurrentItem(curItem);
        subscrList->ensureItemVisible(curItem);
        subscrList->repaintItem(curItem);
        
        CDB.get(atol(curItem->key(0,1)));
        CDB.doSubscriptions();
        
        // How many for this customer?
        curCount += atoi(curItem->key(4,0));
        emit(setProgressRT(curCount, totSubscriptions));

        // Now, refresh our AR amounts.
	    DB.query("select Balance from Accounts where IntAccountNo = %d", ARAcct);
	    DB.getrow();
	    tmpAR  = atof(DB.curRow["Balance"]);
	    sprintf(tmpStr, "$%.2f", tmpAR);
	    accountsReceivable->setText(tmpStr);
	    sprintf(tmpStr, "$%.2f", tmpAR - startingAR);
	    amountCharged->setText(tmpStr);
        
        curItem = curItem->itemBelow();
    }

    emit(setStatus(""));
    
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
