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

#ifndef UnreleasedDomainsReport_included
#define UnreleasedDomainsReport_included

#include "Report.h"

class UnreleasedDomainsReport : public Report
{
    Q_OBJECT

public:

    UnreleasedDomainsReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~UnreleasedDomainsReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // UnreleasedDomainsReport_included

// vim: expandtab

