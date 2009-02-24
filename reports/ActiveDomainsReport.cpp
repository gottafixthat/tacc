/*
** ActiveDomainsReport - Gives a summary of cities that our active customers are
**              located in.
*/

#include "ActiveDomainsReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>

#include <ActiveDomainsDetail.h>


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
        (void) new QListViewItem(repBody, DB.curRow["DomainType"], tmpStr, DB.curRow["InternalID"]);
    }
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void ActiveDomainsReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        ActiveDomainsDetail *add = new ActiveDomainsDetail();
        add->setDomainType(atoi(curItem->key(2,0)));
        add->show();
    }
}


