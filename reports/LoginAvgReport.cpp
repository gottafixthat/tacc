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
#include <Report.h>

#include "LoginAvgReport.h"

using namespace Qt;

struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};


LoginAvgReport::LoginAvgReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Login Count Averages Report" );
	setTitle("Login Count Averages Report");
	
	repBody->setColumnText(0, "Login Type");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Low");                repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("High");               repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Average");            repBody->setColumnAlignment(3, AlignRight);

	refreshReport();
}


LoginAvgReport::~LoginAvgReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void LoginAvgReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    Q3Dict<AvgRepStruct>     repData;
    AvgRepStruct            *curItem;
    char                    sDate[20];
    char                    eDate[20];
    long                    curCount = 0;
    ADB                     DB;
    Q3StrList                keys(TRUE);
    Q3StrList                contents(TRUE);
    int                     entryNo = 0;
    float                   *datum;
    char                    **labels;
    
    reportTitle->setText("Login Count Averages");
    
    // Okay, generate the report.
    repBody->clear();
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d-%02d-%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d-%02d-%02d", endDate().year(), endDate().month(), endDate().day());

    DB.query("select EntryValue from AuditTrail where EntryDate >= '%s' and EntryDate <= '%s' and EntryName = 'UserCount' order by EntryValue", sDate, eDate);

    if (DB.rowCount) {
        while (DB.getrow()) {
            contents.clear();
            splitString(DB.curRow["EntryValue"], '\t', contents);
            curItem = repData[contents.at(0)];
            if (!curItem) {
                curItem = new AvgRepStruct;
                curItem->Min = 999999999;
                curItem->Max = 0;
                curItem->Total = 0;
                curItem->recordCount = 0;
                repData.insert(contents.at(0), curItem);
                keys.append(contents.at(0));
            }
            
            // Do the totals.
            curCount = atol(contents.at(1));
            curItem->recordCount++;
            curItem->Total += curCount;
            if (curItem->Min > curCount) curItem->Min = curCount;
            if (curItem->Max < curCount) curItem->Max = curCount;
        }
        
        // Now, walk through the keys and add the items into the QListView.
        char    itemName[1024];
        char    itemMin[1024];
        char    itemMax[1024];
        char    itemAvg[1024];
        char    itemEntry[1024];
        
        long    TotalMin = 0;
        long    TotalMax = 0;
        float   TotalAvg = 0.0;
        
        datum   = new float[keys.count() + 1];
        labels  = new char*[keys.count() + 1];

        for (unsigned int i = 0; i < keys.count(); i++) {
            curItem = repData[keys.at(i)];
            sprintf(itemName, "%s", (const char *) keys.at(i));
            sprintf(itemMin, "%ld", curItem->Min);
            TotalMin += curItem->Min;
            sprintf(itemMax, "%ld", curItem->Max);
            TotalMax += curItem->Max;
            if (curItem->recordCount) {
                sprintf(itemAvg, "%.1f", (float) ((float)curItem->Total / (float)curItem->recordCount));
                TotalAvg += (float) ((float)curItem->Total / (float)curItem->recordCount);
            } else {
                strcpy(itemAvg, "0");
            }
            sprintf(itemEntry, "%08d", entryNo++);
            (void) new Q3ListViewItem(repBody, itemName, itemMin, itemMax, itemAvg, itemEntry);
            labels[i] = new char[256];
            strcpy(labels[i], itemName);
            // datum[i] = atof(itemAvg);
        }

        // Do the grand totals now.
        sprintf(itemName, "%s", "TOTAL");
        sprintf(itemMin, "%ld", TotalMin);
        sprintf(itemMax, "%ld", TotalMax);
        sprintf(itemAvg, "%.1f", TotalAvg);
        sprintf(itemEntry, "%08d", entryNo++);
        (void) new Q3ListViewItem(repBody, itemName, itemMin, itemMax, itemAvg, itemEntry);
        
        // Reset our sort key.
        repBody->setSorting(4);
        
	    // Do the graph data.
	    for (unsigned int i = 0; i < keys.count(); i++) {
	        float   tmpAvg;
	        curItem = repData[keys.at(i)];
	        strcpy(labels[i], (const char *) keys.at(i));
	        tmpAvg = (float) ((float)curItem->Total / (float)curItem->recordCount);
	        datum[i] = tmpAvg / TotalAvg;
	    }
    } else {
        (void) new Q3ListViewItem(repBody, "No data available for specified period");
    }
    

    
    QApplication::restoreOverrideCursor();
}

// vim: expandtab
