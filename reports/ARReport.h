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

// vim: expandtab
