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

#include <Qt3Support/Q3PopupMenu>

#include <TAATools.h>

#include "AllReports.h"

using namespace Qt;

AllReports::AllReports
(
    QWidget *parent,
    const char *name
) : TAAWidget(parent, name)
{
    hide();

    // Our main report menu
    reportMenu = new Q3PopupMenu(this, "Reports Menu");

    // Manager reports, billing and stuff.
    if (isManager()) {
        Q3PopupMenu  *billingReports = new Q3PopupMenu(this);
        billingReports->insertItem("Credit Card Payments by Type", this, SLOT(showCcPaymentsByTypeReport()));
        billingReports->insertItem("Check Payments", this, SLOT(showCheckPaymentsReport()));
        billingReports->insertItem("Aging Report", this, SLOT(showAgingReport()));

        reportMenu->insertItem("&Billing Reports", billingReports);
        reportMenu->insertItem("G&eneral Ledger", this, SLOT(showGeneralLedgerReport()));
    }
    
    // DNS/Domain reports
    Q3PopupMenu  *domainMenu = new Q3PopupMenu(this);
    if (isManager()) {
        domainMenu->insertItem("&Active Domains", this, SLOT(showActiveDomainsReport()));
        domainMenu->insertItem("A&ll Domains", this, SLOT(showAllDomainsReport()));
        domainMenu->insertItem("&Managed DNS Domains", this, SLOT(showDNSManagedReport()));
        domainMenu->insertItem("&Unmanaged DNS Domains", this, SLOT(showDNSUnmanagedReport()));
    }
    domainMenu->insertItem("Un&released Domains", this, SLOT(showUnreleasedDomainsReport()));
    reportMenu->insertItem("&Domains", domainMenu);

    if (isAdmin()) {
        Q3PopupMenu *salesMenu = new Q3PopupMenu(this);
        salesMenu->insertItem("By Service", this, SLOT(showSalesByServiceReport()));

        reportMenu->insertItem("Call Lo&gs", this, SLOT(showCallLogReport()));
        reportMenu->insertItem("Call Detail Records", this, SLOT(showAsteriskCDRReport()));
        reportMenu->insertItem("Login &Counts (current)", this, SLOT(showLoginCountReport()));
        reportMenu->insertItem("Login Counts (&monthly average)", this, SLOT(showLoginAverageReport()));
        reportMenu->insertItem("&Rate Plans", this, SLOT(showRatePlanReport()));
        reportMenu->insertItem("&Sales", salesMenu);
        reportMenu->insertItem("&User Cities", this, SLOT(showCityReport()));
        reportMenu->insertSeparator();
        reportMenu->insertItem("&Accounts Receivable", this, SLOT(showARReport()));
        reportMenu->insertSeparator();
        reportMenu->insertItem("&Logins to Wipe", this, SLOT(showLoginsToWipeReport()));
    }
}

AllReports::~AllReports()
{
}

/*
** menu - Returns the menu we have created.
*/

Q3PopupMenu *AllReports::menu()
{
    return  reportMenu;
}

/*
** showLoginCountReport - Shows the current login counts.
*/

void AllReports::showLoginCountReport()
{
    emit(setStatus("Generating report..."));
    LoginCountReport    *rep;
    rep = new LoginCountReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showLoginAverageReport - Shows the average login counts.
*/

void AllReports::showLoginAverageReport()
{
    emit(setStatus("Generating report..."));
    LoginAvgReport    *rep;
    rep = new LoginAvgReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showRatePlanReport - Displays the rate plan report
*/

void AllReports::showRatePlanReport()
{
    emit(setStatus("Generating report..."));
    RatePlanReport    *rep;
    rep = new RatePlanReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showCityReport - Displays the customer city report
*/

void AllReports::showCityReport()
{
    emit(setStatus("Generating report..."));
    CityReport  *rep;
    rep = new CityReport();
    rep->show();
    emit(setStatus(""));
}


/*
** showARReport - Displays the accounts receivable report
*/

void AllReports::showARReport()
{
    emit(setStatus("Generating report..."));
    ARReport    *rep;
    rep = new ARReport();
    rep->show();
    emit(setStatus(""));
}


/*
** showLoginsToWipeReport - Displays the list of logins that need to be
**                          wiped from the system.
*/

void AllReports::showLoginsToWipeReport()
{
    LoginsToWipeReport    *LTWR;

    LTWR = new LoginsToWipeReport;
    LTWR->show();
}

/*
** showUnreleasedDomainsReport - Displays the Unreleased Domains report
*/

void AllReports::showUnreleasedDomainsReport()
{
    emit(setStatus("Generating report..."));
    UnreleasedDomainsReport *rep;
    rep = new UnreleasedDomainsReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showAllDomainsReport - Displays the All Domains report
*/

void AllReports::showAllDomainsReport()
{
    emit(setStatus("Generating report..."));
    AllDomainsReport *rep;
    rep = new AllDomainsReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showDNSManagedReport - Displays the All Domains report
*/

void AllReports::showDNSManagedReport()
{
    emit(setStatus("Generating report..."));
    DNS_ManagedReport *rep;
    rep = new DNS_ManagedReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showDNSUnmanagedReport - Displays the unmanaged DNS Domain Report
*/

void AllReports::showDNSUnmanagedReport()
{
    emit(setStatus("Generating report..."));
    DNS_UnmanagedReport *rep;
    rep = new DNS_UnmanagedReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showActiveDomainsReport - Displays the Active Domains report
*/

void AllReports::showActiveDomainsReport()
{
    emit(setStatus("Generating report..."));
    ActiveDomainsReport *rep;
    rep = new ActiveDomainsReport();
    rep->show();
    emit(setStatus(""));
}

/*
** showCallLogReport - Shows the call logs report
*/

void AllReports::showCallLogReport()
{
    emit(setStatus("Generating report..."));
    CallLogReport    *rep;
    rep = new CallLogReport();
    rep->show();
    emit(setStatus(""));
}

/** showAsteriskCDRReport - Shows the Asterisk Call Detail Records report.
  */
void AllReports::showAsteriskCDRReport()
{
    emit(setStatus("Generating report..."));
    AsteriskCDRReport   *rep;
    rep = new AsteriskCDRReport();
    rep->show();
    emit(setStatus(""));
}

/** showSalesByServiceReport - Shows a report for our sales by service.
  */
void AllReports::showSalesByServiceReport()
{
    QApplication::setOverrideCursor(waitCursor);
    emit(setStatus("Generating report..."));
    salesByServiceReport   *rep;
    rep = new salesByServiceReport();
    rep->show();
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
}

/**
 * showCcPaymentsByTypeReport()
 *
 * Creates a ccPaymentsByType report.
 */
void AllReports::showCcPaymentsByTypeReport()
{
    emit(setStatus("Generating report..."));
    ccPaymentsByTypeReport  *rep;
    rep = new ccPaymentsByTypeReport();
    rep->show();
    emit(setStatus(""));
}

/**
 * showCcPaymentDetailReport()
 *
 * Creates a ccPaymentDetail report.
 */
void AllReports::showCcPaymentDetailReport()
{
    emit(setStatus("Generating report..."));
    ccPaymentDetailReport  *rep;
    rep = new ccPaymentDetailReport();
    rep->show();
    emit(setStatus(""));
}

/**
 * showCheckPaymentsReport()
 *
 * Creates a CheckPayments report.
 */
void AllReports::showCheckPaymentsReport()
{
    emit(setStatus("Generating report..."));
    CheckPaymentsReport  *rep;
    rep = new CheckPaymentsReport();
    rep->show();
    emit(setStatus(""));
}

/**
 * showGeneralLedgerReport()
 *
 * Creates a GeneralLedger report.
 */
void AllReports::showGeneralLedgerReport()
{
    emit(setStatus("Generating report..."));
    GeneralLedgerReport  *rep;
    rep = new GeneralLedgerReport();
    rep->show();
    emit(setStatus(""));
}

/**
 * showAgingReport()
 *
 * Creates an Aging report.
 */
void AllReports::showAgingReport()
{
    emit(setStatus("Generating report..."));
    AgingReport *rep;
    rep = new AgingReport();
    rep->show();
    emit(setStatus(""));
}

// vim: expandtab
