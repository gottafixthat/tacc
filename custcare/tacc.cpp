/*
** $Id$
**
***************************************************************************
**
** tacc - Total Accountability Customer Care interface.
**        This is the main program that Blarg employees will use.  It
**        encapsulates the main menu, the Main Window, a Take Call
**        interface, and pulls in the active ticket list.
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
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include <qwindowsstyle.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qaccel.h>
#include <qframe.h>

#include "tacc.h"
#include "Customers.h"
#include "admin.h"
#include "common.h"
#include "TAALogin.h"
#include "TAAConfig.h"
#include <TAATools.h>
#include <Cfg.h>
#include <QSqlDbPool.h>

#include "CallWizard.h"
#include "EditCustomer.h"
#include "TicketEditor.h"
#include "AllReports.h"

// Admin stuff
#include "RunSubscriptions.h"
#include <CCBatch.h>
#include <CCMonetra.h>
#include <StatementEngine.h>
#include "PKG_Main.h"
#include "PaymentTerms.h"
#include "BI_Main.h"
#include "OverdueAccounts.h"
#include "WipedAccounts.h"
#include "EditLoginTypes.h"
#include "LoginFlagList.h"
#include "DomainTypeEdit.h"
#include "DNS_Templates.h"
#include "TE_Main.h"
#include "Accounts.h"
#include "MakeDeposits.h"
#include "RatePlans.h"
#include "BillingCycles.h"
#include "AsteriskManager.h"
#include "QueueMonitor.h"
#include "DIDManager.h"
#include "OriginationProviders.h"
#include "RateCenterManager.h"
#include "ServerGroups.h"
#include "Vendors.h"
#include "VendorTypes.h"
#include "VoIPServiceTypes.h"
#include "CompanyEditor.h"
#include "SettingsManager.h"

#include "UserPrivs.h"

// Monetra/MCVE/CCVS
#include <mcve.h>

#ifdef USEDES
#include "loadKey.h"
#include "bdes.h"
#endif

int main( int argc, char ** argv )
{
    QApplication    a( argc, argv );
    CustomerCare    *aw;
    TAALogin        *lw;
    int             opt;
    a.setDesktopSettingsAware(true);
    
    // BlargDB	*DB;
    
    
    // Check our key.
    #ifdef USEDES
    loadKey();
    #endif
    
    // This will abort if it can't find a config file.
    loadTAAConfig(); 

    if (strlen(cfgVal("DebugLevel"))) {
        setDebugLevel(atoi(cfgVal("DebugLevel")));
    } else {
        setDebugLevel(0);
    }

    // Check our command line arguments.
	while((opt = getopt(argc, argv, "d:")) != EOF) {
	    switch(opt) {
	        case    'd':
	            setDebugLevel(atoi(optarg));
	            break;
	            
	        default:
	            fprintf(stderr, "Invalid argument.\n");
	            exit(-1);
	            break;
	    }
	}

    // Set the database defaults
    //ADB::setDebugLevel(9);
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));
    //ADB::setDebugLevel(9);
    QSqlDbPool::setDefaultHost(cfgVal("TAAMySQLHost"));
    QSqlDbPool::setDefaultName(cfgVal("TAAMySQLDB"));
    QSqlDbPool::setDefaultUser(cfgVal("TAAMySQLUser"));
    QSqlDbPool::setDefaultPass(cfgVal("TAAMySQLPass"));
    QSqlDbPool::setDefaultDriver(cfgVal("TAASQLDriver"));
    
    // BlargDB DB;
    // Initialize the database...
    /*
    strcpy(DBHost, DBHOST);
    strcpy(DBUser, pent->pw_name);
    strcpy(DBPass, DBPASS);
    strcpy(DBName, DBNAME);
    */

    // Setup our font and palette for the entire application.
    QFont       awFont = a.font();
    QPalette    pal;
    pal = a.palette();
    QColorGroup colg = pal.active();

    QColor standardLightGray( 220, 220, 220 );
    QColor light( 250, 250, 250 );
    QColor dark( standardLightGray.dark( 120 ) );
    QColorGroup std_act( Qt::black, dark,
             light, Qt::black, Qt::gray,
             Qt::black, standardLightGray);

    QColorGroup std_dis( Qt::darkGray, dark,
             light, dark, Qt::gray,
             Qt::darkGray, std_act.background() );

    QColorGroup std_inact( Qt::black, dark,
             light, Qt::black, Qt::gray,
             Qt::black, standardLightGray);

    QPalette qt_std_pal( std_act, std_dis, std_inact );

    //awFont.setPointSize(10);
    awFont.setPointSize(10);
    awFont.setFamily(QString("Bistream Vera Sans"));
    awFont.setStyleHint(QFont::SansSerif);
    //awFont.setWeight(50);

    //a.setStyle(new QWindowsStyle);
    //a.setFont(awFont, true);
    //a.setPalette(qt_std_pal);

    // Check the schema version.
    if (!schemaValid()) {
        QMessageBox::critical(NULL, "Invalid Database Schema", "This version of TACC is not compatible\nwith the current databse version.\n\nThe upgradetacc program will bring the version current.");
        exit(-1);
    }

    // Try logging in now.
    lw = new TAALogin();
    //lw->setDBName(DBName);
    //lw->setWindowTitle("TAA Login");
    //lw->setTextTitle("<HTML><CENTER><B>Total<I>Account</I>Ability</B><BR>Customer Care System Login</CENTER></HTML>");
    int status = lw->exec();
    if (status != QDialog::Accepted) exit(-1);


    /*
    strcpy(DBHost,  lw->server());
    strcpy(DBPass,  lw->pass());
    strcpy(DBUser,  lw->user());
    lw->setADBDefaults();
       
    ADB::setDefaultHost(DBHost);
    ADB::setDefaultDBase(DBName);
    ADB::setDefaultUser(DBUser);
    ADB::setDefaultPass(DBPass);
    */


    // Re-Initialize our configuration values.
    cfgInit();

	aw = new CustomerCare(NULL, "CustomerCare");

    a.setMainWidget(aw);
    //aw->setFont(awFont);
    aw->setPalette(qt_std_pal);

    setMainWin(aw);
    aw->show();

    aw->appRef = &a;

    //fprintf(stderr, "Font family: '%s'\n", (const char *) qApp->font().family());
    //fprintf(stderr, "Font point size: '%d'\n", qApp->font().pointSize());

    return a.exec();
}


/*
** CustomerCare - Customer Care is the main program widget.  It is the
**                interface that all Employees will use.  It consists of
**                a small menu, giving the user the ability to quit, a
**                take call button, the active ticket list, and a status
**                bar.
**
**                This should be the TAA MainWin().
*/

CustomerCare::CustomerCare(QWidget *parent, const char *name) : QMainWindow(parent, name)
{
    setMainWin(this);
    setCaption("Total Accountability - Customer Care");
    
    am = new AsteriskManager(this, "Asterisk Manager Connection");
    ccStack = new CustomerCareStack(am, this);
    setCentralWidget(ccStack);

    QAccel  *ac = new QAccel(this);
    ac->connectItem(ac->insertItem(CTRL+Key_T), this, SLOT(takeCall()));
    ac->connectItem(ac->insertItem(CTRL+Key_1), ccStack, SLOT(raiseTab1()));
    ac->connectItem(ac->insertItem(CTRL+Key_2), ccStack, SLOT(raiseTab2()));
    ac->connectItem(ac->insertItem(CTRL+Key_3), ccStack, SLOT(raiseTab3()));

    //setCentralWidget(tickets);

    AllReports  *reports = new AllReports(this);

    // Setup our menu
    QPopupMenu *fileMenu = new QPopupMenu(this);
    menuBar()->insertItem("&File", fileMenu);
    if (isAdmin()) {
        QPopupMenu *dbmenu = new QPopupMenu(this);
        dbmenu->insertItem("Database Integrity Check", this, SLOT(checkDBIntegrity()));
        dbmenu->insertItem("Update Allowed Mailbox Counts", this, SLOT(updateAllMaxMailboxes()));
        fileMenu->insertItem("Database Functions", dbmenu);
        QPopupMenu *printMenu = new QPopupMenu(this);
        printMenu->insertItem("Statements", this, SLOT(printStatements()));
        fileMenu->insertItem("&Print", printMenu);
        fileMenu->insertItem("Send Targeted Email", this, SLOT(sendTargetedEmail()));
        fileMenu->insertSeparator();
    }
    if (isManager()) {
        fileMenu->insertItem("Make Deposits", this, SLOT(make_deposits()));
        fileMenu->insertSeparator();
    }
    fileMenu->insertItem("&Quit", this, SLOT(quitSelected()));
    menuBar()->insertItem("Ti&ckets", ccStack->tickets->menu());

    // Add the voice mail menu now.
    menuBar()->insertItem("V&oice Mail", ccStack->processVM->menu());
    
    // Add the Customers menu now.
    menuBar()->insertItem("C&ustomers", ccStack->custs->menu());
    
    menuBar()->insertItem("&Reports", reports->menu());
        
    
    // Creat the VoIP Menu
    QPopupMenu *voipMenu = new QPopupMenu(this);
    if (isAdmin()) voipMenu->insertItem("&Add DID's", this, SLOT(voipAddDIDs()));
    if (isAdmin()) voipMenu->insertItem("&Origination Providers", this, SLOT(voipOriginationProviderList()));
    if (isAdmin()) voipMenu->insertItem("Manage &Rate Centers", this, SLOT(manageRateCenters()));
    if (isAdmin()) voipMenu->insertItem("VoIP &Service Types", this, SLOT(voipServiceTypeList()));

    menuBar()->insertItem("VoI&P", voipMenu);

    statusBar()->message("Bwahahaha!", 5000);

    if (isManager()) {
        // Create the domains sub-menu
        QPopupMenu  *domainMenu = new QPopupMenu(this);
        domainMenu->insertItem("&Domain Types", this, SLOT(domaintypelist()));
        domainMenu->insertItem("DNS &Templates", this, SLOT(dnstemplates()));

        QPopupMenu  *adminMenu = new QPopupMenu(this);
        if (isAdmin()) adminMenu->insertItem("Settings", this, SLOT(settings()));
        if (isAdmin()) adminMenu->insertItem("&Domains", domainMenu);
        if (isAdmin()) adminMenu->insertItem("Staff Editor", this, SLOT(staffEditor()));
        //adminMenu->insertItem("More...", ccStack->adm->menu());
        adminMenu->insertItem("Run Subscriptions", this, SLOT(run_subscriptions()));
        if (isAdmin()) adminMenu->insertItem("Process Credit Cards", this, SLOT(process_ccbatch()));
        if (isAdmin()) adminMenu->insertItem("Settle Credit Card Batches", this, SLOT(settleCCBatch()));
        if (isAdmin()) adminMenu->insertItem("Export Credit Cards", this, SLOT(export_ccbatch()));
        adminMenu->insertItem("Run Statements (All Customers)", this, SLOT(create_statements()));
        adminMenu->insertItem("Process Overdue Accounts (active)", this, SLOT(processOverdueAccounts()));
        if (isAdmin()) adminMenu->insertItem("Process Overdue Accounts (wiped)", this, SLOT(processWipedAccounts()));

        menuBar()->insertItem("Adm&in", adminMenu);
        // "Other Lists"
        if (isAdmin()) {
            QPopupMenu *otherMenu = new QPopupMenu(this);
            otherMenu->insertItem("Accounts", this, SLOT(accountlist()));
            otherMenu->insertItem("Billable Items", this, SLOT(billableitemslist()));
            otherMenu->insertItem("Billing Cycles", this, SLOT(billingCycleList()));
            otherMenu->insertItem("Company List", this, SLOT(openCompanyList()));
            otherMenu->insertItem("Package Editor", this, SLOT(packagelist()));
            otherMenu->insertItem("Payment Terms", this, SLOT(paymenttermslist()));
            otherMenu->insertItem("Login Types", this, SLOT(logintypelist()));
            otherMenu->insertItem("Login Flags", this, SLOT(loginflaglist()));
            otherMenu->insertItem("Rate Plans", this, SLOT(ratePlanList()));
            otherMenu->insertItem("Server Groups", this, SLOT(serverGroupList()));
            otherMenu->insertItem("Vendors", this, SLOT(vendorList()));
            otherMenu->insertItem("Vendor Types", this, SLOT(vendorTypeList()));
            adminMenu->insertSeparator();
            adminMenu->insertItem("&Other Lists", otherMenu);
        }

    }

    QPopupMenu  *helpMenu = new QPopupMenu(this);
    helpMenu->insertItem("&About", this, SLOT(about()));
    menuBar()->insertSeparator();
    menuBar()->insertItem("&Help", helpMenu);

    
    /*
    // Setup our tool bar
    QToolBar    *userTools = new QToolBar(this, "Program actions");
    addToolBar(userTools, "Program actions", Top, true);

    QPixmap takeCallIcon;
    takeCallIcon.load("/usr/local/lib/taa/icons/phone.xpm");
    QToolButton *takeCallButton = new QToolButton(takeCallIcon, "Take Call", QString::null, this, SLOT(takeCall()), userTools, "Take Call");

    QPixmap custListIcon;
    custListIcon.load("/usr/local/lib/taa/icons/customers.xpm");
    QToolButton *custListButton = new QToolButton(custListIcon, "Customer List", QString::null, this, SLOT(customerList()), userTools, "Open the customer list");

    takeCallButton->setTextLabel("Take Call");
    //takeCallButton->setUsesTextLabel(true);
    takeCallButton->setUsesBigPixmap(true);
    */

    
    /*
    QPixmap ticketIcon;
    ticketIcon.load("/usr/local/lib/taa/icons/ticket.xpm");
    QToolButton *ticketButton = new QToolButton(ticketIcon, "New Ticket", QString::null, this, SLOT(takeCall()), userTools, "Create a new ticket");

    ticketButton->setTextLabel("Create a new Ticket");
    //ticketButton->setUsesTextLabel(true);
    ticketButton->setUsesBigPixmap(true);
    */

    // Move us to the upper left corner of the screen
    //this->setGeometry(1,1,555,300);
    this->resize(555,300);
    
    progressMeter = new QProgressBar(statusBar(), "Progress Meter");
    progressMeter->setMaximumWidth(50);
    progressMeter->setIndicatorFollowsStyle(false);
    progressMeter->setCenterIndicator(true);
    statusBar()->addWidget(progressMeter, 0, true);
    realTimeProgress = false;

    countLabel = new QLabel(statusBar(), "CountLabel");
    countLabel->setAlignment(AlignRight|AlignVCenter);
    //countLabel->setFrameStyle(QFrame::NoFrame|QFrame::Plain);
    statusBar()->addWidget(countLabel, 0, true);

    // Create our clock label and add it into the statusBar()
    clockLabel = new QLabel(statusBar(), "ClockLabel");
    clockLabel->setAlignment(AlignRight|AlignVCenter);
    //clockLabel->setFrameStyle(QFrame::NoFrame|QFrame::Plain);
    statusBar()->addWidget(clockLabel, 0, true);

    // Create our Asterisk Manager conneciton now.
    connect(am, SIGNAL(eventReceived(const astEventRecord)), this, SLOT(asteriskEvent(const astEventRecord)));
    connect(am, SIGNAL(eventReceived(const astEventRecord)), ccStack->qMon, SLOT(asteriskEvent(const astEventRecord)));
    connect(ccStack->qMon, SIGNAL(transferCall(const char *, const char *, const char *, int)), am, SLOT(transferCall(const char *, const char *, const char *, int)));
    //connect(am, SIGNAL(eventReceived(const astEventRecord)), ccStack->agents, SLOT(asteriskEvent(const astEventRecord)));
    //connect(ccStack->agents, SIGNAL(sendAsteriskCommand(int)), am, SLOT(simpleCommand(int)));

    
    updateClock();

    clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(updateClock()));
    clockTimer->start(1000);     // Update the clock once per second
}

CustomerCare::~CustomerCare()
{
}

/*
** takeCall - Takes an incoming call from a user.
*/

void CustomerCare::takeCall()
{
    CallWizard *call = new CallWizard();
    call->show();
}

/*
** customerList - Opens the customer list.
*/

void CustomerCare::customerList()
{
    clist = new Customers();
    clist->show();
    connect(ccStack->qMon, SIGNAL(phoneNumberSelected(const char *)), clist, SLOT(customerSearch(const char *)));
}

/*
** quitSelected - Prompts the user as to whether or not they actually wish
**                to quit the program.  If they do, we quit.
*/

void CustomerCare::quitSelected()
{
    if (QMessageBox::warning(this, "Cutomer Care", "Are you sure you want to quit?", "&Yes", "&No", 0, 1) == 0) {
        qApp->closeAllWindows();
    } else {
        statusBar()->message("Excellent!", 1000);
    }
}

/*
** customerChanged - This is a relay slot.  All it does is emit refreshCustomer
**                   with the passed in customer ID.  All other widgets that
**                   require notification that a customer entry has changed
**                   need to connect to this slot or call it.
*/

void CustomerCare::customerChanged(long custID)
{
    QApplication::setOverrideCursor(waitCursor);
    emit refreshCustomer(custID);
    QApplication::restoreOverrideCursor();
}

/*
** ticketChanged - This is a relay slot.  All it does is emit refreshTicket
**                 with the passed in ticket number.  All other widgets that
**                 require notification that a ticket has changed
**                 need to connect to this slot or call it.
*/

void CustomerCare::ticketChanged(long ticketNo)
{
    QApplication::setOverrideCursor(waitCursor);
    emit refreshTicket(ticketNo);
    QApplication::restoreOverrideCursor();
}

/*
** mainWin slots
*/

void CustomerCare::openCustomerMW(long custID)
{
    if (custID) {
        EditCustomer    *custEdit;
        custEdit = new EditCustomer(0, "", custID);
        custEdit->show();
    }
}

void CustomerCare::openTicketMW(long ticketNo)
{
    if (ticketNo) {
        TicketEditor    *tickEdit;
        tickEdit = new TicketEditor(0, "", ticketNo);
        tickEdit->show();
    }
}

void CustomerCare::refreshCustomerMW(long custID)
{
    emit(refreshCustomer(custID));
}

void CustomerCare::refreshTicketMW(long ticketNo)
{
    emit(refreshTicket(ticketNo));
}

/*
** updateClock - Updates the clock in our status window.
*/

void CustomerCare::updateClock()
{
    QDateTime   curTime = QDateTime::currentDateTime();
    char        tmpStr[1024];
    char        ap[64];
    int         hour = curTime.time().hour();

    if (hour >= 12) {
        strcpy(ap, "pm");
        hour -= 12;
    } else {
        strcpy(ap, "am");
    }
    if (!hour) hour = 12;


    sprintf(tmpStr, "%s, %s %d, %d %2d:%02d%s",
            (const char *) curTime.date().dayName(curTime.date().dayOfWeek()),
            (const char *) curTime.date().monthName(curTime.date().month()),
            curTime.date().day(),
            curTime.date().year(),
            hour,
            curTime.time().minute(),
            ap);
    clockLabel->setText(tmpStr);

    sprintf(tmpStr, "T:%d V:%d", ccStack->tickets->ticketCount(), ccStack->processVM->count());
    countLabel->setText(tmpStr);
}

/*
** setStatusMW - Connected to the TAAWidget setStatus command.  This displays
**               the message on our statusBar().
*/

void CustomerCare::setStatusMW(const char *msg)
{
    if (!msg || !strlen(msg)) {
        statusBar()->clear();
    } else {
        statusBar()->message(msg);
        appRef->processEvents();
        QApplication::flush();
        QApplication::flushX();
        QApplication::syncX();
    }
}

/*
** setProgressMW - Connected to the TAAWidget setProgress command.  This 
**                 displays a progress meter on our statusBar().
*/

void CustomerCare::setProgressMW(int cur, int tot)
{
    //fprintf(stderr, "setProgressMW, cur = %d, tot = %d\n", cur, tot);
    if (tot == 0) {
        progressMeter->setProgress(0);
        progressMeter->setTotalSteps(99999);
        progressMeter->reset();
    } else {
        if (cur < tot) {
            if (cur == 0) {
                progressMeter->setTotalSteps(tot);
                progressMeter->setProgress(cur);
                //progressMeter->show();
            }
            progressMeter->setProgress(cur);
        } else {
            //progressMeter->hide();
            //statusBar()->removeWidget(progressMeter);
            progressMeter->setProgress(0);
            progressMeter->setTotalSteps(99999);
            progressMeter->reset();
        }
        if (realTimeProgress) {
            this->repaint();
            appRef->processEvents();
        }
        QApplication::flush();
        QApplication::flushX();
        QApplication::syncX();
    }
}

/*
** setProgressMWRT - Real-time progress updates (flushing buffers)
*/

void CustomerCare::setProgressMWRT(int cur, int tot)
{
    realTimeProgress = true;
    setProgressMW(cur, tot);
    realTimeProgress = false;
}


/*
** CustomerCareStack - Contains a stack of windows that are used to show
**                     the user tickets, voice mail, customers, etc.
*/

CustomerCareStack::CustomerCareStack(AsteriskManager *astmgr, QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("Total Accountability - Customer Care");
    
    // Create the widget stack and tab bar.
    tabs = new QTabBar(this);
    tabs->setShape(QTabBar::RoundedAbove);
    customerTab = new QTab("Customers");
    ticketTab = new QTab("Tickets");
    voicemailTab = new QTab("Voice Mail");
    //adminTab = new QTab("Admin");
    tabs->insertTab(customerTab);
    tabs->insertTab(ticketTab);
    tabs->insertTab(voicemailTab);
    //tabs->insertTab(adminTab);

    QLabel *hline1 = new QLabel(this);
    hline1->setMinimumSize(0,3);
    hline1->setMaximumSize(32767,3);
    hline1->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    hline1->setLineWidth(1);
    
    QLabel *hline2 = new QLabel(this);
    hline2->setMinimumSize(0,3);
    hline2->setMaximumSize(32767,3);
    hline2->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    hline2->setLineWidth(1);

    connect(tabs, SIGNAL(selected(int)), this, SLOT(tabClicked(int)));

    qws = new QWidgetStack(this, "WidgetStack");
    int curLevel = 0;

    custs = new Customers(qws, "Customers");
    qws->addWidget(custs, curLevel);
    qws->raiseWidget(custs);
    curLevel++;

    tickets = new TicketManager(qws, "TicketManager");
    qws->addWidget(tickets, curLevel);
    curLevel++;

    processVM = new ProcessVoiceMail(qws, "ProcessVoiceMail");
    qws->addWidget(processVM, curLevel);
    curLevel++;

    qMon   = new QueueMonitor(this, "Queue Monitor");
    agents = new AgentStatus(astmgr, this, "Agent List");
    connect(qMon, SIGNAL(phoneNumberSelected(const char *)), custs, SLOT(customerSearch(const char *)));
    
    /*
    if (isAdmin()) {
        adm = new Administration(qws, "Admin");
        qws->addWidget(adm, curLevel);
        curLevel++;
    }
    */

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::LeftToRight, 0, 0);
    QBoxLayout *mcl = new QBoxLayout(QBoxLayout::TopToBottom, 0, 0);
    mcl->addWidget(tabs, 0);
    mcl->addWidget(hline1, 0);
    mcl->addWidget(qws, 1);
    mcl->addWidget(hline2, 0);
    mcl->addWidget(qMon, 1);
    ml->addLayout(mcl, 1);
    ml->addWidget(agents, 0);
}

CustomerCareStack::~CustomerCareStack()
{
}

void CustomerCareStack::tabClicked(int newTab)
{
    qws->raiseWidget(newTab);
}

/*
 * raiseTabN - Raises the tab specified.  These are for accelerator
 *             keys.
 *
*/

void CustomerCareStack::raiseTab1() { tabs->setCurrentTab(0); }
void CustomerCareStack::raiseTab2() { tabs->setCurrentTab(1); }
void CustomerCareStack::raiseTab3() { tabs->setCurrentTab(2); }

void CustomerCare::staffEditor()
{
    UserPrivs   *up = new UserPrivs();
    up->show();
}


void CustomerCare::run_subscriptions()
{
    RunSubscriptions    *RSB;
    RSB = new (RunSubscriptions);
}

void CustomerCare::process_ccbatch()
{
    ADB DB;
    DB.query("select InternalID from CCTrans where BPSExported = 0");
    if (!DB.rowCount) {
        QMessageBox::information(this, "Process Credit Card Batch", "There were no transactions found to process.");
    } else {
        CCMonetra *CCM;
        CCM = new (CCMonetra);
        connect(CCM, SIGNAL(finished()), this, SLOT(updateReminders()));
        CCM->show();
    }
}

void CustomerCare::export_ccbatch()
{
    ADB DB;
    DB.query("select InternalID from CCTrans where BPSExported = 0");
    if (!DB.rowCount) {
        QMessageBox::information(this, "Export Credit Card Batch", "There were no transactions found to export.");
    } else {
        CCBatch    *CCB;
        CCB = new (CCBatch);
        connect(CCB, SIGNAL(finished()), this, SLOT(updateReminders()));
        CCB->show();
    }
}

void CustomerCare::create_statements()
{
    StatementEngine    stEng;
    switch(QMessageBox::information(this,
      "Create Statements",
      "Are you sure you wish to create\nstatements for all customers?",
      "&Yes", "&No", "&Cancel",
      0,        // Enter == button 0, yes
      2         // Escape  == cancel, button 2
    )) {
        case 0:         // Run statements.
            stEng.runStatements();
            break;
            
        default:        // No/Cancel
            break;
    }
}

void CustomerCare::billableitemslist()
{
    BI_Main    *beditor = new BI_Main();
    beditor->show();
}

void CustomerCare::packagelist()
{
    PKG_Main    *pkgeditor = new PKG_Main();
    pkgeditor->show();
    pkgeditor->refreshPackages();
}

void CustomerCare::paymenttermslist()
{
    PaymentTerms *terms = new PaymentTerms();
    terms->show();
    terms->refreshList(1);
}

void CustomerCare::ratePlanList()
{
    RatePlans   *rpl = new RatePlans();
    rpl->show();
    rpl->refreshList(1);
}

void CustomerCare::serverGroupList()
{
    ServerGroups *sgl = new ServerGroups();
    sgl->show();
    sgl->refreshList();
}

void CustomerCare::vendorList()
{
    Vendors *vlist = new Vendors();
    vlist->show();
    vlist->refreshList(1);
}

void CustomerCare::vendorTypeList()
{
    VendorTypes *vtlist = new VendorTypes();
    vtlist->show();
    vtlist->refreshList(1);
}

void CustomerCare::billingCycleList()
{
    BillingCycles   *bcl = new BillingCycles();
    bcl->show();
    bcl->refreshList(1);
}

void CustomerCare::openCompanyList()
{
    CompanyList *cl = new CompanyList();
    cl->show();
    //cl->refreshList(1);
}

void CustomerCare::settings()
{
    SettingsManager *sm = new SettingsManager();
    sm->show();
}

void CustomerCare::logintypelist()
{
    EditLoginTypes *lteditor = new EditLoginTypes();
    lteditor->show();
}

void CustomerCare::loginflaglist()
{
    LoginFlagList *lfl = new LoginFlagList();
    lfl->show();
}

void CustomerCare::domaintypelist()
{
    DomainTypeEdit *dtypes = new DomainTypeEdit();
    dtypes->show();
}

void CustomerCare::dnstemplates()
{
    DNS_Templates *dnst = new DNS_Templates();
    dnst->show();
}

void CustomerCare::processOverdueAccounts()
{
    OverdueAccounts *OAP;
    OAP = new OverdueAccounts;
    OAP->show();
}

void CustomerCare::processWipedAccounts()
{
    WipedAccounts *WAP;
    WAP = new WipedAccounts;
    WAP->show();
}

void CustomerCare::voipAddDIDs()
{
    DIDManagerAdd *MD = new DIDManagerAdd();
    MD->show();
}

void CustomerCare::voipOriginationProviderList()
{
    OriginationProviders *OP = new OriginationProviders();
    OP->show();
}

void CustomerCare::voipServiceTypeList()
{
    VoIPServiceTypes *VST = new VoIPServiceTypes();
    VST->show();
}

void CustomerCare::manageRateCenters()
{
    RateCenterManager *RCM = new RateCenterManager();
    RCM->show();
}

/*
** printStatements - Prints any statements tagged in the database as needing
**                   to be printed.
*/

void CustomerCare::printStatements()
{
    ADB     DB;
    long    curSt   = 0;
    long    stCount = 0;
    char    tmpSt[1024];
    char    theDate[32];
    QDate   tmpDate = QDate::currentDate();
    StatementEngine stEng;

    sprintf(theDate, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());

    QApplication::setOverrideCursor(waitCursor);
    statusBar()->message("Searching for statements to print...");

    DB.query("select StatementNo from Statements where ToBePrinted > 0");
    statusBar()->message("");
    QApplication::restoreOverrideCursor();
    stCount = DB.rowCount;
    if (!DB.rowCount) {
        QMessageBox::information(this, "Print Statements", "There were no statements found to print.");
        return;
    }


    sprintf(tmpSt, "There were %ld statements found to print.\nAre you sure you want to print?", stCount);
    if (QMessageBox::warning(this, "Print Statements", tmpSt, "&Yes", "&No", 0, 1) != 0) return;

    // If we made it here, we're good to print.
    QApplication::setOverrideCursor(waitCursor);
    statusBar()->message("Printing statements...");
    setProgressMW(0, stCount);
    while (DB.getrow()) {
        setProgressMW(++curSt, stCount);
        stEng.printStatement((uint)atol(DB.curRow["StatementNo"]), 0, 0);
    }

    setProgressMW(1,1);
    statusBar()->message("");
    QApplication::restoreOverrideCursor();
}


void CustomerCare::sendTargetedEmail()
{
    TE_Main     *temail = new TE_Main();
    temail->show();
}

/*
** accountlist - Opens the list of accounts.
*/

void CustomerCare::accountlist()
{
    Accounts    *accounts;
    accounts = new Accounts();
    accounts->show();
    accounts->refreshList(1);
}

/*
** make_deposits - Opens the Make Deposits window
*/

void CustomerCare::make_deposits()
{
    MakeDeposits    *MDW = new MakeDeposits();
    MDW->show();
}

/*
** settleCCBatch - Verifies that the user wants to settle the credit 
**                 card batch and then does it.  This could be its own
**                 class, but it doesn't really need to be since it
**                 just needs to send out status information.
*/

void CustomerCare::settleCCBatch()
{
    MCVE_CONN   mcvec;
    long        mcveid;
    long        rows;
    int         cols;
    long        totTrans = 0;
    float       totAmount = 0.00;
    char        mcveHost[1024];
    char        tmpStr[4096];
    int         totBatches = 0;
    long        batches[1024];

    strcpy(mcveHost, cfgVal("MCVEHost"));

    // First, connect to the MCVE engine and see if there is anything
    // to process.
    setStatusMW("Checking batch status...");

    // Init our MCVE connection
    MCVE_InitEngine("CAfile.pem");
    MCVE_InitConn(&mcvec);
    
    // Initialize the connection.
    if (!MCVE_SetIP(&mcvec, mcveHost, atoi(cfgVal("MCVEPort")))) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Unable to connect to the MCVE server.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // We put it into blocking mode since bad things might happen if we
    // don't.  This way, once we get something back from Monetra, we're
    // 100% sure that transaction went through one way or another.
    // the server at once, then we'll loop through until we're finished.
    if (!MCVE_SetBlocking(&mcvec, 1)) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Unable to set MCVE into blocking mode.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // Set a maximum timeout per transaction of 30 seconds.
    if (!MCVE_SetTimeout(&mcvec, 30)) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Unable to set MCVE transaction timeout.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // Finally, connect to the server
    if (!MCVE_Connect(&mcvec)) {
        sprintf(tmpStr, "Connection to MCVE Server failed.\n%s", MCVE_ConnectionError(&mcvec));
        QMessageBox::critical(this, "Close Credit Card Batch", tmpStr);
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // If we're here, we've got a connection.
    // Create a new transaction to get the unsettled transactions (GUT)
    mcveid = MCVE_TransNew(&mcvec);
    MCVE_TransParam(&mcvec, mcveid, MC_USERNAME, cfgVal("MCVEUser"));
    MCVE_TransParam(&mcvec, mcveid, MC_PASSWORD, cfgVal("MCVEPass"));
    MCVE_TransParam(&mcvec, mcveid, MC_TRANTYPE, MC_TRAN_ADMIN);
    MCVE_TransParam(&mcvec, mcveid, MC_ADMIN,    MC_ADMIN_GUT);

    // Send the request....
    if (!MCVE_TransSend(&mcvec, mcveid)) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Error reqeusting report.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    if (MCVE_ReturnStatus(&mcvec, mcveid) != MCVE_SUCCESS) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Unable to generate report.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    if (!MCVE_ParseCommaDelimited(&mcvec, mcveid)) {
        QMessageBox::critical(this, "Close Credit Card Batch", "Unable to parse report.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    rows = MCVE_NumRows(&mcvec, mcveid);
    cols = MCVE_NumColumns(&mcvec, mcveid);

    /*
    for (int i = 0; i < cols; i++) {
        if (i > 0) fprintf(stderr, "|");
        fprintf(stderr, "%s", MCVE_GetHeader(&mcvec, mcveid, i));
    }
    fprintf(stderr, "\n");
    */

    // Find the batches as well.
    totTrans = rows;
    for (int j = 0; j < rows; j++) {
        setProgressMW(j, rows);
        /*
        for (int i = 0; i < cols; i++) {
            if (i > 0) fprintf(stderr, "|");
            fprintf(stderr, "%s", MCVE_GetCellByNum(&mcvec, mcveid, i, j));
        }
        */
        if (!totBatches) {
            totBatches++;
            batches[totBatches-1] = atol(MCVE_GetCell(&mcvec, mcveid, "batch", j));
        } else {
            // See if we're already going to process this batch
            bool foundBatch = false;
            for (int n = 0; n < totBatches; n++) {
                if (batches[n] == atol(MCVE_GetCell(&mcvec, mcveid, "batch", j))) {
                    foundBatch = true;
                }
            }
            if (!foundBatch) {
                batches[totBatches] = atol(MCVE_GetCell(&mcvec, mcveid, "batch", j));
                totBatches++;
            }
        }
        totAmount += atof(MCVE_GetCell(&mcvec, mcveid, "amount", j));
        //fprintf(stderr, "\n");
    }
    setProgressMW(rows, rows);
    //MCVE_DeleteTrans(&mcvec, mcveid);
    setStatusMW("");

    if (!totTrans) {
        MCVE_DestroyConn(&mcvec);
        QMessageBox::information(this, "Settle Credit Card Batches", "No unsettled batches were found.");
        return;
    }

    sprintf(tmpStr, "There are %ld transactions in %d batches to be\nsettled for a total of $%.2f.\nAre you sure you wish to continue?", totTrans, totBatches, totAmount);
    if (QMessageBox::warning(this, "Settle Credit Card Batches", tmpStr, "&Yes", "&No", 0, 1) == 1) {
        MCVE_DestroyConn(&mcvec);
        return;
    }

    setStatusMW("Settling batches...");
    // They're sure.  Build the transaction and close the batches.
    for (int n = 0; n < totBatches; n++) {
        setProgressMW(n, totBatches);
        // Set the batch number
        sprintf(tmpStr, "%ld", batches[n]);
        mcveid = MCVE_TransNew(&mcvec);
        MCVE_TransParam(&mcvec, mcveid, MC_USERNAME, cfgVal("MCVEUser"));
        MCVE_TransParam(&mcvec, mcveid, MC_PASSWORD, cfgVal("MCVEPass"));
        MCVE_TransParam(&mcvec, mcveid, MC_TRANTYPE, MC_TRAN_SETTLE);
        MCVE_TransParam(&mcvec, mcveid, MC_BATCH,    tmpStr);

        // Send it.
        if (!MCVE_TransSend(&mcvec, mcveid)) {
            sprintf(tmpStr, "Error sending settlement request for batch %ld.  Aborting.\nSettlement logs should be examined for errors.", batches[n]);
            QMessageBox::critical(this, "Settle Credit Card Batches", tmpStr);
            MCVE_DestroyConn(&mcvec);
            // Reset our status
            setProgressMW(totBatches, totBatches);
            setStatusMW("");
            return;
        }

        if (MCVE_ReturnStatus(&mcvec, mcveid) != MCVE_SUCCESS) {
            sprintf(tmpStr, "Unable to settle batch %ld.  Aborting.\nSettlement logs should be examined for errors.", batches[n]);
            QMessageBox::critical(this, "Settle Credit Card Batches", tmpStr);
            MCVE_DestroyConn(&mcvec);
            // Reset our status
            setProgressMW(totBatches, totBatches);
            setStatusMW("");
            return;
        }
    }
    setProgressMW(totBatches, totBatches);

    // We're all done.  Close the connection
    MCVE_DestroyConn(&mcvec);
    MCVE_DestroyEngine();
    setStatusMW("");

    sprintf(tmpStr, "%d batches were settled for a total of $%.2f.", totBatches, totAmount);
    QMessageBox::information(this, "Close Credit Card Batch", tmpStr);

    
}



/*
** checkDBIntegrity - Performs various integrity checks on the database.
*/

void CustomerCare::checkDBIntegrity()
{
    if (QMessageBox::warning(this, "Cutomer Care", "A database integirty check could take some time.\nAre you sure you want to do this?", "&Yes", "&No", 0, 1) != 0) return;

    QApplication::setOverrideCursor(waitCursor);
    statusBar()->message("Performing database integrity check...");
    
    ADB         DB1;
    ADB         DB2;
    int         custCount = 0;
    int         curStep   = 0;
    long        rCount;
    
    
    
    DB1.query("select CustomerID from Customers order by CustomerID");
    custCount = DB1.rowCount;
    setProgressMW(0, custCount);
    while (DB1.getrow()) {
        setProgressMW(++curStep, custCount);
        DB2.query("select LoginID from Logins where CustomerID = %s and Active = 1", DB1.curRow["CustomerID"]);
        rCount = DB2.rowCount;
        DB2.dbcmd("update Customers set Active = %ld where CustomerID = %ld", DB2.rowCount, atol(DB1.curRow["CustomerID"]));
        // Now, check to see if they have any login records at all.  If
        // they don't, create one.
        /*
        DB2.query("select LoginID from Logins where CustomerID = %s", DB1.curRow["CustomerID"]);
        if (!DB2.rowCount) {
            fprintf(stderr, "Customer ID %s has no login entries...\n", DB1.curRow["CustomerID"]);
        }
        */
    }
    
    setProgressMW(1,1);
    statusBar()->message("");
    QApplication::restoreOverrideCursor();
}

/*
** updateAllMaxMailboxes - Updates the maximum number of mailboxes a customer
**                         may have without getting billed for more.  It 
**                         does this for *all* customers.
*/

void CustomerCare::updateAllMaxMailboxes()
{
    if (QMessageBox::warning(this, "Cutomer Care", "Updating all customers maximum allowed mailboxes could take some time.\nAre you sure you want to do this?", "&Yes", "&No", 0, 1) != 0) return;

    QApplication::setOverrideCursor(waitCursor);
    statusBar()->message("Updating maximum mailbox counts...");
    
    ADB         DB;
    int         custCount = 0;
    int         curStep   = 0;
    DB.query("select CustomerID from Customers order by CustomerID");
    custCount = DB.rowCount;
    setProgressMW(0, custCount);
    while (DB.getrow()) {
        setProgressMW(++curStep, custCount);
        recountAllowedMailboxes(atol(DB.curRow["CustomerID"]));
    }

    setProgressMW(1,1);
    statusBar()->message("");
    QApplication::restoreOverrideCursor();
}

/** asteriskEvent - Gets called automagically when a new asterisk event comes
  * in from the AsteriskManager class.
  */
void CustomerCare::asteriskEvent(const astEventRecord event)
{
    /*
    for (int i = 0; i < event.count; i++) {
        fprintf(stderr, "CustomerCare::asteriskEvent() - Key: '%s', Val: '%s'\n",
                event.data[i].key, event.data[i].val);
    }
    */
}

