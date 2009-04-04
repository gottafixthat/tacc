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

#include <QtGui/QMessageBox>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3buttongroup.h>

#include <BlargDB.h>
#include <ADB.h>

#include "BI_Main.h"

using namespace Qt;

BI_Main::BI_Main
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
	setCaption( "Edit Billables" );

    // Create our widgets.
    billableList = new Q3ListView(this, "billableList");
    billableList->addColumn("Billables");
    billableList->addColumn("Act");
    billableList->addColumn("Pri");
    billableList->setColumnAlignment(1, AlignCenter);
    billableList->setColumnAlignment(2, AlignRight);
    billableList->setAllColumnsShowFocus(TRUE);
    connect(billableList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(billableSelected(Q3ListViewItem *)));

    newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newBillable()));

    deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteBillable()));

    theTabBar = new QTabBar(this);
    theTabBar->setMinimumSize(0, 25);
    theTabBar->setMaximumSize(4096, 25);
    theTabBar->addTab("&General");
    theTabBar->addTab("&Pricing");
    theTabBar->addTab("&Measurement");

    Q3ButtonGroup *bGroup = new Q3ButtonGroup(this, "bGroup");

    qws = new Q3WidgetStack(bGroup, "WidgetStack");
    connect(theTabBar, SIGNAL(selected(int)), qws, SLOT(raiseWidget(int)));

    // 'qws' is our QWidgetStack
    tgeneral = new BI_General(qws);
    qws->addWidget(tgeneral, 0);
    connect(tgeneral, SIGNAL(itemChanged()), SLOT(updateBillable()));
    
    tprices  = new BI_Prices(qws);
    qws->addWidget(tprices, 1);

    tmeasurement = new BI_Measurement(qws);
    qws->addWidget(tmeasurement, 2);

    // Finally, our close button
    closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    // Create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3BoxLayout *wl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    wl->addWidget(billableList, 0);

    // The right side layout
    Q3BoxLayout *rsl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
    rsl->addWidget(theTabBar, 0);

    // The button group to keep things looking pretty
    Q3BoxLayout *bgl = new Q3BoxLayout(bGroup, Q3BoxLayout::TopToBottom, 2);
    bgl->addWidget(qws, 1);
    rsl->addWidget(bGroup, 1);
    wl->addLayout(rsl, 1);
    ml->addLayout(wl, 1);

    // Our buttons
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addWidget(newButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);
    // Done with the layout.  Ugly, ugly, ugly.  But it looks nice.

    loadBillables();
}


BI_Main::~BI_Main()
{
}

/*
** loadBillables   - The first thing that gets called.  It fills the billable
**                   list with the available billable items.
*/

void BI_Main::loadBillables()
{
    ADB         DB;
    char        activeStr[1024];
    char        priorityStr[1024];

    billableList->clear();

    DB.query("select ItemNumber, ItemID, Active, Priority from Billables");

    if (DB.rowCount) while (DB.getrow()) {
        if (atoi(DB.curRow["Active"])) {
            sprintf(activeStr, "Y");
        } else {
            sprintf(activeStr, "N");
        }

        sprintf(priorityStr, "%5d", atoi(DB.curRow["Priority"]));

        (void) new Q3ListViewItem(billableList, 
          DB.curRow["ItemID"], 
          activeStr, 
          priorityStr,
          DB.curRow["ItemNumber"]
        );
    }
}

/*
** billableSelected - When an item from the list is selected, this function
**                    fills the various tabs with the information from it.
*/

void BI_Main::billableSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        tgeneral->setItemNumber(atol(curItem->key(3,0)));
        tprices->setItemNumber(atol(curItem->key(3,0)));
        tmeasurement->setItemNumber(atol(curItem->key(3,0)));
    }
}

/*
** newBillable    - Called when the user click on the New button.  It inserts
**                  a new billable record into the database and sets it
**                  to be the currently selected billable item.
*/

void BI_Main::newBillable()
{
    Q3ListViewItem   *newItem;
    ADBTable        BIDB;
    BIDB.setTableName("Billables");

    BIDB.setValue("ItemNumber",     (long) 0);
    BIDB.setValue("ItemID",         "New Billable");
    BIDB.setValue("Description",    "New Billable Item");
    BIDB.setValue("ItemType",       0);
    BIDB.setValue("IntAccountNo",   0);
    BIDB.setValue("Taxable",        0);
    BIDB.setValue("Priority",       0);
    BIDB.setValue("Active",         0);
    BIDB.setValue("SetupItem",      0);

    BIDB.ins();

    newItem = new Q3ListViewItem(billableList,
      BIDB.getStr("ItemID"),
      "N",
      "    0",
      BIDB.getStr("ItemNumber")
    );
    billableList->setSelected(newItem, TRUE);
    billableList->setCurrentItem(newItem);
}

/*
** deleteBillable - Called when the user clicks on the Delete button.
**                  It checks for this billable being in use in the
**                  various tables and lets the user know if they can
**                  delete it or not.  If they can, it prompts them to
**                  verify.
*/

void BI_Main::deleteBillable()
{
	ADB     DB;
	char	tmpstr[1024];
	int		itemNo = 0;
	int		count = 0;
	Q3ListViewItem   *curItem = NULL;
	
	curItem = billableList->currentItem();
	if (curItem != NULL) itemNo = atoi(curItem->key(3,0));
		
	if (itemNo) {
		// First, we must ensure that the item is not in use by anything else
		// Bad Things(tm) would happen if we were to blindly delete an item
		// that was in use in a customer record.  Like we would lose money.
		
		DB.query("select InternalID from Subscriptions where ItemNumber = %d", itemNo);
		count += DB.rowCount;
		
		DB.query("select TransID from RegisterSplits where ItemNumber = %d", itemNo);
		count += DB.rowCount;

		if (count) {
			sprintf(tmpstr, "Unable to delete Billable Item.\n\nIt has been used %d times in other tables.", count);
            QMessageBox::critical(this, "Unable to Delete Billable Item", tmpstr);
		} else {
			DB.query("select ItemID from Billables where ItemNumber = %d", itemNo);
			DB.getrow();
			sprintf(tmpstr, "Are you sure you wish to delete the\nBillable Item '%s'?", DB.curRow["ItemID"]);
            if (QMessageBox::warning(this, "Delete Billable Item", tmpstr, "&Yes", "&No", 0, 1) == 0) {
				DB.dbcmd("delete from Billables where ItemNumber = %d", itemNo);
				DB.dbcmd("delete from BillablesData where ItemNumber = %d", itemNo);
                loadBillables();
                tgeneral->setItemNumber(0);
                tprices->setItemNumber(0);
                tmeasurement->setItemNumber(0);
			}
		}
	}
}

/*
** updateBillable - Gets called when we need to update something in the list
**                  it does it "in place" without reloading all of the
**                  billables.
*/

void BI_Main::updateBillable()
{
    Q3ListViewItem   *curItem    = billableList->currentItem();
    if (curItem == NULL) return;
    
    ADBTable    BIDB;
    char        tmpActive[1024];
    char        tmpPriority[1024];
    BIDB.setTableName("Billables");
    BIDB.get(atol(curItem->key(3,0)));
    curItem->setText(0, BIDB.getStr("ItemID"));

    if (BIDB.getInt("Active")) strcpy(tmpActive, "Y");
    else strcpy(tmpActive, "N");
    curItem->setText(1, tmpActive);

    sprintf(tmpPriority, "%5d", BIDB.getInt("Priority"));
    curItem->setText(2, tmpPriority);
}



// vim: expandtab
