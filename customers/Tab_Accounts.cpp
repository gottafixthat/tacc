/*
** $Id: Tab_Accounts.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** FileName     - Overview of its main function.
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
** $Log: Tab_Accounts.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "Tab_Accounts.h"
#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qapplication.h>

#include "BlargDB.h"

#include <ADB.h>


#define Inherited Tab_AccountsData

Tab_Accounts::Tab_Accounts
(
	QWidget* parent,
	const char* name,
	long CustID
)
	:
	Inherited( parent, name )
{
	setCaption( "Accounts" );
	
	myCustID = CustID;
	
	refreshAccountsList(0);
}


Tab_Accounts::~Tab_Accounts()
{
}


/*
** refreshAccountsList - A public slot which will get called when an item
**					     in the Accounts list is changed.
**
*/

void Tab_Accounts::refreshAccountsList(int)
{
    ADB     DB;
    QString tmpstr = "";
    char querystr[512] = "";
    QString tmpQItemID = "";
    char    tmpItemID[128] = "";
    QString tmpQActive = "";
    char    tmpActive[128] = "";
    QString tmpLoginType = "";
    char    loginType[128] = "";
    char	tmpDateStamp[128] = "";
    QString	qds;
    ADB	    DB2;
    
    QListViewItem   *curLogin;
    
    list->clear();
    
	// Fill in the subscriptions ListBox
	if (hideWiped->isChecked()) {
        sprintf(querystr, "select LoginID, LoginType, ContactName, LastModified, Active, InternalID from Logins where CustomerID = %ld and Wiped < '1970-01-01' order by LoginID", myCustID);
    } else {
        sprintf(querystr, "select LoginID, LoginType, ContactName, LastModified, Active, InternalID from Logins where CustomerID = %ld order by LoginID", myCustID);
    }
	DB.query("%s", querystr);
	// subscrIndex = new(int[DB.rowCount + 1]);
	int idxPtr = 0;

    int isOdd = 1;
	if (DB.rowCount) while(DB.getrow()) {
		// subscrIndex[idxPtr] = atoi(DB.curRow[0]);
		idxPtr++;
		tmpQItemID = DB.curRow["ContactName"];
		tmpQItemID.truncate(11);
		strcpy(tmpItemID, tmpQItemID);
		    
		tmpQActive = "No";
		if (atoi(DB.curRow["Active"])) {
			tmpQActive = "Yes";
		}
		strcpy(tmpActive, tmpQActive);
		
		
		DB2.query("select LoginType from LoginTypes where InternalID = %d", atoi(DB.curRow["LoginType"]));
		if (DB2.rowCount && DB2.getrow()) {
			tmpLoginType = DB2.curRow["LoginType"];
		} else {
			tmpLoginType = "Unknown";
		}
		strcpy(loginType, tmpLoginType);
		
		// Parse out the time stamp.
		qds = DB.curRow["LastModified"];
		sprintf(tmpDateStamp, "%4s-%2s-%2s",
		  (const char *) qds.mid(0,4),
		  (const char *) qds.mid(4,2),
		  (const char *) qds.mid(6,2)
		);
		    
		curLogin = new QListViewItem(list);
		curLogin->setText(0, DB.curRow["LoginID"]);     // Login ID
		curLogin->setText(1, loginType);                // Login Type
		curLogin->setText(2, tmpDateStamp);             // Last Modified Date
		curLogin->setText(3, "");                       // Ends On Date
		curLogin->setText(4, DB.curRow["InternalID"]);  // InternalID
		curLogin->setText(5, "");                       // Quantity
		curLogin->setText(6, "");                       // Price
		curLogin->setText(7, "");                       // Amount
		curLogin->setText(8, tmpActive);                // Active?
		
		getSubscriptions(DB.curRow["LoginID"], curLogin);
		
		if (isOdd) {
		    isOdd--;
		} else {
		    isOdd++;
		}
		
		// list->insertItem(i);
	}
	
}

/*
** getSubscriptions  - Gets the subscriptions for the current Login ID.
**                     and adds them to the list.
*/

void Tab_Accounts::getSubscriptions(const char *LoginID, QListViewItem *parent)
{
    ADB             DB;
    BillablesDB	    BDB;
    BillablesDataDB BDDB;
    PackagesDB	    PDB;
    CustomersDB     CDB;
    QString tmpstr = "";
    QString tmpQItemID = "";
    char    tmpItemID[128] = "";
    QString tmpQDescr  = "";
    char    tmpDescr[128] = "";
    QString tmpQActive = "";
    char    tmpActive[128] = "";
    char    tmpLogin[128];
    char    packageInc[128];
    char    tmpPrice[128];
    QListViewItem   *tmpItem;

//    tmpTop = list->topItem();
//    tmpCur = list->currentItem();


    CDB.get(myCustID);
        
	// Fill in the subscriptions ListBox
    DB.query("select InternalID, LoginID, ItemNumber, Quantity, ItemDesc, Price, LastDate, EndsOn, Active, PackageNo, ParentID, AutoPrice from Subscriptions where CustomerID = %ld and LoginID = '%s' order by LoginID", myCustID, LoginID);
	// subscrIndex = new(int[DB.rowCount + 1]);
	int idxPtr = 0;
	if (DB.rowCount) while(DB.getrow()) {
		// subscrIndex[idxPtr] = atoi(DB.curRow[0]);
		idxPtr++;
		if (atoi(DB.curRow["PackageNo"])) {
			PDB.get(atol(DB.curRow["PackageNo"]));
			tmpQItemID = PDB.getStr("PackageTag");
			tmpQItemID.truncate(11);
			strcpy(tmpItemID, tmpQItemID);
		} else {
			BDB.get(atol(DB.curRow["ItemNumber"]));
			tmpQItemID = BDB.getStr("ItemID");
			tmpQItemID.truncate(11);
			strcpy(tmpItemID, tmpQItemID);
		}
		
		// Check for auto pricing
		if (atoi(DB.curRow["AutoPrice"])) {
		    strcpy(tmpPrice, "Auto");
		    // Since its an auto price, go get it.
		    BDDB.getItem(atol(DB.curRow["ItemNumber"]), CDB.getLong("RatePlan"), CDB.getLong("BillingCycle"));
		    sprintf(tmpPrice, "%.2f", BDDB.getFloat("Price"));
		} else {
		    sprintf(tmpPrice, "%.2f", atof(DB.curRow["Price"]));
		}
		    
		tmpQDescr = DB.curRow["ItemDesc"];
		tmpQDescr.truncate(24);
		strcpy(tmpDescr, tmpQDescr);
		    
		tmpQActive = "No";
		if (atoi(DB.curRow["Active"])) {
			tmpQActive = "Yes";
		}
		strcpy(tmpActive, tmpQActive);
		
		// Check to see if this is a package item.
		if (atoi(DB.curRow["PackageNo"])) {
		    strcpy(tmpLogin, "Package");
		} else {
		    sprintf(tmpLogin, "%s", DB.curRow["LoginID"]);
		}
		
		// Check to see if this item has a parent package.
		if (atoi(DB.curRow["ParentID"])) {
		    strcpy(packageInc, "*");
		} else {
		    strcpy(packageInc, " ");
		}
		tmpstr.sprintf("%-8s %s", tmpLogin, packageInc);

		tmpItem = new QListViewItem(parent);
		tmpItem->setText(0, tmpstr);                    // Login ID
		tmpItem->setText(1, tmpItemID);                 // Description
		tmpItem->setText(2, DB.curRow["LastDate"]);     // Last Date
		tmpItem->setText(3, DB.curRow["EndsOn"]);       // Ends On
		tmpItem->setText(4, DB.curRow["InternalID"]);   // ID
		tmpItem->setText(5, DB.curRow["Quantity"]);     // Quantity
		tmpItem->setText(6, tmpPrice);                  // Price
		tmpItem->setText(7, "");                        // Amount
		tmpItem->setText(8, tmpActive);                 // Active

		parent->setExpandable(TRUE);
		parent->setOpen(TRUE);

//		list->insertItem(tmpstr);
	}

}
