/*
** $Id$
**
***************************************************************************
**
** DomainTypeBillablesAdd - ??
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DomainTypeBillablesAdd.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "DomainTypeBillablesAdd.h"
#include "BlargDB.h"
#include "BString.h"
#include <stdio.h>
#include <stdlib.h>
#include <qstrlist.h>
#include <ADB.h>

#define Inherited DomainTypeBillablesAddData

DomainTypeBillablesAdd::DomainTypeBillablesAdd
(
	QWidget* parent,
	const char* name,
	int DomainTypeID
)
	:
	Inherited( parent, name )
{
	setCaption( "Add Billable Item" );

	ADB	DB;
	char	tmpstr[1024];
	
	QStrList tmplist;

	billableIDX = NULL;

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


DomainTypeBillablesAdd::~DomainTypeBillablesAdd()
{
    if (billableIDX != NULL) delete(billableIDX);
}


void DomainTypeBillablesAdd::addBillableItem()
{
	ADB     DB;
	
	DB.dbcmd("insert into DomainTypeBillables values (0, %d, %d)", 
	  myDomainTypeID, 
	  billableIDX[itemList->currentItem()]
	);
	done(Accepted);
}

