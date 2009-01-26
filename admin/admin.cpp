// $Id$
//
// Admin
//
// A new application to replace the Tk/Perl accounting system that I've been
// working on.
//
// By writing it using Qt and Mysql, the application can be statically
// linked and compiled for distribution.  Startup times and memory leaks
// and memory requirements should all be dramatically reduced by this,
// as well as making it cross-platform compatible with any other platform
// that the Qt and Mysql client libraries support.
//

#include <qpopmenu.h>
#include <qkeycode.h>
#include <qapplication.h>
#include <stdio.h>
#include <qfont.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <sys/types.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <Cfg.h>
#include <qwindowsstyle.h>
#include <qcdestyle.h>
#include <qlayout.h>

#include <TAATools.h>
#include "admin.h"
#include "common.h"
//#include "BlargDB.h"
#include "TAALogin.h"
#include "TAAConfig.h"

#ifdef USEDES
#include "loadKey.h"
#include "bdes.h"
#endif

// #include <ConfigTAA.h>

#include "Vendors.h"
#include "BillingCycles.h"
#include <CCBatch.h>
#include "OverdueAccounts.h"
#include "WipedAccounts.h"
#include "RunSubscriptions.h"
#include "RunStatements.h"
#include "LoginCountReport.h"
#include "LoginsToWipeReport.h"
#include "AllReports.h"
#include "EditLoginTypes.h"
#include "PKG_Main.h"
#include "BI_Main.h"
#include "MakeDeposits.h"
// #include "Scheduler.h"
#include "TE_Main.h"
#include "EditCustomer.h"
#include "LoginFlagList.h"

#include <ADB.h>

QApplication    *theApp;

Administration::Administration(QWidget *parent, const char *name)
    : TAAWidget(parent, name, 0) //(QWidget *parent=0, const char *name=0)
    
{
	accounts	= new Accounts;
	cycles		= new BillingCycles;
	rateplans	= new RatePlans;
	vtypes		= new VendorTypes;
	paymentterms= new PaymentTerms;
	customers	= NULL;                     // new Customers;
	dtypes		= new DomainTypeEdit;

/*    QPopupMenu *print = new QPopupMenu;
    CHECK_PTR( print );
    print->insertItem( "Print to printer", this, SLOT(printer()) );
    print->insertItem( "Print to file", this, SLOT(file()) );
    print->insertItem( "Print to fax", this, SLOT(fax()) );
    print->insertSeparator();
    print->insertItem( "Printer Setup", this, SLOT(printerSetup()) );
*/
    QPopupMenu *exportMenu = new QPopupMenu;
    CHECK_PTR( exportMenu );
    // export->insertItem( "Credit Card batch (Merchant Master)", this, SLOT(export_merchantmaster()) );
    exportMenu->insertItem( "Credit Card batch (Merchant Master)", this, SLOT(export_ccbatch()) );

    QPopupMenu *toolsmenu = new QPopupMenu;
    CHECK_PTR( toolsmenu );
    // export->insertItem( "Credit Card batch (Merchant Master)", this, SLOT(export_merchantmaster()) );
    toolsmenu->insertItem( "&Database Integrity Check", this, SLOT(checkDBIntegrity()) );


    QPopupMenu *file = new QPopupMenu();
    CHECK_PTR( file );
//    file->insertItem( "Open",  this, SLOT(open()) );
//    file->insertItem( "Save", this, SLOT(save()), CTRL+Key_S );
//    file->insertItem( "Close", this, SLOT(closeDoc()), CTRL+Key_W );
//    file->insertSeparator();
//    file->insertItem( "Print", print, CTRL+Key_P );
//    file->insertSeparator();
    file->insertItem( "Export", exportMenu);
    file->insertItem( "Tools", toolsmenu);
    file->insertItem( "&Config", this, SLOT(configtaa()));

/*
    QPopupMenu *edit = new QPopupMenu();
    CHECK_PTR( edit );
    int undoID = edit->insertItem( "Undo", this, SLOT(undo()) );
    int redoID = edit->insertItem( "Redo", this, SLOT(redo()) );
    edit->setItemEnabled( undoID, FALSE );
    edit->setItemEnabled( redoID, FALSE );
*/
    QPopupMenu *activities_menu = new QPopupMenu();
    CHECK_PTR( activities_menu );
    activities_menu->insertItem( "&Overdue Account Processing (Active Accounts)", this, SLOT(lockoverdue()), CTRL+Key_O );
    activities_menu->insertItem( "&Overdue Account Processing (Inactive Accounts)", this, SLOT(dowiped()));
//    activities_menu->insertItem( "&Write Checks", this, SLOT(write_checks()) );
//    activities_menu->insertItem( "&Pay Bills", this, SLOT(pay_bills()) );
    activities_menu->insertItem( "Process &Subscriptions", this, SLOT(run_subscriptions()) );
    activities_menu->insertItem( "Create S&tatements", this, SLOT(create_statements()) );
    activities_menu->insertItem( "Make &Deposits", this, SLOT(make_deposits()));
    activities_menu->insertSeparator();
    activities_menu->insertItem( "Send Targeted &Email", this, SLOT(targetedemail()));
    activities_menu->insertSeparator();
    activities_menu->insertItem( "&General Journal Entry", this, SLOT(generaljournalentry()) );
    activities_menu->insertSeparator();
    activities_menu->insertItem( "S&cheduler", this, SLOT(openScheduler()) );
    activities_menu->setCheckable( TRUE );
    
    QPopupMenu *otherlists_menu = new QPopupMenu;
    CHECK_PTR( otherlists_menu );
    otherlists_menu->insertItem( "Payment &Terms", this, SLOT(paymenttermslist()));
    otherlists_menu->insertItem( "&Vendor Types", this, SLOT(vendortypeslist()));
    otherlists_menu->insertItem( "&Login Types", this, SLOT(logintypeslist()));
    otherlists_menu->insertItem( "Login &Flags", this, SLOT(loginflagslist()));
    otherlists_menu->insertItem( "&Domain Types", this, SLOT(domaintypeslist()));

    QPopupMenu *lists_menu = new QPopupMenu();
    CHECK_PTR( lists_menu );
    lists_menu->insertItem( "&Accounts", this, SLOT(accountlist()),CTRL+Key_A  );
    lists_menu->insertItem( "&Customers", this, SLOT(customerlist()),CTRL+Key_C );
    lists_menu->insertItem( "&Vendors", this, SLOT(vendorlist()),CTRL+Key_V );
    lists_menu->insertItem( "&Packages", this, SLOT(packagelist()), CTRL+Key_P );
    lists_menu->insertItem( "&Rate Plans", this, SLOT(rateplanlist()), CTRL+Key_R );
    lists_menu->insertItem( "&Billable Items", this, SLOT(billableitemslist()),CTRL+Key_B );
    lists_menu->insertItem( "B&illing Cycles", this, SLOT(billingcycleslist()) );
    lists_menu->insertItem( "Other Lists", otherlists_menu);
    lists_menu->setCheckable( TRUE );

    QPopupMenu *arreports_menu = new QPopupMenu;
    CHECK_PTR( arreports_menu );
    arreports_menu->insertItem( "&Accounts Receivable", this, SLOT(arreport()));

    QPopupMenu *loginreports_menu = new QPopupMenu;
    CHECK_PTR( loginreports_menu );
    loginreports_menu->insertItem( "&Login Counts (Current)", this, SLOT(logincountreport()));
    loginreports_menu->insertItem( "Login &Counts (Monthly Average)", this, SLOT(loginavgreport()));
    loginreports_menu->insertItem( "Users &Online Report", this, SLOT(usersonlinereport()));
    loginreports_menu->insertItem( "&Logins To Wipe", this, SLOT(loginstowipereport()));
    loginreports_menu->insertItem( "&Rate Plans", this, SLOT(rateplanreport()));
    loginreports_menu->insertItem( "&User Cities", this, SLOT(cityreport()));
    loginreports_menu->insertItem( "&Call Logs", this, SLOT(calllogreport()));
    loginreports_menu->insertItem( "&Modem Totals (Current)", this, SLOT(modemtotalsreport()));
    loginreports_menu->insertItem( "Modem Usage Summary", this, SLOT(modemsummaryreport()));
    loginreports_menu->insertItem( "&Bandwidth Summary", this, SLOT(bandwidthsummaryreport()));

    QPopupMenu *reports_menu = new QPopupMenu();
    CHECK_PTR( reports_menu );
    reports_menu->insertItem( "&Cash flow", arreports_menu);
    reports_menu->insertItem( "&Logins", loginreports_menu);
    reports_menu->setCheckable( TRUE );

    // menu = new QMenuBar( this );
    // CHECK_PTR( menu );
    optionsMenu = new QPopupMenu();
    optionsMenu->insertItem( "&File", file );
//    menu->insertItem( "&Edit", edit );
    optionsMenu->insertItem( "&Activities", activities_menu );
    optionsMenu->insertItem( "&Lists", lists_menu );
    optionsMenu->insertItem( "&Reports", reports_menu );
    optionsMenu->insertSeparator();

    /*
    this->setGeometry(1, 1, 555, 150);
    this->setMaximumSize( 555, 150);
    this->setMinimumSize( 555, 150 );
    */
    
    reminders = new QListView(this, "Reminders");
    reminders->addColumn("Activity Reminders");
    reminders->addColumn("Amount");
    reminders->setFocus();
    reminders->setAllColumnsShowFocus(TRUE);
    //setCentralWidget(reminders);
    
    reminders->setColumnWidth(0, this->width() -100);
    reminders->setColumnWidth(1, 83);
    reminders->setColumnAlignment(1, AlignRight);

    // If the user double clicks or hits enter on a reminder item, we want
    // to take them to the thing that we're reminding them about.
    // The reminderSelected slot will do that for us.
    connect(reminders, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(reminderSelected(QListViewItem *)));
    connect(reminders, SIGNAL(returnPressed(QListViewItem *)), this, SLOT(reminderSelected(QListViewItem *)));

    updateReminders();

    // Refresh our reminders every few minutes...
    //timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateReminders()));
    //timer->start(15 * 60 * 1000);    // Every 15 minutes
    
    //statusBar()->message("Ready", 10000);

    // Do our layout
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(reminders, 1);
}

Administration::~Administration()
{};

/*
** menu - Returns our popup menu.
*/

QPopupMenu *Administration::menu()
{
    return optionsMenu;
}

/*
** customerChanged - This is a relay slot.  All it does is emit refreshCustomer
**                   with the passed in customer ID.  All other widgets that
**                   require notification that a customer entry has changed
**                   need to connect to this slot or call it.
*/

void Administration::customerChanged(long custID)
{
    QApplication::setOverrideCursor(waitCursor);
    emit refreshCustomer(custID);
    QApplication::restoreOverrideCursor();
}

void Administration::configtaa()
{
    /*
    ConfigTAA    *CTAA;

    CTAA = new ConfigTAA;
    CTAA->show();
    */
}

void Administration::accountlist()
{
    accounts->show();
    accounts->raise();
    accounts->setFocus();
    accounts->refreshList(1);
}

void Administration::billingcycleslist()
{
    cycles->show();
    cycles->refreshList(1);
    cycles->raise();
    cycles->setFocus();
}

void Administration::packagelist()
{
    PKG_Main    *pkgeditor = new PKG_Main();
    pkgeditor->show();
    pkgeditor->refreshPackages();
}

void Administration::targetedemail()
{
    TE_Main    *temail = new TE_Main();
    temail->show();
}

void Administration::rateplanlist()
{
    rateplans->show();
    rateplans->refreshList(1);
    rateplans->raise();
}

void Administration::billableitemslist()
{
    BI_Main *beditor = new BI_Main();
    beditor->show();
}

void Administration::vendortypeslist()
{
    vtypes->show();
    vtypes->refreshList(1);
    vtypes->raise();
    // vtypes->setFocus();
}

void Administration::paymenttermslist()
{
    paymentterms->show();
    paymentterms->refreshList(1);
    paymentterms->raise();
    // paymentterms->setFocus();
}

void Administration::vendorlist()
{
    Vendors *vendors = new Vendors(0);
    vendors->refreshList(1);
    vendors->show();
    vendors->raise();
    // vendors->setFocus();
}

void Administration::customerlist()
{
    if (!customers) customers = new Customers(0);
    //statusBar()->message("Loading customers...");
    QApplication::flushX();
    customers->show();
    customers->refreshList(1);
    customers->raise();
    //statusBar()->clear();
}

void Administration::generaljournalentry()
{
    GenJournalEntry *genjourn;
    genjourn = new GenJournalEntry();
    genjourn->show();
}
                
void Administration::logintypeslist()
{
    EditLoginTypes  *ELT = new EditLoginTypes();
    ELT->show();
}

void Administration::loginflagslist()
{
    LoginFlagList *LFL = new LoginFlagList();
    LFL->show();
}

void Administration::domaintypeslist()
{
    dtypes->show();
    dtypes->refreshList();
    dtypes->raise();
}

void Administration::lockoverdue()
{
    OverdueAccounts     *OAP;
    OAP = new (OverdueAccounts);
}

void Administration::dowiped()
{
    WipedAccounts     *WAP;
    WAP = new (WipedAccounts);
}

void Administration::export_ccbatch()
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


void Administration::run_subscriptions()
{
    RunSubscriptions    *RSB;
    RSB = new (RunSubscriptions);
}

void Administration::create_statements()
{
    switch(QMessageBox::information(this,
      "Create Statements",
      "Are you sure you wish to create\nstatements for all customers?",
      "&Yes", "&No", "&Cancel",
      0,        // Enter == button 0, yes
      2         // Escape  == cancel, button 2
    )) {
        case 0:         // Run statements.
            RunStatements();
            break;
            
        default:        // No/Cancel
            break;
    }
}

void Administration::make_deposits()
{
    MakeDeposits    *MDW = new MakeDeposits();
    MDW->show();
}


void Administration::logincountreport()
{
    LoginCountReport    *LCR;

    LCR = new LoginCountReport;
    LCR->show();
}

void Administration::loginstowipereport()
{
    LoginsToWipeReport    *LTWR;

    LTWR = new LoginsToWipeReport;
    LTWR->show();
}

void Administration::modemtotalsreport()
{
    ModemTotalsReport    *LCR;

    LCR = new ModemTotalsReport;
    LCR->show();
}

void Administration::modemsummaryreport()
{
    ModemSummaryReport    *MSR = new ModemSummaryReport;
    MSR->show();
}

void Administration::bandwidthsummaryreport()
{
    BandwidthSummaryReport    *BSR;

    BSR = new BandwidthSummaryReport;
    BSR->show();
}

void Administration::loginavgreport()
{
    LoginAvgReport    *LAR;

    LAR = new LoginAvgReport;
    LAR->show();
}

void Administration::rateplanreport()
{
    RatePlanReport    *RPR;

    RPR = new RatePlanReport;
    RPR->show();
}

void Administration::arreport()
{
    ARReport    *ARR;

    ARR = new ARReport;
    ARR->show();
    // ARR->generateReport();
}

void Administration::cityreport()
{
    CityReport    *CR;

    CR = new CityReport;
    CR->show();
    // ARR->generateReport();
}

void Administration::usersonlinereport()
{
    UsersOnlineReport  *UOR = new UsersOnlineReport;
    UOR->show();
}


void Administration::calllogreport()
{
    CustCallLogReport   *CCLR;

    CCLR = new CustCallLogReport;
    CCLR->setCustID((long)0);
    CCLR->allowReproduction(TRUE);
    CCLR->setUserButton("&Browse");
    connect(CCLR, SIGNAL(userButtonClicked()), SLOT(opencalllogbrowser()));
    CCLR->show();
}

void Administration::opencalllogbrowser()
{
    CustCallDetail  *CCD = new CustCallDetail();
    CCD->setCustID((long) 0);
    CCD->show();
}


void Administration::openScheduler()
{
    /*
    Scheduler   *sched = new Scheduler();
    sched->show();
    */
}



/*
** checkDBIntegrity - Performs various integrity checks on the database.
*/

void Administration::checkDBIntegrity()
{
    //statusBar()->message("Updating customer active flags...",100);
    QApplication::setOverrideCursor(waitCursor);
    QApplication::flushX();
    QApplication::syncX();
    
    ADB         DB1;
    ADB         DB2;
    int         custCount = 0;
    //int         curStep   = 0;
    long        rCount;
    
    
    
    DB1.query("select CustomerID from Customers order by CustomerID");
    custCount = DB1.rowCount;
    /*
    QLabel          *tmpLabel = new QLabel(statusBar());
    tmpLabel->setGeometry(statusBar()->width() - 150, 0, 130, statusBar()->height());
    QProgressBar    Progress(tmpLabel);
    Progress.setTotalSteps(custCount);
    Progress.setGeometry(statusBar()->width() - 150, 0, 130, statusBar()->height());
    statusBar()->addWidget(tmpLabel, 1, TRUE);
    */
    while (DB1.getrow()) {
        /*
        Progress.setProgress(curStep++);
        Progress.repaint();
        */
        theApp->processEvents();
	    QApplication::flushX();
	    QApplication::syncX();
        DB2.query("select LoginID from Logins where CustomerID = %s and Active = 1", DB1.curRow["CustomerID"]);
        rCount = DB2.rowCount;
        DB2.dbcmd("update Customers set Active = %ld where CustomerID = %ld", DB2.rowCount, atol(DB1.curRow["CustomerID"]));
        // Now, check to see if they have any login records at all.  If
        // they don't, create one.
        DB2.query("select LoginID from Logins where CustomerID = %s", DB1.curRow["CustomerID"]);
        if (!DB2.rowCount) {
            fprintf(stderr, "Customer ID %s has no login entries...\n", DB1.curRow["CustomerID"]);
        }
    }
    
    /*
    statusBar()->removeWidget(tmpLabel);

    statusBar()->clear();
    */
    QApplication::restoreOverrideCursor();
}






/*
** updateReminders - Refreshes the reminder listing, should be called by
**                   a QTimerEvent, once every n minutes.
*/

void Administration::updateReminders()
{
    //statusBar()->message("Updating reminders...");
    QApplication::setOverrideCursor(waitCursor);
    QApplication::flushX();
    QApplication::syncX();
    
    reminders->clear();
    ADB     DB;
    char    tmpStr[1024];
    QDate   tmpDate = QDate::currentDate();
    
    float   tmpFloat;
    
    
    // Check for credit card batches...
    tmpFloat = DB.sumFloat("select SUM(Amount) from CCTrans where BPSExported = 0 and TransDate <= '%04d-%02d-%02d'", 
      tmpDate.year(),
      tmpDate.month(),
      tmpDate.day()
    );

    if (tmpFloat > 0.00) {
        sprintf(tmpStr, "$%.2f", tmpFloat);
        (void) new QListViewItem(reminders, "Credit Card Batch", tmpStr, "1");
    }


    // Check for Accounts to Wipe...
    QDateTime   tmpTime = QDateTime::currentDateTime().addDays(-90);
    DB.query("select InternalID from Logins where Wiped = 0 and Active = 0 and LastModified < '%04d%02d%02d%02d%02d%02d'", 
      tmpTime.date().year(), tmpTime.date().month(), tmpTime.date().day(), 
      tmpTime.time().hour(), tmpTime.time().minute(), tmpTime.time().second());
    if (DB.rowCount) {
        sprintf(tmpStr, "%ld", DB.rowCount);
        (void) new QListViewItem(reminders, "Logins to Wipe", tmpStr, "4");
    }
    
    /*
    // Check for money to deposit...
    getCfgVal("UndepositedFundsAcct", tmpStr);
    DB.query("select Balance from Accounts where AccountNo = %d", atoi(tmpStr));
    DB.getrow();
    if (atof(DB.curRow[0]) > 0.00) {
        sprintf(tmpStr, "$%.2f", atof(DB.curRow[0]));
        (void) new QListViewItem(reminders, "Money to deposit", tmpStr, "3");
    }
    */
    

    QApplication::restoreOverrideCursor();
    //statusBar()->clear();
}

void Administration::reminderSelected(QListViewItem *item)
{
    if (item != NULL) {
        // Refresh our reminders 1/2 second after starting up
        QTimer::singleShot(500, this, SLOT(updateReminders()));

        switch (atoi(item->key(2, 0))) {
            case 1:  // Credit Card Batch
                export_ccbatch();
                break;
                
            case 3:  // Make Deposits
                make_deposits();
                break;
            
            case 4:  // Make Deposits
                loginstowipereport();
                break;
                
            default:
                char tmpStr[1024];
                sprintf(tmpStr, "There is no action associated with the\n'%s' reminder.", (const char *) item->key(0,0));
                QMessageBox::information(this, "Reminders", tmpStr);
                break;
        }
    }
}

/*
** mainWin slots
*/

void Administration::openCustomerMW(long custID)
{
    if (custID) {
        EditCustomer    *custEdit;
        custEdit = new EditCustomer(0, "", custID);
        custEdit->show();
    }
}

void Administration::refreshCustomerMW(long custID)
{
    emit(refreshCustomer(custID));
}




void Administration::resizeEvent( QResizeEvent * )
{
    reminders->setColumnWidth(0, width() -100);
    reminders->setColumnWidth(1, 83);
    reminders->triggerUpdate();

}

#ifdef ADMINMAIN
int main( int argc, char ** argv )
{
    // If we print something and spawn a process, they won't zombie if
    // we ignore the signal.
    signal(SIGCHLD, SIG_IGN);

    QApplication a( argc, argv );
    TAALogin    *l;
    uid_t       myUID;
    passwd      *pent;

    theApp = &a;
    
    QFont tmpFont;
    // tmpFont.setPointSize(10);
    tmpFont.setFamily("helvetica");
    
    // #ifdef USEDES
    loadKey();
    // #endif
    
    myUID = getuid();
    pent  = getpwuid(myUID);
    if (pent == NULL) {
        fprintf(stderr, "\nUnable to get user information.\n\n");
        exit(-1);
    }
    
    // BlargDB DB;
    // Initialize the database...
    // DB.connect(DBHOST, DBUSER, DBPASS, DBNAME);
    strcpy(DBHost, DBHOST);
    strcpy(DBUser, pent->pw_name);
    strcpy(DBPass, DBPASS);
    strcpy(DBName, DBNAME);

    l = new TAALogin();
    l->show();
    // fprintf(stderr, "Testing for successful login...\n");
    // if (!l->successfulLogin) exit(-1);
    
    ADB::setDefaultHost(DBHost);
    ADB::setDefaultDBase(DBName);
    ADB::setDefaultUser(DBUser);
    ADB::setDefaultPass(DBPass);

    // Load the config files.        
    cfgInit();
    
    Administration m;

    a.setMainWidget( &m );
    setMainWin(&m);
    a.setFont(tmpFont);
    a.setStyle(new QWindowsStyle);
    m.setCaption("Total Accountability Admin");
    m.show();


    return a.exec();
}
#endif // ADMINMAIN
