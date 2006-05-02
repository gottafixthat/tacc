/**************************************************************************
**
** SalesReports - Creates reports on sales statistics.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/


#ifndef SalesReports_included
#define SalesReports_included

#include "Report.h"

class salesByServiceReport : public Report
{
    Q_OBJECT

public:

    salesByServiceReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~salesByServiceReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    
};
#endif // SalesReports_included
