/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef ARReport_included
#define ARReport_included

#include "Report.h"

class ARReport : public Report
{
    Q_OBJECT

public:

    ARReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ARReport();

    virtual void refreshReport();

};
#endif // ARReport_included
