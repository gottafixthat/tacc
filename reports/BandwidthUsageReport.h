/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef BandwidthUsageReport_included
#define BandwidthUsageReport_included

#include "Report.h"

#include <qintdict.h>
#include <ADB.h>

class BandwidthUsageReport : public Report
{
    Q_OBJECT

public:

    BandwidthUsageReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BandwidthUsageReport();
    
    void    setLoginID(const char *LoginID);

    virtual void refreshReport();
    
private:
    char    myLoginID[32];

    llong  *inDays;
    llong  *outDays;
    llong  *totDays;

};
#endif // BandwidthUsageReport_included
