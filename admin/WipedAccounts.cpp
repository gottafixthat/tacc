/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>

#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3progressbar.h>

#include <AcctsRecv.h>
#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include <EditCustomer.h>
#include <ParseFile.h>
#include <CollReport.h>
#include <Cfg.h>

#include <WipedAccounts.h>

using namespace Qt;

WipedAccounts::WipedAccounts(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
	setCaption( "Overdue Account Processing (Inactive Accounts)" );

    // Create our widgets
    list = new Q3ListView(this, "list");
    list->addColumn("Cust ID");
    list->addColumn("Customer Name");
    list->addColumn("Primary Login");
    list->addColumn("Balance");
    list->setColumnAlignment(3, Qt::AlignRight);
    list->addColumn("Overdue");
    list->setColumnAlignment(4, Qt::AlignRight);
    list->addColumn("Newest");
    list->setColumnAlignment(5, Qt::AlignRight);
	list->setAllColumnsShowFocus(TRUE);
	list->setMultiSelection(TRUE);
    connect(list, SIGNAL(doubleClicked(Q3ListViewItem *)), SLOT(editCustomer(Q3ListViewItem *)));
    connect(list, SIGNAL(returnPressed(Q3ListViewItem *)), SLOT(editCustomer(Q3ListViewItem *)));
    connect(list, SIGNAL(selectionChanged()), SLOT(updateTotals()));

    Q3GroupBox   *grpBox = new Q3GroupBox(2, Qt::Horizontal, "Summary Information", this, "grpBox");

    QLabel *accountsOverdueLabel = new QLabel(grpBox, "accountsOverdueLabel");
    accountsOverdueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    accountsOverdueLabel->setText("Accounts Overdue:");

    accountsOverdue = new QLabel(grpBox, "accountsOverdue");
    accountsOverdue->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    accountsOverdue->setText("");
	
    QLabel *accountsSelectedLabel = new QLabel(grpBox, "accountsSelectedLabel");
    accountsSelectedLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    accountsSelectedLabel->setText("Accounts Selected:");

    accountsSelected = new QLabel(grpBox, "accountsSelected");
    accountsSelected->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    accountsSelected->setText("");
	
    QLabel *selectedOverdueLabel = new QLabel(grpBox, "selectedOverdueLabel");
    selectedOverdueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    selectedOverdueLabel->setText("Total from selected:");

    selectedOverdue = new QLabel(grpBox, "selectedOverdue");
    selectedOverdue->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    selectedOverdue->setText("");

    QLabel *amountOverdueLabel = new QLabel(grpBox, "amountOverdueLabel");
    amountOverdueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    amountOverdueLabel->setText("Total Amount Overdue:");

    amountOverdue = new QLabel(grpBox, "amountOverdue");
    amountOverdue->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    amountOverdue->setText("");

    // Now the button group.
    Q3ButtonGroup *butGrp = new Q3ButtonGroup(1, Qt::Horizontal, "Action", this, "butGrp");
    butGrp->setExclusive(true);
    noAction = new QRadioButton("Non&e", butGrp, "noAction");
    sendFinal = new QRadioButton("&Send Final Notice", butGrp, "sendFinal");
    sendToCollection = new QRadioButton("S&end to collection", butGrp, "sendToCollection");

    // Our action buttons.
    QPushButton *selectNoneButton = new QPushButton(this, "selectNoneButton");
    selectNoneButton->setText("Select &None");
    connect(selectNoneButton, SIGNAL(clicked()), this, SLOT(selectNone()));

    QPushButton *selectAllButton = new QPushButton(this, "selectAllButton");
    selectAllButton->setText("Select &All");
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));

    QPushButton *continueButton = new QPushButton(this, "continueButton");
    continueButton->setText("C&ontinue");
    connect(continueButton, SIGNAL(clicked()), this, SLOT(processSelections()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelOverdue()));

    // Create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);
    ml->addWidget(list, 1);

    // A left to right layout for the two group boxes and the buttons.
    Q3BoxLayout *sl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    sl->addWidget(grpBox, 0);
    sl->addWidget(butGrp, 0);
    sl->addStretch(1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 3);
    bl->addStretch(1);
    bl->addWidget(selectNoneButton, 0);
    bl->addWidget(selectAllButton, 0);
    bl->addWidget(continueButton, 0);
    bl->addWidget(cancelButton, 0);
    sl->addLayout(bl, 0);

    ml->addLayout(sl, 0);

	
    totAccts    = 0;
    selAccts    = 0;
    totOverdue  = 0.0;
    totSelected = 0.0;
    
    myAccountTypes = 0; // AccountTypes;
    	
    fillList();

    QSize   mainSize = sizeHint();
    QSize   listSize = list->sizeHint();
    mainSize.setWidth(listSize.width()+25);
    resize(mainSize);

	show();
    
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
    
    QApplication::setOverrideCursor(Qt::waitCursor);

    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);
    
    emit(setStatus("Searching wiped accounts..."));
    
    DB.query("select CustomerID, FullName, PrimaryLogin, CurrentBalance from Customers where CurrentBalance > 0.00");
    totSteps    = DB.rowCount;
    while (DB.getrow()) {
        emit(setProgress(++curPos, totSteps));
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
    	            (void) new Q3ListViewItem(list, DB.curRow["CustomerID"], DB.curRow["FullName"], DB.curRow["PrimaryLogin"], DB.curRow["CurrentBalance"], tmpBal, newestDue, oldestDue, noteAge);
    	        }
	        }
        }
    }
    
    sprintf(tmpStr, "%ld", totAccts);
    accountsOverdue->setText(tmpStr);
    sprintf(tmpStr, "%.2f", totOverdue);
    amountOverdue->setText(tmpStr);
    
    emit(setStatus(""));
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
    Q3ListViewItem   *curItem;
    
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
    Q3ListViewItem   *curItem;
    
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

void WipedAccounts::editCustomer(Q3ListViewItem *curItem)
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
    
    Q3ListViewItem   *curItem;
    
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
    if (sendFinal->isChecked()) Action = 1;
    if (sendToCollection->isChecked()) Action = 2;
    
    
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
    char        tmpStr[1024];
    
    theDate = QDate::currentDate();
    QDatetomyDate(today, theDate);

    Q3ListViewItem   *curItem;
    long            curCount = 0;

    emit(setStatus("Sending to collections..."));
    
    
    QApplication::setOverrideCursor(Qt::waitCursor);

    // Scan through the list...
    curItem = list->firstChild();
    while (curItem != NULL) {
        emit(setProgress(++curCount, totAccts));
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
            sprintf(tmpStr, "Sent to collection (%s, %s)", cfgVal("CollectionAgency"), cfgVal("CollectionAgencyNumber"));
            AR.ARDB->setValue("Memo", tmpStr);
            AR.SaveTrans();

            // Now, add a note to their account showing that we
            // sent them to collection
            NDB.setValue("NoteID",  0);
            NDB.setValue("LoginID", "");
            NDB.setValue("CustomerID", atol(curItem->key(0, 1)));
            NDB.setValue("NoteType", "Accounting");
            sprintf(tmpStr, "Sent to collection (%s, %s, %s)", cfgVal("CollectionAgency"), cfgVal("CollectionAgencyAddress"), cfgVal("CollectionAgencyNumber"));
            NDB.setValue("NoteText", tmpStr);
            NDB.ins();
        }
        curItem = curItem->itemBelow();
        updateTotals();
    }
    
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();

}


// vim: expandtab
