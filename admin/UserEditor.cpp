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
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3Frame>

#include <ADB.h>
#include <TAATools.h>

#include "UserEditor.h"

using namespace Qt;

UserEditor::UserEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("User Editor");

    // Create our widgets
    QLabel  *loginIDLabel = new QLabel(this);
    loginIDLabel->setText("Login ID:");
    loginIDLabel->setAlignment(AlignRight);

    loginID = new QLineEdit(this, "LoginID");
    loginID->setMaxLength(16);

    QLabel  *accessLabel = new QLabel(this);
    accessLabel->setText("Access Level:");
    accessLabel->setAlignment(AlignRight);

    accessLevel = new QComboBox(false, this, "Access Level");
    accessLevel->insertItem("Administrator");
    accessLevel->insertItem("Manager");
    accessLevel->insertItem("Staff");

    userActive = new QCheckBox(this, "Account Active");
    userActive->setText("Account Active");
    userActive->setChecked(false);

    QLabel  *passwordLabel1 = new QLabel(this);
    passwordLabel1->setText("Password:");
    passwordLabel1->setAlignment(AlignRight);

    password1 = new QLineEdit(this, "Password1");
    password1->setMaxLength(32);
    password1->setEchoMode(QLineEdit::Password);
    
    QLabel  *passwordLabel2 = new QLabel(this);
    passwordLabel2->setText("Password (repeat to change):");
    passwordLabel2->setAlignment(AlignRight);
    
    password2 = new QLineEdit(this, "Password2");
    password2->setMaxLength(32);
    password2->setEchoMode(QLineEdit::Password);

    QLabel *telephoneLabel  = new QLabel(this);
    telephoneLabel->setText("Phone ID:");
    telephoneLabel->setAlignment(AlignRight);

    telephone = new QLineEdit(this, "telephone");
    telephone->setMaxLength(60);

    QLabel *extensionLabel = new QLabel(this);
    extensionLabel->setText("Extension:");
    extensionLabel->setAlignment(AlignRight);

    extension = new QLineEdit(this, "extension");
    extension->setMaxLength(10);

    QLabel *agentIDLabel = new QLabel(this);
    agentIDLabel->setText("Agent ID:");
    agentIDLabel->setAlignment(AlignRight);

    agentID = new QLineEdit(this, "agentID");
    agentID->setMaxLength(20);

    QLabel *queueNameLabel = new QLabel(this);
    queueNameLabel->setText("Queue Name:");
    queueNameLabel->setAlignment(AlignRight);

    queueName = new QLineEdit(this, "queueName");
    queueName->setMaxLength(60);

    sendTicketNotifications = new QCheckBox(this, "Ticket Notifications");
    sendTicketNotifications->setText("Ticket Notifications");
    sendTicketNotifications->setChecked(false);

    // We also need an Add/Save button
    Q3HButtonGroup *bg = new Q3HButtonGroup(this);

    delButton = new QPushButton(bg, "DeleteButton");
    delButton->setText("&Delete");
    delButton->setEnabled(false);
    connect(delButton, SIGNAL(clicked()), this, SLOT(deleteUser()));
    
    newButton = new QPushButton(bg, "NewButton");
    newButton->setText("&New");
    newButton->setEnabled(false);
    connect(newButton, SIGNAL(clicked()), this, SLOT(newUser()));

    addSave = new QPushButton(bg, "AddSaveButton");
    addSave->setText("&Add");
    connect(addSave, SIGNAL(clicked()), this, SLOT(saveUser()));
    
    

    // Create our layout.
    Q3BoxLayout  *mainLayout = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3, "Main Layout");

    // The grid is where we contain all of our form fields.
    Q3GridLayout *formLayout = new Q3GridLayout(4, 2, 1, "User Form");
    formLayout->setColStretch(0,0);
    formLayout->setColStretch(1,1);
   
    int curRow = 0;
    formLayout->addWidget(loginIDLabel,     curRow, 0);
    formLayout->addWidget(loginID,          curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(accessLabel,      curRow, 0);
    formLayout->addWidget(accessLevel,      curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(userActive,       curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(passwordLabel1,   curRow, 0);
    formLayout->addWidget(password1,        curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(passwordLabel2,   curRow, 0);
    formLayout->addWidget(password2,        curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(telephoneLabel,   curRow, 0);
    formLayout->addWidget(telephone,        curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(extensionLabel,   curRow, 0);
    formLayout->addWidget(extension,        curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(agentIDLabel,     curRow, 0);
    formLayout->addWidget(agentID,          curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(queueNameLabel,   curRow, 0);
    formLayout->addWidget(queueName,        curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    formLayout->addWidget(sendTicketNotifications,  curRow, 1);
    formLayout->setRowStretch(curRow, 0);
    curRow++;

    mainLayout->addLayout(formLayout, 1);
    mainLayout->addStretch(1);

    // The button row now.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bg->insert(addSave);
    bl->addStretch(1);
    bl->addWidget(bg, 1);
    mainLayout->addLayout(bl, 0);

    myCurrentID = 0;

}

UserEditor::~UserEditor()
{
}


/*
** loadUser - Loads a user based on their InternalID.
*/

void UserEditor::loadUser(long userID)
{
    // Clear our form, first.
    loginID->setText("");
    accessLevel->setCurrentItem(2);         // Clerk
    userActive->setChecked(false);
    password1->setText("");
    password2->setText("");
    telephone->setText("");
    extension->setText("");
    agentID->setText("");
    queueName->setText("");
    addSave->setText("&Add");
    delButton->setEnabled(false);
    newButton->setEnabled(false);
    sendTicketNotifications->setChecked(false);
    myCurrentID = 0;

    if (userID) {
        ADBTable    alDB("Staff");
        alDB.get(userID);
        if (alDB.getLong("InternalID") == userID) {
            // Got it.  Fill in the form.
            loginID->setText(alDB.getStr("LoginID"));
            accessLevel->setCurrentItem(alDB.getInt("AccessLevel")-1);
            userActive->setChecked(alDB.getInt("Active"));
            addSave->setText("S&ave");
            delButton->setEnabled(true);
            newButton->setEnabled(true);
            sendTicketNotifications->setChecked(alDB.getInt("TicketNotifications"));
            telephone->setText(alDB.getStr("Telephone"));
            extension->setText(alDB.getStr("Extension"));
            agentID->setText(alDB.getStr("AgentID"));
            queueName->setText(alDB.getStr("QueueName"));
            myCurrentID = userID;
        }
    }
}

/*
** saveUser - Validates the data and saves the user into the database.
*/

void UserEditor::saveUser()
{
    if (!loginID->text().length()) {
        QMessageBox::critical(this, "Save User",
                "Login ID's must not be blank.");
        return;
    }

    if (password1->text().length() || password2->text().length()) {
        if (strcmp(password1->text(), password2->text())) {
            QMessageBox::critical(this, "Save User",
                    "The passwords did not match.");
            return;
        }
    }
    
    ADBTable    alDB("Staff");
    ADB         tmpDB;

    // Verify the data in the form.
    if (myCurrentID) {
        char    oldLogin[1024];

        alDB.get(myCurrentID);
        alDB.setValue("AccessLevel", accessLevel->currentItem()+1);

        if (userActive->isChecked()) {
            alDB.setValue("Active", 1);
        } else {
            alDB.setValue("Active", 0);
        }

        // This is an edit, first, make sure that the login ID is the
        // same.  If its not, make sure that it is unique.
        strcpy(oldLogin, alDB.getStr("LoginID"));
        if (strcmp(loginID->text(), alDB.getStr("LoginID"))) {
            // Okay, its different.  Verify that its still unique.
            tmpDB.query("select InternalID from Staff where LoginID = '%s' and InternalID <> %ld", (const char *)loginID->text(), myCurrentID);
            if (tmpDB.rowCount) {
                QMessageBox::critical(this, "Save User",
                        "The specified Login ID already exists.");
                loginID->setFocus();
                return;
            }
        }
        alDB.setValue("LoginID", (const char *)loginID->text());

        // If they changed the user's password, calculate a new MD5 for
        // it.
        if (password1->text().length()) {
            tmpDB.query("select MD5('%s')", (const char *)password1->text());
            tmpDB.getrow();
            alDB.setValue("Password", tmpDB.curRow[0]);
        }

        if (sendTicketNotifications->isChecked()) {
            alDB.setValue("TicketNotifications", 1);
        } else {
            alDB.setValue("TicketNotifications", 0);
        }

        // We should probably verify this data a bit more
        alDB.setValue("Telephone",      (const char *)telephone->text());
        alDB.setValue("Extension",      (const char *)extension->text());
        alDB.setValue("AgentID",        (const char *)agentID->text());
        alDB.setValue("QueueName",      (const char *)queueName->text());

        alDB.upd();

        // Check to see if we need to update the Staff table as well.
        if (strcmp(oldLogin, loginID->text())) {
            // Updating the staff table is relatively straight forward.
            // One database commad.
            ADB     staffDB;
            staffDB.dbcmd("update Staff set LoginID = '%s' where LoginID = '%s'", (const char *)loginID->text(), oldLogin);
        }
        
    } else {
        // The access level is okay regardless.
        alDB.setValue("AccessLevel", accessLevel->currentItem()+1);
        if (userActive->isChecked()) {
            alDB.setValue("Active", 1);
        } else {
            alDB.setValue("Active", 0);
        }
        if (sendTicketNotifications->isChecked()) {
            alDB.setValue("TicketNotifications", 1);
        } else {
            alDB.setValue("TicketNotifications", 0);
        }
        // Verify that the user doesn't already exist.
        tmpDB.query("select InternalID from Staff where LoginID = '%s'", (const char *)loginID->text());
        if (tmpDB.rowCount) {
            QMessageBox::critical(this, "Save User",
                    "The specified Login ID already exists.");
            loginID->setFocus();
            return;
        }

        alDB.setValue("LoginID", (const char *)loginID->text());

        alDB.setValue("Telephone",      (const char *)telephone->text());
        alDB.setValue("Extension",      (const char *)extension->text());
        alDB.setValue("AgentID",        (const char *)agentID->text());
        alDB.setValue("QueueName",      (const char *)queueName->text());


        // If we're creating a new user, they must give a password.
        if (!password1->text().length()) {
            QMessageBox::critical(this, "Save User",
                    "A password must be specified.");
            password1->setFocus();
            return;
        }

        tmpDB.query("select MD5('%s')", (const char *)password1->text());
        tmpDB.getrow();
        alDB.setValue("Password", tmpDB.curRow[0]);

        alDB.ins();
        myCurrentID = alDB.getLong("InternalID");

    }

    emit(userUpdated(myCurrentID));

}

/*
** newUser - Sets up the form to handle a new user.
*/

void UserEditor::newUser()
{
    loadUser(0);
    loginID->setFocus();
}

/*
** deleteUser - Verifies that the LoginID is not in use in any other tables
**              prompts the user and then deletes the user if appropriate.
*/

void UserEditor::deleteUser()
{
    if (!myCurrentID) return;

    // First, reset the form.
    loadUser(myCurrentID);
    /*
    if (charKeyUsage("AccessLevels", "LoginID", (const char *)loginID->text())) {
        QMessageBox::critical(this, "Delete User",
                "The specified Login ID is in use in other\ntables and cannot be deleted.");
        return;

    }
    */

    ADB         tmpDB;
    switch(QMessageBox::information(this, "Delete User",
                "Are you sure you wish to delete this login?",
                "&Yes", "&No", "&Cancel",
                0,      // Enter = yes
                2       // Escape = no
                )) {
        case 0:         // Yes selected
                tmpDB.dbcmd("delete from Staff where InternalID = %ld", myCurrentID);
                myCurrentID = 0;
                emit(userUpdated(0));
            break;

        default:
            break;
    }
}




// vim: expandtab
