/*
** $Id: DNS_UnmanagedReport.h,v 1.1 2004/01/04 19:59:06 marc Exp $
**
***************************************************************************
**
** DNS_UnmanagedReport - Displays a list of all of the domains that we are
**                       NOT managing via MyDNS.
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
** $Log: DNS_UnmanagedReport.h,v $
** Revision 1.1  2004/01/04 19:59:06  marc
** New report, unmanaged active domains.
**
**
*/


#ifndef DNS_UnmanagedReport_included
#define DNS_UnmanagedReport_included

#include "Report.h"

class DNS_UnmanagedReport : public Report
{
    Q_OBJECT

public:

    DNS_UnmanagedReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DNS_UnmanagedReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    
};
#endif // DNS_UnmanagedReport_included
