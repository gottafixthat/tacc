/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef LoginsToWipeReport_included
#define LoginsToWipeReport_included

#include "Report.h"

class LoginsToWipeReport : public Report
{
    Q_OBJECT

public:

    LoginsToWipeReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~LoginsToWipeReport();

    virtual void refreshReport();
    virtual void refreshCustomer(long);

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);

};
#endif // LoginsToWipeReport_included
