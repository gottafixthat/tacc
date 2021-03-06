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

#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3strlist.h>

#include <ADB.h>
#include <BlargDB.h>
#include <BString.h>

#include "LoginTypeBillablesAdd.h"

using namespace Qt;

LoginTypeBillablesAdd::LoginTypeBillablesAdd
(
	QWidget* parent,
	const char* name,
	int	LoginTypeID
) : QDialog( parent, name, true )
{
	setCaption( "Add Billable Item" );

    // Create our widgets
    itemList = new Q3ListBox(this, "itemList");
    connect(itemList, SIGNAL(doubleClicked(Q3ListBoxItem *)), this, SLOT(listDoubleClicked(Q3ListBoxItem *)));
    //itemList->addColumn("Name");
    //itemList->addColumn("Description");

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(pressed()), this, SLOT(addBillableItem()));
    
    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(pressed()), this, SLOT(cancelBillableItem()));

    // Setup our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(itemList, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

	ADB	    DB;
	char	tmpstr[1024];
	
	Q3StrList tmplist;

	billableIDX = NULL;

	if (!LoginTypeID) return;
	
	myLoginTypeID = LoginTypeID;
	
	// Get the list of Billable Item Numbers that are already included.
	DB.query("select ItemNumber from LoginTypeBillables where LoginTypeID = %d", myLoginTypeID);
	if (DB.rowCount) {
		while (DB.getrow()) {
			tmplist.append(DB.curRow["ItemNumber"]);
		}
		joinString(&tmplist, ",", tmpstr);
		DB.query("select ItemNumber, ItemID, Description from Billables where Active = 1 and ItemNumber not in (%s) order by ItemID", tmpstr);
	} else {
		DB.query("select ItemNumber, ItemID, Description from Billables where Active = 1 order by ItemID");
	}
	
	// Fill the itemList with billable Items.
	billableIDX = new(int[DB.rowCount + 1]);
	int idxPtr = 0;
	itemList->clear();
	while (DB.getrow()) {
		billableIDX[idxPtr] = atoi(DB.curRow["ItemNumber"]);
		sprintf(tmpstr, "%-16s  %s", DB.curRow["ItemID"], DB.curRow["Description"]);
		itemList->insertItem(tmpstr);
		idxPtr++;
	}
	itemList->setCurrentItem(0);
	
	addButton->setDefault(TRUE);
	// connect(saveButton, SIGNAL(clicked()), SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

}

LoginTypeBillablesAdd::~LoginTypeBillablesAdd()
{
	if (billableIDX != NULL) delete(billableIDX);
}

void LoginTypeBillablesAdd::addBillableItem()
{
	ADB		DB;
	
	DB.dbcmd("insert into LoginTypeBillables values (0, %d, %d)", 
	  myLoginTypeID, 
	  billableIDX[itemList->currentItem()]
	);
	done(Accepted);
}

void LoginTypeBillablesAdd::listDoubleClicked(Q3ListBoxItem *)
{
    addBillableItem();
}

void LoginTypeBillablesAdd::cancelBillableItem()
{
    reject();
}

// vim: expandtab
