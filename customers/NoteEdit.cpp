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
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>

#include <QtCore/QDateTime>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>
#include <BlargDB.h>
#include <BString.h>
#include <TAATools.h>
#include "NoteEdit.h"

using namespace Qt;

NoteEdit::NoteEdit
(
	QWidget* parent,
	const char* name,
	long CustID,
	long InternalID
) : TAAWidget(parent)
{
    // Create our widgets and our layout
    QLabel  *noteTypeLabel  = new QLabel(this);
    noteTypeLabel->setAlignment(AlignRight|AlignVCenter);
    noteTypeLabel->setText("Note Type:");

    noteType = new QComboBox(false, this, "Note Type List");
    noteType->insertItem("Accounting");
    noteType->insertItem("Operator");
    noteType->insertItem("Support");
    noteType->insertItem("Security");

    QLabel  *loginIDLabel   = new QLabel(this);
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);
    loginIDLabel->setText("User name:");
    
    loginID = new QComboBox(false, this, "Login ID List");
    
    QLabel  *dateTextLabel  = new QLabel(this);
    dateTextLabel->setAlignment(AlignRight|AlignVCenter);
    dateTextLabel->setText("Date:");
    
    dateText = new QLabel(this);
    dateText->setAlignment(AlignLeft|AlignVCenter);
    dateText->setText("notedate");
    
    QLabel  *addedByLabel  = new QLabel(this);
    addedByLabel->setAlignment(AlignRight|AlignVCenter);
    addedByLabel->setText("Added By:");
    
    addedByText = new QLabel(this);
    addedByText->setAlignment(AlignLeft|AlignVCenter);
    addedByText->setText("addedby");
    
    QLabel  *noteTextLabel  = new QLabel(this);
    noteTextLabel->setAlignment(AlignRight|AlignTop);
    noteTextLabel->setText("Note:");

    noteText    = new Q3MultiLineEdit(this, "Note Text");
    noteText->setWordWrap(Q3MultiLineEdit::WidgetWidth);
    
    QPushButton *saveButton = new QPushButton(this, "Save Button");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveNote()));

    QPushButton *cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelNote()));
    
    // Now, create our layout.  A top-to-bottom box will hold the main
    // layout, then a grid to hold our note stuff, then another box
    // to hold our buttons.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // Now create the grid that will hold the labels, lists and note text
    Q3GridLayout *gl = new Q3GridLayout(3, 4);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);
    gl->setRowStretch(0, 0);
    gl->setRowStretch(1, 0);
    gl->setRowStretch(2, 1);

    gl->addWidget(noteTypeLabel,        0, 0);
    gl->addWidget(noteType,             0, 1);
    gl->addWidget(loginIDLabel,         0, 2);
    gl->addWidget(loginID,              0, 3);

    gl->addWidget(dateTextLabel,        1, 0);
    gl->addWidget(dateText,             1, 1);
    gl->addWidget(addedByLabel,         1, 2);
    gl->addWidget(addedByText,          1, 3);

    gl->addWidget(noteTextLabel,        2, 0);
    gl->addMultiCellWidget(noteText,    2, 2, 1, 3);

    ml->addLayout(gl, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);
     
    
	ADB     DB;
	char    title[1024];
	
	myCustID = CustID;
	myInternalID = InternalID;
	strcpy(myLoginID, "");
	strcpy(myNoteType, "");
	strcpy(myDate, "");
	strcpy(myAddedBy, "");
	myNotes = new(char[65536]);
	
	if (myInternalID) {
		strcpy(title, "Edit Note" );
	} else {
		strcpy(title, "New Note" );
	}
	
	
	// If we were not given a customer ID, then exit.
	if (!CustID) return;
	
	// If we're editing one, we need to plug in the data.
	if (InternalID) {
		DB.query("select * from Notes where NoteID = %ld", InternalID);
		if (DB.rowCount == 1) {
			DB.getrow();
			strcpy(myLoginID, DB.curRow["LoginID"]);
			strcpy(myDate, DB.curRow["NoteDate"]);
			strcpy(myNoteType, DB.curRow["NoteType"]);
			strcpy(myNotes, DB.curRow["NoteText"]);
			strcpy(myAddedBy, DB.curRow["AddedBy"]);
			dateText->setText(myDate);
			addedByText->setText(myAddedBy);
			// noteText->setText(wrapString(60, myNotes));
			for (int i = 0; i < noteType->count(); i++) {
				if (!strcasecmp(noteType->text(i), myNoteType)) {
					noteType->setCurrentItem(i);
				}
			}
		} else {
			myInternalID = 0;
		}
	} else {
	    CustomersDB CDB;
	    CDB.get(CustID);
	    strcpy(myLoginID, CDB.getStr("PrimaryLogin"));
	    
	    strcat(title, " - ");
	    strcat(title, CDB.getStr("FullName"));
	}

    noteText->setWordWrap(Q3MultiLineEdit::WidgetWidth);

	if (!myInternalID) {
		time_t	ti = time(NULL);
		struct tm * tm = localtime(&ti);
			
		sprintf(myDate, "%04d-%02d-%02d", tm->tm_year+1900,
		  tm->tm_mon+1, tm->tm_mday);
		dateText->setText(myDate);
		strcpy(myAddedBy, curUser().userName);
		addedByText->setText(myAddedBy);

		// Set the default note type to be operator
		for (int i = 0; i < noteType->count(); i++) {
			if (!strcasecmp(noteType->text(i), "Operator")) {
				noteType->setCurrentItem(i);
			}
		}
	}

	// Fill in our login ID combo box by getting a list of Login ID's.
	DB.query("select LoginID from Logins where CustomerID = %ld order by LoginID", CustID);
	int idxCnt = 0;
	if (DB.rowCount > 0) while (DB.getrow()) {
		loginID->insertItem(DB.curRow["LoginID"]);
		if (InternalID) {
			if (!strcmp(DB.curRow["LoginID"], myLoginID)) {
				loginID->setCurrentItem(idxCnt);
			}
		}
		idxCnt++;
	}
	

	noteText->setFocus();
    #if QT_VERSION >= 210
    noteText->setWordWrap(Q3MultiLineEdit::WidgetWidth);
    #endif
	
	setCaption(title);
	
	show();
}


NoteEdit::~NoteEdit()
{
	delete (myNotes);
}

/**
 ** cancelNote - A private slot which will close the window.
 **
 **/

void NoteEdit::cancelNote()
{
	close();
	delete(this);
}

/**
 ** saveNote - A private slot which will save the note and close the window.
 **
 **/

void NoteEdit::saveNote()
{
	NotesDB NDB;
	
	QDateTime   tmpDT = QDateTime::currentDateTime();

    sprintf(myDate, "%04d-%02d-%02d %02d:%02d:%02d",
      tmpDT.date().year(),
      tmpDT.date().month(),
      tmpDT.date().day(),
      tmpDT.time().hour(),
      tmpDT.time().minute(),
      tmpDT.time().second()
    );
	
	// Copy all of our data into a database object.
	NDB.setValue("NoteID", myInternalID);
	NDB.setValue("LoginID", loginID->text(loginID->currentItem()));
    NDB.setValue("CustomerID", myCustID);
    NDB.setValue("NoteDate", myDate);
    NDB.setValue("NoteType", noteType->text(noteType->currentItem()));
    NDB.setValue("NoteText", noteText->text());
    NDB.setValue("AddedBy", myAddedBy);

	if (myInternalID) {
		NDB.upd();
	} else {
		NDB.ins();
	}
	
	emit(customerUpdated(myCustID));
	close();
	delete(this);
}


// vim: expandtab
