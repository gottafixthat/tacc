/*
** ActiveDomainsDetail - Gives a summary of cities that our active customers are
**              located in.
*/

#include "ActiveDomainsDetail.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>


ActiveDomainsDetail::ActiveDomainsDetail
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Active Domains Report" );
	setTitle("Active Domains");
	
	list->setColumnText(0, "Domain Name");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Cust ID");       list->setColumnAlignment(1, AlignRight);
	list->addColumn("Username");      list->setColumnAlignment(2, AlignLeft);
	list->addColumn("Act");           list->setColumnAlignment(2, AlignLeft);

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
    list->clear();
    if (!myDomainType) return;
    emit(setStatus("Generating report..."));
    ADB     DB;
    char    tmpStr[1024];
    DB.query(" select Domains.DomainName, Domains.CustomerID, Domains.LoginID, Logins.Active from Domains, Logins where Domains.DomainType = %d and Logins.LoginID = Domains.LoginID and Domains.Active > 0", myDomainType);
    while (DB.getrow()) {
        if (atoi(DB.curRow["Active"])) strcpy(tmpStr, "Yes");
        else strcpy(tmpStr, "No");
        (void) new QListViewItem(list, DB.curRow["DomainName"], DB.curRow["CustomerID"], DB.curRow["LoginID"], tmpStr);
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

void ActiveDomainsDetail::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(myCustIDColumn,0))));
    }
}


