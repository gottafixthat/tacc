/*
** ModemSummaryReport - Gives a summary of the Accounts Receivable for a month.
*/

#include "ModemSummaryReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>
#include <qstrlist.h>
#include <qprogressdialog.h>
#include <qregexp.h>
#include <qstring.h>
#include <qdialog.h>
#ifdef USEQWT
#include <qwt_plot.h>
#endif

#include <ADB.h>
#include <BString.h>


struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};


ModemSummaryReport::ModemSummaryReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Modem Usage Summary" );
	setTitle("Modem Usage Summary");
	
	repBody->setColumnText(0, "Time");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Ports In Use");        repBody->setColumnAlignment(1, AlignRight);

    timeDict = NULL;
    
    // Set our default resolution, in minutes.
    timeRes = 5;
    
    allowDates(REP_STARTDATE|REP_ENDDATE);      // Only show the start and
                                                // End dates.
    allowGraph(1);                              // We need to graph this one.
    allowFilters(1);                            // We need to allow filters.

    setEndDate(startDate());
    
    // Filters must be loaded before we refresh...
    myFilters = new ModemSummaryFilters();
    connect(myFilters, SIGNAL(filtersChanged()), SLOT(refreshReport()));

	refreshReport();

    repBody->setItemMargin(2);
    repBody->setAllColumnsShowFocus(TRUE);
    
    
//    resize(700,500);
}


ModemSummaryReport::~ModemSummaryReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ModemSummaryReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    ADB         DB;
    char        sDate[64];
    char        eDate[64];
    int         curStep = 0;
    QDateTime   sessStart;
    QDateTime   sessStop;
    QDate       tmpStartDate;
    QDate       tmpEndDate;
    QString     tmpStr;
    char        tmpTimeStr[128];
    char        tmpPorts[64];
    
    repBody->clear();
    
    // Get the starting and ending dates for our report and setup our counters.
    setupCounters();
    tmpStartDate    = dictStartDate.date();
    tmpEndDate      = dictEndDate.date();
    sprintf(sDate, "%04d-%02d-%02d", tmpStartDate.year(), tmpStartDate.month(), tmpStartDate.day());
    sprintf(eDate, "%04d-%02d-%02d", tmpEndDate.year(), tmpEndDate.month(), tmpEndDate.day());
    
    // fprintf(stderr, "Start Date = '%s', End Date = '%s'\n", sDate, eDate);
    
    DB.query("select StartDate, StartTime, StopDate, StopTime, Host from ModemUsage where StopDate >= '%s' and StartDate <= '%s'",
      sDate,
      eDate
    );
	
    // Set the report title.
    tmpStr = "Modem Usage Summary\n";
    tmpStr += dictStartDate.toString();
    tmpStr += " through ";
    tmpStr += dictEndDate.toString();
    setTitle(tmpStr);
    
    QProgressDialog prog("Generating report...", "Abort", (int) (DB.rowCount+timeSteps), 0, "Generating report...");

    if (DB.rowCount) {
        // fprintf(stderr, "Selected %ld rows...\n", DB.rowCount);
        // fprintf(stderr, "Adding clicks...\n");
        while (DB.getrow()) {
            prog.setProgress(++curStep);
            if (prog.wasCancelled()) break;
            // Convert the dates and times into timestamp format times so
            // we can easily convert them into QDateTime objects.
            tmpStr  = DB.curRow["StartDate"];
            tmpStr += DB.curRow["StartTime"];
            tmpStr.replace(QRegExp("[:-]"), "");
            myDateStamptoQDateTime((const char *)tmpStr, &sessStart);
            
            tmpStr  = DB.curRow["StopDate"];
            tmpStr += DB.curRow["StopTime"];
            tmpStr.replace(QRegExp("[:-]"), "");
            myDateStamptoQDateTime((const char *)tmpStr, &sessStop);
            
            if (myFilters->showHost(atoi(DB.curRow["Host"]))) {
                addClicks(sessStart, sessStop);
            }
        }

        // fprintf(stderr, "Filling list...\n");
        // Now, fill in our QListView.
        for (int i = 0; i < timeSteps; i++) {
            prog.setProgress(++curStep);
            if (prog.wasCancelled()) break;
            sessStop  = dictStartDate;
            sessStart = sessStop.addSecs(i * timeRes * 60);
            // fprintf(stderr, "%04d: Time = '%s'\n", i, (const char *) sessStart.toString());
            sprintf(tmpTimeStr, "%04d-%02d-%02d  %02d:%02d",
              sessStart.date().year(),
              sessStart.date().month(),
              sessStart.date().day(),
              sessStart.time().hour(),
              sessStart.time().minute()
            );
            sprintf(tmpPorts, "%4ld", timeDict[i]);

            (void) new QListViewItem(repBody, tmpTimeStr, tmpPorts);
        }
        
    } else {
        (void) new QListViewItem(repBody, "No data for specified period");
    }
    
    QApplication::restoreOverrideCursor();
}

/*
** plotGraph      - Virtual function that gets called when the user wants
**                  to plot a graph.
*/

#ifdef USEQWT
void ModemSummaryReport::plotGraph(QwtPlot *plot)
{
    double      *x = new double[timeSteps];
    double      *y = new double[timeSteps];
    long        cSin, mX, mY;
    char        ampm[16];
    int         tmpHour, tmpMin;
    long        maxModems = 0, minModems = 1000000;
    QString     tmpStr;
    QDateTime   sessStart;
    QDateTime   sessStop;
    char        tmpTimeStr[128];
    
    // Setup the background color and the title.
    plot->setPlotBackground(QColor(white));
    tmpStr = "Modem Usage Summary - ";
    tmpStr += dictStartDate.toString();
    tmpStr += " through ";
    tmpStr += dictEndDate.toString();
    plot->setTitle(tmpStr);
    
    for (int i = 0; i < timeSteps; i++) {
        y[i] = timeDict[i] * (double) 1.0;
        x[i] = i * (double) 1.0;
    }
    
    // Insert the curves.
    cSin = plot->insertCurve("Modem usage over time");
    
    // Set the curve styles
    plot->setCurvePen(cSin, QPen(blue));
    
    // Copy the data into the graph
    plot->setCurveData(cSin, x, y, timeSteps);
    
    // Set up our axis markers...
    plot->setAxisTitle(QwtPlot::yLeft, "Modems In Use");
    plot->setAxisTitle(QwtPlot::xBottom, "Time");
    plot->enableGridX(FALSE);
    plot->enableGridY(FALSE);
    plot->enableAxis(QwtPlot::xBottom, FALSE);
    
    // And put in some time markers...
    for (int i = 0; i < timeSteps; i++) {
        // Get the minimum and maximum numbers for our plot
        if (timeDict[i] > maxModems) maxModems = timeDict[i];
        if (timeDict[i] < minModems) minModems = timeDict[i];
            
        sessStop  = dictStartDate;
        sessStart = sessStop.addSecs(i * timeRes * 60);
        // fprintf(stderr, "%04d: Time = '%s'\n", i, (const char *) sessStart.toString());
        tmpHour = sessStart.time().hour();
        tmpMin  = sessStart.time().minute();
        strcpy(ampm, "a");
        if (tmpHour > 11) {
            strcpy(ampm, "p");
            tmpHour -= 12;
        }
        if (tmpHour == 0) tmpHour = 12;
        sprintf(tmpTimeStr, "%02d:%02d%s",
          tmpHour,
          tmpMin,
          ampm
        );
        if (sessStart.time().minute() == 0 && (!(sessStart.time().hour() % 4))) {
            mX = plot->insertLineMarker(tmpTimeStr, QwtPlot::xBottom);
            plot->setMarkerLinePen(mX, QPen(DashLine));
            plot->setMarkerXPos(mX, i * (double) 1.0);
            plot->setMarkerYPos(mX, (double) 0.0);
        }
    }

    // Add a marker for the max number of modems in use.
    tmpStr.sprintf("Max in use = %ld", maxModems);
    mY = plot->insertLineMarker(tmpStr, QwtPlot::yLeft);
    plot->setMarkerLinePen(mY, QPen(DashLine));
    plot->setMarkerLinePen(mY, QPen(green));
    plot->setMarkerXPos(mY, (double) 0.0);
    plot->setMarkerYPos(mY, (double) maxModems * 1.0);
}
#endif 


/*
** setupCounters  - Sets up all of the counters and ranges that we need
**                  based on the start and stop dates.
*/

void ModemSummaryReport::setupCounters()
{
    // Set the dictionary start and stop dates.
    QTime   tmpTime(0,0,0,0);
    QDate   tmpDate;
    
    QDateTime   tmpDT1;
    QDateTime   tmpDT2;
    
    // When we have a date, we want to take 2 hours before and 2 hours after
    // the date.  So....
    tmpDate = startDate();
    
    tmpDT1.setDate(tmpDate);
    tmpDT1.setTime(tmpTime);
    tmpDT2 = tmpDT1.addSecs((60 * 120) * -1);       // Subtract 2 hours.
    dictStartDate = tmpDT2;
    
    tmpDate = endDate();
    
    if (startDate().daysTo(endDate()) > 6) {        // Max 1 week
        tmpDate = startDate().addDays(6);
        setEndDate(tmpDate);
    }
    
    tmpTime.setHMS(23,60 - timeRes, 0);
    tmpDT1.setDate(tmpDate);
    tmpDT1.setTime(tmpTime);
    tmpDT2 = tmpDT1.addSecs(60 * 60 * 2);           // Add 2 hours
    dictEndDate   = tmpDT2;
    
    // Set the total number of steps.
    timeSteps = dictStartDate.secsTo(dictEndDate) / (timeRes * 60);
    // fprintf(stderr, "dictStartDate = '%s', dictEndDate = '%s'\n", (const char *) dictStartDate.toString(), (const char *)dictEndDate.toString());
    // fprintf(stderr, "There are %d total time steps.\n", timeSteps);
    
    // Fill the dictionary now so we don't need to worry about NULL checks.
    if (timeDict == NULL) delete timeDict;
    timeDict = new long[timeSteps+1];
    for (int i = 0; i < timeSteps; i++) {
        timeDict[i] = 0;
    }
}

/*
** addClicks  - Adds one to each of the dictionary items relating to the time
**              steps.
**              The dictionary is laid out as follows:
**
**                 int n = [0..timeSteps];
**                 timeDict[n]  = StartTime + (timeRes * n * 60)
*/

void ModemSummaryReport::addClicks(QDateTime sessStart, QDateTime sessStop)
{
    QDateTime   mySessStart = sessStart;
    QDateTime   mySessStop  = sessStop;
    int         srStart = 0;
    int         srStop  = timeSteps;
    
    if (mySessStart < dictStartDate) mySessStart = dictStartDate;
    if (mySessStop  > dictEndDate)   mySessStop  = dictEndDate;
    
    // Calculate the ranges that we have to look at.
    srStart = dictStartDate.secsTo(mySessStart) / (timeRes * 60);
    srStop  = dictStartDate.secsTo(mySessStop)  / (timeRes * 60);
    
    // fprintf(stderr, "srStart = %d, srStop = %d\n", srStart, srStop);

    // Loop through all of our time steps and check for starting and ending
    // ranges
    for (int i = srStart; i < srStop; i++) {
        timeDict[i]++;
    }
}



/*
** editFilters - Brings up the ModemSummaryFilters so we can edit them.
*/

void ModemSummaryReport::editFilters()
{
    myFilters->show();
}

/*
** applyFilters  - Refreshes the report.
*/

void ModemSummaryReport::applyFilters()
{
    refreshReport();
}

