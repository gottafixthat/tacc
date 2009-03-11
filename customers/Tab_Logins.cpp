/*
** $Id$
**
***************************************************************************
**
** FileName     - Overview of its main function.
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
** $Log: Tab_Logins.cpp,v $
** Revision 1.2  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "Tab_Logins.h"
#include "BlargDB.h"
#include "BString.h"
#include "qstring.h"
#include "LoginEdit.h"
#include <qmsgbox.h>
#include "BrassClient.h"
#include "AcctsRecv.h"
#include "ChangePassword.h"
#include "ModemUsageReport.h"
#include "BandwidthUsageReport.h"
#include "TransferLogin.h"
#include <Cfg.h>
#include <stdlib.h>
#include <stdio.h>
#include <qapp.h>
#include <qdatetm.h>
#include <qlistview.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <ADB.h>
#include <QSqlDbPool.h>
#include "TAATools.h"

Tab_Logins::Tab_Logins
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget ( parent, name )
{

	setCaption( "Logins" );
	myCustID = CustID;


    // Create all of our widgets.
    list = new QListView(this, "LoginList");
	list->addColumn("Login ID");
	list->addColumn("Login Type");
	list->addColumn("Contact Name");
	list->addColumn("Last Modified");
	list->addColumn("Disk");
	list->addColumn("Flags");
	list->addColumn("Active");
	list->setAllColumnsShowFocus(TRUE);
	list->setColumnAlignment(4, AlignRight);
	list->setColumnAlignment(5, AlignRight);
	list->setColumnAlignment(6, AlignCenter);
    list->setItemMargin( 2 );

    newButton = new QPushButton(this, "NewButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(createNewLogin()));

    openCloseButton = new QPushButton(this, "OpenCloseButton");
    openCloseButton->setText("(&De)Activate");
    connect(openCloseButton, SIGNAL(clicked()), this, SLOT(unlockLogin()));

    passwdButton = new QPushButton(this, "PasswdButton");
    passwdButton->setText("&Password");
    connect(passwdButton, SIGNAL(clicked()), this, SLOT(changePassword()));

    QPushButton *loginFlagsButton = new QPushButton(this, "LoginFlagsButton");
    loginFlagsButton->setText("Fla&gs");
    connect(loginFlagsButton, SIGNAL(clicked()), this, SLOT(loginFlagsClicked()));

    editButton = new QPushButton(this, "EditButton");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editLogin()));

    modemUsageButton = new QPushButton(this, "Modem Usage Button");
    modemUsageButton->setText("&Modem Usage");
    connect(modemUsageButton, SIGNAL(clicked()), this, SLOT(modemUsageReport()));

    bandwidthButton = new QPushButton(this, "BandwidthButton");
    bandwidthButton->setText("&Bandwidth");
    connect(bandwidthButton, SIGNAL(clicked()), this, SLOT(bandwidthUsageReport()));

    QPushButton *diskSpaceButton = new QPushButton(this, "DiskSpaceButton");
    diskSpaceButton->setText("D&isk Space");
    connect(diskSpaceButton, SIGNAL(clicked()), this, SLOT(diskSpaceClicked()));
    diskSpaceButton->setEnabled(false);
    
    adminMenuArea = new QLabel(this, "Admin Menu Area");
    adminMenu = new QPopupMenu;
    adminMenu->insertItem("&Set Primary Login", this, SLOT(setPrimaryLogin()));
    adminMenu->insertItem("&Transfer Login", this, SLOT(transferLogin()));
    if (isAdmin()) adminMenu->insertItem("&Wipe Login", this, SLOT(wipeLogin()));
    adminMenu->setFocusPolicy(QWidget::StrongFocus);
	
	QMenuBar    *tmpMenu;
	tmpMenu = new QMenuBar(adminMenuArea, "Administrivia");
    tmpMenu->insertItem("&Administrivia", adminMenu);
    tmpMenu->setFocusPolicy(QWidget::StrongFocus);

    hideWiped = new QCheckBox(this, "HideWiped");
    hideWiped->setText("&Hide Wiped");
    hideWiped->setChecked(true);
    connect(hideWiped, SIGNAL(toggled(bool)), this, SLOT(hideWipedChanged(bool)));

    // Create our layouts.  A top to bottom box, with a grid in it.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(list, 1);

    QGridLayout *bl = new QGridLayout(2, 6);
    bl->addWidget(newButton,            0, 0);
    bl->addWidget(openCloseButton,      0, 1);
    bl->addWidget(passwdButton,         0, 2);
    bl->addWidget(loginFlagsButton,     0, 3);
    bl->addWidget(adminMenuArea,        0, 5);
    
    bl->addWidget(editButton,           1, 0);
    bl->addWidget(modemUsageButton,     1, 1);
    bl->addWidget(bandwidthButton,      1, 2);
    bl->addWidget(diskSpaceButton,      1, 3);
    bl->addWidget(hideWiped,            1, 5);

    for (int i = 0 ; i < 5; i++) bl->setColStretch(i, 1);
    bl->setRowStretch(0, 0);
    bl->setRowStretch(1, 0);

    ml->addLayout(bl, 0);

	
	
	// QObject::connect(listtitle, SIGNAL(widthChanged(int, int)),
	//  list, SLOT(setColumnWidth(int, int)));
	
	refreshLoginList(0);
	
	if (!isManager()) {
		// wipeButton->hide();
		adminMenuArea->hide();
	}

}


Tab_Logins::~Tab_Logins()
{
}


/**
 ** refreshLoginList - A public slot which will get called when an item
 **					   in the Logins list is changed.
 **
 **/

void Tab_Logins::refreshLoginList(int)
{
    QSqlDbPool  dbp;
    QSqlDatabase    *qsql = dbp.qsqldb();

    ADB     DB;
    QString tmpstr = "";
    QString tmpQItemID = "";
    QString tmpQActive = "";
    QString	qds;
    ADB	    DB2;

    bool    showIt = true;
    char    isActive[128];
    char    dateStamp[128];
    QDate   d;
    QTime   t;
    
    list->clear();

    DB.query("select Logins.LoginID, LoginTypes.LoginType, Logins.ContactName, Logins.LastModified, Logins.DiskSpace, Logins.DialupChannels, Logins.Active from Logins, LoginTypes where Logins.CustomerID = %ld and Logins.LoginType = LoginTypes.InternalID", myCustID);
    if (DB.rowCount) while (DB.getrow()) {
        strcpy(isActive, "No");
        if (atoi(DB.curRow["Active"])) strcpy(isActive, "Yes");

        // Get the date stamp.
        strcpy(dateStamp, "Unknown");
        if (strncmp("0000", DB.curRow["LastModified"], 4)) {
            // There is a date in the LastModified field.
            d = DB.curRow.col("LastModified")->toQDateTime().date();
            t = DB.curRow.col("LastModified")->toQDateTime().time();

            sprintf(dateStamp, "%04d-%02d-%02d %2d:%02d", d.year(), d.month(), d.day(), t.hour(), t.minute());
            
        }

        // Count how many custom flags there are.
        QString flagCount = "0";
        DB2.query("select count(LoginFlag) from LoginFlagValues where LoginID = '%s'", DB.curRow["LoginID"]);
        if (DB2.rowCount) {
            DB2.getrow();
            flagCount = DB2.curRow[0];
        }

        showIt = true;
        if (hideWiped->isChecked() && !strncmp("W", DB.curRow["LoginID"], 1)) showIt = false;
        if (showIt) {
            (void) new QListViewItem(list,
              DB.curRow["LoginID"],         // Login ID
              DB.curRow["LoginType"],       // Login Type
              DB.curRow["ContactName"],     // Contact Name
              dateStamp,                    // Last Modified Date
              DB.curRow["DiskSpace"],       // Disk Space
              flagCount,                    // Custom flags
              isActive                      // Active?
            );
        }
    }

    /*
    
	// Fill in the subscriptions ListBox
	if (hideWiped->isChecked()) {
        sprintf(querystr, "select LoginID, LoginType, ContactName, LastModified, DiskSpace, DialupChannels, Active from Logins where CustomerID = %ld and Wiped < '1970-01-01' order by LoginID", myCustID);
    } else {
        sprintf(querystr, "select LoginID, LoginType, ContactName, LastModified, DiskSpace, DialupChannels, Active from Logins where CustomerID = %ld order by LoginID", myCustID);
    }
	DB.query("%s", querystr);
	// subscrIndex = new(int[DB.rowCount + 1]);
	int idxPtr = 0;
	while(DB.getrow()) {
		// subscrIndex[idxPtr] = atoi(DB.curRow[0]);
		idxPtr++;
		tmpQItemID = DB.curRow["ContactName"];
		tmpQItemID.truncate(11);
		strcpy(tmpItemID, tmpQItemID);
		    
		tmpQActive = "No";
		if (atoi(DB.curRow["Active"])) {
			tmpQActive = "Yes";
		}
		strcpy(tmpActive, tmpQActive);
		
		
		DB2.query("select LoginType from LoginTypes where InternalID = %d", atoi(DB.curRow["LoginType"]));
		if (DB2.getrow()) {
            strcpy(loginType, DB2.curRow["LoginID"]);
		} else {
            strcpy(loginType, "Unknown");
		}
		
		// Parse out the time stamp.
		qds = DB.curRow["LastModified"];
		sprintf(tmpDateStamp, "%4s-%2s-%2s %2s:%2s.%2s",
		  (const char *) qds.mid(0,4),
		  (const char *) qds.mid(4,2),
		  (const char *) qds.mid(6,2),
		  (const char *) qds.mid(8,2),
		  (const char *) qds.mid(10,2),
		  (const char *) qds.mid(12,2)
		);
		    
		(void) new QListViewItem(list,
		  DB.curRow["LoginID"],         // Login ID
		  loginType,                    // Login Type
		  DB.curRow["ContactName"],     // Contact Name
		  tmpDateStamp,                 // Last Modified Date
		  DB.curRow["DiskSpace"],       // Disk Space
		  DB.curRow["DialupChannels"],  // Dialup Channels
		  tmpActive                     // Active?
		);
		
		// list->insertItem(i);
	}
    */
	
}

void Tab_Logins::hideWipedChanged(bool)
{
    refreshLoginList(1);
}

void Tab_Logins::refreshLoginListV(int)
{
    recountAllowedMailboxes(myCustID);
	refreshLoginList(1);
    emit(refreshCust(myCustID));
}

/*
** createNewLogin - Brings up a dialog to create a new login under this
**                  customers account.
*/

void Tab_Logins::createNewLogin(void)
{
	LoginEdit *le;
	le = new LoginEdit(0, "", myCustID);
	connect(le, SIGNAL(refreshLoginList(int)), SLOT(refreshLoginListV(int)));
	le->show();
}

/*
** editLogin - Brings up a dialog to edit a login under this
**                  customers account.
*/

void Tab_Logins::editLogin(void)
{
	LoginsDB	    LDB;
	QListViewItem   *curItem;
	
	curItem = list->currentItem();
	
    if (curItem != NULL) {
		QApplication::setOverrideCursor(waitCursor);
		// Make sure that the login is active before we edit it.
		LDB.get(myCustID, (const char *) curItem->text(0));
		
		if (!LDB.getLong("InternalID")) {
		    printf("ARG!  Couldn't load Customer ID = %ld, list item = '%s'.\n", myCustID, (const char *) curItem->text(0));
		}
		
        QApplication::restoreOverrideCursor();		
		if (LDB.getInt("Active")) {
			LoginEdit *le;
			le = new LoginEdit(0, "", myCustID, (const char *) curItem->text(0));
			connect(le, SIGNAL(refreshLoginList(int)), SLOT(refreshLoginListV(int)));
			le->show();
		} else {
			QMessageBox::warning(this, "Unable to Edit", "Inactive accounts may not be edited.");
		}
	}
}

/*
** unlockLogin - Locks or Unlocks the currently selected LoginID, depending
**               on whether it is locked or unlocked currently.  If the
**               account is wiped, the user is told as much.
*/

void Tab_Logins::unlockLogin(void)
{
	char		tmpstr[256];
	char		tmpLogin[256];
    char        subj[4096];
    char        body[4096];
	QListViewItem   *curItem;

    curItem = list->currentItem();

    if (curItem != NULL) {
		QApplication::setOverrideCursor(waitCursor);
		ADB		    DB;
		BrassClient	*BC;
		LoginsDB	LDB;
		CustomersDB	CDB;
		SubscriptionsDB SDB;

		LDB.get(myCustID, (const char *) curItem->text(0));
		
		// First, check to see if this account has been wiped already.
		// If so, let the user know and exit.
		strcpy(tmpstr, LDB.getStr("Wiped"));
		if (strcmp(tmpstr, "0000-00-00")) {
			QApplication::restoreOverrideCursor();
			QMessageBox::warning(this, "Unable to set flag.", "The selected login has been wiped, and no longer exists on the system.");
			return;
		} else {
			// Okay, we aren't wiped, so check to see if this account is
			// Active or not.  If its active, lock it.  If its inactive,
			// unlock it.
			// First, authenticate with the Brass server.
			BC = new BrassClient();
			if (!BC->Connect()) {
				QApplication::restoreOverrideCursor();
                QString     tmpQES;
                tmpQES  = "Error connecting to BRASS server ";
                tmpQES += cfgVal("BrassHost");
				QMessageBox::critical(this, "BRASS Error", tmpQES);
				delete BC;
				return;
			} else {
				if (!BC->Authenticate()) {
					QApplication::restoreOverrideCursor();
                    QString     tmpQES;
                    tmpQES  = "Error authenticating with BRASS server ";
                    tmpQES += cfgVal("BrassHost");
					QMessageBox::critical(this, "BRASS Error", tmpQES);
					delete BC;
					return;
				}
			}

			// If we've made it here, we're connected to the server.
			strcpy(tmpLogin, LDB.getStr("LoginID"));
			if (LDB.getInt("Active")) {
				// We are active, lock the account.
				sprintf(tmpstr, "Lock account '%s'\n\nAre you sure?", tmpLogin);
                QApplication::restoreOverrideCursor();
                if (QMessageBox::warning(this, "Lock Account", tmpstr, "&Yes", "&No", 0, 1) == 0) {
                    QApplication::setOverrideCursor(waitCursor);
                    if (!BC->LockUser(tmpLogin)) {
                        QApplication::restoreOverrideCursor();
                        // We were unsuccessful.
                        sprintf(tmpstr, "Unable to lock the account.\n\n%s", BC->ResponseStr(NULL));
                        QMessageBox::critical(this, "BRASS Error", tmpstr);
                    } else {
                        LDB.setValue("Active", (int) 0);
                        LDB.upd();
                        // Now, update the subscriptions for this login ID.
                        DB.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s'", myCustID, tmpLogin);
                        if (DB.rowCount) while (DB.getrow()) {
                            SDB.get(atol(DB.curRow["InternalID"]));
                            SDB.setValue("Active", (int) 0);
                            SDB.upd();
                        }

                        // And create a note for this transaction.
                        NotesDB	    NDB;
                        NDB.setValue("LoginID", tmpLogin);
                        NDB.setValue("CustomerID", myCustID);
                        NDB.setValue("AutoNote", 1);
                        NDB.setValue("NoteType", "Operator");
                        NDB.setValue("Subject", "Login Locked");
                        NDB.setValue("NoteText", "Login Locked.");
                        NDB.ins();

                        // Email the administrators with the change.
                        sprintf(subj, "Login Locked %s", tmpLogin);
                        sprintf(body, "   Customer ID: %ld\n      Login ID: %s\n\n     Locked by: %s", myCustID, tmpLogin, curUser().userName);
                        emailAdmins(subj, body);
                        
                        // Now, update the AR for the lock.
                        if (isManager()) {
                            QApplication::restoreOverrideCursor();
                            if (QMessageBox::warning(this, "Prorate Charges", "Do you wish to prorate the charges for this login?", "&Yes", "&No", 0, 1) == 0) {
                                QApplication::setOverrideCursor(waitCursor);
                                updateARForLock(tmpLogin);
                            } else {
                                QApplication::setOverrideCursor(waitCursor);
                            }
                        }
                        
                        emit(refreshCust(myCustID));
                        emit(customerUpdated(myCustID));
                        refreshLoginList(myCustID);
                        recountAllowedMailboxes(myCustID);
                        QApplication::restoreOverrideCursor();
                    }
                }
			} else {
				if (!BC->UnLockUser(tmpLogin)) {
					// We were unsuccessful.
					QApplication::restoreOverrideCursor();
					sprintf(tmpstr, "Unable to unlock the account.\n\n%s", BC->ResponseStr(NULL));
					QMessageBox::critical(this, "BRASS Error", tmpstr);
				} else {
					LDB.setValue("Active", (int) 1);
					LDB.upd();
					// Now, update the subscriptions for this login ID.
					DB.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s'", myCustID, tmpLogin);
					if (DB.rowCount) while (DB.getrow()) {
					    SDB.get(atol(DB.curRow["InternalID"]));
					    SDB.setValue("Active", (int) 1);
					    SDB.upd();
					}

					// And create a note for this transaction.
                    NotesDB     NDB;
					NDB.setValue("LoginID", tmpLogin);
					NDB.setValue("CustomerID", myCustID);
                    NDB.setValue("AutoNote", 1);
					NDB.setValue("NoteType", "Operator");
                    NDB.setValue("Subject", "Login Unlocked");
					NDB.setValue("NoteText", "Login Unlocked.");
					NDB.ins();
					
                    // Email the administrators with the change.
                    sprintf(subj, "Login Unlocked %s", tmpLogin);
                    sprintf(body, "   Customer ID: %ld\n      Login ID: %s\n\n   Unlocked by: %s", myCustID, tmpLogin, curUser().userName);
                    emailAdmins(subj, body);

					// Now, update the AR for the lock.
					updateARForLock(tmpLogin);
					
					// And update their subscriptions.
					CDB.get(myCustID);
					CDB.doSubscriptions();

                    recountAllowedMailboxes(myCustID);
					emit(refreshCust(myCustID));
                    emit(customerUpdated(myCustID));
					refreshLoginList(myCustID);
					QApplication::restoreOverrideCursor();
				}
			}
			delete BC;
		}
	}
}

/*
** updateARForLock - This function searches through the AcctsRecv table
**                   for any billed items to the specified login ID that
**                   have a date range.  The EndDate must be > Today.
**                   Any transactions found will be copied (ItemID, etc)
**                   and a negative amount adjusting entry will be inserted
**                   into the customer register.  This will quickly pro-rate
**                   a balance for a customer that is closing their account.
**                   The problem with this is that we could end up cutting
**                   a lot of refund checks with this.  This may be an auto
**                   option, or something to do only if the customer has
**                   a non-zero balance, or is below a certain threshold
**                   number of days into their billing cycle.  Haven't
**                   quite decided yet.  On one hand, we want to be fair to
**                   our customers, but on the other hand, we don't want to
**                   be cutting a hundred checks each month losing money...
*/

void Tab_Logins::updateARForLock(const char * LoginID)
{
	ADB			    DB;
    ADB		    	DB2;
	CustomersDB		CDB;
	LoginsDB		LDB;
	SubscriptionsDB SDB;
	QDate			tmpDate;
	QDate			CycleStart;
	QDate			CycleEnd;
	QDate			yesterday;
	char			sYesterday[64];
	int				cycleDays;
	int				daysLeft;
	char			today[64];
	char			sCycleEnd[64];
	float			Qty;
	int             doEntry;
	BillingCyclesDB	BCDB;
	
	tmpDate = QDate::currentDate();
	sprintf(today, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
	
	CDB.get(myCustID);
	LDB.get(myCustID, LoginID);
	BCDB.get(CDB.getInt("BillingCycle"));
	BCDB.getCycleDates(&CycleStart, &CycleEnd, &cycleDays, &daysLeft,NULL,myCustID);
	sprintf(sCycleEnd, "%04d-%02d-%02d", CycleEnd.year(), CycleEnd.month(), CycleEnd.day());
	
	// Okay, the first thing we want/need to do is load up the entries
	// in the AcctsRecv table that have an EndDate > Today.
	DB.query("select ItemID, Price, Amount, Memo, InternalID from AcctsRecv where CustomerID = %ld and LoginID = '%s' and EndDate > '%s'", 
	  myCustID,
	  LoginID,
	  today
	);
	if (DB.rowCount) {
		// If the customer is active, and we have more than one row, we don't
		// want to make any adjustments.  If they are not active, we're here
		// to make adjusting entries.
		while (DB.getrow()) {
		    doEntry = 1;
			// If they are active now, and our amount > 0, that means
			// that this is an account that was not previously prorated.
			// So we don't want to do the adjusting entry.
			if (LDB.getInt("Active") && (atof(DB.curRow["Amount"]) > 0.0)) doEntry = 0;

            // If they are not active now, and our amount < 0 that means
            // they have already been prorated, and we don't need to do 
            // anything more with it.
            if (!LDB.getInt("Active") && (atof(DB.curRow["Amount"]) < 0.0)) doEntry = 0;

            if (doEntry) {
				// It looks as if we need to add to the balance of the user.
				AcctsRecv	AR;
				AR.ARDB->setValue("CustomerID", myCustID);
				AR.ARDB->setValue("LoginID", LoginID);
				AR.ARDB->setValue("ItemID", atoi(DB.curRow["ItemID"]));
				AR.ARDB->setValue("TransDate", today);
				AR.ARDB->setValue("StartDate", today);
				AR.ARDB->setValue("EndDate", sCycleEnd);
				Qty = (float) daysLeft / cycleDays;
				if (atof(DB.curRow["Amount"]) > 0) {
					Qty = Qty * -1.0;
				}
				AR.ARDB->setValue("Quantity", Qty);
				AR.ARDB->setValue("Price", DB.curRow["Price"]);
				AR.ARDB->setValue("Amount", (float) (atof(DB.curRow["Price"]) * Qty));
				AR.ARDB->setValue("Memo", DB.curRow["Memo"]);
				QString tmpQS;
				tmpQS = AR.ARDB->getStr("Memo");
	            if (tmpQS.find('(')) {
	            	tmpQS.truncate(tmpQS.find('(') -1);
	            }
				if (Qty < 0.0) {
					tmpQS.append(" (Adjustment for account closure)");
				} else {
					tmpQS.append(" (Adjustment for account reactiviation)");
				}
				AR.ARDB->setValue("Memo", (const char *) tmpQS);
				
    			AR.SaveTrans();
				
	    		DB2.dbcmd("update AcctsRecv set EndDate = '%s' where InternalID = %ld", today, atol(DB.curRow["InternalID"]));
			}
		}
	
	} else {
		// There were no register entries.  So make sure there are
		// Subscriptions for the user.  If there aren't, add one.  If there
		// are, update all of the subscriptions for this user so the ends on
		// date are yesterday, and then do a subscription run for the 
		// customer.
		DB.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s'", myCustID, LoginID);
		if (DB.rowCount) {
		    QDate   tmpEndsOn;
			tmpDate = QDate::currentDate();
			yesterday = tmpDate.addDays(-1);
			sprintf(sYesterday, "%04d-%02d-%02d", yesterday.year(), yesterday.month(), yesterday.day());
		    while (DB.getrow()) {
		        SDB.get(atol(DB.curRow["InternalID"]));
		        myDatetoQDate((const char *) SDB.getStr("EndsOn"), &tmpEndsOn);
		        // printf("sYesterday = %s, EndsOn = %s\n", sYesterday, (const char *) SDB.getStr("EndsOn"));
		        if (tmpEndsOn < tmpDate) {
		            SDB.setValue("EndsOn", sYesterday);
		            SDB.upd();
		        }
		    }
		} else {
			// No subscriptions.  Add them.
			LDB.get(myCustID, LoginID);
			LDB.addSubscriptions();
		}
	}
}

/*
** wipeLogin   - Prompts the user and wipes the login if they answer yes.
**               This is only available to Administrators.
*/

void Tab_Logins::wipeLogin()
{
    QListViewItem   *curItem;
    curItem = list->currentItem();
    if (!curItem) return;

    // No point in initializing the objects if we don't have any logins.
    ADB		    DB;
    LoginsDB	LDB;
    BrassClient	*BC;
    char		today[16];
    QDate		tmpDate;
    char		tmpLogin[16];
    char        msg[1024];
    
    
    tmpDate	= QDate::currentDate();
    sprintf(today, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());

    LDB.get(myCustID, (const char *) curItem->text(0));
    if (LDB.getInt("Active")) {
        QMessageBox::information(this, "Wipe Login", "Only inactive accounts may be wiped.\nIf you really want to wipe this account, close it first.");
        return;
    } else if (strcmp((const char *)LDB.getStr("Wiped"), "0000-00-00")) {
        QMessageBox::information(this, "Wipe Login", "This login has already been wiped.");
        return;
    } else {
        sprintf(msg, "Are you sure you wish to wipe the login '%s'?", (const char *) curItem->text(0));
        if (QMessageBox::warning(this, "Wipe Login", msg, "&Yes", "&No", 0, 1) != 0) return;

        strcpy(tmpLogin, LDB.getStr("LoginID"));

        // Remove their CCC Filters and other stuff first.
        QApplication::setOverrideCursor(waitCursor);
        emit setStatus("Removing CCC preferences/filters...");
        ADB     CCCDB(cfgVal("CCCMySQLDB"), cfgVal("CCCMySQLUser"), cfgVal("CCCMySQLPass"), cfgVal("CCCMySQLHost"));
        QApplication::restoreOverrideCursor();
        if (CCCDB.Connected()) {
            QApplication::setOverrideCursor(waitCursor);
            CCCDB.dbcmd("delete from AddressBook where LoginID  = '%s'", tmpLogin);
            CCCDB.dbcmd("delete from MailFilters where LoginID  = '%s'", tmpLogin);
            CCCDB.dbcmd("delete from Preferences where LoginID  = '%s'", tmpLogin);
            CCCDB.dbcmd("delete from JunkFilter  where LoginID  = '%s'", tmpLogin);
            CCCDB.dbcmd("delete from Identities  where UserName = '%s'", tmpLogin);
            emit setStatus("");
            QApplication::restoreOverrideCursor();
        } else {
            emit setStatus("");
            QMessageBox::critical(this, "MySQL Error", "Error connecting to the CCC database.\nUnable to wipe the CCC user information.\nAborting.");
            return;
        }
        
        // Remove their CCC Filters and other stuff first.
        QApplication::setOverrideCursor(waitCursor);
        emit setStatus("Removing email aliases...");
        ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
        QApplication::restoreOverrideCursor();
        if (mailDB.Connected()) {
            QApplication::setOverrideCursor(waitCursor);
            mailDB.dbcmd("delete from Virtual where Mailbox = '%s'", tmpLogin);
            emit setStatus("");
            QApplication::restoreOverrideCursor();
        } else {
            emit setStatus("");
            QMessageBox::critical(this, "MySQL Error", "Error connecting to the Postfix database.\nUnable to wipe the Postfix user information.\nAborting.");
            return;
        }
        
        emit setStatus("Connecting to BRASS server...");
        // Okay, they want to.  Wipe it.
        BC = new BrassClient();
        if (!BC->Connect()) {
            QMessageBox::critical(this, "BRASS Error", "Error connecting to the BRASS server.");
        } else {
            if (!BC->Authenticate()) {
                QMessageBox::critical(this, "BRASS Error", "Error authenticating with the BRASS server.");
            } else {
                // Well, we finally made it into brass.  Wipe the login.
                QApplication::setOverrideCursor(waitCursor);
                emit setStatus("Wiping account...");
                if (BC->WipeUser(tmpLogin)) {
                    // Went okay, update the database.
                    // Update any subscriptions, JIC.
                    updateDBForWipe(tmpLogin);
                } else {
                    QMessageBox::warning(this, "BRASS Error", "Unable to wipe the account.");
                }
                emit setStatus("");
                QApplication::restoreOverrideCursor();
            }
        }
        emit setStatus("");
        delete BC;
    }
}


/*
** updateDBForWipe - Goes through the database and updates all of the tables
**                   after wiping an account.
*/

void Tab_Logins::updateDBForWipe(const char * LoginID)
{
    char        tmpLogin[64];
    char        newLogin[64];
    char        tmpstr[1024];
    LoginsDB    LDB;
    ADB         DB;
    NotesDB     NDB;
    int         wasActive = 0;
	QDate       tmpDate;
	char        today[64];

    strcpy(tmpLogin, LoginID);
	tmpDate	= QDate::currentDate();
	sprintf(today, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());

    LDB.get(myCustID, LoginID);
    
	DB.dbcmd("update Subscriptions set Active = 0 where LoginID = '%s' and CustomerID = %ld", tmpLogin, myCustID);
	
    // Remove any custom flags.
    DB.dbcmd("delete from LoginFlagValues where LoginID = '%s'", tmpLogin);

	wasActive = LDB.getInt("Active");
	LDB.setValue("Active", (int) 0);
	LDB.setValue("Wiped", today);
	LDB.upd();
	
	// Now, just if they were open, update the AR.
	// If not, don't make an adjusting entry because
	// the lock routines should have already done that.
	if (wasActive && isManager()) {
  		QApplication::restoreOverrideCursor();
        if (QMessageBox::warning(this, "Prorate Charges", "Do you wish to prorate the charges for this login?", "&Yes", "&No", 0, 1) == 0) {
		    QApplication::setOverrideCursor(waitCursor);
			updateARForLock(tmpLogin);
            QApplication::restoreOverrideCursor();
		}
	    QApplication::setOverrideCursor(waitCursor);
	}

	NDB.setValue("LoginID", tmpLogin);
	NDB.setValue("CustomerID", myCustID);
	NDB.setValue("NoteType", "Operator");
	NDB.setValue("NoteText", "Login Wiped.");
	NDB.ins();
	
	// Now that they're wiped, rename this login
	// to show that it has been wiped.
	// Wiped logins should preface with "Wnn-" where
	// 'nn' is between 00 and 99.  The "W" should be
	// capitalized, that way it will appear at the
	// end of the sort list.
	int StartingNo = 0;
	int FoundOne   = 0;
	while (!FoundOne) {
	    sprintf(tmpstr, "W%02d-%s", StartingNo, tmpLogin);
	    DB.query("select * from Logins where CustomerID = %ld and LoginID = '%s'", myCustID, tmpstr);
	    if (!DB.rowCount) {
	        FoundOne = 1;
	        strcpy(newLogin, tmpstr);
	    } else {
	        StartingNo++;
	    }
	}
	
	// Now that we've got the newLogin, update
	// _ALL_ of the tables with it...
	// Not fun, but necessary.
	LDB.get(myCustID, tmpLogin);
	LDB.changeLoginID(newLogin);

	emit(refreshCust(myCustID));
	refreshLoginList(myCustID);
	
}


/*
** modemUsageReport  - Brings up a report of the customer's modem usage for
**                     the selected login.
*/

void Tab_Logins::modemUsageReport()
{
    QListViewItem   *curItem;
    ModemUsageReport *report;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
    	report = new ModemUsageReport();
    	report->show();
    	report->setLoginID(curItem->key(0,0));
    }
}

/*
** bandwidthUsageReport  - Brings up a report of the customer's traffic usage
**                         for the selected login.
*/

void Tab_Logins::bandwidthUsageReport()
{
    QListViewItem   *curItem;
    BandwidthUsageReport *report;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
    	report = new BandwidthUsageReport();
    	report->show();
    	report->setLoginID(curItem->key(0,0));
    }
}


/*
** changePassword - Brings up a dialog box that allows the user to change
**                  a user's password.
*/

void Tab_Logins::changePassword()
{
    QListViewItem   *curItem;
	LoginsDB	    LDB;
	
	curItem = list->currentItem();
    if (curItem != NULL) {
		// Make sure that the login is active before we change the password for it
		LDB.get(myCustID, (const char *) curItem->text(0));
		if (LDB.getInt("Active")) {
			ChangePassword *pc;
			pc = new ChangePassword(0, "", myCustID, (const char *) curItem->text(0));
			pc->show();
		} else {
			QMessageBox::information(this, "Unable to Change Password", "You may not change the\npassword of an inactive account.");
		}
	}
}

/*
** setPrimaryLogin   - After prompting, this sets the primary login to
**                     the selected login ID.
*/

void Tab_Logins::setPrimaryLogin()
{
    QListViewItem   *curItem;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
		char    tmpSt[1024];
		sprintf(tmpSt, "Are you sure you wish to set the primary\nlogin for this account to '%s'?", (const char *) curItem->text(0));
        if (QMessageBox::warning(this, "Set Primary Login", tmpSt, "&Yes", "&No", 0, 1) == 0) {
		    CustomersDB     CDB;
		    CDB.get(myCustID);
		    if (strcmp(CDB.getStr("PrimaryLogin"), curItem->text(0))) {
		        // The logins are different.  Update it.
		        sprintf(tmpSt, "Set primary login from %s to %s", (const char *) CDB.getStr("PrimaryLogin"), (const char *) curItem->text(0));
                CDB.setValue("PrimaryLogin", curItem->text(0));
                CDB.upd();
                
		        NotesDB NDB;
                NDB.setValue("LoginID", curItem->text(0));
                NDB.setValue("CustomerID", myCustID);
                NDB.setValue("NoteType", "Operator");
                NDB.setValue("NoteText", tmpSt);
                NDB.ins();

            	refreshLoginList(1);
                emit(refreshCust(myCustID));
		    } else {
            	QMessageBox::information(this, "Set Primary Login", "The specified login ID is\nalready the primary login ID.");
		    }
		}
    }
}

/*
** transferLogin   - Gives the user the ability to transfer the selected
**                   login ID to another account.
*/

void Tab_Logins::transferLogin()
{
    QListViewItem   *curItem;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
		TransferLogin   *TL;
		TL = new TransferLogin;
		TL->setSourceLogin(myCustID, (const char *) curItem->text(0));
    	connect(TL, SIGNAL(refreshLoginList(int)), SLOT(refreshLoginListV(int)));
		TL->show();
    }	
}


/*
** loginFlagsClicked - Gets called when the user clicks on the Login Flags
**                     button.  We use this to assign custom key/value pairs
**                     to individual logins.
*/

void Tab_Logins::loginFlagsClicked()
{
    QListViewItem   *curItem;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
        CustomLoginFlagEditor   *clfe;
        clfe = new CustomLoginFlagEditor(NULL, "flagEditor", myCustID, (const char *) curItem->text(0));
		clfe->show();
    }	
}

/*
** diskSpaceClicked  - Gets calle when the user clicks on the Disk Space
**                     button.  We use this to edit the user's quota remotely.
*/

void Tab_Logins::diskSpaceClicked()
{
}


/**
 * CustomLoginFlagEditor::CustomLoginFlagEditor()
 *
 * Constructor for the Custom Login Flag Editor.
 */
CustomLoginFlagEditor::CustomLoginFlagEditor
(
	QWidget* parent,
	const char* name,
	long CustID,
    const char *loginID

) : TAAWidget ( parent, name )
{

	setCaption( "Login Flags" );
	myCustID = CustID;
    strcpy(myLoginID, loginID);

    // Create labels to let the user know who we're editing
    QLabel  *custIDLabel = new QLabel(this, "custIDLabel");
    char    tmpStr[1024];
    sprintf(tmpStr, "%ld", myCustID);
    custIDLabel->setAlignment(AlignRight);
    custIDLabel->setText("Customer ID:");

    QLabel  *custIDValue = new QLabel(this, "custIDValue");
    custIDValue->setAlignment(AlignLeft);
    custIDValue->setText(tmpStr);

    QLabel  *loginIDLabel = new QLabel(this, "loginIDLabel");
    loginIDLabel->setAlignment(AlignRight);
    loginIDLabel->setText("Login ID:");

    QLabel  *loginIDValue = new QLabel(this, "loginIDValue");
    loginIDValue->setAlignment(AlignLeft);
    loginIDValue->setText(myLoginID);

    QStringList colHeaders;
    colHeaders += "Flag";
    colHeaders += "Value";
    // Create all of our widgets.
    flagTable = new QTable(0, 2, this, "flagTable");
    flagTable->setColumnLabels(colHeaders);

    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");

    // Label layout
    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(custIDLabel,              curRow, 0);
    gl->addWidget(custIDValue,              curRow, 1);
    curRow++;
    gl->addWidget(loginIDLabel,             curRow, 0);
    gl->addWidget(loginIDValue,             curRow, 1);
    curRow++;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    // Create the layout
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addLayout(gl, 0);
    ml->addWidget(flagTable, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    fillTable();

    // Connect our buttons
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
}

/**
 * CustomLoginFlagEditor::~CustomLoginFlagEditor()
 *
 * Destructor for the Custom Login Flag Editor.
 */
CustomLoginFlagEditor::~CustomLoginFlagEditor()
{
}

/**
 * CustomLoginFlagEditor::fillTable()
 *
 * Fills the table with the flags and values for the user.
 */
void CustomLoginFlagEditor::fillTable()
{
    ADB     db;
    long    loginType;
    QDict<QString> flagDict;

    // Get the login type.
    db.query("select LoginType from Logins where LoginID = '%s'", myLoginID);
    if (!db.rowCount) return;
    db.getrow();
    loginType = atoi(db.curRow["LoginType"]);

    // Get the available login flags for this login type
    db.query("select Tag, Value from LoginTypeFlags where LoginTypeID = %ld", loginType);
    if (!db.rowCount) return;

    // Load the dictionary
    while(db.getrow()) {
        flagDict.insert(db.curRow["Tag"], new QString(db.curRow["Value"]));
    }

    // Get whatever data is in the LoginFlagValues that is custom for this user.
    db.query("select * from LoginFlagValues where LoginID = '%s'", myLoginID);
    if (db.rowCount) while (db.getrow()) {
        flagDict.replace(db.curRow["LoginFlag"], new QString(db.curRow["FlagValue"]));
    }

    // Now put the stuff from our flagDict into the grid
    QDictIterator<QString> it(flagDict);
    for( ; it.current(); ++it ) {
        flagTable->insertRows(flagTable->numRows(), 1);
        flagTable->setText(flagTable->numRows()-1, 0, it.currentKey());
        flagTable->setText(flagTable->numRows()-1, 1, it.current()->ascii());
    }

    // Set column 0(1) to be read only
    flagTable->setColumnReadOnly(0, true);
    flagTable->adjustColumn(1);

}

/**
 * CustomLoginFlagEditor::cancelClicked()
 *
 * Closes the window without saving.
 */
void CustomLoginFlagEditor::cancelClicked()
{
    delete this;
}

/**
 * CustomLoginFlagEditor::saveClicked()
 *
 * Walks through the flagTable and saves the values.
 */
void CustomLoginFlagEditor::saveClicked()
{
    ADB db;

    if (flagTable->numRows()) {
        // Loop through the rows, grabbing the key/value pairs
        for (int i = 0; i < flagTable->numRows(); i++) {
            long        result;
            QString     flag, val;
            flag = flagTable->text(i-1,0);
            val  = flagTable->text(i-1,1);
            if (flag != NULL) {
                result = db.dbcmd("replace into LoginFlagValues values ('%s', '%s', '%s')", myLoginID, flag.ascii(), val.ascii());
                if (result < 0) {
			        QMessageBox::warning(this, "Database Error", "Unable to save flag.");
                }
            }
        }
    }

    // Let our parent know that something has changed
    emit(refreshCust(myCustID));
    emit(customerUpdated(myCustID));
    delete this;
}

