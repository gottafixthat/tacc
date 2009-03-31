/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef BandwidthSummaryReport_included
#define BandwidthSummaryReport_included

#include "Report.h"

#include <q3intdict.h>
#include <ADB.h>

class BandwidthSummaryReport : public Report
{
    Q_OBJECT

public:

    BandwidthSummaryReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BandwidthSummaryReport();
    
    virtual void refreshReport();

    virtual void listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // BandwidthSummaryReport_included
