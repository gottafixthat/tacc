/*
** $Id: LoginCountReport.cpp,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** LoginCountReport - Creates a report listing the number of logins on 
**                    the system at present.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: LoginCountReport.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "LoginCountReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>

#include <Report.h>

#include <ADB.h>
#include <Cfg.h>
#include <FParse.h>
#include <BString.h>

#include "LoginTypeReport.h"

LoginCountReport::LoginCountReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Current Login Count Report" );
	setTitle("Current Login Counts");

	list->setColumnText(0, "Login Type");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Description");        list->setColumnAlignment(1, AlignLeft);
	list->addColumn("Active");             list->setColumnAlignment(2, AlignRight);
	list->addColumn("Inactive");           list->setColumnAlignment(3, AlignRight);
	list->addColumn("System");             list->setColumnAlignment(4, AlignRight);
	list->addColumn("Total");              list->setColumnAlignment(5, AlignRight);
	
	// Set the sorting to a hidden column.
    list->setSorting(6);

    // resize(675, 400);

    // This is a snapshot report.  We don't want to see the dates or filters.
    allowDates(REP_NODATES);
    allowFilters(0);

	refreshReport();
	
}


LoginCountReport::~LoginCountReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void LoginCountReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    char    tmpst1[1024];
    char    tmpst2[1024];
    char    tmpst3[1024];
    char    tmpst4[1024];
    long    activeCount = 0;
    long    inactiveCount = 0;
    long    curActiveCount = 0;
    long    curInactiveCount = 0;
    long    curSystemCount = 0;
    long    systemCount = 0;
    int     rowCount = 0;
    char    rowCountSt[1024];
    char    todayStr[1024];
    char    typeStr[1024];

    sprintf(todayStr, "As of %s", (const char *) QDateTime::currentDateTime().toString());

    /*
    FParser parser;
    parser.set("Title", "Login Count Report");
    parser.set("ReportDate", todayStr);
    */

    QApplication::setOverrideCursor(waitCursor);

	// Set the sorting to a hidden column.
    list->setSorting(6);
    
    list->clear();
    DB.query("select InternalID, LoginType, Description from LoginTypes order by LoginType");
    if (DB.rowCount) while (DB.getrow()) {
        sprintf(rowCountSt, "%05d", rowCount++);
        sprintf(typeStr, DB.curRow["InternalID"]);
    
        DB2.query("select InternalID from Logins where LoginType = %d and Active <> 0 and Wiped = 0 and CustomerID >= 100", atoi(DB.curRow["InternalID"]));
        curActiveCount  = DB2.rowCount;
        activeCount     += curActiveCount;
        
        DB2.query("select InternalID from Logins where LoginType = %d and Active = 0 and Wiped = 0 and CustomerID >= 100", atoi(DB.curRow["InternalID"]));
        curInactiveCount  = DB2.rowCount;
        inactiveCount     += curInactiveCount;

        DB2.query("select InternalID from Logins where LoginType = %d and CustomerID < 100", atoi(DB.curRow["InternalID"]));
        curSystemCount  = DB2.rowCount;
        systemCount     += curSystemCount;

        // Add the columns.
        sprintf(tmpst1, "%ld", curActiveCount);
        sprintf(tmpst2, "%ld", curInactiveCount);
        sprintf(tmpst3, "%ld", curSystemCount);
        sprintf(tmpst4, "%ld", curActiveCount + curInactiveCount);
        /*
        parser.addRow("ReportData");
        parser.addColumn("LoginType", DB.curRow["LoginType"]);
        parser.addColumn("Description", DB.curRow["Description"]);
        parser.addColumn("ActiveCount", tmpst1);
        parser.addColumn("InactiveCount", tmpst2);
        parser.addColumn("SystemCount", tmpst3);
        parser.addColumn("TotalCount", tmpst4);
        */
        (void) new QListViewItem(list, DB.curRow["LoginType"], DB.curRow["Description"], tmpst1, tmpst2, tmpst3, tmpst4, rowCountSt, DB.curRow["InternalID"]);
    }

    // Add the totals.
    sprintf(rowCountSt, "%05d", rowCount++);
    sprintf(tmpst1, "%ld", activeCount);
    sprintf(tmpst2, "%ld", inactiveCount);
    sprintf(tmpst3, "%ld", systemCount);
    sprintf(tmpst4, "%ld", activeCount + inactiveCount);
    /*
    parser.set("TotalActive", tmpst1);
    parser.set("TotalInactive", tmpst2);
    parser.set("TotalSystem", tmpst3);
    parser.set("GrandTotal", tmpst4);
    */
    (void) new QListViewItem(list, "TOTAL", "", tmpst1, tmpst2, tmpst3, tmpst4);
    /*
    char    fName[4096];
    strcpy(fName, cfgVal("ReportPath"));
    strcpy(fName, "/usr/local/lib/taa/reports/LoginCount.tmpl");
    body->setText(parser.parseFileToMem(fName));
    */

    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected  - Gets called when the user double clicks on an
**                     entry.  Pulls up the LoginTypeReport for the
**                     clicked on login type.
*/

void LoginCountReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem) {
        if (atoi(curItem->key(7,0))) {
            LoginTypeReport *ltr = new LoginTypeReport();
            ltr->setLoginType(atoi(curItem->key(7,0)));
            ltr->show();
        }
    }
}
