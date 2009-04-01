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

#ifndef BandwidthSummaryReport_included
#define BandwidthSummaryReport_included


#include <Qt3Support/q3intdict.h>

#include <ADB.h>

#include "Report.h"

class BandwidthSummaryReport : public Report
{
    Q_OBJECT

public:

    BandwidthSummaryReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BandwidthSummaryReport();
    
    virtual void refreshReport();

    virtual void listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // BandwidthSummaryReport_included

// vim: expandtab
