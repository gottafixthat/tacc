/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef RatePlanReport_included
#define RatePlanReport_included

#include "Report.h"

class RatePlanReport : public Report
{
    Q_OBJECT

public:

    RatePlanReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RatePlanReport();

    virtual void refreshReport();

public slots:
    virtual void listItemSelected(QListViewItem *curItem);

};
#endif // RatePlanReport_included
