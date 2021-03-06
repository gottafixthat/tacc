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

#include <stdio.h>
#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <QtGui/QTabBar>
#include <QtGui/QTabWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3PopupMenu>
#include <Qt3Support/q3accel.h>

#include <ADB.h>

#include "EditCustomer.h"
#include "ReceivePayment.h"
#include "Tab_ContactInfo.h"
#include "Tab_BillingInfo.h"
#include "CustSubscrList.h"
#include "Tab_Logins.h"
#include "Tab_Domains.h"
#ifdef EXPERIMENTAL
#include "Tab_VoIP.h"
#endif
#include "Tab_Notes.h"
// #include "Tab_Accounts.h"
#include "BlargDB.h"
#include <TAATools.h>
#include <TAAWidget.h>
#include <CreateCustTicket.h>

using namespace Qt;

EditCustomer::EditCustomer
(
	QWidget* parent,
	const char* name,
	long CustID
)
	: TAAWidget(parent)
{
	ADB DB;
	char tmpstr[1024];
	
	setCaption( "Edit Customer" );

    customerMenu = new QMenuBar(this, "Customer Menu");

    tabBar = new QTabBar(this);
    tabBar->setMinimumSize(0, 25);
    tabBar->setMaximumSize(4096, 25);
    tabBar->setShape(QTabBar::RoundedAbove);
    tabBar->addTab("Contact Info");
    tabBar->addTab("Billing Info");
    tabBar->addTab("Subscriptions");
    tabBar->addTab("Logins");
    tabBar->addTab("Domains");

#ifdef EXPERIMENTAL
    tabBar->addTab("VoIP");
#endif

    tabBar->addTab("Notes");

    connect(tabBar, SIGNAL(selected(int)), this, SLOT(tabSelected(int)));
    // Setup our hot keys for tabbed windows, Ctrl 1-6 to raise a tab.
    Q3Accel  *ac = new Q3Accel(this);
    ac->connectItem(ac->insertItem(CTRL+Key_1), this, SLOT(raiseTab1()));
    ac->connectItem(ac->insertItem(CTRL+Key_2), this, SLOT(raiseTab2()));
    ac->connectItem(ac->insertItem(CTRL+Key_3), this, SLOT(raiseTab3()));
    ac->connectItem(ac->insertItem(CTRL+Key_4), this, SLOT(raiseTab4()));
    ac->connectItem(ac->insertItem(CTRL+Key_5), this, SLOT(raiseTab5()));
    ac->connectItem(ac->insertItem(CTRL+Key_6), this, SLOT(raiseTab6()));
    ac->connectItem(ac->insertItem(CTRL+Key_7), this, SLOT(raiseTab7()));


    qws = new Q3WidgetStack(this, "Widget Stack");

    QLabel *hline1 = new QLabel(this);
    hline1->setMinimumSize(0, 3);
    hline1->setMaximumSize(32767, 3);
    hline1->setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    hline1->setLineWidth(1);

    QLabel *hline2 = new QLabel(this);
    hline2->setMinimumSize(0, 3);
    hline2->setMaximumSize(32767, 3);
    hline2->setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    hline2->setLineWidth(1);

    saveButton = new QPushButton(this, "Save Button");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCustomer()));

    cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelChanges()));

   
    // Layout the widgets now
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 0, 0);
    ml->addStrut(0);
    ml->addWidget(customerMenu, 1);
    ml->addWidget(tabBar, 1);
    ml->addWidget(hline1, 1);
    ml->addWidget(qws, 1);
    ml->addWidget(hline2, 1);
    ml->addSpacing(3);
    
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);
    ml->addSpacing(3);
    
	
	if (!CustID) return;
	
	myCustID = CustID;

	QApplication::setOverrideCursor(waitCursor);
	
	DB.query("select CustomerID, FullName from Customers where CustomerID = %ld", CustID);
	DB.getrow();
	sprintf(tmpstr, "Edit Customer - %s (%s)", DB.curRow["FullName"], DB.curRow["CustomerID"]);
	setCaption(tmpstr);
	
    int curTab = 0;
    int totTabs = 7;
    //fprintf(stderr, "Creating Tab_ContactInfo...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	contactInfoTab = new Tab_ContactInfo(qws, "Contact Info", CustID);
	qws->addWidget(contactInfoTab, curTab++);

    //fprintf(stderr, "Creating Tab_BillingInfo...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	billingInfoTab = new Tab_BillingInfo(qws, "Billing Info", CustID);
	qws->addWidget(billingInfoTab, curTab++);
	
    //fprintf(stderr, "Creating Tab_Subscriptions...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	subscrsTab = new CustSubscrList(qws, "Subscriptions", CustID);
	qws->addWidget(subscrsTab, curTab++);
	
    //fprintf(stderr, "Creating Tab_Logins...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	loginsTab = new Tab_Logins(qws, "Logins", CustID);
	qws->addWidget(loginsTab, curTab++);
	
    //fprintf(stderr, "Creating Tab_Domains...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	domainsTab = new Tab_Domains(qws, "Domains", CustID);
	qws->addWidget(domainsTab, curTab++);
	
#ifdef EXPERIMENTAL
    //fprintf(stderr, "Creating Tab_VoIP...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	voipTab = new Tab_VoIP(qws, "VoIP", CustID);
	qws->addWidget(voipTab, curTab++);
#endif
	
    //fprintf(stderr, "Creating Tab_Notes...\n"); fflush(stderr);
    emit(setProgress(curTab+1, totTabs));
	notesTab = new Tab_Notes(qws, "Notes", CustID);
	qws->addWidget(notesTab, curTab++);
	
    /*
   	accountsTab = new Tab_Accounts(qws, "Accounts", CustID);
    qws->addWidget(accountsTab, 6);
    */

	// billingInfoTab->raise();
	// billingInfoTab->refreshBillingInfo();
	
    /*
	this->setOKButton("&Save");
	this->setCancelButton("Cancel");
    */
    
    // Create our menu.
    Q3PopupMenu  *fileMenu = new Q3PopupMenu();
    fileMenu->insertItem("&Save",   this, SLOT(saveCustomer()));
    fileMenu->insertItem("&Cancel", this, SLOT(close()));

    Q3PopupMenu  *viewMenu = new Q3PopupMenu();
    viewMenu->insertItem("&Register",   billingInfoTab, SLOT(showCustRegister()), ALT+Key_R);
    
    Q3PopupMenu  *payMenu = new Q3PopupMenu();
    payMenu->insertItem("Take &Credit Card", billingInfoTab, SLOT(takeCCard()), CTRL+Key_C);
    payMenu->insertItem("Cancel &Pending Charge", billingInfoTab, SLOT(cancelPendingCharge()));
    payMenu->insertItem("Remove &Automatic Payment Card", billingInfoTab, SLOT(removeAutoPaymentCard()));
    if (isManager()) payMenu->insertItem("&Issue Credit Card Refund", billingInfoTab, SLOT(issueCardRefund()));
    payMenu->insertItem("Extend &Grace Period", billingInfoTab, SLOT(extendGracePeriod()), CTRL+Key_G);
    payMenu->insertItem("Receive Check", this, SLOT(receivePayment()), CTRL+Key_R);

    Q3PopupMenu  *ticketMenu = new Q3PopupMenu();
    ticketMenu->insertItem("&Create Ticket",   this, SLOT(createTicket()));

    customerMenu->insertItem("&File", fileMenu);
    customerMenu->insertItem("&Payments", payMenu);
    customerMenu->insertItem("&View", viewMenu);
    customerMenu->insertItem("&Tickets", ticketMenu);
    
    if (isManager()) {
        Q3PopupMenu  *admMenu = new Q3PopupMenu();
        admMenu->insertItem("Change &Rate Plan", billingInfoTab, SLOT(changeRatePlan()));
        admMenu->insertItem("Change &Billing Cycle", billingInfoTab, SLOT(changeBillingCycle()));
        admMenu->insertItem("&Run Subscriptions", billingInfoTab, SLOT(runSubscriptions()));
        admMenu->insertItem("&Process a Statement", billingInfoTab, SLOT(generateStatement()));

        stMenu = new Q3PopupMenu();
        printedStatementID     = stMenu->insertItem("&Printed Statements", billingInfoTab, SLOT(togglePrintedStatements()));
        stMenu->setCheckable(TRUE);
        freePrintedStatementID = stMenu->insertItem("&Free Printed Statements", billingInfoTab, SLOT(toggleFreePrintedStatements()));
        stMenu->setCheckable(TRUE);
        admMenu->insertItem("Printed &Statements", stMenu);
        
        customerMenu->insertItem("Adm&in", admMenu);
    }

	
    connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));

    loadCustInfo();
	
    tabBar->setFocus();
    emit(setProgress(8, 8));

    resize(600,400);
    // connect(this, SIGNAL(applyButtonPressed()), SLOT(saveCustomer()));
	QApplication::restoreOverrideCursor();
}


EditCustomer::~EditCustomer()
{
}


/*
** saveCustomer  -	Saves any changes made to any fields and closes the 
**					dialog.
**
*/

void EditCustomer::saveCustomer()
{
	if (contactInfoTab->saveContactInfo()) {
		emit(refreshCustList());
		close();
	}
}


void EditCustomer::refreshAll(int)
{
	subscrsTab->refreshCustomer(myCustID);
#ifdef EXPERIMENTAL
	voipTab->refreshVoIPList(myCustID);
#endif
	notesTab->refreshNotesList(myCustID);
	billingInfoTab->refreshBillingInfo(myCustID);
	emit(refreshCustList());
}

/*
** tabSelected - Raise the specified tab in our QWidgetStack.
*/

void EditCustomer::tabSelected(int newTab)
{
    qws->raiseWidget(newTab);
}

/*
 * raiseTabN - Raises the tab specified.  These are for accelerator
 *             keys.
 *
*/

void EditCustomer::raiseTab1() { tabBar->setCurrentTab(0); }
void EditCustomer::raiseTab2() { tabBar->setCurrentTab(1); }
void EditCustomer::raiseTab3() { tabBar->setCurrentTab(2); }
void EditCustomer::raiseTab4() { tabBar->setCurrentTab(3); }
void EditCustomer::raiseTab5() { tabBar->setCurrentTab(4); }
void EditCustomer::raiseTab6() { tabBar->setCurrentTab(5); }
void EditCustomer::raiseTab7() { tabBar->setCurrentTab(6); }

/*
** receivePayment - Records a check payment from the user.
*/

void EditCustomer::receivePayment()
{
    ReceivePayment *recPay;
    recPay = new ReceivePayment(0, "", myCustID);
    recPay->show();
}

/*
** loadCustInfo - Sets our checkboxes on our menu.
*/

void EditCustomer::loadCustInfo()
{
    ADB     DB;
    DB.query("select PrintedStatement, FreePrintedStatement from Customers where CustomerID = %ld", myCustID);
    if (DB.rowCount) {
        DB.getrow();
        if (isManager()) {
            stMenu->setItemChecked(printedStatementID, (bool) atoi(DB.curRow["PrintedStatement"]));
            stMenu->setItemChecked(freePrintedStatementID, (bool) atoi(DB.curRow["FreePrintedStatement"]));
            stMenu->setItemEnabled(freePrintedStatementID, (bool) atoi(DB.curRow["PrintedStatement"]));
        }
    }
}

/*
** customerChanged - Updates our customer data if our customer ID 
**                   matches.
*/

void EditCustomer::refreshCustomer(long custID)
{
    if (custID == myCustID) loadCustInfo();
}

/*
** createTicket - Opens the ticket creator to create a new ticket for this
**                customer.
*/

void EditCustomer::createTicket()
{
    if (myCustID) {
        CreateCustTicket   *CCT = new CreateCustTicket(0, "", myCustID);
        CCT->show();
    }
}


// vim: expandtab
