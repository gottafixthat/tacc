/*
** $Id: AllDomainsReport.h,v 1.1 2003/12/07 17:49:37 marc Exp $
**
***************************************************************************
**
** AllDomainsReport - Displays the list of all of the domains that TAA has
**                    in the databse.  It allows the user to double click
**                    an item and bring up the customer window associated  
**                    with the domain.
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
*/


#ifndef AllDomainsReport_included
#define AllDomainsReport_included

#include "Report.h"

class AllDomainsReport : public Report
{
    Q_OBJECT

public:

    AllDomainsReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AllDomainsReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    
};
#endif // AllDomainsReport_included
