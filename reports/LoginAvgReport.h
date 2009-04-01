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

// vim: expandtab
