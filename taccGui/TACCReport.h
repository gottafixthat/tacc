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

#ifndef TACCREPORT_H
#define TACCREPORT_H

#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QCloseEvent>
#include <QtGui/QTreeWidget>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3datetimeedit.h>

#include <TAAWidget.h>
#include <calendar.h>

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


class TACCReportItem : public QTreeWidgetItem
{

public:
    TACCReportItem(QTreeWidget *parent);
    virtual ~TACCReportItem();
    enum {rString = 0, rInt = 1, rDouble = 2} rSortTypes;

    bool operator<(const QTreeWidgetItem &other) const;

    void setSortType(int col, int sType);
    void setIsTotalLine(bool tot);

private:
    bool    myIsTotalLine;
    QMap <int, int> colSortMethod;

};

class TACCReport : public TAAWidget
{
    Q_OBJECT

public:

    TACCReport(QWidget* parent = NULL, Qt::WFlags f = Qt::Window);
    virtual ~TACCReport();
    enum repDates { d_thisMonth, d_lastMonth, d_today, d_yesterday, d_thisWeek, d_lastWeek, d_thisYear, d_lastYear, d_thisQuarter, d_lastQuarter};

    void setTitle(const char * newTitle);

    QDate startDate();
    QDate endDate(); 
    void  setStartDate(const QDate newDate);
    void  setEndDate(const QDate newDate);
    void  setDateRange(repDates newRange);
    
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

    virtual void editFilters()                              {};
    virtual void listItemSelected(QTreeWidgetItem *, int)   {};
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
    QTreeWidget *repBody;
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
    Q3DateEdit   *startDateCal;
    Q3DateEdit   *endDateCal;
    Q3TextView   *userText;

    // The layouts so that they can be added to by things that inherit from us
    Q3BoxLayout      *dl;    // Date Layout (the date bar)
    Q3BoxLayout      *bl;    // Button Layout (the button bar)

private:
    // DateInput       *startDateCal;
    // DateInput       *endDateCal;
    QDate   myStartDate;
    QDate   myEndDate;
    
    int     myAllowDates;
    int     myAllowFilters;
    int     myAllowGraph;
};


#endif // Report_included

// vim: expandtab
