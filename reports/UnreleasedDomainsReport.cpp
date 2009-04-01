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

#include "UnreleasedDomainsReport.h"

using namespace Qt;

UnreleasedDomainsReport::UnreleasedDomainsReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Unreleased Domains Report" );
	setTitle("Unreleased Domains");
	
	repBody->setColumnText(0, "CustID");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Username");       repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");           repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Domain Name");    repBody->setColumnAlignment(3, AlignLeft);
	
    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}


UnreleasedDomainsReport::~UnreleasedDomainsReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void UnreleasedDomainsReport::refreshReport()
{
    repBody->clear();
    ADB     DB;
    DB.query("select Domains.CustomerID, Domains.LoginID, DomainTypes.DomainType, Domains.DomainName from Domains, DomainTypes where Domains.Active <> 0 and Domains.Released = '' and DomainTypes.InternalID = Domains.DomainType");
    if (DB.rowCount) while (DB.getrow()) {
        (void) new Q3ListViewItem(repBody, DB.curRow["CustomerID"], DB.curRow["LoginID"], DB.curRow["DomainType"], DB.curRow["DomainName"]);
    }
    
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void UnreleasedDomainsReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(0,0))));
    }
}



// vim: expandtab
