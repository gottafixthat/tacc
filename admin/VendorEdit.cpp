/*
** $Id: VendorEdit.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** VendorEdit - Vendor Edit window.
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
** $Log: VendorEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "VendorEdit.h"
#include <ADB.h>

#define Inherited VendorEditData

VendorEdit::VendorEdit
(
	QWidget* parent,
	const char* name,
	int	IntID
)
	:
	Inherited( parent, name )
{
	myIntID = IntID;
	
	vendor = new VendorsDB;
	
	if (myIntID) {
		setCaption("Edit Vendor");
		vendor->get(myIntID);
	} else {
		setCaption("New Vendor");
	}

	vendorID->setText(vendor->getStr("VendorID"));
	companyName->setText(vendor->getStr("CompanyName"));
	contactName->setText(vendor->getStr("ContactName"));
	altContact->setText(vendor->getStr("AltContact"));
	address1->setText(vendor->getStr("Address1"));
	address2->setText(vendor->getStr("Address2"));
	city->setText(vendor->getStr("City"));
	state->setText(vendor->getStr("State"));
	zip->setText(vendor->getStr("ZIP"));
	checkName->setText(vendor->getStr("CheckName"));
	accountNo->setText(vendor->getStr("AccountNo"));
	taxID->setText(vendor->getStr("TaxID"));
	use1099->setChecked(vendor->getInt("Use1099"));
	creditLimit->setText(vendor->getStr("CreditLimit"));
	active->setChecked(vendor->getInt("Active"));

/*
	vendorID->setText(vendor->VendorID);
	companyName->setText(vendor->CompanyName);
	contactName->setText(vendor->ContactName);
	altContact->setText(vendor->AltContact);
	address1->setText(vendor->Address1);
	address2->setText(vendor->Address2);
	city->setText(vendor->City);
	state->setText(vendor->State);
	zip->setText(vendor->ZIP);
	checkName->setText(vendor->CheckName);
	accountNo->setText(vendor->AccountNo);
	taxID->setText(vendor->TaxID);
	use1099->setChecked(vendor->Use1099.toInt());
	creditLimit->setText(vendor->CreditLimit);
	active->setChecked(vendor->Active.toInt());
*/
}


VendorEdit::~VendorEdit()
{
	delete vendor;
}



/*
** saveVendor - The slot for saveing.  Saves the entry and cleans up 
**				the memory used.
*/ 

void VendorEdit::saveVendor()
{
	vendor->setValue("VendorID", vendorID->text());
	vendor->setValue("CompanyName", companyName->text());
	vendor->setValue("ContactName", contactName->text());
	vendor->setValue("AltContact", altContact->text());
	vendor->setValue("Address1", address1->text());
	vendor->setValue("Address2", address2->text());
	vendor->setValue("City", city->text());
	vendor->setValue("State", state->text());
	vendor->setValue("ZIP", zip->text());
	vendor->setValue("CheckName", checkName->text());
	vendor->setValue("AccountNo", accountNo->text());
	vendor->setValue("TaxID", taxID->text());
	vendor->setValue("Use1099", use1099->isChecked());
	vendor->setValue("CreditLimit", creditLimit->text());
	vendor->setValue("Active", active->isChecked());

	if (myIntID) { 
		vendor->upd();
	} else {
		vendor->ins();
	}
	emit refresh(1);
	close();
}

/**
 ** cancelVendor - The slot for cancelling.  Cleans up the memory used.
 **/ 

void VendorEdit::cancelVendor()
{
	close();
}

