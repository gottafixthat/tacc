/*
** $Id$
**
***************************************************************************
**
** RunStatements - Does a statement run.
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
** $Log: RunStatements.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include <stdio.h>
#include <stdlib.h>

#include "RunStatements.h"
#include <unistd.h>
#include <qdatetm.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qprogressdialog.h>
#include "BlargDB.h"
#include "BString.h"
#include "ParseFile.h"
#include <ADB.h>
#include "Cfg.h"
#include "TAATools.h"

/*
** RunStatements - Runs statements for all customers that need them.
*/

void RunStatements(void)
{
    ADB         DB;
    CustomersDB CDB;
    long        totItems;
    long        curItem = 0;
    
    long        StNo = 0;

    //QApplication::setOverrideCursor(Qt::WaitCursor);
    
    DB.query("select CustomerID from Customers where CurrentBalance <> 0.00 order by CustomerID");
    
    totItems    = DB.rowCount;
    
    if (totItems) {
        QProgressDialog progress("Creating Statements...", "Abort", totItems, 0, "Creating Statements...");
        progress.setCancelButtonText("Abort");
        while (DB.getrow()) {
            progress.setProgress(curItem++);
            if (progress.wasCancelled()) break;
            
            CDB.get(atol(DB.curRow["CustomerID"]));
            progress.setLabelText(CDB.getStr("FullName"));
            StNo = doStatement(atol(DB.curRow["CustomerID"]));
            if (StNo) {
                doAutoPayments(atol(DB.curRow["CustomerID"]));
            
                emailStatement(StNo, 0, 0);
                //printStatement(StNo, 0, 0);
            }
        }
        
        progress.setProgress(totItems);
    }
    
    //QApplication::restoreOverrideCursor();
}


/*
** doStatement - Takes a customer ID, loads them, and creates a statement
**               for them, if necessary.
**
**     Returns - 1 on error, 0 on success.
*/

int doStatement(long CustID)
{
	int				RetVal = 0;
	ADB			    DB;
	ADB			    DB2;
	char			tmpstr[1024];
	QString			tmpqstr;
	CustomersDB		CDB;
	BillingCyclesDB BCDB;
	PaymentTermsDB  PTDB;


	QDate			CycleStart;		// The day that their billing cycle starts
	QDate			CycleEnd;		// The day that their billing cycle ends
	QDate			CurrentDate;    // The current date.
	QDate			prevCycleStart;		// The day that their billing cycle starts
	QDate			prevCycleEnd;		// The day that their billing cycle ends

	QDate           qToday;
	QDate           qYesterday;
	QDate			DueDate;		// The due date.
	QDate           GraceDate;      // The Grace Date...
	char			sDueDate[64];	// Also the due date.
	char            sGraceDate[64]; // Also the Grace date
	char			chargesThrough[64]; // ChargesThrough date.
	char            Today[64];
	char            Yesterday[64];
	char            oldEndStr[64];
	int				CycleDays;      // The number of days in the billing cycle for pro-rating
	int             prevCycleDays;
	int				DaysLeft;		// The remaining days in this cycle.
	int             prevDaysLeft;
	int				NeedsStatement = 0;
	QStrList		addrlist;
	
	QDate           tmpDate;
	
	float           Credits     = 0.0;
	float           NewCharges  = 0.0;
	
	qToday = QDate::currentDate();
	QDatetomyDate(Today, qToday);
	
	qYesterday = qToday.addDays(-1);
	QDatetomyDate(Yesterday, qYesterday);
	
	// First off, lets load some preliminary information about this user.
	// Things such as their billing cycle, etc.
	// Then check to see if they require a statement.
	CDB.get(CustID);
	BCDB.get(CDB.getLong("BillingCycle"));
	
	// Find out the start and stop dates of the customers billing cycle.
	BCDB.getCycleDates(&CycleStart, &CycleEnd, &CycleDays, &DaysLeft);
	CurrentDate = QDate::currentDate();
	
	// Get the start date of the previous cycle.
	tmpDate = CycleStart.addDays(-1);
	QDatetomyDate(oldEndStr, tmpDate);
	BCDB.getCycleDates(&prevCycleStart, &prevCycleEnd, &prevCycleDays, &prevDaysLeft, oldEndStr);
	
	// Load their payment terms, and set up the due date.
	PTDB.get(CDB.getInt("Terms"));
	DueDate = CurrentDate.addDays(PTDB.getInt("DueDays"));
	sprintf(sDueDate, "%04d-%02d-%02d", DueDate.year(), DueDate.month(), DueDate.day());
	GraceDate = DueDate.addDays(PTDB.getInt("GraceDays"));
	sprintf(sGraceDate, "%04d-%02d-%02d", GraceDate.year(), GraceDate.month(), GraceDate.day());

	// We need to check to see if the customer has any unbilled
	// transactions.  If they do, then they need a statement.  If they don't,
	// we can skip them.
	DB.query("select InternalID from AcctsRecv where CustomerID = %ld and StatementNo = 0", 
	  CDB.getLong("CustomerID")
	);
	if (DB.rowCount || (CDB.getFloat("CurrentBalance") > 0.00)) {
	    // One last check.  If the've had a statement today, skip them.
	    DB.query("select StatementNo from Statements where CustomerID = %ld and StatementDate = '%s'", CustID, Today);
	    if (!DB.rowCount) NeedsStatement = 1;
	}
	
	// Now, check to see if they have active logins.
	if (NeedsStatement) {
	    if (CDB.getFloat("CurrentBalance") <= 0.00) {
	        // They have a credit balance or a zero balance
	        // so check to see if they have any active logins
	        // if they don't, then don't send them a statement
	        DB.query("select InternalID from Logins where CustomerID = %ld and Active <> 0", CDB.getLong("CustomerID"));
	        if (!DB.rowCount) NeedsStatement = 0;
	    }
	}

	// If we actually need a statement, start processing it.  If not, return.
	if (NeedsStatement) {
		AddressesDB		addrDB;
		SubscriptionsDB	SDB;
		PackagesDB		PDB;
		BillablesDB		BDB;
		BillablesDataDB	BDDB;
		RatePlansDB		RPDB;
		StatementsDB	STDB;
		StatementsDataDB SDDB;
		ADBTable        arDB;
		ADBTable        lnDB;

		arDB.setTableName("AcctsRecv");
		lnDB.setTableName("Logins");

		// Setup the initial information in the statement record.
		STDB.setValue("CustomerID", CDB.getLong("CustomerID"));
		STDB.setValue("CustName", CDB.getStr("FullName"));
		STDB.setValue("CreatedOn", Today);
		sprintf(tmpstr, "%04d-%02d-%02d", CycleStart.year(), CycleStart.month(), CycleStart.day());
		STDB.setValue("StatementDate", Today);
		STDB.setValue("Terms", PTDB.getStr("TermsDesc"));
		STDB.setValue("DueDate", sDueDate);
		
		// Get their address record.
		if (!strlen((const char *)CDB.getStr("BillingAddress"))) {
			printf("\nError!  Customer ID %ld (%s) has no billing address defined.\n", CDB.getLong("CustomerID"), (const char *) CDB.getStr("FullName"));
			return(1);
		} else {
			// Load the billing address.
			strcpy(tmpstr, CDB.getStr("BillingAddress"));
			addrDB.get(REF_CUSTOMER, CDB.getLong("CustomerID"), tmpstr);
			tmpqstr = "";
			strcpy(tmpstr, "");
			if (strlen((const char *)CDB.getStr("ContactName"))) addrlist.append(CDB.getStr("ContactName"));
			if (addrDB.Address1.length()) addrlist.append(addrDB.Address1);
			if (addrDB.Address2.length()) addrlist.append(addrDB.Address2);
			if (addrDB.International.toInt()) {
				if (addrDB.City.length()) {
				    tmpqstr.append(addrDB.City);
				    tmpqstr.append("  ");
				}
				if (addrDB.PostalCode.length()) tmpqstr.append(addrDB.PostalCode);
				if (tmpqstr.length()) addrlist.append(tmpqstr);
				if (addrDB.Country.length()) addrlist.append(addrDB.Country);
			} else {
				tmpqstr.append(addrDB.City);
				tmpqstr.append(", ");
				tmpqstr.append(addrDB.State);
				tmpqstr.append(" ");
				tmpqstr.append(addrDB.ZIP);
				addrlist.append(tmpqstr);
			}
			
			if (addrlist.count() >= 1) STDB.setValue("CustAddr1", addrlist.at(0));
			if (addrlist.count() >= 2) STDB.setValue("CustAddr2", addrlist.at(1));
			if (addrlist.count() >= 3) STDB.setValue("CustAddr3", addrlist.at(2));
			if (addrlist.count() >= 4) STDB.setValue("CustAddr4", addrlist.at(3));
		}
		
		// Make sure that the customer balance and the AcctsRecv table
		// agree with each other.
        float   tmpFloat;
		tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld", CDB.getLong("CustomerID"));
		if (tmpFloat != 0.00) {
			// The amount we just got is the balance for the customer.
			// Check to make sure that their balance matches the amount we
			// just got.  If it doesn't, update the balance.
			if (CDB.getFloat("CurrentBalance") != tmpFloat) {
				CDB.setValue("CurrentBalance", tmpFloat);
				CDB.upd();
			}
			
			// Set our 'ChargesThrough' date, which are the only transactions
			// we will be messing with.  Coincidentally this is our cycle
			// start date...
			sprintf(chargesThrough, "%04d-%02d-%02d",
			  CycleStart.year(),
			  CycleStart.month(),
			  CycleStart.day()
			);
			strcpy(chargesThrough, Yesterday);
			STDB.setValue("ChargesThrough", chargesThrough);
			
			// Get our previous balance, which are any charges that have
			// a statement number associated with them.
			tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld and StatementNo <> 0",
			  CDB.getLong("CustomerID")
		    );
		    STDB.setValue("PrevBalance", tmpFloat);
			
			// Now, get the new charges.
			tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld and StatementNo = 0 and TransDate <= '%s' and Amount > 0.00",
			  CDB.getLong("CustomerID"),
			  chargesThrough
			);
			STDB.setValue("NewCharges", tmpFloat);
			NewCharges += tmpFloat;
			
			// Now, get the new charges.
			tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld and StatementNo = 0 and TransDate <= '%s' and Amount < 0.00",
			  CDB.getLong("CustomerID"),
			  chargesThrough
			);
			STDB.setValue("Credits", tmpFloat);
			Credits += tmpFloat;

			// We can now calculate the total due.
			STDB.setValue("TotalDue", STDB.getFloat("PrevBalance") + STDB.getFloat("NewCharges") + STDB.getFloat("Credits"));

            // Before we insert, we need to determine if it needs to
            // be emailed, printed, or both.  Start out by saying that neither
            // needs to be done, and we'll only change it if we need to do
            // it.
            STDB.setValue("ToBeEmailed", (int) 0);
            STDB.setValue("ToBePrinted", (int) 0);

            if (strlen(CDB.getStr("PrimaryLogin"))) {
	            DB.query("select InternalID from Logins where CustomerID = %ld and LoginID = '%s'", CDB.getLong("CustomerID"), (const char *)CDB.getStr("PrimaryLogin"));
	            if (!DB.rowCount) {
	                // Primary login doesn't seem to exist in the database.  Check for any
	                // active logins.
                    DB.query("select InternalID from Logins where CustomerID = %ld and Active <> 0", CDB.getLong("CustomerID"));
                    if (!DB.rowCount) {
                        // No active logins.  Set this statement to
                        // be printed.
                        STDB.setValue("ToBePrinted", (int) 1);
                    } else {
                        // They have active logins.  Take the first
                        // one and email it to them.
                        DB.getrow();
                        lnDB.get(atol(DB.curRow["InternalID"]));
                        STDB.setValue("ToBeEmailed", (int) 1);
                        STDB.setValue("EmailedTo", lnDB.getStr("LoginID"));
                    }
	            } else {
	                // Okay, lets make sure that the primary login is active.
	                DB.getrow();
	                lnDB.get(atol(DB.curRow["InternalID"]));
	                if (!lnDB.getInt("Active")) {
	                    // Primary login is not active.  See if they have
	                    // any other logins that can get the bill.
	                    DB.query("select InternalID from Logins where CustomerID = %ld and Active <> 0", CDB.getLong("CustomerID"));
	                    if (!DB.rowCount) {
	                        // No active logins.  Set this statement to
	                        // be printed.
	                        STDB.setValue("ToBePrinted", (int) 1);
	                    } else {
	                        // They have active logins.  Take the first
	                        // one and email it to them.
	                        DB.getrow();
	                        lnDB.get(atol(DB.curRow["InternalID"]));
	                        STDB.setValue("ToBeEmailed", (int) 1);
	                        STDB.setValue("EmailedTo", lnDB.getStr("LoginID"));
	                    }
	                } else {
	                    // Primary login is active.  All is well.
	                    STDB.setValue("ToBeEmailed", (int) 1);
	                    STDB.setValue("EmailedTo", CDB.getStr("PrimaryLogin"));
	                }
	            }
            } else {
                // No primary login.  Set the statement to be printed.
                STDB.setValue("ToBePrinted", (int) 1);
            }
            
            // Now, check for printing.
            if (CDB.getInt("PrintedStatement")) {
                STDB.setValue("ToBePrinted", (int) 1);
            }
            
			// Okay, insert the record...
			STDB.ins();
			// printf("\nThe statement number is %ld.\n", STDB.getLong("StatementNo"));
			RetVal = STDB.getLong("StatementNo");
			
			// The first line item should be the previous balance.
			SDDB.setValue("StatementNo", STDB.getLong("StatementNo"));
			SDDB.setValue("TransDate", chargesThrough);
			SDDB.setValue("LoginID", "");
			SDDB.setValue("StartDate", chargesThrough);
			SDDB.setValue("EndDate", chargesThrough);
			SDDB.setValue("Special", 1);
			SDDB.setValue("Quantity", (float)0.0);
			SDDB.setValue("Price", (float)0.0);
			SDDB.setValue("Amount", STDB.getFloat("PrevBalance"));
			SDDB.setValue("Description", "Previous balance");
			SDDB.ins();
			
			// Now that we have a statement number, we want to get all of
			// the individual line items we're going to charge the user for
			// from the AcctsRecv table.
			DB.query("select InternalID from AcctsRecv where CustomerID = %ld and StatementNo = 0 and TransDate <= '%s'",
			  CDB.getLong("CustomerID"),
			  chargesThrough
			);
			while (DB.getrow()) {
			    arDB.get(atol(DB.curRow["InternalID"]));
			    SDDB.clearData();
			    SDDB.setValue("StatementNo", STDB.getLong("StatementNo"));
				SDDB.setValue("TransDate", arDB.getStr("TransDate"));
				SDDB.setValue("LoginID", arDB.getStr("LoginID"));
				SDDB.setValue("StartDate", arDB.getStr("StartDate"));
				SDDB.setValue("EndDate", arDB.getStr("EndDate"));
				// Check to see if this is a payment.
				if (arDB.getFloat("Amount") <= 0.0) {
					// Payments don't have date ranges.
					SDDB.setValue("Special", 1);
				} else {
					SDDB.setValue("Special", 0);
				}
				SDDB.setValue("Quantity", arDB.getStr("Quantity"));
				SDDB.setValue("Price", arDB.getStr("Price"));
				SDDB.setValue("Amount", arDB.getStr("Amount"));
				SDDB.setValue("Description", arDB.getStr("Memo"));
				SDDB.ins();
				// printf("StatementNo = %ld, Line InternalID = %ld\n", STDB.getLong("StatementNo"), SDDB.getLong("InternalID"));
				
				// Now, update the dates from the AcctsRecv table.
				DB2.dbcmd("update AcctsRecv set StatementNo = %ld, BilledDate = '%s', DueDate = '%s' where InternalID = %ld",
				  STDB.getLong("StatementNo"),
				  Today,
				  sDueDate,
				  atol(DB.curRow["InternalID"])
				);
				
				// And update the dates in the GL
				DB2.dbcmd("update GL set BilledDate = '%s', DueDate = '%s' where TransID = %ld",
				  Today,
				  sDueDate,
				  arDB.getLong("GLTransID")
				);
			}
			
			// Now that we're all done, update the customer record again.
			CDB.setValue("LastBilled", Today);
			CDB.setValue("LastStatementNo", STDB.getLong("StatementNo"));
			CDB.setValue("GraceDate", sGraceDate);
			CDB.upd();
		} else {
			// Their balance is really zero, so update their 
			// balance to zero and return.
			CDB.setValue("CurrentBalance", (float) 0.00);
			CDB.upd();
		}
	}

	return(RetVal);
}

/*
** doAutoPayments   - Checks to see if the user is set up with any automatic
**                    payment methods.  If they are, it posts it in the batch.
*/

void doAutoPayments(long CustID)
{
    ADB                 DB;
    AutoPaymentsDB      APDB;
    CCTransDB           CCTDB;
    CustomersDB         CDB;
    QDate               tmpDate1;
    QDate               tmpDate2;
    char                chargeDate[80];
    char                expDate[80];
    char                tmpStr[1024];
    int                 tmpYear;
    
    tmpDate1    = QDate::currentDate();
    
    // Make sure the customer has a balance before we go charging them.
    CDB.get(CustID);
    if (CDB.getFloat("CurrentBalance") <= 0.00) return;
    
    // See if the user has any active automatic payment methods.
    DB.query("select InternalID from AutoPayments where CustomerID = %ld and Active <> 0", CustID);
    if (DB.rowCount) {
        DB.getrow();
        APDB.get(atol(DB.curRow["InternalID"]));
        
        CDB.get(CustID);
        
        // Calculate the transaction date for the charge.
        tmpDate2 = tmpDate1.addDays(APDB.getInt("ChargeDay"));
        QDatetomyDate(chargeDate, tmpDate2);
        
        // Find out what type of payment it is.
        switch(APDB.getInt("PaymentType")) {
            case 1:             // Credit card transaction
                // Turn the expiration date into MMYY format.
                strcpy(tmpStr, APDB.getStr("ExpDate"));
                myDatetoQDate(tmpStr, &tmpDate1);
                tmpYear = tmpDate1.year();
                if (tmpYear >= 2000) tmpYear -= 2000;
                if (tmpYear >= 1900) tmpYear -= 1900;
                sprintf(expDate, "%02d%02d", tmpDate1.month(), tmpYear);
            
                CCTDB.setValue("BPSExported",   0);
                CCTDB.setValue("CustomerID",    CustID);
                CCTDB.setValue("EnteredBy",     "Auto");
                CCTDB.setValue("TransDate",     chargeDate);
                CCTDB.setValue("Name",          APDB.getStr("Name"));
                CCTDB.setValue("Address",       APDB.getStr("Address"));
                CCTDB.setValue("ZIP",           APDB.getStr("ZIP"));
                CCTDB.setValue("Amount",        CDB.getStr("CurrentBalance"));
                CCTDB.setValue("CardType",      atoi(APDB.getStr("CardType")));
                CCTDB.setValue("ExpDate",       expDate);
                CCTDB.setValue("CardNo",        APDB.getStr("AcctNo"));
                CCTDB.ins();
                break;
                
            default:
                break;
        }
        
    }
}


/*
** emailStatement   - Checks the specified statement number and emails it
**                    to the email address in EmailedTo.
*/

void emailStatement(long StNo, int Force = 0, int Dup = 0)
{
	ADB                 DB;
	CustomersDB			CDB;
	StatementsDB		STDB;
	StatementsDataDB	SDDB;
	NotesDB             NDB;
	FILE				*stfp;
	FILE                *tmpfp;
	char				tmpstr[1024];
	char				fmtLine1[128];
	char				fmtLine2[128];
	char				fmtLine1s[128];
	char                tmpFName[512];
	char                headerFName[512];
	char                footerFName[512];
	QDate               tmpDate;
	QDate               tmpDate2;
	QDate               tmpDate3;
	char                theDate[32];
	char                tmpLogin[64];
	char                duplicateMsg[128];
	float               tmpAmountDue;

	STDB.get(StNo);

    // Check to see if we need to email this statement.  If not, return.
    if (!Force) {
    	if (!STDB.getInt("ToBeEmailed")) return;
    }

    if (Dup) {
        sprintf(duplicateMsg, "(Duplicate)");
    } else {
        strcpy(duplicateMsg, "");
    }
	
	CDB.get(STDB.getLong("CustomerID"));
	
	// Get a temporary file name so we can write the statement to it.
	tmpnam(tmpFName);
	tmpnam(headerFName);
	tmpnam(footerFName);
	stfp = fopen(tmpFName, "w");
	if (stfp == NULL) {
		printf("Unable to create file '%s' for writing.\n", tmpFName);
		return;
	}

    // Parse the header and footer files.
    strcpy(tmpLogin, STDB.getStr("EmailedTo"));
    parseFile("/usr/local/lib/taa/StatementHeader.txt", headerFName, STDB.getLong("CustomerID"), tmpLogin, "");
    parseFile("/usr/local/lib/taa/StatementFooter.txt", footerFName, STDB.getLong("CustomerID"), tmpLogin, "");

    // Put the email headers into the file.
    fprintf(stfp, "From: Avvanta Accounting <accounting@avvanta.com>\n");
    fprintf(stfp, "To: %s@%s\n", tmpLogin, cfgVal("EmailDomain"));
    // fprintf(stfp, "To: marc\n");
    fprintf(stfp, "Subject: Account Statement\n");
    fprintf(stfp, "\n\n");

    // Now, write the header file to our output file.
    tmpfp = fopen(headerFName, "r");
    if (tmpfp != NULL) {
        while (fgets(tmpstr, sizeof(tmpstr), tmpfp) != NULL) {
            fprintf(stfp, tmpstr);
        }
        fclose(tmpfp);
        unlink(headerFName);
    } else {
        printf("Warning!  Could not open header file '%s' for reading.\n", headerFName);
    }
    
    // Now for the actual statement data itself.
		
	// The Customer's address
	fprintf(stfp, "%s\n", (const char *) STDB.getStr("CustName"));
	fprintf(stfp, "%s\n", (const char *) STDB.getStr("CustAddr1"));
	fprintf(stfp, "%s\n", (const char *) STDB.getStr("CustAddr2"));
	fprintf(stfp, "%s\n", (const char *) STDB.getStr("CustAddr3"));
	fprintf(stfp, "%s\n", (const char *) STDB.getStr("CustAddr4"));
	fprintf(stfp, "\n\n");
	
	// The header information
	myDatetoQDate(STDB.getStr("StatementDate"), &tmpDate);
	//fprintf(stfp, "%18s:  %s\n",   "Statement Date",   (const char *) tmpDate.toString());
	myDatetoQDate(STDB.getStr("ChargesThrough"), &tmpDate);
	fprintf(stfp, "%18s:  %s\n",   "Charges Through",  (const char *) tmpDate.toString());
	fprintf(stfp, "%18s:  %09ld %s\n","Reference Number", STDB.getLong("StatementNo"), duplicateMsg);
	//fprintf(stfp, "%18s:  %s\n",   "Terms",            (const char *) STDB.getStr("Terms"));
    // Convert the date into something more readable.
	myDatetoQDate(STDB.getStr("DueDate"), &tmpDate);
	fprintf(stfp, "%18s:  %s\n",   "Payment Due By",       (const char *) tmpDate.toString());
	fprintf(stfp, "%18s:  %8.2f\n","Previous Balance", STDB.getFloat("PrevBalance"));
	fprintf(stfp, "%18s:  %8.2f\n","Credits/Payments", STDB.getFloat("Credits"));
	fprintf(stfp, "%18s:  %8.2f\n","New Charges", 	   STDB.getFloat("NewCharges"));

	tmpAmountDue = STDB.getFloat("TotalDue");
    // Check for pending automatic payments...
	tmpAmountDue -= getAutoPaymentLine(STDB.getLong("CustomerID"), tmpstr);
	if (strlen(tmpstr)) {
	    fprintf(stfp, "%s\n", tmpstr);
	}
	
	if (tmpAmountDue <= 0.00) {
	    fprintf(stfp, "%18s:  %8.2f %s\n","Total Amount Due", tmpAmountDue, "DO NOT PAY");
	} else {
   		fprintf(stfp, "%18s:  %8.2f\n","Total Amount Due", tmpAmountDue);
    }
    
	// Now, print each of the line items.
	// Setup the format for each of the line items.
	strcpy(fmtLine1, "%02d/%02d/%02d  %-16s  %02d/%02d/%02d-%02d/%02d/%02d  %5.2f %7.2f %7.2f\n");
	strcpy(fmtLine1s,"%02d/%02d/%02d  %-50s %7.2f\n");
	strcpy(fmtLine2, "%s\n");
	
	// Print the line item headers.
	fprintf(stfp, "\n\n%-8s  %-16s  %-16s  %4s  %7s  %7s\n",
	  "Date",
	  "User Name",
	  "Service Period",
	  "Qty",
	  "Price",
	  "Amount"
	);
	fprintf(stfp, "Description\n\n");
	
	DB.query("select InternalID from StatementsData where StatementNo = %ld order by InternalID", STDB.getLong("StatementNo"));
	while (DB.getrow()) {
		SDDB.get(atol(DB.curRow["InternalID"]));
		
		if (SDDB.getInt("Special")) {
		    myDatetoQDate(SDDB.getStr("TransDate"), &tmpDate);
			fprintf(stfp, fmtLine1s,
			  tmpDate.month(),
			  tmpDate.day(),
			  tmpDate.year() % 100,
			  SDDB.getStr("LoginID"),
			  SDDB.getFloat("Amount")
			);
		} else {
		    myDatetoQDate(SDDB.getStr("TransDate"), &tmpDate);
		    myDatetoQDate(SDDB.getStr("StartDate"), &tmpDate2);
		    myDatetoQDate(SDDB.getStr("EndDate"), &tmpDate3);
			fprintf(stfp, fmtLine1,
			  tmpDate.month(),
			  tmpDate.day(),
			  tmpDate.year() % 100,
			  (const char *) SDDB.getStr("LoginID"),
			  tmpDate2.month(),
			  tmpDate2.day(),
			  tmpDate2.year() % 100,
              tmpDate3.month(),
              tmpDate3.day(),
              tmpDate3.year() % 100,
			  SDDB.getFloat("Quantity"),
			  SDDB.getFloat("Price"),
			  SDDB.getFloat("Amount")
			);
		}
		
		if (strlen(SDDB.getStr("Description"))) {
    		fprintf(stfp, fmtLine2, SDDB.getStr("Description"));
        }
		fprintf(stfp, "\n");
		
	}
	
    // Now, write the footer file to our output file.
    tmpfp = fopen(footerFName, "r");
    if (tmpfp != NULL) {
        while (fgets(tmpstr, sizeof(tmpstr), tmpfp) != NULL) {
            fprintf(stfp, tmpstr);
        }
        fclose(tmpfp);
        unlink(footerFName);
    } else {
        printf("Warning!  Could not open footer file '%s' for reading.\n", headerFName);
    }
    


	fclose(stfp);

    sprintf(tmpstr, "cp %s /var/spool/taamail/statement-%09ld", tmpFName, STDB.getLong("StatementNo"));
	system(tmpstr);
	unlink(tmpFName);
	
	// Now, update the database with the date that the statement was mailed.
	tmpDate = QDate::currentDate();
	QDatetomyDate(theDate, tmpDate);
	STDB.setValue("EmailedOn", theDate);
	STDB.setValue("ToBeEmailed", (int) 0);
	STDB.upd();
	
    NDB.setValue("AutoNote", 1);
	NDB.setValue("LoginID", tmpLogin);
	NDB.setValue("CustomerID", CDB.getLong("CustomerID"));
	NDB.setValue("AddedBy", "Accounting");
	NDB.setValue("NoteType", "Accounting");
	NDB.setValue("NoteText", "Emailed statement number ");
	NDB.appendStr("NoteText", STDB.getStr("StatementNo"));
	NDB.appendStr("NoteText", " sent to ");
	NDB.appendStr("NoteText", tmpLogin);
	if (Dup) {
	    NDB.appendStr("NoteText", " (duplicate)");
	}
	NDB.ins();

}

/*
** getAutoPaymentLine - Creates the text for the automatic payment line
**                      and returns the amount of the automatic payment.
*/

float getAutoPaymentLine(long CustID, char *prline)
{
    float   RetVal = 0.00;
    ADB     DB;
    char    cardType[80];
    
    // Check for any pending charges (from auto payments)
    DB.query("select InternalID from CCTrans where CustomerID = %ld and BPSExported = 0", CustID);
    if (DB.rowCount) {
        CCTransDB   CCTDB;
        DB.getrow();
        CCTDB.get(atol(DB.curRow["InternalID"]));
        
        // Card Type
        switch (CCTDB.getInt("CardType")) {
            case CCTYPE_MC:
                sprintf(cardType, "Mastercard");
                break;
            
            case CCTYPE_VISA:
                sprintf(cardType, "Visa");
                break;
                
            case CCTYPE_AMEX:
                sprintf(cardType, "American Express");
                break;
                
            default:
                sprintf(cardType, "???");
                break;
        }
        
        sprintf(prline, "%18s:  %8.2f  (%s on %s)",
            "Automatic Payment",
            CCTDB.getFloat("Amount"),
            cardType,
            (const char *) CCTDB.getStr("TransDate")
        );
        RetVal = CCTDB.getFloat("Amount");
        
    } else {
        strcpy(prline, "");
    }
   
    return(RetVal);
}

/*
** printStatement   - Checks the specified statement number and prints it
**                    if necessary.
*/

void printStatement(long StNo, int Force = 0, int Dup = 0)
{
    StatementsDB    STDB;
    QDate           tmpDate;
    char            theDate[32];
    NotesDB         NDB;
    
    tmpDate = QDate::currentDate();
    QDatetomyDate(theDate, tmpDate);
    
    STDB.get(StNo);
    if (!Force) {
        if (!STDB.getInt("ToBePrinted")) return;
    }

    int useInternal = 1;
    if (strlen(cfgVal("BuiltInPrintedStatements"))) {
        if (!atoi(cfgVal("BuiltInPrintedStatements"))) {
            useInternal = 0;
        }
    }
    if (useInternal) {
        STDB.print(NULL);
    } else {
        printStatementFromFile(StNo);
    }
    STDB.setValue("PrintedOn", theDate);
    STDB.setValue("ToBePrinted", (int) 0);
    STDB.upd();

    NDB.setValue("AutoNote", 1);
	NDB.setValue("LoginID", "");
	NDB.setValue("CustomerID", STDB.getLong("CustomerID"));
	NDB.setValue("NoteType", "Accounting");
	NDB.setValue("AddedBy", "Accounting");
	NDB.setValue("NoteText", "Printed statement number ");
	NDB.appendStr("NoteText", STDB.getStr("StatementNo"));
	if (Dup) {
	    NDB.appendStr("NoteText", " (duplicate)");
	}
	NDB.ins();

}


