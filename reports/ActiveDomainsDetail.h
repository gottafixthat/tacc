/*
** $Id: ActiveDomainsDetail.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** ActiveDomainsDetail - Displays a list of active domains of a particular
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


#ifndef ActiveDomainsDetail_included
#define ActiveDomainsDetail_included

#include "Report.h"

class ActiveDomainsDetail : public Report
{
    Q_OBJECT

public:

    ActiveDomainsDetail
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ActiveDomainsDetail();

    virtual void refreshReport();
    virtual void setDomainType(int);

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);

protected:
    int         myDomainType;
    int         myCustIDColumn;
    
};
#endif // ActiveDomainsDetail_included
