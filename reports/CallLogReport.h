/*
** $Id: $
**
***************************************************************************
**
** CallLogReport - Given a date range, this shows the calls that were
**                 logged.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: $
**
*/


#ifndef CallLogReport_included
#define CallLogReport_included

#include "Report.h"
#include <qcombobox.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <QEvent>

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
