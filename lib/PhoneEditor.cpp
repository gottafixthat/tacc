/*
** $Id: PhoneEditor.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** PhoneEditor - A telephone number editor.
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
** $Log: PhoneEditor.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "PhoneEditor.h"
#include "BlargDB.h"
#include "BString.h"

#include <ADB.h>

#include <stdio.h>
#include <stdlib.h>
#include <qmessagebox.h>

#define Inherited PhoneEditorData

PhoneEditor::PhoneEditor
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Edit Phone Number" );
	myIntID			= 0;
	myRefFrom		= 0;
	myRefID			= 0;
	myTag[0]		= '\0';
	myInternational	= 0;
}


PhoneEditor::~PhoneEditor()
{
}


// editPhone - Loads up the address editor window

void PhoneEditor::editPhone(long refFrom, long refID, const char *tag)

{
	PhoneNumbersDB	PDB;
	
	PDB.get(refFrom, refID, tag);
	prepareEditForm(&PDB);
}


void PhoneEditor::editPhoneByID(long intID)

{
	PhoneNumbersDB	PDB;
	
	PDB.getByID(intID);
	prepareEditForm(&PDB);
}


// prepareEditForm - Prepares the input form by plugging in the various
//                   values.

void PhoneEditor::prepareEditForm(PhoneNumbersDB *PDB)

{
	ADB         DB;
	CustomersDB	CDB;
	VendorsDB	VDB;
	char		tmpStr[64];
	
	myIntID	= PDB->InternalID;

	switch (PDB->RefFrom.toInt()) {
		case REF_CUSTOMER:
			CDB.get(PDB->RefID.toLong());
			typeLabel->setText("Customer:");
			typeLabel2->setText(CDB.getStr("FullName"));
			break;
			
		case REF_VENDOR:
			VDB.get(PDB->RefID.toLong());
			typeLabel->setText("Vendor:");
			typeLabel2->setText(VDB.getStr("CompanyName"));
			break;
			
		default:
			break;
	}
	
	if (myIntID) {
		tagList->hide();
		tagLabel->setText(PDB->Tag);
	} else {
		tagLabel->hide();
		// Populate the tag list
		DB.query("select distinct Tag from PhoneNumbers");
		if (DB.rowCount) while(DB.getrow()) {
			tagList->insertItem(DB.curRow["Tag"]);
		}
	}
	
	active->setChecked(PDB->Active.toInt());
	international->setChecked(PDB->International.toInt());
	phoneNumber->setText(PDB->PhoneNumber);
	myDatetoHuman(tmpStr, (const char *)PDB->LastModified);
	lastModified->setText(tmpStr);
	lastModifiedBy->setText(PDB->LastModifiedBy);
	
	updateInternational();
	
	show();
}

// newPhone - Sets up the address editor window

void PhoneEditor::newPhone(long refFrom, long refID)

{
	PhoneNumbersDB	PDB;
	myIntID 	= 0;
	myRefFrom 	= refFrom;
	myRefID 	= refID;
	
	PDB.InternalID = 0;
	PDB.RefFrom.setNum(refFrom);
	PDB.RefID.setNum(refID);
	PDB.Active.setNum(1);
	
	prepareEditForm(&PDB);
}


void PhoneEditor::updateInternational(void)
{
/*
	if (international->isChecked()) {
		cszLabel->setText("City:");
		state->hide();
		zip->hide();
		postalLabel->show();
		postalCode->show();
		countryLabel->show();
		country->show();
	} else {
		cszLabel->setText("City, State, ZIP:");
		state->show();
		zip->show();
		postalLabel->hide();
		postalCode->hide();
		countryLabel->hide();
		country->hide();
	}
*/
}

void PhoneEditor::internationalChanged(bool)
{
	updateInternational();
}

void PhoneEditor::savePhone()
{

	PhoneNumbersDB	PDB;
	ADB  			DB;
	
	if (myIntID) {
		PDB.getByID(myIntID);
	} else {
		// Check to make sure that we're not adding a duplicate tag
		DB.query("select InternalID from PhoneNumbers where Tag = '%s' and RefFrom = %d and RefID = %ld",
		  (const char *) tagList->currentText(),
		  myRefFrom,
		  myRefID
		);
		if (DB.rowCount) {
            QMessageBox::warning(this, "Duplicate Tag", "The specified tag is already in use for this\ncustomer/vendor.  Use edit to change a phone number.", "&Ok");
			return;
		}
	}
	PDB.PhoneNumber = phoneNumber->text();
	PDB.International.setNum((int)international->isChecked());
	PDB.Active.setNum((int)active->isChecked());


	if (myIntID) {
		PDB.upd();
	} else {
		PDB.RefFrom.setNum(myRefFrom);
		PDB.RefID.setNum(myRefID);
		PDB.Tag = tagList->currentText();
		PDB.ins();
	}

	emit(phoneUpdated(PDB.RefFrom.toInt(), PDB.RefID.toLong()));
	close();
}

void PhoneEditor::cancelPhone()
{
	close();
}

