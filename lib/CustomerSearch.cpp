/*
** $Id: CustomerSearch.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CustomerSearch - A standalone widget that allows the user to search
**                  through the customer database and pick a customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CustomerSearch.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "CustomerSearch.h"

#include <stdlib.h>
#include <stdio.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <BlargDB.h>
#include <ADB.h>

CustomerSearch::CustomerSearch(QWidget *parent, const char *name, WFlags f)
    : TAAWidget(parent, name, f)
{
    // A simple (in appearance) widget that will allow the user to
    // search for, and select, a customer.
    //
    // The layout will look like so:
    //
    //  Query: [ Search Box         ] [Search] [Clear]
    //  +---------+----------+-----------------------+
    //  + Cust ID | Login ID | Account Name          |
    //  +         |          |                       |
    //  +---------+----------+-----------------------+
    //
    // When the user types something in the search box and presses enter
    // or clicks the search button, the widget will do a search on the
    // customer database against the search text and fill in the list.
    // When the user clicks or scrolls onto an entry, itemHighlighted()
    // will be emitted with the customer ID.  When the user double clicks
    // or presses enter on an entry itemSelected() will be emitted with
    // the customer ID.


    searchText = new QLineEdit(this, "SearchText");
    searchText->setMaxLength(80);
    connect(searchText, SIGNAL(returnPressed()), this, SLOT(startSearch()));
    
    QLabel  *queryLabel = new QLabel(this, "Query Label");
    queryLabel->setText("&Query:");
    queryLabel->setAlignment(AlignRight|AlignVCenter);
    queryLabel->setBuddy(searchText);

    searchButton = new QPushButton(this, "SearchButton");
    searchButton->setText("Search");
    connect(searchButton, SIGNAL(clicked()), this, SLOT(startSearch()));

    QPushButton *clearButton = new QPushButton(this, "Clear Button");
    clearButton->setText("C&lear");
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearSearch()));

    custList = new QListView(this, "SearchResults");
    custList->setMargin(2);
    custList->setAllColumnsShowFocus(true);
    custList->setRootIsDecorated(true);
    custList->addColumn("Account Name");
    custList->addColumn("LoginID");
    custList->addColumn("Cust ID");
    connect(custList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(itemHighlighted(QListViewItem *)));
    connect(custList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(itemSelected(QListViewItem *)));
    connect(custList, SIGNAL(returnPressed(QListViewItem *)), this, SLOT(itemSelected(QListViewItem *)));

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 0, 0);
    QBoxLayout *sl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    sl->addSpacing(3);
    sl->addWidget(queryLabel, 0);
    sl->addSpacing(3);
    sl->addWidget(searchText, 1);
    sl->addSpacing(3);
    sl->addWidget(searchButton, 0);
    sl->addWidget(clearButton, 0);
    ml->addLayout(sl, 0);
    ml->addSpacing(3);
    ml->addWidget(custList, 1);

}

CustomerSearch::~CustomerSearch()
{
}


/*
** startSearch - Takes the text in searchText() and searches the database for
**               it, filling the list with the found customers.
**
**               Several different queries are performed to try and find
**               matches.  The first query that gets a match wins.
**                 1.  Search for an exact numeric match on CustomerID
**                 2.  Exact match on a LoginID
**                 3.  Matches in Customers.FullName or Customers.ContactName
**                 4.  Matches in Logins.LoginID or Logins.ContactName
**                 5.  No matches found...
*/

void CustomerSearch::startSearch()
{
    ADB         DB;
    ADB         DB2;
    ADBTable    custDB("Customers");
    char        tmpstr[4096]; 
    int         rowcnt = 0;
    int         curRow = 0;
    long        curCustID = 0;
    bool        allLoaded = false;
    QString tmpsub;
    tmpsub = "";
    char    *isActive  = new char[64];
    
    // Change the cursor to an hourglass
    QApplication::setOverrideCursor(waitCursor);
    emit(setStatus("Searching..."));
    
    // Save the state of the list.
    QListViewItem       *tmpCur;
    tmpCur  = custList->currentItem();
    
    emit(customerHighlighted(0));
    custList->clear();

    // Clear our map of loaded customers as well.
    loadedCusts.clear();

    tmpsub = searchText->text();

    QApplication::restoreOverrideCursor();
    // If there is no query at all, simply return since the
    // list is now empty.
    if (!tmpsub.length()) return;
    QApplication::setOverrideCursor(waitCursor);

    
    // First query.  CustomerID
    if (tmpsub.toLong()) {
        // They specified a customer ID.  Check for it
        DB.query("select CustomerID from Customers where CustomerID = %ld", tmpsub.toLong());
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    } 
    
    // Check to see if the user specified a search string.
    // If they didn't, load the whole list.
    if (!rowcnt) {
        if (!strlen((const char *)tmpsub)) {
            DB.query("select CustomerID from Customers");
            rowcnt = DB.rowCount;
            if (rowcnt) {
                while (DB.getrow()) {
                    curCustID = atol(DB.curRow["CustomerID"]);
                    // Load them into our loadedCusts map
                    if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
                }
            }
            allLoaded = true;
        }
    }

    // Second query.  LoginID
    if (!allLoaded) {
        DB.query("select CustomerID from Logins where LoginID = '%s'", (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }
    
    // Third query.  Customers.FullName or Customers.ContactName
    if (!allLoaded) {
        DB.query("select distinct CustomerID from Customers where FullName LIKE '%%%%%s%%%%' or ContactName LIKE '%%%%%s%%%%'", (const char *) tmpsub, (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }

    // Fourth query.  Logins.LoginID or Logins.ContactName
    if (!allLoaded) {
        DB.query("select distinct CustomerID from Logins where LoginID LIKE '%%%%%s%%%%' or ContactName LIKE '%%%%%s%%%%'", (const char *) tmpsub, (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }
    
    if (loadedCusts.count()) {
        // One of our queries matched.  DB will have a query full of
        // Customer IDs.  We run several other queries on this one
        // to get the entire customer record for them.
        emit(setStatus("Loading matching customers..."));
        loadedCustMap::Iterator it;
        for (it = loadedCusts.begin(); it != loadedCusts.end(); ++it) {
            emit(setProgress(++curRow, rowcnt));
            custDB.get(it.data());
            
            sprintf(tmpstr, "%s", custDB.getStr("FullName"));
            if (strlen(custDB.getStr("ContactName"))) {
                strcat(tmpstr, "/");
                strcat(tmpstr, custDB.getStr("ContactName"));
            }
            if (custDB.getInt("Active")) strcpy(isActive, "Yes");
            else strcpy(isActive, "No");

            // Create the parent record.
            tmpCur = new QListViewItem(custList, 
              tmpstr,               // FullName/Contact Name
              custDB.getStr("PrimaryLogin"),
              custDB.getStr("CustomerID"),
              custDB.getStr("CurrentBalance"),
              isActive              // Active
            );

            // Get any child records.
            DB2.query("select ContactName, LoginID, Active from Logins where CustomerID = %ld", custDB.getLong("CustomerID"));
            if (DB2.rowCount) while (DB2.getrow()) {
                // Make sure that we're not re-loading the primary login
                if (strcmp(custDB.getStr("PrimaryLogin"), DB2.curRow["LoginID"])) {
                    if (atoi(DB2.curRow["Active"])) strcpy(isActive, "Yes");
                    else strcpy(isActive, "No");
                    (void) new QListViewItem(tmpCur,
                                             DB2.curRow["ContactName"],
                                             DB2.curRow["LoginID"],
                                             custDB.getStr("CustomerID"),
                                             "",
                                             isActive
                                             );
                }
            }
            
        }
        emit(setStatus(""));
    }

    emit(setStatus(""));    // Searching...
    
    // emit(refreshTabs(1));
    
    QApplication::restoreOverrideCursor();

}

/*
** currentCustomer - When swallowing this widget, other widgets may wish
**                   to know what the currently highlighted customer ID
**                   is.  This function returns it, or zero if there
**                   is no customer currently selected.
*/

long CustomerSearch::currentCustomer()
{
    long            curCust  = 0;
    QListViewItem   *curItem = custList->currentItem();
    if (curItem) {
        curCust = atol(curItem->key(2,0));
    }
    return curCust;
}

/*
** itemHighlighted - An internal slot that gets called when the user 
**                   selects an item from the list.
*/

void CustomerSearch::itemHighlighted(QListViewItem *curItem)
{
    if (curItem) {
        emit(customerHighlighted(atol(curItem->key(2,0))));
    }
}

/*
** itemSelected   - An internal slot that gets called when the user double
**                  clicks or presses enter on a list item.
*/

void CustomerSearch::itemSelected(QListViewItem *curItem)
{
    if (curItem) {
        emit(customerSelected(atol(curItem->key(2,0))));
    }
}

/*
** clearSearch    - Gets called when the user hits the Clear button.
**                  it clears the query text and list and sets the
**                  focus to the query line edit.
*/

void CustomerSearch::clearSearch()
{
    emit(customerHighlighted(0));
    custList->clear();
    searchText->setText("");
    searchText->setFocus();
}

/*
** takeFocus - A nothing function that gives the searchText focus.
**             This is used for interface asthetics.
*/

void CustomerSearch::takeFocus()
{
    searchText->setFocus();
}

