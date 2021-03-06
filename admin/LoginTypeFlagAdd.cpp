/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdio.h>
#include <stdlib.h>

#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3strlist.h>

#include <ADB.h>
#include <BlargDB.h>
#include <BString.h>

#include "LoginTypeFlagAdd.h"

using namespace Qt;

LoginTypeFlagAdd::LoginTypeFlagAdd
(
	QWidget* parent,
	const char* name,
	int LoginTypeID
) : QDialog( parent, name, true )
{
	setCaption( "Add Login Type Flag" );
    
    // Create our widgets.
    flagList = new Q3ListBox(this, "flagList");
    connect(flagList, SIGNAL(doubleClicked(Q3ListBoxItem *)), this, SLOT(flagDoubleClicked(Q3ListBoxItem *)));

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addFlag()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlag()));
    
    // Create our layout.

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(flagList, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);


	ADB	    DB;
	char	tmpstr[4096];
	
	Q3StrList tmplist;

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

void LoginTypeFlagAdd::flagDoubleClicked(Q3ListBoxItem *)
{
    addFlag();
}

void LoginTypeFlagAdd::cancelFlag()
{
    reject();
}


// vim: expandtab
