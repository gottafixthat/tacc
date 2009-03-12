/**
 * AgingReport.cpp - Finds all customers with an open balance that is overdue.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>
#include <Cfg.h>

#include <AgingReport.h>

/**
 * AgingReport()
 *
 * Constructor.
 */
AgingReport::AgingReport(QWidget* parent, const char* name) : 
    Report( parent, name )
{
	setCaption( "Aging Report" );
	setTitle("Aging Report");
	
	repBody->setColumnText(0, "Domain Name");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Username");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");                repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Cust ID");             repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Act");                 repBody->setColumnAlignment(4, AlignLeft);
	
    custIDCol = 3;

    allowDates(REP_NODATES);
    allowFilters(0);
	refreshReport();
}

/**
 * ~AgingReport()
 *
 * Destructor.
 */
AgingReport::~AgingReport()
{
}


/**
 * refreshReport()
 *
 * Fills the report with the overdue customers.
 */
void AgingReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);
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

            (void) new QListViewItem(repBody, domainName, DB.curRow["LoginID"], domainType, DB.curRow["CustomerID"], isActive);
        }
    }
    
    QApplication::restoreOverrideCursor();
}

/**
 * listItemSelected()
 *
 * Called when the user double clicks an entry in the report.
 * Opens the customer window.
 */
void AgingReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        emit(openCustomer(atol(curItem->key(custIDCol,0))));
    }
}


