/**
 * ChartOfAccounts.h - Displays the Chart of Accounts
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <BlargDB.h>
#include <mysql/mysql.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <stdlib.h>
#include <stdio.h>
#include <ADB.h>
#include <ChartOfAccounts.h>
#include <GenLedger.h>
#include <GLAccountEditor.h>

ChartOfAccounts::ChartOfAccounts
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Chart of Accounts" );

    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newAccount()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editAccount()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteAccount()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu = new QMenuBar(this, "MainMenu");
    menu->insertItem("&Options", options);

    // Create the buttons for the top of the window
    QPushButton *newButton = new QPushButton(this, "NewButton");
    newButton->setText("New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newAccount()));
    
    QPushButton *editButton = new QPushButton(this, "EditButton");
    editButton->setText("Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editAccount()));

    QPushButton *deleteButton = new QPushButton(this, "DeleteButton");
    deleteButton->setText("Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteAccount()));

    QPushButton *closeButton = new QPushButton(this, "CloseButton");
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(Hide()));

    QPushButton *registerButton = new QPushButton(this, "RegisterButton");
    registerButton->setText("Register");
    registerButton->setEnabled(false);

    list = new QListView(this, "AccountList");
    list->addColumn("Number");
    list->addColumn("Name");
    list->addColumn("Type");
    list->addColumn("Balance");
    list->setFocus();
    list->setAllColumnsShowFocus(TRUE);
    list->setRootIsDecorated(TRUE);

    // Create our layout and add our widgets.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(menu, 0);
    
    // Add a widget for our menu buttons.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);
    bl->addWidget(registerButton, 0);
    bl->addStretch(1);

    ml->addLayout(bl, 0);
    ml->addWidget(list, 1);

    refreshList(1);
}


ChartOfAccounts::~ChartOfAccounts()
{
}


void ChartOfAccounts::Hide()
{
    delete this;
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

void ChartOfAccounts::addToList(int AcctNo, QListViewItem *parent)
{
    ADB	      DB;
    QString   tmpBal;
    QString   tmpName;
    QString   tmpAcctType;
    char      tmpAcctTypeC[128];
    QListViewItem   *curItem;
        
    
    DB.query("select AccountNo, AcctName, Balance, HasSubAccts, AcctType from Accounts where SubAcctOf = %d order by AcctType", AcctNo);
    
    while(DB.getrow()) {
        
        tmpAcctType = GLAccountTypes[atoi(DB.curRow["AcctType"])];
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

void ChartOfAccounts::refreshList(int)
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
        tmpAcctType = GLAccountTypes[atoi(DB.curRow["AcctType"])];
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

void ChartOfAccounts::newAccount()
{
    GLAccountEditor *newAccounts;
    newAccounts = new GLAccountEditor(0,"",0);
    newAccounts->show();
    connect(newAccounts, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    
}

//
// editAccount  - Allows the user to edit an account
//

void ChartOfAccounts::editAccount()
{
    QListViewItem *curItem;
    GLAccountEditor *account;
    curItem = list->currentItem();
    if (curItem != NULL) {
        account = new GLAccountEditor(0, "", atoi(curItem->text(0)));
        account->show();
        connect(account, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
}

//
// editAccountL  - Allows the user to edit a new VendorType from the list
//

void ChartOfAccounts::editAccountL(int msg)
{
    msg = 0;
    editAccount();
}

//
// deleteAccount  - Allows the user to delete an Account
//

void ChartOfAccounts::deleteAccount()
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
