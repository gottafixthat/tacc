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

#ifndef Report_included
#define Report_included

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3listview.h>
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

typedef enum { d_thisMonth, d_lastMonth, d_today, d_yesterday, d_thisWeek, d_lastWeek, d_thisYear, d_lastYear, d_thisQuarter, d_lastQuarter} repDates;


class EmailReportDialog : public QDialog
{
    Q_OBJECT
public:
    EmailReportDialog(QWidget *parent = NULL, const char *name = NULL);
    virtual ~EmailReportDialog();

    QString emailAddress();
    bool    doPlainText();
    bool    attachCSV();

protected:
    QLineEdit   *emailAddr;
    QCheckBox   *includePlainText;
    QCheckBox   *includeCSV;
};

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

    virtual void editFilters()          {};
    virtual void listItemSelected(Q3ListViewItem *)      {};
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
    Q3ListView   *repBody;
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

/**
 * ReportFilter()
 *
 * Base class for a report filter.  Includes some convenience functions.
 */
class ReportFilter : public TAAWidget
{
    Q_OBJECT

public:
    ReportFilter(QWidget *parent = NULL, const char *name = NULL);
    virtual ~ReportFilter();

    void            setReportName(const QString);
    void            setAllowSave(int allow);

signals:
    void            optionsUpdated();

protected slots:
    virtual void    loadFilters(const QString)  {};
    virtual void    loadClicked();
    virtual void    saveFilters()               {};
    virtual void    updateClicked()             { emit(optionsUpdated()); };
    virtual void    closeClicked()              { hide(); };

protected:
    QLabel          *saveListLabel;
    QComboBox       *saveList;
    QPushButton     *loadButton;

    QPushButton     *saveButton;
    QPushButton     *updateButton;
    QPushButton     *closeButton;

    Q3BoxLayout      *ml;
    Q3BoxLayout      *bl;

    int             myAllowSave;
    QString         myReportName;

    void            loadSavedReportList();
};

#endif // Report_included

// vim: expandtab
