/*
** $Id$
**
***************************************************************************
**
** UserPrivs.cpp  -  UserPrivs is the widget that will allow the system
**                   administrator to create and delete accounts, as well
**                   as change their passwords and such.
**
***************************************************************************
** Written by R. Marc Lewis, 
**     under contract to The Public Defender Association
***************************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <ADB.h>

#include <TAATools.h>

#include "UserPrivs.h"

#define IntIDCol 2

UserPrivs::UserPrivs
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("User Administration");

    // Create our widgets
    userList = new Q3ListView(this);
    userList->addColumn("User Name");
    userList->addColumn("Access");
    userList->setGeometry(5,5,150,250);
    userList->setMinimumSize(150,0);
    userList->setMaximumSize(150,32767);
    userList->setAllColumnsShowFocus(true);
    connect(userList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(userSelected(Q3ListViewItem *)));
    connect(userList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(userSelected(Q3ListViewItem *)));

    editArea = new UserEditor(this);

    // Create our layout.
    Q3BoxLayout *mainLayout = new Q3BoxLayout(this, Q3BoxLayout::LeftToRight, 2, 2);
    mainLayout->addWidget(userList, 0);
    mainLayout->addWidget(editArea, 1);

    //setGeometry(5,5,575,300);
    
    myCurrentID = 0;

    refreshList();

    connect(editArea, SIGNAL(userUpdated(long)), this, SLOT(userUpdated(long)));
}

UserPrivs::~UserPrivs()
{
}

/*
** userSelected - Gets called when the user clicks on a user entry in the
**                list.  It pulls up their information from the database.
*/

void UserPrivs::userSelected(Q3ListViewItem* curItem)
{
    if (curItem) {
        // Get load the current user into the editor form.
        editArea->loadUser(atoi(curItem->key(IntIDCol, 0)));
    } else {
        editArea->loadUser(0);
    }
}

/*
** refreshList - Refreshes the user list.  If we have one selected already
**               then we will find it again in the list, if not, we will
**               highlight the first one in the list.
*/

void UserPrivs::refreshList()
{
    ADB             DB;
    char            levelStr[1024];
    AccessLevels    level;
    Q3ListViewItem   *curItem;
    bool            foundSel = false;

    userList->clear();
    DB.query("select * from Staff");
    if (DB.rowCount) {
        while(DB.getrow()) {
            level = (AccessLevels) atoi(DB.curRow["AccessLevel"]);
            switch(level) {
                case Admin:
                    strcpy(levelStr, "Administrator");
                    break;

                case Manager:
                    strcpy(levelStr, "Manager");
                    break;

                case Staff:
                    strcpy(levelStr, "Support Staff");
                    break;

                default:
                    strcpy(levelStr, "Unknown");
                    break;
            }

            curItem = new Q3ListViewItem(userList, DB.curRow["LoginID"], levelStr, DB.curRow["InternalID"]);

            // Is this the user we previously had hilighted?  If so,
            // hilight it now.
            if (myCurrentID == atol(DB.curRow["InternalID"])) {
                foundSel = true;
                userList->setCurrentItem(curItem);
                userList->setSelected(curItem, true);
                userList->ensureItemVisible(curItem);
            }
        }
    }

    // If we didn't find our previously hilighted entry, hilight the
    // first entry in the list.
    if (!foundSel) {
        // None selected, hilight the first one in the list.
        if (userList->firstChild()) {
            userList->setCurrentItem(userList->firstChild());
            userList->setSelected(userList->firstChild(), true);
            userList->ensureItemVisible(userList->firstChild());
        }
    }
}

/*
** userUpdated - A slot that we tie to the UserEditor to refresh things
**               when the user changes something.
*/

void UserPrivs::userUpdated(long newCurrent)
{
    myCurrentID = newCurrent;
    refreshList();
    userList->setFocus();
}

