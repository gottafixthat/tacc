/*
** $Id: LoginCountReport.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** LoginCountReport - Generates a report of the number of active logons on
**                    the system.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: LoginCountReport.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef LoginCountReport_included
#define LoginCountReport_included

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
    virtual void listItemSelected(QListViewItem *curItem);
    virtual void QCloseEvent(QEvent *)              { delete this; }

};
#endif // LoginCountReport_included
