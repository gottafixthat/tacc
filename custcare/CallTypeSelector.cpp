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

#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QLabel>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3Frame>

#include <ADB.h>
#include <TAAWidget.h>
#include <TAATools.h>
#include <CustomerSearch.h>
#include <CustomerTickets.h>

#include "CallTypeSelector.h"

using namespace Qt;

CallTypeSelector::CallTypeSelector
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{

    Q3ButtonGroup   *bGroup = new Q3ButtonGroup(5, Qt::Horizontal, this, "ButtonGroup");
    bGroup->setFrameStyle(Q3Frame::NoFrame);
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
    hline1->setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    hline1->setLineWidth(1);

    custSearch      = new CustomerSearch(this, "CustomerSearch");
    connect(custSearch, SIGNAL(customerSelected(long)), mainWin(), SLOT(openCustomerMW(long)));
    connect(custSearch, SIGNAL(customerHighlighted(long)), this, SLOT(delayedRefresh(long)));

    contactList     = new Q3ListView(this, "ContactList");
    contactList->addColumn("Name");
    contactList->addColumn("Access");
    contactList->addColumn("Type");
    contactList->addColumn("Skill");
    contactList->setMargin(2);
    contactList->setAllColumnsShowFocus(true);
    
    ticketList      = new CustomerTickets(this, "Ticket List");

    // Create our layout now.  This one is a bit more complicated,
    // but it will look nice.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 2, 2);

    ml->addWidget(bGroup, 0);
    ml->addWidget(hline1, 0);
    ml->addWidget(custSearch, 1);

    Q3BoxLayout  *ll = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
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
            (void) new Q3ListViewItem(contactList, DB.curRow["Name"], DB.curRow["Access"], DB.curRow["Tag"]);
        }
    }

    QApplication::restoreOverrideCursor();
}


// vim: expandtab
