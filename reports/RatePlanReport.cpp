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

#include <ADB.h>
#include <BString.h>

#include "RatePlanDetailReport.h"
#include "RatePlanReport.h"

using namespace Qt;

RatePlanReport::RatePlanReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Rate Plans Report" );
	setTitle("Rate Plan Summary");
	
	repBody->setColumnText(0, "Plan Tag");   repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Description");       repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Active");            repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Inactive");          repBody->setColumnAlignment(3, AlignRight);
	
	refreshReport();
	
	allowDates(0);
}


RatePlanReport::~RatePlanReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void RatePlanReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    tmpActiveStr[1024];
    char    tmpInactiveStr[1024];
    
    char    *query = new char[65536];
    ADB     DB;
    ADB     DB2;
    

    // Get a list of the rate plans.  Then we'll count the customers that
    // are assigned that rate plan.
    DB.query("select InternalID, PlanTag, Description from RatePlans");
    if (DB.rowCount) {
        while (DB.getrow()) {
            // Now, get the number of active customers associated with this
            // rate plan.
            DB2.query("select CustomerID from Customers where RatePlan = %s and Active <> 0", DB.curRow["InternalID"]);
            sprintf(tmpActiveStr, "%6ld", DB2.rowCount);
            
            // Now, get the number of INactive customers associated with this
            // rate plan.
            DB2.query("select CustomerID from Customers where RatePlan = %s and Active = 0", DB.curRow["InternalID"]);
            sprintf(tmpInactiveStr, "%6ld", DB2.rowCount);

            // Now, add the entry into the list, including the last column,
            // which is not shown, containing the internal ID so we can
            // zoom in on it.
            (void) new Q3ListViewItem(repBody, DB.curRow["PlanTag"], DB.curRow["Description"], tmpActiveStr, tmpInactiveStr, DB.curRow["InternalID"]);
        } 
    } else {
        (void) new Q3ListViewItem(repBody, "No rate plans found!!!");
    }
    
    
    delete query;
    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected - Brings up a list of customers with the double clicked
**                    rate plan.
*/

void RatePlanReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        RatePlanDetailReport *RPDR = new RatePlanDetailReport();
        RPDR->setRatePlanID(atol(curItem->key(4, 0)));
        RPDR->show();
    }
}


// vim: expandtab
