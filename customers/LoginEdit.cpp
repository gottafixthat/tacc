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
#include <unistd.h>

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3StrList>

#include <ADB.h>
#include <TAATools.h>
#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include <ParseFile.h>
#include <Cfg.h>

#include "LoginEdit.h"

using namespace Qt;

LoginEdit::LoginEdit
(
	QWidget* parent,
	const char* name,
	long CustID,
	const char* LoginID
) : TAAWidget( parent, name )
{

    // Create our widgets and our layout.
    QLabel  *contactLabel = new QLabel(this);
    contactLabel->setText("Contact Name:");
    contactLabel->setAlignment(AlignRight);

    contactName = new QLineEdit(this);
    contactName->setMaxLength(30);
    
    QLabel  *loginIDLabel = new QLabel(this);
    loginIDLabel->setText("Login ID:");
    loginIDLabel->setAlignment(AlignRight);

    loginID = new QLineEdit(this);
    loginID->setMaxLength(atoi(cfgVal("MaxLoginIDLength")));

    QLabel  *loginTypeLabel = new QLabel(this);
    loginTypeLabel->setText("Type:");
    loginTypeLabel->setAlignment(AlignRight);

    loginType = new QComboBox(false, this);

    QLabel  *passwordLabel = new QLabel(this);
    passwordLabel->setText("Password:");
    passwordLabel->setAlignment(AlignRight);

    passwordEntry = new QLineEdit(this);
    passwordEntry->setMaxLength(atoi(cfgVal("MaxPasswordLength")));

    QLabel  *fingerLabel = new QLabel(this);
    fingerLabel->setText("Finger Name:");
    fingerLabel->setAlignment(AlignRight);

    fingerEntry = new QLineEdit(this);
    fingerEntry->setMaxLength(64);

    QPushButton *saveButton = new QPushButton(this);
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveLogin()));

    QPushButton *cancelButton = new QPushButton(this);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelLogin()));

    // Now, create the layout for this widget.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // Create a grid to hold the multi-entries
    Q3GridLayout *gl  = new Q3GridLayout(4, 4, 3);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);
    for (int i = 0; i < 4; i++) gl->setRowStretch(i, 0);
    gl->addWidget(contactLabel,         0, 0);
    gl->addMultiCellWidget(contactName, 0, 0, 1, 3);
    gl->addWidget(loginIDLabel,         1, 0);
    gl->addWidget(loginID,              1, 1);
    gl->addWidget(loginTypeLabel,       1, 2);
    gl->addWidget(loginType,            1, 3);
    gl->addWidget(passwordLabel,        2, 0);
    gl->addWidget(passwordEntry,        2, 1);
    gl->addWidget(fingerLabel,          3, 0);
    gl->addMultiCellWidget(fingerEntry, 3, 3, 1, 3);

    ml->addLayout(gl, 0);

    // Create a box layout for our action buttons.
    Q3BoxLayout *abl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    abl->addStretch(1);
    abl->addWidget(saveButton, 0);
    abl->addWidget(cancelButton, 0);
    ml->addStretch(1);
    ml->addLayout(abl, 0);

    
	LoginsDB	    LDB;
	CustomersDB	    CDB;
	ADB		        DB;
	char            tmpaddr[64];
	
	
	myCustID = CustID;
	if (LoginID != NULL) {
		strcpy(myLoginID, LoginID);
	} else {
		myLoginID[0] = '\0';
	}
	
	// We have to have a customer ID to do anything.
	if (!myCustID) return;
	
	
	loginID->setText(myLoginID);
	
	setCaption("New Login");

	// Check to see if we're editing something.
	if ((strlen(myLoginID)) && (LDB.get(myCustID, myLoginID))) {
    	setCaption( "Edit Login" );
		loginType->setCurrentItem(LDB.getInt("LoginType"));
		contactName->setText(LDB.getStr("ContactName"));
    	loginID->setEnabled(FALSE);
    	passwordEntry->setEnabled(FALSE);
    	fingerEntry->setEnabled(FALSE);
    	contactName->setFocus();
	} else {
    	setCaption( "New Login" );
		CDB.get(myCustID);
		strcpy(myLoginID, "");
		loginID->setFocus();
		contactName->setText(CDB.getStr("ContactName"));
		strcpy(tmpaddr, CDB.getStr("BillingAddress"));
	}
	
	// We need to fill the Login Types list with the valid login types
	// in the database.
	int curItem = 0;
	loginType->clear();
	DB.query("select LoginType, InternalID from LoginTypes where Active <> 0 order by LoginType");
	while (DB.getrow()) {
		loginType->insertItem(DB.curRow["LoginType"]);
		if (atoi(DB.curRow["InternalID"]) == LDB.getInt("LoginType")) {
			loginType->setCurrentItem(curItem);
		}
		curItem++;
	}
}


LoginEdit::~LoginEdit()
{
}

/*
** saveLogin - A private slot which will get called when the user
**             selects the Save button from the Login Edit window.
**
*/

void LoginEdit::saveLogin()
{
	LoginsDB		LDB;
	BrassClient		*BC;
	char			tmpstr[1024];
    char            subj[4096];
    char            body[4096];
	ADB 			DB;
	ADB 			DB2;
	int             loginTypeID = 0;
    int             dchan = 0;
    int             diskspace = 0;
	NotesDB		    NDB;
	char			oldType[1024];
	char			newType[1024];
	CustomersDB		CDB;
	int             isNewLogin = 0;

    QApplication::setOverrideCursor(WaitCursor);

	DB.query("select InternalID, DiskSpace, DialupChannels from LoginTypes where LoginType = '%s'", (const char *)loginType->text(loginType->currentItem()));
	if (DB.getrow()) {
		loginTypeID = atol(DB.curRow["InternalID"]);
        diskspace   = atoi(DB.curRow["DiskSpace"]);
        dchan       = atoi(DB.curRow["DialupChannels"]);
	}

	// Connect to the BRASS server and see if the login ID is good.
	BC = new BrassClient();
	if (!BC->Connect()) {
		QApplication::restoreOverrideCursor();
        QString tmpQS;
        tmpQS  = "Error connecting to BRASS server ";
        tmpQS += cfgVal("BrassHost");
        QMessageBox::critical(this, "BRASS Error", tmpQS, "&Ok");
		delete BC;
		return;
	} else {
		if (!BC->Authenticate()) {
			QApplication::restoreOverrideCursor();
            QString tmpQS;
            tmpQS  = "Error authenticating with BRASS server ";
            tmpQS += cfgVal("BrassHost");
            QMessageBox::critical(this, "BRASS Error", tmpQS, "&Ok");
			delete BC;
			return;
		}
	}
	

    if (strlen(myLoginID)) {
		LDB.get(myCustID, myLoginID);
    	LDB.setValue("LoginID", myLoginID);
		// Check to see if we need to change any flags for the login type.
		if (LDB.getInt("LoginType") != loginTypeID) {
            // Clear out all of their flags.
            BC->ZapFlags(myLoginID);

			// Now, update the login with the new flags.
			DB.query("select Tag, Value from LoginTypeFlags where LoginTypeID = %ld", loginTypeID);
			while (DB.getrow()) {
				BC->SetFlag((const char *)loginID->text(), DB.curRow["Tag"], DB.curRow["Value"]);
			}
            // Update the number of dialup channels.
            DB.query("select DialupChannels from LoginTypes where InternalID = %ld", loginTypeID);
            if (DB.rowCount) {
                DB.getrow();
                BC->SetFlag((const char *)loginID->text(), "DChan", DB.curRow["DialupChannels"]);
            }
			
			// Update any subscriptions for this login ID now that we have
			// changed its type.
			// LDB.updateSubscriptions(LDB.getInt("LoginType"), loginTypeID);
			
			// Now we want to add a note into the database for changing the
			// Login type.
			// First, get the login types.
			DB.query("select LoginType from LoginTypes where InternalID = %d", LDB.getInt("LoginType"));
			if (DB.rowCount) {
				DB.getrow();
				strcpy(oldType, DB.curRow["LoginType"]);
			} else {
				strcpy(oldType, "unknown");
			}
			DB.query("select LoginType from LoginTypes where InternalID = %ld", loginTypeID);
			if (DB.rowCount) {
				DB.getrow();
				strcpy(newType, DB.curRow["LoginType"]);
			} else {
				strcpy(newType, "unknown");
			}
			NDB.setValue("LoginID", myLoginID);
			NDB.setValue("CustomerID", myCustID);
			NDB.setValue("NoteType", "Operator");
			sprintf(tmpstr, "Changed login type from %s to %s.", oldType, newType);
			NDB.setValue("NoteText", tmpstr);
			NDB.ins();

            // Email the administrators with the change.
            sprintf(subj, "Login Changed %s", myLoginID);
            sprintf(body, "   Customer ID: %ld\nOLD Login Type: %s\nNEW Login Type: %s\n\n    Changed by: %s", myCustID, oldType, newType, curUser().userName);
            emailAdmins(subj, body);
		}
	} else {
		LDB.setValue("CustomerID", myCustID);
		LDB.setValue("LoginID", loginID->text());
		if (!BC->ValidateLogin((const char *) loginID->text())) {
			QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "Invalid LoginID", "Login ID contains invalid characters.", "&Ok");
			delete BC;
			return;
		} else if (BC->LoginExists((const char *) loginID->text())) {
			QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "Invalid LoginID", "Login ID already exists or is invalid.  Select another.", "&Ok");
			delete BC;
			return;
		}
		BC->AddLogin((const char *) loginID->text());
	}
	LDB.setValue("LoginType", loginTypeID);
	LDB.setValue("ContactName", contactName->text());
    LDB.setValue("DiskSpace", diskspace);
    LDB.setValue("DialupChannels", dchan);
	if (strlen(myLoginID)) {
		LDB.upd(0);
	} else {
	    isNewLogin = 1;
        LDB.setValue("Active", (int) 1);
		LDB.ins();
		BC->SetPassword((const char *) loginID->text(), (const char *) passwordEntry->text());
		strcpy(tmpstr, LDB.getStr("ContactName"));
		if (strlen(fingerEntry->text())) {
			strcpy(tmpstr, fingerEntry->text());
		}
		BC->SetFingerName((const char *) loginID->text(), tmpstr);
		
		// Here we want to run through the dictionary items and set all
		// of the flags for this type of login.
		DB.query("select Tag, Value from LoginTypeFlags where LoginTypeID = %ld", loginTypeID);
		while (DB.getrow()) {
			BC->SetFlag((const char *)loginID->text(), DB.curRow["Tag"], DB.curRow["Value"]);
		}

        // Update the number of dialup channels.
        DB.query("select DialupChannels from LoginTypes where InternalID = %ld", loginTypeID);
        if (DB.rowCount) {
            DB.getrow();
            BC->SetFlag((const char *)loginID->text(), "DChan", DB.curRow["DialupChannels"]);
        }
			
		// Now, set the login to active and update it so the last modified
		// date gets updated.
		LDB.setValue("Active", (int) 1);
		LDB.upd(0);
		
		strcpy(myLoginID, LDB.getStr("LoginID"));
	}
	
	// Finally, charge the user for any of the subscription items that 
	// they may have.
	CDB.get(myCustID);
	if (!strlen((const char *) CDB.getStr("PrimaryLogin")) ||
            !strncmp(CDB.getStr("PrimaryLogin"), "W", 1)) {
	    CDB.setValue("PrimaryLogin", myLoginID);
	    CDB.upd();
	}
	CDB.doSubscriptions();
	
	if (isNewLogin) {
        // Do their domains now.
        createDomainAliases(myCustID);

		// And now queue up any messages that they should be sent.
		if (!strcmp(CDB.getStr("PrimaryLogin"), myLoginID)) {
		    // This is the primary login, send them everything.
		    mailPrimaryLogin();
		} else {
		    // This is not the primary login, don't send them everything.
		    mailAddOnLogin();
		    mailParent();
		}
		// We mail the admins no matter what.
		mailAdmins();
	}
	
	emit(refreshLoginList(1));
	emit(refreshCust(myCustID));
	emit(customerUpdated(myCustID));
	
	delete BC;

	QApplication::restoreOverrideCursor();
		
	close();

}

/**
 ** cancelLogin - A private slot which will get called when the user
 **               selects the Cancel button from the Login Edit window.
 **
 **/

void LoginEdit::cancelLogin()
{
	close();
}


/*
** mailPrimaryLogin - Goes through the configuration and sends email to
**                    the primary login ID when it is newly created.
*/

void LoginEdit::mailPrimaryLogin(void)
{
    char    fname[1024];        // The complete filename of the message to parse
    char    prefix[1024];       // The path to the message to parse
    char    mname[1024];        // The file name of the message.
    char    *tmpstr;
    FILE    *fp;
    FILE    *fp2;
    
    Q3StrList    files;          // The list of files to mail to the parents
    
    files.setAutoDelete(TRUE);
    
    tmpstr = new(char[65536]);

    strcpy(prefix, cfgVal("WelcomeFilePrefix"));
    strcpy(tmpstr, cfgVal("PrimaryLoginWelcomeFiles"));
    splitString(tmpstr, ':', files, 1);
    
    for (unsigned int j = 0; j < files.count(); j++) {
        // Create the tmp files.
        tmpnam(mname);
        fp = fopen(mname, "w");
        if (fp != NULL) {
            fprintf(fp, "From: Avvanta Support <support@avvanta.com>\n");
            fprintf(fp, "To: %s@avvanta.com\n", myLoginID);
            
            // Append the parsed file to our email message
            sprintf(tmpstr, "%s/%s", prefix, (const char *) files.at(j));
            tmpnam(fname);
            parseFile(tmpstr, fname, myCustID, myLoginID, "");
            
            fp2 = fopen(fname, "r");
            if (fp2 != NULL) {
                while (fgets(tmpstr, sizeof(tmpstr), fp2) != NULL) {
                    fprintf(fp, tmpstr);
                }
            }
            fclose(fp2);
            unlink(fname);
            
            fclose(fp);
            
            sprintf(tmpstr, "cp %s /var/spool/taamail/%s-%s-%s", mname, myLoginID, (const char *) files.at(j), myLoginID);
            system(tmpstr);
            
            unlink(mname);
        }
    }
    
    delete(tmpstr);
}

/*
** mailAddOnLogin - Goes through the configuration and sends email to
**                  the add-on login ID when it is newly created.
*/

void LoginEdit::mailAddOnLogin(void)
{
    char    fname[1024];        // The complete filename of the message to parse
    char    prefix[1024];       // The path to the message to parse
    char    mname[1024];        // The file name of the message.
    char    *tmpstr;
    FILE    *fp;
    FILE    *fp2;
    
    Q3StrList    files;          // The list of files to mail to the parents
    
    files.setAutoDelete(TRUE);
    
    tmpstr = new(char[65536]);

    strcpy(prefix, cfgVal("WelcomeFilePrefix"));
    strcpy(tmpstr, cfgVal("AddOnLoginWelcomeFiles"));
    splitString(tmpstr, ':', files, 1);
    
    for (unsigned int j = 0; j < files.count(); j++) {
        // Create the tmp files.
        tmpnam(mname);
        fp = fopen(mname, "w");
        if (fp != NULL) {
            fprintf(fp, "From: Avvanta Support <support@avvanta.com>\n");
            fprintf(fp, "To: %s@avvanta.com\n", myLoginID);
            
            // Append the parsed file to our email message
            sprintf(tmpstr, "%s/%s", prefix, (const char *) files.at(j));
            tmpnam(fname);
            parseFile(tmpstr, fname, myCustID, myLoginID, "");
            
            fp2 = fopen(fname, "r");
            if (fp2 != NULL) {
                while (fgets(tmpstr, sizeof(tmpstr), fp2) != NULL) {
                    fprintf(fp, tmpstr);
                }
            }
            fclose(fp2);
            unlink(fname);
            
            fclose(fp);
            
            sprintf(tmpstr, "cp %s /var/spool/taamail/%s-%s-%s", mname, myLoginID, (const char *) files.at(j), myLoginID);
            system(tmpstr);
            
            unlink(mname);
        }
    }
    
    delete(tmpstr);
}

/*
** mailParent - Goes through the configuration and sends email to
**              the the parent account when a new add-on login ID created.
*/

void LoginEdit::mailParent(void)
{
    char    fname[1024];        // The complete filename of the message to parse
    char    prefix[1024];       // The path to the message to parse
    char    mname[1024];        // The file name of the message.
    char    *tmpstr;
    FILE    *fp;
    FILE    *fp2;
    
    CustomersDB CDB;
    
    CDB.get(myCustID);

    Q3StrList    files;          // The list of files to mail to the parents
    
    files.setAutoDelete(TRUE);
    
    tmpstr  = new(char[65536]);

    strcpy(prefix, cfgVal("WelcomeFilePrefix"));
    strcpy(tmpstr, cfgVal("AddOnLoginPrimaryFiles"));
    splitString(tmpstr, ':', files, 1);
    
    for (unsigned int j = 0; j < files.count(); j++) {
        // Create the tmp files.
        tmpnam(mname);
        fp = fopen(mname, "w");
        if (fp != NULL) {
            fprintf(fp, "From: Avvanta Support <support@avvanta.com>\n");
            fprintf(fp, "To: %s@avvanta.com\n", (const char *) CDB.getStr("PrimaryLogin"));
            
            // Append the parsed file to our email message
            sprintf(tmpstr, "%s/%s", prefix, (const char *) files.at(j));
            tmpnam(fname);
            parseFile(tmpstr, fname, myCustID, myLoginID, "");
            
            fp2 = fopen(fname, "r");
            if (fp2 != NULL) {
                while (fgets(tmpstr, sizeof(tmpstr), fp2) != NULL) {
                    fprintf(fp, tmpstr);
                }
            }
            fclose(fp2);
            unlink(fname);
            
            fclose(fp);
            
            sprintf(tmpstr, "cp %s /var/spool/taamail/%s-%s-%s", mname, myLoginID, (const char *) files.at(j), (const char *) CDB.getStr("PrimaryLogin"));
            system(tmpstr);
            
            unlink(mname);
        }
    }
    
    delete(tmpstr);
}

/*
** mailAdmins - Goes through the configuration and sends email to
**              the administrators when a login ID is newly created.
*/

void LoginEdit::mailAdmins(void)
{
    char    fname[1024];        // The complete filename of the message to parse
    char    prefix[1024];       // The path to the message to parse
    char    mname[1024];        // The file name of the message.
    char    *tmpstr;
    FILE    *fp;
    FILE    *fp2;

    Q3StrList    admins;         // The list of admins to mail.
    Q3StrList    files;          // The list of files to mail to the admins
    
    admins.setAutoDelete(TRUE);
    files.setAutoDelete(TRUE);
    
    tmpstr  = new(char[65536]);

    strcpy(prefix, cfgVal("WelcomeFilePrefix"));
    strcpy(tmpstr, cfgVal("AdminEmailList"));
    splitString(tmpstr, ':', admins, 1);
    strcpy(tmpstr, cfgVal("AdminNewLoginFiles"));
    splitString(tmpstr, ':', files, 1);
    
    for (unsigned int i = 0; i < admins.count(); i++)
      for (unsigned int j = 0; j < files.count(); j++) {
        // Create the tmp files.
        tmpnam(mname);
        fp = fopen(mname, "w");
        if (fp != NULL) {
            fprintf(fp, "From: Avvanta Support <support@avvanta.com>\n");
            fprintf(fp, "To: %s@avvanta.com\n", (const char *) admins.at(i));
            
            // Append the parsed file to our email message
            sprintf(tmpstr, "%s/%s", prefix, (const char *) files.at(j));
            tmpnam(fname);
            parseFile(tmpstr, fname, myCustID, myLoginID, "");
            
            fp2 = fopen(fname, "r");
            if (fp2 != NULL) {
                while (fgets(tmpstr, sizeof(tmpstr), fp2) != NULL) {
                    fprintf(fp, tmpstr);
                }
            }
            fclose(fp2);
            unlink(fname);
            
            fclose(fp);
            
            sprintf(tmpstr, "cp %s /var/spool/taamail/%s-%s-%s", mname, myLoginID, (const char *) files.at(j), (const char *) admins.at(i));
            system(tmpstr);
            
            unlink(mname);
        }
    }
    
    delete(tmpstr);
}


// vim: expandtab
