/*
** $Id: HTMLReport.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** HTMLReport.h - Provides a class framework for creating reports in HTML.
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
** $Log: HTMLReport.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef HTMLReport_included
#define HTMLReport_included

#include "HTMLReportData.h"

// Report.h contains several definitions that we want.
#include "Report.h"

#include <qdatetm.h>

class HTMLReport : public HTMLReportData
{
    Q_OBJECT

public:

    HTMLReport(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~HTMLReport();

    QDate   startDate();
    QDate   endDate();
    
    void  setStartDate(const QDate newDate);
    void  setEndDate(const QDate newDate);
    
    void  allowDates(int newSetting);
    void  allowFilters(int newSetting);
    void  allowReproduction(int newSetting);
    void  allowGraph(int newSetting);
    void  setUserButton(const char *newText);

public slots:

    virtual void graphReport();
    virtual void printReportNew();
    virtual void printReport();
    virtual void emailReport();

protected slots:

    virtual void closeReport();
    virtual void editFilters();
    virtual void refreshReport();
    virtual void dateRangeSelected(const QString&);
    virtual void userButtonClickedInt();

private:
    QDate   myStartDate;
    QDate   myEndDate;

    int     myAllowDates;
    int     myAllowFilters;
    int     myAllowGraph;
};
#endif // HTMLReport_included
