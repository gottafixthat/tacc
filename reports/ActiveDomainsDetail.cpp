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

#include <BlargDB.h>
#include <BString.h>

#include "ActiveDomainsDetail.h"

using namespace Qt;

ActiveDomainsDetail::ActiveDomainsDetail
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Active Domains Report" );
	setTitle("Active Domains");
	
	repBody->setColumnText(0, "Domain Name");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Cust ID");       repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("Username");      repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Act");           repBody->setColumnAlignment(2, AlignLeft);

    myCustIDColumn = 1;
    myDomainType = 0;
	
    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}


ActiveDomainsDetail::~ActiveDomainsDetail()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ActiveDomainsDetail::refreshReport()
{
    repBody->clear();
    if (!myDomainType) return;
    emit(setStatus("Generating report..."));
    ADB     DB;
    char    tmpStr[1024];
    DB.query(" select Domains.DomainName, Domains.CustomerID, Domains.LoginID, Logins.Active from Domains, Logins where Domains.DomainType = %d and Logins.LoginID = Domains.LoginID and Domains.Active > 0", myDomainType);
    while (DB.getrow()) {
        if (atoi(DB.curRow["Active"])) strcpy(tmpStr, "Yes");
        else strcpy(tmpStr, "No");
        (void) new Q3ListViewItem(repBody, DB.curRow["DomainName"], DB.curRow["CustomerID"], DB.curRow["LoginID"], tmpStr);
    }
    emit(setStatus(""));
}

/*
** setDomainType - Sets the type of domain we are reporting by number.
*/

void ActiveDomainsDetail::setDomainType(int newDomainType)
{
    myDomainType = newDomainType;
    ADB     DB;
    char    tmpStr[1024];
    DB.query("select DomainType from DomainTypes where InternalID = %d", myDomainType);
    if (DB.rowCount) {
        DB.getrow();
        sprintf(tmpStr, "Active %s Report", DB.curRow["DomainType"]);
        setCaption(tmpStr);
        setTitle(tmpStr);
    } else {
        setCaption( "Active Domains Report" );
        setTitle("Active Domains");
    }
    refreshReport();
}

/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void ActiveDomainsDetail::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(myCustIDColumn,0))));
    }
}



// vim: expandtab
