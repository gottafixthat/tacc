/*
** $Id: Report.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** Report.h   - A base class for generic reports.  Defines the bulk of the
**              window manipulation and allows generic printing, emailing
**              and graphing.
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
** $Log: Report.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef Report_included
#define Report_included

#include <TAAWidget.h>

#include <qdatetm.h>
#include <calendar.h>
#include <qpainter.h>
#include <qlistview.h>
#include <qtextview.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include "calendar.h"

#ifdef USEQWT
#include "Graph.h"
// #include <qwt_plot.h>
#endif

// Bitwise values for showing the date buttons and stuff
#define REP_NODATES     0
#define REP_STARTDATE   1
#define REP_ENDDATE     2
#define REP_DATELIST    4
#define REP_GENERATE  128
#define REP_ALLDATES  255



class Report : public TAAWidget
{
    Q_OBJECT

public:

    Report
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~Report();

    void setTitle(const char * newTitle);

    QDate startDate();
    QDate endDate(); 
    void  setStartDate(const QDate newDate);
    void  setEndDate(const QDate newDate);
    
    void  allowDates(int newSetting);
    void  allowFilters(int newSetting);
    void  allowReproduction(int newSetting);
    void  allowGraph(int newSetting);
    void  setUserButton(const char *newText);

signals:
    void  userButtonClicked();

public slots:
    virtual void graphReport();
    virtual void printReport();
    virtual void emailReport();
    virtual void dateRangeSelected(const char *newRange);
    virtual void dateRangeSelected(const QString &newRange);
    virtual void userButtonClickedInt();

    virtual void editFilters()          {};
    virtual void listItemSelected(QListViewItem *)      {};
#ifdef USEQWT
//    virtual void plotGraph(QwtPlot *plot);
#endif
    
protected slots:
    virtual void editStartDate();
    virtual void editEndDate();
    virtual void closeReport();
    virtual void closeEvent(QCloseEvent *);
    virtual void refreshReport()                    {};

protected:
    QLabel      *reportTitle;
    QListView   *list;
    QLabel      *startDateLabel;
    QLabel      *endDateLabel;
    QPushButton *generateButton;
    QPushButton *refreshButton;
    QPushButton *closeButton;
    QPushButton *emailButton;
    QPushButton *printButton;
    QPushButton *graphButton;
    QPushButton *filterButton;
    QComboBox   *dateList;
    QPushButton *userButton;
    DateInput   *startDateCal;
    DateInput   *endDateCal;
    QTextView   *userText;

private:
    void printReportHeader(QPainter *p);
    void printReportFooter(QPainter *p, int PageNo, int totPages);
    QListViewItem * printReportRows(QPainter *p, QListViewItem *startItem);

    // DateInput       *startDateCal;
    // DateInput       *endDateCal;
    QDate   myStartDate;
    QDate   myEndDate;
    
    // This is used for printing.
    int     prColWidths[20];
    
    int     myAllowDates;
    int     myAllowFilters;
    int     myAllowGraph;
};
#endif // Report_included
