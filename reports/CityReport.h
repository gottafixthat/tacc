/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef CityReport_included
#define CityReport_included

#include "Report.h"

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
    virtual void    applyFilters(int newShowActive);
    
private:
    int     myShowActive;
};
#endif // CityReport_included
