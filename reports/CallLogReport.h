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

#ifndef CallLogReport_included
#define CallLogReport_included

#include <QtCore/QEvent>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>

#include "Report.h"

class CallLogReport : public Report
{
    Q_OBJECT

public:

    CallLogReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CallLogReport();

    virtual void refreshReport();

protected:
    QComboBox    *answeredByList;
    QComboBox    *logTypeList;

protected slots:
    virtual void filterActivated(int);
    virtual void listItemSelected(Q3ListViewItem *curItem);
    virtual void QCloseEvent(QEvent *)              { delete this; }

};
#endif // CallLogReport_included

// vim: expandtab
