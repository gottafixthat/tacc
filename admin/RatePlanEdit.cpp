/*
** $Id$
**
***************************************************************************
**
** RatePlanEdit - Lets the user edit a rateplan.
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
** $Log: RatePlanEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "RatePlanEdit.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <ADB.h>
#include <qmessagebox.h>

#define Inherited RatePlanEditData

RatePlanEdit::RatePlanEdit
(
	QWidget* parent,
	const char* name,
	long IntID
)
	:
	Inherited( parent, name )
{
	ADB         DB;
	RatePlansDB RPDB;

	myIntID = IntID;
	planIndex = NULL;
	
	// Fill in the combo box with the plans we can convert to.
	DB.query("select InternalID, Description from RatePlans where InternalID <> %ld and AutoConvert = 0", myIntID);
	if (DB.rowCount) {
		planIndex = new(long[DB.rowCount +1]);
		int indexPtr = 0;
		while(DB.getrow()) {
			planIndex[indexPtr++] = atol(DB.curRow["InternalID"]);
			convertToList->insertItem(DB.curRow["Description"]);
		}
	} else {
		autoConvertDays->setEnabled(FALSE);
		convertDaysLabel->setEnabled(FALSE);
		autoConvertDate->setEnabled(FALSE);
	}
	
	if (myIntID) {
		setCaption( "Edit Rate Plan" );
		RPDB.get(myIntID);
		planTag->setText(RPDB.getStr("PlanTag"));
		description->setText(RPDB.getStr("Description"));
		switch (RPDB.getInt("AutoConvert")) {
			case 1:
				noAutoConvert->setChecked(FALSE);
				autoConvertDays->setChecked(TRUE);
				convertDays->setText(RPDB.getStr("ConvertDays"));
				promoPlan->setChecked(RPDB.getInt("PromoPlan"));
				if (promoPlan->isChecked()) {
					promoEndsOn->setText(RPDB.getStr("PromoEnds"));
				}
				toggleConvertDays(TRUE);
				break;
				
			case 2:
				noAutoConvert->setChecked(FALSE);
				autoConvertDate->setChecked(TRUE);
				convertDate->setText(RPDB.getStr("ConvertDate"));
				promoPlan->setChecked(RPDB.getInt("PromoPlan"));
				if (promoPlan->isChecked()) {
					promoEndsOn->setText(RPDB.getStr("PromoEnds"));
				}
				toggleConvertDate(TRUE);
				break;
				
			default:
				toggleAutoConvert(TRUE);
				break;
		}
	} else {
		toggleAutoConvert(TRUE);
		setCaption( "New Rate Plan" );
	}
	
	planTag->setFocus();
}


RatePlanEdit::~RatePlanEdit()
{
	if (planIndex != NULL) {
		delete(planIndex);
	}
}


/*
** toggleAutoConvert - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleAutoConvert(bool newState)
{
    if (newState) {
    	// Disable the appropriate fields since we're not auto-converting
    	convertDays->setEnabled(FALSE);
    	convertDate->setEnabled(FALSE);
    	convertToLabel->setEnabled(FALSE);
    	convertToList->setEnabled(FALSE);
    	promoPlan->setEnabled(FALSE);
	    promoEndsOnLabel->setEnabled(FALSE);
    	promoEndsOn->setEnabled(FALSE);
    }
}

/*
** toggleConvertDays - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleConvertDays(bool newState)
{
    if (newState) {
    	// Enable/Disable the appropriate fields to auto convert 
    	// in a specified number of days
    	convertDays->setEnabled(TRUE);
    	convertDate->setEnabled(FALSE);
    	convertToLabel->setEnabled(TRUE);
    	convertToList->setEnabled(TRUE);
    	promoPlan->setEnabled(TRUE);
	    promoEndsOnLabel->setEnabled(promoPlan->isChecked());
    	promoEndsOn->setEnabled(promoPlan->isChecked());
    }
}

/*
** toggleConvertDays - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleConvertDate(bool newState)
{
    if (newState) {
    	// Enable/Disable the appropriate fields to auto convert 
    	// in a specified number of days
    	convertDays->setEnabled(FALSE);
    	convertDate->setEnabled(TRUE);
    	convertToLabel->setEnabled(TRUE);
    	convertToList->setEnabled(TRUE);
    	promoPlan->setEnabled(TRUE);
	    promoEndsOnLabel->setEnabled(promoPlan->isChecked());
    	promoEndsOn->setEnabled(promoPlan->isChecked());
    }
}

/*
** togglePromoPlan - Toggles the availability of the various input fields,
**                   based on the checkbox that the user has selected.
**
*/

void RatePlanEdit::togglePromoPlan(bool newState)
{
    promoEndsOnLabel->setEnabled(newState);
   	promoEndsOn->setEnabled(newState);
}

/*
** cancelRatePlan - Closes the window and destroys itself.
**
*/

void RatePlanEdit::cancelRatePlan()
{
	close();
	delete this;
}

/*
** saveRatePlan - Saves the data, closes the window and destroys itself.
**
*/

void RatePlanEdit::saveRatePlan()
{
	RatePlansDB RPDB;
	
	// Check to make sure that the planTag has something in it...
	if (!strlen(planTag->text())) {
        QMessageBox::critical(this, "Save Rate Plan", "A Rate Plan must have a tag.");
		planTag->setFocus();
		return;
	}
	
	// Check to make sure the description has something in it.
	if (!strlen(description->text())) {
        QMessageBox::critical(this, "Save Rate Plan", "A Rate Plan must have a description.");
		description->setFocus();
		return;
	}
	
	RPDB.setValue("PlanTag", planTag->text());
	RPDB.setValue("Description", description->text());
	if (noAutoConvert->isChecked()) {
		RPDB.setValue("AutoConvert", (int) 0);
		RPDB.setValue("ConvertDays", (int) 0);
		RPDB.setValue("ConvertDate", "0000-00-00");
		RPDB.setValue("PromoPlan", (int) 0);
		RPDB.setValue("PromoEnds", "0000-00-00");
	} else {
		if (autoConvertDays->isChecked()) {
			RPDB.setValue("AutoConvert", (int) 1);
			RPDB.setValue("ConvertDays", convertDays->text());
			RPDB.setValue("ConvertDate", "0000-00-00");
		} else {
			RPDB.setValue("AutoConvert", (int) 2);
			RPDB.setValue("ConvertDays", (int) 0);
			RPDB.setValue("ConvertDate", convertDays->text());
		}
		RPDB.setValue("PromoPlan", (int) promoPlan->isChecked());
		if (promoPlan->isChecked()) {
			RPDB.setValue("PromoPlan", promoEndsOn->text());
		} else {
			RPDB.setValue("PromoPlan", "0000-00-00");
		}
	}
	
	if (!myIntID) {
		RPDB.ins();
	} else {
		RPDB.setValue("InternalID", (long) myIntID);
		RPDB.upd();
	}
	
	close();
	delete this;
}
