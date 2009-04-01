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
#include <stdlib.h>

#include <QtCore/QRegExp>
#include <QtCore/QDateTime>
#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
//Added by qt3to4:
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3Frame>
#include <Qt3Support/q3listview.h>

#include <BlargDB.h>
#include <TAATools.h>

#include "LogCall.h"

#define TICK_INCREMENT 60

using namespace Qt;

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

    reasonList = new Q3ListView(this, "reasonList");
    reasonList->addColumn("Call Subject");
    reasonList->setMaximumHeight(125);
    reasonLabel = new QLabel(reasonList, "&Reason", this);
    reasonLabel->setAlignment(AlignRight|AlignTop);
    
    solutionNotes = new Q3MultiLineEdit(this, "solutionNotes");
    solutionNotes->setWordWrap(Q3MultiLineEdit::WidgetWidth);
    solutionLabel = new QLabel(solutionNotes, "S&olution", this);
    solutionLabel->setAlignment(AlignRight|AlignTop);
    
    // Now, our buttons.
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
    statusLabel->setFrameStyle(Q3Frame::Sunken|Q3Frame::WinPanel);

    clockArea = new QLCDNumber(this, "clockArea");
    clockArea->setMode(QLCDNumber::DEC);
    clockArea->setSegmentStyle(QLCDNumber::Flat);
    clockArea->setNumDigits(7);

    
    // Main Layout, top to bottom, with a grid and stuff thrown in.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3GridLayout *gl = new Q3GridLayout(3, 5);
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

    Q3BoxLayout  *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
    abl->addWidget(custWinButton,  1);
    abl->addWidget(clockButton,  1);
    abl->addWidget(doneButton,  1);

    // These options don't really work very well, so lets disable them for
    // now.

    ml->addLayout(abl, 0);


    Q3BoxLayout  *sbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);

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
		    (void) new Q3ListViewItem(reasonList, "General");
		    (void) new Q3ListViewItem(reasonList, "Connection Problems");
		    (void) new Q3ListViewItem(reasonList, "CCC Problems");
		    (void) new Q3ListViewItem(reasonList, "Mail Retrieval Problems");
            break;
            
        case    1:      // Sales/Info call
            reasonList->clear();
		    (void) new Q3ListViewItem(reasonList, "General Information");
            break;

        case    2:      // New User Setup
            reasonList->clear();
            (void) new Q3ListViewItem(reasonList, "New Add-on Account");
		    (void) new Q3ListViewItem(reasonList, "New User Setup");
            break;

        case    3:      // Accounting Call
            reasonList->clear();
            (void) new Q3ListViewItem(reasonList, "Posted Payment");
            (void) new Q3ListViewItem(reasonList, "Grace Extension");
		    (void) new Q3ListViewItem(reasonList, "Duplicate Setup Charges");
		    (void) new Q3ListViewItem(reasonList, "Billing Dipsute");
		    (void) new Q3ListViewItem(reasonList, "Late Payment");
            break;
    }
    // This one applies to all.
    (void) new Q3ListViewItem(reasonList, "Uncategorized/Other");

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
    Q3ListViewItem   *curItem = reasonList->currentItem();
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
** toggleTimer - Turns off the clock.
*/

void LogCall::toggleTimer()
{
    timerRunning = 0;
    clockButton->setEnabled(FALSE);
    statusLabel->setText("Timer stopped.");
}




// vim: expandtab
