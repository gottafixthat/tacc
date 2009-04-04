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

#include <stdlib.h>
#include <stdio.h>

#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3strlist.h>
#include <Qt3Support/Q3BoxLayout>

#include <FParse.h>

#include <CustCallLogReport.h>
#include "CustCallDetail.h"
#include "Tab_VoIP.h"
#include "BlargDB.h"
#include "BString.h"
#include "NoteEdit.h"
#include <ADB.h>

#include <TAA.h>
#include <TAATools.h>
#include <Ticket.h>
#include <Cfg.h>

#define NOTEIDCOL   4
#define NOTETYPECOL 5

Tab_VoIP::Tab_VoIP
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget(parent)
{
	setCaption( "VoIP" );

    // Create our widgets.

    list = new Q3ListView(this, "VoIP List");
    list->addColumn("DID");
    list->addColumn("Login");
    list->addColumn("Service");
    list->addColumn("Type");
    list->addColumn("City");
    list->addColumn("State");
    list->addColumn("Country");
    list->setAllColumnsShowFocus(true);
    list->setRootIsDecorated(true);
	list->setSorting(0, false);

    // Create our action buttons.
    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addVoIPService()));

    // Create the main layout to hold them all.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    ml->addWidget(list, 0);

    HorizLine   *hline1 = new HorizLine(this);
    ml->addWidget(hline1, 0);

    // Now, create a box to hold our action buttons.
    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
    abl->addStretch(1);
    abl->addWidget(addButton, 0);

    ml->addLayout(abl, 0);
    // Done.

	myCustID = CustID;
	
	refreshVoIPList(0);
}


Tab_VoIP::~Tab_VoIP()
{
}


/*
** refreshVoIPList - A public slot which will get called when an item
**					   in the VoIP list is changed.
**
*/

void Tab_VoIP::refreshVoIPList(int)
{
    ADB             DB;
    char            provMethod[1024];

    list->clear();

    DB.query("select DID_Inventory.DID, DID_Inventory.CustomerID, DID_Inventory.LoginID, DID_Inventory.ProvisioningMethod, DID_Rate_Centers.City, DID_Rate_Centers.State, DID_Rate_Centers.Country from DID_Inventory, DID_Rate_Centers where DID_Inventory.CustomerID = %ld and DID_Rate_Centers.RateCenterID = DID_Inventory.RateCenterID", myCustID);
    if (DB.rowCount) {
        while (DB.getrow()) {
            switch(atoi(DB.curRow["ProvisioningMethod"])) {
                case 0:
                    strcpy(provMethod, "IAX");
                    break;

                case 1:
                    strcpy(provMethod, "SIP");
                    break;

                default:
                    strcpy(provMethod, "Unknown");
                    break;
            }
            Q3ListViewItem *curItem = new Q3ListViewItem(list, DB.curRow["DID"], DB.curRow["LoginID"], "", provMethod, DB.curRow["City"], DB.curRow["State"], DB.curRow["Country"]);

        }
    }

}

/**
  * refreshCustomer - Gets called automatically by the system when a customer
  * is changed.  We should re-scan the VoIP database and refresh the list.
  */
void Tab_VoIP::refreshCustomer(long custID)
{
    if (custID == myCustID) refreshVoIPList(1);
}


/**
  * addVoIPService - Gets called when the CSR wants to Add VoIP service
  */
void Tab_VoIP::addVoIPService()
{
    QMessageBox::information(this, "Add Service", "Coming soon.");
}



// vim: expandtab
