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

#include <ADB.h>
#include <BString.h>

#include "CityDetailReport.h"

using namespace Qt;

CityDetailReport::CityDetailReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Customer City Report" );
	setTitle("Customer Cities");
	
	repBody->setColumnText(0, "Customer ID");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Customer Name");       repBody->setColumnAlignment(1, AlignLeft);
    repBody->addColumn("Primary Login");       repBody->setColumnAlignment(2, AlignLeft);
    repBody->addColumn("City");                repBody->setColumnAlignment(3, AlignLeft);
    repBody->addColumn("State");               repBody->setColumnAlignment(4, AlignLeft);
	
    allowDates(REP_NODATES);
	strcpy(myCity, "");
	strcpy(myState, "");
	refreshReport();
}


CityDetailReport::~CityDetailReport()
{
}

/*
** setCityState -  Sets the city and state...
*/

void CityDetailReport::setCityState(const char *newCity, const char *newState)
{
    strcpy(myCity, newCity);
    strcpy(myState, newState);
    refreshReport();
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void CityDetailReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
//    if (strlen(myCity) && strlen(myState)) {
	    char    *query  = new char[65536];
	    ADB     DB;
	    ADB     DB2;
	    
	    // Get the list of customer ID's for this City.
	    DB.query("select RefID from Addresses where City = '%s' and State = '%s' and RefFrom = 0", myCity, myState);
	    while (DB.getrow()) {
	        DB2.query("select CustomerID, FullName, PrimaryLogin, Active from Customers where CustomerID = %ld", atol(DB.curRow["RefID"]));
	        if (DB2.rowCount) {
	            DB2.getrow();
	            if (atoi(DB2.curRow["Active"])) {
	                (void) new Q3ListViewItem(repBody, 
	                  DB2.curRow["CustomerID"], 
	                  DB2.curRow["FullName"], 
	                  DB2.curRow["PrimaryLogin"],
	                  myCity,
	                  myState
	                );
	            }
	        }
	    }

	    delete query;

//    }    
    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void CityDetailReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        long    CustID = atol(curItem->key(0,0));
        emit(openCustomer(CustID));
    }
}

// vim: expandtab
