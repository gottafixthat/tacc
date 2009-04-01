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
#include <Cfg.h>
#include <DNSManager.h>

#include "DNS_ManagedReport.h"

using namespace Qt;

DNS_ManagedReport::DNS_ManagedReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Managed DNS Domains Report" );
	setTitle("Managed DNS Domains Report");
	
	repBody->setColumnText(0, "Domain Name");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Username");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");                repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Cust ID");             repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Act");                 repBody->setColumnAlignment(4, AlignLeft);
	repBody->addColumn("Template");            repBody->setColumnAlignment(5, AlignLeft);
	
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
    repBody->clear();
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

        (void) new Q3ListViewItem(repBody, domainName, dnsDB.curRow["LoginID"], domainType, dnsDB.curRow["CustomerID"], isActive, templateName);
    }
    
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void DNS_ManagedReport::listItemSelected(Q3ListViewItem *curItem)
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
    Q3ListViewItem *curItem = repBody->currentItem();
    if (curItem != NULL) {
        DNSManager  *dnsm = new DNSManager();
        dnsm->setDomainName(curItem->key(0,0));
        dnsm->show();
    }
}


// vim: expandtab
