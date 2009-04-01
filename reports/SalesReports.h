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
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    
};

class salesByServiceDetailReport : public Report
{
    Q_OBJECT

public:

    salesByServiceDetailReport(QWidget *parent = NULL, const char *name = NULL);
    virtual ~salesByServiceDetailReport();

    virtual void setBillableItem(long billableID, int isPackage = 0);
    virtual void refreshReport();

public slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);

protected:
    int     custIDCol;
    long    myBillableID;
    int     myIsPackage;
};
#endif // SalesReports_included

// vim: expandtab

