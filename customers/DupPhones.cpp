/*
** $Id: DupPhones.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** DupPhones - Brings up a duplicate phone number window.
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
** $Log: DupPhones.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "DupPhones.h"
#include "EditCustomer.h"
#include <stdio.h>
#include <stdlib.h>
#include <qstrlist.h>
#include <qstring.h>

#define Inherited DupPhonesData

DupPhones::DupPhones
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Duplicate Phone Numbers" );
	dupeList->setFocus();
	
	custIDIndex = new QStrList(TRUE);
}


DupPhones::~DupPhones()
{
}


/*
** AddDupeEntry - Allows the calling function to add items to the duplicate
**                phone number customer list.
*/

void DupPhones::AddDupeEntry(long CustID, const char * FullName, const char * DayPhone, float Balance)
{
	char tmpstr[512];
	sprintf(tmpstr, "%-36s %-16s %12.2f", FullName, DayPhone, Balance);
	dupeList->insertItem(tmpstr);
	sprintf(tmpstr, "%ld", CustID);
	custIDIndex->append(tmpstr);
	dupeList->setCurrentItem(0);
}


/*
** viewDupCustomer -	Brings up the customer editor for the currently
**						selected customer in the duplicate list.
**
*/

void DupPhones::viewDupCustomer()
{
	QString tmpqstr;
	uint tmpIdx = dupeList->currentItem();
	tmpqstr = custIDIndex->at(tmpIdx);
	if (tmpqstr.toLong()) {
		EditCustomer *custEdit;
		custEdit = new EditCustomer(0, "", tmpqstr.toLong());
		custEdit->show();
	}
}


/*
** viewDupCustomerL -	Brings up the customer editor for the list item that
**						the user double clicks on.
**
*/

void DupPhones::viewDupCustomerL(int)
{
	QString tmpqstr;
	uint tmpIdx = dupeList->currentItem();
	tmpqstr = custIDIndex->at(tmpIdx);
	if (tmpqstr.toLong()) {
		EditCustomer *custEdit;
		custEdit = new EditCustomer(0, "", tmpqstr.toLong());
		custEdit->show();
	}
}
