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

#include <ActiveDomainsDetail.h>
#include "ActiveDomainsReport.h"

using namespace Qt;

ActiveDomainsReport::ActiveDomainsReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Active Domains Report" );
	setTitle("Active Domains");
	
	repBody->setColumnText(0, "Domain Type");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Count");       repBody->setColumnAlignment(1, AlignRight);
	
    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}


ActiveDomainsReport::~ActiveDomainsReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ActiveDomainsReport::refreshReport()
{
    repBody->clear();
    ADB     DB;
    ADB     DB2;
    char    tmpStr[1024];
    DB.query("select * from DomainTypes order by DomainType");
    while (DB.getrow()) {
        DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        sprintf(tmpStr, "%5d", DB2.rowCount);
        (void) new Q3ListViewItem(repBody, DB.curRow["DomainType"], tmpStr, DB.curRow["InternalID"]);
    }
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void ActiveDomainsReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        ActiveDomainsDetail *add = new ActiveDomainsDetail();
        add->setDomainType(atoi(curItem->key(2,0)));
        add->show();
    }
}



// vim: expandtab
