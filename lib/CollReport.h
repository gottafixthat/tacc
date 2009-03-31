/*
***************************************************************************
**
** CollReport.h - Collections Report Utility Class
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2005, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#ifndef COLLREPORT_H
#define COLLREPORT_H

#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <BlargDB.h>

class CollReport
{
public:
    CollReport();
    ~CollReport();

    void printReport(long CustID);
    
protected:
    void printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance);
    void printFooter(QPainter *p, int PageNo);
    void registerHeader(QPainter *p);

    // The positions of our columns
    int         transDateX1;
    int         transDateX2;
    int         dueDateX1;
    int         dueDateX2;
    int         descriptionX1;
    int         descriptionX2;
    int         amountX1;
    int         amountX2;
    int         balanceX1;
    int         balanceX2;
};


#endif // COLLREPORT_H

