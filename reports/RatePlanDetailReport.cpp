/*
** RatePlanDetailReport - Gives a summary of the Customers and the rate plans they
**                  are associated with.
*/

#include "RatePlanDetailReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <q3listview.h>
#include <qapplication.h>

#include <ADB.h>
#include <BString.h>


RatePlanDetailReport::RatePlanDetailReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Rate Plan Detail Report" );
	setTitle("Rate Plan Detail");
	
	repBody->setColumnText(0, "Customer ID");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Customer Name");       repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Primary Login");       repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Plan Date");           repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Billing Cycle");       repBody->setColumnAlignment(4, AlignLeft);
	repBody->addColumn("Balance");             repBody->setColumnAlignment(5, AlignRight);
	repBody->addColumn("Active");              repBody->setColumnAlignment(6, AlignCenter);
	
	allowDates(REP_NODATES);
	myRatePlanID = -1;
    
    resize(700, 500);

	refreshReport();
}


RatePlanDetailReport::~RatePlanDetailReport()
{
}

void RatePlanDetailReport::setRatePlanID(long newPlanID)
{
    myRatePlanID = newPlanID;
    refreshReport();
}

/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void RatePlanDetailReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    if (myRatePlanID >= 0) {
	    char    *tmpStr= new char[65536];
	    char    *query = new char[65536];
	    char    tmpBalance[1024];
	    char    tmpActive[1024];
	    char    tmpCycle[1024];
	    ADB     DB;
	    ADB     DB2;
	    

	    // Get a list of the rate plans.  Then we'll count the customers that
	    // are assigned that rate plan.
	    DB.query("select PlanTag, Description from RatePlans where InternalID = %ld", myRatePlanID);
	    if (DB.rowCount) {
	        // Set up our title.
            DB.getrow();
            sprintf(tmpStr, "Rate Plan Detail Report\n%s - %s", DB.curRow["PlanTag"], DB.curRow["Description"]);
			setCaption( tmpStr);
			setTitle(tmpStr);

            DB.query("select CustomerID, FullName, RatePlanDate, BillingCycle, CurrentBalance, Active, PrimaryLogin from Customers where RatePlan = %ld", myRatePlanID);
            
            if (DB.rowCount) while (DB.getrow()) {

                // Check to see if they are active...
	            if (atoi(DB.curRow["Active"])) {
	                strcpy(tmpActive, "Yes");
	            } else {
	                strcpy(tmpActive, "No");
	            }
	            
	            // Get their billing cycle.
	            DB2.query("select CycleID from BillingCycles where InternalID = %s", DB.curRow["BillingCycle"]);
	            if (DB2.rowCount) {
	                DB2.getrow();
	                strcpy(tmpCycle, DB2.curRow["CycleID"]);
	            } else {
	                strcpy(tmpCycle, DB.curRow["BillingCycle"]);
	            }
	            
	            // Now, reformat their balance so we can sort it.
	            sprintf(tmpBalance, "%8.2f", atof(DB.curRow["CurrentBalance"]));

                // Finally, add the row to the list.
                (void) new Q3ListViewItem(repBody, 
                  DB.curRow["CustomerID"],
                  DB.curRow["FullName"],
                  DB.curRow["PrimaryLogin"],
                  DB.curRow["RatePlanDate"],
                  tmpCycle,
                  tmpBalance,
                  tmpActive
                );	            
	            
	        } else {
	            (void) new Q3ListViewItem(repBody, "No matching customers found");
	        }
	    } else {
	        (void) new Q3ListViewItem(repBody, "No matching rate plan found!!!");
	    }
	    
	    
	    delete query;
	    
    }
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void RatePlanDetailReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        long    CustID = atol(curItem->key(0,0));
        emit(openCustomer(CustID));
    }
}
