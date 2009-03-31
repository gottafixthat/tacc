/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef RatePlanDetailReport_included
#define RatePlanDetailReport_included

#include "Report.h"

class RatePlanDetailReport : public Report
{
    Q_OBJECT

public:

    RatePlanDetailReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RatePlanDetailReport();

    virtual void refreshReport();

    void    setRatePlanID(long newPlanID);

public slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);

private:
    long    myRatePlanID;

};
#endif // RatePlanDetailReport_included
