/*
** $Id: UnreleasedDomainsReport.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** UnreleasedDomainsReport - Displays a list of unreleased active domains
**                           and allows the user to double click on an item
**                           to bring up the customer window.
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


#ifndef UnreleasedDomainsReport_included
#define UnreleasedDomainsReport_included

#include "Report.h"

class UnreleasedDomainsReport : public Report
{
    Q_OBJECT

public:

    UnreleasedDomainsReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~UnreleasedDomainsReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    
};
#endif // UnreleasedDomainsReport_included
