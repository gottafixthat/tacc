/*
** $Id: ADBLogin.cpp,v 1.2 2002/09/29 18:43:47 marc Exp $
**
***************************************************************************
**
** ADBLogin   - Creates a generic interface for logging into a database.
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
** $Log: ADBLogin.cpp,v $
** Revision 1.2  2002/09/29 18:43:47  marc
** *** empty log message ***
**
** Revision 1.1  2000/06/12 21:48:07  marc
** Added a new class - ADBLogin.  This is a generic login dialog that can
** be used in any Qt based program that uses ADB for database access.
** Altered the defines for using the Qt exentions to ADB.  To use them, the
** user must now define ADBQT=1 in their Makefiles.
**
**
*/

#include "ADB.h"

#include <unistd.h>
#include <pwd.h>

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QApplication>
//Added by qt3to4:
#include <QtGui/QBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QDialog>
#include <mysql/mysql.h>

ADBLogin::ADBLogin(QWidget *parent, bool showServer) : QDialog(parent)
{
    // Set the current and maximum number of attempts we will allow the user
    // to make
    currentAttempt = 0;
    maxAttempts    = 3;
    
    int     userRows = 4;
    if (showServer) {
        userRows--;
        // Set our window size and make it so the user cant resize it.
        resize(275,200);
        setMinimumSize(275,200);
        setMaximumSize(275,200);
    } else {
        // Set our window size and make it so the user cant resize it.
        resize(275,180);
        setMinimumSize(275,180);
        setMaximumSize(275,180);
    }
    
    setWindowTitle("Database Login");

    titleText = new Q3TextView(this, "Title Area");
    titleText->setMinimumSize(250, 20);
    titleText->setMaximumSize(300, 80);
    titleText->setText("<CENTER>Please Login</CENTER>");
    titleText->setTextFormat(Qt::RichText);
    titleText->setFocusPolicy(Qt::NoFocus);

    
    // Create our server name label and input box.
    serverLabel = new QLabel(this);
    serverLabel->setMinimumSize(80, 20);
    serverLabel->setMaximumSize(80, 20);
    serverLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    serverLabel->setText("Server:");
    if (!showServer) serverLabel->hide();

    serverList = new QComboBox(this);
    serverList->setMinimumSize(120, 20);
    serverList->setMaximumSize(120, 20);
    serverList->setAutoCompletion(true);
    serverList->setDuplicatesEnabled(false);
    serverList->addItem("localhost");
    connect(serverList, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));
    if (!showServer) serverList->hide();
    
    // Create our user name label and input box.
    QLabel  *userNameLabel = new QLabel(this);
    userNameLabel->setMinimumSize(80, 20);
    userNameLabel->setMaximumSize(80, 20);
    userNameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    userNameLabel->setText("User Name:");

    userName = new QLineEdit(this);
    userName->setMinimumSize(120, 20);
    userName->setMaximumSize(120, 20);
    userName->setMaxLength(16);
    connect(userName, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));
    
    
    // Create our password label and input box.
    QLabel  *passwordLabel = new QLabel(this);
    passwordLabel->setMinimumSize(80, 20);
    passwordLabel->setMaximumSize(80, 20);
    passwordLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    passwordLabel->setText("Password:");
    
    password = new QLineEdit(this);
    password->setMinimumSize(120, 20);
    password->setMaximumSize(120, 20);
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    connect(password, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));

    // Create our login and cancel buttons.
    loginButton = new QPushButton(this);
    loginButton->setMinimumSize(60, 23);
    loginButton->setMaximumSize(60, 23);
    loginButton->setText("&Login");
    loginButton->setDefault(true);
    connect(loginButton, SIGNAL(clicked()), this, SLOT(checkLogin()));

    cancelButton = new QPushButton(this);
    cancelButton->setMinimumSize(60, 23);
    cancelButton->setMaximumSize(60, 23);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


    // Create the status bar.
    status  = new QLabel(this);
    status->setFrameStyle(Q3Frame::WinPanel|Q3Frame::Sunken);
    status->setText("Please Login.");
    

    // Now, create our main layout for the entire widget.
    QBoxLayout  *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    
    // Add the title
    mainLayout->addWidget(titleText, 0, 0);

    // Add the user input area.
    int     curRow = 0;
    QGridLayout *userInputGrid = new QGridLayout();
    if (showServer) {
        userInputGrid->addWidget(serverLabel,   curRow,0,Qt::AlignRight);
        userInputGrid->addWidget(serverList,    curRow,1,Qt::AlignLeft);
        curRow++;
    }
    userInputGrid->addWidget(userNameLabel, curRow,0,Qt::AlignRight);
    userInputGrid->addWidget(userName,      curRow,1,Qt::AlignLeft);
    curRow++;
    userInputGrid->addWidget(passwordLabel, curRow,0,Qt::AlignRight);
    userInputGrid->addWidget(password,      curRow,1,Qt::AlignLeft);
    curRow++;
    userInputGrid->addWidget(loginButton,   curRow,0,Qt::AlignRight);
    userInputGrid->addWidget(cancelButton,  curRow,1,Qt::AlignLeft);

    mainLayout->addLayout(userInputGrid,0);

    mainLayout->addWidget(status, 0, 0);

    // Setup our defaults
    // The default database name
    myDBName = NULL;
    setDBName("mysql");
    setDefaultUser(NULL);

    userName->setFocus();
}

ADBLogin::~ADBLogin()
{
}


/*
** setTextTitle - Sets the TextView title text
*/

void ADBLogin::setTextTitle(const char *newTitle)
{
    if (newTitle) titleText->setText(newTitle);
    else titleText->setText("<CENTER>Please Login</CENTER>");
    titleText->setTextFormat(Qt::RichText);
}

/*
** setDBName - Sets the database name to try to select.  If this is not
**             set before exec()ing, we might get false results since it
**             will try logging into 'mysql'.
*/

void ADBLogin::setDBName(const char *newDBName)
{
    if (myDBName) delete myDBName;
    myDBName = new char[strlen(newDBName)+16];
    strcpy(myDBName, newDBName);
}

/*
** setDefaultUser - Sets the default user in the userName edit box.  If
**                  set to NULL, we will get the currently logged in user.
*/

void ADBLogin::setDefaultUser(const char *user)
{
    if (user) userName->setText(user);
    else {
        // Get the Unix user name.
        uid_t   myUID;
        passwd  *pent;
        myUID = getuid();
        pent = getpwuid(myUID);
        if (pent != NULL) userName->setText(pent->pw_name);
        else userName->setText("");
    }
}

/*
** setUserEditable - If true, we will allow the user to input a user name.
*/

void ADBLogin::setUserEditable(bool allowUserEdit)
{
    userName->setEnabled(allowUserEdit);
}


/*
** clearServerList - Clears our server list.
*/

void ADBLogin::clearServerList()
{
    serverList->clear();
}

/*
** addServer - Adds a server to the server list.
*/

void ADBLogin::addServer(const char *serverName)
{
    serverList->addItem(serverName);
}

/*
** setServerEditable - If true, we will allow the user to select a server.
*/

void ADBLogin::setServerEditable(bool allowServerEdit)
{
    serverList->setEnabled(allowServerEdit);
}


/*
** server - Returns the current server (i.e. the one we logged in to).
*/

const char *ADBLogin::server()
{
    return serverList->currentText().toAscii();
}

/*
** user - Returns the current user (i.e. the one we logged in with).
*/

const char *ADBLogin::user()
{
    return userName->text().toAscii();
}

/*
** pass - Returns the current password (i.e. the one we logged in with).
*/

const char *ADBLogin::pass()
{
    return password->text().toAscii();
}

/*
** setADBDefaults  - Sets the global ADB Defaults for the user based on
**                   what was entered in the login window.
**                   The user must still set the default database.
*/

void ADBLogin::setADBDefaults()
{
    ADB::setDefaultHost(serverList->currentText().toAscii());
    ADB::setDefaultUser(userName->text().toAscii());
    ADB::setDefaultPass(password->text().toAscii());
}

/*
** checkLogin - Attempts to connect to the database.  If successful, we
**              call "accept()" to close the dialog and continue on with
**              the program.   If we fail, we increment currentAttempt, and
**              if we exceed maxAttempts, we exit.
*/

void ADBLogin::checkLogin()
{
    // First, test the server name and login to make sure that we are able to
    // login (i.e. they have values).
    if (serverList->currentText().length() && userName->text().length()) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        status->setText("Logging in...");
        status->repaint();
        QApplication::syncX();
        // Create our MySQL connection.
        MYSQL   *tmpSock;
        MYSQL   tmpConn;
        mysql_init(&tmpConn);
        if(!(tmpSock = mysql_real_connect(&tmpConn, serverList->currentText().toAscii(), userName->text().toAscii(), password->text().toAscii(), NULL, 0, NULL, 0))) {
            badLogin();
        } else {
            // We must now select a database as well to complete the login.
            if (mysql_select_db(tmpSock, myDBName) == -1) {
                mysql_close(tmpSock);
                badLogin();
            } else {
                QApplication::restoreOverrideCursor();
                mysql_close(tmpSock);
                accept();
            }
        }
    }
}

/*
** badLogin - Called by checkLogin() only.
*/

void ADBLogin::badLogin()
{
    status->setText("Login incorrect.");
    status->repaint();
    QApplication::syncX();
    sleep(2);
    currentAttempt++;
    if (currentAttempt >= maxAttempts) {
        status->repaint();
        status->setText("Too many failed attempts.");
        status->repaint();
        QApplication::syncX();
        sleep(1);
        QApplication::restoreOverrideCursor();
        reject();
    }
    status->setText("Please login.");
    QApplication::restoreOverrideCursor();
}

/*
** inputChanged - Gets called whenever the user input has changed in
**                one of our input widgets.
**                It makes sure that serverList and userName both have
**                some sort of text in them, or else we disable the
**                login button.
*/

void ADBLogin::inputChanged(const QString &)
{
    bool    okay = true;
    if (!serverList->currentText().length()) okay = false;
    if (!userName->text().length()) okay = false;
    loginButton->setEnabled(okay);
}


