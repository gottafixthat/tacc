/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef ModemTotalsReport_included
#define ModemTotalsReport_included

#include <Qt3Support/q3dict.h>
#include <Qt3Support/q3intdict.h>
#include <Qt3Support/q3strlist.h>

#include "Report.h"

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

// vim: expandtab
