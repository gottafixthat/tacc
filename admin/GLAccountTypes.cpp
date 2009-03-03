/**
 * GLAccountTypes.h - Displays the list of GL Account types in TACC
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
#include <qmessagebox.h>
#include <qlayout.h>

#include <ADB.h>
#include <GLAccountTypes.h>
#include <GenLedger.h>

/**
 * GLAccountTypeEditor()
 *
 * Constructor.
 */
GLAccountTypeEditor::GLAccountTypeEditor
(
    int acctType,
    QWidget *parent,
    const char *name
) : TAAWidget(parent, name)
{
    setCaption("GL Account Type Editor");

    QLabel  *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    description = new QLineEdit(this, "description");
    description->setMaxLength(80);
    
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(3,3);
    int curRow = 0;
    gl->addWidget(descriptionLabel, curRow, 0);
    gl->addWidget(description,      curRow, 1);
    curRow++;
    gl->setColStretch(0,0);
    gl->setColStretch(1,1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);
    ml->addLayout(bl, 0);

    if (acctType) setAccountType(acctType);
}

/**
 * ~GLAccountTypeEditor()
 *
 * Destructor.
 */
GLAccountTypeEditor::~GLAccountTypeEditor()
{
}

/**
 * setAccountType()
 *
 * Loads the account type into memory for editing.
 */
void GLAccountTypeEditor::setAccountType(int newAcctType)
{
    ADB     DB;
    DB.query("select * from GLAccountTypes where AccountType = %d", newAcctType);
    if (DB.rowCount) {
        DB.getrow();
        myAccountType = newAcctType;
        description->setText(DB.curRow["Description"]);
        setCaption("Edit GL Account Type");
    } else {
        myAccountType = 0;
        description->setText("");
        setCaption("New GL Account Type");
    }
}

/**
 * saveClicked()
 *
 * Called when the user clicks save.
 */
void GLAccountTypeEditor::saveClicked()
{
    QString tmpSt;
    tmpSt = description->text();
    if (!tmpSt.length()) {
        QMessageBox::critical(this, "Save GL Account Type", "The account description may not be blank.");
        return;
    }

    ADBTable    DB("GLAccountTypes");
    if (myAccountType) {
        DB.get(myAccountType);
        DB.setValue("Description", description->text().ascii());
        DB.upd();
    } else {
        DB.setValue("AccountType",  myAccountType);
        DB.setValue("Description", description->text().ascii());
        DB.ins();
    }
    emit(refresh(1));
    delete this;
}

/**
 * cancelClicked()
 *
 * Called when the user hits the cancel button.
 */
void GLAccountTypeEditor::cancelClicked()
{
    delete this;
}


GLAccountTypes::GLAccountTypes
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "GL Account Types" );

    // Load the list of GL Account Types
    glAcctTypes = GLAccountTypesDB::getAccountTypeList();

    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newAcctType()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editAcctType()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deleteAcctType()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(closeClicked()), CTRL+Key_C);
    
    menu = new QMenuBar(this, "MainMenu");
    menu->insertItem("&Options", options);

    // Create the buttons for the top of the window
    QPushButton *newButton = new QPushButton(this, "NewButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newAcctType()));
    
    QPushButton *editButton = new QPushButton(this, "EditButton");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editAcctType()));

    QPushButton *deleteButton = new QPushButton(this, "DeleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteAcctType()));

    QPushButton *closeButton = new QPushButton(this, "CloseButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    acctTypeList = new QListView(this, "acctTypeList");
    acctTypeList->addColumn("Name");
    acctTypeList->setAllColumnsShowFocus(true);
    acctTypeList->setRootIsDecorated(false);
    accountTypeCol = 1;

    // Create our layout and add our widgets.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(menu, 0);
    
    // Add a widget for our menu buttons.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);
    bl->addStretch(1);

    ml->addLayout(bl, 0);
    ml->addWidget(acctTypeList, 1);

    refreshList(1);
}


GLAccountTypes::~GLAccountTypes()
{
}


void GLAccountTypes::closeClicked()
{
    delete this;
}


// refreshList    - Connects to the database and refreshes the Accounts
//                  list.
//

void GLAccountTypes::refreshList(int)
{
    // Calling addToList with 0 will get all of the accounts.
    acctTypeList->clear();
    glAcctTypes = GLAccountTypesDB::getAccountTypeList();
    GLAccountTypeRecord *rec;
    for (rec = glAcctTypes.first(); rec; rec = glAcctTypes.next()) {
        QString idStr;
        idStr.setNum(rec->accountType);
        new QListViewItem(acctTypeList, rec->description, idStr);
    }
}


//
// newAccount  - Allows the user to create a new PaymentTerms
//

void GLAccountTypes::newAcctType()
{
    GLAccountTypeEditor *newAcctType;
    newAcctType = new GLAccountTypeEditor(0);
    newAcctType->show();
    connect(newAcctType, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    
}

//
// editAccount  - Allows the user to edit an account
//

void GLAccountTypes::editAcctType()
{
    QListViewItem *curItem;
    GLAccountTypeEditor *acctType;
    curItem = acctTypeList->currentItem();
    if (curItem != NULL) {
        acctType = new GLAccountTypeEditor(atoi(curItem->text(accountTypeCol)));
        acctType->show();
        connect(acctType, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
}


//
// deleteAccount  - Allows the user to delete an Account
//

void GLAccountTypes::deleteAcctType()
{
    char   tmpstr[256];
    ADB    DB;
    QListViewItem   *curItem;
    
    curItem = acctTypeList->currentItem();
    if (curItem != NULL) {
        sprintf(tmpstr, "Are you sure you wish to delete the\nGL Account Type '%s'?", curItem->key(0,0).ascii());
        if (QMessageBox::warning(this, "Delete Account Type", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            DB.query("select IntAccountNo from Accounts where AccountType = %d", curItem->key(accountTypeCol, 0).toInt());
            if (DB.rowCount) {
                QMessageBox::critical(this, "Delete Account", "The account type could not be delete as there are accounts using it.");
            } else {
                DB.dbcmd("delete from GLAccountTypes where AccountType = %d", curItem->key(accountTypeCol, 0).toInt());
                refreshList(1);
            }
        }
    }
}

