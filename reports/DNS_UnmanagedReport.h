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


#ifndef DNS_UnmanagedReport_included
#define DNS_UnmanagedReport_included

#include "Report.h"

class DNS_UnmanagedReport : public Report
{
    Q_OBJECT

public:

    DNS_UnmanagedReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DNS_UnmanagedReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // DNS_UnmanagedReport_included

// vim: expandtab
