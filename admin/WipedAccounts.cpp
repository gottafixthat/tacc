/*
** $Id$
**
***************************************************************************
**
** WipedAccounts - Actions to take on wiped accounts.
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
** $Log: WipedAccounts.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "WipedAccounts.h"

#include <qmessagebox.h>
#include <qprogressbar.h>
#include <qdatetm.h>
#include <qapplication.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <AcctsRecv.h>
#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include "EditCustomer.h"
#include "ParseFile.h"

#define Inherited WipedAccountsData

WipedAccounts::WipedAccounts
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Overdue Account Processing (Inactive Accounts)" );
	
	// Set up our list.
	list->setAllColumnsShowFocus(TRUE);
	list->setMultiSelection(TRUE);
	
    totAccts    = 0;
    selAccts    = 0;
    totOverdue  = 0.0;
    totSelected = 0.0;
    
    myAccountTypes = 0; // AccountTypes;
    	
	show();
    fillList();
    
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), SLOT(editCustomer(QListViewItem *)));
    connect(list, SIGNAL(returnPressed(QListViewItem *)), SLOT(editCustomer(QListViewItem *)));
    connect(list, SIGNAL(selectionChanged()), SLOT(updateTotals()));
}


WipedAccounts::~WipedAccounts()
{
}

/*
** fillList - Fills the list with the overdue transactions.
*/

void WipedAccounts::fillList(void)
{
    int         totSteps = 0;
    int         curPos = 0;
    ADB         DB;
    ADB         DB2;
    ADB         DB3;
    ADB         DB4;
    
    QDate       theDate;
    QDate       newestDate;
    QDate       oldestDate;
    QDate       tmpDate;
    char        today[16];
    float       overdueBal;
    
    char        tmpStr[64];
    char        tmpBal[64];
    char        noteAge[64];
    char        noteDate[64];
    QString     QnoteDate;
    
    char        oldestDue[64];
    char        newestDue[64];
    
    QApplication::setOverrideCursor(waitCursor);

    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);
    
    progressLabel->setText("Searching...");
    progressLabel->show();
    
    progress->setProgress(0);
    DB.query("select CustomerID, FullName, PrimaryLogin, CurrentBalance from Customers where CurrentBalance > 0.00");
    totSteps    = DB.rowCount;
    progress->setTotalSteps(totSteps);
    progress->show();
    while (DB.getrow()) {
        progress->setProgress(++curPos);
        DB2.query("select Amount, ClearedAmount, DueDate from AcctsRecv where CustomerID = %ld and DueDate < '%s' and ClearedAmount <> Amount order by DueDate", atol(DB.curRow["CustomerID"]), today);
        if (DB2.rowCount) {
            // For this particular report, we need to make sure that they
            // have no active logins, so tighten our query down even further.
            DB4.query("select LoginID from Logins where CustomerID = %ld and Wiped < '1990-01-01'", atol(DB.curRow["CustomerID"]));
            //DB3.query("select LoginID from Logins where CustomerID = %ld and Active = 0 and Wiped > '1990-01-01'", atol(DB.curRow["CustomerID"]));
            if (!DB4.rowCount) {
	            oldestDate = QDate::currentDate();
	            newestDate.setYMD(1900, 01, 01);
	            overdueBal = 0.00;
	            while (DB2.getrow()) {
	                myDatetoQDate(DB2.curRow["DueDate"], &tmpDate);
	                if (tmpDate < oldestDate) myDatetoQDate(DB2.curRow["DueDate"], &oldestDate);
	                if (tmpDate > newestDate) myDatetoQDate(DB2.curRow["DueDate"], &newestDate);
	                overdueBal += (atof(DB2.curRow["Amount"]) - atof(DB2.curRow["ClearedAmount"]));
	            }
	            
	            sprintf(oldestDue, "%03d", oldestDate.daysTo(theDate));
	            sprintf(newestDue, "%03d", newestDate.daysTo(theDate));
	            sprintf(tmpBal, "%.2f", overdueBal);
	            totOverdue += overdueBal;
	            totAccts++;
	            
	            // Check to see if they have any notes.  If so, we want to
	            // Show the number of days old that the most recent one is.
	            DB2.query("select NoteDate from Notes where CustomerID = %ld order by NoteDate DESC", atol(DB.curRow["CustomerID"]));
	            if (DB2.rowCount) {
	                // We only want the first row.
	                DB2.getrow();
	                QnoteDate = DB2.curRow["NoteDate"];
	                QnoteDate.truncate(10);
					sprintf(noteDate, "%4s-%2s-%2s",
					  (const char *) QnoteDate.mid(0,4),
					  (const char *) QnoteDate.mid(4,2),
					  (const char *) QnoteDate.mid(6,2)
					);
			        myDatetoQDate(noteDate, &tmpDate);
	                sprintf(noteAge, "%d", tmpDate.daysTo(theDate));

	            } else {
	                strcpy(noteAge, "N/A");
	            }
	            
	            // Only add them if the oldest date > 0 days.
                if (atoi(oldestDue)) {
    	            (void) new QListViewItem(list, DB.curRow["CustomerID"], DB.curRow["FullName"], DB.curRow["PrimaryLogin"], DB.curRow["CurrentBalance"], tmpBal, newestDue, oldestDue, noteAge);
    	        }
	        }
        }
    }
    
    sprintf(tmpStr, "%ld", totAccts);
    accountsOverdue->setText(tmpStr);
    sprintf(tmpStr, "%.2f", totOverdue);
    amountOverdue->setText(tmpStr);
    
    progress->hide();
    progressLabel->hide();
    QApplication::restoreOverrideCursor();
}

/*
** cancelOverdue  - A simple function that just closes the window.
*/

void WipedAccounts::cancelOverdue()
{
    close();
}


/*
** selectNone   - A simple function that scans through the list and
**                un-selects any selected items.
*/

void WipedAccounts::selectNone()
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

void WipedAccounts::selectAll()
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

void WipedAccounts::editCustomer(QListViewItem *curItem)
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

void WipedAccounts::updateTotals()
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

void WipedAccounts::processSelections()
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
                            // sendReminders();
                            break;
                        
                        case 2:
                            sendToCollections();
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
** sendToCollections - Scans through the list and sends all selected accounts
**                     to collections. This one will be fun.
*/

void WipedAccounts::sendToCollections(void)
{
    CustomersDB CDB;
    NotesDB     NDB;
    QDate       theDate;
    char        today[16];
    
    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);

    QListViewItem   *curItem;
    long            curCount = 0;

    progressLabel->setText("Sending to collections...");
    progressLabel->show();
    
    progress->setTotalSteps(totAccts);
    progress->setProgress(curCount);
    progress->show();
    progress->reset();
    
    
    QApplication::setOverrideCursor(waitCursor);

    // Scan through the list...
    curItem = list->firstChild();
    while (curItem != NULL) {
        progress->setProgress(++curCount);
        if (curItem->isSelected()) {

            // Do the collections report
            CollReport  CR;
            CR.printReport(atol(curItem->key(0,1)));
            
            list->setCurrentItem(curItem);
            list->setSelected(curItem, FALSE);
            list->repaintItem(curItem);
            list->ensureItemVisible(curItem);
            list->repaint();

            // Now, update their balance to $0.00
            // This isn't the best way to do it, but it will work
            // for now.  Treat it like a payment...Should be a transfer
            // from the customer's account to the collection agency.
            AcctsRecv   AR;
            
            AR.ARDB->setValue("TransDate", today);
            AR.ARDB->setValue("CustomerID", atol(curItem->key(0, 1)));
            AR.ARDB->setValue("RefNo", "");
            AR.ARDB->setValue("TransType", TRANSTYPE_PAYMENT);
            AR.ARDB->setValue("Price", (float) (atof(curItem->key(3,1)) * -1.0));
            AR.ARDB->setValue("Amount", (float) (atof(curItem->key(3,1)) * -1.0));
            AR.ARDB->setValue("Memo", "Sent to collection");
            AR.SaveTrans();

            // Now, add a note to their account showing that we
            // sent them to collection
            NDB.setValue("NoteID",  0);
            NDB.setValue("LoginID", "");
            NDB.setValue("CustomerID", atol(curItem->key(0, 1)));
            NDB.setValue("NoteType", "Accounting");
            NDB.setValue("NoteText", "Sent to collection");
            NDB.ins();
        }
        curItem = curItem->itemBelow();
        updateTotals();
    }
    
    QApplication::restoreOverrideCursor();

}



// The print class itself...

CollReport::CollReport()
{
}

CollReport::~CollReport()
{
}

/*
** printHeader - Puts a header on the page containing the Blarg address,
**               the date, etc.
*/

void CollReport::printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance)
{
    QDate       theDate;
    QRect       rect;
    QString     tmpSt;
    int         yPos;
    long        CustID = cust->getLong("CustomerID");

    theDate = QDate::currentDate();
    
    // p->rotate(55);
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(10, 30, "Blarg! Online Services, Inc.");
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText(10, 40, "PO Box 1827");
    p->drawText(10, 50, "Bellevue, WA 98009-1827");
    p->drawText(10, 60, "Phone:  425.401.9821");
    p->drawText(10, 70, "Fax: 425.401.9741");

    p->setFont(QFont("helvetica", 14, QFont::Bold));
    p->drawText(450, 40, "Collections Report");
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(470, 60, theDate.toString());
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawLine(  0,  75, 692, 75);

    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText( 20, 90, "Customer:");
    yPos = 90;
    if (strlen(cust->getStr("FullName"))) {
        p->drawText( 70, yPos, cust->getStr("FullName"));
        yPos += 10;
    }
    if (strlen(cust->getStr("ContactName"))) {
        p->drawText( 70, yPos, cust->getStr("ContactName"));
        yPos += 10;
    }
    if (strlen(cont->Address1)) {
        p->drawText( 70, yPos, cont->Address1);
        yPos += 10;
    }
    if (strlen(cont->Address2)) {
        p->drawText( 70, yPos, cont->Address2);
        yPos += 10;
    }

    tmpSt = cont->City;
    tmpSt.append(", ");
    tmpSt.append(cont->State);
    tmpSt.append(" ");
    tmpSt.append(cont->ZIP);
    p->drawText( 70,yPos, tmpSt);
    yPos += 10;



    p->setFont(QFont("courier", 10, QFont::Normal));
    rect.setCoords(350,80,450, 93);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Customer ID:");
    rect.setCoords(460,80,600, 93);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, cust->getStr("CustomerID"));

    rect.setCoords(350,90,450,103);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Balance:");
    rect.setCoords(460,90,600, 103);
    tmpSt.sprintf("$%.2f", Balance);
    p->setFont(QFont("courier", 10, QFont::Bold));
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, tmpSt);
    p->setFont(QFont("courier", 10, QFont::Normal));

    PhoneNumbersDB  PDB;

    rect.setCoords(350,100,450,113);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Day Phone:");
    rect.setCoords(460,100,600,113);
    PDB.get(REF_CUSTOMER, CustID, "Daytime");
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, PDB.PhoneNumber);
    rect.setCoords(350,110,450,123);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Eve Phone:");
    PDB.get(REF_CUSTOMER, CustID, "Evening");
    rect.setCoords(460,110,600,123);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, PDB.PhoneNumber);

}

/*
** printFooter - Puts a footer on the page containing the page number
*/

void CollReport::printFooter(QPainter *p, int PageNo)
{
    char    tmpSt[1024];
    
    sprintf(tmpSt, "Page %d", PageNo);

    p->drawLine(  0, 740, 692,740);
    p->drawText(290, 760, tmpSt);
}

/*
** registerHeader - Puts a register header on the page.
*/

void CollReport::registerHeader(QPainter *p)
{
    QRect       rect;
    QString     tmpSt;
    QBrush      bbrush;

    p->setFont(QFont("helvetica", 10, QFont::Normal));
//    p->drawRect(40, 150, 525, 15);
    bbrush.setStyle(Qt::SolidPattern);
    bbrush.setColor(Qt::black);

    p->setBackgroundMode(Qt::OpaqueMode);
    p->setPen(Qt::white);

    rect.setCoords(20, 150, 80, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Trans Date");

    rect.setCoords(80, 150, 140, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Due Date");
    p->drawLine(80, 150, 80, 165);
        
    rect.setCoords(140, 150, 200, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Login ID");
    p->drawLine(140, 150, 140, 165);
        
    rect.setCoords(200, 150, 460, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Description");
    p->drawLine(200, 150, 200, 165);

    rect.setCoords(460, 150, 520, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Amount");
    p->drawLine(460, 150, 460, 165);

    rect.setCoords(520, 150, 580, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Balance");
    p->drawLine(520, 150, 520, 165);

    // Reset our pen back to a transparent background and black letters.
    p->setBackgroundMode(Qt::TransparentMode);
    p->setPen(Qt::black);
}

void CollReport::printReport(long CustID)
{
    QDate       theDate;
    QPrinter    prn(QPrinter::PrinterResolution);
    QPainter    p;
    QRect       rect;
    ADBTable    cust;
    ADBTable    cont;
    ADB         DB;
    QString     tmpSt;
    char        tStr[1024];
    int         yPos;
    int         pageNo = 1;
    float       Balance = 0.00;
    float       EndingBalance;
    CustomersDB CDB;
    AddressesDB addrDB;
    
    CDB.get(CustID);
    addrDB.get(REF_CUSTOMER, CustID, "Billing");
    
    theDate = QDate::currentDate();
    
    // prn.setPrintProgram("ghostview");
    prn.setPrinterName("PostScript");
    // prn.setOutputFileName("/home/marc/test.ps");
    // prn.setOutputToFile(TRUE);
    prn.setPageSize(QPrinter::Letter);
    
    prn.setDocName("Collection Report");
    prn.setCreator("Blarg! Online Services, Inc.");
    p.begin(&prn);
    
    EndingBalance = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld", CustID);
    
    // Put the Blarg header and contact information on the page.
    printHeader(&p, &CDB, &addrDB, EndingBalance);
    
    // Put the register header on the page.
    registerHeader(&p);
    
    // Now, get the register information from the database.
    DB.query("select TransDate, DueDate, LoginID, Memo, Amount from AcctsRecv where CustomerID = %ld order by TransDate, LoginID", CustID);
    
    yPos = 165;
    p.setFont(QFont("courier", 8, QFont::Normal));
    while (DB.getrow()) {
        int Lines = (int) (strlen(DB.curRow["Memo"]) / 52) + 1;
        int RowHeight = 15 * Lines;
        
        // Check to see if we have enough room on the page left for this
        // line.
        if (yPos+RowHeight >= 740) {
            printFooter(&p, pageNo++);
            prn.newPage();
            printHeader(&p, &CDB, &addrDB, EndingBalance);
            registerHeader(&p);
            yPos = 165;
            p.setFont(QFont("courier", 8, QFont::Normal));
        } 
    
        // The transaction date.
        rect.setCoords(20, yPos, 79, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, DB.curRow["TransDate"]);
        
        // The Due Date
        rect.setCoords(80, yPos, 139, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, DB.curRow["DueDate"]);
        
        // The Login ID
        rect.setCoords(140, yPos, 199, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, DB.curRow["LoginID"]);
        
        // The description...
        rect.setCoords(200, yPos, 459, yPos + RowHeight);
        p.drawRect(rect);
        rect.setCoords(203, yPos, 459, yPos + RowHeight);
        p.drawText(rect, Qt::WordBreak|Qt::AlignLeft|Qt::AlignVCenter, DB.curRow["Memo"]);
        
        // The amount.
        rect.setCoords(460, yPos, 519, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, DB.curRow["Amount"]);
        
        // The balance.
        Balance += atof(DB.curRow["Amount"]);
        sprintf(tStr, "%.2f", Balance);
        if (Balance == 0.0) strcpy(tStr, "0.00");
        rect.setCoords(520, yPos, 579, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tStr);
        
        yPos += RowHeight;
    }

    // Put the footer on the page.
    printFooter(&p, pageNo);
    
    // prn.newPage();
    
    // p.drawText(300, 600, "Page 2");
    
    p.end();
}



