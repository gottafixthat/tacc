/**********************************************************************

	--- Qt Architect generated file ---

	File: CityFilters.h
	Last generated: Sun Apr 11 13:58:51 1999

 *********************************************************************/

#ifndef CityFilters_included
#define CityFilters_included

#include "CityFiltersData.h"

class CityFilters : public CityFiltersData
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

};
#endif // CityFilters_included
