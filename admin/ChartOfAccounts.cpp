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
#include <qnamespace.h>
#include <qmessagebox.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <Q3PopupMenu>
#include <stdlib.h>
#include <stdio.h>
#include <TAATools.h>
#include <ADB.h>
#include <GLAccountTypes.h>
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

    // Load the list of GL Account Types
    glAcctTypes = GLAccountTypesDB::getAccountTypeList();

    Q3PopupMenu * options = new Q3PopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newAccount()), Qt::CTRL+Qt::Key_N);
    options->insertItem("Edit", this, SLOT(editAccount()), Qt::CTRL+Qt::Key_E);
    options->insertItem("Delete", this, SLOT(deleteAccount()), Qt::CTRL+Qt::Key_D);
    if (isAdmin()) {
        options->insertSeparator();
        options->insertItem("Edit GL Account Types", this, SLOT(editAccountTypes()));
    }
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), Qt::CTRL+Qt::Key_C);
    
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

    list = new Q3ListView(this, "AccountList");
    list->addColumn("Number");
    list->addColumn("Name");
    list->addColumn("Type");
    list->addColumn("Balance");
    list->setColumnAlignment(3, Qt::AlignRight);
    list->setFocus();
    list->setAllColumnsShowFocus(TRUE);
    list->setRootIsDecorated(TRUE);
    intAcctNoCol = 4;

    // Create our layout and add our widgets.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(menu, 0);
    
    // Add a widget for our menu buttons.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
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

int ChartOfAccounts::addToList(int ParentID, Q3ListViewItem *parent)
{
    ADB	            DB;
    QString         tmpBal;
    QString         tmpName;
    QString         tmpAcctType;
    Q3ListViewItem   *curItem;
    int             retVal = 0;
    int             intAcctNo;
        
    
    DB.query("select * from Accounts where ParentID = %d order by AccountType, AccountNo", ParentID);
    retVal = DB.rowCount;
    if (retVal) {
        
        while(DB.getrow()) {
            
            intAcctNo = atoi(DB.curRow["IntAccountNo"]);

            // Find the name of the account type
            tmpAcctType = "";
            GLAccountTypeRecord *rec;
            for (rec = glAcctTypes.first(); rec; rec = glAcctTypes.next()) {
                if (rec->accountType == atoi(DB.curRow["AccountType"])) {
                    tmpAcctType = rec->description;
                }
            }
            
            if (!parent) curItem = new Q3ListViewItem(list, DB.curRow["AccountNo"], DB.curRow["AcctName"], tmpAcctType, DB.curRow["Balance"], DB.curRow["IntAccountNo"]);
            else curItem = new Q3ListViewItem(parent, DB.curRow["AccountNo"], DB.curRow["AcctName"], tmpAcctType, DB.curRow["Balance"], DB.curRow["IntAccountNo"]);
            if (addToList(intAcctNo, curItem)) {
                curItem->setOpen(true);
            }
        }
    }

    return retVal;
}


// refreshList    - Connects to the database and refreshes the Accounts
//                  list.
//

void ChartOfAccounts::refreshList(int)
{
    // Calling addToList with 0 will get all of the accounts.
    list->clear();
    addToList(0, NULL);
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
    Q3ListViewItem *curItem;
    GLAccountEditor *account;
    curItem = list->currentItem();
    if (curItem != NULL) {
        account = new GLAccountEditor(0, "", atoi(curItem->text(intAcctNoCol)));
        account->show();
        connect(account, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
}


//
// deleteAccount  - Allows the user to delete an Account
//

void ChartOfAccounts::deleteAccount()
{
    char   tmpstr[256];
    ADB    DB;
    Q3ListViewItem   *curItem;
    
    curItem = list->currentItem();
    if (curItem != NULL) {
        sprintf(tmpstr, "Are you sure you wish to delete the\nAccount '%s'?", curItem->key(1,0).ascii());
        if (QMessageBox::warning(this, "Delete Account", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            AccountsDB ADB;
            if (ADB.del(atoi(curItem->key(intAcctNoCol,0)))) {
                QMessageBox::critical(this, "Delete Account", "The account could not be delete as it contains transactions.");
            } else {
                refreshList(1);
            }
        };
    }
}

/**
 * editAccountTypes()
 *
 * Brings up the GL Account Type list.
 */
void ChartOfAccounts::editAccountTypes()
{
    GLAccountTypes   *glatl = new GLAccountTypes();
    glatl->show();
}
