/*
***************************************************************************
**
** Tab_VoIP - Provides an interface for customer care personnel to add
** VoIP services for customers.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>

#include <qprinter.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qregexp.h>
#include <qapplication.h>
#include <qlayout.h>
#include <qmessagebox.h>

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
) : TAAWidget( parent, name )
{
	setCaption( "VoIP" );

    // Create our widgets.

    list = new QListView(this, "VoIP List");
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
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    ml->addWidget(list, 0);

    HorizLine   *hline1 = new HorizLine(this);
    ml->addWidget(hline1, 0);

    // Now, create a box to hold our action buttons.
    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
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
            QListViewItem *curItem = new QListViewItem(list, DB.curRow["DID"], DB.curRow["LoginID"], "", provMethod, DB.curRow["City"], DB.curRow["State"], DB.curRow["Country"]);

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


