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


#ifndef LoginCountReport_included
#define LoginCountReport_included

#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>

#include "Report.h"

class LoginCountReport : public Report
{
    Q_OBJECT

public:

    LoginCountReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~LoginCountReport();

    virtual void refreshReport();

protected slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);
    virtual void QCloseEvent(QEvent *)              { delete this; }

};
#endif // LoginCountReport_included

// vim: expandtab
