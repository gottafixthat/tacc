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

GLAccountEditor::GLAccountEditor
(
	QWidget* parent,
	const char* name,
	int AcctNo
) : TAAWidget( parent, name )
{
    ADB     DB;

    AcctNoIndex = 0;

    QLabel  *acctNameLabel = new QLabel(this, "acctNameLabel");
    acctNameLabel->setText("Account Name:");
    acctNameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    acctName = new QLineEdit(this, "acctName");
    acctName->setMaxLength(30);

    QLabel  *subAcctOfLabel = new QLabel(this, "subAcctOfLabel");
    subAcctOfLabel->setText("Sub Account Of:");
    subAcctOfLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    subAcctOf = new QComboBox(false, this, "subAcctOf");
    subAcctOf->insertItem("None");

    QLabel  *acctTypeLabel = new QLabel(this, "acctTypeLabel");
    acctTypeLabel->setText("Account Type:");
    acctTypeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    acctType = new QComboBox(false, this, "acctType");
    acctType->insertStringList(GLAccountTypes);
    connect(acctType, SIGNAL(activated(int)), this, SLOT(acctTypeChanged(int)));

    QLabel  *acctNumberLabel = new QLabel(this, "acctNumberLabel");
    acctNumberLabel->setText("Account Number:");
    acctNumberLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    acctNumber = new QSpinBox(this, "acctNumber");
    acctNumber->setRange(1000, 1999);
    acctNumber->setValue(1000);

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

    gl->addWidget(acctNameLabel,        curRow, 0);
    gl->addWidget(acctName,             curRow, 1);
    curRow++;
    gl->addWidget(subAcctOfLabel,       curRow, 0);
    gl->addWidget(subAcctOf,            curRow, 1);
    curRow++;
    gl->addWidget(acctTypeLabel,        curRow, 0);
    gl->addWidget(acctType,             curRow, 1);
    curRow++;
    gl->addWidget(acctNumberLabel,      curRow, 0);
    gl->addWidget(acctNumber,           curRow, 1);
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

    setAccountNo(AcctNo);
}


GLAccountEditor::~GLAccountEditor()
{
}

/**
 * setAccountNo()
 *
 * Sets up the account number and puts the editor into edit mode.
 */
void GLAccountEditor::setAccountNo(int newAccountNo)
{
    ADB    DB;
    int    tmpSubOf = 0;

    myAccountNo = newAccountNo;
    
    if (myAccountNo) {
    	setCaption("Edit Account");
    } else {
        setCaption("New New");
    }

    // Clear our mappings to internal ID's
    delete AcctNoIndex;
    
    // We're editing one, so get the data for it.
    if (myAccountNo) {
        AccountsDB acctDB;
        acctDB.get(myAccountNo);
        acctName->setText(acctDB.AcctName);
        tmpSubOf = acctDB.SubAcctOf.toInt();
        acctType->setCurrentItem(acctDB.AcctType.toInt());
        acctTypeChanged(acctDB.AcctType.toInt());
        // Find our parent in the sub account list.
        if (tmpSubOf) {
            for (int i = 0; i < subAcctOf->count(); i++) {
                if (AcctNoIndex[i] == tmpSubOf) {
                    subAcctOf->setCurrentItem(i);
                }
            }
        }
        acctNumber->setValue(acctDB.AccountNo);
        acctType->setEnabled(false);
        acctType->setEnabled(false);
        acctNumber->setEnabled(false);
        taxLine->setText(acctDB.TaxLine);
        acctDB.Balance.sprintf("%12.2f", acctDB.Balance.toFloat());
        acctDB.Balance.stripWhiteSpace();
        balance->setText(acctDB.Balance);
        transCount->setText(acctDB.TransCount);
    }

    
    acctName->setFocus();
}

/**
 * acctTypeChanged()
 *
 * Updates the account number ranges when the user selects
 * the type of account this is.
 */
void GLAccountEditor::acctTypeChanged(int newIDX)
{
    int acctRange = (newIDX + 1) * 1000;
    acctNumber->setRange(acctRange, acctRange+999);
    acctNumber->setValue(acctRange);

    // Fill the combo box...
    ADB DB;
    DB.query("select AccountNo, AcctName from Accounts where AccountNo <> %d and SubAcctOf = 0 and AcctType = %d order by AcctType, AcctName", myAccountNo, newIDX);
    if (AcctNoIndex) delete AcctNoIndex;
    AcctNoIndex = new(int[DB.rowCount+1]);
    AcctNoIndex[0] = 0;
    int indexPtr = 0;
    subAcctOf->clear();
    subAcctOf->insertItem("None");
    while (DB.getrow()) {
        indexPtr++;
        AcctNoIndex[indexPtr] = atoi(DB.curRow["AccountNo"]);
        subAcctOf->insertItem(DB.curRow["AcctName"]);
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
    
    if (myAccountNo) {
        acctDB.get(myAccountNo);
    } else {
        acctDB.HasSubAccts.setNum(0);
        acctDB.Reimbursable.setNum(0);
        acctDB.Balance.setNum(0.00);
        acctDB.TransCount.setNum(0);
    }
    
    // Get the data from the dialog
    acctDB.AcctName           = acctName->text();
    acctDB.SubAcctOf.setNum(AcctNoIndex[subAcctOf->currentItem()]);
    acctDB.AcctType.setNum(acctType->currentItem());
    //acctDB.AcctNumber         = acctNumber->text();
    acctDB.TaxLine            = taxLine->text();
    
    if (!myAccountNo) {
        // New record...
        // Determine the new account number for it.
        int done = 0;
        myAccountNo = acctNumber->value();
        while (!done) {
            DB.query("select AccountNo from Accounts where AccountNo = %d", myAccountNo);
            if (!DB.rowCount) {
                done = 1;
            } else {
                myAccountNo++;
            }
        }
        acctDB.AccountNo = myAccountNo;
        acctDB.ins();
    } else {
        // Updating a record...
        acctDB.AccountNo = myAccountNo;
        acctDB.upd();
    }
    
    emit refresh(1);
    close();
    delete this;
}

