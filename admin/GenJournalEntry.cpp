/*
** $Id$
**
***************************************************************************
**
** GenJournalEntry - Lets the user make a General Journal entry
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
** $Log: GenJournalEntry.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "GenJournalEntry.h"
#include "BlargDB.h"
#include "GenLedger.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>

#define Inherited GenJournalEntryData

GenJournalEntry::GenJournalEntry
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	ADB     DB;
	
	setCaption( "General Journal Entry" );
	entryDate->setFocus();
	
	// We need to fill each of the account lists with the account
	// that will be used.
	
	int YPos = 95;
	for (int i = 0; i < GJELINES; i++) {
		accounts[i] = new QComboBox(FALSE, this, "");
		accounts[i]->setGeometry(10, YPos, 130, 20);
		accounts[i]->setSizeLimit(10);
		accounts[i]->setAutoResize(FALSE);
		accounts[i]->insertItem("N/A");
		
		credits[i] = new QLineEdit(this, "");
		credits[i]->setGeometry(140, YPos, 100, 20);
		credits[i]->setText("");
		credits[i]->setMaxLength(14);
		credits[i]->setEchoMode(QLineEdit::Normal);
		credits[i]->setFrame(TRUE);
		
		debits[i] = new QLineEdit(this, "");
		debits[i]->setGeometry(240, YPos, 100, 20);
		debits[i]->setText("");
		debits[i]->setMaxLength(14);
		debits[i]->setEchoMode(QLineEdit::Normal);
		debits[i]->setFrame(TRUE);
		
		memos[i] = new QLineEdit(this, "");
		memos[i]->setGeometry(340, YPos, 255, 20);
		memos[i]->setText("");
		memos[i]->setMaxLength(200);
		memos[i]->setEchoMode(QLineEdit::Normal);
		memos[i]->setFrame(TRUE);

		YPos += 20;
	}
	
	DB.query("select AccountNo, AcctName from Accounts order by AcctType, AcctName");
	acctNoIndex = new(int[DB.rowCount + 1]);
	int indexPtr = 0;
	acctNoIndex[indexPtr++] = 0;
	while(DB.getrow()) {
		acctNoIndex[indexPtr++] = atoi(DB.curRow["AccountNo"]);
		for (int i = 0; i < GJELINES; i++) {
			accounts[i]->insertItem(DB.curRow["AcctName"]);
		}
	}
	// Plug today's date into the form.
	time_t	ti = time(NULL);
	struct tm * tm = localtime(&ti);
			
	char myDate[20];
	sprintf(myDate, "%04d-%02d-%02d", tm->tm_year+1900,
	  tm->tm_mon+1, tm->tm_mday);
	entryDate->setText(myDate);

}


GenJournalEntry::~GenJournalEntry()
{
}

/*
** validateEntry - Makes sure that the totals balance, that the accounts
**					are different, and that valid acconts are present
**					for each line.
**
**		Returns - 	0 for failure, 1 for success (valid entry)
**
*/

int GenJournalEntry::validateEntry()
{
	int		Ret = 1;		// We'll start by assuming its valid.
	
	
	
	return Ret;
}

/*
** saveEntry - Saves the General Journal Entry and closes the window.
*/

void GenJournalEntry::saveEntry()
{
	float	creditAmt;
	float	debitAmt;
	float	tmpAmt;
	char	tmpDate[64];
	char	memo[1024];
	
	if (validateEntry()) {
		// Get the transaction ID and inser the base description
		GenLedger glDB;
		glDB.BaseDesc = description->text();
		
		// TransID = glDB.ins();
		// fprintf(stderr, "TransID = %ld\n", TransID);

    	strcpy(tmpDate, entryDate->text());
		
		// Go through each line and get the data from it.
	    for (int i = 0; i < GJELINES; i++) {
	    	creditAmt = atof(credits[i]->text());
	    	debitAmt  = atof(debits[i]->text());
	    	strcpy(memo, memos[i]->text());
	    	if (creditAmt != 0.0 && debitAmt != 0.0) {
	    		// Bogus.  We can only have a credit or debit, not both.
	    	} else if (creditAmt == 0.0 && debitAmt == 0.0) {
	    		// Bogus.  No amount specified
	    	} else {
	    		// Valid.  Check the account.
	    		if (acctNoIndex[accounts[i]->currentItem()]) {
			    	glDB.AddSplit();
	    			if (creditAmt != 0.0) {
	    				tmpAmt = creditAmt;
	    			} else {
	    				tmpAmt = debitAmt * -1;
	    			}
	    			// Should be good.
	    			glDB.CurrentSplit->TransDate = tmpDate;
	    			glDB.CurrentSplit->AccountNo.setNum(acctNoIndex[accounts[i]->currentItem()]);
	    			glDB.CurrentSplit->Amount.setNum(tmpAmt);
	    			glDB.CurrentSplit->Memo = memos[i]->text();
	    			//glDB.insTrans();
	    		}
	    	}
	    }
	    
	    glDB.SaveTrans();
		close();
//		delete this;
	}

}


/*
** cancelEntry - Close the window and cancel the entry.
*/

void GenJournalEntry::cancelEntry()
{
	close();
	delete this;
}
