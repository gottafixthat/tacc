/*
** $Id$
**
***************************************************************************
**
** Tab_BillingInfo - Maintains various billing information for the
**                   customer.
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
** $Log: Tab_BillingInfo.cpp,v $
** Revision 1.3  2004/04/15 18:11:44  marc
** Changed the way that automatic credit cards and pending payments display in the
** Billing tab of the Edit Customer window.  It now shows what day automatic
** payments go through on, and the last four digits of pending payments.  Added
** options to remove pending charges and automatic payment cards in the Payments
** menu option of the Edit Customer window.  Taking payments now checks for
** pending payments and asks the user if they want to remove the pending payment
** before taking the new one.
**
** Revision 1.2  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "Tab_BillingInfo.h"
#include "BlargDB.h"
#include <qstring.h>
#include <qmenubar.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCEntry.h>
#include <CCRefund.h>
#include "ChangeRatePlan.h"
#include "ChangeBillingCycle.h"
#include "StatementView.h"
#include "ExtendGraceDate.h"
#include "CustRegister.h"
#include <BString.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include <TAATools.h>
#include <ADB.h>
#include <StatementEngine.h>

Tab_BillingInfo::Tab_BillingInfo
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
    
	if (!CustID) return;
    recountAllowedMailboxes(CustID);
	myCustID	= CustID;
	myParent    = parent;
	
	setCaption( "Billing Info" );


    // Create all of our widgets
    QLabel  *ratePlanLabel = new QLabel(this);
    ratePlanLabel->setText("Rate Plan:");
    ratePlanLabel->setAlignment(AlignRight|AlignVCenter);

    ratePlan = new QLabel(this, "Rate Plan");
    ratePlan->setText("");
    ratePlan->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *billingCycleLabel = new QLabel(this);
    billingCycleLabel->setText("Billing Cycle:");
    billingCycleLabel->setAlignment(AlignRight|AlignVCenter);

    billingCycle = new QLabel(this, "Billing Cycle");
    billingCycle->setText("");
    billingCycle->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *autoPaymentLabel = new QLabel(this);
    autoPaymentLabel->setText("Auto Payment:");
    autoPaymentLabel->setAlignment(AlignRight|AlignVCenter);

    autoPayment = new QLabel(this, "Auto Payment");
    autoPayment->setText("");
    autoPayment->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *ratePlanDateLabel = new QLabel(this);
    ratePlanDateLabel->setText("Rate Plan Date:");
    ratePlanDateLabel->setAlignment(AlignRight|AlignVCenter);

    ratePlanDate = new QLabel(this, "Rate Plan Date");
    ratePlanDate->setText("");
    ratePlanDate->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *termsLabel = new QLabel(this);
    termsLabel->setText("Terms:");
    termsLabel->setAlignment(AlignRight|AlignVCenter);

    terms = new QLabel(this, "Terms");
    terms->setText("");
    terms->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *pendingChargeLabel = new QLabel(this);
    pendingChargeLabel->setText("Pending Payment:");
    pendingChargeLabel->setAlignment(AlignRight|AlignVCenter);

    pendingCharge = new QLabel(this, "Pending Payment");
    pendingCharge->setText("");
    pendingCharge->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *accountOpenedLabel = new QLabel(this);
    accountOpenedLabel->setText("Account Opened:");
    accountOpenedLabel->setAlignment(AlignRight|AlignVCenter);

    accountOpened = new QLabel(this, "Account Opened");
    accountOpened->setText("");
    accountOpened->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *accountExpiresLabel = new QLabel(this);
    accountExpiresLabel->setText("Account Expires:");
    accountExpiresLabel->setAlignment(AlignRight|AlignVCenter);

    accountExpires = new QLabel(this, "Account Expires");
    accountExpires->setText("");
    accountExpires->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *mailboxTallyLabel = new QLabel(this);
    mailboxTallyLabel->setText("Mailboxes:");
    mailboxTallyLabel->setAlignment(AlignRight|AlignVCenter);

    mailboxTally = new QLabel(this, "mailboxTally");
    mailboxTally->setText("");
    mailboxTally->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *accountClosedLabel = new QLabel(this);
    accountClosedLabel->setText("Account Closed:");
    accountClosedLabel->setAlignment(AlignRight|AlignVCenter);

    accountClosed = new QLabel(this, "Account Closed");
    accountClosed->setText("");
    accountClosed->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *accountReOpenedLabel = new QLabel(this);
    accountReOpenedLabel->setText("Account Re-opened:");
    accountReOpenedLabel->setAlignment(AlignRight|AlignVCenter);

    accountReOpened = new QLabel(this, "Account Re-Opened");
    accountReOpened->setText("");
    accountReOpened->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *activeLabel = new QLabel(this);
    activeLabel->setText("Account Active:");
    activeLabel->setAlignment(AlignRight|AlignVCenter);

    active = new QLabel(this, "Account Active");
    active->setText("");
    active->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *graceEndsLabel = new QLabel(this);
    graceEndsLabel->setText("Account Active:");
    graceEndsLabel->setAlignment(AlignRight|AlignVCenter);

    graceEnds = new QLabel(this, "Account Active");
    graceEnds->setText("");
    graceEnds->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *statementTypeLabel = new QLabel(this);
    statementTypeLabel->setText("Statement Type:");
    statementTypeLabel->setAlignment(AlignRight|AlignVCenter);

    statementType = new QLabel(this, "Statement Type");
    statementType->setText("");
    statementType->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *lastBilledLabel = new QLabel(this);
    lastBilledLabel->setText("Last Billed Date:");
    lastBilledLabel->setAlignment(AlignRight|AlignVCenter);

    lastBilled = new QLabel(this, "Last Billed Date");
    lastBilled->setText("");
    lastBilled->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *lastStatementNoLabel = new QLabel(this);
    lastStatementNoLabel->setText("Last Statement:");
    lastStatementNoLabel->setAlignment(AlignRight|AlignVCenter);

    lastStatementNo = new QLabel(this, "Last Statement");
    lastStatementNo->setText("");
    lastStatementNo->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *lastDueDateLabel = new QLabel(this);
    lastDueDateLabel->setText("Due Date:");
    lastDueDateLabel->setAlignment(AlignRight|AlignVCenter);

    lastDueDate = new QLabel(this, "Due Date");
    lastDueDate->setText("");
    lastDueDate->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *currentBalanceLabel = new QLabel(this);
    currentBalanceLabel->setText("Current Balance:");
    currentBalanceLabel->setAlignment(AlignRight|AlignVCenter);

    currentBalance = new QLabel(this, "Current Balance");
    currentBalance->setText("");
    currentBalance->setAlignment(AlignLeft|AlignVCenter);

    stList = new QListView(this);
    stList->addColumn("Number");
    stList->addColumn("Date");
    stList->addColumn("Due Date");
    stList->addColumn("Sent To");
    stList->addColumn("P");
    stList->setAllColumnsShowFocus(TRUE);
    stList->setItemMargin( 2 );
    stList->setSorting(0, false);   // Reverse order sort on statement number
    connect(stList, SIGNAL(doubleClicked( QListViewItem *)), SLOT(viewStatement(QListViewItem *)));

    // Create our layout.  The main widget will be in a box, then two grids
    // will be displayed, the first containing the basic billing information,
    // the second grid displaying the account dates, then a another box
    // will be created containing a grid holding the last statement information
    // and on the right will be the list of statements.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // The first grid.  This holds the billing information.
    QGridLayout *bigrid = new QGridLayout(3, 4, 3);
    bigrid->setColStretch(0, 0);
    bigrid->setColStretch(1, 1);
    bigrid->setColStretch(2, 0);
    bigrid->setColStretch(3, 1);
    
    int curRow = 0;
    bigrid->setRowStretch(curRow, 0);
    bigrid->addWidget(ratePlanLabel,        curRow, 0);
    bigrid->addWidget(ratePlan,             curRow, 1);
    bigrid->addWidget(ratePlanDateLabel,    curRow, 2);
    bigrid->addWidget(ratePlanDate,         curRow, 3);
    curRow++;

    bigrid->setRowStretch(curRow, 0);
    bigrid->addWidget(billingCycleLabel,    curRow, 0);
    bigrid->addWidget(billingCycle,         curRow, 1);
    bigrid->addWidget(termsLabel,           curRow, 2);
    bigrid->addWidget(terms,                curRow, 3);
    curRow++;

    bigrid->setRowStretch(curRow, 0);
    bigrid->addWidget(autoPaymentLabel,     curRow, 0);
    bigrid->addMultiCellWidget(autoPayment, curRow, curRow, 1, 3);
    //bigrid->addWidget(pendingChargeLabel,   curRow, 2);
    //bigrid->addWidget(pendingCharge,        curRow, 3);
    curRow++;

    bigrid->setRowStretch(curRow, 0);
    bigrid->addWidget(pendingChargeLabel,    curRow, 0);
    bigrid->addMultiCellWidget(pendingCharge,curRow, curRow, 1, 3);
    curRow++;


    // Add it to the main layout
    ml->addLayout(bigrid, 0);
    ml->addWidget(new HorizLine(this), 0);

    // The second grid.  This holds various dates and other info
    QGridLayout *dtgrid = new QGridLayout(2, 6, 3);
    dtgrid->setColStretch(0, 0);
    dtgrid->setColStretch(1, 1);
    dtgrid->setColStretch(2, 0);
    dtgrid->setColStretch(3, 1);
    dtgrid->setColStretch(4, 0);
    dtgrid->setColStretch(5, 1);

    curRow = 0;
    dtgrid->setRowStretch(curRow, 0);
    dtgrid->addWidget(accountOpenedLabel,   curRow, 0);
    dtgrid->addWidget(accountOpened,        curRow, 1);
    dtgrid->addWidget(accountClosedLabel,   curRow, 2);
    dtgrid->addWidget(accountClosed,        curRow, 3);
    dtgrid->addWidget(accountReOpenedLabel, curRow, 4);
    dtgrid->addWidget(accountReOpened,      curRow, 5);
    curRow++;

    dtgrid->setRowStretch(curRow, 0);
    dtgrid->addWidget(accountExpiresLabel,  curRow, 0);
    dtgrid->addWidget(accountExpires,       curRow, 1);
    dtgrid->addWidget(activeLabel,          curRow, 2);
    dtgrid->addWidget(active,               curRow, 3);
    dtgrid->addWidget(graceEndsLabel,       curRow, 4);
    dtgrid->addWidget(graceEnds,            curRow, 5);
    curRow++;

    dtgrid->setRowStretch(curRow, 0);
    dtgrid->addWidget(mailboxTallyLabel,    curRow, 0);
    dtgrid->addWidget(mailboxTally,         curRow, 1);
    curRow++;


    // Add it to the main layout
    ml->addLayout(dtgrid, 0);
    ml->addWidget(new HorizLine(this), 0);

    // Now we create another box, left to right, to hold the statement
    // info.  We'll put a grid in that box for the "highlights" and then
    // leave the statement list by itself.
    QBoxLayout *stbl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    QBoxLayout *stgbox = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    QGridLayout *stgrid = new QGridLayout(4, 2, 3);
    stgrid->setColStretch(0, 0);
    stgrid->setColStretch(1, 1);
    
    curRow = 0;
    stgrid->setRowStretch(curRow, 0);
    stgrid->addWidget(statementTypeLabel,   curRow, 0);
    stgrid->addWidget(statementType,        curRow, 1);
    curRow++;

    stgrid->setRowStretch(curRow, 0);
    stgrid->addWidget(lastBilledLabel,      curRow, 0);
    stgrid->addWidget(lastBilled,           curRow, 1);
    curRow++;
    
    stgrid->setRowStretch(curRow, 0);
    stgrid->addWidget(lastStatementNoLabel, curRow, 0);
    stgrid->addWidget(lastStatementNo,      curRow, 1);
    curRow++;
    
    stgrid->setRowStretch(curRow, 0);
    stgrid->addWidget(lastDueDateLabel,     curRow, 0);
    stgrid->addWidget(lastDueDate,          curRow, 1);
    curRow++;
    
    stgrid->setRowStretch(curRow, 0);
    stgrid->addWidget(currentBalanceLabel,  curRow, 0);
    stgrid->addWidget(currentBalance,       curRow, 1);
    curRow++;

    stgbox->addLayout(stgrid, 0);
    stgbox->addStretch(1);

    stbl->addLayout(stgbox, 0);
    stbl->addWidget(stList, 1);

    ml->addLayout(stbl, 1);

    // All Done with the layouts.
    
	refreshBillingInfo(myCustID);

    connect(this, SIGNAL(customerChanged(long)), mainWin(), SLOT(customerChanged(long)));
    connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
}


Tab_BillingInfo::~Tab_BillingInfo()
{
}

/*
** refreshBillingInfo - A public slot that can be called to refresh things
**                      like the current balance, last billed on, etc.
*/

void Tab_BillingInfo::refreshBillingInfo(int)
{
	ADB         DB;
	CustomersDB cdb;
	PackagesDB  pdb;
	RatePlansDB rpdb;
	PaymentTermsDB ptdb;
	BillingCyclesDB bcdb;
	StatementsDB    STDB;
	QString tmpqstr;
	QString tmpqstr2;
	char    tmpstr[1024];
	QDate   tmpDate;
	
    recountAllowedMailboxes(myCustID);
	
	cdb.get(myCustID);
/*	pdb.get(cdb.Package.toInt()); */
	rpdb.get(cdb.getLong("RatePlan"));
	ptdb.get(cdb.getInt("Terms"));
	bcdb.get(cdb.getLong("BillingCycle"));

	tmpqstr = (const char *) rpdb.getStr("PlanTag");
	tmpqstr.append(" - ");
	tmpqstr.append((const char *) rpdb.getStr("Description"));
	ratePlan->setText((const char *) tmpqstr);
	ratePlanDate->setText((const char *) cdb.getStr("RatePlanDate"));
	
	tmpqstr = (const char *) bcdb.CycleID;
	tmpqstr.append(" - ");
	tmpqstr.append((const char *) bcdb.Description);
	billingCycle->setText((const char *) tmpqstr);
	
	terms->setText((const char *) ptdb.getStr("TermsDesc"));
	
	graceEnds->setText((const char *) cdb.getDate("GraceDate").toString());
	
	sprintf(tmpstr, "$%.2f", cdb.getFloat("CurrentBalance"));
	currentBalance->setText(tmpstr);
	if (cdb.getLong("LastStatementNo")) {
    	lastStatementNo->setText((const char *) cdb.getStr("LastStatementNo"));
	    STDB.get(cdb.getLong("LastStatementNo"));
    	lastBilled->setText((const char *) cdb.getDate("LastBilled").toString());
	    lastDueDate->setText(STDB.getDate("DueDate").toString());
	} else {
    	lastBilled->setText("N/A");
	    lastStatementNo->setText("N/A");
	    lastDueDate->setText("N/A");
	}
	
	
	accountOpened->setText((const char *) cdb.getStr("AccountOpened"));
	accountClosed->setText((const char *) cdb.getStr("AccountClosed"));
	accountReOpened->setText((const char *) cdb.getStr("AccountReOpened"));
	
	accountExpires->setText((const char *) cdb.getStr("AccountExpires"));
    if (cdb.getInt("Active")) active->setText("Yes");
    else active->setText("No");
	
    // Check for automatic payments...
    DB.query("select InternalID from AutoPayments where CustomerID = %ld and Active <> 0", myCustID);
    if (DB.rowCount) {
        AutoPaymentsDB  apdb;
        int             tmpCardType;
        char            cardType[1024];
        DB.getrow();
        apdb.get(atol(DB.curRow["InternalID"]));
        
        sprintf(tmpstr, "Yes, type %d", apdb.getInt("PaymentType"));
        // Check to see if this is an ECheck or a credit card
        if (apdb.getInt("PaymentType") == 1) {
            // Credit card
            tmpCardType = atoi(apdb.getStr("CardType"));
            switch (tmpCardType) {
                case CCTYPE_VISA:
                    sprintf(cardType, "Visa");
                    break;
                case CCTYPE_MC:
                    sprintf(cardType, "MasterCard");
                    break;
                case CCTYPE_DISC:
                    sprintf(cardType, "Discover");
                    break;
                case CCTYPE_AMEX:
                    sprintf(cardType, "AmEx");
                    break;
                case CCTYPE_DINERS:
                    sprintf(cardType, "Diners Club");
                    break;
                    
                default:
                    sprintf(cardType, "Unknown Credit Card");
                    break;
            }
            
            tmpqstr = apdb.getStr("AcctNo");
            tmpqstr2 = tmpqstr.right(4);
            tmpqstr  = "xxxx...";
            tmpqstr.append(tmpqstr2);
            
            sprintf(tmpstr, "%s (%s), Exp %s, Charged on the %dth",
              cardType,
              (const char *) tmpqstr,
              (const char *) apdb.getStr("ExpDate"),
              apdb.getInt("ChargeDay") + 2
            );
        }
        
    } else {
        sprintf(tmpstr, "None specified");
    }    
    autoPayment->setText(tmpstr);

    // Check for any pending charges (from auto payments)
    DB.query("select InternalID from CCTrans where CustomerID = %ld and BPSExported = 0", myCustID);
    if (DB.rowCount) {
        CCTransDB   CCTDB;
        char        cardType[64];
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
        
            tmpqstr = CCTDB.getStr("CardNo");
            tmpqstr2 = tmpqstr.right(4);
            tmpqstr  = "xxxx...";
            tmpqstr.append(tmpqstr2);
            
        myDatetoQDate((const char *) CCTDB.getStr("TransDate"), &tmpDate);
        sprintf(tmpstr, "%s (%s) for $%.2f on %s", 
          cardType,
          (const char *) tmpqstr,
          CCTDB.getFloat("Amount"),
          (const char *) tmpDate.toString()
        );
        
    } else {
        sprintf(tmpstr, "None");
    }
    pendingCharge->setText(tmpstr);

	if (cdb.getInt("PrintedStatement")) {
		if (cdb.getInt("FreePrintedStatement")) {
		    statementType->setText("Printed (free)");
		} else {
		    statementType->setText("Printed (charge)");
		}
	} else {
	    statementType->setText("Emailed Only");
	}

    // Mailbox counts
    char tmpStr[1024];
    DB.query("select LoginID from Logins where CustomerID = %ld and Active <> 0", myCustID);
    sprintf(tmpStr, "%ld/%d", DB.rowCount, cdb.getInt("MailboxesAllowed"));
    mailboxTally->setText(tmpStr);
	
    fillStatementList();
}

/*
** changeRatePlan - Brings up a window allowing the user to alter the
**                  user's rate plan.
*/

void Tab_BillingInfo::changeRatePlan()
{
    ChangeRatePlan  *CRP;
    
    CRP = new ChangeRatePlan(0, 0, myCustID);
    CRP->show();
}


/*
** changeBillingCycle - Brings up a window allowing the user to alter the
**                  user's rate plan.
*/

void Tab_BillingInfo::changeBillingCycle()
{
    ChangeBillingCycle  *CBC;
    
    CBC = new ChangeBillingCycle(0, 0, myCustID);
	connect(CBC, SIGNAL(refreshCust(int)), myParent, SLOT(refreshAll(int)));
    CBC->show();
}

/*
** generateStatement - Crteates a statement for this customer.
*/

void Tab_BillingInfo::generateStatement()
{
    long StNo;
    StatementEngine stEng;
    
    StNo = stEng.doStatement(myCustID);
    stEng.emailStatement(StNo, 0, 0);
    stEng.printStatement(StNo, 0, 0);
	refreshBillingInfo(myCustID);
}

/*
** runSubscriptions - Runs subscriptions for this customer.
*/

void Tab_BillingInfo::runSubscriptions()
{
    QApplication::setOverrideCursor(waitCursor);
    CustomersDB     CDB;
    CDB.get(myCustID);
    CDB.doSubscriptions();
    QApplication::restoreOverrideCursor();
    emit(customerChanged(myCustID));
}


/*
** TakeCCard  - Brings up a window allowing the user to take a credit card
**              payment.
**
*/

void Tab_BillingInfo::takeCCard()
{
	CCEntry		    *CCE;
	CustomersDB		CDB;
	int				takeCCard = 0;
	
	CDB.get(myCustID);
	if (!CDB.getFloat("CurrentBalance")) {
        if (QMessageBox::warning(this, "Cutomer Care", "The customer's balance is zero.\nDo you still want to take a credit card payment?", "&Yes", "&No", 0, 1) == 0) takeCCard = 1;
	} else {
		takeCCard	= 1;
	}

	// Check for automatic payments.
	if (takeCCard) {
	    ADB         DB;
    	DB.query("select * from AutoPayments where CustomerID = %ld and Active <> 0", myCustID);
	    if (DB.rowCount) {
            if (QMessageBox::warning(this, "Cutomer Care", "This customer is setup for automatic payments.\nDo you still want to take a credit card payment?", "&Yes", "&No", 0, 1) == 0) takeCCard = 1;
            else takeCCard = 0;
    	}
        
        // Check for current pending payments
    	DB.query("select * from CCTrans where CustomerID = %ld and BPSExported = 0", myCustID);
	    if (DB.rowCount) {
            DB.getrow();
            long    tmpIntID = atol(DB.curRow["InternalID"]);
            if (QMessageBox::warning(this, "Cutomer Care", "A pending credit card payment was found.\nDo you wish to remove it before taking the new payment?","&Yes", "&No", 0, 1) == 0) {
                DB.dbcmd("delete from CCTrans where InternalID = %ld", tmpIntID);
                emit(customerChanged(myCustID));
            }
    	}
    }
	
	if (takeCCard) {
		CCE = new CCEntry(0, "", myCustID);
		CCE->show();
	}
}

/*
** issueCardRefund - Brings up a window allowing the user to issue a credit
**                   card refund payment.
**
*/
void Tab_BillingInfo::issueCardRefund()
{
    CCRefund    *CCR;
    CCR = new CCRefund();
    CCR->setCustomerID(myCustID);
}


/*
** fillStatementList  - Fills in the statement list.
*/

void Tab_BillingInfo::fillStatementList(void)
{
    ADB     DB;
    
    DB.query("select StatementNo, StatementDate, DueDate, EmailedTo from Statements where CustomerID = %ld order by StatementDate desc", myCustID);

    stList->clear();
    while (DB.getrow()) {
        (void) new QListViewItem(stList,
          DB.curRow["StatementNo"],
          DB.curRow["StatementDate"],
          DB.curRow["DueDate"],
          DB.curRow["EmailedTo"]
        );
    }
    
}

/*
** viewStatement - Displays a statement on the screen.
*/

void Tab_BillingInfo::viewStatement(QListViewItem *curItem)
{
    if (curItem != NULL) {
        StatementView   *SV;
        SV = new StatementView();
        SV->show();
        SV->loadStatement(atol(curItem->key(0, TRUE)));
    }
}


/*
** togglePrintedStatements  - Updates the database with the new printed
**                            statement value.
*/

void Tab_BillingInfo::togglePrintedStatements()
{
    CustomersDB     tmpCDB;
    tmpCDB.get(myCustID);
    if (tmpCDB.getInt("PrintedStatement")) {
        tmpCDB.setValue("PrintedStatement", (int) 0);
    } else {
        tmpCDB.setValue("PrintedStatement", (int) 1);
    }
    tmpCDB.upd();
    emit(customerChanged(myCustID));
}

/*
** toggleFreePrintedStatements  - Updates the database with the new printed
**                                statement value.
*/

void Tab_BillingInfo::toggleFreePrintedStatements()
{
    CustomersDB     tmpCDB;
    tmpCDB.get(myCustID);
    if (tmpCDB.getInt("FreePrintedStatement")) {
        tmpCDB.setValue("FreePrintedStatement", (int) 0);
    } else {
        tmpCDB.setValue("FreePrintedStatement", (int) 1);
    }
    tmpCDB.upd();
    emit(customerChanged(myCustID));
}

/*
** extendGracePeriod  - Allows the user to extend the customers grace period
**                      by up to 7 days.
*/

void Tab_BillingInfo::extendGracePeriod()
{
    ExtendGraceDate     *EGD;
    EGD = new ExtendGraceDate(this, NULL, myCustID);
    EGD->exec();
}

/*
** showCustRegister  - Creates a new CustRegister widget which contains
**                     the customers register.
*/

void Tab_BillingInfo::showCustRegister()
{
    if (myCustID) {
        CustRegister    *CR;
        CR = new CustRegister(0, NULL, myCustID);
        CR->show();
    }
}


void Tab_BillingInfo::refreshCustomer(long custID)
{
    if (custID == myCustID) refreshBillingInfo(custID);
}

/*
** cancelPendingCharge - Cancels a pending credit card charge.
*/

void Tab_BillingInfo::cancelPendingCharge()
{
    // First, check to see if there is one.
    ADB     DB;

    DB.query("select InternalID from CCTrans where CustomerID = %ld and BPSExported = 0", myCustID);
    if (!DB.rowCount) {
        QMessageBox::information(this, "Cutomer Care", "No pending charges were found.");
        return;
    }

    CCTransDB   CCTDB;
    char        cardType[64];
    QString     tmpqstr;
    QString     tmpqstr2;
    QDate       tmpDate;
    char        tmpstr[1024];
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
    
    tmpqstr = CCTDB.getStr("CardNo");
    tmpqstr2 = tmpqstr.right(4);
    tmpqstr  = "xxxx...";
    tmpqstr.append(tmpqstr2);
        
    myDatetoQDate((const char *) CCTDB.getStr("TransDate"), &tmpDate);
    sprintf(tmpstr, "Are you sure you wish to remove the\n%s (%s) payment\nfor $%.2f on %s?", 
      cardType,
      (const char *) tmpqstr,
      CCTDB.getFloat("Amount"),
      (const char *) tmpDate.toString()
    );
    
    if (QMessageBox::warning(this, "Cutomer Care", tmpstr, "&Yes", "&No", 0, 1) == 0) {
        // Go ahead and remove it.
        CCTDB.del();
        emit(customerChanged(myCustID));
    }

    // pendingCharge->setText(tmpstr);
}

/*
** removeAutoPaymentCard - Removes the automatic payment card the user has.
*/

void Tab_BillingInfo::removeAutoPaymentCard()
{
    ADB     DB;
    // First, check to see if they have an automatic payment setup.
    DB.query("select InternalID from AutoPayments where CustomerID = %ld and Active <> 0", myCustID);
    if (!DB.rowCount) {
        QMessageBox::information(this, "Cutomer Care", "No automatic payment card was found.");
        return;
    }

    AutoPaymentsDB  apdb;
    int             tmpCardType;
    char            cardType[1024];
    QString         tmpqstr;
    QString         tmpqstr2;
    QDate           tmpDate;
    char            tmpstr[1024];
    DB.getrow();
    apdb.get(atol(DB.curRow["InternalID"]));
    
    sprintf(tmpstr, "Yes, type %d", apdb.getInt("PaymentType"));
    // Check to see if this is an ECheck or a credit card
    // Credit card
    tmpCardType = atoi(apdb.getStr("CardType"));
    switch (tmpCardType) {
        case CCTYPE_VISA:
            sprintf(cardType, "Visa");
            break;
        case CCTYPE_MC:
            sprintf(cardType, "MasterCard");
            break;
        case CCTYPE_DISC:
            sprintf(cardType, "Discover");
            break;
        case CCTYPE_AMEX:
            sprintf(cardType, "AmEx");
            break;
        case CCTYPE_DINERS:
            sprintf(cardType, "Diners Club");
            break;
            
        default:
            sprintf(cardType, "Unknown Credit Card");
            break;
    }
    
    tmpqstr = apdb.getStr("AcctNo");
    tmpqstr2 = tmpqstr.right(4);
    tmpqstr  = "xxxx...";
    tmpqstr.append(tmpqstr2);
    
    sprintf(tmpstr, "Are you sure you wish to remove the\n%s (%s), Exp %s\nthat is automatically charged on the %dth\nof each month?",
      cardType,
      (const char *) tmpqstr,
      (const char *) apdb.getStr("ExpDate"),
      apdb.getInt("ChargeDay") + 2
    );
    
    
    if (QMessageBox::warning(this, "Cutomer Care", tmpstr, "&Yes", "&No", 0, 1) == 0) {
        // Go ahead and remove it.
        apdb.del();
        emit(customerChanged(myCustID));
    }
        
}
