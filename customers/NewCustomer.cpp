/*
** $Id$
**
***************************************************************************
**
** NewCustomer - 
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
** $Log: NewCustomer.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "NewCustomer.h"
#include "BlargDB.h"
#include "BString.h"
#include "EditCustomer.h"
#include "LoginEdit.h"
#include <Cfg.h>
#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qapp.h>
#include <qcursor.h>
#include <qdatetm.h>
#include <time.h>
#include <sys/timeb.h>
#include <ADB.h>
#include <TAATools.h>
#include <qmessagebox.h>


#define Inherited NewCustomerData

NewCustomer::NewCustomer
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	ADB     DB;
	QDate   todaysDate;
	QDate   packageDate;
	
	todaysDate  = QDate::currentDate();
	
	setCaption( "New Customer Creation" );
	
	QApplication::setOverrideCursor(waitCursor);
	// Fill in the rateplan combo box.
	DB.query("select PlanTag from RatePlans");
	while (DB.getrow()) {
		ratePlan->insertItem(DB.curRow["PlanTag"]);
	}

	// Fill in the package combo box.
	DB.query("select PackageTag, AutoConvert, PromoPackage, PromoEnds from Packages where Active <> 0");
	while (DB.getrow()) {
	    // Check for a promotional package.
	    if (atoi(DB.curRow["AutoConvert"]) && atoi(DB.curRow["PromoPackage"])) {
	        // Check for a promotional package.
	        myDatetoQDate(DB.curRow["PromoEnds"], &packageDate);
	        if (packageDate >= todaysDate) {
	            package->insertItem(DB.curRow["PackageTag"]);
	        }
	    } else {
    		package->insertItem(DB.curRow["PackageTag"]);
        }
	}

    DB.query("select DISTINCT ReferredBy from Customers order by ReferredBy");
    while (DB.getrow()) {
    	if (strlen(DB.curRow["ReferredBy"])) referredBy->insertItem(DB.curRow["ReferredBy"]);
	}
	if (referredBy->count()) {
		referredBy->setCurrentItem(0);
	}
	
	// doneButton->setFocus();
	doneButton->setDefault(TRUE);
	fullName->setFocus();

	QApplication::restoreOverrideCursor();

    dayPhone->setInputMask("000/000-0000 xxxxxxxxxx");
    evePhone->setInputMask("000/000-0000 xxxxxxxxxx");
    faxPhone->setInputMask("000/000-0000 xxxxxxxxxx");
    altPhone->setInputMask("000/000-0000 xxxxxxxxxx");
}


NewCustomer::~NewCustomer()
{
}



/*
** addCustomer - Validates the entry and, if everything checks out,
**				 saves the customer into the database, then loads the
**				 Customer Editor to give the user the ability to add
**				 logins to the account.
**
*/

void NewCustomer::addCustomer()
{
	QString	tmpqstr;
	QString tmpqstr2;
	QString tmpqstr3;
	QString tmpphone;
	QString warnings;
	char	tmpstr[512];
	char	query[1024];
	ADB     DB;
	int		defaultYes = 1;
	CustomersDB     cdb;
	AddressesDB     adb;
	PhoneNumbersDB  pdb;
	NotesDB		    ndb;
	RatePlansDB		rpdb;
	BillingCyclesDB	bcdb;
	QString	PhoneQuery;
	char	PhoneQueryFMT[128];
	char	calledBackAt[128];
	char    cdayPhone[64] = "";
	char    cevePhone[64] = "";
	char    cfaxPhone[64] = "";
	char    caltPhone[64] = "";
	int     tmpPackageNo;
    char    pct[10];
    
    strcpy(pct, "%");


	PhoneQuery = "";	
	strcpy(PhoneQueryFMT, "PhoneNumber LIKE '%s'");
	
	QStrList matchList(TRUE);
	
	char	theDate[16];
	time_t	ti = time(NULL);
	struct tm * tm = localtime(&ti);
	sprintf(theDate, "%04d-%02d-%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
	
	// Okay.  The user has clicked the add button.  Lets do some validation.
	warnings = "";

    fprintf(stderr, "Checking full name...\n");
    // First, validate that the full name is valid.
	tmpqstr  = fullName->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (!strlen(tmpstr)) {
		fullName->setFocus();
        QMessageBox::critical(this, "Invalid Entry", "A full name must be specified.");
		return;
	} else {
		cdb.setValue("FullName", tmpstr);
	}
	cdb.setValue("ContactName", contactName->text());
	cdb.setValue("AltContact",  altContact->text());

    fprintf(stderr, "Checking address...\n");
	// Now, the address...
	tmpqstr  = address1->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (!strlen(tmpstr)) {
		warnings.append("Address Line 1 is blank.\n");
	} else {
		adb.Address1 = tmpstr;
	}
	
	tmpqstr = address2->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	adb.Address2 = tmpstr;

	tmpqstr = city->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (!strlen(tmpstr)) {
		warnings.append("City line is blank.\n");
	} else {
		adb.City = tmpstr;
	}

	tmpqstr = state->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (!strlen(tmpstr)) {
		warnings.append("State entry is blank.\n");
	} else {
		adb.State = tmpstr;
	}

	tmpqstr = zip->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (!strlen(tmpstr)) {
		warnings.append("ZIP Code entry is blank.\n");
	} else {
		adb.ZIP = tmpstr;
	}

	tmpqstr = country->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	strcpy(tmpstr, tmpqstr);
	if (strlen(tmpstr)) {
		warnings.append("Addresses in the US should have the country field left blank.\n");
		adb.Country = tmpstr;
	}
	
	if (warnings.length()) {
        QMessageBox::warning(this, "Warnings", warnings);
		defaultYes = 0;
	}
	
    fprintf(stderr, "Checking phone numbers...\n");
	// Make sure that each of the phone numbers are in the correct
	// format ###/###-#### or, international format (not implemented)
	strcpy(tmpstr, dayPhone->text());
	if (checkPhone(tmpstr)) {
		dayPhone->setFocus();
        QMessageBox::critical(this, "Invalid Entry", "Invalid daytime phone number.\n\nPhone numbers should be in the format\n\n ###/###-####\nor\n###/###-#### x####");
		return;
	} else {
	    strcpy(cdayPhone, tmpstr);
	}
	
	strcpy(tmpstr, evePhone->text());
	if (checkPhone(tmpstr)) {
		evePhone->setFocus();
        QMessageBox::critical(this, "Invalid Entry", "Invalid evening phone number.\n\nPhone numbers should be in the format\n\n ###/###-####\nor\n###/###-#### x####");
		return;
	} else {
	    strcpy(cevePhone, tmpstr);
	}
	
	strcpy(tmpstr, faxPhone->text());
	if (checkPhone(tmpstr)) {
		faxPhone->setFocus();
        QMessageBox::critical(this, "Invalid Entry", "Invalid fax phone number.\n\nPhone numbers should be in the format\n\n ###/###-####\nor\n###/###-#### x####");
		return;
	} else {
	    strcpy(cfaxPhone, tmpstr);
	}
	
	strcpy(tmpstr, altPhone->text());
	if (checkPhone(tmpstr)) {
		altPhone->setFocus();
        QMessageBox::critical(this, "Invalid Entry", "Invalid other phone number.\n\nPhone numbers should be in the format\n\n ###/###-####\nor\n###/###-#### x####");
		return;
	} else {
	    strcpy(caltPhone, tmpstr);
	}
	
    fprintf(stderr, "Checking for duplicate phone numbers...\n");
	// Do a phone number cross reference.
	// First, on the day phone number.  We want to strip out the area code
	strcpy(tmpstr, dayPhone->text());
    if (strlen(tmpstr)) {
        tmpqstr = getField('/', 2, tmpstr);
        strcpy(tmpstr, tmpqstr);
        tmpqstr = getField(' ', 1, tmpstr);
        strcpy(tmpstr, "%");
        strcat(tmpstr, tmpqstr);
        strcat(tmpstr, "%");
        // strcpy(tmpstr, tmpqstr);
        if (strlen(tmpstr)) {
            if (PhoneQuery.length()) PhoneQuery.append(" or ");
            sprintf(query, PhoneQueryFMT, tmpstr, tmpstr, tmpstr, tmpstr);
            PhoneQuery.append(query);
        }
    }

	// Now, on the evening phone number.  We want to strip out the area code.
	strcpy(tmpstr, evePhone->text());
    if (strlen(tmpstr)) {
        tmpqstr = getField('/', 2, tmpstr);
        strcpy(tmpstr, tmpqstr);
        tmpqstr = getField(' ', 1, tmpstr);
        // strcpy(tmpstr, tmpqstr);
        strcpy(tmpstr, "%");
        strcat(tmpstr, tmpqstr);
        strcat(tmpstr, "%");
        if (strlen(tmpstr)) {
            if (PhoneQuery.length()) PhoneQuery.append(" or ");
            sprintf(query, PhoneQueryFMT, tmpstr, tmpstr, tmpstr, tmpstr);
            PhoneQuery.append(query);
        }
    }

	// Now, on the Fax phone number.  We want to strip out the area code.
	strcpy(tmpstr, faxPhone->text());
    if (strlen(tmpstr)) {
        tmpqstr = getField('/', 2, tmpstr);
        strcpy(tmpstr, tmpqstr);
        tmpqstr = getField(' ', 1, tmpstr);
        //strcpy(tmpstr, tmpqstr);
        strcpy(tmpstr, "%");
        strcat(tmpstr, tmpqstr);
        strcat(tmpstr, "%");
        if (strlen(tmpstr)) {
            if (PhoneQuery.length()) PhoneQuery.append(" or ");
            sprintf(query, PhoneQueryFMT, tmpstr, tmpstr, tmpstr, tmpstr);
            PhoneQuery.append(query);
        }
    }

	// Now, on the Alternate phone number.  We want to strip out the area code.
	strcpy(tmpstr, altPhone->text());
    if (strlen(tmpstr)) {
        tmpqstr = getField('/', 2, tmpstr);
        strcpy(tmpstr, tmpqstr);
        tmpqstr = getField(' ', 1, tmpstr);
        //strcpy(tmpstr, tmpqstr);
        strcpy(tmpstr, "%");
        strcat(tmpstr, tmpqstr);
        strcat(tmpstr, "%");
        if (strlen(tmpstr)) {
            if (PhoneQuery.length()) PhoneQuery.append(" or ");
            sprintf(query, PhoneQueryFMT, tmpstr, tmpstr, tmpstr, tmpstr);
            PhoneQuery.append(query);
        }
    }

	// Now, make sure that at least one phone number has been entered.
	if (!strlen(dayPhone->text()) && !strlen(evePhone->text()) &&
	    !strlen(faxPhone->text()) && !strlen(altPhone->text())) {
	    
	    dayPhone->setFocus();
        QMessageBox::critical(this, "Unable to process", "At least one phone number must\nbe entered for a customer account.");
	    return;
	}

    fprintf(stderr, "Setting up the rest of the customer fields...\n");
	// Get the remaining fields and shove them into the cdb object.
	// Get the Rate Plan
	rpdb.getbytag(ratePlan->currentText());
	cdb.setValue("RatePlan", rpdb.getLong("InternalID"));
	bcdb.getDefaultCycle();
	cdb.setValue("BillingCycle", bcdb.InternalID);
	
	// And their payment terms.
	strcpy(tmpstr, cfgVal("DefaultCustomerTerms"));
	cdb.setValue("Terms", atoi(tmpstr));

	// Get the Statement Type
	if (statementType->currentItem()) {
		cdb.setValue("PrintedStatement", 1);
		cdb.setValue("FreePrintedStatement", 0);
		if (statementType->currentItem() == 2) cdb.setValue("FreePrintedStatement", 1);
	} else {
		cdb.setValue("PrintedStatement", 0);
		cdb.setValue("FreePrintedStatement", 0);
	}
	// cdb.FreePrintedStatement.setNum(freePrinted->isChecked());
	cdb.setValue("ReferredBy", referredBy->currentText());
	cdb.setValue("RatePlanDate", theDate);
	cdb.setValue("AccountOpened", theDate);
	cdb.setValue("BillingAddress", "Billing");

    fprintf(stderr, "Inserting the new customer into the database...\n");
	// Finally, check to see if the user really wants to add this customer.
	// Insert the record and get the new customer ID
	long newCustID = cdb.ins();
		
	// Add their address into the database...
	adb.RefFrom.setNum(REF_CUSTOMER);
	adb.RefID.setNum(newCustID);
	adb.Tag = "Billing";
	adb.International.setNum(0);
	adb.Active.setNum(1);
	adb.LastModifiedBy = curUser().userName;
	adb.ins();
	
	// Add the various phone numbers that were entered...
	pdb.InternalID = 0;
	pdb.RefFrom.setNum(REF_CUSTOMER);
	pdb.RefID.setNum(newCustID);
	pdb.International.setNum(0);
	pdb.Active.setNum(1);
	pdb.LastModifiedBy = curUser().userName;
	
	if (strlen(cdayPhone)) {
	    pdb.Tag = "Daytime";
	    pdb.PhoneNumber = cdayPhone;
	    pdb.ins();
	}
	if (strlen(cevePhone)) {
	    pdb.Tag = "Evening";
	    pdb.PhoneNumber = cevePhone;
	    pdb.ins();
	}
	if (strlen(cfaxPhone)) {
	    pdb.Tag = "Fax";
	    pdb.PhoneNumber = cfaxPhone;
	    pdb.ins();
	}
	if (strlen(caltPhone)) {
	    pdb.Tag = "Alternate";
	    pdb.PhoneNumber = caltPhone;
	    pdb.ins();
	}
	

	// Add the note for the callback.
	ndb.setValue("CustomerID", newCustID);
	ndb.setValue("NoteType", "Operator");
	ndb.setValue("NoteText", calledBackAt);
	ndb.ins();
	
	// Add their initial notes, if any.
	tmpqstr = initialNotes->text();
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	if (tmpqstr.length()) {
        NotesDB     inDB;
		inDB.setValue("CustomerID", newCustID);
		inDB.setValue("NoteType", "Operator");
		inDB.setValue("NoteText", (const char *) tmpqstr);
		inDB.ins();
	}
		
	// Did the user select an initial package type for the customer?
	// If so, put it into the subscriptions.
	if (package->currentItem()) {
		SubscriptionsDB sdb;
		ADB	    tmpDB;
		tmpDB.query("select InternalID from Packages where PackageTag = '%s'", (const char *) package->currentText());
		tmpDB.getrow();
		tmpPackageNo = atoi(tmpDB.curRow["InternalID"]);
		tmpDB.query("select InternalID, Price from PackagesData where PackageID = %d and RatePlanID = %ld and CycleID = %ld", 
		  atoi(tmpDB.curRow["InternalID"]),
		  rpdb.getLong("InternalID"),
		  bcdb.InternalID
		);
		if (tmpDB.rowCount) {
			tmpDB.getrow();
			sdb.setValue("CustomerID", newCustID);
			sdb.setValue("Active", (int) 1);
			sdb.setValue("PackageNo", tmpPackageNo);
			sdb.setValue("Quantity", (float) 1.0);
			sdb.setValue("Price", atof(tmpDB.curRow["Price"]));
			sdb.ins();
		}
	}

	// Bring up the Customer Editor so the user can add records.
    
	EditCustomer *editCust;
	editCust = new EditCustomer(0, "", newCustID);
	editCust->show();
		
    // Now bring up a login editor so the user can immediatly add
    // a new login for the customer.
    // editCust->showPage(editCust->loginsTab);
    editCust->loginsTab->createNewLogin();
//    LoginEdit *le;
//    le = new LoginEdit(0, "", newCustID, "");
//    le->show();

	emit(refreshCustList());
		
	// Close this window, we are done...
	done(0);
}


/*
** checkPhone - Validates that a phone number is in the correct format.
**              Either ###/###-#### or ###/###-#### x#####
**
*/

int NewCustomer::checkPhone(char * phone)
{
	QString	tmpqstr;
	QString tmpqstr2;
	QString tmpqstr3;
	QString tmpphone;
	char	tmpstr[512];

	tmpqstr = phone;
	tmpqstr2 = tmpqstr.simplifyWhiteSpace();
	tmpqstr  = tmpqstr2.stripWhiteSpace();
	tmpphone = tmpqstr;
	if (tmpqstr.length()) {
		strcpy(tmpstr, tmpphone);
		// fprintf(stderr, "Phone number for area code = %s\n", tmpstr);
		tmpqstr2 = getField('/', 1, tmpstr);
		strcpy(tmpstr, tmpqstr2);
		// fprintf(stderr, "Area code = %s\n", tmpstr);
		if (tmpqstr2.toInt() < 100 || tmpqstr2.toInt() > 999 || tmpqstr2.length() != 3) {
			// dayPhone->setFocus();
			// msgbox.message("Invalid Entry", "Invalid daytime phone number area code.\n\nPhone numbers should be in the format ###/###-####");
			return(-1);
		}

		strcpy(tmpstr, tmpphone);
		tmpqstr3 = getField('/', 2, tmpstr);
		strcpy(tmpstr, tmpqstr3);
	    tmpqstr2 = getField(' ', 1, tmpstr);  // Ignore extensions.
		strcpy(tmpstr, tmpqstr2);
		// fprintf(stderr, "Phone number = %s\n", tmpstr);
		tmpqstr = getField('-', 1, tmpstr);
		tmpqstr2 = getField('-', 2, tmpstr);
		// fprintf(stderr, "Phone number = %s\n", tmpstr);
		if (tmpqstr.toInt() < 100 || tmpqstr.toInt() > 999 || tmpqstr.length() != 3) {
			// dayPhone->setFocus();
			// msgbox.message("Invalid Entry", "Invalid daytime phone number prefix.\n\nPhone numbers should be in the format ###/###-####");
			return(-1);
		}

		if (tmpqstr2.toInt() < 1 || tmpqstr2.toInt() > 9999 || tmpqstr2.length() != 4) {
			// dayPhone->setFocus();
			// msgbox.message("Invalid Entry", "Invalid daytime phone number.\n\nPhone numbers should be in the format ###/###-####");
			return(-1);
		}
	}
	
	return(0);
	
}
