/**************************************************************************
**
** AsteriskCDRReport - Displays the asterisk Call Detail Records.
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

#include "AsteriskCDRReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>

#include <BlargDB.h>
#include <BString.h>

//#include <AsteriskCDRDetail.h>


AsteriskCDRReport::AsteriskCDRReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Asterisk Call Detail Records" );
	setTitle("Call Detail Records");
	
	list->setColumnText(0, "Call Time");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("CallerID");          list->setColumnAlignment(1, AlignLeft);
	list->addColumn("Source");            list->setColumnAlignment(2, AlignLeft);
	list->addColumn("Dest");              list->setColumnAlignment(3, AlignLeft);
	list->addColumn("Context");           list->setColumnAlignment(4, AlignLeft);
	list->addColumn("Src Channel");       list->setColumnAlignment(5, AlignLeft);
	list->addColumn("Dest Channel");      list->setColumnAlignment(6, AlignLeft);
	list->addColumn("lastapp");           list->setColumnAlignment(7, AlignLeft);
	list->addColumn("lastdata");          list->setColumnAlignment(8, AlignLeft);
	list->addColumn("duration");          list->setColumnAlignment(9, AlignLeft);
	list->addColumn("billsec");           list->setColumnAlignment(10, AlignLeft);
	list->addColumn("disposition");       list->setColumnAlignment(11, AlignLeft);
	list->addColumn("uniqueid");          list->setColumnAlignment(12, AlignLeft);
	
    allowDates(REP_ALLDATES);
    allowFilters(0);
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
    list->clear();
    ADB     DB;
    ADB     DB2;
    char    tmpStr[1024];
    DB.query("select * from cdr order by calldate");
    while (DB.getrow()) {
        //DB2.query("select * from Domains where DomainType = %d and Active > 0", atol(DB.curRow["InternalID"]));
        //sprintf(tmpStr, "%5d", DB2.rowCount);
        QListViewItem *curItem = new QListViewItem(list, 
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

void AsteriskCDRReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        //AsteriskCDRDetail *add = new AsteriskCDRDetail();
        //add->setDomainType(atoi(curItem->key(2,0)));
        //add->show();
    }
}


