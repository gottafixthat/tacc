/*
** $Id: LogCall.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** LogCall - An interface for the user to log a call.
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
** $Log: LogCall.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "LogCall.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qdatetm.h>
#include <qapplication.h>
#include <qregexp.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <CustCallLogReport.h>

#include "CustomerPicker.h"
#include "TAATools.h"

#define TICK_INCREMENT 60

LogCall::LogCall
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    setCaption("Call Logger");

    // Create all of our widgets.
    loginID = new QLineEdit(this, "loginID");
    loginID->setMaxLength(16);
    connect(loginID, SIGNAL(textChanged(const QString &)), this, SLOT(loginIDChanged(const QString &)));
    loginIDLabel = new QLabel(loginID, "&Login ID", this);
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);

    searchButton = new QPushButton(this, "searchButton");
    searchButton->setText("&Search");
    connect(searchButton, SIGNAL(clicked()), this, SLOT(findLogin()));
    
    // Searching doesn't really work very well, so lets hide the button for
    // now.
    searchButton->hide();

    callType = new QComboBox(false, this, "callType");
    callType->insertItem("Support");
    callType->insertItem("Sales");
    callType->insertItem("New User");
    callType->insertItem("Accounting");
    connect(callType, SIGNAL(highlighted(int)), this, SLOT(callTypeChanged(int)));
    typeLabel = new QLabel(callType, "&Type", this);
    typeLabel->setAlignment(AlignRight|AlignVCenter);

    reasonList = new QListView(this, "reasonList");
    reasonList->addColumn("Call Subject");
    reasonList->setMaximumHeight(125);
    reasonLabel = new QLabel(reasonList, "&Reason", this);
    reasonLabel->setAlignment(AlignRight|AlignTop);
    
    solutionNotes = new QMultiLineEdit(this, "solutionNotes");
    solutionNotes->setWordWrap(QMultiLineEdit::WidgetWidth);
    solutionLabel = new QLabel(solutionNotes, "S&olution", this);
    solutionLabel->setAlignment(AlignRight|AlignTop);
    
    // Now, our buttons.
    appendButton = new QPushButton(this, "appendButton");
    appendButton->setText("&Append to call");
    connect(appendButton, SIGNAL(clicked()), this, SLOT(addToCall()));
    
    viewLogButton = new QPushButton(this, "viewLogButton");
    viewLogButton->setText("&View Log");
    connect(viewLogButton, SIGNAL(clicked()), this, SLOT(viewCallLog()));

    custWinButton = new QPushButton(this, "custWinButton");
    custWinButton->setText("&Customer Window");
    connect(custWinButton, SIGNAL(clicked()), this, SLOT(loadCustomer()));
    
    clockButton = new QPushButton(this, "clockButton");
    clockButton->setText("Sto&p Timer");
    connect(clockButton, SIGNAL(clicked()), this, SLOT(toggleTimer()));

    doneButton = new QPushButton(this, "doneButton");
    doneButton->setText("&Done");
    connect(doneButton, SIGNAL(clicked()), this, SLOT(hangupCall()));

    statusLabel = new QLabel(this, "statusLabel");
    statusLabel->setFrameStyle(QFrame::Sunken|QFrame::WinPanel);

    clockArea = new QLCDNumber(this, "clockArea");
    clockArea->setMode(QLCDNumber::DEC);
    clockArea->setSegmentStyle(QLCDNumber::Flat);
    clockArea->setNumDigits(7);

    
    // Main Layout, top to bottom, with a grid and stuff thrown in.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout(3, 5);
    gl->setRowStretch(0, 0);
    gl->setRowStretch(1, 0);
    gl->setRowStretch(2, 1);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 0);
    gl->setColStretch(4, 0);

    gl->addWidget(loginIDLabel,             0, 0);
    gl->addWidget(loginID,                  0, 1);
    gl->addWidget(searchButton,             0, 2);
    gl->addWidget(typeLabel,                0, 3);
    gl->addWidget(callType,                 0, 4);

    gl->addWidget(reasonLabel,              1, 0);
    gl->addMultiCellWidget(reasonList,      1, 1, 1, 4);
    
    gl->addWidget(solutionLabel,            2, 0);
    gl->addMultiCellWidget(solutionNotes,   2, 2, 1, 4);

    ml->addLayout(gl, 1);

    QBoxLayout  *abl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    abl->addWidget(appendButton, 1);
    abl->addWidget(viewLogButton,  1);
    abl->addWidget(custWinButton,  1);
    abl->addWidget(clockButton,  1);
    abl->addWidget(doneButton,  1);

    // These options don't really work very well, so lets disable them for
    // now.
    appendButton->hide();
    viewLogButton->hide();

    ml->addLayout(abl, 0);


    QBoxLayout  *sbl = new QBoxLayout(QBoxLayout::LeftToRight, 3);

    sbl->addWidget(statusLabel,             1);
    sbl->addWidget(clockArea,               0);

    ml->addLayout(sbl, 0);

    
    // Set the starting login ID to empty, and the starting customer ID to 0
    strcpy(myLoginID, "");
    myCustID = 0;

    // Start the clock ticking...
    callStart.setDate(QDate::currentDate());
    callStart.setTime(QTime::currentTime());

    // Set the status label to the default.
    statusLabel->setText("No customer selected.");
    
    callTypeChanged(0);
    
    // And setup the timer to update our clock once every second.
    updateClock();
    callTimer   = new QTimer(this);
    connect(callTimer, SIGNAL(timeout()), this, SLOT(updateClock()));
    callTimer->start(1000);         // Once every second
    timerRunning = 1;
    clockArea->display("00:00:00");


    // Make some hotkeys...
    loginIDLabel->setBuddy(loginID);
    typeLabel->setBuddy(callType);
    reasonLabel->setBuddy(reasonList);
    solutionLabel->setBuddy(solutionNotes);
    
    // Add wordwrap.
    myLinkedCall = 0;

    loginID->setFocus();
}

LogCall::~LogCall()
{
    disconnect(callTimer, SIGNAL(timeout()));
}


/*
** callTypeChanged - Updates the type of call made whenever the
**                   selected item in the drop down list changes.
*/

void LogCall::callTypeChanged(int newType)
{
    switch(newType) {
        case    0:      // Support Call
            reasonList->clear();
		    (void) new QListViewItem(reasonList, "General");
		    (void) new QListViewItem(reasonList, "Connection Problems");
		    (void) new QListViewItem(reasonList, "CCC Problems");
		    (void) new QListViewItem(reasonList, "Mail Retrieval Problems");
            break;
            
        case    1:      // Sales/Info call
            reasonList->clear();
		    (void) new QListViewItem(reasonList, "General Information");
            break;

        case    2:      // New User Setup
            reasonList->clear();
            (void) new QListViewItem(reasonList, "New Add-on Account");
		    (void) new QListViewItem(reasonList, "New User Setup");
            break;

        case    3:      // Accounting Call
            reasonList->clear();
            (void) new QListViewItem(reasonList, "Posted Payment");
            (void) new QListViewItem(reasonList, "Grace Extension");
		    (void) new QListViewItem(reasonList, "Duplicate Setup Charges");
		    (void) new QListViewItem(reasonList, "Billing Dipsute");
		    (void) new QListViewItem(reasonList, "Late Payment");
            break;
    }
    // This one applies to all.
    (void) new QListViewItem(reasonList, "Uncategorized/Other");

    reasonList->repaint();
}


/*
** updateClock - The slot that gets called once per second to update
**               the clock and the timer bar.
*/

void LogCall::updateClock()
{
    if (timerRunning) {
	    char    tmpStr[1024];
	    QTime   tmpTime(0,0,0,0);       // An empty time.
	    QTime   elapsed(0,0,0,0);
	    int     tmpSecs;
	    
	    callEnd.setDate(QDate::currentDate());
	    callEnd.setTime(QTime::currentTime());
	    
	    tmpSecs = callStart.secsTo(callEnd);
	    elapsed = tmpTime.addSecs(tmpSecs);
	    
	    
	    sprintf(tmpStr, "%2d:%02d:%02d", elapsed.hour(), elapsed.minute(), elapsed.second());
        clockArea->display(tmpStr);
    }    
}


/*
** setLoginID  - Sets the login ID of the customer that is calling.
*/

void LogCall::setLoginID(const char * newLoginID)
{
    QApplication::setOverrideCursor(waitCursor);
    if (strlen(newLoginID) < 32) {
        strcpy(myLoginID, newLoginID);
        loginID->setText(myLoginID);
        
        if (strlen(myLoginID)) {
            // Now, get the customer ID for this login ID
            char        *query = new char[32768];
            ADB         DB;
        
            DB.query("select CustomerID from Logins where LoginID = '%s'", myLoginID);
            if (DB.rowCount == 1) {
                DB.getrow();
                myCustID = atol(DB.curRow["CustomerID"]);
                // Load the customer's name so we can verify who we're talking
                // to.
                CustomersDB CDB;
                CDB.get(myCustID);
                strcpy(custName, (const char *) CDB.getStr("FullName"));
                sprintf(query, "Customer ID %ld selected (%s)", myCustID, custName);
                statusLabel->setText(query);
            } else {
                myCustID = 0;
                statusLabel->setText("No customer selected");
            }
        } else {
            myCustID = 0;
            statusLabel->setText("No customer selected");
        }
    }
    QApplication::restoreOverrideCursor();
}


/*
** loadCustomer - Opens up a customer window if the customer ID is valid.
*/

void LogCall::loadCustomer()
{
    if (myCustID) {
        char    tmpStr[1024];
        statusLabel->setText("Loading customer information...");
        emit(openCustomer(myCustID));
        sprintf(tmpStr, "Customer ID %ld selected (%s)", myCustID, custName);
        statusLabel->setText(tmpStr);
    } else {
        statusLabel->setText("No customer selected.");
    }
}

/*
** loginIDChanged - Gets called whenever the user types something into
**                  the login ID box.
*/

void LogCall::loginIDChanged(const char * newLoginID)
{
    setLoginID(newLoginID);
}

/*
** loginIDChanged - Gets called whenever the user types something into
**                  the login ID box.
*/

void LogCall::loginIDChanged(const QString &newLoginID)
{
    setLoginID((const char *) newLoginID);
}


/*
** hangupCall - Validates that we have enough information to disconnect the
**              call.  If so, it stops the timer and stores the record in
**              the database.
*/

void LogCall::hangupCall()
{
    
    // First, turn off the timer.
    timerRunning = 0;
    
    // If the call type is sales, we don't need a login ID or customer.
    if (strcasecmp("Sales", callType->text(callType->currentItem()))) {
	    
	    // Verify we have a login ID
	    if (!strlen(myLoginID)) {
	        statusLabel->setText("No login ID specified.  Select a customer before closing.");
	        return;
	    }
	    
	    // Verify that we have a valid customer record.
	    if (!myCustID) {
	        statusLabel->setText("No customer selected.  Select a customer before closing.");
	        return;
	    }
	}
    
    // Verify that a reason has been selected
    QListViewItem   *curItem = reasonList->currentItem();
    if (curItem == NULL) {
        statusLabel->setText("No call reason selected.  Select one before closing.");
        return;
    }
    
    // Verify that a reason has been selected
    if (!strlen(solutionNotes->text())) {
        statusLabel->setText("No solution entered.");
        return;
    }

    // If we've made it this far, we can save the record.
    statusLabel->setText("Saving call...");
    
    ADBTable    CLDB("Notes");
    
    // Okay, start storing the data...
    CLDB.setValue("NoteDate", callStart);
    CLDB.setValue("StopDate",  callEnd);
    CLDB.setValue("Duration",  callStart.secsTo(callEnd));
    
    CLDB.setValue("AddedBy", curUser().userName);
    CLDB.setValue("CustomerID", myCustID);
    CLDB.setValue("LoginID", myLoginID);
    
    CLDB.setValue("Category", callType->text(callType->currentItem()));
    CLDB.setValue("SubCategory", curItem->key(0,1));
    CLDB.setValue("NoteType", "Call Log");
    CLDB.setValue("Subject",  "Call Log");
    
    CLDB.setValue("NoteText", solutionNotes->text());

    
    CLDB.ins();
    
    emit(customerUpdated(myCustID));
    
    close();
}

/*
** viewCallLog   - If a login/customer is selected, this will bring up
**                 their previous call history in a new window.
*/

void LogCall::viewCallLog()
{
    if (myCustID) {
        char    tmpStr[1024];
        strcpy(tmpStr, statusLabel->text());
        statusLabel->setText("Loading call log information...");
        CustCallLogReport   *CCLR = new CustCallLogReport();
        CCLR->setCustID(myCustID);
        CCLR->show();
        statusLabel->setText(tmpStr);
    } else {
        statusLabel->setText("No customer ID selected.");
    }
}

/*
** addToCall - If a login/customer is selected, this will bring up a 
**             new window with all of the customers previous calls to allow
**             a single problem to be tracked.
*/

void LogCall::addToCall()
{
    if (myCustID) {
	    CallPicker  *CP = new CallPicker();
	    CP->setCustID(myCustID);
	    CP->show();
        connect(CP, SIGNAL(CallSelected(long)), SLOT(callLinked(long)));
    } else {
        statusLabel->setText("No customer ID selected.");
    }
}

/*
** findLogin  - Brings up a search window to find the login ID of a customer.
*/

void LogCall::findLogin()
{
    CustomerPicker *CP = new CustomerPicker();
    CP->show();
    statusLabel->setText("Not yet implemented...");
}

/*
** callLinked - Gets connected to the CallPicker when it is activated.
*/

void LogCall::callLinked(long callID)
{
    myLinkedCall = callID;
    char    tmpStr[1024];
    sprintf(tmpStr, "Linked to call ID %ld...", callID);
    statusLabel->setText(tmpStr);
}

/*
** toggleTimer - Turns off the clock.
*/

void LogCall::toggleTimer()
{
    timerRunning = 0;
    clockButton->setEnabled(FALSE);
    statusLabel->setText("Timer stopped.");
}



/*
** CallPicker - The main widget that selects a previous call.
*/

CallPicker::CallPicker
(
	QWidget* parent,
	const char* name
)
	:
	CustCallLogReport( parent, name )
{
    allowReproduction(isManager());
    setTitle("Select the call to link to");
}

CallPicker::~CallPicker()
{
}


/*
** listItemSelected - Gets called when an item in the list is double 
**                    clicked.
*/

void CallPicker::listItemSelected(QListViewItem *curItem)
{
    emit CallSelected(atol(curItem->key(7,0)));
    close();
}

