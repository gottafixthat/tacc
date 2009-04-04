/*
***************************************************************************
**
** ServerGroups - An interface that allows an admin to manage the
** server groups.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <QtCore/QDateTime>
#include <QtGui/QLayout>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <QtGui/QLabel>
#include <Cfg.h>

#include <TAA.h>
#include "ServerGroups.h"
//#include "LogVoiceMail.h"

ServerGroups::ServerGroups
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    setCaption( "Server Groups" );

    sgList = new Q3ListView(this, "Server Groups");
    sgList->setAllColumnsShowFocus(true);
    sgList->setRootIsDecorated(false);
    sgList->addColumn("Server Group");
    sgList->addColumn("Type");
    sgList->addColumn("Description");

    idColumn        = 3;

    connect(sgList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));
    connect(sgList, SIGNAL(returnPressed(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));


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

    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(sgList, 1);

    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

ServerGroups::~ServerGroups()
{
}

/**
  * refreshList - Refreshes the server group list.
  */

void ServerGroups::refreshList()
{
    ADB     myDB;
    ServerTypes     tmpType;

    QApplication::setOverrideCursor(Qt::waitCursor);
    sgList->clear();
    // Get the list of server groups
    myDB.query("select ServerGroupID, ServerGroup, ServerType, Description from ServerGroups");
    if (myDB.rowCount) while (myDB.getrow()) {
        // Walk through the list of server groups and populate it
        tmpType = (ServerTypes) atoi(myDB.curRow["ServerType"]);
        if (tmpType > (ServerTypes) MAX_SERVER_TYPES) tmpType = Unknown;
        new Q3ListViewItem(sgList, myDB.curRow["ServerGroup"], server_types[tmpType].description, myDB.curRow["Description"], myDB.curRow["ServerGroupID"]);
    }

    QApplication::restoreOverrideCursor();
}

/** refreshServerGroup - A slot that connects to the server group editor
  * so the server group that was affected can be refreshed after an edit.
  */
void ServerGroups::refreshServerGroup(long sgID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void ServerGroups::addClicked()
{
    ServerGroupEditor   *ope;
    ope = new ServerGroupEditor();
    ope->show();
    connect(ope, SIGNAL(serverGroupSaved(long)), this, SLOT(refreshServerGroup(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void ServerGroups::editClicked()
{
    Q3ListViewItem   *curItem;
    curItem = sgList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                ServerGroupEditor   *ope;
                ope = new ServerGroupEditor();
                ope->setServerGroupID(tmpID);
                ope->show();
                connect(ope, SIGNAL(serverGroupSaved(long)), this, SLOT(refreshServerGroup(long)));
            }
        }
    }
}

/**
  * itemDoubleClicked - Connected to the server group list, gets called when the user
  * double clicks on an item.
  */
void ServerGroups::itemDoubleClicked(Q3ListViewItem *selItem)
{
    if (selItem) {
        long    tmpID;
        if (selItem->key(idColumn,0).length()) {
            tmpID = atoi(selItem->key(idColumn, 0));
            if (tmpID) {
                ServerGroupEditor   *ope;
                ope = new ServerGroupEditor();
                ope->setServerGroupID(tmpID);
                ope->show();
                connect(ope, SIGNAL(serverGroupSaved(long)), this, SLOT(refreshServerGroup(long)));
            }
        }
    }
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void ServerGroups::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void ServerGroups::closeClicked()
{
    delete this;
}


/**
  * ServerGroupEditor - Allows the user to add or edit a
  * server group.
  */

ServerGroupEditor::ServerGroupEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    setCaption( "Server Group Add" );

    serverGroupID = 0;

    ADB     DB;

    QLabel *serverGroupLabel = new QLabel(this, "serverGroupLabel");
    serverGroupLabel->setText("Server Group:");
    serverGroupLabel->setAlignment(Qt::AlignRight);

    serverGroup = new QLineEdit(this, "serverGroup");

    QLabel *serverTypeLabel = new QLabel(this, "serverTypeLabel");
    serverTypeLabel->setText("Server Type:");
    serverTypeLabel->setAlignment(Qt::AlignRight);

    serverType = new QComboBox(this, "serverType");
    int i = 0;
    while (server_types[i].servertype >= 0) {
        serverType->insertItem(server_types[i].description);
        i++;
    }

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    description = new Q3MultiLineEdit(this, "description");

    QLabel *databaseHostLabel = new QLabel(this, "databaseHostLabel");
    databaseHostLabel->setText("Database Host:");
    databaseHostLabel->setAlignment(Qt::AlignRight);

    databaseHost = new QLineEdit(this, "databaseHost");

    QLabel *databaseNameLabel = new QLabel(this, "databaseNameLabel");
    databaseNameLabel->setText("Database Name:");
    databaseNameLabel->setAlignment(Qt::AlignRight);

    databaseName = new QLineEdit(this, "databaseName");

    QLabel *databaseUserLabel = new QLabel(this, "databaseUserLabel");
    databaseUserLabel->setText("Database User:");
    databaseUserLabel->setAlignment(Qt::AlignRight);

    databaseUser = new QLineEdit(this, "databaseUser");

    QLabel *databasePass1Label = new QLabel(this, "databasePass1Label");
    databasePass1Label->setText("Database Password:");
    databasePass1Label->setAlignment(Qt::AlignRight);

    databasePass1 = new QLineEdit(this, "databasePass1");
    databasePass1->setEchoMode(QLineEdit::Password);

    QLabel *databasePass2Label = new QLabel(this, "databasePass2Label");
    databasePass2Label->setText("Verify Password:");
    databasePass2Label->setAlignment(Qt::AlignRight);

    databasePass2 = new QLineEdit(this, "databasePass2");
    databasePass2->setEchoMode(QLineEdit::Password);

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3GridLayout *gl = new Q3GridLayout(2, 3);
    int curRow = 0;
    gl->addWidget(serverGroupLabel,         curRow, 0);
    gl->addWidget(serverGroup,              curRow, 1);
    curRow++;
    gl->addWidget(serverTypeLabel,          curRow, 0);
    gl->addWidget(serverType,               curRow, 1);
    curRow++;
    gl->addWidget(descriptionLabel,         curRow, 0);
    gl->addWidget(description,              curRow, 1);
    curRow++;
    gl->addWidget(databaseHostLabel,        curRow, 0);
    gl->addWidget(databaseHost,             curRow, 1);
    curRow++;
    gl->addWidget(databaseNameLabel,        curRow, 0);
    gl->addWidget(databaseName,             curRow, 1);
    curRow++;
    gl->addWidget(databaseUserLabel,        curRow, 0);
    gl->addWidget(databaseUser,             curRow, 1);
    curRow++;
    gl->addWidget(databasePass1Label,       curRow, 0);
    gl->addWidget(databasePass1,            curRow, 1);
    curRow++;
    gl->addWidget(databasePass2Label,       curRow, 0);
    gl->addWidget(databasePass2,            curRow, 1);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~ServerGroupEditor - Destructor.
  */
ServerGroupEditor::~ServerGroupEditor()
{
}

/**
  * setServerGroupID - Activates the "edit" mode of the editor.
  * Loads up the specified server group ID and puts its values into
  * the form.
  * Returns true if the operation was successful, otherwise false.
  */
int ServerGroupEditor::setServerGroupID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("ServerGroups");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        serverGroup->setText(DB.getStr("ServerGroup"));
        serverType->setCurrentItem(DB.getInt("ServerType"));
        description->setText(DB.getStr("Description"));
        databaseHost->setText(DB.getStr("DBHost"));
        databaseName->setText(DB.getStr("DBName"));
        databaseUser->setText(DB.getStr("DBUser"));
        databasePass1->setText(DB.getStr("DBPass"));
        databasePass2->setText(DB.getStr("DBPass"));

        retVal = 1;
        serverGroupID = newID;
    }

    setCaption( "Server Group Edit" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void ServerGroupEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;

    // Validate the server group name.
    if (serverGroup->text().length() < 2 || serverGroup->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Server Group", "Server group names must be between 2 and 64 characters long.");
        return;
    }
    DB.query("select ServerGroup from ServerGroups where ServerGroup = '%s' and ServerGroupID <> %ld", (const char *)serverGroup->text(), serverGroupID);
    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Server Group", "Server group names must be unique.");
        return;
    }

    // Validate the database host
    if (databaseHost->text().length() < 2 || databaseHost->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Server Group", "Database host name must be between 2 and 64 characters long.");
        return;
    }

    if (databaseName->text().length() < 2 || databaseName->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Server Group", "Database name must be between 2 and 64 characters long.");
        return;
    }

    if (databaseUser->text().length() < 2 || databaseUser->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Server Group", "Database user must be between 2 and 64 characters long.");
        return;
    }

    if (databasePass1->text().length() < 2 || databasePass1->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Server Group", "Database password must be between 2 and 64 characters long.");
        return;
    }

    if (strcmp(databasePass1->text(), databasePass2->text())) {
        QMessageBox::critical(this, "Unable to save Server Group", "Database passwords do not match.");
        return;
    }

    // Try connecting to the database they specified, and if we can't, give them a warning
    // but save anyway.
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     tmpDB((const char *)databaseName->text(), (const char *)databaseUser->text(), (const char *)databasePass1->text(), (const char *)databaseHost->text());
    QApplication::restoreOverrideCursor();
    if (!tmpDB.Connected()) {
        QMessageBox::warning(this, "Database Connection Problem", "Warning:  Unable to connect to the specified database.");
    }

    // Now check to see if we are updating or adding.
    ADBTable    sgDB("ServerGroups");
    if (serverGroupID) {
        sgDB.get(serverGroupID);
    }

    sgDB.setValue("ServerGroup",    (const char *)serverGroup->text());
    sgDB.setValue("ServerType",     serverType->currentItem());
    sgDB.setValue("Description",    (const char *)description->text());
    sgDB.setValue("DBHost",         (const char *)databaseHost->text());
    sgDB.setValue("DBName",         (const char *)databaseName->text());
    sgDB.setValue("DBUser",         (const char *)databaseUser->text());
    sgDB.setValue("DBPass",         (const char *)databasePass1->text());

    if (serverGroupID) {
        sgDB.upd();
    } else {
        sgDB.ins();
        serverGroupID = sgDB.getLong("ServerGroupID");
    }

    emit(serverGroupSaved(serverGroupID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void ServerGroupEditor::cancelClicked()
{
    delete this;
}


/**
  * ServerGroupSelector - Allows the user to add or remove server groups
  * using a two pane list widget.
  */

ServerGroupSelector::ServerGroupSelector
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    ADB     DB;

    // Server groups.
    available = new Q3ListView(this, "serverGroupsAvailable");
    available->addColumn("Available Server Groups");
    idColumn = 1;
    connect(available, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(availableDoubleClicked(Q3ListViewItem *)));
    DB.query("select ServerGroupID, ServerGroup from ServerGroups");
    if (DB.rowCount) while (DB.getrow()) {
        new Q3ListViewItem(available, DB.curRow["ServerGroup"], DB.curRow["ServerGroupID"]);
    }

    addButton = new QPushButton(this, "addButton");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    rmButton = new QPushButton(this, "rmButton");
    rmButton->setText("&Remove");
    connect(rmButton, SIGNAL(clicked()), this, SLOT(rmClicked()));

    assigned = new Q3ListView(this, "assigned");
    assigned->addColumn("Assigned Server Groups");
    connect(assigned, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(assignedDoubleClicked(Q3ListViewItem *)));

    // Create a layout for the server groups
    Q3BoxLayout *sgl = new Q3BoxLayout(this, Q3BoxLayout::LeftToRight, 3, 3);
    sgl->addWidget(available, 1);

    Q3BoxLayout *sgbl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 3);
    sgbl->addStretch(1);
    sgbl->addWidget(addButton, 0);
    sgbl->addWidget(rmButton, 0);
    sgbl->addStretch(1);

    sgl->addLayout(sgbl, 0);

    sgl->addWidget(assigned, 1);

}

ServerGroupSelector::~ServerGroupSelector()
{
}

/**
  * setAddButtonText - Lets the caller set the text of the add button.
  */
void ServerGroupSelector::setAddButtonText(const char *newText)
{
    addButton->setText(newText);
}

/**
  * setRemoveButtonText - Lets the caller set the text of the remove button.
  */
void ServerGroupSelector::setRemoveButtonText(const char *newText)
{
    rmButton->setText(newText);
}

/**
  * reset - Resets all of the assigned items back to available.
  */
void ServerGroupSelector::reset()
{
    Q3ListViewItem   *curItem;

    // Find the item in available list
    curItem = assigned->firstChild();
    if (curItem) {
        while(curItem) {
            new Q3ListViewItem(available, curItem->key(0,0), curItem->key(1,0));
            curItem = curItem->itemBelow();
        }
        assigned->clear();
    }
}

/**
  * assign - Moves an item from the available to the assigned list.
  */
void ServerGroupSelector::assign(long serverGroupID)
{
    Q3ListViewItem   *curItem;

    // Find the item in available list
    curItem = available->firstChild();
    while(curItem) {
        if (serverGroupID == atoi(curItem->key(idColumn,0))) {
            new Q3ListViewItem(assigned, curItem->key(0,0), curItem->key(1,0));
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
void ServerGroupSelector::unassign(long serverGroupID)
{
    Q3ListViewItem   *curItem;

    // Find the item in available list
    curItem = assigned->firstChild();
    while(curItem) {
        if (serverGroupID == atoi(curItem->key(idColumn,0))) {
            Q3ListViewItem *tmpItem = new Q3ListViewItem(available, curItem->key(0,0), curItem->key(1,0));
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
long *ServerGroupSelector::getAssigned()
{
    Q3ListViewItem   *curItem;
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
void ServerGroupSelector::addClicked()
{
    Q3ListViewItem *curItem = available->currentItem();
    if (curItem) assign(atol(curItem->key(idColumn,0)));
}

/**
  * rmClicked - Gets called when the Remove button is clicked.
  */
void ServerGroupSelector::rmClicked()
{
    Q3ListViewItem *curItem = assigned->currentItem();
    if (curItem) unassign(atol(curItem->key(idColumn,0)));
}

/**
  * availableDoubleClicked - Gets called when the user double
  * clicks on an available server group.
  */
void ServerGroupSelector::availableDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) assign(atol(curItem->key(idColumn,0)));
}

/**
  * assignedDoubleClicked - Gets called when the user double
  * clicks on an assigned server group.
  */
void ServerGroupSelector::assignedDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) unassign(atol(curItem->key(idColumn,0)));
}

