/*
** $Id: BillingCycleEdit.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** BillingCycleEdit - THe Billing Cycle Editor
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
** $Log: BillingCycleEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "BillingCycleEdit.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>
#include <qmessagebox.h>

#define Inherited BillingCycleEditData

BillingCycleEdit::BillingCycleEdit
(
	QWidget* parent,
	const char* name,
	const char* CycleID
)
	:
	Inherited( parent, name )
{
    strcpy(myCycleID, CycleID);    
    
    if (strlen(CycleID)) {
    	setCaption("Edit Billing Cycle");
    } else {
        setCaption("New Billing Cycle");
    }

    EditingCycle = 0;
    cycleID->setFocus();
    // We're editing one, so get the data for it.
    if (strlen(CycleID)) {
        EditingCycle = 1;
        BillingCyclesDB BCDB;
        BCDB.getByCycleID(CycleID);
        cycleID->setText(BCDB.CycleID);
        cycleID->setEnabled(FALSE);
        description->setText(BCDB.Description);
        january->setChecked(BCDB.Jan.toInt());
        february->setChecked(BCDB.Feb.toInt());
        march->setChecked(BCDB.Mar.toInt());
        april->setChecked(BCDB.Apr.toInt());
        may->setChecked(BCDB.May.toInt());
        june->setChecked(BCDB.Jun.toInt());
        july->setChecked(BCDB.Jul.toInt());
        august->setChecked(BCDB.Aug.toInt());
        september->setChecked(BCDB.Sep.toInt());
        october->setChecked(BCDB.Oct.toInt());
        november->setChecked(BCDB.Nov.toInt());
        december->setChecked(BCDB.Dece.toInt());
        day->setText(BCDB.Day);
        defaultCycle->setChecked(BCDB.DefaultCycle.toInt());
        description->setFocus();
    }
}

BillingCycleEdit::~BillingCycleEdit()
{
}


// The user clicked on the close button...

void BillingCycleEdit::cancelCycle()
{
    close();
    delete this;
}

// The user clicked on the save button...

void BillingCycleEdit::saveCycle()
{
    BillingCyclesDB BCDB;
    int             Unique = 1;

    // First things first.  Check to see if we're editing one or creating 
    // a new one.  If we're creating a new one, make sure that the CycleID
    // is unique.
    if (!EditingCycle) {
        // We're Creating a new one, verify the CycleID is unique.
        QString tmpStr = cycleID->text();
        tmpStr.stripWhiteSpace();
        tmpStr.truncate(16);
        if (BCDB.getByCycleID(tmpStr)) {
            Unique = 0;
            QMessageBox::critical(this, "Save Billing Cycle", "The billing cycle could not be saved.\nThe Cycle ID must be unique.");
        }
    } else {
    	BCDB.getByCycleID(cycleID->text());
    }
    
    if (Unique) {
        BCDB.CycleID     = cycleID->text();
        BCDB.Description = description->text();
        BCDB.Jan.setNum((int)january->isChecked());
        BCDB.Feb.setNum((int)february->isChecked());
        BCDB.Mar.setNum((int)march->isChecked());
        BCDB.Apr.setNum((int)april->isChecked());
        BCDB.May.setNum((int)may->isChecked());
        BCDB.Jun.setNum((int)june->isChecked());
        BCDB.Jul.setNum((int)july->isChecked());
        BCDB.Aug.setNum((int)august->isChecked());
        BCDB.Sep.setNum((int)september->isChecked());
        BCDB.Oct.setNum((int)october->isChecked());
        BCDB.Nov.setNum((int)november->isChecked());
        BCDB.Dece.setNum((int)december->isChecked());
        BCDB.Day = day->text();
        BCDB.DefaultCycle.setNum((int)defaultCycle->isChecked());
        if (EditingCycle) {
            BCDB.upd();
        } else {
            // Last check.  Make sure the user put something in the CycleID
            if (!BCDB.CycleID.length()) {
                QMessageBox::critical(this, "Save Billing Cycle", "The Billing Cycle ID must not be blank.");
                return;
            } else {
                BCDB.ins();
            }
        }
    }
    emit refresh(1);
    close();
    delete this;
}

// A public slot for saving...

void BillingCycleEdit::refreshCycleList(int)
{
}

