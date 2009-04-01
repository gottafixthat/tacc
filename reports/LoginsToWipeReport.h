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

#ifndef LoginsToWipeReport_included
#define LoginsToWipeReport_included

#include "Report.h"

class LoginsToWipeReport : public Report
{
    Q_OBJECT

public:

    LoginsToWipeReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~LoginsToWipeReport();

    virtual void refreshReport();
    virtual void refreshCustomer(long);

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);

};
#endif // LoginsToWipeReport_included

// vim: expandtab
