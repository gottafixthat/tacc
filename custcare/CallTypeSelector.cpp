/*
** $Id$
**
***************************************************************************
**
** CallTypeSelector - A set of widgets that determines what type of 
**                    incoming call is being taken.
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
** $Log: CallTypeSelector.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CallTypeSelector.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapp.h>
#include <qvbuttongroup.h>
#include <qlayout.h>
#include <qwidget.h>
#include <qradiobutton.h>
#include <qlistview.h>
#include <qlabel.h>

#include <TAAWidget.h>
#include <TAATools.h>
#include <CustomerSearch.h>
#include <CustomerTickets.h>

#include <ADB.h>

CallTypeSelector::CallTypeSelector
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    QButtonGroup   *bGroup = new QButtonGroup(5, Qt::Horizontal, this, "ButtonGroup");
    bGroup->setFrameStyle(QFrame::NoFrame);
    // bGroup->setTitle("Call Type");

    typeSupport     = new QRadioButton(bGroup, "Support");
    typeSales       = new QRadioButton(bGroup, "Sales");
    // typeAddService  = new QRadioButton(bGroup, "AddService");
    typeAccounting  = new QRadioButton(bGroup, "Accounting");
    typeTicket      = new QRadioButton(bGroup, "Ticket");
    typeUnsolicited = new QRadioButton(bGroup, "Unsolicted");
    typeSupport->setText("&Support");
    typeSales->setText("S&ales");
    // typeAddService->setText("A&dd Service");
    typeAccounting->setText("A&ccounting");
    typeTicket->setText("T&icket");
    typeUnsolicited->setText("&Unsolicited");

    QLabel *hline1 = new QLabel(this);
    hline1->setMinimumSize(0, 3);
    hline1->setMaximumSize(32767, 3);
    hline1->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    hline1->setLineWidth(1);

    custSearch      = new CustomerSearch(this, "CustomerSearch");
    connect(custSearch, SIGNAL(customerSelected(long)), mainWin(), SLOT(openCustomerMW(long)));
    connect(custSearch, SIGNAL(customerHighlighted(long)), this, SLOT(delayedRefresh(long)));

    contactList     = new QListView(this, "ContactList");
    contactList->addColumn("Name");
    contactList->addColumn("Access");
    contactList->addColumn("Type");
    contactList->addColumn("Skill");
    contactList->setMargin(2);
    contactList->setAllColumnsShowFocus(true);
    
    ticketList      = new CustomerTickets(this, "Ticket List");

    // Create our layout now.  This one is a bit more complicated,
    // but it will look nice.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 2, 2);

    ml->addWidget(bGroup, 0);
    ml->addWidget(hline1, 0);
    ml->addWidget(custSearch, 1);

    QBoxLayout  *ll = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    ll->addWidget(contactList, 1);
    ll->addWidget(ticketList,  1);

    ml->addLayout(ll, 1);


    custSearch->takeFocus();

    // Setup for our delayed refreshes.
    refreshCustID = 0;
    refreshTimer = new QTimer(this, "Refresh Timer");
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));

}



CallTypeSelector::~CallTypeSelector()
{
}

/*
** delayedRefresh  - Starts a timer that refreshes the contact and
**                   ticket lists.  If the Customer ID passed is zero
**                   we will stop any pending timer and simply
**                   clear the lists.
*/

void CallTypeSelector::delayedRefresh(long custID)
{
    refreshCustID = custID;
    if (refreshTimer->isActive()) refreshTimer->stop();
    if (!custID) {
        contactList->clear();
        ticketList->setCustomerID(custID);
    } else {
        refreshTimer->start(750, true);     // 3/4 of a second, one time.
    }
}

/*
** refresh - Refreshes the customer contact list and open ticket list.
*/

void CallTypeSelector::refresh()
{
    ticketList->setCustomerID(refreshCustID);
    contactList->clear();

    if (!refreshCustID) return;

    ADB     DB;

    QApplication::setOverrideCursor(waitCursor);

    // Get the list of contacts for this customer and display them
    DB.query("select * from Contacts where CustomerID = %ld", refreshCustID);
    if (DB.rowCount) {
        while (DB.getrow()) {
            (void) new QListViewItem(contactList, DB.curRow["Name"], DB.curRow["Access"], DB.curRow["Tag"]);
        }
    }

    QApplication::restoreOverrideCursor();
}

