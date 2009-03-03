/*
** $Id$
**
***************************************************************************
**
** GenLedger  - An interface to the General Ledger.  Manages both the
**              transaction and the individual split lines.
**
** 2000-10-11 - This GL logic is horribly broken and will be replaced RSN.
**              It does work how I understood a GL to work three years ago
**              however...
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
** $Log: GenLedger.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <qstrlist.h>

#include <GenLedger.h>
#include <ADB.h>


/**
 * GLAccountTypesDB()
 *
 * Constructor for the GLAccountTypes database class
 */
GLAccountTypesDB::GLAccountTypesDB()
{
}

/**
 * ~GLAccountTypesDB()
 *
 * Destructor.
 */
GLAccountTypesDB::~GLAccountTypesDB()
{
}

/**
 * getAccountTypeList()
 *
 * Gets the list of account types from the database and returns
 * them in a QList.
 */
GLAccountTypeList GLAccountTypesDB::getAccountTypeList()
{
    ADB     DB;
    GLAccountTypeList   myList;

    DB.query("select * from GLAccountTypes order by AccountType");
    if (DB.rowCount) {
        while (DB.getrow()) {
            GLAccountTypeRecord *rec = new GLAccountTypeRecord;
            rec->accountType = atoi(DB.curRow["AccountType"]);
            rec->description = DB.curRow["Description"];
            myList.append(rec);
        }
    }

    return myList;
}


/**
 * GLDB()
 *
 * Constructor for the General Ledger Database Class (GLDB)
 */
GLDB::GLDB(void)
{
	// from the GL table
    InternalID      = 0;
    TransID			= "";
    IntAccountNo	= "";
    Amount			= "";
    LinkedTrans		= "";
    TransType		= "";
    TransTypeLink	= "";
    TransDate		= "";
    BilledDate		= "";
    DueDate			= "";
    Cleared			= "";
    Number			= "";
    NumberStr		= "";
    ItemID			= "";
    Quantity		= "";
    Price			= "";
    Memo			= "";
    
    // from the GLIndex
    IDXTransID		= "";
    SplitCount		= "";
    BaseDesc		= "";
    
}

GLDB::~GLDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified Internal ID.
//
// Args        - int IntID - the internal ID of the record
//
// Returns     - 0 on failure, 1 on success
//

int GLDB::get(long IntID)
{
    int		Ret = 0;
    int		tmpID = 0;
    ADB	    DB;

    DB.query("select * from GL where InternalID = %ld", IntID);
    if (DB.rowCount) {
        DB.getrow();
        tmpID = atoi(DB.curRow["InternalID"]);
    };

    if (IntID == tmpID) {
    
        InternalID		= IntID;
        TransID			= DB.curRow["TransID"];
        IntAccountNo	= DB.curRow["IntAccountNo"];
        Amount			= DB.curRow["Amount"];
        LinkedTrans		= DB.curRow["LinkedTrans"];
        TransType		= DB.curRow["TransType"];
        TransTypeLink	= DB.curRow["TransTypeLink"];
        TransDate		= DB.curRow["TransDate"];
        BilledDate		= DB.curRow["BilledDate"];
        DueDate			= DB.curRow["DueDate"];
        Cleared			= DB.curRow["Cleared"];
        Number			= DB.curRow["Number"];
        NumberStr		= DB.curRow["NumberStr"];
        ItemID			= DB.curRow["ItemID"];
        Quantity		= DB.curRow["Quantity"];
        Price			= DB.curRow["Price"];
        Memo			= DB.curRow["Memo"];
        
        // And now, load the base record for it
        DB.query("select * from GLIndex where TransID = %ld", TransID.toLong());
        DB.getrow();
        
        IDXTransID		= DB.curRow["TransID"];
        SplitCount		= DB.curRow["SplitCount"];
        BaseDesc		= DB.curRow["BaseDesc"];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts ONLY an Index record into the database.
//
// Args        - None.
//
// Returns     - the new Trans ID
//

long GLDB::ins(void)
{
    long    Ret = 0;
    QString qquery;
    char    desc[4096];
	ADB	    DB;
    
    // Escape the strings
    strcpy(desc, DB.escapeString(BaseDesc));

    Ret = DB.dbcmd("insert into GLIndex values (0, 0, '%s')",
      desc
    );
    
    IDXTransID.setNum(Ret);
    
    return(Ret);
}

//
// insTrans()       - Inserts ONLY a Transaction record into the database.
//
// Args        - None.
//
// Returns     - the new InternalID
//

long GLDB::insTrans(void)
{
    long    Ret = 0;
    float	tmpAmt;
    char	tmpOp[16];
	ADB	    DB;
    ADBTable    glDB("GL");
    
    // Escape the strings

    glDB.setValue("TransID",        TransID.toLong());
    glDB.setValue("IntAccountNo",   IntAccountNo.toInt());
    glDB.setValue("Amount",         Amount.toFloat());
    glDB.setValue("LinkedTrans",    LinkedTrans.toLong());
    glDB.setValue("TransType",      TransType.toInt());
    glDB.setValue("TransTypeLink",  TransTypeLink.toLong());
    glDB.setValue("TransDate",      (const char *) TransDate);
    glDB.setValue("BilledDate",     (const char *) BilledDate);
    glDB.setValue("DueDate",        (const char *) DueDate);
    glDB.setValue("Cleared",        Cleared.toInt());
    glDB.setValue("Number",         Number.toInt());
    glDB.setValue("NumberStr",      (const char *) NumberStr);
    glDB.setValue("ItemID",         ItemID.toLong());
    glDB.setValue("Quantity",       Quantity.toFloat());
    glDB.setValue("Price",          Price.toFloat());
    glDB.setValue("Memo",           (const char *) Memo);

    Ret = glDB.ins();
    TransID.setNum(Ret);
    
    tmpAmt = Amount.toFloat();
    if (tmpAmt < 0.00) {
    	strcpy(tmpOp, "-");
    	tmpAmt = tmpAmt * -1;
    } else {
    	strcpy(tmpOp, "+");
    }
   	DB.dbcmd("update Accounts set Balance = Balance %s %12.2f, TransCount = TransCount + 1 where IntAccountNo = %d", tmpOp, tmpAmt, IntAccountNo.toInt());

    return(Ret);
}

//
// upd()       - Updates a GLIndex record ONLY in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int GLDB::upd(void)
{
    int Ret = 0;

    ADBTable    glDB("GLIndex");

    glDB.get(TransID.toLong());
    glDB.setValue("SplitCount", SplitCount.toInt());
    glDB.setValue("BaseDesc",   (const char *) BaseDesc);
    glDB.upd();
    
    return(Ret);
}

//
// del()       - Deletes a GLIndex record and all of its associated GL records.
//
// Args        - The TransID to delete.
//
// Returns     - -1 on failure, 0 on success
//

int GLDB::del(long TransID)
{
    int     Ret = 0;
    long    *intIDIdx;
    
    int	    tmpAccountNo;
    float   tmpAmount;
	ADB     DB;
    
    // We need to add some checks in here to make sure that this transaction
    // isn't required by some other transaction.
    
    DB.dbcmd("delete from GLIndex where TransID = %ld", TransID);
    
    // We now need to load all of the splits and adjust the balances of the
    // Accounts that they refer to.
    DB.query("select InternalID from GL where TransID = %ld", TransID);
    if (DB.rowCount) {
    	intIDIdx = new(long[DB.rowCount + 1]);
    	int tmpCount = DB.rowCount;
    	for (int i = 0; i < tmpCount; i++) {
    		DB.getrow();
    		intIDIdx[i] = atol(DB.curRow["InternalID"]);
    	}
    	
    	// Now that we have the individual row counts, update the accounts.
    	for (int i = 0; i < tmpCount; i++) {
    	    // Load the record from the GL to get the amounts
	    	DB.query("select * from GL where InternalID = %ld", intIDIdx[i]);
	    	DB.getrow();
	    	tmpAccountNo = atoi(DB.curRow["IntAccountNo"]);
	    	tmpAmount = atof(DB.curRow["Amount"]);
	    	tmpAmount = tmpAmount * -1.0;
	    	
	    	// Now, update the account it refers to.
		    DB.dbcmd("update Accounts set TransCount = TransCount - 1, Balance = Balance + %f where IntAccountNo = %d", tmpAmount, tmpAccountNo);
		    
		    // Now that we've updated the balance, delete the split record
		    DB.dbcmd("delete from GL where InternalID = %ld", intIDIdx[i]);
    	}
    	
    	// Free up our memory...
    	delete(intIDIdx);
    }
    
    
    return(Ret);
}



GenLedger::GenLedger(void)
{
	// Our pointer variables
	TransID			= 0;			// Meaning we don't have one loaded.
	SplitCount		= 0;			// No splits are loaded, either.
	BaseDesc		= "";			// The base description is blank.
	
	// References for the caller
	CurSplitNo		= 0;
	CurrentSplit	= NULL;			// Initialize the interface split entry.
	
	// We need to make sure that if we get rid of a list item, that its memory
	// also gets cleaned up.
	Splits.setAutoDelete(TRUE);
	
}

GenLedger::~GenLedger()
{
	// Free up some of our other memory.
    // delete CurrentSplit;
    while (Splits.count()) Splits.removeLast();
}

/*
** GetTrans     - Loads the specified transaction and its splits into memory.
**                Note:  It doesn't care if it has already loaded another
**                       transaction.  It will happily stomp all over itself.
*/

int GenLedger::GetTrans(long TransNo)
{
	int			Ret = 0;
	GLDB		*tmpGLDB;
	QStrList	splitList(TRUE);
	QString		tmpQStr;
	ADB         DB;
	
	DB.query("select * from GLIndex where TransID = %ld", TransNo);
	if (DB.rowCount == 1) {
		DB.getrow();
		TransID = atoi(DB.curRow["TransID"]);
		SplitCount	= atoi(DB.curRow["SplitCount"]);
		BaseDesc = DB.curRow["BaseDesc"];
		
		// Since we can't yet nest queries into the database, 
		// load the internal ID's for all of the splits into a qstring 
		// array so we can loop through them out of memory.
		DB.query("select InternalID from GL where TransID = %ld", TransNo);
		while (DB.getrow()) {
			splitList.append(DB.curRow["InternalID"]);
		}

		// Now, load the individual splits into memory.
		for (uint i = 0; i < splitList.count()-1; i++) {
			tmpGLDB = new GLDB;
			tmpQStr = splitList.at(i);
			tmpGLDB->get(tmpQStr.toInt());
			Splits.append(tmpGLDB);
		}

		// Point the current split at the first one.
		CurrentSplit = Splits.at(0);
		
		Ret = 1;
	}  // else, the transaction wasn't found.  Return our error...

    while (splitList.count()) splitList.removeLast();
    
	return(Ret);
}

/*
** SaveTrans     - Saves the current transaction and its splits back into the
**                 databse.  If TransID == 0, it will insert a new 
**                 transaction, else it will update the old transaction.
**                 Returns the transaction ID.
*/

long GenLedger::SaveTrans(void)
{
	char		query[1024];
	int			updating = 0;
	QStrList	tmpList(TRUE);
	QString		tmpQStr;
	int			tmpAcctNo;
	float		tmpAmount;
	QString		tmpOperator;
	int			tmpCur;
	ADB         DB;

	// insert or update the changes to the Index.
	if (!TransID) {
		// Since we're inserting, we need the new TransID.
		TransID = DB.dbcmd("insert into GLIndex values (0, %d, '%s')",
		  SplitCount,
		  (const char *) DB.escapeString(BaseDesc)
		);
	} else {
		updating = 1;
		DB.dbcmd("update GLIndex set SplitCount = %d, BaseDesc = '%s' where TransID = %ld",
		  SplitCount,
		  (const char *) DB.escapeString(BaseDesc),
		  TransID
		);
	}
	
	// If we're updating a transaction, we need to first delete all
	// of the splits for the transaction first.  Before we can do that,
	// however, we need to adjust any balances of accounts they reference.
	// First things first, if we are updating, load the old internal list
	// so we can delete them one by one and adjust their referenced accounts.
	if (updating) {
		// Get the list of internal ID's
		DB.query("select IntAccountNo, Amount from GL where TransID = %ld", TransID);
		while (DB.getrow()) {
			tmpAcctNo = atoi(DB.curRow["IntAccountNo"]);
			tmpAmount = atof(DB.curRow["Amount"]);
			if (tmpAmount < 0.00) {
				tmpOperator = "+";
				tmpAmount   = tmpAmount * -1.00;
			} else {
				tmpOperator = "-";
			}
			sprintf(query, "update Accounts set Balance = Balance %s %f, TransCount = TransCount - 1 where IntAccountNo = %d",
			  (const char *) tmpOperator,
			  tmpAmount,
			  tmpAcctNo
			);
			tmpList.append(query);
		}

		// Go through them each, one by one adjust the account balance
		for (tmpQStr = tmpList.first(); tmpQStr != 0; tmpQStr = tmpList.next()) {
			// tmpQStr = tmpList.at(i);
			DB.dbcmd("%s", (const char *) tmpQStr);
		}
		
		// Delete the splits.
		DB.dbcmd("delete from GL where TransID = %ld", TransID);
	}
	
	// Now, insert the individual split lines into the database.
	tmpCur = CurSplitNo;
	
	for (CurrentSplit = Splits.first(); CurrentSplit != 0; CurrentSplit = Splits.next()) {
		CurrentSplit->TransID.setNum(TransID);
		CurrentSplit->insTrans();
	}

	GetSplit(tmpCur);

	return(1);
}

/*
** TransCheck     - Checks that a transaction is okay and in balance.
**                  Returns 1 if it is, 0 if it isn't.
*/

int GenLedger::TransCheck(void)
{
	return(1);
}

/*
** GetSplit        - This sets the CurrentSplit variable to the specified
**                   SplitNo.  The data should have been already loaded in
**                   from the database, so its not necessary to do another
**                   query.  Returns 1 on success.
*/

int GenLedger::GetSplit(int SplitNo)
{
	int	Ret = 0;
	if (SplitNo <= SplitCount) {
		CurSplitNo = SplitNo;
		CurrentSplit = Splits.at(CurSplitNo - 1);
		Ret = 1;
	}
	return(Ret);
}

/*
** AddSplit        - Adds a split entry into the internal list of splits
**                   and sets the CurrentSplit to the new split item.
**                   It also sets up some of the internal variables in the
**                   split line so that they will save correctly.  Returns
**                   1 if the split was sucessfully added.
*/

int GenLedger::AddSplit(void)
{
    #ifdef DBDEBUG
	fprintf(stdout, "Adding split...\n");
	#endif
	Splits.append(new GLDB);
	CurrentSplit = Splits.last();
	SplitCount++;
	CurSplitNo = Splits.count();
	#ifdef DBDEBUG
	fprintf(stdout, "Sucessfully added split.\n");
	#endif
	return(1);
}

/*
** DelSplit       - Deletes the specified split item from the list.  If it
**                  is the current SplitItem, then the current split will
**                  be set to the first split item.  Returns 1 on success.
*/

int GenLedger::DelSplit(int SplitNo)
{
	int Ret = 0;
    if (SplitCount) {
		Splits.remove(SplitNo);
		CurrentSplit = Splits.current();
		SplitCount--;
		Ret = 1;
		if (CurSplitNo > Splits.count()) {
			CurSplitNo = Splits.count();
		}
	}
	return(Ret);
}




