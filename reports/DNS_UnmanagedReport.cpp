/*
** $Id$
**
***************************************************************************
**
** DNS_UnmanagedReport - Displays the list of all of the domains that TAA
**                       has that are NOT managed via MyDNS.
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
** $Log: DNS_UnmanagedReport.cpp,v $
** Revision 1.1  2004/01/04 19:59:06  marc
** New report, unmanaged active domains.
**
**
*/


#include "DNS_UnmanagedReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <q3listview.h>
#include <qapplication.h>
#include <q3dict.h>

#include <BlargDB.h>
#include <BString.h>
#include <Cfg.h>


DNS_UnmanagedReport::DNS_UnmanagedReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Unmanaged DNS Domains Report" );
	setTitle("Unmanaged DNS Domains Report");
	
	repBody->setColumnText(0, "Domain Name");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Username");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");                repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Cust ID");             repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Act");                 repBody->setColumnAlignment(4, AlignLeft);
	
    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}


DNS_UnmanagedReport::~DNS_UnmanagedReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void DNS_UnmanagedReport::refreshReport()
{
    repBody->clear();
    ADB     DB;
    ADB     DB2;
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char    domainName[1024];
    char    origin[1024];
    char    domainType[1024];
    char    isActive[1024];
    long    counter = 0;

    DB.query("select * from Domains where Active <> 0");
    if (DB.rowCount) while (DB.getrow()) {
        emit(setProgress(++counter, DB.rowCount));
        strcpy(domainType, "Unknown");
        strcpy(isActive, "");
        // Copy the domain name into a temp string and create the origin
        // for our remote query.
        strcpy(domainName, DB.curRow["DomainName"]);
        strcpy(origin, domainName);
        strcat(origin, ".");
        dnsDB.query("select soa.id, ZoneInfo.CustomerID from soa, ZoneInfo where soa.id = ZoneInfo.zoneid and soa.origin = '%s' and ZoneInfo.CustomerID = %ld", origin, atol(DB.curRow["CustomerID"]));
        if (!dnsDB.rowCount) {
            // Get the type of domain it is
            DB2.query("select DomainTypes.DomainType, Domains.InternalID from DomainTypes, Domains where Domains.DomainName = '%s' and DomainTypes.InternalID = Domains.DomainType", domainName);
            if (DB2.rowCount) {
                DB2.getrow();
                strcpy(domainType, DB2.curRow["DomainType"]);
            }
            // Check to see if it is active
            if (atoi(DB.curRow["Active"])) strcpy(isActive, "Yes");
            else strcpy(isActive, "No");

            (void) new Q3ListViewItem(repBody, domainName, DB.curRow["LoginID"], domainType, DB.curRow["CustomerID"], isActive);
        }
    }
    
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void DNS_UnmanagedReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(3,0))));
    }
}


