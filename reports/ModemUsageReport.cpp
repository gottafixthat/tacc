/*
** ModemUsageReport - Gives a summary of the Accounts Receivable for a month.
*/

#include "ModemUsageReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>
#include <qstrlist.h>

#include <ADB.h>
#include <BString.h>


typedef struct AvgRepStruct {
    long    Min;
    long    Max;
    long    Total;
    long    recordCount;
};


ModemUsageReport::ModemUsageReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Modem Usage Report" );
	setTitle("Modem Usage Report");
	
	list->setColumnText(0, "Start Time");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("StopTime");           list->setColumnAlignment(1, AlignLeft);
	list->addColumn("Duration");           list->setColumnAlignment(2, AlignRight);
	list->addColumn("Port");               list->setColumnAlignment(3, AlignLeft);
	list->addColumn("Connect Info");       list->setColumnAlignment(4, AlignLeft);
	list->addColumn("In K");               list->setColumnAlignment(5, AlignRight);
	list->addColumn("Out K");              list->setColumnAlignment(6, AlignRight);
	list->addColumn("Total K");            list->setColumnAlignment(7, AlignRight);

    strcpy(myLoginID, "");
    strxlt.resize(521);
    loadDict();
	refreshReport();

    list->setItemMargin(2);
    list->setAllColumnsShowFocus(TRUE);
    
    resize(700,500);
}


ModemUsageReport::~ModemUsageReport()
{
}


/*
** setLoginID - Without this, the report won't run.
*/

void ModemUsageReport::setLoginID(const char *LoginID)
{
    strncpy(myLoginID, LoginID, 16);
    refreshReport();
}

/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ModemUsageReport::refreshReport()
{
    if (!strlen(myLoginID)) return;

    QApplication::setOverrideCursor(waitCursor);

    list->clear();
    
    char    sDate[64];
    char    eDate[64];
    char    tmpStr[2048];
    char    fStart[1024];
    char    fStop[1024];
    char    fSess[1024];
    char    finK[1024];
    char    foutK[1024];
    char    ftotK[1024];
    int     cDays = 0;
    int     cHours = 0;
    int     cMins  = 0;
    long    cSecs  = 0;
    
    int     totCons = 0;
    long    totSess = 0;
    long    inK     = 0;
    long    outK    = 0;
    long    totK    = 0;
    llong   totIn   = 0;
    llong   totOut  = 0;
    
    ADB     DB;
    
    sprintf(tmpStr, "Modem Usage Report for '%s'", myLoginID);
    reportTitle->setText(tmpStr);
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d%02d%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d%02d%02d", endDate().year(), endDate().month(), endDate().day());
    
    DB.query("select StartDate, StartTime, StopDate, StopTime, SessionLength, Host, Port, Speed, InBytes, OutBytes from ModemUsage where LoginID = '%s' and StartDate >= '%s' and StartDate <= '%s'", 
      myLoginID,
      sDate,
      eDate
    );
    if (DB.rowCount) {
        totCons = DB.rowCount;
        while (DB.getrow()) {
            // Format the start and stop times, duration, etc.
            sprintf(tmpStr, "%s:%03d", strxlt[atol(DB.curRow["Host"])], atoi(DB.curRow["Port"]));
            
            // Now, format the session length
            cSecs = atol(DB.curRow["SessionLength"]);
            totSess += cSecs;
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

            sprintf(fStart, "%s %s", DB.curRow["StartDate"], DB.curRow["StartTime"]);
            sprintf(fStop,  "%s %s", DB.curRow["StopDate"], DB.curRow["StopTime"]);
            totIn  += atol(DB.curRow["InBytes"]);
            totOut += atol(DB.curRow["OutBytes"]);
            inK  = atol(DB.curRow["InBytes"]) / 1024;
            outK = atol(DB.curRow["OutBytes"]) / 1024;
            totK = (atol(DB.curRow["InBytes"]) + atol(DB.curRow["OutBytes"])) / 1024;
            sprintf(finK,   "%ld", inK);
            sprintf(foutK,  "%ld", outK);
            sprintf(ftotK,  "%ld", totK);

            (void) new QListViewItem(list, 
              fStart,
              fStop,
              fSess,
              tmpStr,
              strxlt[atol(DB.curRow["Speed"])],
              finK,
              foutK,
              ftotK
            );
        }
    } else {
        (void) new QListViewItem(list, "No data for specified period");
    }
    
    // Update our title, include total number of connections and total
    // session time, and the bandwidth usage for this user.
    cSecs = totSess;
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
    sprintf(finK,  "%.2f", (float) (totIn  / 1024) / 1024);
    sprintf(foutK, "%.2f", (float) (totOut / 1024) / 1024);
    sprintf(ftotK, "%.2f", (float) ((totIn + totOut) / 1024) / 1024);
    sprintf(fSess, "Modem Usage Report for '%s' - %d connections totalling %3d.%02d:%02d:%02ld\nBandwidth: Input = %sMB, Output = %sMB, Total = %sMB", myLoginID, totCons, cDays, cHours, cMins, cSecs, finK, foutK, ftotK);
    reportTitle->setText(fSess);
    
    QApplication::restoreOverrideCursor();
}

/*
** loadDict - Loads the ModemUsageStrings dictionary into memory.
*/

void ModemUsageReport::loadDict()
{
    ADB         DB;
    
    DB.query("select TextID, TextStr from ModemUsageStrings order by TextID");
    if (DB.rowCount) while (DB.getrow()) {
        char *tmpStr = new char[strlen(DB.curRow["TextStr"])+2];
        strcpy(tmpStr, DB.curRow["TextStr"]);
        strxlt.insert(atol(DB.curRow["TextID"]), tmpStr);
    }
}

