/*
** $Id$
**
***************************************************************************
**
** ProcessVoiceMail - A voice mail return call tracking interface.
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
** $Log: ProcessVoiceMail.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>

#include <TAA.h>
#include "ProcessVoiceMail.h"
#include "LogVoiceMail.h"

ProcessVoiceMail::ProcessVoiceMail
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Process Voice Mail" );

    vmList = new QListView(this, "Voice Mail List");
    vmList->addColumn("Date/Time");
    vmList->addColumn("Disposition");
    vmList->addColumn("Message");
    vmList->setAllColumnsShowFocus(true);
    connect(vmList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(listItemChanged(QListViewItem *)));

    QLabel  *msgDateLabel = new QLabel(this);
    msgDateLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    msgDateLabel->setText("Date:");

    msgDate = new QLabel(this, "MsgDate");
    msgDate->setText("");
    
    QLabel  *msgTimeLabel = new QLabel(this);
    msgTimeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    msgTimeLabel->setText("Time:");
    
    msgTime = new QLabel(this, "MsgTime");
    msgTime->setText("");
    
    QLabel  *loggedByLabel= new QLabel(this);
    loggedByLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    loggedByLabel->setText("Logged by:");

    loggedBy= new QLabel(this);
    loggedBy->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    loggedBy->setText(" - logged by - ");
    
    QLabel  *loggedAtLabel= new QLabel(this);
    loggedAtLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    loggedAtLabel->setText("at:");

    loggedAt= new QLabel(this);
    loggedAt->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    loggedAt->setText(" - logged at - ");

    message = new QTextView(this, "Message Content");
    message->setTextFormat(Qt::RichText);

    QLabel  *dispositionLabel= new QLabel(this);
    dispositionLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    dispositionLabel->setText("Disposition:");

    disposition = new QComboBox(false, this, "Disposition");
    int i = 0;
    char tmpSt[1024];
    while (voicemail_codes[i].dispid >= 0) {
        sprintf(tmpSt, "%s - %s", voicemail_codes[i].code, voicemail_codes[i].description);
        disposition->insertItem(tmpSt);
        i++;
    }
    /*
    disposition->insertItem("New/Pending");
    disposition->insertItem( "RCLVM - Returned Call, Left Voice Mail" );
    disposition->insertItem( "RCSWP - Returned Call, Spoke with Person" );
    disposition->insertItem( "RCNA - Returned Call, No Answer" );
    disposition->insertItem( "RVE - Responded via Email" );
    disposition->insertItem( "NCIG - No Contact Informatin Given" );
    */

    QLabel  *loginIDLabel= new QLabel(this);
    loginIDLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    loginIDLabel->setText("Login ID:");

    loginID = new QLineEdit(this, "Login ID");
    loginID->setMaxLength(16);
    connect(loginID, SIGNAL(textChanged(const QString &)), this, SLOT(loginIDChanged(const QString &)));

    QLabel  *customerIDLabel= new QLabel(this);
    customerIDLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    customerIDLabel->setText("Customer ID:");

    customerID= new QLabel(this);
    customerID->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    customerID->setText(" - CustID - ");
    
    QLabel  *customerNameLabel= new QLabel(this);
    customerNameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    customerNameLabel->setText("Customer:");

    customerName= new QLabel(this);
    customerName->setAlignment(Qt::AlignLeft);
    customerName->setText(" - Customer Name - ");
    
    QLabel  *followUpLabel= new QLabel(this);
    followUpLabel->setAlignment(Qt::AlignLeft);
    followUpLabel->setText("Follow-up:");

    followUp = new QMultiLineEdit(this, "FollowUp");
    followUp->setWordWrap(QMultiLineEdit::WidgetWidth);
    followUp->setWrapPolicy(QMultiLineEdit::AtWhiteSpace);

    processed = new QCheckBox(this, "Processed");
    processed->setText("Processed");

    saveButton = new QPushButton(this, "Save Button");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));


    // Create our layout.
    // Box layout, left to right, with a grid layout for the actions.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3); 

    ml->addWidget(vmList, 0);

    // Now we add another layout, left to right where the log details go.
    // The voice mail data goes on the left and the followup goes on the 
    // right.
    QBoxLayout *dl = new QBoxLayout(QBoxLayout::LeftToRight, 5);

    // Create a grid of our widgets, 4 wide, 9 high.
    QGridLayout *gl1 = new QGridLayout(9, 4, 2);
    int curRow = 0;
    gl1->setColStretch(0, 0);
    gl1->setColStretch(1, 1);
    gl1->setColStretch(2, 0);
    gl1->setColStretch(3, 1);

    gl1->addWidget(msgDateLabel,           curRow, 0);
    gl1->addWidget(msgDate,                curRow, 1);
    gl1->addWidget(msgTimeLabel,           curRow, 2);
    gl1->addWidget(msgTime,                curRow, 3);
    gl1->setRowStretch(curRow, 0);
    curRow++;

    gl1->addWidget(loggedByLabel,          curRow, 0);
    gl1->addWidget(loggedBy,               curRow, 1);
    gl1->addWidget(loggedAtLabel,          curRow, 2);
    gl1->addWidget(loggedAt,               curRow, 3);
    gl1->setRowStretch(curRow, 0);
    curRow++;

    gl1->addMultiCellWidget(message,       curRow, curRow, 0, 3);
    gl1->setRowStretch(curRow, 1);
    curRow++;

    // Add this grid to our detail layout
    dl->addLayout(gl1, 0);

    // Create the second grid
    QGridLayout *gl2 = new QGridLayout(9, 4, 2);
    curRow = 0;
    gl2->addWidget(dispositionLabel,       curRow, 0);
    gl2->addMultiCellWidget(disposition,   curRow, curRow, 1, 3);
    gl2->setRowStretch(curRow, 0);
    curRow++;

    gl2->addWidget(loginIDLabel,           curRow, 0);
    gl2->addWidget(loginID,                curRow, 1);
    gl2->addWidget(customerIDLabel,        curRow, 2);
    gl2->addWidget(customerID,             curRow, 3);
    gl2->setRowStretch(curRow, 0);
    curRow++;

    gl2->addWidget(customerNameLabel,      curRow, 0);
    gl2->addMultiCellWidget(customerName,  curRow, curRow, 1, 3);
    gl2->setRowStretch(curRow, 0);
    curRow++;

    gl2->addWidget(followUpLabel,          curRow, 0);
    gl2->setRowStretch(curRow, 0);
    curRow++;
    
    gl2->addMultiCellWidget(followUp,      curRow, curRow, 0, 3);
    gl2->setRowStretch(curRow, 1);
    curRow++;

    // A button layout in the middle of the grid...
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 0, 0);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    gl2->addWidget(processed,              curRow, 1);
    gl2->addItem(bl,                       curRow, 3);
    gl2->setRowStretch(curRow, 0);
    curRow++;

    dl->addLayout(gl2, 1);
    // Add the grid to our main layout
    ml->addLayout(dl, 1);

    // Connect our slots...
   
    refreshList();
    QTimer  *lTimer = new QTimer(this);
    connect(lTimer, SIGNAL(timeout()), this, SLOT(refreshList()));
    lTimer->start(5000);       // Update once every 5 seconds for now.

    vmList->setFocus();

    // Create a menu for our parent to pull into its own menu bar
    vmMenu = new QPopupMenu(this, "Voice Mail Menu");
    vmMenu->insertItem("&Log Voice Mail", this, SLOT(logVM()));

    myCustID = 0;

}

ProcessVoiceMail::~ProcessVoiceMail()
{
}

/*
** menu - Returns the pointer to our QPopupMenu
*/

QPopupMenu *ProcessVoiceMail::menu()
{
    return vmMenu;
}

void ProcessVoiceMail::saveClicked()
{
    // Check to see if we have one highlighted.  If so, load it up
    // and make any changes to it.
    if (!vmList->currentItem()) return;

    ADBTable    vmDB("VoiceMail");
    char        tmpStr[1024];
    QDate       tmpDate = QDate::currentDate();
    QTime       tmpTime = QTime::currentTime();

    vmDB.get(atoi(vmList->currentItem()->key(3,0)));
    vmDB.setValue("Followup", (const char *)followUp->text());
    vmDB.setValue("Disposition", disposition->currentItem());
    if (myCustID) {
        vmDB.setValue("CustomerID", myCustID);
        vmDB.setValue("LoginID",    (const char *)loginID->text());
    }
    if (processed->isChecked()) {
        vmDB.setValue("Processed", 1);
        sprintf(tmpStr, "%04d-%02d-%02d %02d:%02d:%02d",
                tmpDate.year(), tmpDate.month(), tmpDate.day(),
                tmpTime.hour(), tmpTime.minute(), tmpTime.second());
        vmDB.setValue("ProcessedOn", tmpStr);
    } else {
        vmDB.setValue("Processed", 0);
        sprintf(tmpStr, "%04d-%02d-%02d %02d:%02d:%02d",
                tmpDate.year(), tmpDate.month(), tmpDate.day(),
                tmpTime.hour(), tmpTime.minute(), tmpTime.second());
        vmDB.setValue("ProcessedOn", tmpStr);
    }
    vmDB.upd();

    refreshList();
}

/*
** count - Returns the number of voice mail message in the list.
*/

int ProcessVoiceMail::count()
{
    // Figure out which entry is selected.  
    return myVMCount;
}

/*
** refreshList - Refreshes the list of un-processed voice mail messages.
*/

void ProcessVoiceMail::refreshList()
{
    ADB     DB;
    bool    foundIt;
    int     statusColumn = 4;
    char    dispStr[1024];
    QListViewItem   *curItem = vmList->currentItem();
    long    curSel = 0;
    
    // Save the currently selected item.
    if (curItem) {
        curSel = atol(curItem->key(3,0));
    }

    // Scan through the list and mark all entries as removable
    {
        QListViewItemIterator   tmpit(vmList);
        for (;tmpit.current(); ++tmpit) tmpit.current()->setText(statusColumn, "0");
    }

    myVMCount = 0;
    DB.query("select MsgID, MsgDateTime, Disposition, Message from VoiceMail where Processed = 0");
    if (DB.rowCount) {
        while (DB.getrow()) {
            myVMCount++;
            // Get the verbose description of the disposition
            if (atoi(DB.curRow["Disposition"]) > MAX_VOICEMAIL_CODE ||
                    atoi(DB.curRow["Disposition"]) < 0) {
                strcpy(dispStr, voicemail_codes[0].code);
            } else {
                strcpy(dispStr, voicemail_codes[atoi(DB.curRow["Disposition"])].code);
            }

            // See if we have this VM in the list already, if so, update
            // it rather than adding it.
            foundIt = false;
            QListViewItemIterator   it(vmList);
            for (; it.current(); ++it) {
                if ((atoi(it.current()->key(3,0))) == atoi(DB.curRow["MsgID"])) {
                    foundIt = true;
                    it.current()->setText(0, DB.curRow["MsgDateTime"]);
                    it.current()->setText(1, dispStr);
                    it.current()->setText(2, DB.curRow["Message"]);
                    it.current()->setText(statusColumn, "1");
                }
            }

            if (!foundIt) {
                // We don't have this one loaded, add it.
                (void) new QListViewItem(vmList, DB.curRow["MsgDateTime"], dispStr, DB.curRow["Message"], DB.curRow["MsgID"], "1");
            }

        }
    }

    // Now, walk through the list and delete any items that were not
    // in the query result.
    {
        QListViewItemIterator   tmpit(vmList);
        for (; tmpit.current(); ++tmpit) {
            if (!atol(tmpit.current()->key(statusColumn,0))) {
                vmList->takeItem(tmpit.current());
            }
        }
    }

    // Now, walk through the list and see if the currently selected
    // item is still in the list.  If its not, select the first item
    // in the list.
    foundIt = false;
    {
        QListViewItemIterator   tmpit(vmList);
        for (; tmpit.current(); ++tmpit) {
            if (atol(tmpit.current()->key(3,0)) == curSel) foundIt = true;
        }
    }
    if (!foundIt && vmList->childCount()) {
        vmList->clearSelection();
        vmList->setSelected(vmList->firstChild(),true);
        vmList->setCurrentItem(vmList->firstChild());
        vmList->ensureItemVisible(vmList->firstChild());
        listItemChanged(vmList->firstChild());
    }

    
}

/*
** listItemChanged - A public slot that gets called by the vmList whenever
**                   the selection changes.
*/

void ProcessVoiceMail::listItemChanged(QListViewItem *curItem)
{
    if (curItem) {
        ADBTable    vmDB("VoiceMail");
        vmDB.get(atol(curItem->key(3,0)));
        msgDate->setText(vmDB.getDateTime("MsgDateTime").date().toString());
        msgTime->setText(vmDB.getDateTime("MsgDateTime").time().toString());
        loggedBy->setText(vmDB.getStr("LoggedBy"));
        loggedAt->setText(vmDB.getStr("LoggedAt"));
        message->setText(vmDB.getStr("Message"));
        disposition->setCurrentItem(vmDB.getInt("Disposition"));
        followUp->setText(vmDB.getStr("Followup"));
        msgDate->setEnabled(true);
        message->setEnabled(true);
        disposition->setEnabled(true);
        followUp->setEnabled(true);
        processed->setEnabled(true);
        processed->setChecked(vmDB.getInt("Processed"));
        loginID->setText(vmDB.getStr("LoginID"));
        loginID->setEnabled(true);
        customerID->setText("");
        customerName->setText("");
        saveButton->setEnabled(true);
        myCustID = 0;
        // Load up the customer info
        if (vmDB.getLong("CustomerID")) {
            ADB tmpDB;
            tmpDB.query("select CustomerID, FullName from Customers where CustomerID = %ld",
                    vmDB.getLong("CustomerID"));
            if (tmpDB.rowCount) {
                tmpDB.getrow();
                customerID->setText(tmpDB.curRow["CustomerID"]);
                customerName->setText(tmpDB.curRow["FullName"]);
                myCustID = vmDB.getLong("CustomerID");
            }
        }
    } else {
        // Clear all entries and disable them.
        msgDate->setText("");
        msgTime->setText("");
        loggedBy->setText("");
        loggedAt->setText("");
        message->setText("");
        disposition->setCurrentItem(0);
        followUp->clear();
        msgDate->setEnabled(false);
        message->setEnabled(false);
        disposition->setEnabled(false);
        followUp->setEnabled(false);
        processed->setEnabled(false);
        processed->setChecked(false);
        loginID->setEnabled(false);
        loginID->setText("");
        customerID->setText("");
        customerName->setText("");
        saveButton->setEnabled(false);
        myCustID = 0;
    }
}


/*
** loginIDChanged - Gets called whenever the user changes the text in the
**                  login ID edit box.
*/

void ProcessVoiceMail::loginIDChanged(const QString &newLogin)
{
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    ADB     tmpDB;
    tmpDB.query("select Logins.LoginID, Customers.CustomerID, Customers.FullName from Logins, Customers where Logins.LoginID = '%s' and Customers.CustomerID = Logins.CustomerID",
            tmpDB.escapeString((const char *)newLogin));
    if (tmpDB.rowCount) {
        tmpDB.getrow();
        customerID->setText(tmpDB.curRow["CustomerID"]);
        customerName->setText(tmpDB.curRow["FullName"]);
        myCustID = atol(tmpDB.curRow["CustomerID"]);
    } else {
        customerID->setText("");
        customerName->setText("");
        myCustID = 0;
    }
    //QApplication::restoreOverrideCursor();
}

/*
** logVM - Opens the LogVoiceMail window.
*/

void ProcessVoiceMail::logVM()
{
    LogVoiceMail    *lvm = new LogVoiceMail();
    lvm->show();
}
