/*
** $Id: TransferLogin.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** TransferLogin - Transfers a LoginID to another account.
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
** $Log: TransferLogin.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "TransferLogin.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include <ADB.h>

#define Inherited TransferLoginData

TransferLogin::TransferLogin
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Transfer Login" );
	list->addColumn("Customer ID");
	list->addColumn("Customer Name");
	list->setAllColumnsShowFocus(TRUE);
	
	listQuery->setFocus();
}


TransferLogin::~TransferLogin()
{
}

/*
** setSourceLogin  - Gives us the information that we need to finish the
**                   transfer.
*/

void TransferLogin::setSourceLogin(long custID, const char * LoginID)
{
    char    tmpSt[1024];
    
    myCustID    = custID;
    strcpy(sourceLogin, LoginID);
    
    sprintf(tmpSt, "Select the target customer for transferring the login '%s'.", LoginID);
    messageLabel->setText(tmpSt);
    doQuery();
}

/*
** doTransfer - A slot that does the actual transfer and emits the
**              refresh signal.
**
**              For now, it only moves the login records, the subscription
**              records and puts a note in both accounts advising the
**              transfer.
**
*/

void TransferLogin::doTransfer()
{
    QListViewItem   *curItem;
    
    curItem = list->currentItem();
    
    if (curItem == NULL) return;

    QApplication::setOverrideCursor(waitCursor);

    long    DestCustID  = atol(curItem->text(0));
    
    ADB             DB;
    NotesDB         NDB;
    CustomersDB     CDB;
    char            tmpStr[1024];
    
    CDB.get(DestCustID);
    
    // Add a note into the old customer's records first.
    sprintf(tmpStr, "Transferred login %s to Customer %ld (%s)", sourceLogin, DestCustID, (const char *) CDB.getStr("FullName"));
    NDB.setValue("CustomerID", myCustID);
    NDB.setValue("LoginID", sourceLogin);
    NDB.setValue("NoteType", "Operator");
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();
    
    // Update the subscriptions...
    DB.dbcmd("update Subscriptions set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin);

    // Update the Logins...
    DB.dbcmd("update Logins set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin);

    // Update the Domains...
    DB.dbcmd("update Domains set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin);

    // And now put a note into the destination account.
    CDB.get(myCustID);
    sprintf(tmpStr, "Transferred login %s from Customer %ld (%s)", sourceLogin, myCustID, (const char *) CDB.getStr("FullName"));
    NDB.setValue("CustomerID", DestCustID);
    NDB.setValue("LoginID", sourceLogin);
    NDB.setValue("NoteType", "Operator");
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();

    emit(refreshLoginList(1));
    emit(refreshCust(myCustID));

    QApplication::restoreOverrideCursor();

    QMessageBox::information(this, "Login Transfer Warning", "Note that login ID transfers are not\nrerated automatically.  You will need to\nmake any necessary adjustments to the\ncustomers register manually.");

    close();
}

/*
** doQuery - A slot that refines the query in our list.
*/

void TransferLogin::doQuery()
{
    char    querystr[1024];
    char    query[1024];
    char    tmpStr[1024];
    ADB     DB;
    
    
    list->clear();

    strcpy(querystr, listQuery->text());
        
    if (!strlen(querystr)) {
        sprintf(query, "select CustomerID, FullName, ContactName from Customers");
    } else {
        sprintf(query, "select CustomerID, FullName, ContactName from Customers where FullName LIKE '%%%s%%' or ContactName LIKE '%%%s%%'", querystr, querystr);
    }

    QApplication::setOverrideCursor(waitCursor);
    DB.query("%s", query);
    
    if (DB.rowCount) while (DB.getrow()) {
        strcpy(tmpStr, DB.curRow["FullName"]);
        if (strlen(DB.curRow["ContactName"])) {
            strcat(tmpStr, "/");
            strcat(tmpStr, DB.curRow["ContactName"]);
        }
        (void) new QListViewItem(list, DB.curRow["CustomerID"], tmpStr);
    }
    QApplication::restoreOverrideCursor();
}



/*
** cancelTransfer - A slot that closes the window.
*/

void TransferLogin::cancelTransfer()
{
    close();
}

