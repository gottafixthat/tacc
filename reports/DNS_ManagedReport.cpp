/*
** $Id: DNS_ManagedReport.cpp,v 1.2 2004/01/12 23:26:25 marc Exp $
**
***************************************************************************
**
** DNS_ManagedReport - Displays the list of all of the domains that TAA has
**                     in the databse that have managed DNS.
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
** $Log: DNS_ManagedReport.cpp,v $
** Revision 1.2  2004/01/12 23:26:25  marc
** Added a DNS button as a user button as a shortcut to editing DNS records.
**
** Revision 1.1  2004/01/04 19:35:48  marc
** New report, Managed DNS domains
**
**
*/


#include "DNS_ManagedReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>
#include <Cfg.h>
#include <DNSManager.h>


DNS_ManagedReport::DNS_ManagedReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Managed DNS Domains Report" );
	setTitle("Managed DNS Domains Report");
	
	list->setColumnText(0, "Domain Name");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Username");            list->setColumnAlignment(1, AlignLeft);
	list->addColumn("Type");                list->setColumnAlignment(2, AlignLeft);
	list->addColumn("Cust ID");             list->setColumnAlignment(3, AlignLeft);
	list->addColumn("Act");                 list->setColumnAlignment(4, AlignLeft);
	list->addColumn("Template");            list->setColumnAlignment(5, AlignLeft);
	
    allowDates(REP_NODATES);
    allowFilters(0);
    setUserButton("&DNS");
	refreshReport();
}


DNS_ManagedReport::~DNS_ManagedReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void DNS_ManagedReport::refreshReport()
{
    list->clear();
    ADB     DB;
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char    domainName[1024];
    char    domainType[1024];
    char    isActive[1024];
    char    templateName[1024];
    long    counter = 0;

    dnsDB.query("select ZoneInfo.CustomerID, ZoneInfo.LoginID, ZoneInfo.Propogate, ZoneInfo.AllowCustEdit, ZoneInfo.TemplateID, ZoneInfo.Active, soa.origin from ZoneInfo, soa where soa.id = ZoneInfo.zoneid");
    if (dnsDB.rowCount) while (dnsDB.getrow()) {
        emit(setProgress(++counter, dnsDB.rowCount));
        strcpy(templateName, "");
        strcpy(domainType, "Unknown");
        strcpy(isActive, "");
        // Get rid of the trailing "." at the end of the origin
        strcpy(domainName, dnsDB.curRow["origin"]);
        while (domainName[strlen(domainName)-1] == '.') {
            domainName[strlen(domainName)-1] = '\0';
        }
        DB.query("select TemplateName from DNS_Templates where TemplateID = %ld", atol(dnsDB.curRow["TemplateID"]));
        if (DB.rowCount) {
            DB.getrow();
            strcpy(templateName, DB.curRow["TemplateName"]);
        }
        // Get the type of domain it is
        DB.query("select DomainTypes.DomainType, Domains.InternalID from DomainTypes, Domains where Domains.DomainName = '%s' and DomainTypes.InternalID = Domains.DomainType", domainName);
        if (DB.rowCount) {
            DB.getrow();
            strcpy(domainType, DB.curRow["DomainType"]);
        }
        // Check to see if it is active
        if (atoi(dnsDB.curRow["Active"])) strcpy(isActive, "Yes");
        else strcpy(isActive, "No");

        (void) new QListViewItem(list, domainName, dnsDB.curRow["LoginID"], domainType, dnsDB.curRow["CustomerID"], isActive, templateName);
    }
    
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void DNS_ManagedReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(3,0))));
    }
}

/*
** userButtonClickedInt - Gets called when the user clicks on the DNS button.
*/

void DNS_ManagedReport::userButtonClickedInt()
{
    QListViewItem *curItem = list->currentItem();
    if (curItem != NULL) {
        DNSManager  *dnsm = new DNSManager();
        dnsm->setDomainName(curItem->key(0,0));
        dnsm->show();
    }
}

