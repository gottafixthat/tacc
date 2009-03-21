/**
 * CityReport.h - Gives a report for customers broken down by city.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CityReport_included
#define CityReport_included

#include <Report.h>
#include <CityFilters.h>

class CityReport : public Report
{
    Q_OBJECT

public:

    CityReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CityReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    virtual void    editFilters();    
    
private:
    CityFilters     *filters;
    int     myShowActive;
};
#endif // CityReport_included
