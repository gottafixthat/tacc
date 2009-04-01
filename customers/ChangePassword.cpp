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

#include <QtGui/QApplication>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <QtGui/QLayout>
#include <QtGui/QToolTip>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>
#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include "ChangePassword.h"

using namespace Qt;

ChangePassword::ChangePassword(QWidget* parent, const char* name, long CustomerID, const char* LoginID) :
	TAAWidget( parent, name )
{
	setCaption( "Change Password" );
	
	if (CustomerID == 0) return;
	if (!strlen(LoginID)) return;

    // Setup the widget.
    QLabel  *customerIDLabel = new QLabel("Customer ID:", this, "customerIDLabel");
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);

    customerID = new QLabel(this, "customerID");
    customerID->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *customerNameLabel = new QLabel("Customer Name:", this, "customerNameLabel");
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);

    customerName = new QLabel(this, "customerName");
    customerName->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *contactNameLabel = new QLabel("Contact Name:", this, "contactNameLabel");
    contactNameLabel->setAlignment(AlignRight|AlignVCenter);

    contactName = new QLabel(this, "contactName");
    contactName->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *loginIDLabel = new QLabel("Login ID:", this, "loginIDLabel");
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);

    loginID = new QLabel(this, "loginID");
    loginID->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *loginTypeLabel = new QLabel("Login Type:", this, "loginTypeLabel");
    loginTypeLabel->setAlignment(AlignRight|AlignVCenter);

    loginType = new QLabel(this, "loginType");
    loginType->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *newPasswordLabel = new QLabel("New Password:", this, "newPassword");
    newPasswordLabel->setAlignment(AlignRight|AlignVCenter);
    newPassword = new QLineEdit(this, "newPassword");
    newPassword->setMaxLength(16);

    QPushButton *generateButton = new QPushButton("&Generate", this, "generateButton");
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generatePassword()));

    QPushButton *saveButton = new QPushButton("&Save", this, "saveButton");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savePassword()));
    
    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPassword()));
    
    // Create our layout.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);

    Q3GridLayout *gl = new Q3GridLayout(2, 6);
    int curRow = 0;
    gl->addWidget(customerIDLabel,      curRow, 0);
    gl->addWidget(customerID,           curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(customerNameLabel,    curRow, 0);
    gl->addWidget(customerName,         curRow, 1);
    gl->setRowStretch(curRow, 0);
	
    curRow++;
    gl->addWidget(contactNameLabel,     curRow, 0);
    gl->addWidget(contactName,          curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(loginIDLabel,         curRow, 0);
    gl->addWidget(loginID,              curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(loginTypeLabel,       curRow, 0);
    gl->addWidget(loginType,            curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(newPasswordLabel,     curRow, 0);
    gl->addWidget(newPassword,          curRow, 1);
    gl->setRowStretch(curRow, 0);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    // Button layout.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(generateButton, 0);
    bl->addSpacing(10);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);



	CustomersDB     CDB;
	LoginsDB        LDB;
	
    ADB             DB;

	CDB.get(CustomerID);
	LDB.get(CustomerID, LoginID);
	myCustID    = CustomerID;
	
    myLoginID = LoginID;
	
	customerID->setText(CDB.getStr("CustomerID"));
	customerName->setText(CDB.getStr("FullName"));
	contactName->setText(LDB.getStr("ContactName"));
	loginID->setText(LoginID);
	
	// Get the login type from the database
	DB.query("select LoginType from LoginTypes where InternalID = %d", LDB.getInt("LoginType"));
	if (DB.rowCount) {
	    if (DB.getrow()) {
	        loginType->setText(DB.curRow["LoginType"]);
	    } else {
	        loginType->setText("Unknown");
	    }
	} else {
	    loginType->setText("Unknown");
	}
	
	QToolTip::add(generateButton, "This will generate a secure 8 character long\npassword consisting of lower-case letters and numbers.");
	
	newPassword->setFocus();
	
}


ChangePassword::~ChangePassword()
{
}

/*
** generatePassword - Generates a random password for the user.
*/

void ChangePassword::generatePassword()
{
    char    tmpstr[64];
    int     doIt = 1;
    
    strcpy(tmpstr, newPassword->text());
    if (strlen(tmpstr)) {
        if (QMessageBox::warning(this, "Passward Already Input", "There is already a password specified.\n\nDo you still wish to generate one?", "&Yes", "&No", 0, 1) != 0) doIt = 0;
    }
    
    if (doIt) {
        pwGen(tmpstr);
        newPassword->setText(tmpstr);
    }
}


/*
** savePassword - Connects to the BRASS server and changes the password for
**                the login.
*/

void ChangePassword::savePassword()
{
    BrassClient *BC;
    char        newPass[64];
    
    strcpy(newPass, newPassword->text());
    if (strlen(newPass) < 5) {
        QMessageBox::warning(this, "Invalid Password", "The password specified is too short.\n\nPasswords must be between 5 and 8 characters long.", "&Ok");
        return;
    }
    

	QApplication::setOverrideCursor(WaitCursor);
	BC = new BrassClient();
	if (!BC->Connect()) {
    	QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "BRASS Error", "Error connecting to the BRASS server.", "&Ok");
    	QApplication::setOverrideCursor(WaitCursor);
	} else {
		if (!BC->Authenticate()) {
        	QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "BRASS Error", "Error authenticating with the BRASS server.", "&Ok");
        	QApplication::setOverrideCursor(WaitCursor);
		} else {
			// Well, we finally made it into brass.  Wipe the login.
			if (!BC->SetPassword(myLoginID.ascii(), newPass)) {
            	QApplication::restoreOverrideCursor();
                QMessageBox::critical(this, "BRASS Error", "Unable to change the password.", "&Ok");
            	QApplication::setOverrideCursor(WaitCursor);
			}
		}
	}
	QApplication::restoreOverrideCursor();
	delete BC;
	emit(customerUpdated(myCustID));

	close();
}


/*
** cancelPassword - Does nothing more than close the dialog box.
*/

void ChangePassword::cancelPassword()
{
    close();
}


// vim: expandtab
