/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef ModemUsageReport_included
#define ModemUsageReport_included

#include "Report.h"

#include <qintdict.h>

class ModemUsageReport : public Report
{
    Q_OBJECT

public:

    ModemUsageReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ModemUsageReport();
    
    void    setLoginID(const char *LoginID);

    virtual void refreshReport();
    
private:
    char    myLoginID[32];

    void    loadDict();

    QIntDict<char> strxlt;
};
#endif // ModemUsageReport_included
