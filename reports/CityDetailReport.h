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

#ifndef CityDetailReport_included
#define CityDetailReport_included

#include "Report.h"

class CityDetailReport : public Report
{
    Q_OBJECT

public:

    CityDetailReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CityDetailReport();

    virtual void refreshReport();
    
    void    setCityState(const char *newCity, const char *newState);

public slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);

private:
    char    myCity[512];
    char    myState[512];

};
#endif // CityDetailReport_included

// vim: expandtab
