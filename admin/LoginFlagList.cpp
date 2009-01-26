/*
***************************************************************************
**
** LoginFlagsList - Manages the login flags that can be associated with
** login types.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <Cfg.h>

#include <TAA.h>
#include "LoginFlagList.h"

LoginFlagList::LoginFlagList
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Login Flags" );

    flagList = new QListView(this, "Login Flags");
    flagList->setAllColumnsShowFocus(true);
    flagList->setRootIsDecorated(false);
    flagList->addColumn("Login Flag");
    flagList->addColumn("Description");
    flagList->addColumn("Default Value");
    flagList->addColumn("Bool");

    idColumn        = 4;

    connect(flagList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(itemDoubleClicked(QListViewItem *)));
    connect(flagList, SIGNAL(returnPressed(QListViewItem *)), this, SLOT(itemDoubleClicked(QListViewItem *)));


    addButton = new QPushButton(this, "Add Button");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    editButton = new QPushButton(this, "Edit Button");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editClicked()));

    deleteButton = new QPushButton(this, "Delete Button");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    deleteButton->setEnabled(false);

    closeButton = new QPushButton(this, "Close Button");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(flagList, 1);

    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

LoginFlagList::~LoginFlagList()
{
}

/**
  * refreshList - Refreshes the server group list.
  */

void LoginFlagList::refreshList()
{
    ADB     myDB;

    QApplication::setOverrideCursor(Qt::waitCursor);
    flagList->clear();
    // Get the list of server groups
    myDB.query("select InternalID, LoginFlag, Description, DefaultValue, IsBool from LoginFlags");
    if (myDB.rowCount) while (myDB.getrow()) {
        // Walk through the list of server groups and populate it
        QString boolText = "No";
        if (atoi(myDB.curRow["IsBool"])) boolText = "Yes";
        new QListViewItem(flagList, myDB.curRow["LoginFlag"], myDB.curRow["Description"], myDB.curRow["DefaultValue"], boolText, myDB.curRow["InternalID"]);
    }

    QApplication::restoreOverrideCursor();
}

/** refreshLoginFlag - A slot that connects to the server group editor
  * so the server group that was affected can be refreshed after an edit.
  */
void LoginFlagList::refreshLoginFlag(long intID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void LoginFlagList::addClicked()
{
    LoginFlagEditor   *ope;
    ope = new LoginFlagEditor();
    ope->show();
    connect(ope, SIGNAL(loginFlagSaved(long)), this, SLOT(refreshLoginFlag(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void LoginFlagList::editClicked()
{
    QListViewItem   *curItem;
    curItem = flagList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                LoginFlagEditor   *ope;
                ope = new LoginFlagEditor();
                ope->setLoginFlagID(tmpID);
                ope->show();
                connect(ope, SIGNAL(loginFlagSaved(long)), this, SLOT(refreshLoginFlag(long)));
            }
        }
    }
}

/**
  * itemDoubleClicked - Connected to the server group list, gets called when the user
  * double clicks on an item.
  */
void LoginFlagList::itemDoubleClicked(QListViewItem *selItem)
{
    if (selItem) {
        long    tmpID;
        if (selItem->key(idColumn,0).length()) {
            tmpID = atoi(selItem->key(idColumn, 0));
            if (tmpID) {
                LoginFlagEditor   *ope;
                ope = new LoginFlagEditor();
                ope->setLoginFlagID(tmpID);
                ope->show();
                connect(ope, SIGNAL(loginFlagSaved(long)), this, SLOT(refreshLoginFlag(long)));
            }
        }
    }
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void LoginFlagList::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void LoginFlagList::closeClicked()
{
    delete this;
}


/**
  * LoginFlagEditor - Allows the user to add or edit a
  * server group.
  */

LoginFlagEditor::LoginFlagEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Add Login Flag" );

    loginFlagID = 0;

    ADB     DB;

    QLabel *loginFlagLabel = new QLabel(this, "loginFlagLabel");
    loginFlagLabel->setText("Login Flag:");
    loginFlagLabel->setAlignment(Qt::AlignRight);

    loginFlag = new QLineEdit(this, "loginFlag");

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    description = new QMultiLineEdit(this, "description");

    QLabel *defaultValueLabel = new QLabel(this, "defaultValueLabel");
    defaultValueLabel->setText("Default Value:");
    defaultValueLabel->setAlignment(Qt::AlignRight);

    defaultValue = new QLineEdit(this, "defaultValue");

    isBool = new QCheckBox(this, "Boolean");
    isBool->setText("Boolean");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(loginFlagLabel,           curRow, 0);
    gl->addWidget(loginFlag,                curRow, 1);
    curRow++;
    gl->addWidget(descriptionLabel,         curRow, 0);
    gl->addWidget(description,              curRow, 1);
    curRow++;
    gl->addWidget(defaultValueLabel,        curRow, 0);
    gl->addWidget(defaultValue,             curRow, 1);
    curRow++;
    gl->addWidget(isBool,                   curRow, 1);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~LoginFlagEditor - Destructor.
  */
LoginFlagEditor::~LoginFlagEditor()
{
}

/**
  * setLoginFlagID - Activates the "edit" mode of the editor.
  * Loads up the specified login flag by its InternalID and puts its
  * values into the form.
  * Returns true if the operation was successful, otherwise false.
  */
int LoginFlagEditor::setLoginFlagID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("LoginFlags");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        loginFlag->setText(DB.getStr("LoginFlag"));
        description->setText(DB.getStr("Description"));
        defaultValue->setText(DB.getStr("DefaultValue"));
        if (atoi(DB.getStr("IsBool"))) isBool->setChecked(true);
        else isBool->setChecked(false);

        retVal = 1;
        loginFlagID = newID;
    }

    setCaption( "Edit Login Flag" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void LoginFlagEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;

    // Validate the server group name.
    if (loginFlag->text().length() < 1 || loginFlag->text().length() > 16) {
        QMessageBox::critical(this, "Unable to save Login Flag", "Login Flags must be between 1 and 16 characters long.");
        return;
    }
    DB.query("select LoginFlag from LoginFlags where LoginFlag = '%s' and InternalID <> %ld", (const char *)loginFlag->text(), loginFlagID);
    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Login Flag", "Login Flags must be unique.");
        return;
    }


    // Now check to see if we are updating or adding.
    ADBTable    sgDB("LoginFlags");
    if (loginFlagID) {
        sgDB.get(loginFlagID);
    }

    int tmpBool = 0;
    if (isBool->isChecked()) tmpBool = 1;

    sgDB.setValue("LoginFlag",      (const char *)loginFlag->text());
    sgDB.setValue("Description",    (const char *)description->text());
    sgDB.setValue("DefaultValue",   (const char *)defaultValue->text());
    sgDB.setValue("IsBool",         tmpBool);

    if (loginFlagID) {
        sgDB.upd();
    } else {
        sgDB.ins();
        loginFlagID = sgDB.getLong("InternalID");
    }

    emit(loginFlagSaved(loginFlagID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void LoginFlagEditor::cancelClicked()
{
    delete this;
}


/**
  * LoginFlagSelector - Allows the user to add or remove server groups
  * using a two pane list widget.
  */

LoginFlagSelector::LoginFlagSelector
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    ADB     DB;

    // Server groups.
    available = new QListView(this, "serverGroupsAvailable");
    available->addColumn("Available Server Groups");
    idColumn = 1;
    connect(available, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(availableDoubleClicked(QListViewItem *)));
    DB.query("select ServerGroupID, ServerGroup from ServerGroups");
    if (DB.rowCount) while (DB.getrow()) {
        new QListViewItem(available, DB.curRow["ServerGroup"], DB.curRow["ServerGroupID"]);
    }

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    rmButton = new QPushButton(this, "rmButton");
    rmButton->setText("&Remove");
    connect(rmButton, SIGNAL(clicked()), this, SLOT(rmClicked()));

    assigned = new QListView(this, "assigned");
    assigned->addColumn("Assigned Server Groups");
    connect(assigned, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(assignedDoubleClicked(QListViewItem *)));

    // Create a layout for the server groups
    QBoxLayout *sgl = new QBoxLayout(this, QBoxLayout::LeftToRight, 3, 3);
    sgl->addWidget(available, 1);

    QBoxLayout *sgbl = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    sgbl->addStretch(1);
    sgbl->addWidget(addButton, 0);
    sgbl->addWidget(rmButton, 0);
    sgbl->addStretch(1);

    sgl->addLayout(sgbl, 0);

    sgl->addWidget(assigned, 1);

}

LoginFlagSelector::~LoginFlagSelector()
{
}

/**
  * setAddButtonText - Lets the caller set the text of the add button.
  */
void LoginFlagSelector::setAddButtonText(const char *newText)
{
    addButton->setText(newText);
}

/**
  * setRemoveButtonText - Lets the caller set the text of the remove button.
  */
void LoginFlagSelector::setRemoveButtonText(const char *newText)
{
    rmButton->setText(newText);
}

/**
  * reset - Resets all of the assigned items back to available.
  */
void LoginFlagSelector::reset()
{
    QListViewItem   *curItem;

    // Find the item in available list
    curItem = assigned->firstChild();
    if (curItem) {
        while(curItem) {
            new QListViewItem(available, curItem->key(0,0), curItem->key(1,0));
            curItem = curItem->itemBelow();
        }
        assigned->clear();
    }
}

/**
  * assign - Moves an item from the available to the assigned list.
  */
void LoginFlagSelector::assign(long serverGroupID)
{
    QListViewItem   *curItem;

    // Find the item in available list
    curItem = available->firstChild();
    while(curItem) {
        if (serverGroupID == atoi(curItem->key(idColumn,0))) {
            new QListViewItem(assigned, curItem->key(0,0), curItem->key(1,0));
            available->removeItem(curItem);
            curItem = 0;
        } else {
            curItem = curItem->itemBelow();
        }
    }
}

/**
  * unassign - Moves an item from the assigned to the available list.
  */
void LoginFlagSelector::unassign(long serverGroupID)
{
    QListViewItem   *curItem;

    // Find the item in available list
    curItem = assigned->firstChild();
    while(curItem) {
        if (serverGroupID == atoi(curItem->key(idColumn,0))) {
            QListViewItem *tmpItem = new QListViewItem(available, curItem->key(0,0), curItem->key(1,0));
            assigned->removeItem(curItem);
            curItem = 0;
        } else {
            curItem = curItem->itemBelow();
        }
    }
}

/**
  * getAssigned - Returns an array of ServerGroupID's containing the list of
  * ids in the assigned list.
  * The list will end with "0".
  */
long *LoginFlagSelector::getAssigned()
{
    QListViewItem   *curItem;
    long    *retVal = new long[assigned->childCount()+2];
    int     count = 0;

    retVal[count] = 0;

    // Find the item in available list
    curItem = assigned->firstChild();
    while(curItem) {
        retVal[count] = atol(curItem->key(idColumn,0));
        count++;
        retVal[count] = 0;
        curItem = curItem->itemBelow();
    }

    return (retVal);
}

/**
  * addClicked - Gets called when the Add button is clicked.
  */
void LoginFlagSelector::addClicked()
{
    QListViewItem *curItem = available->currentItem();
    if (curItem) assign(atol(curItem->key(idColumn,0)));
}

/**
  * rmClicked - Gets called when the Remove button is clicked.
  */
void LoginFlagSelector::rmClicked()
{
    QListViewItem *curItem = assigned->currentItem();
    if (curItem) unassign(atol(curItem->key(idColumn,0)));
}

/**
  * availableDoubleClicked - Gets called when the user double
  * clicks on an available server group.
  */
void LoginFlagSelector::availableDoubleClicked(QListViewItem *curItem)
{
    if (curItem) assign(atol(curItem->key(idColumn,0)));
}

/**
  * assignedDoubleClicked - Gets called when the user double
  * clicks on an assigned server group.
  */
void LoginFlagSelector::assignedDoubleClicked(QListViewItem *curItem)
{
    if (curItem) unassign(atol(curItem->key(idColumn,0)));
}

