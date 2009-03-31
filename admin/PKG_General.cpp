/*
** $Id$
**
***************************************************************************
**
** PKG_General - Handles the package editor tab for Name, Description and
**               general toggles for packages.
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
** $Log: PKG_General.cpp,v $
** Revision 1.3  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.2  2003/12/29 23:11:32  marc
** Removed QTArch dialogs, doing them manually.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "PKG_General.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3BoxLayout>

#include <ADB.h>

PKG_General::PKG_General
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    ADB     DB;
    // Create our widgets.
    QLabel  *itemNameLabel = new QLabel(this, "itemNameLabel");
    itemNameLabel->setText("Package Name:");
    itemNameLabel->setAlignment(AlignRight|AlignVCenter);

    itemName = new QLineEdit(this, "itemName");
    itemName->setMaxLength(32);
    connect(itemName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));

    QLabel  *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(AlignRight|AlignTop);

    description = new Q3MultiLineEdit(this, "description");
    connect(description, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    isActive = new QCheckBox(this, "isActive");
    isActive->setText("Active");
    connect(isActive, SIGNAL(clicked()), this, SLOT(checkForSave()));
    
    showIncluded = new QCheckBox(this, "showIncluded");
    showIncluded->setText("Show Included Items");
    connect(showIncluded, SIGNAL(clicked()), this, SLOT(checkForSave()));

    isPrivate = new QCheckBox(this, "isPrivate");
    isPrivate->setText("Private Package");
    connect(isPrivate, SIGNAL(clicked()), this, SLOT(checkForSave()));

    QLabel *glAccountLabel = new QLabel(this, "glAccountLabel");
    glAccountLabel->setText("Account:");
    glAccountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    glAccount = new QComboBox(false, this, "glAccount");
    // Load the account list and seutp our index
    DB.query("select IntAccountNo, AccountNo, AcctName from Accounts order by AccountNo, AcctName");
    glAcctIDX = new int[DB.rowCount + 2];
    int curIDX = 0;
    QString tmpStr;
    if (DB.rowCount) while (DB.getrow()) {
        glAcctIDX[curIDX] = atoi(DB.curRow["IntAccountNo"]);
        tmpStr = DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        glAccount->insertItem(tmpStr);
        curIDX++;
    }
    connect(glAccount, SIGNAL(activated(int)), this, SLOT(glAccountChanged(int)));

    QLabel *numMailboxesLabel = new QLabel(this, "numMailboxesLabel");
    numMailboxesLabel->setText("Number of Mailboxes:");
    numMailboxesLabel->setAlignment(AlignRight|AlignVCenter);

    numMailboxes = new QSpinBox(this, "numMailboxes");
    numMailboxes->setRange(0,100);
    connect(numMailboxes, SIGNAL(valueChanged(int)), this, SLOT(numMailboxesChanged(int)));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // The grid to hold most of our data
    Q3GridLayout *gl = new Q3GridLayout(5, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(itemNameLabel,            curRow, 0);
    gl->addWidget(itemName,                 curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(descriptionLabel,         curRow, 0);
    gl->addWidget(description,              curRow, 1);
    gl->setRowStretch(curRow++, 1);

    gl->addWidget(isActive,                 curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(showIncluded,             curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(isPrivate,                curRow, 1);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(glAccountLabel,           curRow, 0);
    gl->addWidget(glAccount,                curRow, 1);
    gl->setRowStretch(curRow++, 1);
    
    gl->addWidget(numMailboxesLabel,        curRow, 0);
    gl->addWidget(numMailboxes,             curRow, 1);
    gl->setRowStretch(curRow++, 0);

    ml->addLayout(gl, 1);

    // Create the layout for our row of button.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // Done with our layout.

    intItemName     = NULL;
    intItemDesc     = NULL;
    intItemActive   = 0;
    intItemPrivate  = 0;
    intShowIncluded = 0;
    intGLAccount    = 0;
    intNumMailboxes = 0;

    myPackageID     = 0;
    
    setPackageID((long) 0);
}


PKG_General::~PKG_General()
{
}


/*
** setPackageID - Sets the current package ID.
**                Loads the information from the database.
*/

void PKG_General::setPackageID(long newPackageID)
{
    if (newPackageID) {
        ADBTable    PDB;
        PDB.setTableName("Packages");
        if (PDB.get(newPackageID)) {
            myPackageID = newPackageID;
	        itemName->setText(PDB.getStr("PackageTag"));
	        itemName->setEnabled(TRUE);
	        description->setText(PDB.getStr("Description"));
	        description->setEnabled(TRUE);
	        isActive->setChecked(PDB.getInt("Active"));
	        isActive->setEnabled(TRUE);
	        isPrivate->setChecked(PDB.getInt("PackageType"));
	        isPrivate->setEnabled(TRUE);
	        showIncluded->setChecked(PDB.getInt("ShowIncluded"));
	        showIncluded->setEnabled(TRUE);
            for(int i=0; i < glAccount->count(); i++) {
                if (glAcctIDX[i] == PDB.getInt("IntAccountNo")) {
                    glAccount->setCurrentItem(i);
                }
            }
            numMailboxes->setValue(PDB.getInt("NumMailboxes"));
	        numMailboxes->setEnabled(TRUE);

            saveButton->setEnabled(FALSE);
            
            // Now, store the values in memory for comparison.
            if (intItemName != NULL) delete intItemName;
            if (intItemDesc != NULL) delete intItemDesc;
            
            intItemName = new char[strlen(PDB.getStr("PackageTag"))+2];
            strcpy(intItemName, PDB.getStr("PackageTag"));
            intItemDesc = new char[strlen(PDB.getStr("Description"))+2];
            strcpy(intItemDesc, PDB.getStr("Description"));
            intItemActive = isActive->isChecked();
            intItemPrivate = isPrivate->isChecked();
            intShowIncluded = showIncluded->isChecked();
            intGLAccount    = PDB.getInt("IntAccountNo");
            intNumMailboxes = PDB.getInt("NumMailboxes");
            
        } else {
            myPackageID = 0;
        }
    } else {
        myPackageID = 0;
    }
    
    if (!myPackageID) {
        itemName->setText("");
        itemName->setEnabled(FALSE);
        description->setText("");
        description->setEnabled(FALSE);
        isActive->setChecked(FALSE);
        isActive->setEnabled(FALSE);
        isPrivate->setChecked(FALSE);
        isPrivate->setEnabled(FALSE);
        showIncluded->setChecked(FALSE);
        showIncluded->setEnabled(FALSE);
        saveButton->setEnabled(FALSE);
        numMailboxes->setEnabled(false);

        // Clear our internal variables
        if (intItemName != NULL) delete intItemName;
        if (intItemDesc != NULL) delete intItemDesc;
        intItemName      = NULL;
        intItemDesc      = NULL;
        intItemActive    = 0;
        intItemPrivate   = 0;
        intShowIncluded  = 0;
        intGLAccount     = 0;
        intNumMailboxes  = 0;
    }
}


/*
** checkForSave - Compares the edited values for our selections with
**                the ones in memory to see if we are going to allow
**                the user to save.
*/

void PKG_General::checkForSave(void)
{
    int     allowSave = 0;
    
    if (intItemName != NULL) {
        if (strcmp(intItemName, itemName->text())) allowSave++;
    }
    
    if (intItemDesc != NULL) {
        if (strcmp(intItemDesc, description->text())) allowSave++;
    }
    
    if (intItemActive != isActive->isChecked()) allowSave++;
    
    if (intItemPrivate != isPrivate->isChecked()) allowSave++;
    
    if (intShowIncluded != showIncluded->isChecked()) allowSave++;

    if (intGLAccount != glAcctIDX[glAccount->currentItem()]) allowSave++;

    if (intNumMailboxes != numMailboxes->cleanText().toInt()) allowSave++;
    
    saveButton->setEnabled(allowSave);
    
}

/*
** save  - Saves the entries in the general tab to the database.
*/

void PKG_General::save()
{
    QApplication::setOverrideCursor(waitCursor);

	ADBTable    PDB;
	PDB.setTableName("Packages");

    PDB.get(myPackageID);
    PDB.setValue("PackageTag", itemName->text());
    PDB.setValue("Description", description->text());
    PDB.setValue("Active", (int)isActive->isChecked());
    PDB.setValue("PackageType", (int)isPrivate->isChecked());
    PDB.setValue("ShowIncluded", (int)showIncluded->isChecked());
    PDB.setValue("IntAccountNo", glAcctIDX[glAccount->currentItem()]);
    PDB.setValue("NumMailboxes", numMailboxes->cleanText().toInt());
    PDB.upd();
    
    setPackageID(myPackageID);
    
    emit(packageChanged());
    
    QApplication::restoreOverrideCursor();
}

