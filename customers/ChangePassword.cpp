/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangePassword.cpp
	Last generated: Fri May 22 18:03:34 1998

 *********************************************************************/

#include "ChangePassword.h"
#include "BlargDB.h"
#include "BString.h"
#include "BrassClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <qapp.h>
#include <qtooltip.h>
#include <ADB.h>
#include <qmessagebox.h>

#define Inherited ChangePasswordData

ChangePassword::ChangePassword
(
	QWidget* parent,
	const char* name,
	long CustomerID,
	const char* LoginID
)
	:
	Inherited( parent, name )
{
	setCaption( "Change Password" );
	
	if (CustomerID == 0) return;
	if (!strlen(LoginID)) return;
	
	CustomersDB     CDB;
	LoginsDB        LDB;
	
    ADB             DB;

	CDB.get(CustomerID);
	LDB.get(CustomerID, LoginID);
	myCustID    = CustomerID;
	
	strcpy(myLoginID, LoginID);
	
	customerIDLabel->setText(CDB.getStr("CustomerID"));
	customerNameLabel->setText(CDB.getStr("FullName"));
	contactNameLabel->setText(LDB.getStr("ContactName"));
	loginIDLabel->setText(LoginID);
	
	// Get the login type from the database
	DB.query("select LoginType from LoginTypes where InternalID = %d", LDB.getInt("LoginType"));
	if (DB.rowCount) {
	    if (DB.getrow()) {
	        loginTypeLabel->setText(DB.curRow["LoginType"]);
	    } else {
	        loginTypeLabel->setText("Unknown");
	    }
	} else {
	    loginTypeLabel->setText("Unknown");
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
    

	QApplication::setOverrideCursor(waitCursor);
	BC = new BrassClient();
	if (!BC->Connect()) {
    	QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "BRASS Error", "Error connecting to the BRASS server.", "&Ok");
    	QApplication::setOverrideCursor(waitCursor);
	} else {
		if (!BC->Authenticate()) {
        	QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "BRASS Error", "Error authenticating with the BRASS server.", "&Ok");
        	QApplication::setOverrideCursor(waitCursor);
		} else {
			// Well, we finally made it into brass.  Wipe the login.
			if (!BC->SetPassword(myLoginID, newPass)) {
            	QApplication::restoreOverrideCursor();
                QMessageBox::critical(this, "BRASS Error", "Unable to change the password.", "&Ok");
            	QApplication::setOverrideCursor(waitCursor);
			}
		}
	}
	QApplication::restoreOverrideCursor();
	delete BC;
	
	close();
}


/*
** cancelPassword - Does nothing more than close the dialog box.
*/

void ChangePassword::cancelPassword()
{
    close();
}

