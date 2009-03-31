/*
** $Id$
**
***************************************************************************
**
** CustomerTickets - Overview of its main function.
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
** $Log: CustomerTickets.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CustomerTickets.h"

#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3BoxLayout>

#include <stdio.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <q3header.h>
#include <qlayout.h>
#include <qtimer.h>

#include <ADB.h>

#include <Ticket.h>

CustomerTickets::CustomerTickets
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    ticketList = new Q3ListView(this);
    ticketList->addColumn("Ticket");
    ticketList->addColumn("Status");
    ticketList->addColumn("Owner");
    ticketList->addColumn("Issue");
    ticketList->setAllColumnsShowFocus(true);
    ticketList->setMargin(2);
    connect(ticketList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(ticketSelected(Q3ListViewItem *)));
    connect(ticketList, SIGNAL(returnPressed(Q3ListViewItem *)), this, SLOT(ticketSelected(Q3ListViewItem *)));

    // Now, do the layout for the widget.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 0, 3);
    ml->addWidget(ticketList, 1);

    // Create a timer to refresh the ticket list every 5 minutes
    QTimer  *ticketTimer = new QTimer(this);
    connect(ticketTimer, SIGNAL(timeout()), this, SLOT(refreshTicketList()));
    // ticketTimer->start(300000);        // 5 Minutes
    ticketTimer->start(3000);        // 3 seconds
    refreshTicketList();
    myCustomerID = 0;
    myHideClosed = 1;
}


CustomerTickets::~CustomerTickets()
{
}

/*
** refreshTicketList - Loads the list of open tickets.
*/

void CustomerTickets::refreshTicketList()
{
    ADB     DB;
    Ticket  curTicket;
    bool    foundIt;

    // ticketList->clear();

    // Scan through the list and mark all of the entries there
    // as "removable".
    {
        Q3ListViewItemIterator   tmpit(ticketList);
        for (; tmpit.current(); ++tmpit) tmpit.current()->setText(4,"0");
    }

    if (myCustomerID) {
        int startStatus = 0;
        if (myHideClosed) startStatus = -1;
        DB.query("select TicketNo from Tickets where Status > %d and CustomerID = %ld", startStatus, myCustomerID);
        while (DB.getrow()) {
            // See if we have this one in the list already.
            // If we find it, update it.
            curTicket.setTicketNo(atol(DB.curRow["TicketNo"]));
            foundIt = false;
            Q3ListViewItemIterator   it(ticketList);
            for (; it.current(); ++it) {
                if ((atol(it.current()->key(0,0))) == atol(DB.curRow["TicketNo"])) {
                    foundIt = true;
                    // Update the individual columns
                    it.current()->setText(1, curTicket.statusStr());
                    it.current()->setText(2, curTicket.owner());
                    it.current()->setText(3, curTicket.summary());
                    it.current()->setText(4, "1");
                }
            }

            if (!foundIt) {
                (void) new Q3ListViewItem(ticketList,
                        DB.curRow["TicketNo"],
                        curTicket.statusStr(),
                        curTicket.owner(),
                        curTicket.summary(),
                        "1"
                        );
            }
        }
    }
    // Now, walk through the list and delete any items that were not
    // in the query result
    Q3ListViewItemIterator   tmpit(ticketList);
    for (; tmpit.current(); ++tmpit) {
        if (!atol(tmpit.current()->key(4,0))) {
            ticketList->takeItem(tmpit.current());
        }
    }
}

/*
** setCustomerID - Sets the customer ID for this list.
*/

void CustomerTickets::setCustomerID(long newCustID)
{
    myCustomerID = newCustID;
    refreshTicketList();
    // else our built in timer will refresh the list.
}

/*
** ticketSelected - Gets called when a user presses enter or double clicks
**                  on a ticket.
*/

void CustomerTickets::ticketSelected(Q3ListViewItem *curItem)
{
    if (curItem) {
        emit openTicket(atol(curItem->key(0,0)));
    }
}

