// $Id: admin.h,v 1.2 2004/04/15 18:13:47 marc Exp $
//
// Total Accountability admin definitions.
//


#ifndef ADMIN_H
#define ADMIN_H

#include <qapp.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <qlistview.h>
#include <qtimer.h>

#include "Accounts.h"
#include "BillingCycles.h"
#include "RatePlans.h"
#include "VendorTypes.h"
#include "PaymentTerms.h"
#include "GenJournalEntry.h"
#include "Customers.h"
#include "DomainTypeEdit.h"
#include "TAATools.h"
#include "TAAWidget.h"

class Administration : public TAAWidget
{
    Q_OBJECT
public:
    Administration(QWidget* parent = NULL, const char *name = NULL);
    ~Administration();
    
    Accounts		* accounts;
    BillingCycles	* cycles;
    RatePlans		* rateplans;
    VendorTypes		* vtypes;
    PaymentTerms	* paymentterms;
    Customers		* customers;
    DomainTypeEdit	* dtypes;

    QPopupMenu      *menu();

public slots:
    void    customerChanged(long);
    void configtaa();
	void accountlist();
	void billingcycleslist();
	void rateplanlist();
	void billableitemslist();
	void packagelist();
	void vendortypeslist();
	void paymenttermslist();
	void vendorlist();
	void generaljournalentry();
	void customerlist();
	void logintypeslist();
	void domaintypeslist();
	void lockoverdue();
	void dowiped();
    void export_ccbatch();
	void run_subscriptions();
	void create_statements();
    void make_deposits();
	void logincountreport();
	void loginstowipereport();
	void modemtotalsreport();
	void modemsummaryreport();
	void bandwidthsummaryreport();
	void loginavgreport();
	void rateplanreport();
	void arreport();
	void cityreport();
	void usersonlinereport();
	void calllogreport();
	void opencalllogbrowser();
	void targetedemail();
    void openScheduler();
	
	void checkDBIntegrity();
	
	void updateReminders();
	void reminderSelected(QListViewItem *item);

    void about()    { showAbout(); };

protected slots:
    void    openCustomerMW(long);
    void    refreshCustomerMW(long);

signals:
    void    refreshCustomer(long);

protected:
    void    resizeEvent( QResizeEvent * );
    QPopupMenu  *optionsMenu;
    QPopupMenu *activities_menu;
    QPopupMenu *lists_menu;

private:
    QListView   *reminders;
    // QMenuBar   *menu;
    
    QTimer      *timer;
    
    // Vendor Types List
    // QDialog     *VendTypesWindow;
};


#endif // ADMIN_H
