/*
** $Id: CreateCustTicket.cpp,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** CreateCustTicket - Creates a new ticket for the specified customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include "Ticket.h"
#include "TicketEditor.h"
#include "CreateCustTicket.h"

#include <qmessagebox.h>
#include <qtextview.h>
#include <qdatetm.h>
#include <qheader.h>
#include <qlayout.h>
#include <qcheckbox.h>

#include <TAA.h>
#include <TAATools.h>

#include <ADB.h>
#include <Cfg.h>
#include <FParse.h>


/*
** CreateCustTicket class.  This class allows a user to create a new ticket
** for a customer.
*/

CreateCustTicket::CreateCustTicket
(
	QWidget* parent,
	const char* name,
    long custID
) : TAAWidget( parent, name )
{
    setCaption("Create New Ticket");
    
    // Create the various widgets that allow the user to specify what type of
    // ticket this is.
    
    HorizLine *hline1 = new HorizLine(this);
    
    QLabel  *customerIDLabel = new QLabel(this);
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);
    customerIDLabel->setText("Customer ID:");

    customerIDText = new QLabel(this);
    customerIDText->setAlignment(AlignLeft|AlignVCenter);
    customerIDText->setText("");

    QLabel  *customerNameLabel = new QLabel(this);
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);
    customerNameLabel->setText("Customer Name:");

    customerName = new QLabel(this);
    customerName->setAlignment(AlignLeft|AlignVCenter);
    customerName->setText("");
    
    QLabel  *ticketTypeLabel = new QLabel(this);
    ticketTypeLabel->setAlignment(AlignRight|AlignVCenter);
    ticketTypeLabel->setText("Type:");

    ticketType = new QComboBox(false, this, "TicketType");
    ticketType->insertItem("Support");
    ticketType->insertItem("Accounting");
    ticketType->insertItem("PreSales");
    ticketType->insertItem("Sales");
    ticketType->insertItem("Misc");

    QLabel  *ticketSummaryLabel = new QLabel(this);
    ticketSummaryLabel->setAlignment(AlignRight|AlignVCenter);
    ticketSummaryLabel->setText("Summary:");

    ticketSummary = new QLineEdit(this, "TicketSummary");
    ticketSummary->setMaxLength(80);

    QLabel  *ticketNotesLabel = new QLabel(this);
    ticketNotesLabel->setAlignment(AlignRight|AlignTop);
    ticketNotesLabel->setText("Notes/Detail:");

    ticketNotes = new QTextEdit(this, "TicketNotes");
    ticketNotes->setWordWrap(QTextEdit::WidgetWidth);

    // Now, create the buttons for our actions.
    customerButton = new QPushButton(this, "Customer Button");
    customerButton->setText("Custom&er");
    connect(customerButton, SIGNAL(clicked()), this, SLOT(customerClicked()));

    saveTicketButton = new QPushButton(this, "Save Ticket");
    saveTicketButton->setText("&Save");
    connect(saveTicketButton, SIGNAL(clicked()), this, SLOT(saveTicket()));

    cancelButton = new QPushButton(this, "Close Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    
    // Now, do the layout for the widget.
    // We'll do a top to bottom box, and then put a grid in the middle of it
    // for the tabular information.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // Create a grid layout for the items the user can manipulate
    QGridLayout *tl = new QGridLayout(3, 2, 2);
    tl->setColStretch(0, 0);
    tl->setColStretch(1, 1);

    int curRow = -1;
    int curCol = 0;

    curRow++;
    curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(customerIDLabel,      curRow, curCol++);
    tl->addWidget(customerIDText,       curRow, curCol++);

    curRow++;
    curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(customerNameLabel,    curRow, curCol++);
    tl->addWidget(customerName,         curRow, curCol++);

    curRow++;
    curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(ticketTypeLabel,      curRow, curCol++);
    tl->addWidget(ticketType,           curRow, curCol++);

    curRow++;
    curCol = 0;
    tl->setRowStretch(curRow, 0);
    tl->addWidget(ticketSummaryLabel,   curRow, curCol++);
    tl->addWidget(ticketSummary,        curRow, curCol++);

    curRow++;
    curCol = 0;
    tl->setRowStretch(curRow, 1);
    tl->addWidget(ticketNotesLabel,     curRow, curCol++);
    tl->addWidget(ticketNotes,          curRow, curCol++);

    ml->addLayout(tl, 1);
    ml->addWidget(hline1, 0);

    // Add in our action buttons.
    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    abl->addStretch(1);
    abl->addWidget(customerButton, 0);
    abl->addWidget(saveTicketButton, 0);
    abl->addWidget(cancelButton, 0);
    
    ml->addLayout(abl, 0);

    // Make the window big enough to see several ticket notes.
    resize(sizeHint().width()*2, sizeHint().height()*1.5);

    if (custID) setCustomerID(custID);
    else myCustID = 0;
}


CreateCustTicket::~CreateCustTicket()
{
}


/*
** setCustomerID - Sets the customer ID that we're working on.
*/

void CreateCustTicket::setCustomerID(long custID)
{
    ADBTable    custDB("Customers");
    myCustID = custID;

    custDB.get(myCustID);
    customerIDText->setText(custDB.getStr("CustomerID"));
    customerName->setText(custDB.getStr("FullName"));
}

/*
** saveTicket - Saves the ticket that the user is working on.
*/

void CreateCustTicket::saveTicket()
{
    Ticket  newTick;
    newTick.setCustomerID(myCustID);
    newTick.setName((const char *)customerName->text());
    newTick.setSummary((const char *)ticketSummary->text());
    newTick.setOpened();
    newTick.setModified();
    newTick.setStatus(Ticket::Unexamined);
    newTick.setType((Ticket::t_Type)ticketType->currentItem());
    newTick.create();
    newTick.addLogEntry(Ticket::Normal, "%s", (const char *) ticketNotes->text());
    close();
}

/*
** customerClicked - Opens the customer window.
*/

void CreateCustTicket::customerClicked()
{
    if (myCustID) emit(openCustomer(myCustID));
}

/*
** cancelClicked - If there is text in the ticketNotes, it will prompt
**                 the user to make sure they want to cancel the ticket
**                 creation.
*/

void CreateCustTicket::cancelClicked()
{
    bool    okayToClose = true;
    if (ticketNotes->text().length() || ticketSummary->text().length()) {
        if (QMessageBox::warning(this, "Create Ticket", "Are you sure you want to abort this ticket?", "&Yes", "&No", 0, 1) == 1) {
            okayToClose = false;
        }
    }
    if (okayToClose) close();
}
