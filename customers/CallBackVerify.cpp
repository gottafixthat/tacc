/*
** $Id$
**
***************************************************************************
**
** CallBackVerify - Prompts the user to select the number where the user
**                  was call-back verified.
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
** $Log: CallBackVerify.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CallBackVerify.h"
#include <stdio.h>

#define Inherited CallBackVerifyData

CallBackVerify::CallBackVerify
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Callback Verification" );
	dayPhoneButton->setEnabled(FALSE);
	evePhoneButton->setEnabled(FALSE);
	faxPhoneButton->setEnabled(FALSE);
	altPhoneButton->setEnabled(FALSE);
	otherPhone->setFocus();
}


CallBackVerify::~CallBackVerify()
{
}


/*
** setDayPhoneText - Sets the text on the daytime phone number and
**					 makes it so the user can select it.
*/

void CallBackVerify::setDayPhoneText(const char * newText)
{
	dayPhoneLabel->setText(newText);
	dayPhoneButton->setEnabled(TRUE);
}

/*
** setEvePhoneText - Sets the text on the evetime phone number and
**					 makes it so the user can select it.
*/

void CallBackVerify::setEvePhoneText(const char * newText)
{
	evePhoneLabel->setText(newText);
	evePhoneButton->setEnabled(TRUE);
}

/*
** setFaxPhoneText - Sets the text on the faxtime phone number and
**					 makes it so the user can select it.
*/

void CallBackVerify::setFaxPhoneText(const char * newText)
{
	faxPhoneLabel->setText(newText);
	faxPhoneButton->setEnabled(TRUE);
}

/*
** setAltPhoneText - Sets the text on the alttime phone number and
**					 makes it so the user can select it.
*/

void CallBackVerify::setAltPhoneText(const char * newText)
{
	altPhoneLabel->setText(newText);
	altPhoneButton->setEnabled(TRUE);
}

/*
** getSelected - Builds a string showing which phone number the user was
**				 call back verified on.
**
*/

const char * CallBackVerify::getSelected(void)
{
	char retStr[128];

	if (dayPhoneButton->isChecked()) {
		sprintf(retStr, "Callback Verified at %s", (const char *) dayPhoneLabel->text());
	} else 	if (evePhoneButton->isChecked()) {
		sprintf(retStr, "Callback Verified at %s", (const char *) evePhoneLabel->text());
	} else 	if (faxPhoneButton->isChecked()) {
		sprintf(retStr, "Callback Verified at %s", (const char *) faxPhoneLabel->text());
	} else 	if (altPhoneButton->isChecked()) {
		sprintf(retStr, "Callback Verified at %s", (const char *) altPhoneLabel->text());
	} else 	if (otherButton->isChecked()) {
		sprintf(retStr, "Callback Verified at %s", (const char *) otherPhone->text());
	} else {
		sprintf(retStr, "Error.  No callback buttons where checked.");
	}

	return retStr;
}
