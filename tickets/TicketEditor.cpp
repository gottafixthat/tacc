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

#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtGui/QLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3Frame>

#include <TAA.h>
#include <TAATools.h>
#include <ADB.h>
#include <Cfg.h>
#include <FParse.h>

#include "Ticket.h"
#include "TicketEditor.h"

using namespace Qt;

TicketHeader::TicketHeader
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("Ticket Editor");
    
    // Create the various widgets.  Most of them will be display only, but
    // we need them all to be there for completeness.
    
    // The ticket number
    QLabel      *dispTicketNoLabel  = new QLabel(this);
    dispTicketNoLabel->setAlignment(AlignRight|AlignVCenter);
    dispTicketNoLabel->setText("Ticket No:");

    dispTicketNo = new QLabel(this);
    dispTicketNo->setAlignment(AlignLeft|AlignVCenter);
    dispTicketNo->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The staff member who owns the ticket.
    QLabel      *dispOwnerLabel  = new QLabel(this);
    dispOwnerLabel->setAlignment(AlignRight|AlignVCenter);
    dispOwnerLabel->setText("Owner:");

    dispOwner = new QLabel(this);
    dispOwner->setAlignment(AlignLeft|AlignVCenter);
    dispOwner->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);
    
    // The current status of this ticket
    QLabel      *dispStatusLabel  = new QLabel(this);
    dispStatusLabel->setAlignment(AlignRight|AlignVCenter);
    dispStatusLabel->setText("Status:");

    dispStatus = new QLabel(this);
    dispStatus->setAlignment(AlignLeft|AlignVCenter);
    dispStatus->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The date the ticket was opened
    QLabel      *dispOpenedLabel  = new QLabel(this);
    dispOpenedLabel->setAlignment(AlignRight|AlignVCenter);
    dispOpenedLabel->setText("Opened:");

    dispOpened = new QLabel(this);
    dispOpened->setAlignment(AlignLeft|AlignVCenter);
    dispOpened->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The date the ticket was last modified
    QLabel      *dispModifiedLabel  = new QLabel(this);
    dispModifiedLabel->setAlignment(AlignRight|AlignVCenter);
    dispModifiedLabel->setText("Modified:");

    dispModified = new QLabel(this);
    dispModified->setAlignment(AlignLeft|AlignVCenter);
    dispModified->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The date the ticket was closed
    QLabel      *dispClosedLabel  = new QLabel(this);
    dispClosedLabel->setAlignment(AlignRight|AlignVCenter);
    dispClosedLabel->setText("Closed:");

    dispClosed = new QLabel(this);
    dispClosed->setAlignment(AlignLeft|AlignVCenter);
    dispClosed->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The customer ID
    QLabel      *dispCustomerIDLabel  = new QLabel(this);
    dispCustomerIDLabel->setAlignment(AlignRight|AlignVCenter);
    dispCustomerIDLabel->setText("Customer ID:");

    dispCustomerID = new QLabel(this);
    dispCustomerID->setAlignment(AlignLeft|AlignVCenter);
    dispCustomerID->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The contact name, generally the person that opened the ticket.
    QLabel      *dispNameLabel  = new QLabel(this);
    dispNameLabel->setAlignment(AlignRight|AlignVCenter);
    dispNameLabel->setText("Contact Name:");

    dispName = new QLabel(this);
    dispName->setAlignment(AlignLeft|AlignVCenter);
    dispName->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The department that this ticket is in
    QLabel      *dispTypeLabel  = new QLabel(this);
    dispTypeLabel->setAlignment(AlignRight|AlignVCenter);
    dispTypeLabel->setText("Type/Dept:");

    dispType = new QLabel(this);
    dispType->setAlignment(AlignLeft|AlignVCenter);
    dispType->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // Summary
    QLabel      *summaryLabel  = new QLabel(this);
    summaryLabel->setAlignment(AlignRight|AlignVCenter);
    summaryLabel->setText("Summary:");

    summary = new QLabel(this);
    summary->setAlignment(AlignLeft|AlignVCenter);
    summary->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    
    // Now, do the layout for the widget.
    // Create the grid for the tabular information.  We'll do this
    // 6 columns wide.
    Q3GridLayout *tl = new Q3GridLayout(this, 5, 6, 2, 1);
    tl->setColStretch(0, 0);
    tl->setColStretch(1, 1);
    tl->setColStretch(2, 0);
    tl->setColStretch(3, 1);
    tl->setColStretch(4, 0);
    tl->setColStretch(5, 1);

    int curRow = 0;
    int curCol = 0;

    tl->setRowStretch(curRow, 0);
    tl->addWidget(dispTicketNoLabel,        curRow, curCol++);
    tl->addWidget(dispTicketNo,             curRow, curCol++);
    tl->addWidget(dispOwnerLabel,           curRow, curCol++);
    tl->addWidget(dispOwner,                curRow, curCol++);
    tl->addWidget(dispStatusLabel,          curRow, curCol++);
    tl->addWidget(dispStatus,               curRow, curCol++);

    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(dispOpenedLabel,          curRow, curCol++);
    tl->addWidget(dispOpened,               curRow, curCol++);
    tl->addWidget(dispModifiedLabel,        curRow, curCol++);
    tl->addWidget(dispModified,             curRow, curCol++);
    tl->addWidget(dispClosedLabel,          curRow, curCol++);
    tl->addWidget(dispClosed,               curRow, curCol++);

    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(dispCustomerIDLabel,      curRow, curCol++);
    tl->addWidget(dispCustomerID,           curRow, curCol++);
    tl->addWidget(dispNameLabel,            curRow, curCol++);
    tl->addWidget(dispName,                 curRow, curCol++);
    tl->addWidget(dispTypeLabel,            curRow, curCol++);
    tl->addWidget(dispType,                 curRow, curCol++);

    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(summaryLabel,             curRow, curCol++);
    tl->addMultiCellWidget(summary,         curRow, curRow, curCol, 6);
    
    myTicket = new Ticket();

}


TicketHeader::~TicketHeader()
{
}


/*
** setTicketNo - Sets the ticket number that we are going to edit.
*/

void TicketHeader::setTicketNo(long newTicketNo)
{
    myTicket->setTicketNo(newTicketNo);
    refreshTicket(newTicketNo);
}

/*
** refresh - Re-loads the ticket and re-displays the header information.
*/

void TicketHeader::refreshTicket(long ticketNo)
{
    // Is this us?  If so refresh, if not return.
    if (ticketNo != myTicket->ticketNo()) return;

    // Clear the display data
    dispTicketNo->setText("");
    dispCustomerID->setText("");
    dispName->setText("");
    dispOwner->setText("");
    dispType->setText("");
    dispOpened->setText("");
    dispClosed->setText("");
    dispModified->setText("");
    dispStatus->setText("");
    summary->setText("");

    // If we were just clearing out the data, return.
    if (!myTicket->ticketNo()) return;

    // Force a refresh of the data
    myTicket->setTicketNo(ticketNo);

    // Fill in the form
    char    tmpStr[1024];
    sprintf(tmpStr, "%ld", myTicket->ticketNo());
    dispTicketNo->setText(tmpStr);
    sprintf(tmpStr, "%ld", myTicket->customerID());
    if (!myTicket->customerID()) strcpy(tmpStr, "N/A");
    dispCustomerID->setText(tmpStr);
    dispName->setText(myTicket->name());
    dispOwner->setText(myTicket->owner());
    dispType->setText(myTicket->typeStr());
    dispOpened->setText(myTicket->openedStr(MM_DD_YY_HH_MMa_NA));
    dispClosed->setText(myTicket->closedStr(MM_DD_YY_HH_MMa_NA));
    dispModified->setText(myTicket->modifiedStr(MM_DD_YY_HH_MMa_NA));
    dispStatus->setText(myTicket->statusStr());
    summary->setText(myTicket->summary());
}



/*
** TicketEditor class.  This class is a "leaping off point" for the various
** editing widgets.  It is from here that tickets are altered and maintained.
*/

TicketEditor::TicketEditor
(
	QWidget* parent,
	const char* name,
    long ticketNo
) : TAAWidget( parent, name )
{
    setCaption("Ticket Editor");
    
    // Create the various widgets.  Most of them will be display only, but
    // we need them all to be there for completeness.
    
    // The ticket header
    header = new TicketHeader(this);

    HorizLine *hline1 = new HorizLine(this);

    QLabel      *logLevelLabel  = new QLabel(this);
    logLevelLabel->setAlignment(AlignRight|AlignVCenter);
    logLevelLabel->setText("Log Level:");

    logLevel = new QComboBox(false, this);
    logLevel->insertItem("Normal");
    logLevel->insertItem("Automatic");
    if (isAdmin()) logLevel->insertItem("Audit");
    connect(logLevel, SIGNAL(activated(int)), this, SLOT(setLogLevel(int)));

    reverseLog = new QCheckBox(this, "Rervese Log");
    reverseLog->setText("Reverse Log");
    reverseLog->setChecked(true);
    connect(reverseLog, SIGNAL(clicked()), this, SLOT(refreshLog()));
    
    // The history of this ticket.
    QLabel      *ticketLogLabel  = new QLabel(this);
    ticketLogLabel->setAlignment(AlignRight|AlignTop);
    ticketLogLabel->setText("History:");

    ticketLog = new Q3TextView(this, "Ticket Log");

    // Now, create the buttons for our actions.
    customerButton = new QPushButton(this, "Customer Button");
    customerButton->setText("Custom&er");
    connect(customerButton, SIGNAL(clicked()), this, SLOT(customerClicked()));

    annotateButton = new QPushButton(this, "Annotate Button");
    annotateButton->setText("&Annotate");
    connect(annotateButton, SIGNAL(clicked()), this, SLOT(annotateTicket()));

    takeTicketButton = new QPushButton(this, "Take Ticket Button");
    takeTicketButton->setText("&Take");
    connect(takeTicketButton, SIGNAL(clicked()), this, SLOT(takeTicket()));

    releaseTicketButton = new QPushButton(this, "Release Ticket Button");
    releaseTicketButton->setText("&Release");
    connect(releaseTicketButton, SIGNAL(clicked()), this, SLOT(releaseTicket()));

    QPushButton *closeButton = new QPushButton(this, "Close Button");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    
    // Now, do the layout for the widget.
    // We'll do a top to bottom box, and then put a grid in the middle of it
    // for the tabular information.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    ml->addWidget(header,0);
    ml->addWidget(hline1, 0);

    // Create the grid for the tabular information.  We'll do this
    // 6 columns wide.
    Q3GridLayout *tl = new Q3GridLayout(5, 6, 2);
    tl->setColStretch(0, 0);
    tl->setColStretch(1, 1);
    tl->setColStretch(2, 0);
    tl->setColStretch(3, 1);
    tl->setColStretch(4, 0);
    tl->setColStretch(5, 1);

    int curRow = 0;
    int curCol = 0;
    
    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(logLevelLabel,            curRow, curCol++);
    tl->addMultiCellWidget(logLevel,        curRow, curRow, curCol, 5);
    tl->addWidget(reverseLog,               curRow, 6);
    
    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 1);
    tl->addWidget(ticketLogLabel,       curRow, curCol++);
    tl->addMultiCellWidget(ticketLog,   curRow, curRow, curCol, 6);
    
    
    ml->addLayout(tl, 1);


    // Add in our action buttons.
    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
    abl->addStretch(1);
    abl->addWidget(customerButton, 0);
    abl->addWidget(annotateButton, 0);
    abl->addWidget(takeTicketButton, 0);
    abl->addWidget(releaseTicketButton, 0);
    abl->addWidget(closeButton, 0);
    
    ml->addLayout(abl, 0);

    loggedView = false;
    myLogLevel = 0;
    myTicket = new Ticket();
    setTicketNo(ticketNo);
    header->setTicketNo(ticketNo);

    // Make the window big enough to see several ticket notes.
    resize(sizeHint().width(), 450);
}


TicketEditor::~TicketEditor()
{
}


/*
** setTicketNo - Sets the ticket number that we are going to edit.
*/

void TicketEditor::setTicketNo(long newTicketNo)
{
    myTicket->setTicketNo(newTicketNo);
    if (myTicket->status() == Ticket::Unexamined) myTicket->setStatus(Ticket::Unassigned);
    header->setTicketNo(newTicketNo);
    refreshTicket(newTicketNo);
    if (newTicketNo && !loggedView) {
        myTicket->addLogEntry(Ticket::Audit, "Ticket examined by %s", curUser().userName);
        loggedView = true;
    }
}

/*
** refreshTicket - This gets called automagically by the mainWin whenever
**                 another widget says that it has updated this ticket.
*/

void TicketEditor::refreshTicket(long ticketNo)
{
    if (ticketNo != myTicket->ticketNo()) return;

    // Clear the display data
    customerButton->setEnabled(false);
    customerButton->hide();

    takeTicketButton->setEnabled(false);
    takeTicketButton->hide();

    releaseTicketButton->setEnabled(false);
    releaseTicketButton->hide();

    // If we were just clearing out the data, return.
    if (!myTicket->ticketNo()) return;

    // Fill in the form
    char    tmpStr[1024];

    sprintf(tmpStr, "Ticket Editor (%ld)", myTicket->ticketNo());
    setCaption(tmpStr);

    if (myTicket->customerID()) {
        customerButton->setEnabled(true);
        customerButton->show();
    }

    if (!strlen(myTicket->owner()) || isAdmin()) {
        takeTicketButton->show();
        takeTicketButton->setEnabled(true);
    }
    if (strlen(myTicket->owner()) && isAdmin()) {
        releaseTicketButton->show();
        releaseTicketButton->setEnabled(true);
    }
    if (!strcmp(myTicket->owner(), curUser().userName)) {
        takeTicketButton->setEnabled(false);
        takeTicketButton->hide();
        releaseTicketButton->show();
        releaseTicketButton->setEnabled(true);
    }

    refreshLog();
}


/*
** refreshLog   - Clears the log text and re-loads it.
*/

void TicketEditor::refreshLog()
{
    QApplication::setOverrideCursor(waitCursor);
    ADB         DB;
    FParser     fparse;
    QString     tmpText;
    fparse.setPath(cfgVal("TemplatePath"));

    char        orderStr[1024];
    
    if (reverseLog->isChecked()) {
        strcpy(orderStr, "order by Urgent desc, EntryTime desc");
    } else {
        strcpy(orderStr, "order by Urgent desc, EntryTime asc");
    }

    tmpText.append("<HTML>");
    DB.query("select * from TicketLog where TicketNo = %d and EntryType <= %d %s", myTicket->ticketNo(), myLogLevel, orderStr);
    if (DB.rowCount) {
        while (DB.getrow()) {
            fparse.addRow("TicketLog");
            fparse.addColumn("EntryTime",     timeToStr(DB.curRow.col("EntryTime")->toTime_t(), Mmm_DD_YYYY_HH_MMa));
            fparse.addColumn("EntryBy",       DB.curRow["EntryBy"]);
            fparse.addColumn("LogNotes",      DB.curRow["LogNotes"]);
            if (atoi(DB.curRow["Urgent"])) {
                fparse.addColumn("Urgent", cfgVal("UrgentTicketTag"));
            }
            switch((Ticket::t_LogType) atoi(DB.curRow["EntryType"])) {
                case    Ticket::Normal:
                    fparse.addColumn("EntryType", "");
                    break;

                case    Ticket::Automatic:
                    fparse.addColumn("EntryType", "Automatic: ");
                    break;

                case    Ticket::Audit:
                    fparse.addColumn("EntryType", "Audit: ");
                    break;

                default:
                    fparse.addColumn("EntryType", DB.curRow["EntryType"]);
                    break;
            }

        }
        tmpText.append(fparse.parseFileToMem("tickets/History.bhtml"));
    } else {
        tmpText.append("No log entries were found for this ticket.");
    }
    tmpText.append("</HTML>");
    ticketLog->setText(tmpText);
    QApplication::restoreOverrideCursor();
}


/*
** setLogLevel  - Called whenever the user selects a new loglevel from
**                the dropdown list.
*/

void TicketEditor::setLogLevel(int newLevel)
{
    myLogLevel = newLevel;
    refreshLog();
}

/*
** customerClicked - Called when the user clicks on the Customer button.
**                   This function will emit the OpenCustomer signal.
*/

void TicketEditor::customerClicked()
{
    if (myTicket->customerID()) emit(openCustomer(myTicket->customerID()));
}

/*
** annotateTicket - Brings up a small window where the user can annotate
**                  a ticket.
*/

void TicketEditor::annotateTicket()
{
    TicketLogAnnotater  *tla = new TicketLogAnnotater();
    tla->setTicketNo(myTicket->ticketNo());
    tla->show();
}

/*
** takeTicket - Takes the ticket and makes it our own.
*/

void TicketEditor::takeTicket()
{
    // First, make sure that we can do this...
    if (!strlen(myTicket->owner()) || isAdmin()) {
        myTicket->setOwner(curUser().userName);
        if (myTicket->status() == Ticket::Unexamined ||
            myTicket->status() == Ticket::Unassigned) myTicket->setStatus(Ticket::StaffPending);
        emit ticketUpdated(myTicket->ticketNo());
    }
}

/*
** releaseTicket - Releases the ticket and gives it back to the pool.
*/

void TicketEditor::releaseTicket()
{
    // First, make sure that we can do this...
    if (!strcmp(myTicket->owner(), curUser().userName) || isAdmin()) {
        myTicket->setOwner("");
        emit ticketUpdated(myTicket->ticketNo());
    }
}







/*
** TicketLogAnnotater  - Allows the user to annotate a ticket.
*/

TicketLogAnnotater::TicketLogAnnotater
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("Annotate Ticket");

    myTicket = new Ticket();

    // Create the various widgets.  Most of them will be display only, but
    // we need them all to be there for completeness.
    header = new TicketHeader(this);

    HorizLine *hline1 = new HorizLine(this);

    QLabel  *newStatusLabel = new QLabel(this);
    newStatusLabel->setAlignment(AlignRight|AlignVCenter);
    newStatusLabel->setText("Se&t status:");

    newStatus = new QComboBox(false, this, "New Status");
    newStatus->insertItem("Closed");
    newStatus->insertItem("Staff Pending");
    newStatus->insertItem("Customer Pending");
    newStatus->insertItem("Telco Pending");
    newStatus->insertItem("Vendor Pending");
    newStatusLabel->setBuddy(newStatus);

    // Fill in our status map.  This is a map from the actual ticket
    // status, to those in our list box.
    statusMap[(int) Ticket::Closed]         = 0;
    statusMap[(int) Ticket::Unexamined]     = 1;
    statusMap[(int) Ticket::Unassigned]     = 1;
    statusMap[(int) Ticket::StaffPending]   = 1;
    statusMap[(int) Ticket::CustPending]    = 2;
    statusMap[(int) Ticket::TelcoPending]   = 3;
    statusMap[(int) Ticket::VendorPending]  = 4;
    statusMap[7] = 0;
    statusMap[8] = 0;
    statusMap[9] = 0;

    // Our reverse status map.
    revStatusMap[0] = (int) Ticket::Closed;
    revStatusMap[1] = (int) Ticket::StaffPending;
    revStatusMap[2] = (int) Ticket::CustPending;
    revStatusMap[3] = (int) Ticket::TelcoPending;
    revStatusMap[4] = (int) Ticket::VendorPending;
    revStatusMap[5] = (int) Ticket::StaffPending;

    // Is this an "Urgent" entry?  Urgent entries are always positioned
    // at the top of the log list, and are flagged
    isUrgent = new QCheckBox(this, "Is Urgent Checkbox");
    isUrgent->setText("&Urgent");
    isUrgent->setChecked(false);
    
    QLabel  *noteTextLabel = new QLabel(this);
    noteTextLabel->setAlignment(AlignRight|AlignTop);
    noteTextLabel->setText("C&omments:");

    noteText = new Q3MultiLineEdit(this, "Note Text");
    noteText->setWordWrap(Q3MultiLineEdit::WidgetWidth);
    noteTextLabel->setBuddy(noteText);


    saveNoteButton = new QPushButton(this, "Save Note Button");
    saveNoteButton->setText("&Save");
    connect(saveNoteButton, SIGNAL(clicked()), this, SLOT(saveNote()));

    QPushButton *cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    
    // Now, do the layout for the widget.
    // We'll do a top to bottom box, and then put a grid in the middle of it
    // for the tabular information.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    ml->addWidget(header,   0);
    ml->addWidget(hline1,   0);

    // Create the grid for the tabular information.  We'll do this
    // 2 columns wide.
    Q3GridLayout *tl = new Q3GridLayout(3, 2, 2);
    tl->setColStretch(0, 0);
    tl->setColStretch(1, 1);

    int curRow = 0;
    int curCol = 0;
    
    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(newStatusLabel,   curRow, curCol++);
    tl->addWidget(newStatus,        curRow, curCol++);
    tl->addWidget(isUrgent,         curRow, curCol++);
    
    curRow++; curCol = 0;
    tl->setRowStretch(curRow, 1);
    tl->addWidget(noteTextLabel,    curRow, curCol++);
    tl->addMultiCellWidget(noteText,curRow, curRow, curCol, curCol+1);
    
    
    ml->addLayout(tl, 1);



    // Add in our action buttons.
    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
    abl->addStretch(1);
    abl->addWidget(saveNoteButton, 0);
    abl->addWidget(cancelButton, 0);
    
    ml->addLayout(abl, 0);


    noteText->setFocus();
}

TicketLogAnnotater::~TicketLogAnnotater()
{
}


void TicketLogAnnotater::setTicketNo(long newTicketNo)
{
    myTicket->setTicketNo(newTicketNo);
    header->setTicketNo(newTicketNo);

    char    tmpStr[1024];
    sprintf(tmpStr, "Annotate Ticket (%ld)", myTicket->ticketNo());
    setCaption(tmpStr);

    // Set the current status in the combo box
    if ((int) myTicket->status() > 9) myTicket->setStatus(Ticket::StaffPending);
    prevStatus = myTicket->status();

    newStatus->setCurrentItem(statusMap[(int)myTicket->status()]);

}

/*
** saveNote - Validates the contents of the note and saves it.
*/

void TicketLogAnnotater::saveNote()
{
    QString ts1;
    QString ts2;
    
    ts1 = noteText->text();
    ts2 = ts1.stripWhiteSpace();
    ts1 = ts2.simplifyWhiteSpace();
    
    if (ts1.length()) {
        // If the status has changed, create an automatic note.
        if (prevStatus != revStatusMap[newStatus->currentItem()]) {
            // They changed the status.  Update the ticket and
            // create an automatic log entry.
            myTicket->setStatus((Ticket::t_Status)revStatusMap[newStatus->currentItem()]);
        }
        
        if (isUrgent->isChecked()) {
            myTicket->addUrgentLogEntry(Ticket::Normal, "%s", (const char *)ts1);
        } else {
            myTicket->addLogEntry(Ticket::Normal, "%s", (const char *)ts1);
        }

        emit ticketUpdated(myTicket->ticketNo());
        close();
    } else {
        noteText->setText(ts1);
    }
}


// vim: expandtab
