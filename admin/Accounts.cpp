/*
** $Id$
**
***************************************************************************
**
** Accounts - Displays a list of accounts.
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
** $Log: Accounts.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "BlargDB.h"
#include "Accounts.h"
#include "AccountsEdit.h"
#include <mysql/mysql.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <stdlib.h>
#include <stdio.h>
#include <ADB.h>

#define Inherited AccountsData

Accounts::Accounts
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Chart of Accounts" );

    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newAccount()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editAccount()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteAccount()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);
    
    list->setFocus();
    list->setAllColumnsShowFocus(TRUE);
    list->setRootIsDecorated(TRUE);

}


Accounts::~Accounts()
{
}


void Accounts::Hide()
{
    hide();
}

// addToList   - Adds to the Accounts List.  This function is actually called
//               recursivly by the refreshList routine, and should not be
//               called directly.
//
//   Args      - AcctNo should be the one we're getting sub types for.
//
//                  We cheat a bit here and do the query ourselves to 
//                  speed things up since the BlargDB routines can only
//                  handle one active query at a time...
//

void Accounts::addToList(int AcctNo, QListViewItem *parent)
{
    ADB	      DB;
    QString   tmpBal;
    QString   tmpName;
    QString   tmpAcctType;
    char      tmpAcctTypeC[128];
    QListViewItem   *curItem;
        
    
    DB.query("select AccountNo, AcctName, Balance, HasSubAccts, AcctType from Accounts where SubAcctOf = %d order by AcctType", AcctNo);
    
    while(DB.getrow()) {
        
        tmpAcctType = ACCOUNTTYPES[atoi(DB.curRow["AcctType"])];
        tmpAcctType.truncate(20);
        strcpy(tmpAcctTypeC, tmpAcctType);
        
        curItem = new QListViewItem(parent, DB.curRow["AccountNo"], DB.curRow["AcctName"], tmpAcctTypeC, DB.curRow["Balance"]);
        list->triggerUpdate();
        if (atoi(DB.curRow["HasSubAccts"])) {
            addToList(atoi(DB.curRow["AccountNo"]), curItem);
            curItem->setOpen(TRUE);
            list->triggerUpdate();
        }
    }
}


// refreshList    - Connects to the database and refreshes the Accounts
//                  list.
//

void Accounts::refreshList(int)
{
    ADB     DB;
    QString tmpBal;
    QString tmpName;
    QString   tmpAcctType;
    char      tmpAcctTypeC[128];
    QListViewItem   *curItem;
    
    // Save the state of the list.
    
    list->clear();
    DB.query("select AccountNo, AcctName, Balance, HasSubAccts, AcctType from Accounts where SubAcctOf = 0 order by AcctType");
    while(DB.getrow()) {
        
        // tmpBal = DB.curRow[2];
        tmpAcctType = ACCOUNTTYPES[atoi(DB.curRow["AcctType"])];
        tmpAcctType.truncate(20);
        strcpy(tmpAcctTypeC, tmpAcctType);
        
        curItem = new QListViewItem(list, DB.curRow["AccountNo"], DB.curRow["AcctName"], tmpAcctTypeC, DB.curRow["Balance"]);
        if (atoi(DB.curRow["HasSubAccts"])) {
            addToList(atoi(DB.curRow["AccountNo"]), curItem);
            curItem->setOpen(TRUE);
        }
    }
    
    list->triggerUpdate();
}


//
// newAccount  - Allows the user to create a new PaymentTerms
//

void Accounts::newAccount()
{
    AccountsEdit * newAccounts;
    newAccounts = new AccountsEdit(0,"",0);
    newAccounts->show();
    connect(newAccounts, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    
}

//
// editAccount  - Allows the user to edit an account
//

void Accounts::editAccount()
{
    QListViewItem *curItem;
    AccountsEdit * newAccounts;
    curItem = list->currentItem();
    if (curItem != NULL) {
        newAccounts = new AccountsEdit(0, "", atoi(curItem->text(0)));
        newAccounts->show();
        connect(newAccounts, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
}

//
// editAccountL  - Allows the user to edit a new VendorType from the list
//

void Accounts::editAccountL(int msg)
{
    msg = 0;
    editAccount();
}

//
// deleteAccount  - Allows the user to delete an Account
//

void Accounts::deleteAccount()
{
    char   tmpstr[256];
    ADB    DB;
    QListViewItem   *curItem;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
        DB.query("select AcctName from Accounts where AccountNo = %d", atoi(curItem->text(0)));
        DB.getrow();
        sprintf(tmpstr, "Are you sure you wish to delete the\nAccount '%s'?", DB.curRow["AcctName"]);
        if (QMessageBox::warning(this, "Delete Account", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            AccountsDB ADB;
            if (ADB.del(atoi(curItem->text(0)))) {
                QMessageBox::critical(this, "Delete Account", "The account could not be delete as it contains transactions.");
            } else {
                refreshList(1);
            }
        };
    }
}

