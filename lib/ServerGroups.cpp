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

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <Cfg.h>

#include <TAA.h>
#include "ServerGroups.h"
//#include "LogVoiceMail.h"

ServerGroups::ServerGroups
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Server Groups" );

    sgList = new QListView(this, "Server Groups");
    sgList->setAllColumnsShowFocus(true);
    sgList->setRootIsDecorated(false);
    sgList->addColumn("Server Group");
    sgList->addColumn("ID");
    sgList->addColumn("Description");

    idColumn        = 1;

    sgList->setColumnAlignment(idColumn, Qt::AlignRight);


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
    ml->addWidget(sgList, 1);

    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
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

    QApplication::setOverrideCursor(Qt::waitCursor);
    sgList->clear();
    // Get the list of server groups
    myDB.query("select ServerGroupID, ServerGroup, Description from ServerGroups");
    if (myDB.rowCount) while (myDB.getrow()) {
        // Walk through the list of server groups and populate it
        QListViewItem   *curItem = new QListViewItem(sgList, myDB.curRow["ServerGroup"], myDB.curRow["ServerGroupID"], myDB.curRow["Description"]);
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
    QListViewItem   *curItem;
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
) : TAAWidget( parent, name )
{
    setCaption( "Server Group Add" );

    serverGroupID = 0;

    ADB     DB;

    QLabel *serverGroupLabel = new QLabel(this, "serverGroupLabel");
    serverGroupLabel->setText("Server Group:");
    serverGroupLabel->setAlignment(Qt::AlignRight);

    serverGroup = new QLineEdit(this, "serverGroup");

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    description = new QMultiLineEdit(this, "description");

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

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(serverGroupLabel,         curRow, 0);
    gl->addWidget(serverGroup,              curRow, 1);
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
    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
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
    long    vendorID = 0;

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

