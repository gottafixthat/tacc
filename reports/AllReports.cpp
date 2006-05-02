/*
** $Id$
**
***************************************************************************
**
** AllReports - A class that encapsulates all of the standalone reports.
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
** $Log: AllReports.cpp,v $
** Revision 1.5  2004/01/04 19:58:44  marc
** New report, domains that are active but are not setup for DNS management.
**
** Revision 1.4  2004/01/04 19:35:48  marc
** New report, Managed DNS domains
**
** Revision 1.3  2003/12/10 00:50:54  marc
** Minor changes to menus and fixed the startup application font for SuSE.
**
** Revision 1.2  2003/12/07 17:49:37  marc
** New report, AllDomains.
**
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "AllReports.h"

#include <TAATools.h>

AllReports::AllReports
(
    QWidget *parent,
    const char *name
) : TAAWidget(parent, name)
{
    hide();
    QPopupMenu  *domainMenu = new QPopupMenu(this);
    if (isManager()) domainMenu->insertItem("&Active Domains", this, SLOT(showActiveDomainsReport()));
    if (isManager()) domainMenu->insertItem("A&ll Domains", this, SLOT(showAllDomainsReport()));
    if (isManager()) domainMenu->insertItem("&Managed DNS Domains", this, SLOT(showDNSManagedReport()));
    if (isManager()) domainMenu->insertItem("&Unmanaged DNS Domains", this, SLOT(showDNSUnmanagedReport()));
    domainMenu->insertItem("Un&released Domains", this, SLOT(showUnreleasedDomainsReport()));
    reportMenu = new QPopupMenu(this, "Reports Menu");
    reportMenu->insertItem("&Domains", domainMenu);
    if (isAdmin()) {
        QPopupMenu *salesMenu = new QPopupMenu(this);
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

QPopupMenu *AllReports::menu()
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

