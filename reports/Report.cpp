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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QtCore/QDateTime>
#include <QtCore/QRect>
#include <QtCore/QFile>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtGui/QCloseEvent>
#include <QtGui/QPrinter>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3progressdialog.h>

#include <calendar.h>
#include <ADB.h>
#include <QListViewPrint.h>
#include <EmailMessage.h>
#include <TAATools.h>
#include <Cfg.h>

#include "Report.h"

using namespace Qt;

Report::Report
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
	setCaption( "Report" );

    // Create our widgets and our layout
    startDateLabel = new QLabel(this);
    startDateLabel->setText("Start:");
    startDateLabel->setAlignment(AlignVCenter|AlignRight);

    startDateCal = new Q3DateEdit(QDate::currentDate(), this, "StartDate");
    
    endDateLabel = new QLabel(this);
    endDateLabel->setText("End:");
    endDateLabel->setAlignment(AlignVCenter|AlignRight);
    
    endDateCal = new Q3DateEdit(QDate::currentDate(), this, "StartDate");

    dateList = new QComboBox(false, this);
    dateList->insertItem("This Month");
    dateList->insertItem("Last Month");
    dateList->insertItem("Today");
    dateList->insertItem("Yesterday");
    dateList->insertItem("This Week");
    dateList->insertItem("Last Week");
    dateList->insertItem("This Year");
    dateList->insertItem("Last Year");
    dateList->insertItem("This Quarter");
    dateList->insertItem("Last Quarter");
    connect(dateList, SIGNAL(activated(const QString&)), this, SLOT(dateRangeSelected(const QString&)));


    generateButton = new QPushButton(this);
    generateButton->setText("&Generate");
    connect(generateButton, SIGNAL(clicked()), this, SLOT(refreshReport()));

    reportTitle = new QLabel(this);
    reportTitle->setText("reportTitle");
    reportTitle->setAlignment(AlignVCenter|AlignHCenter|ExpandTabs|WordBreak);

    repBody = new Q3ListView(this);
    repBody->addColumn("First Column");
    connect(repBody, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(listItemSelected(Q3ListViewItem *)));

    userText = new Q3TextView(this);
    //userText->setFocusPolicy(QWidget::NoFocus);

    // The action buttons...
    filterButton = new QPushButton(this);
    filterButton->setText("&Filters");
    connect(filterButton, SIGNAL(clicked()), this, SLOT(editFilters()));

    userButton = new QPushButton(this);
    userButton->setText("User");
    connect(userButton, SIGNAL(clicked()), this, SLOT(userButtonClickedInt()));

    graphButton = new QPushButton(this);
    graphButton->setText("&Graph");
    connect(graphButton, SIGNAL(clicked()), this, SLOT(graphReport()));

    refreshButton = new QPushButton(this);
    refreshButton->setText("&Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshReport()));

    printButton = new QPushButton(this);
    printButton->setText("&Print");
    connect(printButton, SIGNAL(clicked()), this, SLOT(printReport()));

    emailButton = new QPushButton(this);
    emailButton->setText("E&mail");
    connect(emailButton, SIGNAL(clicked()), this, SLOT(emailReport()));

    closeButton = new QPushButton(this);
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeReport()));


    // Now, do the layout for the widget.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // A layout for the top row where our dates and such are
    dl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    dl->addWidget(startDateLabel,   0);
    dl->addWidget(startDateCal,     1);
    dl->addWidget(endDateLabel,     0);
    dl->addWidget(endDateCal,       1);
    dl->addWidget(dateList,         1);
    dl->addStretch(1);
    dl->addWidget(generateButton,   0);
    
    ml->addLayout(dl,           0);
    ml->addWidget(reportTitle,  0);
    ml->addWidget(repBody,      1);
    ml->addWidget(userText,     0);
    
    // Add the button layout
    bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(filterButton, 0);
    bl->addWidget(userButton,   0);
    bl->addWidget(graphButton,  0);
    bl->addWidget(refreshButton,0);
    bl->addWidget(printButton,  0);
    bl->addWidget(emailButton,  0);
    bl->addWidget(closeButton,  0);

    ml->addLayout(bl,           0);

	
	// Start out by setting our start and end dates.
	QDate       tmpDate = QDate::currentDate();
	myStartDate.setYMD(tmpDate.year(), tmpDate.month(), 1);
	myEndDate.setYMD(  tmpDate.year(), tmpDate.month(), tmpDate.daysInMonth());

    // Now, set the text on the buttons.
    char tmpStr[512];
    sprintf(tmpStr, "%s %02d, %04d", (const char *) myStartDate.monthName(myStartDate.month()), myStartDate.day(), myStartDate.year());
    // startDateButton->setText(tmpStr);
    
    sprintf(tmpStr, "%s %02d, %04d", (const char *) myEndDate.monthName(myEndDate.month()), myEndDate.day(), myEndDate.year());
    // endDateButton->setText(tmpStr);

    startDateCal->setDate(myStartDate);
    endDateCal->setDate(myEndDate);
    
    allowDates(REP_ALLDATES);
    allowGraph(0);
    
    // By default, we don't want to allow filters.
    allowFilters(0);
    
    // If we have a parent, don't show the close button or the title
    if (parent) {
        closeButton->hide();
        reportTitle->hide();
    }

    userText->hide();       // Hide the user text as well.

    userButton->hide();     // By default, we don't have a user button
    
    allowReproduction(1);   // We can graph/print/email by default.

    repBody->setAllColumnsShowFocus(true);

}


Report::~Report()
{
}

/*
** closeEvent  - Deletes the CallLog window when the user requests to
**               close the window.
*/

void Report::closeEvent(QCloseEvent *)
{
    // Close the window and delete everything we own...
    delete this;
}



/*
** setTitle - Sets the title of the report.
*/

void Report::setTitle(const char * newTitle)
{
    reportTitle->setText(newTitle);
}

/*
** startDate  - Returns the current start date of the report.
*/

QDate Report::startDate()
{
    QDate   tmpDate = startDateCal->date();
    myStartDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myStartDate;
}

/*
** endDate  - Returns the current end date of the report.
*/

QDate Report::endDate()
{
    QDate   tmpDate = endDateCal->date();
    myEndDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myEndDate;
}

/*
** setStartDate - Sets the start date for the report.
*/

void Report::setStartDate(const QDate newDate)
{
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    startDateCal->setDate(newDate);
    myStartDate = newDate;
}

/*
** setEndDate - Sets the end date for the report.
*/

void Report::setEndDate(const QDate newDate)
{
    myEndDate = newDate;
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    endDateCal->setDate(newDate);
}


/*
** allowDates    - Toggles the dates on the window.  By turning it off, it
**                 prevents the user from seeing or showing the dates.
**                 Useful for "snapshot" reports like login counts.
*/

void Report::allowDates(int newSetting)
{
    myAllowDates = newSetting;
    // Start by hiding everything.
    startDateLabel->hide();
    startDateCal->hide();
    endDateLabel->hide();
    endDateCal->hide();
    generateButton->hide();
    dateList->hide();
    
    // If we have anything enabled, the generate button needs to be displayed.
    if (myAllowDates) generateButton->show();
    
    // Now, walk through the flags and enable anything that interests us.
    if (myAllowDates && REP_STARTDATE) {
        startDateLabel->show();
        startDateCal->show();
    }

    if (myAllowDates && REP_ENDDATE) {
        endDateLabel->show();
        endDateCal->show();
    }
    
    if (myAllowDates && REP_DATELIST) {
        dateList->show();
    }
}

/*
** allowFilters    - Toggles the filters button on the window.
*/

void Report::allowFilters(int newSetting)
{
    myAllowFilters = newSetting;
    if (myAllowFilters) {
        filterButton->show();
    } else {
        filterButton->hide();
    }
}


/*
** allowGraph   - Toggles the graph button on the window.
*/

void Report::allowGraph(int newSetting)
{
    myAllowGraph = newSetting;
    if (myAllowGraph) {
        graphButton->show();
    } else {
        graphButton->hide();
    }
}



/*
** closeReport   - Closes the current report.
*/

void Report::closeReport()
{
    delete this;
    //close();
}


/*
** graphReport   - Plots a generic graph of the data in the report in 
**                 a new window.
*/
void Report::graphReport()
{
#ifdef USEQWT
    Graph   *newGraph = new Graph(0, "Report Graph");
    plotGraph(newGraph->plotPtr());
    newGraph->show();
#endif
}

/*
** plotGraph     - A virtual function that must be overridden by the report
**                 if it wishes to plot a graph.
*/

#ifdef USEQWT
void Report::plotGraph(QwtPlot *plot)
{
    if (!plot) return;
}
#endif


/*
** printReport   - Creates a generic report and spits it out on the printer.
**
**                 This is a big, ugly, function, but it should produce 
**                 nice results.
*/

void Report::printReport()
{
    QListViewPrint  lvPrn(repBody);
    lvPrn.setTitle(reportTitle->text());
    if (myAllowDates) {
        QString tmpStr = "For the period from ";
        tmpStr += startDate().toString();
        tmpStr += " to ";
        tmpStr += endDate().toString();
        lvPrn.setDateLine(tmpStr);
    }
    lvPrn.print();
}



/*
** emailReport   - Creates a generic report and emails it to the current user.
*/

void Report::emailReport()
{
    uint    colWidths[20];
    uint    numCols;
    QString fmtStr;
    char    tmpStr[4096];
    QString tmpQstr;
    QString colStr;
    QString txtBody;
    QString txtStr;
    QString csvName;
    QDate   tmpDate = QDate::currentDate();
    QTime   tmpTime = QTime::currentTime();

    numCols = repBody->header()->count();
    
    if (!numCols) return;

    //debug(5,"QListViewToCSV() returned %d lines\n", QListViewToCSV(repBody, "/tmp/test.csv"));

    EmailReportDialog   *eOpts = new EmailReportDialog(this, "emailReportDialog");
    if (eOpts->exec() != QDialog::Accepted) {
        delete eOpts;
        return;
    }
    
    // Get the width of each header item.
    for (uint i = 0; i < numCols; i++) {
        colWidths[i] = repBody->header()->label(i).length();
    }

    // Now, get the longest item for each of the keys.
    Q3ListViewItem   *curItem;
    for (curItem = repBody->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        for (int i = 0; i < numCols; i++) {
            if (curItem->key(i, 0).length() > colWidths[i]) {
                colWidths[i] = curItem->key(i, 0).length();
            }
        }
    }
    
    // If dates are allowed, then put them in here.
    if (myAllowDates) {
        txtBody += txtStr.sprintf("%11s: %s\n", "Start Date", startDate().toString().ascii());
        txtBody += txtStr.sprintf("%11s: %s\n\n\n", "End Date", endDate().toString().ascii());
    }
    
    // Okay, now print our headers
    for (uint i = 0; i < numCols; i++) {
        colStr  = repBody->header()->label(i).ascii();
        if (repBody->columnAlignment(i) == Qt::AlignRight) {
            colStr = colStr.rightJustify(colWidths[i]+1);
        } else {
            colStr = colStr.leftJustify(colWidths[i]+1);
        }
        txtBody += colStr;
    }
    txtBody += "\n";

    // And now a line seperating the headers from the data.
    for (uint i = 0; i < numCols; i++) {
        strcpy(tmpStr, "");
        for (uint n = 0; n < colWidths[i]; n++) strcat(tmpStr, "=");
        
        txtBody += tmpStr;
        txtBody += " ";
        
    }
    txtBody += "\n";
    
    // And now, finally, the data itself
    for (curItem = repBody->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        for (uint i = 0; i < numCols; i++) {
            fmtStr = "%";
            if (repBody->columnAlignment(i) != AlignRight) fmtStr += "-";
            sprintf(tmpStr, "%d", colWidths[i]);
            fmtStr += tmpStr;
            fmtStr += "s ";
            txtBody += txtStr.sprintf(fmtStr.ascii(), curItem->key(i, 0).ascii());
        }
        txtBody += "\n";
    }
    
    txtBody += txtStr.sprintf("\n\nReport generated on %s at %s\n\n", tmpDate.toString().ascii(), tmpTime.toString().ascii());

    // Now, create the email.
    QString     fromAddr;
    QString     subj;

    fromAddr = fromAddr.sprintf("%s@%s", curUser().userName, cfgVal("EmailDomain"));
    subj = subj.sprintf("Report - %s", reportTitle->text().ascii());

    EmailMessage    msg;

    msg.setFrom(fromAddr);
    msg.setTo(eOpts->emailAddress());
    msg.setSubject(subj);

    // Only allow them to not send the body if we're attaching a CSV
    if (eOpts->attachCSV()) {
        if (eOpts->doPlainText()) {
            msg.setBody(txtBody);
        }
    } else {
        msg.setBody(txtBody);
    }

    // Are we attaching the CSV part?
    if (eOpts->attachCSV()) {
        // Create a temp file name.
        csvName = makeTmpFileName("/tmp/report-XXXXXX.csv");
        QListViewToCSV(repBody, csvName.ascii());

        msg.addAttachment(csvName.ascii(), "text/csv", "report.csv");
    }

    msg.send();

    // Delete our csv file if necessary
    if (eOpts->attachCSV()) {
        unlink(csvName.ascii());
    }

    QMessageBox::information(this, "Email Report", "The report has been spooled for mailing.");


}

/**
 * setDateRange()
 *
 * Given a report_DateRanges this sets the start and end dates to
 * that range.
 */
void Report::setDateRange(repDates newRange)
{
    QDate   today = QDate::currentDate();
    QDate   yest = today.addDays(-1);
    QDate   weekStart = today.addDays(((today.dayOfWeek() - 1) * -1));
    QDate   weekEnd   = weekStart.addDays(6);
    int     thisMonth = today.month();
    
    switch(newRange) {
        case d_lastMonth:
            if (today.month() == 1) {
                today.setYMD(today.year()-1, 12, 1);
            } else {
                today.setYMD(today.year(), today.month()-1, 1);
            }
            myStartDate.setYMD(today.year(), today.month(), 1);
            myEndDate.setYMD(today.year(), today.month(), today.daysInMonth());
            dateList->setCurrentText("Last Month");
            break;
        case d_today:
            myStartDate.setYMD(today.year(), today.month(), today.day());
            myEndDate.setYMD(today.year(), today.month(), today.day());
            dateList->setCurrentText("Today");
            break;
        case d_yesterday:
            myStartDate.setYMD(yest.year(), yest.month(), yest.day());
            myEndDate.setYMD(yest.year(), yest.month(), yest.day());
            dateList->setCurrentText("Yesterday");
            break;
        case d_thisWeek:
            myStartDate.setYMD(weekStart.year(), weekStart.month(), weekStart.day());
            myEndDate.setYMD(weekEnd.year(), weekEnd.month(), weekEnd.day());
            dateList->setCurrentText("This Week");
            break;
        case d_lastWeek:
            weekStart = today.addDays(((today.dayOfWeek() - 1) * -1) - 7);
            weekEnd   = weekStart.addDays(6);
            myStartDate.setYMD(weekStart.year(), weekStart.month(), weekStart.day());
            myEndDate.setYMD(weekEnd.year(), weekEnd.month(), weekEnd.day());
            dateList->setCurrentText("Last Week");
            break;
        case d_thisYear:
            myStartDate.setYMD(today.year(), 1, 1);
            myEndDate.setYMD(today.year(), 12, 31);
            dateList->setCurrentText("This Year");
            break;
        case d_lastYear:
            myStartDate.setYMD(today.year()-1, 1, 1);
            myEndDate.setYMD(today.year()-1, 12, 31);
            dateList->setCurrentText("Last Year");
            break;
        case d_thisQuarter:
            // This one is trickier.  We have to figure out what month we are
            // in and then set the date accordingly.
            switch (thisMonth) {
                case 1:         // First quarter.
                case 2:
                case 3:
                    myStartDate.setYMD(today.year(), 1, 1);
                    myEndDate.setYMD(today.year(), 3, 31);
                    break;
                    
                case 4:         // Second quarter.
                case 5:
                case 6:
                    myStartDate.setYMD(today.year(), 4, 1);
                    myEndDate.setYMD(today.year(), 6, 30);
                    break;
                    
                case 7:         // Third quarter.
                case 8:
                case 9:
                    myStartDate.setYMD(today.year(), 7, 1);
                    myEndDate.setYMD(today.year(), 9, 30);
                    break;
                    
                default:        // Last Quarter
                    myStartDate.setYMD(today.year(), 10, 1);
                    myEndDate.setYMD(today.year(), 12, 31);
                    break;
                    
            }
            dateList->setCurrentText("This Quarter");
            break;
        case d_lastQuarter:
            // This one is trickier still.  We have to figure out what month 
            // quarter we are in, and then subtract three months from it.
            // in and then set the date accordingly.

            switch (thisMonth) {
                case 1:         // Last quarter of last year.
                case 2:
                case 3:
                    myStartDate.setYMD(today.year()-1, 10, 1);
                    myEndDate.setYMD(today.year()-1, 12, 31);
                    break;
                    
                case 4:         // first quarter of this year.
                case 5:
                case 6:
                    myStartDate.setYMD(today.year(), 1, 1);
                    myEndDate.setYMD(today.year(), 3, 31);
                    break;

                case 7:         // Second quarter.
                case 8:
                case 9:
                    myStartDate.setYMD(today.year(), 4, 1);
                    myEndDate.setYMD(today.year(), 6, 30);
                    break;
                    
                default:        // Third quarter of this year.
                    myStartDate.setYMD(today.year(), 7, 1);
                    myEndDate.setYMD(today.year(), 9, 30);
                    break;
            }
            dateList->setCurrentText("Last Quarter");
            break;
        default:
        case d_thisMonth:
            myStartDate.setYMD(today.year(), today.month(), 1);
            myEndDate.setYMD(today.year(), today.month(), today.daysInMonth());
            dateList->setCurrentText("This Month");
            break;
    }
    
    startDateCal->setDate(myStartDate);
    endDateCal->setDate(myEndDate);
}

/*
** dateRangeSelected  - Changes the dates in the date spinboxes when the
**                      user selects a text description of a date range.
*/

void Report::dateRangeSelected(const char *newRange)
{
    
    if (!strcmp("This Month", newRange))    setDateRange(d_thisMonth);
    if (!strcmp("Last Month", newRange))    setDateRange(d_lastMonth);
    if (!strcmp("Today", newRange))         setDateRange(d_today);
    if (!strcmp("Yesterday", newRange))     setDateRange(d_yesterday); 
    if (!strcmp("This Week", newRange))     setDateRange(d_thisWeek);
    if (!strcmp("Last Week", newRange))     setDateRange(d_lastWeek);
    if (!strcmp("This Year", newRange))     setDateRange(d_thisYear);
    if (!strcmp("Last Year", newRange))     setDateRange(d_lastYear);
    if (!strcmp("This Quarter", newRange))  setDateRange(d_thisQuarter);
    if (!strcmp("Last Quarter", newRange))  setDateRange(d_lastQuarter);
    
    refreshReport();
}

void Report::dateRangeSelected(const QString &newRange)
{
    dateRangeSelected(newRange.ascii());
}

/*
** editStartDate   - Gets a new start date from the user.
*/

void Report::editStartDate()
{
}

/*
** editEndDate     - Gets a new end date from the user.
*/

void Report::editEndDate()
{
}


void Report::userButtonClickedInt()
{
    emit userButtonClicked();
}


/*
** allowReproduction - If set to false, the Graph, Print and Email buttons
**                     will be disabled.
*/

void Report::allowReproduction(int newSetting)
{
    if (newSetting) {
        printButton->show();
        emailButton->show();
    } else {
        printButton->hide();
        emailButton->hide();
    }
}

/*
** setUserButton - Sets the text of the user button and enables it.
*/

void Report::setUserButton(const char *newText)
{
    userButton->show();
    userButton->setText(newText);
}

/**
 * EmailReportDialog()
 *
 * Constructor that gives the user a couple options for sending
 * out the email.
 */
EmailReportDialog::EmailReportDialog(QWidget *parent, const char *name) :
    QDialog(parent, name)
{
    setCaption("Email Report Options");

    QLabel  *emailAddressLabel = new QLabel(this, "emailAddressLabel");
    emailAddressLabel->setText("Email Address:");
    emailAddressLabel->setAlignment(AlignVCenter|AlignRight);

    emailAddr = new QLineEdit(this, "emailAddress");
    emailAddr->setMaxLength(80);
    QString tmpStr;
    tmpStr = curUser().userName;
    tmpStr += "@";
    tmpStr += cfgVal("EmailDomain");
    emailAddr->setText(tmpStr);

    includePlainText = new QCheckBox("Include plain-text report", this, "includePlainText");
    includePlainText->setChecked(true);

    includeCSV = new QCheckBox("Attach report as CSV file", this, "includeCSV");
    includeCSV->setChecked(false);

    QPushButton *sendButton = new QPushButton(this, "sendButton");
    sendButton->setText("&Send");
    sendButton->setDefault(true);
    connect(sendButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);

    Q3GridLayout *gl = new Q3GridLayout(2,3);
    int curRow = 0;
    gl->addWidget(emailAddressLabel,        curRow, 0);
    gl->addWidget(emailAddr,                curRow, 1);
    curRow++;

    gl->addWidget(includePlainText,         curRow, 1);
    curRow++;

    gl->addWidget(includeCSV,               curRow, 1);
    curRow++;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(sendButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl);


}

/**
 * ~EmailReportDialog()
 *
 * Destructor.
 */
EmailReportDialog::~EmailReportDialog()
{
}

/**
 * emailAddress()
 *
 * Returns the email address specified by the user.
 */
QString EmailReportDialog::emailAddress()
{
    return emailAddr->text();
}

/**
 * doPlainText()
 *
 * Returns true if the user wants to include the plain-text body
 * of the report in the email.
 */
bool EmailReportDialog::doPlainText()
{
    return (includePlainText->isChecked());
}

/**
 * attachCSV()
 *
 * Returns true if the user wants to include a CSV 
 * of the report in the email.
 */
bool EmailReportDialog::attachCSV()
{
    return (includeCSV->isChecked());
}


/**
 * ReportFilter()
 *
 * Constructor for ReportFilter base class.
 */
ReportFilter::ReportFilter(QWidget *parent, const char *name) :
    TAAWidget(parent)
{
    // Create our common widgets.
    saveListLabel = new QLabel(this, "saveListLabel");
    saveListLabel->setText("Saved Filters:");
    saveListLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    saveList = new QComboBox(true, this, "saveList");

    loadButton = new QPushButton(this, "loadButton");
    loadButton->setText("Load");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadClicked()));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveFilters()));

    updateButton = new QPushButton(this, "updateButton");
    updateButton->setText("&Update");
    connect(updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));

    closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    // Now, create our layout.  This is a bit different than the normal
    // layout since classes that inherit us will want to put stuff
    // in the middle.
    Q3BoxLayout *myML = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);
    ml = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 3);
    myML->addLayout(ml, 1);

    // Add a horizontal line before the button rows.
    myML->addWidget(new HorizLine(this), 0);

    // A grid layout for our load list.
    Q3GridLayout *ll = new Q3GridLayout(1, 3);
    int curRow = 0;
    ll->addWidget(saveListLabel,        curRow, 0);
    ll->addWidget(saveList,             curRow, 1);
    ll->addWidget(loadButton,           curRow, 2);
    ll->setColStretch(0, 0);
    ll->setColStretch(1, 1);
    ll->setColStretch(2, 0);
    ll->setRowStretch(curRow, 0);

    myML->addLayout(ll, 0);

    // Add our button layout.
    bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(updateButton, 0);
    bl->addWidget(closeButton, 0);

    myML->addLayout(bl, 0);

    setAllowSave(0);

}

/**
 * ~ReportFilter()
 *
 * Destructor for ReportFilter base class.
 */
ReportFilter::~ReportFilter()
{
}

/**
 * setAllowSave()
 *
 * Tells the widget whether or not to show the save button.
 */
void ReportFilter::setAllowSave(int allow)
{
    myAllowSave = allow;
    bool isEnabled = myAllowSave ? true : false;
    bool isHidden = myAllowSave ? false : true;
    saveListLabel->setHidden(isHidden);
    saveListLabel->setEnabled(isEnabled);
    saveList->setHidden(isHidden);
    saveList->setEnabled(isEnabled);
    loadButton->setHidden(isHidden);
    loadButton->setEnabled(isEnabled);
    saveButton->setHidden(isHidden);
    saveButton->setEnabled(isEnabled);
    if (myAllowSave) loadSavedReportList();
}

/**
 * setReportName()
 *
 * Sets the report name.
 */
void ReportFilter::setReportName(const QString repName)
{
    myReportName = repName;
    if (myAllowSave) loadSavedReportList();
}

/**
 * loadSavedReportList()
 *
 * Loads the saved report list from the database.
 * It populates the saveList QComboBox with the names.
 */
void ReportFilter::loadSavedReportList()
{
    if (!myReportName.length()) return;
    ADB     DB;

    saveList->clear();
    saveList->insertItem("");
    DB.query("select SaveName from SavedReports where LoginID = '%s' and ReportName = '%s' order by SaveName", curUser().userName, myReportName.ascii());
    if (DB.rowCount) {
        while(DB.getrow()) {
            saveList->insertItem(DB.curRow["SaveName"]);
        }
    }
}

/**
 * loadClicked()
 *
 * Emits "loadFilters" with the selected saved report name.
 */
void ReportFilter::loadClicked()
{
    if (saveList->currentItem() >= 0) {
        emit(loadFilters(saveList->currentText()));
    }
}


// vim: expandtab
