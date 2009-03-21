/**
 * CityFilters.h - Filters for the Customer Cities report.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CityFilters_included
#define CityFilters_included

#include <qradiobutton.h>

#include <Report.h>

class CityFilters : public ReportFilter
{
    Q_OBJECT

public:

    CityFilters
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CityFilters();
    
    void    setActiveStatus(int newStatus);
    int     getActiveStatus(void);
    
signals:
    void    filtersChanged(int);

public slots:
    virtual void cancelFilters();
    virtual void applyFilters();
    virtual void applyAndExit();

protected:
    QRadioButton    *activeOnlyButton;
    QRadioButton    *inactiveOnlyButton;
    QRadioButton    *bothButton;
};
#endif // CityFilters_included
