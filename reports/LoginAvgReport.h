/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef LoginAvgReport_included
#define LoginAvgReport_included

#include "Report.h"

class LoginAvgReport : public Report
{
    Q_OBJECT

public:

    LoginAvgReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~LoginAvgReport();

    virtual void refreshReport();

};
#endif // LoginAvgReport_included
