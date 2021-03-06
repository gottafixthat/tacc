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

#ifndef RatePlanDetailReport_included
#define RatePlanDetailReport_included

#include "Report.h"

class RatePlanDetailReport : public Report
{
    Q_OBJECT

public:

    RatePlanDetailReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RatePlanDetailReport();

    virtual void refreshReport();

    void    setRatePlanID(long newPlanID);

public slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);

private:
    long    myRatePlanID;

};
#endif // RatePlanDetailReport_included

// vim: expandtab
