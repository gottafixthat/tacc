/*
** BandwidthUsageReport - Gives a summary of the Accounts Receivable for a month.
*/

#include "BandwidthUsageReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>
#include <qstrlist.h>

#include <ADB.h>
#include <BString.h>


struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};

#define BW_MBYTE 1048576.0

BandwidthUsageReport::BandwidthUsageReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Bandwidth Usage Report" );
	setTitle("Bandwidth Usage Report");
	
	repBody->setColumnText(0, "Date");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Input MB");     repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("Output MB");    repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Total MB");     repBody->setColumnAlignment(3, AlignRight);

    strcpy(myLoginID, "");
	refreshReport();

    inDays  = NULL;
    outDays = NULL;
    totDays = NULL;

    repBody->setItemMargin(2);
    repBody->setAllColumnsShowFocus(TRUE);
    
    resize(600,500);
}


BandwidthUsageReport::~BandwidthUsageReport()
{
}


/*
** setLoginID - Without this, the report won't run.
*/

void BandwidthUsageReport::setLoginID(const char *LoginID)
{
    strncpy(myLoginID, LoginID, 16);
    refreshReport();
}

/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void BandwidthUsageReport::refreshReport()
{
    if (!strlen(myLoginID)) return;

    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    sDate[64];
    char    eDate[64];
    char    *query = new char[65536];
    char    tmpStr[2048];
    char    inStr[1024];
    char    outStr[1024];
    char    totStr[1024];
    char    dateStr[1024];
    
    int     totCons = 0;
    
    int     curDay = 0;
    
    int     daysInReport = 0;

    llong  lastIn  = (llong) 0;
    llong  lastOut = (llong) 0;
    llong  bytesIn = (llong) 0;
    llong  currIn  = (llong) 0;
    llong  currOut = (llong) 0;
    llong  bytesOut= (llong) 0;
    
    llong  grandTotal = (llong) 0;
    
    QDate   sampDate;
    
    ADB     DB;

    if (inDays  != NULL) { delete inDays;  inDays  = NULL; }
    if (outDays != NULL) { delete outDays; outDays = NULL; }
    if (totDays != NULL) { delete totDays; totDays = NULL; }

    sprintf(tmpStr, "Bandwidth Usage Report for '%s'", myLoginID);
    reportTitle->setText(tmpStr);
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d-%02d-%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d-%02d-%02d", endDate().year(), endDate().month(), endDate().day());

    DB.query("select SampleDate, SampleTime, OctetsIn, OctetsOut from TrafficUsage where LoginID = '%s' and SampleDate >= '%s' and SampleDate <= '%s' order by SampleDate, SampleTime", 
      myLoginID,
      sDate,
      eDate
    );
    
    if (DB.rowCount) {
    
        // Calculate the number of days in our report.
        daysInReport = startDate().daysTo(endDate()) + 1;
        
        inDays  = new llong[daysInReport];
        outDays = new llong[daysInReport];
        totDays = new llong[daysInReport];

        for (int i = 0; i < daysInReport; i++) {
            inDays[i]  = (llong) 0;
            outDays[i] = (llong) 0;
            totDays[i] = (llong) 0;
        }
    
        totCons = DB.rowCount;
        while (DB.getrow()) {
            // We walk through the samples and add to to our totals, provided
            // that the last sample is less than the current sample.  If the
            // last sample is less than the current sample, this indicates
            // that our router counter wrapped, or the router rebooted.
            // So, we give the customer some bandwidth for free...
            
            // Get the sample date.
            myDatetoQDate(DB.curRow["SampleDate"], &sampDate);
            curDay  = startDate().daysTo(sampDate);
            currIn  = atoll(DB.curRow["OctetsIn"]);
            currOut = atoll(DB.curRow["OctetsOut"]);

            if (currIn > lastIn && (lastIn)) {
                bytesIn = currIn - lastIn;
            } else {
                bytesIn = (llong) 0;
            }
            
            if (currOut > lastOut && (lastOut)) {
                bytesOut = currOut - lastOut;
            } else {
                bytesOut = (llong) 0;
            }

            inDays[curDay]  += bytesIn;
            totDays[curDay] += bytesIn;
            
            outDays[curDay] += bytesOut;
            totDays[curDay] += bytesOut;
            
            lastIn  = currIn;
            lastOut = currOut;
        }
        
        // Okay, now, go through each of the days in the month and insert
        // a report item for it, giving a grand total at the end.
        for (int i = 0; i < daysInReport; i++) {
            sampDate = startDate().addDays(i);
            
            sprintf(dateStr, "%04d-%02d-%02d", sampDate.year(), sampDate.month(), sampDate.day());
            sprintf(inStr,  "%.2f", inDays[i]  / BW_MBYTE);
            sprintf(outStr, "%.2f", outDays[i] / BW_MBYTE);
            sprintf(totStr, "%.2f", totDays[i] / BW_MBYTE);

            grandTotal += totDays[i];
            
            (void) new QListViewItem(repBody,
              dateStr,
              inStr,
              outStr,
              totStr
            );
        }
        
        // Now, put the grand totals in the display.
        sprintf(tmpStr, "Bandwidth Usage Report for '%s'\nTotal Transfer = %.2fMB (%.2fGB)",
          myLoginID,
          grandTotal / BW_MBYTE,
          grandTotal / (BW_MBYTE * 1024)
        );
        reportTitle->setText(tmpStr);
        
    } else {
        (void) new QListViewItem(repBody, "No data for specified period");
    }
    
    delete query;
    
    QApplication::restoreOverrideCursor();
}

