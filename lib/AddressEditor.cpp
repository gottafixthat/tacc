/*
** $Id: AddressEditor.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** AddressEditor - An address editor.
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
** $Log: AddressEditor.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "AddressEditor.h"
#include "BlargDB.h"
#include "BString.h"

#include <TAATools.h>

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <TAAWidget.h>


AddressEditor::AddressEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Edit Address" );
	myIntID			= 0;
	myRefFrom		= 0;
	myRefID			= 0;
	myTag[0]		= '\0';
	myInternational	= 0;

    // Create our widgets.
    typeLabel   = new QLabel(this, "TypeLabel");
    typeLabel->setAlignment(AlignRight|AlignVCenter);
    typeLabel->setText("Customer:");

    typeData   = new QLabel(this, "TypeData");
    typeData->setAlignment(AlignLeft|AlignVCenter);
    typeData->setText("Customer/Vendor Name");

    
    HorizLine   *hline1 = new HorizLine(this);
   
    QLabel *tagLabel = new QLabel(this);
    tagLabel->setAlignment(AlignRight|AlignVCenter);
    tagLabel->setText("Tag:");

    tagList = new QComboBox(true, this);
    tagList->setAutoCompletion(true);
    
    tagData = new QLabel(this);
    tagData->setAlignment(AlignLeft|AlignVCenter);
    tagData->setText("Address Tag");

    
    QLabel  *addressLabel1 = new QLabel(this);
    addressLabel1->setAlignment(AlignRight|AlignVCenter);
    addressLabel1->setText("Address Line 1:");

    QLabel  *addressLabel2 = new QLabel(this);
    addressLabel2->setAlignment(AlignRight|AlignVCenter);
    addressLabel2->setText("Address Line 2:");

    cszLabel = new QLabel(this);
    cszLabel->setAlignment(AlignRight|AlignVCenter);
    cszLabel->setText("City, State, ZIP:");

    countryLabel = new QLabel(this);
    countryLabel->setAlignment(AlignRight|AlignVCenter);
    countryLabel->setText("Country:");

    postalCodeLabel = new QLabel(this);
    postalCodeLabel->setAlignment(AlignRight|AlignVCenter);
    postalCodeLabel->setText("Postal Code:");

    address1 = new QLineEdit(this);
    address1->setMaxLength(80);
    
    address2 = new QLineEdit(this);
    address2->setMaxLength(80);
    
    city = new QLineEdit(this);
    city->setMaxLength(80);

    state = new QLineEdit(this);
    state->setMaxLength(2);

    zip = new QLineEdit(this);
    zip->setMaxLength(15);

    postalCode = new QLineEdit(this);
    postalCode->setMaxLength(15);

    country = new QLineEdit(this);
    country->setMaxLength(35);

    saveButton = new QPushButton(this);
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAddress()));

    cancelButton = new QPushButton(this);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAddress()));

    active = new QCheckBox(this);
    active->setText("Active");

    international = new QCheckBox(this);
    international->setText("International");
    connect(international, SIGNAL(toggled(bool)), SLOT(internationalChanged(bool)));

    
    // Now do the layout.  Its a box and a grid
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // Then the grid
    QGridLayout *gl = new QGridLayout(3, 5, 3);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    int totCols = 3;
    int curRow = -1;
    int curCol = 0;

    curRow++; curCol = 0;
    gl->addWidget(typeLabel,            curRow, curCol++);
    gl->addMultiCellWidget(typeData,    curRow, curRow, curCol, totCols-1);

    curRow++; curCol = 0;
    gl->addMultiCellWidget(hline1,      curRow, curRow, 0, totCols-1);

    curRow++; curCol = 0;
    gl->addWidget(tagLabel,             curRow, curCol++);
    gl->addWidget(tagData,              curRow, curCol++);
    gl->addWidget(tagList,              curRow, curCol-1);
    gl->addWidget(active,               curRow, curCol++);
    
    curRow++; curCol = 2;
    gl->addWidget(international,        curRow, curCol++);

    curRow++; curCol = 0;
    gl->addWidget(addressLabel1,        curRow, curCol++);
    gl->addMultiCellWidget(address1,    curRow, curRow, curCol, totCols-1);

    curRow++; curCol = 0;
    gl->addWidget(addressLabel2,        curRow, curCol++);
    gl->addMultiCellWidget(address2,    curRow, curRow, curCol, totCols-1);

    // A layout for the City, state, zip.
    QBoxLayout *cszl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    cszl->addWidget(city, 4);
    cszl->addWidget(state, 1);
    cszl->addWidget(zip, 2);

    curRow++; curCol = 0;
    gl->addWidget(cszLabel,             curRow, curCol++);
    gl->addMultiCellLayout(cszl,        curRow, curRow, curCol, totCols-1);

    curRow++; curCol = 0;
    gl->addWidget(postalCodeLabel,      curRow, curCol++);
    gl->addMultiCellWidget(postalCode,  curRow, curRow, curCol, totCols-1);

    curRow++; curCol = 0;
    gl->addWidget(countryLabel,         curRow, curCol++);
    gl->addMultiCellWidget(country,     curRow, curRow, curCol, totCols-1);


    ml->addLayout(gl);

    // Add a button layout
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addStretch(1);
    ml->addLayout(bl, 0);


    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
    // connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
}


AddressEditor::~AddressEditor()
{
}


// editAddress - Loads up the address editor window

void AddressEditor::editAddress(long refFrom, long refID, const char *tag)

{
	AddressesDB		addrDB;
	
	addrDB.get(refFrom, refID, tag);
	prepareEditForm(&addrDB);
}


void AddressEditor::editAddressByID(long intID)

{
	AddressesDB		addrDB;
	
	addrDB.getByID(intID);
	prepareEditForm(&addrDB);
}


// prepareEditForm - Prepares the input form by plugging in the various
//                   values.

void AddressEditor::prepareEditForm(AddressesDB *addrDB)

{
	ADB         DB;
	CustomersDB	CDB;
	VendorsDB	VDB;
	char		tmpStr[64];
	
	myIntID	= addrDB->InternalID;

	switch (addrDB->RefFrom.toInt()) {
		case REF_CUSTOMER:
			CDB.get(addrDB->RefID.toLong());
			typeLabel->setText("Customer:");
			typeData->setText(CDB.getStr("FullName"));
			break;
			
		case REF_VENDOR:
			VDB.get(addrDB->RefID.toLong());
			typeLabel->setText("Vendor:");
			typeData->setText(VDB.getStr("CompanyName"));
			break;
			
		default:
			break;
	}
	
	if (myIntID) {
		tagList->hide();
		tagData->setText(addrDB->Tag);
	} else {
		tagData->hide();
		// Populate the tag list
		DB.query("select distinct Tag from Addresses order by Tag");
		if (DB.rowCount) while(DB.getrow()) {
			tagList->insertItem(DB.curRow["Tag"]);
		}
	}
	
	active->setChecked(addrDB->Active.toInt());
	international->setChecked(addrDB->International.toInt());
	address1->setText(addrDB->Address1);
	address2->setText(addrDB->Address2);
	city->setText(addrDB->City);
	state->setText(addrDB->State);
	zip->setText(addrDB->ZIP);
	postalCode->setText(addrDB->PostalCode);
	country->setText(addrDB->Country);
	myDatetoHuman(tmpStr, (const char *)addrDB->LastModified);
	
	updateInternational();
	
	show();
}

// newAddress - Sets up the address editor window

void AddressEditor::newAddress(long refFrom, long refID)

{
	AddressesDB	addrDB;
	myIntID 	= 0;
	myRefFrom 	= refFrom;
	myRefID 	= refID;
	
	addrDB.InternalID = 0;
	addrDB.RefFrom.setNum(refFrom);
	addrDB.RefID.setNum(refID);
	addrDB.Active.setNum(1);
	
	prepareEditForm(&addrDB);
}


void AddressEditor::updateInternational(void)
{
	if (international->isChecked()) {
		cszLabel->setText("City:");
		state->hide();
		zip->hide();
		postalCodeLabel->show();
		postalCode->show();
		countryLabel->show();
		country->show();
	} else {
		cszLabel->setText("City, State, ZIP:");
		state->show();
		zip->show();
		postalCodeLabel->hide();
		postalCode->hide();
		countryLabel->hide();
		country->hide();
	}
}

void AddressEditor::internationalChanged(bool)
{
	updateInternational();
}

void AddressEditor::saveAddress()
{

	AddressesDB		addrDB;
	ADB	    		DB;
	
	if (myIntID) {
		addrDB.getByID(myIntID);
	} else {
		// Check to make sure that we're not adding a duplicate tag
		DB.query("select InternalID from Addresses where Tag = '%s' and RefFrom = %ld and RefID = %ld",
		  (const char *) tagList->currentText(),
		  myRefFrom,
		  myRefID
		);
		if (DB.rowCount) {
            QMessageBox::warning(this, "Duplicate Tag", "The specified tag is already in use for this\ncustomer/vendor.  Use edit to change an address.", "&Ok");
			return;
		}
	}
	addrDB.Address1      = address1->text();
	addrDB.Address2      = address2->text();
	addrDB.City          = city->text();
	addrDB.State         = state->text();
	addrDB.ZIP           = zip->text();
	addrDB.Country       = country->text();
	addrDB.PostalCode    = postalCode->text();
	addrDB.International = international->isChecked();
	addrDB.Active.setNum((int)active->isChecked());


	if (myIntID) {
		addrDB.upd();
	} else {
		addrDB.RefFrom.setNum(myRefFrom);
		addrDB.RefID.setNum(myRefID);
		addrDB.Tag = tagList->currentText();
		addrDB.ins();
	}

    if (myRefFrom == REF_CUSTOMER) emit(customerChanged(addrDB.RefID.toLong()));
	emit(addressUpdated(addrDB.RefFrom.toInt(), addrDB.RefID.toLong()));
	close();
}

void AddressEditor::cancelAddress()
{
	close();
}

