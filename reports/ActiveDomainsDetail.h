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

#ifndef ActiveDomainsDetail_included
#define ActiveDomainsDetail_included

#include "Report.h"

class ActiveDomainsDetail : public Report
{
    Q_OBJECT

public:

    ActiveDomainsDetail
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ActiveDomainsDetail();

    virtual void refreshReport();
    virtual void setDomainType(int);

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);

protected:
    int         myDomainType;
    int         myCustIDColumn;
    
};
#endif // ActiveDomainsDetail_included

// vim: expandtab
