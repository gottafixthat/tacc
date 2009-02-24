/*
** $Id$
**
***************************************************************************
**
** AllDomainsReport - Displays the list of all of the domains that TAA has
**                    in the databse.  It allows the user to double click
**                    an item and bring up the customer window associated  
**                    with the domain.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: AllDomainsReport.cpp,v $
** Revision 1.2  2003/12/07 18:12:32  marc
** Report complete.
**
** Revision 1.1  2003/12/07 17:49:37  marc
** New report, AllDomains.
**
**
*/


#include "AllDomainsReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>
#include <Cfg.h>


AllDomainsReport::AllDomainsReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "All Domains Report" );
	setTitle("All Domains");
	
	repBody->setColumnText(0, "Domain Name");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Username");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");                repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Cust ID");             repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Act");                 repBody->setColumnAlignment(4, AlignLeft);
	repBody->addColumn("DNS");                 repBody->setColumnAlignment(5, AlignLeft);
	
    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}


AllDomainsReport::~AllDomainsReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void AllDomainsReport::refreshReport()
{
    repBody->clear();
    ADB     DB;
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char    doingDNS[1024];
    char    isActive[1024];
    long    counter = 0;

    DB.query("select Domains.CustomerID, Domains.LoginID, Domains.Active, DomainTypes.DomainType, Domains.DomainName from Domains, DomainTypes where DomainTypes.InternalID = Domains.DomainType");
    if (DB.rowCount) while (DB.getrow()) {
        emit(setProgress(++counter, DB.rowCount));
        strcpy(doingDNS, "No");
        strcpy(isActive, "No");
        dnsDB.query("select id from soa where origin= '%s.'", DB.curRow["DomainName"]);
        if (dnsDB.rowCount) strcpy(doingDNS, "Yes");
        if (atoi(DB.curRow["Active"])) strcpy(isActive, "Yes");

        (void) new QListViewItem(repBody, DB.curRow["DomainName"], DB.curRow["LoginID"], DB.curRow["DomainType"], DB.curRow["CustomerID"], isActive, doingDNS);
    }
    
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void AllDomainsReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(3,0))));
    }
}


