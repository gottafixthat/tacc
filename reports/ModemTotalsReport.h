/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef ModemTotalsReport_included
#define ModemTotalsReport_included

#include "Report.h"

#include <qdict.h>
#include <qintdict.h>
#include <qstrlist.h>

typedef struct ModemTotalStruct {
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

    virtual void listItemSelected(QListViewItem *curItem);
    
private:
    QDict<ModemTotalStruct> loginDict;
    QStrList                loginList;
};
#endif // ModemTotalsReport_included
