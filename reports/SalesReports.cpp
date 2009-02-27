/**************************************************************************
**
** SalesReports - Creates reports on sales statistics.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#include "SalesReports.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>


salesByServiceReport::salesByServiceReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Sales By Service" );
	setTitle("Sales By Service");
	
    int c = 0;
	repBody->setColumnText(0, "Service");  repBody->setColumnAlignment(c++, AlignLeft);
    //repBody->addColumn("Qty");             repBody->setColumnAlignment(c++, AlignRight);
    repBody->addColumn("Amount");          repBody->setColumnAlignment(c++, AlignRight);
	
    // When starting out, do this month.
    allowDates(REP_ALLDATES);
    QDate   curDate = QDate::currentDate();
    QDate   dayFirst;
    QDate   dayLast;

    dayFirst.setYMD(curDate.year(), curDate.month(), 1);
    dayLast.setYMD(curDate.year(), curDate.month(), curDate.daysInMonth());

    startDateCal->setDate(dayFirst);
    endDateCal->setDate(dayLast);
    dateList->setCurrentText("This Month");


    allowFilters(0);
	refreshReport();
}


salesByServiceReport::~salesByServiceReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void salesByServiceReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    QDate   sDate = this->startDateCal->date();
    QDate   eDate = this->endDateCal->date();
    char    tmpStr[1024];
    float   grandTotal = 0.0;

    QApplication::setOverrideCursor(waitCursor);
    repBody->clear();

    // Two queries, one for standalone billables, one for packages.
    DB.query("select AcctsRecv.ItemID, Billables.ItemID, sum(AcctsRecv.Amount) from AcctsRecv, Billables where Billables.ItemNumber = AcctsRecv.ItemID and AcctsRecv.TransType = 0 and AcctsRecv.ItemID > 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' group by AcctsRecv.ItemID order by Billables.ItemID", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        if (atof(DB.curRow[2]) > 0.0) {
            QListViewItem *curItem = new QListViewItem(repBody, 
                    DB.curRow[1], 
                    DB.curRow[2]
                    );
            grandTotal += atof(DB.curRow[2]);
        }
    }
    
    // The seocnd query for packages.
    DB.query("select AcctsRecv.PackageItem, Packages.PackageTag, sum(AcctsRecv.Amount) from AcctsRecv, Packages where Packages.InternalID = AcctsRecv.PackageItem and AcctsRecv.TransType = 0 and AcctsRecv.ItemID = 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d' group by Packages.InternalID order by Packages.PackageTag", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        sprintf(tmpStr, "%s - Package", DB.curRow[1]);
        if (atof(DB.curRow[2]) > 0.0) {
            QListViewItem *curItem = new QListViewItem(repBody, 
                    tmpStr,
                    DB.curRow[2]
                    );
            grandTotal += atof(DB.curRow[2]);
        }
    }
    // One last query to get the stuff in the database prior to 5/1/06
    DB.query("select sum(AcctsRecv.Amount) from AcctsRecv where AcctsRecv.TransType = 0 and AcctsRecv.ItemID = 0 and AcctsRecv.PackageItem = 0 and AcctsRecv.TransDate  >= '%04d-%02d-%02d' and AcctsRecv.TransDate <= '%04d-%02d-%02d'", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day());
    if (DB.rowCount) while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        //sprintf(tmpStr, "%s - Package", DB.curRow[1]);
        if (atof(DB.curRow[0]) > 0.0) {
            QListViewItem *curItem = new QListViewItem(repBody, 
                    "[Unknown Items]",
                    DB.curRow[0]
                    );
            grandTotal += atof(DB.curRow[0]);
        }
    }

    sprintf(tmpStr, "Sales by Service\n%02d-%02d-%04d through %02d-%02d-%04d, $%.2f", sDate.year(), sDate.month(), sDate.day(), eDate.year(), eDate.month(), eDate.day(), grandTotal);
	setTitle(tmpStr);

    QApplication::restoreOverrideCursor();
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void salesByServiceReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        //AsteriskCDRDetail *add = new AsteriskCDRDetail();
        //add->setDomainType(atoi(curItem->key(2,0)));
        //add->show();
    }
}


