/**
 * GLAccountEditor.cpp - Allows a manager or administrator to edit or create
 * an account in the GL/Chart of Accounts.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdio.h>
#include <stdlib.h>

#include <qstring.h>
#include <qlayout.h>

#include <ADB.h>
#include <BlargDB.h>
#include <GenLedger.h>
#include <GLAccountEditor.h>
#include <Cfg.h>

GLAccountEditor::GLAccountEditor
(
	QWidget* parent,
	const char* name,
	int AcctNo
) : TAAWidget( parent, name )
{
    ADB     DB;

    // Load our GL Account type index.
    glAcctTypes = GLAccountTypesDB::getAccountTypeList();
    intAcctNoIndex = 0;

    QLabel  *accountNoLabel = new QLabel(this, "accountNoLabel");
    accountNoLabel->setText("Account Number:");
    accountNoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    accountNo = new QLineEdit(this, "accountNo");
    accountNo->setInputMask(cfgVal("GLAccountNoMask"));

    QLabel  *acctNameLabel = new QLabel(this, "acctNameLabel");
    acctNameLabel->setText("Account Name:");
    acctNameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    acctName = new QLineEdit(this, "acctName");
    acctName->setMaxLength(80);

    QLabel  *parentAcctLabel = new QLabel(this, "ParentAcctLabel");
    parentAcctLabel->setText("Parent Account:");
    parentAcctLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    parentAcct = new QComboBox(false, this, "parentAcct");
    parentIDX = 0;
    loadParentList();


    QLabel  *accountTypeLabel = new QLabel(this, "accountTypeLabel");
    accountTypeLabel->setText("Account Type:");
    accountTypeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    accountType = new QComboBox(false, this, "accountType");
    // Fill the account type list.
    acctTypeIDX = new int[glAcctTypes.count()+1];
    int idxPtr = 0;
    GLAccountTypeRecord *rec;
    for (rec = glAcctTypes.first(); rec; rec = glAcctTypes.next()) {
        acctTypeIDX[idxPtr] = rec->accountType;
        accountType->insertItem(rec->description);
        idxPtr++;
    }

    QLabel  *providerAcctNoLabel = new QLabel(this, "providerAcctNoLabel");
    providerAcctNoLabel->setText("Provider Account Number:");
    providerAcctNoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    providerAcctNo = new QLineEdit(this, "providerAcctNo");
    providerAcctNo->setMaxLength(80);

    QLabel  *taxLineLabel = new QLabel(this, "taxLineLabel");
    taxLineLabel->setText("Tax Line:");
    taxLineLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    taxLine = new QLineEdit(this, "taxLine");
    taxLine->setMaxLength(250);

    QLabel  *balanceLabel = new QLabel(this, "balanceLabel");
    balanceLabel->setText("Account Balance:");
    balanceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    balance = new QLabel(this, "balance");
    balance->setText("");
    balance->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel  *transCountLabel = new QLabel(this, "transCountLabel");
    transCountLabel->setText("Transaction Count:");
    transCountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    transCount = new QLabel(this, "transCount");
    transCount->setText("");
    transCount->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAccount()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAccount()));

    // Create our layout.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);
    
    // A grid for most of the widgets
    QGridLayout *gl = new QGridLayout(2,6);
    int curRow = 0;

    gl->addWidget(accountNoLabel,       curRow, 0);
    gl->addWidget(accountNo,            curRow, 1);
    curRow++;
    gl->addWidget(acctNameLabel,        curRow, 0);
    gl->addWidget(acctName,             curRow, 1);
    curRow++;
    gl->addWidget(parentAcctLabel,      curRow, 0);
    gl->addWidget(parentAcct,           curRow, 1);
    curRow++;
    gl->addWidget(accountTypeLabel,     curRow, 0);
    gl->addWidget(accountType,          curRow, 1);
    curRow++;
    gl->addWidget(providerAcctNoLabel,  curRow, 0);
    gl->addWidget(providerAcctNo,       curRow, 1);
    curRow++;
    gl->addWidget(taxLineLabel,         curRow, 0);
    gl->addWidget(taxLine,              curRow, 1);
    curRow++;
    gl->addWidget(balanceLabel,         curRow, 0);
    gl->addWidget(balance,              curRow, 1);
    curRow++;
    gl->addWidget(transCountLabel,      curRow, 0);
    gl->addWidget(transCount,           curRow, 1);
    curRow++;

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    ml->addLayout(gl, 0);
    ml->addStretch(1);

    // Button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    setIntAccountNo(AcctNo);
}


GLAccountEditor::~GLAccountEditor()
{
    if (parentIDX) {
        delete parentIDX;
        parentIDX = 0;
    }
}

/**
 * loadParentList()
 *
 * Loads the list of possible parent accounts into memory.
 */
void GLAccountEditor::loadParentList()
{
    ADB     DB;
    if (parentIDX) {
        delete parentIDX;
        parentIDX = 0;
    }

    parentAcct->clear();
    parentAcct->insertItem("None");

    DB.query("select * from Accounts where ParentID = 0 and IntAccountNo <> %d", myIntAccountNo);
    parentIDX = new int[DB.rowCount+2];
    int idxPtr = 1;
    parentIDX[0] = 0;
    if (DB.rowCount) while (DB.getrow()) {
        QString tmpStr;
        tmpStr = DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        parentAcct->insertItem(tmpStr);
        parentIDX[idxPtr] = atoi(DB.curRow["IntAccountNo"]);
        idxPtr++;
    }
}

/**
 * setIntAccountNo()
 *
 * Sets up the account number and puts the editor into edit mode.
 */
void GLAccountEditor::setIntAccountNo(int newIntAccountNo)
{
    ADB    DB;
    int    parentID = 0;

    myIntAccountNo = newIntAccountNo;
    
    if (myIntAccountNo) {
    	setCaption("Edit Account");
    } else {
        setCaption("New Account");
    }

    // Clear our mappings to internal ID's
    if(intAcctNoIndex) delete intAcctNoIndex;
    
    // We're editing one, so get the data for it.
    if (myIntAccountNo) {
        AccountsDB acctDB;
        acctDB.get(myIntAccountNo);
        accountNo->setText(acctDB.AccountNo);
        acctName->setText(acctDB.AcctName);
        loadParentList();
        parentID = acctDB.ParentID;
        // Find our account type in the list.
        for (int i = 0; i < glAcctTypes.count(); i++) {
            if (acctTypeIDX[i] == acctDB.AccountType) accountType->setCurrentItem(i);
        }
        // Find our parent in the list.
        for (int i = 0; i < parentAcct->count(); i++) {
            if (parentIDX[i] == parentID) parentAcct->setCurrentItem(i);
        }
        /*
        if (tmpSubOf) {
            for (int i = 0; i < subAcctOf->count(); i++) {
                if (intAcctNoIndex[i] == tmpSubOf) {
                    subAcctOf->setCurrentItem(i);
                }
            }
        }
        acctNumber->setValue(acctDB.AccountNo);
        acctType->setEnabled(false);
        acctNumber->setEnabled(false);
        */
        taxLine->setText(acctDB.TaxLine);
        QString tmpStr;
        tmpStr = tmpStr.sprintf("%.2f", acctDB.Balance);
        balance->setText(tmpStr);
        tmpStr.setNum(acctDB.TransCount);
        transCount->setText(tmpStr);
    }
}

/**
 * acctTypeChanged()
 *
 * Updates the account number ranges when the user selects
 * the type of account this is.
 */
void GLAccountEditor::accountTypeChanged(int newIDX)
{
    int acctRange = (newIDX + 1) * 1000;

    // Fill the combo box...
    ADB DB;
    DB.query("select IntAccountNo, AccountNo, AcctName from Accounts where IntAccountNo <> %d and ParentID = 0 and AccountType = %d order by AccountType, AcctName", myIntAccountNo, newIDX);
    if (intAcctNoIndex) delete intAcctNoIndex;
    intAcctNoIndex = new(int[DB.rowCount+1]);
    intAcctNoIndex[0] = 0;
    int indexPtr = 0;
    parentAcct->clear();
    parentAcct->insertItem("None");
    while (DB.getrow()) {
        indexPtr++;
        intAcctNoIndex[indexPtr] = atoi(DB.curRow["IntAccountNo"]);
        QString tmpStr;
        tmpStr = DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        parentAcct->insertItem(tmpStr);
    }
}

// The user clicked on the close button...

void GLAccountEditor::cancelAccount()
{
    close();
    delete this;
}

// The user clicked on the save button...

void GLAccountEditor::saveAccount()
{
    ADB         DB;
    AccountsDB  acctDB;
    
    if (myIntAccountNo) {
        acctDB.get(myIntAccountNo);
    } else {
        acctDB.IntAccountNo = 0;
        acctDB.Balance = 0.00;
        acctDB.TransCount = 0;
    }
    
    // Get the data from the dialog
    acctDB.AccountNo        = accountNo->text();
    acctDB.AcctName         = acctName->text();
    acctDB.ParentID         = parentIDX[parentAcct->currentItem()];
    acctDB.AccountType      = acctTypeIDX[accountType->currentItem()];
    acctDB.ProviderAccountNo= providerAcctNo->text();
    acctDB.TaxLine          = taxLine->text();
    
    if (!myIntAccountNo) {
        acctDB.ins();
    } else {
        // Updating a record...
        acctDB.IntAccountNo = myIntAccountNo;
        acctDB.upd();
    }
    
    emit refresh(1);
    close();
    delete this;
}

