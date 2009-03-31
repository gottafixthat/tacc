/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef ModemTotalsReport_included
#define ModemTotalsReport_included

#include "Report.h"

#include <q3dict.h>
#include <q3intdict.h>
#include <q3strlist.h>

struct ModemTotalStruct {
    int  loginCount;
    long totalTime;
};

class ModemTotalsReport : public Report
{
    Q_OBJECT

public:

    ModemTotalsReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ModemTotalsReport();
    
    virtual void refreshReport();

    virtual void listItemSelected(Q3ListViewItem *curItem);
    
private:
    Q3Dict<ModemTotalStruct> loginDict;
    Q3StrList                loginList;
};
#endif // ModemTotalsReport_included
