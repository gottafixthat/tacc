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

#include "AsteriskCDRReport.h"

using namespace Qt;

AsteriskCDRReport::AsteriskCDRReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Asterisk Call Detail Records" );
	setTitle("Call Detail Records");
	
	repBody->setColumnText(0, "Call Time");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("CallerID");          repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Source");            repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Dest");              repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("Context");           repBody->setColumnAlignment(4, AlignLeft);
	repBody->addColumn("Src Channel");       repBody->setColumnAlignment(5, AlignLeft);
	repBody->addColumn("Dest Channel");      repBody->setColumnAlignment(6, AlignLeft);
	repBody->addColumn("lastapp");           repBody->setColumnAlignment(7, AlignLeft);
	repBody->addColumn("lastdata");          repBody->setColumnAlignment(8, AlignLeft);
	repBody->addColumn("duration");          repBody->setColumnAlignment(9, AlignLeft);
	repBody->addColumn("billsec");           repBody->setColumnAlignment(10, AlignLeft);
	repBody->addColumn("disposition");       repBody->setColumnAlignment(11, AlignLeft);
	repBody->addColumn("uniqueid");          repBody->setColumnAlignment(12, AlignLeft);
	
    allowDates(REP_ALLDATES);
    allowFilters(0);
    dateList->setCurrentText("Today");
    startDateCal->setDate(QDate::currentDate());
    endDateCal->setDate(QDate::currentDate());
	refreshReport();
}


AsteriskCDRReport::~AsteriskCDRReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void AsteriskCDRReport::refreshReport()
{
    repBody->clear();
    ADB     DB;
    ADB     DB2;
    char    tmpStr[1024];
    QDate   sDate = this->startDateCal->date();
    QDate   eDate = this->endDateCal->date();

    DB.query("select * from cdr where calldate >= '%04d-%02d-%02d 00:00:00' and calldate <= '%04d-%02d-%02d 23:59:59' order by calldate",
            sDate.year(), sDate.month(), sDate.day(),
            eDate.year(), eDate.month(), eDate.day());
    while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        Q3ListViewItem *curItem = new Q3ListViewItem(repBody, 
                DB.curRow["calldate"], 
                DB.curRow["clid"], 
                DB.curRow["src"], 
                DB.curRow["dst"],
                DB.curRow["dcontext"],
                DB.curRow["channel"],
                DB.curRow["dstchannel"],
                DB.curRow["lastapp"]
                );
        curItem->setText(8, DB.curRow["lastdata"]);
        curItem->setText(9, DB.curRow["duration"]);
        curItem->setText(10, DB.curRow["billsec"]);
        curItem->setText(11, DB.curRow["disposition"]);
        curItem->setText(12, DB.curRow["uniqueid"]);

    }
}


/*
** listItemSelected - Loads the selected customer when the list item
**                    is double clicked.
*/

void AsteriskCDRReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        //AsteriskCDRDetail *add = new AsteriskCDRDetail();
        //add->setDomainType(atoi(curItem->key(2,0)));
        //add->show();
    }
}



// vim: expandtab
