/**********************************************************************

	--- Qt Architect generated file ---

	File: ModemSummaryFilters.cpp
	Last generated: Thu Oct 7 15:13:53 1999

 *********************************************************************/

#include "ModemSummaryFilters.h"

#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>

#include <qapplication.h>

#define Inherited ModemSummaryFiltersData

ModemSummaryFilters::ModemSummaryFilters
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Modem Summary Filters" );
    
    // Load all of the hosts into memory.
    loadHosts();
    
    showAllHosts = 1;
    selectedServers->setChecked(FALSE);
    allServersButton->setChecked(TRUE);
    serverList->setEnabled(FALSE);
    serverList->setMultiSelection(TRUE);
    
}


ModemSummaryFilters::~ModemSummaryFilters()
{
}

/*
** showHost - Tells the caller whether or not to display a particular hostID.
*/

int ModemSummaryFilters::showHost(int hostID)
{
    if (showAllHosts) return 1;
    if (hostList[hostID]) return 1;
    return 0;
}


/*
** loadHosts - Loads the list of hosts and their names from the database.
*/

void ModemSummaryFilters::loadHosts()
{
    QApplication::setOverrideCursor(waitCursor);
    ADB             DB1;
    ADB             DB2;
    HostInfoStruct  *curItem;
    
    DB1.query("select distinct Host from ModemUsage order by Host");
    if (DB1.rowCount) {
        while (DB1.getrow()) {
            curItem = new HostInfoStruct;
            curItem->HostID = atoi(DB1.curRow["Host"]); // atoi(DB1.curRow[0]);
            DB2.query("select TextStr from ModemUsageStrings where TextID = %d", curItem->HostID);
            if (DB2.rowCount) {
                DB2.getrow();
                strcpy(curItem->HostName, DB2.curRow["TextStr"]);
            } else {
                sprintf(curItem->HostName, "%d - Unknown", curItem->HostID);
            }
            allHosts.append(curItem);
            (void) new QListViewItem(serverList, curItem->HostName, DB1.curRow[0]);
        }
    }
    QApplication::restoreOverrideCursor();
}

/*
** allServersSelected  - Gets called when the user changes the selection.
*/

void ModemSummaryFilters::allServersSelected()
{
    showAllHosts = 1;
    serverList->setEnabled(FALSE);
}

/*
** selectedServersSelected - Gets called when the user changes the selection
**                           they only want to display the selected hosts.
*/

void ModemSummaryFilters::selectedServersSelected()
{
    showAllHosts = 0;
    serverList->setEnabled(TRUE);
    listSelectionChanged();
}

/*
** listSelectionChanged  - When the user selects or deselects an entry
**                         from the host list, this gets called.  This
**                         function recreates the dictionary we have
**                         containing the list of selected hosts we have.
*/

void ModemSummaryFilters::listSelectionChanged()
{
    hostList.clear();
    QListViewItem   *curItem;
    for (curItem = serverList->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        if (curItem->isSelected()) {
            long    *tmpLong = new long[1];
            tmpLong[0] = atol(curItem->key(1,0));
            hostList.insert(atol(curItem->key(1,0)), tmpLong);
        }
    }
}

void ModemSummaryFilters::applyFilters()
{
    emit filtersChanged();
}

void ModemSummaryFilters::applyAndExit()
{
    emit filtersChanged();
    hide();
}


