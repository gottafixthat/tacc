/*
** CityReport - Gives a summary of cities that our active customers are
**              located in.
*/

#include "CityReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <q3listview.h>
#include <qapplication.h>
#include <q3dict.h>

#include <BlargDB.h>
#include <BString.h>

#include "CityDetailReport.h"
#include "CityFilters.h"

CityReport::CityReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Customer City Report" );
	setTitle("Customer Cities");
	
	repBody->setColumnText(0, "City");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("State");        repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Count");        repBody->setColumnAlignment(2, AlignRight);
	
    filters = new CityFilters();
    connect(filters, SIGNAL(optionsUpdated()), this, SLOT(refreshReport()));

    allowDates(REP_NODATES);
    allowFilters(1);
    myShowActive = 0;
	refreshReport();
}


CityReport::~CityReport()
{
    delete filters;
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void CityReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);
    myShowActive = filters->getActiveStatus();

    repBody->clear();
    repBody->setSorting(2, FALSE);     // Sort by count, highest first.
    
    // We'll use a QDict for speed...
    Q3Dict<int>     dict(521, FALSE);   // 521 is a prime number...

    char    *query  = new char[65536];
    char    *tmpSt  = new char[65536];
    char    *tmpSt2 = new char[65536];
    char    filterPart[2048];
    BlargDB DB;
    BlargDB DB2;
    
    // Populate our city list.
    sprintf(query, "select distinct City, State from Addresses order by City");
    DB.query(query);
    while (DB.getrow()) {
        int     *tmpInt = new int;
        *tmpInt = 0;
        sprintf(tmpSt, "%s, %s", DB.curRow[0], DB.curRow[1]);
        dict.insert(tmpSt, tmpInt);
    }

    // Okay, now, get the list of active customers...
    switch (myShowActive) {
        case    1:      // Inactive only
            strcpy(filterPart, " and Customers.Active = 0");
            sprintf(query, "select CustomerID, BillingAddress from Customers where Active = 0");
        	setTitle("Customer Cities\nInactive Customers");
            break;
            
        case    2:      // Both
            strcpy(filterPart, "");
            sprintf(query, "select CustomerID, BillingAddress from Customers");
        	setTitle("Customer Cities\nAll Customers");
            break;
        
        default:        // Active only
            strcpy(filterPart, " and Customers.Active > 0");
            sprintf(query, "select CustomerID, BillingAddress from Customers where Active > 0");
        	setTitle("Customer Cities\nActive Customers");
            break;
    }
    sprintf(query, "select distinct Customers.CustomerID, Addresses.City, Addresses.State from Customers, Addresses where Addresses.RefFrom = 0 and Addresses.RefID = Customers.CustomerID %s", filterPart);
    DB.query(query);
    while (DB.getrow()) {
        sprintf(tmpSt, "%s, %s", DB.curRow[1], DB.curRow[2]);
        int *tmpInt;
        tmpInt = dict[tmpSt];
        if (tmpInt != NULL) *tmpInt += 1;
        
        #if 0
        sprintf(query, "select City, State from Addresses where RefFrom = 0 and RefID = %ld and Tag = '%s'", atol(DB.curRow[0]), DB.curRow[1]);
        DB2.query(query);
        if (DB2.rowCount) {
            DB2.getrow();
            sprintf(tmpSt, "%s, %s", DB2.curRow[0], DB2.curRow[1]);
            int *tmpInt;
            tmpInt = dict[tmpSt];
            if (tmpInt != NULL) *tmpInt += 1;
        }
        #endif
    }
    
    // Now, go through the city list a second time and add any 
    // cities that have a customer count > 0 to our report list.
    sprintf(query, "select distinct City, State from Addresses order by City");
    DB.query(query);
    while (DB.getrow()) {
        int     *tmpInt;
        sprintf(tmpSt, "%s, %s", DB.curRow[0], DB.curRow[1]);
        tmpInt = dict[tmpSt];
        if (tmpInt != NULL) {
            if (*tmpInt > 0) {
                sprintf(tmpSt2, "%8d", *tmpInt);
                (void) new Q3ListViewItem(repBody, DB.curRow[0], DB.curRow[1], tmpSt2);
            }
        }
    }

    delete query;
    delete tmpSt;
    delete tmpSt2;
    
    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void CityReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        char    tmpCity[512];
        char    tmpState[512];
        strcpy(tmpCity, curItem->key(0,0));
        strcpy(tmpState, curItem->key(1,0));
        
        CityDetailReport    *CDR = new CityDetailReport();
        CDR->setCityState(tmpCity, tmpState);
        CDR->show();
    }
}

/*
** editFilters  - Allows the user to edit the filters that are applied
**                to the report.
*/

void CityReport::editFilters()
{
    filters->show();
}

