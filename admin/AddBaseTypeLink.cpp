/*
** $Id: AddBaseTypeLink.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** AddBaseTypeLink - ???
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
** $Log: AddBaseTypeLink.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "AddBaseTypeLink.h"
#include "BlargDB.h"

#include <stdlib.h>
#include <stdio.h>
#include <ADB.h>

#define Inherited AddBaseTypeLinkData

AddBaseTypeLink::AddBaseTypeLink
(
	QWidget* parent,
	const char* name,
	int ItemNo
)
	:
	Inherited( parent, name )
{
	setCaption( "Add Base Type Link" );
	
	myItemNo = ItemNo;
	
	if (!myItemNo) reject();
	
	ADB DB;
	
	DB.query("select BaseType, Description from BaseTypes order by Description");
	baseItemIDX = new(int[DB.rowCount+1]);
	int idxPtr = 0;
	while (DB.getrow()) {
		baseItemIDX[idxPtr] = atoi(DB.curRow["BaseType"]);
		baseItemList->insertItem(DB.curRow["Description"]);
		idxPtr++;
	}
	baseItemList->setCurrentItem(0);
	baseItemList->setFocus();
	
	acceptButton->setDefault(TRUE);
	connect(acceptButton, SIGNAL(clicked()), SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}


AddBaseTypeLink::~AddBaseTypeLink()
{
	delete(baseItemIDX);
}

void AddBaseTypeLink::itemSelected(int itemNo)
{
	// We want to insert the database item here.  We have the myItemNo, and
	// we also have the BaseItemNo.
	// We should also check to be sure we don't already have this Base
	// type in our list already.  No good way of doing that before hand, but
	// I'm sure that I'll have to figure out a way...
	ADB DB;
	DB.dbcmd("insert into BaseTypeLinks values (0, %d, %d)", myItemNo, baseItemIDX[baseItemList->currentItem()]);
	accept();
}

void AddBaseTypeLink::acceptSelected()
{
	itemSelected(baseItemList->currentItem());
}
