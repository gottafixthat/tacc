/*
** CityDetailReport - Gives a summary of cities that our active customers are
**              located in.
*/

#include "CityDetailReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <ADB.h>
#include <BString.h>

CityDetailReport::CityDetailReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Customer City Report" );
	setTitle("Customer Cities");
	
	list->setColumnText(0, "Customer ID");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Customer Name");       list->setColumnAlignment(1, AlignLeft);
    list->addColumn("Primary Login");       list->setColumnAlignment(2, AlignLeft);
    list->addColumn("City");                list->setColumnAlignment(3, AlignLeft);
    list->addColumn("State");               list->setColumnAlignment(4, AlignLeft);
	
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

    list->clear();
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
	                (void) new QListViewItem(list, 
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

void CityDetailReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        long    CustID = atol(curItem->key(0,0));
        emit(openCustomer(CustID));
    }
}
