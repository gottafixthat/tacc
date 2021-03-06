/*
** AcctsRecv.h    - A unified, objectified interface into the general
**                  ledger.  Manages both the transaction index as well
**                  as individual split lines.
*/

#include <stdio.h>
#include <stdlib.h>

#include <Qt3Support/q3strlist.h>

#include <Cfg.h>
#include <ADB.h>
#include <TAATools.h>
#include "AcctsRecv.h"


// Okay, first off, we need to define the information we need to keep
// track of and create two lists for them.
struct AREntry {
    long IntID;
    float Amount;
    float ClearedAmount;
};

/**
 * getCustomerAgingData()
 *
 * Utility function that returns the AR Aging data
 * for the specified customer ID.
 *
 * currentBalance = The customer's current balance, including all charges and payments
 * totalOverdue = What portion of the currentBalance is overdue
 * currentDue = What portion of the currentBalance is not overdue
 * overdue = Amount overdue between 0 and 30 days
 * overdue30 = Amunt overdue between 31 and 60 days
 * overdue60 = Amount overdue between 61 and 90 days
 * overdue90 = Amount overdue 91 days or greater.
 */
const customerARAgingRecord getCustomerAgingData(long custID)
{
    customerARAgingRecord   retVal;
    ADB                     DB;

    // Set our return values
    retVal.customerID = 0;
    retVal.currentBalance = 0.00;
    retVal.totalOverdue = 0.00;
    retVal.currentDue = 0.00;
    retVal.overdue = 0.00;
    retVal.overdue30 = 0.00;
    retVal.overdue60 = 0.00;
    retVal.overdue90 = 0.00;

    DB.query("select CurrentBalance from Customers where CustomerID = %ld", custID);
    if (!DB.rowCount) return retVal;
    DB.getrow();
    retVal.customerID = custID;
    retVal.currentBalance = atof(DB.curRow["CurrentBalance"]);
    if (retVal.currentBalance == 0.00) return retVal;

    // If we've made it here, we have a customer with a balance
    QDate   today = QDate::currentDate();
    QDate   dueDate = QDate::currentDate();
    QString dueDateSt;

    DB.query("select Amount, ClearedAmount, DueDate from AcctsRecv where CustomerID = %ld and DueDate <> '0000-00-00' and ClearedAmount <> Amount", custID);
    if (DB.rowCount) while (DB.getrow()) {
        double  amount  = 0.00;
        double  cleared = 0.00;
        double  diff    = 0.00;
        int     days    = 0;

        dueDateSt = DB.curRow["DueDate"];
        myDateToQDate(dueDateSt.ascii(), dueDate);
        amount  = atof(DB.curRow["Amount"]);
        cleared = atof(DB.curRow["ClearedAmount"]);
        diff = amount - cleared;
        days = dueDate.daysTo(today);

        if (days > 90) {
            retVal.overdue90 += diff;
            retVal.totalOverdue += diff;
        } else if (days > 60) {
            retVal.overdue60 += diff;
            retVal.totalOverdue += diff;
        } else if (days > 30) {
            retVal.overdue30 += diff;
            retVal.totalOverdue += diff;
        } else if (days >= 1) {
            retVal.overdue += diff;
            retVal.totalOverdue += diff;
        } else {
            // else its not overdue yet, don't necessarily add them
            retVal.currentDue += diff;
        }
    }


    return retVal;

}



AcctsRecv::AcctsRecv(void)
{
	
	// Our pointer variables
	InternalID		= 0;			// Meaning we don't have one loaded.
	
	GL	 = new GenLedger;
	ARDB = new AcctsRecvDB;
	myIntAcctNo = 0;
}

AcctsRecv::~AcctsRecv()
{
	// Free up some of our other memory.
	// delete CurrentSplit;
	delete(GL);
	delete(ARDB);
}

/**
 * setGLAccount()
 *
 * Overrides the GL Account that we post to - not the AcctsRecv side
 * but the billable side of the transaction.
 *
 * Returns 1 if successful, 0 if not.
 */
int AcctsRecv::setGLAccount(const char *acctNo)
{
    int     retVal = 0;
    ADB     DB;
    DB.query("select IntAccountNo from Accounts where AccountNo = '%s'", acctNo);
    if (DB.rowCount) { 
        DB.getrow();
        myIntAcctNo = atoi(DB.curRow["IntAccountNo"]);
        retVal = 1;
    }
    return retVal;
}

/**
 * setGLAccount()
 *
 * Overrides the GL Account that we post to - not the AcctsRecv side
 * but the billable side of the transaction.
 *
 * Returns 1 if successful, 0 if not.
 */
int AcctsRecv::setGLAccount(int intAcctNo)
{
    int     retVal = 0;
    ADB     DB;
    DB.query("select IntAccountNo from Accounts where IntAccountNo = '%d'", intAcctNo);
    if (DB.rowCount) { 
        myIntAcctNo = intAcctNo;
        retVal = 1;
    }
    return retVal;
}

/**
** GetTrans     - Loads the specified transaction and its splits into memory.
**                Note:  It doesn't care if it has already loaded another
**                       transaction.  It will happily stomp all over itself.
*/

int AcctsRecv::GetTrans(long IntID)
{
	return IntID;
}

/*
** SaveTrans     - Saves the current transaction and its splits back into the
**                 databse.  If TransID == 0, it will insert a new 
**                 transaction, else it will update the old transaction.
**                 Returns the transaction ID.
*/

long AcctsRecv::SaveTrans(void)
{
	ADB		    DB;
	BillablesDB	BDB;
	// CustomersDB CDB;
	int			ARAcct = 0;
	
	int         SrcAcct, DstAcct = 0;
	
	// We need to get the AR account number
	ARAcct = atoi(cfgVal("AcctsRecvAccount"));
	
	// Now, load the Billable Item so we can have its account number.
	BDB.get(ARDB->getLong("ItemID"));
	
	// CDB.get(ARDB->CustomerID.toInt());
	// CDB.CurrentBalance.setNum(CDB.CurrentBalance.toFloat() + ARDB->Amount.toFloat());
	// CDB.upd();

	if (InternalID) {
	} else {
		// Its a new transaction.
		InternalID = ARDB->ins();
		GL->BaseDesc = ARDB->getStr("Memo");
		
		switch(ARDB->getInt("TransType")) {
		    case TRANSTYPE_PAYMENT:
		        DstAcct = atoi(cfgVal("UndepositedFundsAcct"));
		        SrcAcct = ARAcct;
		        break;
		        
		    case TRANSTYPE_CCPAYMENT:
		        DstAcct = atoi(cfgVal("UndepositedCCAcct"));
		        SrcAcct = ARAcct;
		        break;
		        
		    default:
		        SrcAcct = ARAcct;
                if (myIntAcctNo) DstAcct = myIntAcctNo;
                else DstAcct = BDB.getInt("IntAccountNo");
		        break;
		}
		
		// First, put in the AR account split
		GL->AddSplit();
		GL->CurrentSplit->IntAccountNo.setNum(SrcAcct);
		GL->CurrentSplit->Amount.setNum(ARDB->getFloat("Amount"));
		GL->CurrentSplit->TransType.setNum(ARDB->getInt("TransType"));
		GL->CurrentSplit->TransTypeLink.setNum(InternalID);
		GL->CurrentSplit->TransDate = ARDB->getStr("TransDate");
		GL->CurrentSplit->BilledDate = ARDB->getStr("BilledDate");
		GL->CurrentSplit->DueDate = ARDB->getStr("DueDate");
		GL->CurrentSplit->Cleared.setNum(ARDB->getInt("Cleared"));
		GL->CurrentSplit->Number.setNum(atol(ARDB->getStr("RefNo")));
		GL->CurrentSplit->NumberStr = ARDB->getStr("RefNo");
		GL->CurrentSplit->ItemID.setNum(ARDB->getLong("ItemID"));
		GL->CurrentSplit->Quantity.setNum(ARDB->getFloat("Quantity"));
		GL->CurrentSplit->Price.setNum(ARDB->getFloat("Price"));
		GL->CurrentSplit->Memo = ARDB->getStr("Memo");
		
		// Now, put in the sales split.
		GL->AddSplit();
		GL->CurrentSplit->IntAccountNo.setNum(DstAcct);
		GL->CurrentSplit->Amount.setNum(ARDB->getFloat("Amount") * -1.0);
		GL->CurrentSplit->TransType.setNum(ARDB->getInt("TransType"));
		GL->CurrentSplit->TransTypeLink.setNum(InternalID);
		GL->CurrentSplit->TransDate = ARDB->getStr("TransDate");
		GL->CurrentSplit->BilledDate = ARDB->getStr("BilledDate");
		GL->CurrentSplit->DueDate = ARDB->getStr("DueDate");
		GL->CurrentSplit->Cleared.setNum(ARDB->getInt("Cleared"));
		GL->CurrentSplit->Number.setNum(atol(ARDB->getStr("RefNo")));
		GL->CurrentSplit->NumberStr = ARDB->getStr("RefNo");;
		GL->CurrentSplit->ItemID.setNum(ARDB->getLong("ItemID"));
		GL->CurrentSplit->Quantity.setNum(ARDB->getFloat("Quantity"));
		GL->CurrentSplit->Price.setNum(ARDB->getFloat("Price"));
		GL->CurrentSplit->Memo = ARDB->getStr("Memo");
		
		GL->SaveTrans();
		#ifdef DBDEBUG
		//fprintf(stderr, "GL Trans ID = %ld\n", GL->TransID);
		#endif
		ARDB->setValue("GLTransID", GL->TransID);
		ARDB->upd();
	}
	
	applyCredits(ARDB->getLong("CustomerID"));
	
	return(InternalID);
}




/*
** applyCredits  - A function that, given a Customer ID, will scan through
**                 their AcctsRecv entries and apply credits to charges,
**                 marking any fully applied payments/credits as cleared.
**
**                 This function should be called whenever a transaction
**                 is added, deleted, or the amount is otherwise altered.
*/

void applyCredits(long CustomerID)
{
    ADB         DB1;
    ADB         DB2;
    float       tmpFloat;

    // Now, just for good measure, update the customer's balance...
    CustomersDB     CDB;
    CDB.get(CustomerID);
    tmpFloat = DB1.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld", CustomerID);
    CDB.setValue("CurrentBalance", tmpFloat);
    CDB.upd();

    QList<AREntry *> Charges;
    QList<AREntry *> Credits;
    
    AREntry *curEntry;
    
    float   RunningTotal = 0.00;
    
    //Charges.setAutoDelete(TRUE);
    //Credits.setAutoDelete(TRUE);
    
    // Now, fill the lists...
    // First, do the queries.  If there are no credits and no charges,
    // we can safely exit.
    DB1.query("select InternalID, Amount, ClearedAmount from AcctsRecv where CustomerID = %ld and Amount > 0.00 and Cleared = 0 order by TransDate, InternalID", CustomerID);

    DB2.query("select InternalID, Amount, ClearedAmount from AcctsRecv where CustomerID = %ld and Amount < 0.00 and Cleared = 0 order by TransDate, InternalID", CustomerID);

    if (!DB1.rowCount || !DB2.rowCount) {
        // There were no charges, we can exit now.
        return;
    }
    
    // If we made it this far, there are charges and credits that need to
    // be brought together.
    while (DB1.getrow()) {
        curEntry = new(AREntry);
        curEntry->IntID = atol(DB1.curRow["InternalID"]);
        curEntry->Amount = atof(DB1.curRow["Amount"]);
        curEntry->ClearedAmount = atof(DB1.curRow["ClearedAmount"]);
        Charges.append(curEntry);
        RunningTotal += curEntry->Amount;
    }
    
    #ifdef DBDEBUG
    fprintf(stderr, "applyCredits: Loaded $%0.2f worth of charges.\n", RunningTotal);
    #endif


    // Now, get all of the credits.
    RunningTotal = 0.00;
    while (DB2.getrow()) {
        curEntry = new(AREntry);
        curEntry->IntID = atol(DB2.curRow["InternalID"]);
        curEntry->Amount = atof(DB2.curRow["Amount"]);
        curEntry->ClearedAmount = atof(DB2.curRow["ClearedAmount"]);
        Credits.append(curEntry);
        RunningTotal += curEntry->Amount;
    }
    
    #ifdef DBDEBUG
    fprintf(stderr, "applyCredits: Loaded $%0.2f worth of credits.\n", RunningTotal);
    #endif

    // Now, since there will almost always be more charges than credits, 
    // we want our outside loop to be the Charges.
    int     looping  = 1;
    float   chgamt;
    float   cdtamt;
    AREntry *curCharge;
    AREntry *curCredit;
    while(looping) {
        curCharge = Charges.at(0);
        curCredit = Credits.at(0);
        chgamt = curCharge->Amount - curCharge->ClearedAmount;
        cdtamt = curCredit->Amount - curCredit->ClearedAmount;
        // The most frequently "hit" combination is going to be one for one
        // since most of our accounts are monthly, and have only one charge
        // per month, and one payment, so lets cover that scenario first.
        if ((chgamt + cdtamt) == 0.00) {
            // Update the database for these two items and remove them
            // from our lists.
            DB1.dbcmd("update AcctsRecv set ClearedAmount = %f, Cleared = 1 where InternalID = %ld", curCharge->Amount, curCharge->IntID);
            Charges.removeFirst();
            if (!Charges.count()) looping = 0;

            DB1.dbcmd("update AcctsRecv set ClearedAmount = %f, Cleared = 1 where InternalID = %ld", curCredit->Amount, curCredit->IntID);
            Credits.removeFirst();
            if (!Credits.count()) looping = 0;
        } else {
            // The second most frequently hit is when the customer pays
            // more than the charge, so lets check for that next.  This will
            // most frequently happen when there are multiple logins, but
            // only one check.
            if (chgamt + cdtamt < 0.00) {
                // Update the database for these two items, and remove the
                // charge from our list.
                DB1.dbcmd("update AcctsRecv set ClearedAmount = %f, Cleared = 1 where InternalID = %ld", curCharge->Amount, curCharge->IntID);
                Charges.removeFirst();
                if (!Charges.count()) looping = 0;
                
                curCredit->ClearedAmount -= chgamt;
                DB1.dbcmd("update AcctsRecv set ClearedAmount = %f where InternalID = %ld", curCredit->ClearedAmount, curCredit->IntID);
            } else {
                // The only thing left is for our charge to be greater than
                // our payment, which also happens on a semi-regular basis.
                DB1.dbcmd("update AcctsRecv set ClearedAmount = %f, Cleared = 1 where InternalID = %ld", curCredit->Amount, curCredit->IntID);
                Credits.removeFirst();
                if (!Credits.count()) looping = 0;
                
                curCharge->ClearedAmount += (cdtamt * -1);
                DB1.dbcmd("update AcctsRecv set ClearedAmount = %f where InternalID = %ld", curCharge->ClearedAmount, curCharge->IntID);
            }
        }
    }

    // Free up our memory
    while (Charges.count()) Charges.removeLast();
    while (Credits.count()) Credits.removeLast();
}



