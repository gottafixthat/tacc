/*
** $Id: ExtendGraceDate.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** ExtendGraceDate - Gives the operator the ability to extend the grace
**                   date of a customer.
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
** $Log: ExtendGraceDate.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "ExtendGraceDate.h"
#include "BlargDB.h"

#include <ADB.h>

#include <stdio.h>
#include <stdlib.h>

#include <TAATools.h>

#define Inherited ExtendGraceDateData

ExtendGraceDate::ExtendGraceDate
(
	QWidget* parent,
	const char* name,
	long CustomerID
)
	:
	Inherited( parent, name )
{
    myCustID = CustomerID;
    
    CustomersDB     CDB;
    CDB.get(myCustID);
    newGrace = origGrace = CDB.getDate("GraceDate");
    daysChanged(1);

    char    tmpStr[1024];
    sprintf(tmpStr, "Extending grace period for %s", (const char *) CDB.getStr("FullName"));
    titleLabel->setText(tmpStr);
    setCaption(tmpStr);
    
    extendDays->setFocus();

    currentGraceDate->setText(CDB.getDate("GraceDate").toString());

    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
}


ExtendGraceDate::~ExtendGraceDate()
{
}


/*
** daysChanged - Gets called when the user changes something on the
**               spinbox.
*/

void ExtendGraceDate::daysChanged(int newDays)
{
    newGrace = origGrace.addDays(newDays);
    newGraceDate->setText(newGrace.toString());
}

/*
** cancelGrace - Gets called when the user hits the cancle button.
*/

void ExtendGraceDate::cancelGrace()
{
    close();
}

/*
** saveGrace - Gets called when the user hits the cancle button.
*/

void ExtendGraceDate::saveGrace()
{
    CustomersDB     CDB;
    CDB.get(myCustID);
    
    char    tmpStr[1024];
    sprintf(tmpStr, "%04d-%02d-%02d", newGrace.year(), newGrace.month(), newGrace.day());

    CDB.setValue("GraceDate", tmpStr);
    CDB.upd();

    NotesDB NDB;
    NDB.setValue("CustomerID", myCustID);
    NDB.setValue("LoginID", CDB.getStr("PrimaryLogin"));
    NDB.setValue("NoteType", "Accounting");
    sprintf(tmpStr, "Extended grace period to %s (%d days)", (const char *) newGrace.toString(), origGrace.daysTo(newGrace));
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();

    emit(customerChanged(myCustID));
    close();
}

