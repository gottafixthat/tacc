/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

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
