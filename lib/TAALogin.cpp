/*
** $Id$
**
***************************************************************************
**
** TAALogin   - Creates the interface for logging into the database.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis, All Rights Reserved.
**   Based on the ADB class ADBLogin, (C)opyright 1998-2001
***************************************************************************
*/

#include "TAALogin.h"

#include <ADB.h>
#include <TAATools.h>

#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qapplication.h>
#include <mysql/mysql.h>
#include "version.h"
#include "buildnum.h"

TAALogin::TAALogin(QWidget *parent, const char *name) : QDialog(parent, name, true)
{
    // Set the current and maximum number of attempts we will allow the user
    // to make
    currentAttempt = 0;
    maxAttempts    = 3;
    QString	aboutText;
    
    aboutText  = "<html><center>";
    aboutText += "<b>Total <I>Account</I>Ability</b><BR>";
    aboutText += "Customer Care System Login<br>";
    aboutText += "<p>Version ";
    aboutText += VERSION;
    aboutText += "<br>Build Number ";
    aboutText += BUILDNUM();
    aboutText += "</center></html>";

    int     userRows = 4;
    resize(275,200);
    setMinimumSize(275,200);
    setMaximumSize(275,200);
    
    setCaption("TAA Login");

    titleText = new QTextView(this, "Title Area");
    titleText->setMinimumSize(250, 20);
    //titleText->setMaximumSize(300, 80);
    titleText->setText(aboutText);
    titleText->setTextFormat(RichText);
    titleText->setFocusPolicy(NoFocus);

    
    // Create our user name label and input box.
    QLabel  *userNameLabel = new QLabel(this, "User Name Label");
    userNameLabel->setMinimumSize(80, 20);
    userNameLabel->setMaximumSize(80, 20);
    userNameLabel->setAlignment(AlignRight|AlignVCenter);
    userNameLabel->setText("User Name:");

    userName = new QLineEdit(this, "User Name");
    userName->setMinimumSize(120, 20);
    userName->setMaximumSize(120, 20);
    userName->setMaxLength(16);
    connect(userName, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));
    
    
    // Create our password label and input box.
    QLabel  *passwordLabel = new QLabel(this, "Password Label");
    passwordLabel->setMinimumSize(80, 20);
    passwordLabel->setMaximumSize(80, 20);
    passwordLabel->setAlignment(AlignRight|AlignVCenter);
    passwordLabel->setText("Password:");
    
    password = new QLineEdit(this, "Password");
    password->setMinimumSize(120, 20);
    password->setMaximumSize(120, 20);
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    connect(password, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));

    // Create our login and cancel buttons.
    loginButton = new QPushButton(this, "Login Button");
    loginButton->setMinimumSize(60, 23);
    loginButton->setMaximumSize(60, 23);
    loginButton->setText("&Login");
    loginButton->setDefault(true);
    connect(loginButton, SIGNAL(clicked()), this, SLOT(checkLogin()));

    cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setMinimumSize(60, 23);
    cancelButton->setMaximumSize(60, 23);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


    // Create the status bar.
    status  = new QLabel(this, "Status Bar");
    status->setFrameStyle(QFrame::WinPanel|QFrame::Sunken);
    status->setText("Please Login.");
    

    // Now, create our main layout for the entire widget.
    QBoxLayout  *mainLayout = new QBoxLayout(this, QBoxLayout::TopToBottom, 0, 5, NULL);
    
    // Add the title
    mainLayout->addWidget(titleText, 1, 0);

    // Add the user input area.
    int     curRow = 0;
    QGridLayout *userInputGrid = new QGridLayout(userRows, 2, 5, "User Input Grid");
    userInputGrid->addWidget(userNameLabel, curRow,0,AlignRight);
    userInputGrid->addWidget(userName,      curRow,1,AlignLeft);
    curRow++;
    userInputGrid->addWidget(passwordLabel, curRow,0,AlignRight);
    userInputGrid->addWidget(password,      curRow,1,AlignLeft);
    curRow++;
    userInputGrid->addWidget(loginButton,   curRow,0,AlignRight);
    userInputGrid->addWidget(cancelButton,  curRow,1,AlignLeft);

    mainLayout->addLayout(userInputGrid,0);

    mainLayout->addWidget(status, 0, 0);

    // Setup our defaults
    // The default database name
    setDefaultUser(NULL);

    userName->setFocus();
}

TAALogin::~TAALogin()
{
}


/*
** setDefaultUser - Sets the default user in the userName edit box.  If
**                  set to NULL, we will get the currently logged in user.
*/

void TAALogin::setDefaultUser(const char *user)
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

void TAALogin::setUserEditable(bool allowUserEdit)
{
    userName->setEnabled(allowUserEdit);
}


/*
** user - Returns the current user (i.e. the one we logged in with).
*/

const char *TAALogin::user()
{
    return (const char *) userName->text();
}

/*
** pass - Returns the current password (i.e. the one we logged in with).
*/

const char *TAALogin::pass()
{
    return (const char *) password->text();
}


/*
** badLogin - Called by checkLogin() only.
*/

void TAALogin::badLogin()
{
    status->setText("Login incorrect.");
    status->repaint();
    QApplication::syncX();
    QApplication::flushX();
    sleep(2);
    currentAttempt++;
    if (currentAttempt >= maxAttempts) {
        status->repaint();
        status->setText("Too many failed attempts.");
        status->repaint();
        QApplication::syncX();
        QApplication::flushX();
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

void TAALogin::inputChanged(const QString &)
{
    bool    okay = true;
    if (!userName->text().length()) okay = false;
    loginButton->setEnabled(okay);
}


/*
** checkPasswd  - Attempts a connection to the database with the
**                specified user name and password.
**
*/

void TAALogin::checkLogin()
{
    ADB DB;
    if (!DB.Connected()) {
        status->setText("Unable to connect to the database.");
        badLogin();
    } else {
        DB.query("select * from Staff where LoginID = '%s' and Password = MD5('%s')", (const char *) userName->text(), DB.escapeString((const char *)password->text()));
        if (DB.rowCount) {
            // Good login.  Lets set our system info.
            DB.getrow();
            taa_User    userInfo;
            strcpy(userInfo.userName, (const char *) userName->text());
            userInfo.accessLevel = (AccessLevels) atoi(DB.curRow["AccessLevel"]);
            setCurUser(userInfo);
            setResult(1);
            accept();
        } else {
            badLogin();
        }
    }

    /*
    extern char DBHost[64];
    extern char DBUser[64];
    extern char DBPass[64];
//  extern char DBName[64];
    
    strcpy(DBPass, passwd->text());

    // Now, attempt a connection.  If it fails, try again until we hit 3
    MYSQL   *tmpSock;
    MYSQL   tmpConn;
    if (!(tmpSock = mysql_connect(&tmpConn, DBHost, DBUser, DBPass))) {
        versionNumber->setText("Login incorrect.");
        attemptNo++;
        if (attemptNo > 3) {
            passwd->setText("");
            printf("Too many failed attempts.\n");
            exit(-1);
        }
    } else {
        // fprintf(stderr, "Our login was successful...\n");
        successfulLogin = 1;
        setResult(1);
        mysql_close(tmpSock);
        accept();
        // close();
    }
    // fprintf(stderr, "Returning from checkPasswd()\n");
    */
}

