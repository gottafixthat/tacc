/*
** $Id: ELT_Flags.cpp,v 1.3 2003/12/23 14:59:18 marc Exp $
**
***************************************************************************
**
** ELT_Flags - Edit Login Types Flags page
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
** $Log: ELT_Flags.cpp,v $
** Revision 1.3  2003/12/23 14:59:18  marc
** Minor updates to work with the new dialogs.
**
** Revision 1.2  2003/12/22 23:46:48  marc
** Removed QTArchitect created files from ELT_Flags
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "ELT_Flags.h"

#include <stdio.h>
#include <stdlib.h>

#include <BlargDB.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlayout.h>

#include "LoginTypeFlagAdd.h"
#include <ADB.h>

ELT_Flags::ELT_Flags
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Create our widgets.
    QLabel *flagListLabel = new QLabel(this, "flagListLabel");
    flagListLabel->setText("Flags:");
    flagListLabel->setAlignment(AlignRight|AlignTop);

    flagList = new QListView(this, "flagList");
    flagList->addColumn("Name");
    flagList->addColumn("Value");
    connect(flagList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(itemSelected(QListViewItem *)));

    QLabel *flagValueLabel = new QLabel(this, "flagValueLabel");
    flagValueLabel->setText("Value:");
    flagValueLabel->setAlignment(AlignRight|AlignVCenter);

    flagValue = new QLineEdit(this, "flagValue");
    flagValue->setMaxLength(80);
    connect(flagValue, SIGNAL(textChanged(const QString &)), this, SLOT(flagValueChanged(const QString &)));

    setValueButton = new QPushButton(this, "setValueButton");
    setValueButton->setText("&Set Value");
    connect(setValueButton, SIGNAL(clicked()), this, SLOT(setValue()));

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addFlag()));

    removeButton = new QPushButton(this, "removeButton");
    removeButton->setText("&Remove");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeFlag()));

    // Create our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout(2, 3);
    gl->setColStretch(0, false);
    gl->setColStretch(1, true);
    gl->setColStretch(2, false);

    int curRow = 0;
    gl->addWidget(flagListLabel,        curRow, 0);
    gl->addMultiCellWidget(flagList,    curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, true);
    curRow++;
    gl->addWidget(flagValueLabel,       curRow, 0);
    gl->addWidget(flagValue,            curRow, 1);
    gl->addWidget(setValueButton,       curRow, 2);
    gl->setRowStretch(curRow, false);

    ml->addLayout(gl, 0);

    // Our button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(removeButton, 0);
    
    ml->addLayout(bl, 0);

    
    // We start out by disabling all of our buttons and fields.
    // By setting the Login Type ID to zero.
    setLoginTypeID((long) 0);

    flagList->setAllColumnsShowFocus(TRUE);
}


ELT_Flags::~ELT_Flags()
{
}


/*
** setLoginTypeID - Sets the current login type ID.
**                  Loads the information from the database.
*/

void ELT_Flags::setLoginTypeID(long newLoginTypeID)
{
    myLoginTypeID = newLoginTypeID;
    flagList->clear();
    if (newLoginTypeID) {
        ADB         DB;
        DB.query("select InternalID, Tag, Value from LoginTypeFlags where LoginTypeID = %ld", newLoginTypeID);
        if (DB.rowCount) {
            // Enable/disable buttons and stuff here...
            while (DB.getrow()) { 
                (void) new QListViewItem(flagList, DB.curRow["Tag"], DB.curRow["Value"], DB.curRow["InternalID"]);
            }
        } 
    }
    
    itemSelected(flagList->currentItem());
}

/*
** itemSelected - Gets called whenever the user hilights an item from the
**                list.
*/

void ELT_Flags::itemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        flagValue->setText(curItem->key(1, 0));
        flagValue->setEnabled(TRUE);
        
        // We only want this to be enabled if the user changed the value.
        setValueButton->setEnabled(TRUE);
        removeButton->setEnabled(TRUE);
    } else {
        flagValue->setText("");
        flagValue->setEnabled(true);
        setValueButton->setEnabled(true);
        removeButton->setEnabled(true);
    }
    
    if (myLoginTypeID) addButton->setEnabled(TRUE);
    else addButton->setEnabled(true);
}


/*
** flagValueChanged - We get this signal whenever the user changes the 
**                    value of the selected item.  When this happens we
**                    enable the "Set Value" button so the user can save it.
*/

void ELT_Flags::flagValueChanged(const QString &newVal)
{
    if (flagList->currentItem() != NULL) {
        //setValueButton->setEnabled(strcmp(flagList->currentItem()->key(1,0), newVal));
        setValueButton->setEnabled(true);
    } else {
        setValueButton->setEnabled(true);
    }
}

/*
** setValue - Stores the new value that the user has input into the database.
*/

void ELT_Flags::setValue()
{
    QApplication::setOverrideCursor(waitCursor);
    
    ADBTable    FDB;
    FDB.setTableName("LoginTypeFlags");
    
    FDB.get(atol(flagList->currentItem()->key(2,0)));
    FDB.setValue("Value", flagValue->text());
    FDB.upd();
    
    flagList->currentItem()->setText(1, flagValue->text());
    
    flagValueChanged(flagValue->text());
    
    QApplication::restoreOverrideCursor();
}


/*
** removeFlag - Prompts the user to ensure that they wish to remove the
**              currently selected flag.  If so, it is removed.
*/

void ELT_Flags::removeFlag()
{
    QListViewItem   *curItem = flagList->currentItem();
    if (curItem == NULL) return;
    
    char    *tmpStr = new char[4096];
    ADB     DB;
    
    sprintf(tmpStr, "Are you sure you wish to\nremove the '%s' flag?", (const char *) curItem->key(0,0));
    switch(QMessageBox::information(this, "Remove Flag", tmpStr, "&No", "&Yes", "&Cancel", 0, 2)) {
        case 1:     // Remove the flag
            DB.dbcmd("delete from LoginTypeFlags where InternalID = %ld", atol(curItem->key(2,0)));
            setLoginTypeID(myLoginTypeID);
            break;
            
        default:    // Don't remove the flag
            break;
    }
    
}


/*
** addFlag   - Gives the user a list of available flags that may be added
**             and allows them to add one.
*/

void ELT_Flags::addFlag()
{
    if (!myLoginTypeID) return;
    
    LoginTypeFlagAdd    *ltfa;
    
    ltfa = new LoginTypeFlagAdd(0, "", myLoginTypeID);
    ltfa->exec();
    if (ltfa->result()) setLoginTypeID(myLoginTypeID);
    delete ltfa;
}
