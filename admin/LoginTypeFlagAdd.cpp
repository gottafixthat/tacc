/*
** $Id$
**
***************************************************************************
**
** LoginTypeFlagAdd - Allows flags to be added to login type definitions.
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
** $Log: LoginTypeFlagAdd.cpp,v $
** Revision 1.2  2003/12/23 02:20:41  marc
** Fixed the non-working dialog.  Removed the old QTArch created dialog.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "LoginTypeFlagAdd.h"
#include "BlargDB.h"
#include "BString.h"
#include <stdio.h>
#include <stdlib.h>
#include <qstrlist.h>
#include <qlayout.h>
#include <ADB.h>

LoginTypeFlagAdd::LoginTypeFlagAdd
(
	QWidget* parent,
	const char* name,
	int LoginTypeID
) : QDialog( parent, name, true )
{
	setCaption( "Add Login Type Flag" );
    
    // Create our widgets.
    flagList = new QListBox(this, "flagList");
    connect(flagList, SIGNAL(doubleClicked(QListBoxItem *)), this, SLOT(flagDoubleClicked(QListBoxItem *)));

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addFlag()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlag()));
    
    // Create our layout.

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(flagList, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);


	ADB	    DB;
	char	tmpstr[4096];
	
	QStrList tmplist;

	if (!LoginTypeID) return;
	
	myLoginTypeID = LoginTypeID;
	
	// Get the list of Login Flags that are already included.
	DB.query("select Tag from LoginTypeFlags where LoginTypeID = %d", myLoginTypeID);
	if (DB.rowCount) {
		while (DB.getrow()) {
			sprintf(tmpstr, "'%s'", DB.curRow["Tag"]);
			tmplist.append(tmpstr);
		}
		joinString(&tmplist, ",", tmpstr);
		DB.query("select LoginFlag from LoginFlags where LoginFlag not in (%s) order by LoginFlag", tmpstr);
	} else {
		DB.query("select LoginFlag from LoginFlags order by LoginFlag");
	}
	
	// Fill the itemList with billable Items.
	flagList->clear();
	while (DB.getrow()) {
		flagList->insertItem(DB.curRow["LoginFlag"]);
	}
	flagList->setCurrentItem(0);
	
	addButton->setDefault(TRUE);
	// connect(saveButton, SIGNAL(clicked()), SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

}


LoginTypeFlagAdd::~LoginTypeFlagAdd()
{
}


void LoginTypeFlagAdd::addFlag()
{
	ADB     DB;
	char	tmptag[1024];
	char	tagval[1024];
	
	strcpy(tmptag, flagList->text(flagList->currentItem()));
	DB.query("select DefaultValue from LoginFlags where LoginFlag = '%s'", tmptag);
	DB.getrow();
	strcpy(tagval, DB.curRow["DefaultValue"]);
	
	DB.dbcmd("insert into LoginTypeFlags values (0, %d, '%s', '%s')", 
	  myLoginTypeID, 
	  tmptag,
	  tagval
	);
	done(Accepted);
}

void LoginTypeFlagAdd::flagDoubleClicked(QListBoxItem *)
{
    addFlag();
}

void LoginTypeFlagAdd::cancelFlag()
{
    reject();
}

