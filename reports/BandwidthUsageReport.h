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

#ifndef BandwidthUsageReport_included
#define BandwidthUsageReport_included

#include <Qt3Support/q3intdict.h>

#include <ADB.h>

#include "Report.h"

class BandwidthUsageReport : public Report
{
    Q_OBJECT

public:

    BandwidthUsageReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BandwidthUsageReport();
    
    void    setLoginID(const char *LoginID);

    virtual void refreshReport();
    
private:
    char    myLoginID[32];

    llong  *inDays;
    llong  *outDays;
    llong  *totDays;

};
#endif // BandwidthUsageReport_included

// vim: expandtab
