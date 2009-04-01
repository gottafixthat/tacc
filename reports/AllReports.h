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

#ifndef ALLREPORTS_H
#define ALLREPORTS_H

#include <Qt3Support/q3popupmenu.h>

#include <TAAWidget.h>

#include "CityReport.h"
#include "LoginAvgReport.h"
#include "RatePlanReport.h"
#include "ModemUsageReport.h"
#include "ModemTotalsReport.h"
#include "BandwidthSummaryReport.h"
#include "ARReport.h"
#include "LoginCountReport.h"
#include "LoginsToWipeReport.h"
#include "UnreleasedDomainsReport.h"
#include "AllDomainsReport.h"
#include "DNS_ManagedReport.h"
#include "DNS_UnmanagedReport.h"
#include "ActiveDomainsReport.h"
#include "CallLogReport.h"
#include "AsteriskCDRReport.h"
#include "SalesReports.h"
#include <ccPaymentsByTypeReport.h>
#include <ccPaymentDetailReport.h>
#include <CheckPaymentsReport.h>
#include <GeneralLedgerReport.h>
#include <AgingReport.h>

class AllReports : public TAAWidget
{
    Q_OBJECT

public:
    AllReports(QWidget *parent = NULL, const char *name = NULL);
    virtual     ~AllReports();

    Q3PopupMenu  *menu();

public slots:
    void        showLoginCountReport();
    void        showLoginAverageReport();
    void        showRatePlanReport();
    void        showCityReport();
    void        showARReport();
    void        showLoginsToWipeReport();
    void        showUnreleasedDomainsReport();
    void        showAllDomainsReport();
    void        showDNSManagedReport();
    void        showDNSUnmanagedReport();
    void        showActiveDomainsReport();
    void        showCallLogReport();
    void        showAsteriskCDRReport();
    void        showSalesByServiceReport();
    void        showCcPaymentsByTypeReport();
    void        showCcPaymentDetailReport();
    void        showCheckPaymentsReport();
    void        showGeneralLedgerReport();
    void        showAgingReport();

private:
    Q3PopupMenu  *reportMenu;
};

#endif /* ALLREPORTS_H */

// vim: expandtab
