/*
** UsersOnlineReport - Gives a summary of cities that our active customers are
**              located in.
*/

#include "UsersOnlineReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <ADB.h>
#include <BString.h>

#include "ModemUsageReport.h"
#include "UsersOnlineFilters.h"


UsersOnlineReport::UsersOnlineReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Users Online Report" );
	setTitle("Customer Cities");
	
	repBody->setColumnText(0, "Login ID"); repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Server");          repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Port");            repBody->setColumnAlignment(2, AlignRight);
    repBody->addColumn("Start Time");      repBody->setColumnAlignment(3, AlignLeft);
    repBody->addColumn("Stop Time");       repBody->setColumnAlignment(4, AlignLeft);
    repBody->addColumn("Duration");        repBody->setColumnAlignment(5, AlignRight);
	
    allowDates(REP_NODATES);
    allowFilters(1);
    
    displayTime = QDateTime::currentDateTime().addDays(-1);
    
    strxlt.resize(521);
    loadDict();
    
	refreshReport();
}


UsersOnlineReport::~UsersOnlineReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void UsersOnlineReport::refreshReport()
{

    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    sDate[64];
    char    sTime[64];
    QDate   tmpDate;
    QTime   tmpTime;
    char    *query = new char[65536];
    char    tmpStr[2048];
    char    fStart[1024];
    char    fStop[1024];
    char    fSess[1024];
    int     cDays = 0;
    int     cHours = 0;
    int     cMins  = 0;
    long    cSecs  = 0;
    
    long    totUsers = 0;
    
    int     totCons = 0;
    long    totSess = 0;
    
    ADB     DB;
    
    // Get the starting and ending dates.
    tmpDate = displayTime.date();
    tmpTime = displayTime.time();
    sprintf(sDate, "%04d%02d%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
    sprintf(sTime, "%02d:%02d", tmpTime.hour(), tmpTime.minute());
    
    DB.query("select LoginID, Host, Port, StartDate, StartTime, StopDate, StopTime, SessionLength from ModemUsage where StartDate <= '%s' and StartTime <= '%s:00' and StopDate >= '%s' and StopTime >= '%s:00'", 
      sDate,
      sTime,
      sDate,
      sTime
    );
    
    totUsers = DB.rowCount;
    sprintf(tmpStr, "Users Online Report\n%ld users on at %s %02d:%02d", totUsers, (const char *) tmpDate.toString(), tmpTime.hour(), tmpTime.minute());
    reportTitle->setText(tmpStr);

    if (DB.rowCount) {
        totCons = DB.rowCount;
        while (DB.getrow()) {
            // Format the start and stop times, duration, etc.
            sprintf(tmpStr, "%03d", atoi(DB.curRow["Port"]));
            
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
            sprintf(fStop,  "%s %s", DB.curRow["StopTime"], DB.curRow["StopDate"]);

            (void) new QListViewItem(repBody, 
              DB.curRow["LoginID"],
              strxlt[atol(DB.curRow["Host"])],
              tmpStr,
              fStart,
              fStop,
              fSess
            );
        }
    } else {
        (void) new QListViewItem(repBody, "No data for specified period");
    }

    delete query;
    
    QApplication::restoreOverrideCursor();

}


/*
** applyFilters - Gets the new filters and refreshes the report.
*/

void UsersOnlineReport::applyFilters(QDateTime newDateTime)
{
    displayTime = newDateTime;
    refreshReport();
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void UsersOnlineReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        char    tmpLogin[512];
        strcpy(tmpLogin, curItem->key(0,0));
        
        ModemUsageReport *MUR = new ModemUsageReport();
        MUR->setLoginID(tmpLogin);
        MUR->setStartDate(displayTime.date());
        MUR->setEndDate(displayTime.date());
        MUR->show();
    }
}

/*
** editFilters  - Allows the user to edit the filters that are applied
**                to the report.
*/

void UsersOnlineReport::editFilters()
{
    UsersOnlineFilters *UOF  = new UsersOnlineFilters();
    UOF->setDateTime(displayTime);
    // UOF->setActiveStatus(myShowActive);
    connect(UOF, SIGNAL(filtersChanged(QDateTime)), SLOT(applyFilters(QDateTime)) );
    UOF->show();
}


/*
** loadDict - Loads the ModemUsageStrings dictionary into memory.
*/

void UsersOnlineReport::loadDict()
{
    ADB     DB;
    char    *query = new char[32768];
    
    DB.query("select TextID, TextStr from ModemUsageStrings order by TextID");
    if (DB.rowCount) while (DB.getrow()) {
        char *tmpStr = new char[strlen(DB.curRow["TextStr"])+2];
        strcpy(tmpStr, DB.curRow["TextStr"]);
        strxlt.insert(atol(DB.curRow["TextID"]), tmpStr);
    }
    
    delete query;
}


