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

#include <ModemUsageReport.h>
#include "ModemTotalsReport.h"

using namespace Qt;

struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};


ModemTotalsReport::ModemTotalsReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Modem Usage Report" );
	setTitle("Modem Usage Report");
	
	repBody->setColumnText(0, "Login ID");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Connections");      repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("Total Time");       repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Average Call");     repBody->setColumnAlignment(3, AlignRight);

    loginDict.resize(4253);
	refreshReport();

    repBody->setItemMargin(2);
    repBody->setAllColumnsShowFocus(TRUE);
    
    resize(495,500);
}


ModemTotalsReport::~ModemTotalsReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ModemTotalsReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    sDate[64];
    char    eDate[64];
    char    tmpStr[2048];
    char    fCount[1024];
    char    fSess[1024];
    char    fAvg[1024];
    int     cDays = 0;
    int     cHours = 0;
    int     cMins  = 0;
    long    cSecs  = 0;
    
    ModemTotalStruct    *curItem;
    
    loginDict.clear();
    loginList.clear();

    ADB     DB;
    
    sprintf(tmpStr, "Modem Usage Report\n%s through %s", (const char *) startDate().toString(), (const char *) endDate().toString());
    reportTitle->setText(tmpStr);
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d%02d%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d%02d%02d", endDate().year(), endDate().month(), endDate().day());
    
    // First, get the list of login ID's that used the modems during the
    // specified time period.
    DB.query("select distinct LoginID from ModemUsage where StartDate >= '%s' and StartDate <= '%s'", 
      sDate,
      eDate
    );
    if (DB.rowCount) {
        // Now, load up the records into a dictionary and into a list.
        while (DB.getrow()) {
            loginList.append(DB.curRow["LoginID"]);
            curItem = new ModemTotalStruct;
            curItem->loginCount = 0;
            curItem->totalTime  = 0;
            loginDict.insert(DB.curRow["LoginID"], curItem);
        }
        
        // Okay, we've got a definitive list of login ID's.  Go through the 
        // entire date range and add them all up.
	    DB.query("select LoginID, SessionLength from ModemUsage where StartDate >= '%s' and StartDate <= '%s'", 
	      sDate,
	      eDate
	    );
        if (DB.rowCount) while (DB.getrow()) {
            curItem = loginDict[DB.curRow["LoginID"]];
            if (curItem) {
                curItem->loginCount++;
                curItem->totalTime += atol(DB.curRow["SessionLength"]);
            }
        }


        // Okay, we're done.  Now, walk through the login ID list and put
        // all of the stuff into our list...
        for (unsigned int i = 0 ; i < loginList.count(); i++) {

            curItem = loginDict[loginList.at(i)];
            
            if (curItem) {
	            
	            // Now, format the session length
	            cSecs = curItem->totalTime;
		        if (cSecs > 86400) {
		        	cDays    = cSecs / 86400;
		        	cSecs -= (cDays * 86400);
		        } else {
		            cDays    = 0;
		        } 
		        if (cSecs > 3600) {
		            cHours   = cSecs / 3600;
		            cSecs -= (cHours * 3600);
		        } else {
		            cHours = 0;
		        }
		        if (cSecs > 60) {
		            cMins = cSecs / 60;
		            cSecs -= (cMins * 60);
		        } else {
		        	cMins = 0;
		        }
	            sprintf(fSess, "%3d.%02d:%02d:%02ld", cDays, cHours, cMins, cSecs);

	            sprintf(fCount, "%5d", curItem->loginCount);

                // Get the average connection length
                cSecs = curItem->totalTime / curItem->loginCount;
		        if (cSecs > 86400) {
		        	cDays    = cSecs / 86400;
		        	cSecs -= (cDays * 86400);
		        } else {
		            cDays    = 0;
		        } 
		        if (cSecs > 3600) {
		            cHours   = cSecs / 3600;
		            cSecs -= (cHours * 3600);
		        } else {
		            cHours = 0;
		        }
		        if (cSecs > 60) {
		            cMins = cSecs / 60;
		            cSecs -= (cMins * 60);
		        } else {
		        	cMins = 0;
		        }
	            sprintf(fAvg, "%3d.%02d:%02d:%02ld", cDays, cHours, cMins, cSecs);
            
	            (void) new Q3ListViewItem(repBody, 
	              (const char *) loginList.at(i),
	              fCount,
	              fSess,
	              fAvg
	            );
            }
        }
    } else {
        (void) new Q3ListViewItem(repBody, "No data for specified period");
    }
    
    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Allows the user to zoom in on a customer in the 
**                    report.
*/

void ModemTotalsReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        char    tmpLoginID[1024];
        strcpy(tmpLoginID, curItem->key(0,0));
        
        ModemUsageReport    *MUR = new ModemUsageReport();
        MUR->setStartDate(startDate());
        MUR->setEndDate(endDate());
        MUR->setLoginID(tmpLoginID);
        MUR->show();
    }
}

// vim: expandtab
