/*
** $Id: OverdueAccounts.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** OverdueAccounts - Selects overdue accounts for processing.
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
** $Log: OverdueAccounts.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "OverdueAccounts.h"
#include <qmessagebox.h>
#include <qdatetm.h>
#include <qapplication.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include "EditCustomer.h"
#include "ParseFile.h"
#include <ADB.h>

#define Inherited OverdueAccountsData

OverdueAccounts::OverdueAccounts
(
	QWidget* parent,
	const char* name,
	int AccountTypes
)
	:
	Inherited( parent, name )
{
	setCaption( "Overdue Account Processing (Active Accounts)" );
	
	list->setAllColumnsShowFocus(TRUE);
	list->setMultiSelection(TRUE);
	
    totAccts    = 0;
    selAccts    = 0;
    totOverdue  = 0.0;
    totSelected = 0.0;
    
    myAccountTypes = AccountTypes;
    	
	show();
    fillList();
    
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), SLOT(editCustomer(QListViewItem *)));
    connect(list, SIGNAL(returnPressed(QListViewItem *)), SLOT(editCustomer(QListViewItem *)));
    connect(list, SIGNAL(selectionChanged()), SLOT(updateTotals()));
}


OverdueAccounts::~OverdueAccounts()
{
}

/*
** fillList - Fills the list with the overdue transactions.
*/

void OverdueAccounts::fillList(void)
{
    int         totSteps = 0;
    int         curPos = 0;
    ADB         DB;
    ADB         DB2;
    ADB         DB3;
    
    QDate       theDate;
    QDate       oldestDate;
    QDate       graceDate;
    QDate       tmpDate;
    char        today[16];
    float       overdueBal;
    
    char        tmpStr[64];
    char        curBal[64];
    char        tmpBal[64];
    char        graceStr[64];
    int         pastGrace = 0;
    QString     QnoteDate;
    
    char        oldestDue[64];
    
    QApplication::setOverrideCursor(waitCursor);

    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);
    
    statusBar->setText("Searching...");
    
    progress->setProgress(0);
    DB.query("select Customers.CustomerID, Customers.FullName, Customers.PrimaryLogin, Customers.CurrentBalance, Customers.GraceDate, SUM(AcctsRecv.Amount), SUM(AcctsRecv.ClearedAmount), MIN(AcctsRecv.DueDate), SUM(Logins.Active) from Customers, AcctsRecv, Logins where Customers.CustomerID = AcctsRecv.CustomerID and Logins.CustomerID = Customers.CustomerID and Customers.CurrentBalance > 0.00 and AcctsRecv.ClearedAmount <> AcctsRecv.Amount and AcctsRecv.DueDate <> '0000-00-00' and Logins.Active > 0 GROUP by CustomerID");
    totSteps    = DB.rowCount;
    emit(setProgress(0, totSteps));
    if (DB.rowCount) while (DB.getrow()) {
        emit(setProgress(++curPos, totSteps));

        // Get the number of days until their grace expires.
        tmpDate = QDate::currentDate();
        strcpy(tmpStr, DB.curRow["GraceDate"]);
        myDatetoQDate(tmpStr, &graceDate);
        pastGrace = graceDate.daysTo(tmpDate);
        sprintf(graceStr, "%03d", pastGrace);

        // Get the oldest overdue item
        myDatetoQDate(DB.curRow["MIN(AcctsRecv.DueDate)"], &oldestDate);   // MIN(AcctsRecv.DueDate)
	    sprintf(oldestDue, "%03d", oldestDate.daysTo(theDate));

        sprintf(curBal, "%8.2f", atof(DB.curRow["CurrentBalance"]));

	    overdueBal = atof(DB.curRow["SUM(AcctsRecv.Amount)"]) - atof(DB.curRow["SUM(AcctsRecv.ClearedAmount)"]);  // SUM(AcctsRecv.Amount) - SUM(AcctsRecv.ClearedAmount)
	    sprintf(tmpBal, "%8.2f", overdueBal);
	    totOverdue += overdueBal;
	    totAccts++;
	            
        
        // Only add them if the oldest date > 0 days.
        if (atoi(oldestDue)) {
            (void) new QListViewItem(list, 
                                     DB.curRow["CustomerID"], 
                                     DB.curRow["FullName"], 
                                     DB.curRow["PrimaryLogin"], 
                                     curBal,
                                     tmpBal, oldestDue, graceStr);
        }
    }
    
    sprintf(tmpStr, "%ld", totAccts);
    accountsOverdue->setText(tmpStr);
    sprintf(tmpStr, "%.2f", totOverdue);
    amountOverdue->setText(tmpStr);
    
    emit setProgress(1,1);
    statusBar->setText("");
    QApplication::restoreOverrideCursor();
}

/*
** cancelOverdue  - A simple function that just closes the window.
*/

void OverdueAccounts::cancelOverdue()
{
    close();
}


/*
** selectNone   - A simple function that scans through the list and
**                un-selects any selected items.
*/

void OverdueAccounts::selectNone()
{
    QListViewItem   *curItem;
    
    list->hide();
    curItem = list->firstChild();
    while (curItem != NULL) {
        curItem->setSelected(FALSE);
        curItem = curItem->itemBelow();
    }
    updateTotals();
    list->show();
}

/*
** selectNone   - A simple function that scans through the list and
**                un-selects any selected items.
*/

void OverdueAccounts::selectAll()
{
    QListViewItem   *curItem;
    
    list->hide();
    curItem = list->firstChild();
    while (curItem != NULL) {
        curItem->setSelected(TRUE);
        curItem = curItem->itemBelow();
    }
    updateTotals();
    list->show();
}


/*
** editCustomer - When the user double clicks on an item, it will
**                bring up the customer editor to make any changes to
**                the customer record.
*/

void OverdueAccounts::editCustomer(QListViewItem *curItem)
{
    long CustID = atol(curItem->key(0, 1));
    EditCustomer *custEdit;
    custEdit = new EditCustomer(0, "", CustID);
    custEdit->show();
}

/*
** updateTotals  - Connected to list->selectionChanged.  It scans through
**                 the list and updates the totals.
*/

void OverdueAccounts::updateTotals()
{
    long    totSelected = 0;
    float   amtSelected = 0.0;
    char    tmpStr[512];
    
    QListViewItem   *curItem;
    
    curItem = list->firstChild();
    while (curItem != NULL) {
        if (curItem->isSelected()) {
            totSelected++;
            amtSelected += atof(curItem->key(4, 1));
        }
        curItem = curItem->itemBelow();
    }
    
    sprintf(tmpStr, "%ld", totSelected);
    accountsSelected->setText(tmpStr);
    sprintf(tmpStr, "%.2f", amtSelected);
    selectedOverdue->setText(tmpStr);
    
}



/*
** processSelections - Processes the selected items in the list.
*/

void OverdueAccounts::processSelections()
{
    long    totSelected;
    int     Action = 0;
    char    tmpStr[1024];
    
    totSelected = atol(accountsSelected->text());
    
    if (noAction->isChecked()) Action = 0;
    if (actionOne->isChecked()) Action = 1;
    if (actionTwo->isChecked()) Action = 2;
    
    
    if (!totSelected) {
        QMessageBox::warning(this, "Process Overdue Accounts", "There were no accounts selected to process.");
    } else {
        // Make sure that we want to do this by prompting the user.
        sprintf(tmpStr, "Are you sure you want to\nprocess %ld overdue accounts?", totSelected);
        switch(QMessageBox::information(this,
          "Overdue Account Processing",
          tmpStr,
          "&Yes", "&No", "&Cancel",
          0,            // Enter == button 0
          2             // Escape == button 2
        )) {
            case 0:
                // They want to process the selections.
                // doSelections...
                if (!myAccountTypes) {
                    switch (Action) {
                        case 1:
                            sendReminders();
                            break;
                        
                        case 2:
                            lockAccounts();
                            break;
                        
                        default:
                            break;
                    }
                    close();        // We're done.
                }

                break;
                
            case 1:
                // They don't want to process the selections;
                close();
                break;
                
            case 2:
            default:
                // They hit escape.
                break;
        }
    }
}


/*
** sendReminders - Scans throught the list and sends a reminder message
**                 to all selected accounts.
*/

void OverdueAccounts::sendReminders(void)
{
    ADB             DB;
    CustomersDB     CDB;
    NotesDB         NDB;
    char            tmpStr[1024];
    QDate           theDate;
    QDate           transDate;
    QDate           dueDate;
    char            today[16];
    char            tmpLogin[32];
    char            tmpDomain[32];
    float           overdueBalance;
    
    FILE            *fp;
    FILE            *fp1;
    
    
    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);

    strcpy(tmpDomain, "");
    QListViewItem   *curItem;
    long            curCount = 0;

    statusBar->setText("Sending reminders...");
    
    progress->setTotalSteps(totAccts);
    progress->setProgress(curCount);
    progress->show();
    progress->reset();
    
    QApplication::setOverrideCursor(waitCursor);

    curItem = list->firstChild();
    while (curItem != NULL) {
        progress->setProgress(++curCount);
        if (curItem->isSelected()) {
            list->setCurrentItem(curItem);
            list->setSelected(curItem, FALSE);
            list->repaintItem(curItem);
            list->ensureItemVisible(curItem);
            list->repaint();
            
            // Here we actually want to get the account information
            // and send the reminder off to the user.
	        DB.query("select Amount, ClearedAmount, TransDate, DueDate, Memo, LoginID from AcctsRecv where CustomerID = %ld and DueDate <= '%s' and ClearedAmount <> Amount order by DueDate", atol(curItem->key(0,1)), today);
	        if (DB.rowCount) {
	            CDB.get(atol(curItem->key(0, 1)));
	            // Okay, the customer has charges.  Create the header.
	            strcpy(tmpLogin, CDB.getStr("PrimaryLogin"));
	            sprintf(tmpStr, "/tmp/.reminder.a%08ld", atol(curItem->key(0, 1)));
	            parseFile("/usr/local/lib/taa/ReminderHeader.txt", tmpStr, atol(curItem->key(0,1)), tmpLogin, tmpDomain);
	            fp = fopen(tmpStr, "a");
	            overdueBalance = 0.0;
	            if (fp != NULL) {
	                fprintf(fp, "%-10s    %-10s           %-10s        %10s %10s\n%-60s\n\n",
	                  "Trans Date", "Due Date", "Login ID", "Amount", "Overdue", "Description"
	                );
                    while (DB.getrow()) {
                        myDatetoQDate(DB.curRow["TransDate"], &transDate);
                        myDatetoQDate(DB.curRow["DueDate"], &dueDate);
		                fprintf(fp, "%02d/%02d/%04d    %02d/%02d/%04d           %-10s        %10.2f %10.2f\n%-75s\n\n",
		                  transDate.month(), transDate.day(), transDate.year(),
		                  dueDate.month(), dueDate.day(), dueDate.year(),
		                  DB.curRow["LoginID"], 
		                  atof(DB.curRow["Amount"]),
		                  atof(DB.curRow["Amount"]) - atof(DB.curRow["ClearedAmount"]),
		                  DB.curRow["Memo"]
		                );
		                overdueBalance += (atof(DB.curRow["Amount"]) - atof(DB.curRow["ClearedAmount"]));
                    }
                    // Give the user some nice totals.
                    fprintf(fp, "\n%62s: %10.2f\n", "Current Balance", CDB.getFloat("CurrentBalance"));
                    fprintf(fp, "%62s: %10.2f\n", "Overdue Balance", overdueBalance);
                    
                    
                    // Now, add the footer information to the working
                    // file.
		            sprintf(tmpStr, "/tmp/.reminder.b%08ld", atol(curItem->key(0, 1)));
		            parseFile("/usr/local/lib/taa/ReminderFooter.txt", tmpStr, atol(curItem->key(0,1)), tmpLogin, tmpDomain);
		            sprintf(tmpStr, "/tmp/.reminder.b%08ld", atol(curItem->key(0, 1)));
                    fp1 = fopen(tmpStr, "r");
                    while (fgets(tmpStr, sizeof(tmpStr), fp1)) fputs(tmpStr, fp);
                    fclose(fp);
                    fclose(fp1);
		            sprintf(tmpStr, "/tmp/.reminder.b%08ld", atol(curItem->key(0, 1)));
                    unlink(tmpStr);

                    // Now, lets add some sendmail headers to this guy and
                    // pipe it through sendmail.
		            sprintf(tmpStr, "/tmp/.reminder.b%08ld", atol(curItem->key(0, 1)));
                    fp  = fopen(tmpStr, "w");
                    fprintf(fp, "From: Blarg Accounting <accounting@blarg.net>\n");
                    // fprintf(fp, "To: %s@blarg.net\n", "marc");
                    fprintf(fp, "To: %s@blarg.net\n", (const char *) CDB.getStr("PrimaryLogin"));
                    fprintf(fp, "Subject: Reminder\n");
                    fprintf(fp, "\n");
                    
                    // Now, add the other file to this one.
		            sprintf(tmpStr, "/tmp/.reminder.a%08ld", atol(curItem->key(0, 1)));
                    fp1 = fopen(tmpStr, "r");
                    while (fgets(tmpStr, sizeof(tmpStr), fp1)) fputs(tmpStr, fp);
                    fclose(fp);
                    fclose(fp1);
		            sprintf(tmpStr, "/tmp/.reminder.a%08ld", atol(curItem->key(0, 1)));
                    unlink(tmpStr);
                    
                    sprintf(tmpStr, "cp /tmp/.reminder.b%08ld /var/spool/taamail/reminder.b%08ld", atol(curItem->key(0,1)), atol(curItem->key(0,1)));
                    // sprintf(tmpStr, "cat /tmp/.reminder.b%08ld | /usr/sbin/sendmail -t -Faccounting@blarg.net", atol(curItem->key(0, 1)));
                    system(tmpStr);
                    
                    sprintf(tmpStr, "/tmp/.reminder.b%08ld", atol(curItem->key(0, 1)));
                    unlink(tmpStr);
                    
                    // Add a note so we know that a reminder was sent.
                    NDB.setValue("LoginID", CDB.getStr("PrimaryLogin"));
                    NDB.setValue("CustomerID", atol(curItem->key(0, 1)));
                    NDB.setValue("NoteType", "Accounting");
                    NDB.setValue("NoteText", "Overdue balance reminder sent");
                    NDB.setValue("AutoNote", 1);
                    NDB.ins();
                    
	            } else {
                    QMessageBox::critical(this, "Send Reminder", "Error creating work file.");
	            }
            }
        }
        curItem = curItem->itemBelow();
        updateTotals();
    }
    QApplication::restoreOverrideCursor();
}

/*
** lockAccounts - Scans through the list and locks all active logins
**                for all selected accounts.  This one will be fun.
*/

void OverdueAccounts::lockAccounts(void)
{
    ADB             DB;
    ADB             DB2;
    CustomersDB     CDB;
    NotesDB         NDB;
    LoginsDB        LDB;
    char            tmpstr[1024];
    QDate           theDate;
    char            today[16];
    
    BrassClient     *BC;
    
    
    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);

    QListViewItem   *curItem;
    long            curCount = 0;

    statusBar->setText("Disabling logins...");
    
    progress->setTotalSteps(totAccts);
    progress->setProgress(curCount);
    progress->show();
    progress->reset();
    
    
    QApplication::setOverrideCursor(waitCursor);
    // Create our Brass connection.
    BC  = new BrassClient();
    if (!BC->Connect()) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "BRASS Error", "Error connecting to the BRASS server.");
        delete BC;
        return;
    } else if (!BC->Authenticate()) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "BRASS Error", "Error authenticating with BRASS server.");
        delete BC;
        return;
    }

    // If we made it this far, we should be okay and connected to the BRASS
    // server.

    curItem = list->firstChild();
    while (curItem != NULL) {
        progress->setProgress(++curCount);
        if (curItem->isSelected()) {
            list->setCurrentItem(curItem);
            list->setSelected(curItem, FALSE);
            list->repaintItem(curItem);
            list->ensureItemVisible(curItem);
            list->repaint();
            
            // Here we actually want to get the account information
            // and send the reminder off to the user.
	        DB.query("select Amount, ClearedAmount, TransDate, DueDate, Memo, LoginID from AcctsRecv where CustomerID = %ld and DueDate <= '%s' and ClearedAmount <> Amount order by DueDate", atol(curItem->key(0,1)), today);
	        if (DB.rowCount) {
	            CDB.get(atol(curItem->key(0, 1)));
	            // Now, that we have the customer ID, find a list of
	            // accounts that the user has active.
	            DB.query("select LoginID from Logins where CustomerID = %ld and Active <> 0", atol(curItem->key(0,1)));
	            if (DB.rowCount) while (DB.getrow()) {
	                LDB.get(atol(curItem->key(0,1)), DB.curRow["LoginID"]);
	                // printf("Locking Customer ID %08ld, Login %-10s\n", atol(curItem->key(0,1)), DB.curRow[0]);

                    // Lock the account through the Brass connection.
                    if (!BC->LockUser(DB.curRow["LoginID"])) {
                        // The locking of the user failed.
                        QApplication::restoreOverrideCursor();
                        sprintf(tmpstr, "Unable to lock the account.\n\n%s", BC->ResponseStr(NULL));
                        QMessageBox::critical(this, "BRASS Error", tmpstr);
                        QApplication::setOverrideCursor(waitCursor);
                    } else {
                        // Set the Active Flag on the login.
                        LDB.setValue("Active", (int) 0);
                        LDB.upd();
                        
                        // Okay, the account is locked.  Update their 
                        // subscriptions.
                        DB2.dbcmd("update Subscriptions set Active = 0 where CustomerID = %ld and LoginID = '%s'", atol(curItem->key(0,1)), DB.curRow["LoginID"]);
                        
	                    // Now, add a note to their account showing that we
	                    // locked it.
                        NDB.setValue("NoteID",  0);
	                    NDB.setValue("LoginID", DB.curRow["LoginID"]);
	                    NDB.setValue("CustomerID", atol(curItem->key(0, 1)));
                        NDB.setValue("ParentNoteID", 0);
                        NDB.setValue("AutoNote", 1);
	                    NDB.setValue("NoteType", "Accounting");
	                    NDB.setValue("Category", "Accounting");
	                    NDB.setValue("SubCategory", "Overdue Account");
	                    NDB.setValue("Subject", "Overdue Account");
	                    NDB.setValue("NoteText", "Account locked for overdue balance.");
	                    NDB.ins();
                    }
	            }
            }
        }
        curItem = curItem->itemBelow();
        updateTotals();
    }
    
    delete BC;
    
    QApplication::restoreOverrideCursor();

}

