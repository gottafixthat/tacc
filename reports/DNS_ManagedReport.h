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

#ifndef DNS_ManagedReport_included
#define DNS_ManagedReport_included

#include "Report.h"

class DNS_ManagedReport : public Report
{
    Q_OBJECT

public:

    DNS_ManagedReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DNS_ManagedReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    virtual void    userButtonClickedInt();
    
};
#endif // DNS_ManagedReport_included

// vim: expandtab
