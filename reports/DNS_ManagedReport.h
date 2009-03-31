/*
** $Id: DNS_ManagedReport.h,v 1.2 2004/01/12 23:26:25 marc Exp $
**
***************************************************************************
**
** DNS_ManagedReport - Displays a list of all of the DNS zones we are
**                     managing via MyDNS.
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
** $Log: DNS_ManagedReport.h,v $
** Revision 1.2  2004/01/12 23:26:25  marc
** Added a DNS button as a user button as a shortcut to editing DNS records.
**
** Revision 1.1  2004/01/04 19:35:48  marc
** New report, Managed DNS domains
**
**
*/


#ifndef DNS_ManagedReport_included
#define DNS_ManagedReport_included

#include "Report.h"

class DNS_ManagedReport : public Report
{
    Q_OBJECT

public:

    DNS_ManagedReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DNS_ManagedReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    virtual void    userButtonClickedInt();
    
};
#endif // DNS_ManagedReport_included
