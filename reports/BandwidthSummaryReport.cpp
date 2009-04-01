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

#include <QtGui/QApplication>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3dict.h>
#include <Qt3Support/q3strlist.h>

#include <ADB.h>
#include <BString.h>

#include "BandwidthUsageReport.h"
#include "BandwidthSummaryReport.h"

using namespace Qt;

struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};

#define BW_MBYTE 1048576.0

BandwidthSummaryReport::BandwidthSummaryReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Bandwidth Usage Summary Report" );
	setTitle("Bandwidth Usage Summary Report");
	
	repBody->setColumnText(0, "Login ID");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Input MB");         repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("Output MB");        repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Total MB");         repBody->setColumnAlignment(3, AlignRight);
	repBody->addColumn("Total GB");         repBody->setColumnAlignment(4, AlignRight);

	refreshReport();

    repBody->setItemMargin(2);
    repBody->setAllColumnsShowFocus(TRUE);
    
    resize(600,500);
}


BandwidthSummaryReport::~BandwidthSummaryReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void BandwidthSummaryReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    sDate[64];
    char    eDate[64];
    char    *query = new char[65536];
    char    inStr[1024];
    char    outStr[1024];
    char    totStr[1024];
    char    gbtotStr[1024];
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
    
    llong  totalIn    = (llong) 0;
    llong  totalOut   = (llong) 0;
    llong  grandTotal = (llong) 0;
    
    QDate   sampDate;
    
    ADB     DB;
    ADB     DB2;

    llong  *inDays  = NULL;
    llong  *outDays = NULL;
    llong  *totDays = NULL;
            
    // Get the starting and ending dates.
    sprintf(sDate, "%04d-%02d-%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d-%02d-%02d", endDate().year(), endDate().month(), endDate().day());
    
    // Calculate the number of days in our report.
    daysInReport = startDate().daysTo(endDate()) + 1;
    
    inDays  = new llong[daysInReport];
    outDays = new llong[daysInReport];
    totDays = new llong[daysInReport];

    DB2.query("select distinct LoginID from TrafficUsage where SampleDate >= '%s' and SampleDate <= '%s'",
      sDate,
      eDate
    );
    if (DB2.rowCount) while (DB2.getrow()) {
	    DB.query("select SampleDate, SampleTime, OctetsIn, OctetsOut from TrafficUsage where LoginID = '%s' and SampleDate >= '%s' and SampleDate <= '%s' order by SampleDate, SampleTime", 
	      DB2.curRow["LoginID"],
	      sDate,
	      eDate
	    );
	    
	    if (DB.rowCount) {
	    
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
	        
	        // Clear our counters.
	        totalIn     = (llong) 0;
	        totalOut    = (llong) 0;
	        grandTotal  = (llong) 0;
	        
	        // Okay, now, go through each of the days in the month and insert
	        // a report item for it, giving a grand total at the end.
	        for (int i = 0; i < daysInReport; i++) {
	            sampDate = startDate().addDays(i);
	            
	            sprintf(dateStr, "%04d-%02d-%02d", sampDate.year(), sampDate.month(), sampDate.day());
	            sprintf(inStr,  "%.2f", inDays[i]  / BW_MBYTE);
	            sprintf(outStr, "%.2f", outDays[i] / BW_MBYTE);
	            sprintf(totStr, "%.2f", totDays[i] / BW_MBYTE);

	            totalIn    += inDays[i];
	            totalOut   += outDays[i];
	            grandTotal += totDays[i];
	          
	            /*  
	            (void) new QListViewItem(list,
	              dateStr,
	              inStr,
	              outStr,
	              totStr
	            );
	            */
	        }
	        
            sprintf(inStr,    "%.2f", totalIn    / BW_MBYTE);
            sprintf(outStr,   "%.2f", totalOut   / BW_MBYTE);
            sprintf(totStr,   "%.2f", grandTotal / BW_MBYTE);
            sprintf(gbtotStr, "%.2f", grandTotal / (BW_MBYTE * 1024));
	        
	        (void) new Q3ListViewItem(repBody,
	          DB2.curRow["LoginID"],
	          inStr,
	          outStr,
	          totStr,
	          gbtotStr
	        );
        }
    } else {
        (void) new Q3ListViewItem(repBody, "No data for specified period");
    }
    
    delete query;
    
    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Shows the selected customers bandwidth detail.
*/

void BandwidthSummaryReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        char    tmpLoginID[1024];
        strcpy(tmpLoginID, curItem->key(0,0));
        
        BandwidthUsageReport    *BUR = new BandwidthUsageReport();
        BUR->setStartDate(startDate());
        BUR->setEndDate(endDate());
        BUR->setLoginID(tmpLoginID);
        BUR->show();
    }
}



// vim: expandtab
