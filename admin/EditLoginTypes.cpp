/*
** $Id$
**
***************************************************************************
**
** EditLoginTypes - 
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
** $Log: EditLoginTypes.cpp,v $
** Revision 1.5  2003/12/24 01:09:22  marc
** Minor changes
**
** Revision 1.4  2003/12/22 23:46:48  marc
** Removed QTArchitect created files from ELT_Flags
**
** Revision 1.3  2003/12/21 20:17:36  marc
** Removed some debug info
**
** Revision 1.2  2003/12/21 20:11:11  marc
** Removed QTArchitecht created dialogs and did them by hand.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "EditLoginTypes.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapplication.h>
#include <qtabdialog.h>
#include <qwidgetstack.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qbuttongroup.h>

#include <BlargDB.h>
#include <ADB.h>

EditLoginTypes::EditLoginTypes
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Edit Login Types" );

    // Create our widgets.  
    loginTypeList = new QListView(this, "loginTypeList");
    //loginTypeList->setColumnText(0, "Login Type");
    loginTypeList->addColumn("Login Type");
    loginTypeList->addColumn("A");
    loginTypeList->setAllColumnsShowFocus(TRUE);
    connect(loginTypeList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(loginTypeSelected(QListViewItem *)));

    theTabBar = new QTabBar(this, "theTabBar");
    theTabBar->setMinimumSize(0, 25);
    theTabBar->setMaximumSize(4096, 25);
    theTabBar->setFocusPolicy(QWidget::StrongFocus);
    theTabBar->addTab(new QTab("&General"));
    theTabBar->addTab(new QTab("&Flags"));
    theTabBar->addTab(new QTab("&Billables"));
    //connect(theTabBar, SIGNAL(selected(int)), this, SLOT(tabSelected(int)));

    QButtonGroup *bGroup = new QButtonGroup(this, "bGroup");
    bGroup->setFocusPolicy(QWidget::NoFocus);

    qws = new QWidgetStack(bGroup, "WidgetStack");
    qws->setFocusPolicy(QWidget::NoFocus);
	connect(theTabBar, SIGNAL(selected(int)), qws, SLOT(raiseWidget(int)));

    // qws is our QWidgetStack.
    tgeneral = new ELT_General(qws);
    qws->addWidget(tgeneral, 0);
    
	tflags = new ELT_Flags(qws);
	qws->addWidget(tflags, 1);

	tbillables = new ELT_Billables(qws);
	qws->addWidget(tbillables, 2);

    QPushButton *newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newLoginType()));

    deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteLoginType()));
    
    QPushButton *closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	
    // Create the layout now.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QBoxLayout *wl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    wl->addWidget(loginTypeList, 0);

    QBoxLayout *sl = new QBoxLayout(QBoxLayout::TopToBottom, 0);
    sl->addWidget(theTabBar, 0);

    QBoxLayout *bgl = new QBoxLayout(bGroup, QBoxLayout::TopToBottom, 2);
    bgl->addWidget(qws, 1);
    sl->addWidget(bGroup, 1);

    wl->addLayout(sl, 1);
    ml->addLayout(wl, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addWidget(newButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);


    loadLoginTypes();
    
    tgeneral->setLoginTypeID((long) 0);
    connect(tgeneral, SIGNAL(loginTypeChanged()), SLOT(updateLoginType()));
    

    // tabd->showPage(tlogins);
}


EditLoginTypes::~EditLoginTypes()
{
}


/*
** loadLoginTypes - Loads the list of login types and fills the list with
**                  them.
*/

void EditLoginTypes::loadLoginTypes()
{
    ADB     DB;
    char    tmpStr[1024];

    loginTypeList->clear();    
    DB.query("select InternalID, LoginType, Active from LoginTypes");
    
    if (DB.rowCount) while (DB.getrow()) {
        if (atoi(DB.curRow["Active"])) {
            sprintf(tmpStr, "Y");
        } else {
            sprintf(tmpStr, "N");
        }
        
        (void) new QListViewItem(loginTypeList, DB.curRow["LoginType"], tmpStr, DB.curRow["InternalID"]);
    }

    for (unsigned int i = 0; i < 3 ; i++) {
        theTabBar->setTabEnabled(i, FALSE);
    }

    theTabBar->setCurrentTab(0);
    deleteButton->setEnabled(FALSE);
}


/*
** loginTypeSelected - Calls each of the tabs so that they load the correct
**                     information when a login type is selected.
*/

void EditLoginTypes::loginTypeSelected(QListViewItem *curItem)
{
    long    typeID = 0;
    if (curItem) {
        for (unsigned int i = 0; i < 3 ; i++) {
            theTabBar->setTabEnabled(i, TRUE);
        }
        typeID = atol(curItem->key(2, 0));
        deleteButton->setEnabled(TRUE);
    } else {
        for (unsigned int i = 0; i < 3 ; i++) {
            theTabBar->setTabEnabled(i, FALSE);
        }
        deleteButton->setEnabled(FALSE);
    }
    theTabBar->setCurrentTab(0);
    tgeneral->setLoginTypeID(typeID);
    tflags->setLoginTypeID(typeID);
    tbillables->setLoginTypeID(typeID);
}

/*
** updateLoginType - A slot that gets called when the General tab emits
**                   loginTypeChanged.  This way we can refresh the current
**                   item without reloading the list.
*/

void EditLoginTypes::updateLoginType()
{
    QListViewItem   *curItem = loginTypeList->currentItem();
    
    if (curItem != NULL) {
        char        tmpStr[1024];
		ADBTable    LTDB;
		LTDB.setTableName("LoginTypes");

	    LTDB.get(atol(curItem->key(2,0)));
	    
        curItem->setText(0, LTDB.getStr("LoginType"));
        if (LTDB.getInt("Active")) {
            strcpy(tmpStr, "Y");
        } else {
            strcpy(tmpStr, "N");
        }
        curItem->setText(1, tmpStr);
    
    }
}


/*
** newLoginType - Inserts a new login type entry in the database and makes
**                it the currently selected one.
*/

void EditLoginTypes::newLoginType()
{
    QListViewItem   *newItem;
    ADBTable    LTDB;
    LTDB.setTableName("LoginTypes");
    
    LTDB.setValue("InternalID", (long) 0);
    LTDB.setValue("LoginType",  "New Login Type");
    LTDB.setValue("Description", "New Login Type");
    LTDB.setValue("DiskSpace",   10);
    LTDB.setValue("DialupChannels", 0);
    LTDB.setValue("DiskSpaceBillable", (long) 0);
    LTDB.setValue("DialupChannelsBillable", (long) 0);
    LTDB.setValue("Active", 0);
    
    LTDB.ins();
    
    newItem = new QListViewItem(loginTypeList, LTDB.getStr("LoginType"), "N", LTDB.getStr("InternalID"));
    loginTypeList->setSelected(newItem, TRUE);
    loginTypeList->setCurrentItem(newItem);
    
}

/*
** deleteLoginType - Checks to see if the login type is in use.  If not
**                   it prompts to verify that the user wishes to delete it.
**                   if it is in use, it tells the user that it cannot be
**                   deleted until it is no longer in use.
*/

void EditLoginTypes::deleteLoginType()
{
    QListViewItem   *curItem = loginTypeList->currentItem();
    // First, make sure that our current item is not null
    if (curItem == NULL) return;
    
    // Okay, now make sure that it is not in use.
    QApplication::setOverrideCursor(waitCursor);
    ADB         DB;
    long        myIntID = atol(curItem->key(2, 0));
    DB.query("select InternalID from Logins where LoginType = %ld", myIntID);
    QApplication::restoreOverrideCursor();
    
    if (DB.rowCount) {
        // This login type is in use.  Let the user know.
        QMessageBox::critical(this, "Delete Login Type", "The login type is in use by customers, and may not be deleted.\n\nUpdate the customer logins before proceeding.");
    } else {
        switch(QMessageBox::information(this, "Delete Login Type", 
          "Are you sure you wish to delete\nthe currently selected login type?",
          "&No", "&Yes", "&Cancel", 0, 2)) {
          
            case 1:     // Delete it.
                QApplication::setOverrideCursor(waitCursor);
                DB.dbcmd("delete from LoginTypeFlags where LoginTypeID = %ld", myIntID);
                DB.dbcmd("delete from LoginTypeBillables where LoginTypeID = %ld", myIntID);
                DB.dbcmd("delete from LoginTypes where InternalID = %ld", myIntID);
                
                // Refresh our list
                loadLoginTypes();
                loginTypeSelected(NULL);
                QApplication::restoreOverrideCursor();
                break;
                
            default:    // Don't delete it
                break;
        }
    }
}
