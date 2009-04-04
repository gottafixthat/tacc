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

#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3strlist.h>

#include <BlargDB.h>
#include <BString.h>
#include <ADB.h>

#include <DomainTypeBillablesAdd.h>

using namespace Qt;

DomainTypeBillablesAdd::DomainTypeBillablesAdd(QWidget* parent, const char* name, int DomainTypeID) :
	TAAWidget(parent)
{
	setCaption( "Add Billable Item" );

    // Create our widgets.
    itemList = new Q3ListView(this, "itemList");
    itemList->addColumn("Billable");
    itemList->addColumn("Description");
    itemNumCol = 2;
    itemList->setAllColumnsShowFocus(true);

    QPushButton *addButton = new QPushButton("&Add", this, "addButton");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addBillableItem()));

    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelBillableItemAdd()));

    // Our layout.  Fairly straightforward, one box top to bottom, one for the buttons
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);
    ml->addWidget(itemList, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(cancelButton, 0);
    ml->addLayout(bl, 0);
   
	ADB	DB;
	char	tmpstr[1024];
	
	Q3StrList tmplist;

	if (!DomainTypeID) return;
	
	myDomainTypeID = DomainTypeID;
	
	// Get the list of Billable Item Numbers that are already included.
	DB.query("select ItemNumber from DomainTypeBillables where DomainTypeID = %d", myDomainTypeID);
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
	itemList->clear();
	while (DB.getrow()) {
        (void) new Q3ListViewItem(itemList, DB.curRow["ItemID"], DB.curRow["Description"], DB.curRow["ItemNumber"]);
	}
	
	//addButton->setDefault(TRUE);
	// connect(saveButton, SIGNAL(clicked()), SLOT(accept()));
	//connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    show();
}


DomainTypeBillablesAdd::~DomainTypeBillablesAdd()
{
}


void DomainTypeBillablesAdd::addBillableItem()
{
    Q3ListViewItem   *curItem = itemList->currentItem();
    if (curItem) {
        ADBTable    DTDB("DomainTypeBillables");
        DTDB.setValue("DomainTypeID",   myDomainTypeID);
        DTDB.setValue("ItemNumber",     curItem->key(itemNumCol, 0).toInt());
        DTDB.ins();
        emit(domainTypeBillableAdded(myDomainTypeID));
        delete this;
    }
}

void DomainTypeBillablesAdd::cancelBillableItemAdd()
{
    delete this;
}


// vim: expandtab
