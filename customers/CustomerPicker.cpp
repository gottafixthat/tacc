/*
** $Id: CustomerPicker.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CustomerPicker - A dialog that lets the user find a customer.
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
** $Log: CustomerPicker.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CustomerPicker.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapplication.h>

#include <BlargDB.h>
#include <ADB.h>


#define Inherited CustomerPickerData

CustomerPicker::CustomerPicker
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Customer Picker" );
	queryLabel->setBuddy(listQuery);
	
    list->setMultiSelection(FALSE);
	list->setRootIsDecorated(TRUE);
	list->setAllColumnsShowFocus(TRUE);
	
	// By default, we'll allow an empty query.
	allowEmptyQuery = 0;
	
	// Fill the list
	doQuery();
	
	listQuery->setFocus();
}


CustomerPicker::~CustomerPicker()
{
}


/*
** doQuery - This function does the query and fills the list.
*/

void CustomerPicker::doQuery(void)
{
    // Clear the list before doing anything else.
    list->clear();

    // First, check to see if our query string is empty.  If it is,
    // make sure that we allow empty queries.  If we do not allow empty
    // queries, and the query is empty, simply clear the list and return.
    if (!strlen(listQuery->text()) && !allowEmptyQuery) return;
    
    QApplication::setOverrideCursor(waitCursor);
    ADB     DB;
    int     doExpansion = FALSE;
    int     rowcnt = 0;
    QString tmpsub;
    char    priLogin[1024];
    char    tmpstr[4096];
    char    *query = new char[32768];
    char    *querystr = new char[32768];
    char    *subquery = new char[16384];
    char    basequery[4096] = "select Customers.CustomerID, Customers.FullName, Customers.ContactName, Customers.PrimaryLogin, Customers.CurrentBalance, Logins.LoginID, Logins.ContactName, Logins.Active from Customers, Logins";
    char    isActive[64];
    long    curCustID = 0;
    
    QListViewItem   *tmpCur = NULL;
    QListViewItem   *tmpChild;

    // Okay, no do the actual query.
    tmpsub = listQuery->text();
    strcpy(subquery, listQuery->text());
    if (strlen(subquery)) {
        sprintf(querystr, "%s where ((Customers.FullName LIKE '%%%s%%' or Customers.ContactName LIKE '%%%s%%') or Logins.LoginID LIKE '%%%s%%') and Customers.CustomerID = Logins.CustomerID order by Customers.CustomerID", basequery, subquery, subquery, subquery);
    } else {
        sprintf(querystr, "%s where Customers.CustomerID = Logins.CustomerID order by Customers.CustomerID", basequery);
    }

    // We'll run four queries.  Trying the next only if the previous
    // query returns no records.  This will allow us to very quickly
    // get a single customer record out of the database.
    
    // First, we want to see if the user specified a customer ID.  If they
    // did, then we load that customer ID directly.
    if (tmpsub.toLong()) {
	    DB.query("%s where (Customers.CustomerID = %ld) and Customers.CustomerID = Logins.CustomerID", basequery, tmpsub.toLong());
	    rowcnt = DB.rowCount;
    }
    if (!rowcnt) {
        // Nope.  Try getting it by the primary login ID.
	    DB.query("%s where (Customers.PrimaryLogin = '%s') and Customers.CustomerID = Logins.CustomerID", basequery, (const char *)tmpsub);
	    rowcnt = DB.rowCount;
    }
    if (!rowcnt) {
        // Try finding it in the logins database.
        DB.query("select CustomerID from Logins where LoginID = '%s'", (const char *)tmpsub);
        if (DB.rowCount) {
            DB.getrow();
            // Got it, now re-do the query by CustomerID to get everything.
		    DB.query("%s where (Customers.CustomerID = %ld) and Customers.CustomerID = Logins.CustomerID", basequery, atol(DB.curRow["CustomerID"]));
		    rowcnt = DB.rowCount;
        }
    }
    if (!rowcnt) {
        // Nope.  Do the generic query.
        DB.query(querystr);
        rowcnt = DB.rowCount;
    } else {
        doExpansion = TRUE;
    }

    rowcnt = 0;
    while(DB.getrow()) {
        if (atoi(DB.curRow["Active"])) strcpy(isActive, "Yes");
        else strcpy(isActive, "No");
        QString tmpStr1 = DB.curRow["FullName"];
        if (strlen(DB.curRow["ContactName"])) {
            tmpStr1.append("/");
            tmpStr1.append(DB.curRow["ContactName"]);
        }
        tmpStr1.truncate(48);
        strcpy(tmpstr, tmpStr1);
        
        // Check to see if we have already inserted this customer into our
        // list.  If we have, it must be a child account, and gets added 
        // as such.  If we haven't, create a new top-level entry for this
        // record.
        if (atol(DB.curRow[0]) != curCustID) {
            strcpy(priLogin, DB.curRow["PrimaryLogin"]);
            tmpCur = new QListViewItem(list, 
              tmpstr,                       // FullName/Contact Name
              DB.curRow["CustomerID"],      // CustomerID
              DB.curRow["PrimaryLogin"],    // Primary Login
              DB.curRow["CurrentBalance"],  // Current Balance,
              isActive                      // Active
            );
            
            // If PrimaryLogin != LoginID, add the Login ID record.
            if (strcmp(DB.curRow["PrimaryLogin"], DB.curRow["LoginID"])) {
                // Not the parent.  Add a child record.
                tmpChild = new QListViewItem(tmpCur,
                  DB.curRow["ContactName"], // Logins.ContactName
                  DB.curRow["CustomerID"],  // CustomerID,
                  DB.curRow["LoginID"],     // LoginID
                  "",                       // Current Balance
                  isActive                  // Active
                );
	            if (!tmpsub.isEmpty()) {
	                tmpCur->setOpen(doExpansion);
	            }
            } else {
                // It is the primary login ID, we need to set the 
                // Active flag on the account.
                tmpCur->setText(4, isActive);
            }
            
            curCustID = atol(DB.curRow["CustomerID"]);
           
        } else {
            // Child record
            // If PrimaryLogin != LoginID, add the Login ID record.
            if (strcmp(DB.curRow["PrimaryLogin"], DB.curRow["LoginID"])) {
	            tmpChild = new QListViewItem(tmpCur,
	              DB.curRow["ContactName"],     // Logins.ContactName
	              DB.curRow["CustomerID"],      // CustomerID,
	              DB.curRow["LoginID"],         // LoginID
	              "",                           // Current Balance
	              isActive                      // Active
	            );
	            if (!tmpsub.isEmpty()) {
	                tmpCur->setOpen(doExpansion);
	            }
            } else {
                // It is the primary login ID, we need to set the 
                // Active flag on the account.
                tmpCur->setText(4, isActive);
            }

        }

        rowcnt++;
    }
    
    // Clean up...
    QApplication::restoreOverrideCursor();
    delete subquery;
    delete query;
}


/*
** listQueryS  - When the customer presses "Enter" on the list query
**               this slots gets called.  We refresh the list.
*/

void CustomerPicker::listQueryS()
{
    doQuery();
}

/* 
** clearQuery - When the user hits the "Clear" button, this slot gets
**              called.  It clears the query string, but does not clear
**              the list.  It also sets the focus to the query string.
*/

void CustomerPicker::clearQuery()
{
    listQuery->setText("");
    listQuery->setFocus();
}


/*
** itemDoubleClicked - Gets called when the user double clicks on a customer.
*/

void CustomerPicker::itemDoubleClicked(QListViewItem *selItem)
{
    if (selItem != NULL) {
        emit customerSelected(atol(selItem->key(3,0)));
    }
}

/*
** itemEnterHit - Gets called when the user presses enter on an item.
*/

void CustomerPicker::itemEnterHit(QListViewItem *selItem)
{
    if (selItem != NULL) {
        emit customerSelected(atol(selItem->key(3,0)));
    }
}


/*
** itemSelChanged - Gets called when the user selects a customer.
*/

void CustomerPicker::itemSelChanged(QListViewItem *selItem)
{
    if (selItem != NULL) {
        emit customerHilighted(atol(selItem->key(3,0)));
    }
}


