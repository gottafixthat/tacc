/**********************************************************************

	--- Qt Architect generated file ---

	File: UsersOnlineFilters.cpp
	Last generated: Thu Sep 30 14:14:20 1999

 *********************************************************************/

#include "UsersOnlineFilters.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapp.h>
#include <ADB.h>

#include <qdatetimeedit.h>

#define Inherited UsersOnlineFiltersData

UsersOnlineFilters::UsersOnlineFilters
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Users Online Filters" );
    
    dateCal = new DateInput(dateParent, myDateTime.date());
    timeSpin = new QTimeEdit(QTime::currentTime(), timeParent);
    dateTimeSet = 0;
}


UsersOnlineFilters::~UsersOnlineFilters()
{
}

/*
** setDateTime  - Sets our date and time to filter on.
*/

void UsersOnlineFilters::setDateTime(QDateTime newDateTime)
{
    myDateTime = newDateTime;
    dateCal->setDate(myDateTime.date());
    timeSpin->setTime(QTime::currentTime());
    dateTimeSet = 1;
    loadHosts();
}


/*
** cancelFilters - Closes the window.
*/

void UsersOnlineFilters::cancelFilters()
{
    close();
    delete this;
}


/*
** applyFilters - Emits filtersChanged.
*/

void UsersOnlineFilters::applyFilters()
{
    QTime       tmpTime(timeSpin->time().hour(), timeSpin->time().minute(), 0);

    myDateTime.setDate(dateCal->getQDate());
    myDateTime.setTime(tmpTime);
    emit filtersChanged(myDateTime);
}

/*
** applyAndExit  - Emits filtersChanged, then closes the window.
*/

void UsersOnlineFilters::applyAndExit()
{
    applyFilters();
    cancelFilters();
}


/*
** loadHosts - Loads the list of available hosts from the database.
*/

void UsersOnlineFilters::loadHosts()
{
    if (!dateTimeSet) return;

    QApplication::setOverrideCursor(waitCursor);
    ADB     DB;
    ADB     DB2;
    QDate   tmpDate;
    QTime   tmpTime;
    char    sDate[1024];
    char    sTime[1024];
    
    tmpDate = myDateTime.date();
    tmpTime = myDateTime.time();
    sprintf(sDate, "%04d%02d%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
    sprintf(sTime, "%02d:%02d", tmpTime.hour(), tmpTime.minute());

    serverList->clear();
    DB.query("select distinct Host from ModemUsage where StartDate <= '%s' and StartTime <= '%s:59' and StopDate >= '%s' and StopTime >= '%s:00'",
      sDate,
      sTime,
      sDate,
      sTime
    );
    if (DB.rowCount) while (DB.getrow()) {
        DB2.query("select TextStr from ModemUsageStrings where TextID = %s", DB.curRow[0]);
        DB2.getrow();
        (void) new QListViewItem(serverList, DB2.curRow[0], DB.curRow[0]);
    }
    QApplication::restoreOverrideCursor();
}

