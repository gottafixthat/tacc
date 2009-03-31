/*
** $Id: ActiveDomainsReport.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** ActiveDomainsReport - Displays a summary of active domain types.
**                       This allows the user to double click on a domain
**                       type and bring up the list of domains in that
**                       type.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2002, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/


#ifndef ActiveDomainsReport_included
#define ActiveDomainsReport_included

#include "Report.h"

class ActiveDomainsReport : public Report
{
    Q_OBJECT

public:

    ActiveDomainsReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ActiveDomainsReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // ActiveDomainsReport_included
