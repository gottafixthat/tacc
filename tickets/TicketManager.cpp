/*
** $Id$
**
***************************************************************************
**
** TicketManager - Overview of its main function.
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

#include "TicketManager.h"
#include "TicketsCommon.h"

#include <qlistview.h>
#include <qdatetm.h>
#include <qlayout.h>
#include <qfont.h>
#include <qtimer.h>

#include <ADB.h>
#include <TAATools.h>

#include "Ticket.h"
#include "TicketEditor.h"

TicketManager::TicketManager
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    // Create a menu for our parent to pull into its own menu bar.
    ticketMenu = new QPopupMenu(this, "Ticket Menu");
    ticketMenu->setCheckable(true);
    showPersonalMenuID = ticketMenu->insertItem("Show Personal", this, SLOT(showPersonalSelected()));
    ticketMenu->setItemChecked(showPersonalMenuID, true);
    showAllPersonalMenuID = -1;
    if (isAdmin()) {
        showAllPersonalMenuID = ticketMenu->insertItem("Show All Personal", this, SLOT(showAllPersonalSelected()));
        ticketMenu->setItemChecked(showAllPersonalMenuID, false);
    }

    ticketList = new QListView(this);
    ticketList->addColumn("Ticket");
    ticketList->addColumn("CustID");
    ticketList->addColumn("Name");
    ticketList->addColumn("Owner");
    ticketList->addColumn("Status");
    ticketList->addColumn("Updated");
    ticketList->addColumn("Summary");
    ticketList->setAllColumnsShowFocus(true);
    ticketList->setMargin(2);

    connect(ticketList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(editTicket(QListViewItem *)));
    connect(ticketList, SIGNAL(returnPressed(QListViewItem *)), this, SLOT(editTicket(QListViewItem *)));

    // Now, do the layout for the widget.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    ml->addWidget(ticketList, 1);

    // Create a timer to refresh the ticket list every 5 minutes
    QTimer  *ticketTimer = new QTimer(this);
    connect(ticketTimer, SIGNAL(timeout()), this, SLOT(refreshTicketList()));
    //ticketTimer->start(300000);        // 5 Minutes
    ticketTimer->start(120000);        // 2 Minutes
    //ticketTimer->start(3000);        // 3 seconds
    refreshTicketList();

    myTicketCount = 0;
    // Ticket  tmpTicket;
    // tmpTicket.setTicketNo(1);
    // tmpTicket.setOpened(0);
    // fprintf(stderr, "Right now = %s, %s, %s\n", tmpTicket.openedStr(), tmpTicket.openedStr(MM_DD_YY_HHa), tmpTicket.openedStr(YYYY_MM_DD_HH_MM_SS));
}


TicketManager::~TicketManager()
{
}

/*
** menu  - Returns the pointer to our QPopupMenu.
*/

QPopupMenu *TicketManager::menu()
{
    return ticketMenu;
}

/*
** ticketCount - Returns the number of items in the ticketList.
*/

int TicketManager::ticketCount()
{
    return myTicketCount;
}

/*
** refreshTicketList - Loads the list of open tickets.
*/

void TicketManager::refreshTicketList()
{
    ADB     DB;
    bool    foundIt;
    int     statusColumn = 7;
    Ticket  tick;
    char    tmpStr[1024];

    // ticketList->clear();

    // Scan through the list and mark all of the entries there
    // as "removable".
    {
        QListViewItemIterator   tmpit(ticketList);
        for (; tmpit.current(); ++tmpit) tmpit.current()->setText(statusColumn,"0");
    }

    myTicketCount = 0;
    DB.query("select TicketNo from Tickets where Status > 0");
    while (DB.getrow()) {
        // See if we have this one in the list already.
        // If we find it, update it.
        tick.setTicketNo(atol(DB.curRow["TicketNo"]));
        foundIt = false;
        QListViewItemIterator   it(ticketList);
        for (; it.current(); ++it) {
            if ((atol(it.current()->key(0,0))) == atol(DB.curRow["TicketNo"])) {
                myTicketCount++;
                foundIt = true;
                // Update the individual columns
                sprintf(tmpStr, "%ld", tick.customerID());
                if (!tick.customerID()) strcpy(tmpStr, "N/A");
                it.current()->setText(1, tmpStr);
                it.current()->setText(2, tick.name());
                it.current()->setText(3, tick.owner());
                it.current()->setText(4, tick.statusStr());
                it.current()->setText(5, tick.modifiedStr(MM_DD_YY_HH_MMa_NA));
                it.current()->setText(6, tick.summary());
                it.current()->setText(statusColumn, "1");
                // The ticket was found in the list, verify that it is
                // not a personal ticket that belongs to someone else.
                if (tick.type() == Ticket::Personal) {
                    // Since its a personal ticket, default to don't show.
                    it.current()->setText(statusColumn, "0");
                    if (ticketMenu->isItemChecked(showPersonalMenuID)) {
                        // We want to show personal tickets, if we are the owner,
                        // its okay to display this one.
                        if (!strcmp(tick.owner(), curUser().userName)) it.current()->setText(statusColumn, "1");
                        // If they are an admin, and want to show all of them
                        // go ahead and display this one.
                        if (isAdmin()) {
                            if (ticketMenu->isItemChecked(showAllPersonalMenuID)) it.current()->setText(statusColumn, "1");
                        }
                    }
                }
            }
        }

        if (!foundIt) {
            // Make sure that this isn't a personal ticket that belongs
            // to someone else.
            bool okayToInsert = true;
            if (tick.type() == Ticket::Personal) {
                okayToInsert = false;
                if (ticketMenu->isItemChecked(showPersonalMenuID)) {
                    // We want to show personal tickets, if we are the owner,
                    // its okay to display this one.
                    if (!strcmp(tick.owner(), curUser().userName)) okayToInsert = true;
                    // If they are an admin, and want to show all of them
                    // go ahead and display this one.
                    if (isAdmin()) {
                        if (ticketMenu->isItemChecked(showAllPersonalMenuID)) okayToInsert = true;
                    }
                }
            }

            if (okayToInsert) {
                sprintf(tmpStr, "%ld", tick.customerID());
                if (!tick.customerID()) strcpy(tmpStr, "N/A");
                (void) new QListViewItem(ticketList,
                        DB.curRow["TicketNo"],
                        tmpStr,
                        tick.name(),
                        tick.owner(),
                        tick.statusStr(),
                        tick.modifiedStr(MM_DD_YY_HH_MMa_NA),
                        tick.summary(),
                        "1"
                        );
            }
        }
    }
    // Now, walk through the list and delete any items that were not
    // in the query result
    QListViewItemIterator   tmpit(ticketList);
    for (; tmpit.current(); ++tmpit) {
        if (!atol(tmpit.current()->key(statusColumn,0))) {
            ticketList->takeItem(tmpit.current());
        }
    }
}

/*
** editTicket - Starts the ticket editor on the current ticket.
*/

void TicketManager::editTicket(QListViewItem *selItem)
{
    if (selItem) {
        TicketEditor    *te = new TicketEditor(0);
        te->setTicketNo(atol(selItem->key(0,0)));
        te->show();
    }
}

/*
** showPersonalSelected - Toggles whether or not we display personal items.
*/

void TicketManager::showPersonalSelected()
{
    if (ticketMenu->isItemChecked(showPersonalMenuID)) {
        ticketMenu->setItemChecked(showPersonalMenuID, false);
        if (isAdmin()) {
            ticketMenu->setItemEnabled(showAllPersonalMenuID, false);
        }
    } else {
        ticketMenu->setItemChecked(showPersonalMenuID, true);
        if (isAdmin()) {
            ticketMenu->setItemEnabled(showAllPersonalMenuID, true);
        }
    }
    refreshTicketList();
}

/*
** showAllPersonalSelected - Toggles whether or not we display everyone's
**                           personal items.
*/

void TicketManager::showAllPersonalSelected()
{
    if (!isAdmin()) return;

    if (ticketMenu->isItemChecked(showAllPersonalMenuID)) {
        ticketMenu->setItemChecked(showAllPersonalMenuID, false);
    } else {
        ticketMenu->setItemChecked(showAllPersonalMenuID, true);
    }
    refreshTicketList();
}
