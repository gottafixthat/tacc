/*
** $Id: DomainTypeEdit.cpp,v 1.3 2004/01/10 01:53:15 marc Exp $
**
***************************************************************************
**
** DomainTypeEdit - DomainType Editor.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DomainTypeEdit.cpp,v $
** Revision 1.3  2004/01/10 01:53:15  marc
** Re-written Domain type editor.  Still needs some work, but is getting there.
**
** Revision 1.2  2004/01/01 01:57:54  marc
** *** empty log message ***
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "DomainTypeEdit.h"
#include "DomainTypeBillablesAdd.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlabel.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <ADB.h>


DomainTypeEdit::DomainTypeEdit
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Edit Domain Types" );
    // Create our widgets
    domainTypeList = new QListView(this, "domainTypeList");
    domainTypeList->addColumn("Domain Type");
    connect(domainTypeList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(itemSelected(QListViewItem *)));

    newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newDomainType()));

    delButton = new QPushButton(this, "delButton");
    delButton->setText("Delete");
    connect(delButton, SIGNAL(clicked()), this, SLOT(deleteDomainType()));
    
    QLabel  *domainTypeLabel = new QLabel(this, "domainTypeNameLabel");
    domainTypeLabel->setText("Name:");
    domainTypeLabel->setAlignment(AlignRight|AlignVCenter);

    domainType = new QLineEdit(this, "domainTypeName");
    domainType->setMaxLength(31);

    QLabel  *billablesListLabel = new QLabel(this, "billablesListLabel");
    billablesListLabel->setText("Billables:");
    billablesListLabel->setAlignment(AlignRight|AlignTop);

    billablesList = new QListBox(this, "billablesList");
    
    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addBillable()));

    removeButton = new QPushButton(this, "removeButton");
    removeButton->setText("&Remove");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeBillable()));

    QLabel  *dnsTemplateLabel = new QLabel(this, "dnsTemplateLabel");
    dnsTemplateLabel->setText("DNS Template:");
    dnsTemplateLabel->setAlignment(AlignRight|AlignTop);

    dnsTemplate = new QComboBox(false, this, "dnsTemplate");

    allowEmail = new QCheckBox(this, "allowEmail");
    allowEmail->setText("Allow email hosting/aliases");

    allowHosting = new QCheckBox(this, "allowHosting");
    allowHosting->setText("Allow Web hosting");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDomainType()));

    // Okay, create our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::LeftToRight, 3, 3);

    // The layout for our list of domain types
    QBoxLayout *dtl = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    dtl->addWidget(domainTypeList, 1);
    // And another for the buttons for the domain type list.
    QBoxLayout *dtabl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    dtabl->addStretch(1);
    dtabl->addWidget(newButton, 0);
    dtabl->addWidget(delButton, 0);
    dtabl->addStretch(1);

    dtl->addLayout(dtabl, 0);

    // Add the domain type list and its buttons to the main layout
    ml->addLayout(dtl, 0);

    QBoxLayout *owl = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    // Now, create a grid to hold the other widgets.
    QGridLayout *gl = new QGridLayout(5, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(domainTypeLabel,                  curRow, 0);
    gl->addWidget(domainType,                       curRow, 1);
    gl->setRowStretch(curRow++, 0);

    // The billable list row is harder.  We are going to include it in
    // a layout of its own, and include its buttons with it.
    QBoxLayout *bil = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    bil->addWidget(billablesList, 1);
    // And add another layout for its buttons.
    QBoxLayout *bibl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bibl->addStretch(1);
    bibl->addWidget(addButton, 0);
    bibl->addWidget(removeButton, 0);
    bil->addLayout(bibl, 0);

    gl->addWidget(billablesListLabel,               curRow, 0);
    gl->addLayout(bil,                              curRow, 1);
    gl->setRowStretch(curRow++, 1);

    gl->addWidget(dnsTemplateLabel,                 curRow, 0);
    gl->addWidget(dnsTemplate,                      curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(allowEmail,                       curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(allowHosting,                     curRow, 1);
    gl->setRowStretch(curRow++, 0);

    owl->addLayout(gl, 1);

    // Add a layout for our save button
    QBoxLayout *sbl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    sbl->addStretch(1);
    sbl->addWidget(saveButton, 0);
    owl->addLayout(sbl, 0);
    
    ml->addLayout(owl, 1);

    // Load the DNS Template List
    ADB     DB;
    DB.query("select TemplateID, TemplateName from DNS_Templates order by TemplateName");
    templateIDX = (long *) calloc(DB.rowCount+2, sizeof(long));
    dnsTemplate->insertItem("None");
    long idxPtr = 1;
    while (DB.getrow()) {
        dnsTemplate->insertItem(DB.curRow["TemplateName"]);
        templateIDX[idxPtr++] = atol(DB.curRow["TemplateID"]);
    }


    refreshList();
}


DomainTypeEdit::~DomainTypeEdit()
{
}


void DomainTypeEdit::refreshList(void)
{
	ADB	    DB;
	
	DB.query("select DomainType, InternalID from DomainTypes order by DomainType");
	domainTypeList->clear();
	while (DB.getrow()) {
        (void) new QListViewItem(domainTypeList, 
                                 DB.curRow["DomainType"],
                                 DB.curRow["InternalID"]);
	}
}

/*
** itemSelected   - Gets called whenever an item is selected from the
**                  account types list.  What it does is scans the database
**                  for the associated flags and displays them in the
**                  flagTypeList.
*/

void DomainTypeEdit::itemSelected(QListViewItem *curItem)
{
	ADB	    DB;
    ADB	    DB2;
	long	tmpID = 0;

	// Clear the two lists and the other artifacts...
	billablesList->clear();
	
    // Grab the entry from the database.
    DB.query("select * from DomainTypes where InternalID = %ld", atol(curItem->key(1,0)));
    DB.getrow();

    domainType->setText(DB.curRow["DomainType"]);

    if (atoi(DB.curRow["AllowAliases"])) allowEmail->setChecked(true);
    else allowEmail->setChecked(false);

    if (atoi(DB.curRow["AllowWebHosting"])) allowHosting->setChecked(true);
    else allowHosting->setChecked(false);

    // Find the Template ID in the template list
    dnsTemplate->setCurrentItem(0);
    long    idxPtr = 0;
    for (int i = 0; i < dnsTemplate->count(); i++) {
        if (atol(DB.curRow["TemplateID"]) == templateIDX[i]) {
            dnsTemplate->setCurrentItem(i);
        }
    }

	// Update the list of associated flags.
    tmpID = atol(curItem->key(1,0));
    // Update the list of associated Billables
    DB.query("select DomainTypeBillables.ItemNumber, Billables.ItemID from DomainTypeBillables, Billables where DomainTypeBillables.DomainTypeID = %ld and DomainTypeBillables.ItemNumber = Billables.ItemNumber order by Billables.ItemID", tmpID);
    if (DB.rowCount) {
        while (DB.getrow()) {
            billablesList->insertItem(DB.curRow["ItemID"]);
        }
    }
	
	// Setup the initial highlights
	if (billablesList->count()) {
		billablesList->setCurrentItem(0);
	}
}


/*
** addBillable - Brings up dialog box and lets the user add 
**               billables associted with a domain type.
*/

void DomainTypeEdit::addBillable()
{
    DomainTypeBillablesAdd *dtba;
    QListViewItem           *curItem;

    curItem = domainTypeList->currentItem();
    if (curItem == NULL) return;
	
    dtba = new DomainTypeBillablesAdd(this, "", atoi(curItem->key(1,0)));

    dtba = new DomainTypeBillablesAdd(0, "", atoi(curItem->key(1,0)));
    dtba->show();
    if (dtba->result()) itemSelected(curItem);
}

/*
** removeBillable - Brings up dialog box and makes sure that the user really
**                  wants to delete this associated billable.
*/

void DomainTypeEdit::removeBillable()
{
	ADB		    DB;
	int			tmpID;
	int			tmpNum;
    QListViewItem           *curItem;

    curItem = domainTypeList->currentItem();
    if (curItem == NULL) return;
	
	if (billablesList->currentItem() >= 0) {
        tmpID = atoi(curItem->key(1,0));
		DB.query("select ItemNumber from Billables where ItemID = '%s'", (const char *) billablesList->text(billablesList->currentItem()));
		DB.getrow();
		tmpNum = atoi(DB.curRow["ItemNumber"]);
		DB.dbcmd("delete from DomainTypeBillables where DomainTypeID = %d and ItemNumber = %d", tmpID, tmpNum);
		itemSelected(curItem);
	}
}

/*
** saveDomainType - Saves the changes made.
*/

void DomainTypeEdit::saveDomainType()
{
    QListViewItem   *curItem = domainTypeList->currentItem();
    if (curItem == NULL) return;

    ADBTable    dtDB("DomainTypes");
    dtDB.get(atol(curItem->key(1,0)));

    dtDB.setValue("DomainType",     (const char *)domainType->text());
    if (allowEmail->isChecked()) dtDB.setValue("AllowAliases",  1);
    else dtDB.setValue("AllowAliases", 0);
    if (allowHosting->isChecked()) dtDB.setValue("AllowWebHosting",  1);
    else dtDB.setValue("AllowWebHosting", 0);
    dtDB.setValue("TemplateID", templateIDX[dnsTemplate->currentItem()]);
    dtDB.upd();

    curItem->setText(0, domainType->text());

}



