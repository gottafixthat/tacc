/**********************************************************************

	--- Dlgedit generated file ---

	File: AccountsEdit.cpp
	Last generated: Fri Sep 19 17:56:49 1997

 *********************************************************************/

#include "BlargDB.h"
#include "AccountsEdit.h"
#include <qstring.h>
#include <stdlib.h>
#include <stdio.h>
#include <ADB.h>

#define Inherited AccountsEditData

AccountsEdit::AccountsEdit
(
	QWidget* parent,
	const char* name,
	int AcctNo
)
	:
	Inherited( parent, name )
{
    ADB    DB;
    char   tmpstr[128];
    int    indexPtr = 0;
    int    tmpSubOf = 0;
    
    AccountNo = AcctNo;
    
    if (AccountNo) {
    	setCaption("Edit Account");
    } else {
        setCaption("New New");
    }

    // Clear our mappings to internal ID's
    AcctNoIndex = NULL;
    
    // We're editing one, so get the data for it.
    if (AccountNo) {
        AccountsDB acctDB;
        acctDB.get(AccountNo);
        acctName->setText(acctDB.AcctName);
        tmpSubOf = acctDB.SubAcctOf.toInt();
        acctType->setCurrentItem(acctDB.AcctType.toInt());
        acctNumber->setText(acctDB.AcctNumber);
        taxLine->setText(acctDB.TaxLine);
        sprintf(tmpstr, "%d", AccountNo);
        accountNo->setText(tmpstr);
        acctDB.Balance.sprintf("%12.2f", acctDB.Balance.toFloat());
        acctDB.Balance.stripWhiteSpace();
        balance->setText(acctDB.Balance);
        transCount->setText(acctDB.TransCount);
    }

    // Fill our combo box...
    DB.query("select AccountNo, AcctName from Accounts where AccountNo <> %d and SubAcctOf = 0 order by AcctType, AcctName", AccountNo);
    AcctNoIndex = new(int[DB.rowCount+1]);
    AcctNoIndex[0] = 0;
    indexPtr = 0;
    while (DB.getrow()) {
        indexPtr++;
        AcctNoIndex[indexPtr] = atoi(DB.curRow["AccountNo"]);
        sprintf(tmpstr, "%s", DB.curRow["AcctName"]);
        subAcctOf->insertItem(tmpstr);
        if (AcctNoIndex[indexPtr] == tmpSubOf) {
            subAcctOf->setCurrentItem(indexPtr);
        }
    }
    
    acctName->setFocus();
}


AccountsEdit::~AccountsEdit()
{
}


// The user clicked on the close button...

void AccountsEdit::cancelAccount()
{
    close();
    delete this;
}

// The user clicked on the save button...

void AccountsEdit::saveAccount()
{
    ADB         DB;
    AccountsDB  acctDB;
    
    if (AccountNo) {
        acctDB.get(AccountNo);
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
    acctDB.AcctNumber         = acctNumber->text();
    acctDB.TaxLine            = taxLine->text();
    
    if (!AccountNo) {
        // New record...
        // Determine the new account number for it.
        int done = 0;
        while (!done) {
            AccountNo++;
            DB.query("select AccountNo from Accounts where AccountNo = %d", AccountNo);
            if (!DB.rowCount) {
                done = 1;
            }
        }
        acctDB.AccountNo = AccountNo;
        acctDB.ins();
    } else {
        // Updating a record...
        acctDB.AccountNo = AccountNo;
        acctDB.upd();
    }
    
    emit refresh(1);
    close();
    delete this;
}

