/*
** AllReports.h - A way for admin.cpp to include all reports without having
**                to include the individual report files directly.
*/

#ifndef ALLREPORTS_H
#define ALLREPORTS_H

#include "CityReport.h"
#include "LoginAvgReport.h"
#include "UsersOnlineReport.h"
#include "RatePlanReport.h"
#include "ModemUsageReport.h"
#include "ModemSummaryReport.h"
#include "ModemTotalsReport.h"
#include "BandwidthSummaryReport.h"
#include "ARReport.h"
#include "CustCallLogReport.h"
#include "CustCallDetail.h"
#include "LoginCountReport.h"
#include "LoginsToWipeReport.h"
#include "UnreleasedDomainsReport.h"
#include "AllDomainsReport.h"
#include "DNS_ManagedReport.h"
#include "DNS_UnmanagedReport.h"
#include "ActiveDomainsReport.h"

#include <TAAWidget.h>
#include <qpopupmenu.h>

class AllReports : public TAAWidget
{
    Q_OBJECT

public:
    AllReports(QWidget *parent = NULL, const char *name = NULL);
    virtual     ~AllReports();

    QPopupMenu  *menu();

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

private:
    QPopupMenu  *reportMenu;
};

#endif /* ALLREPORTS_H */
