/*
** $Id$
**
***************************************************************************
**
** CalLLogReport - Given a date range, this shows the calls that were
**                 logged.
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
** $Log: $
**
*/

#include "CallLogReport.h"

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

CallLogReport::CallLogReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Call Log Report" );
	setTitle("Current Login Counts");

	list->setColumnText(0, "Date/Time");   list->setColumnAlignment(0, AlignLeft);
	list->addColumn("By");                 list->setColumnAlignment(1, AlignLeft);
	list->addColumn("Type");               list->setColumnAlignment(2, AlignLeft);
	list->addColumn("CustID");             list->setColumnAlignment(3, AlignRight);
	list->addColumn("LoginID");            list->setColumnAlignment(4, AlignLeft);
	list->addColumn("Duration");           list->setColumnAlignment(5, AlignRight);
	list->addColumn("Summary");            list->setColumnAlignment(6, AlignLeft);
	
	// Set the sorting to a hidden column.
    //list->setSorting(6);

    // resize(675, 400);

    // This is a snapshot report.  We don't want to see the dates or filters.
    //allowDates(REP_NODATES);
    allowFilters(0);

    answeredByList = new QComboBox(false, this);
    answeredByList->insertItem("All");
    dl->insertWidget(dl->findWidget(generateButton) - 1, answeredByList, 1);

    logTypeList = new QComboBox(false, this);
    logTypeList->insertItem("All");
    dl->insertWidget(dl->findWidget(generateButton) - 1, logTypeList, 1);

    // Fix our tab ordering.
    QWidget::setTabOrder(dateList, answeredByList);
    QWidget::setTabOrder(answeredByList, logTypeList);
    QWidget::setTabOrder(logTypeList, generateButton);

    // Fill the added by list
    ADB DB;
    DB.query("select LoginID from Staff where Active > 0 order by LoginID");
    if (DB.rowCount) {
        while(DB.getrow()) {
            answeredByList->insertItem(DB.curRow["LoginID"]);
        }
    }
    
    // Fill the log type list
    DB.query("select distinct NoteType from Notes order by NoteType");
    if (DB.rowCount) {
        while(DB.getrow()) {
            logTypeList->insertItem(DB.curRow["NoteType"]);
        }
    }
    
    // Set our default dates and types.
    logTypeList->setCurrentText("Call Log");
    dateList->setCurrentText("Today");

    connect(answeredByList, SIGNAL(activated(int)), this, SLOT(filterActivated(int)));
    connect(logTypeList,    SIGNAL(activated(int)), this, SLOT(filterActivated(int)));
    
    endDateCal->setDate(QDate::currentDate());
    startDateCal->setDate(QDate::currentDate());
	refreshReport();
	
}


CallLogReport::~CallLogReport()
{
}

/*
** filterActivated - Gets called when one of the filter boxes changes.
**                   It just calls refreshReport.
*/

void CallLogReport::filterActivated(int)
{
    refreshReport();
}

/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void CallLogReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    char    startDate[1024];
    char    endDate[1024];
    char    typeStr[1024];
    char    titleStr[1024];
    char    ansby[1024];
    char    notetype[1024];
    QDate   sDate = this->startDateCal->date();
    QDate   eDate = this->endDateCal->date();

    sprintf(startDate, "%04d-%02d-%02d 00:00:00", sDate.year(), sDate.month(), sDate.day());
    sprintf(endDate, "%04d-%02d-%02d 23:59:59", eDate.year(), eDate.month(), eDate.day());


    QApplication::setOverrideCursor(waitCursor);

    // Check to see if we're going by a particular person
    if (strcmp("All", answeredByList->currentText())) {
        sprintf(ansby, "and AddedBy = '%s'", (const char *) answeredByList->currentText());
    } else {
        strcpy(ansby, "");
    }
    
    // Check for note type selection
    if (strcmp("All", logTypeList->currentText())) {
        sprintf(notetype, "and NoteType = '%s'", (const char *) logTypeList->currentText());
    } else {
        strcpy(notetype, "");
    }

    // Clear the list.
    list->clear();
    DB.query("select NoteID, NoteDate, NoteType, AddedBy, Duration, CustomerID, LoginID, NoteText from Notes where NoteDate >= '%s' and NoteDate <= '%s' %s %s", startDate, endDate, ansby, notetype);

    if (DB.rowCount) {
        sprintf(titleStr, "Call Logs for %s - %s, %d entries found", (const char*)sDate.toString(), (const char *)eDate.toString(), DB.rowCount);
        this->setTitle(titleStr);
        while (DB.getrow()) {
            // Pretty up the call duration.
            int dt = atoi(DB.curRow["Duration"]);
            int dh = 0;
            int dm = 0;
            int ds = 0;
            char dstr[1024];

            // Hours
            if (dt >= 3600) {
                dh = dt / 3600;
                dt = dt - (dh * 3600);
            }
            // Minutes
            if (dt > 60) {
                dm = dt / 60;
                dt = dt - (dm * 60);
            }
            // Seconds
            ds = dt;
            // Pretty
            sprintf(dstr, "%d:%02d:%02d", dh, dm, ds);

            QString text(DB.curRow["NoteText"]);
            if (text.length() > 40) {
                text.truncate(37);
                text.append("...");
            }

            (void) new QListViewItem(list, 
                                     DB.curRow["NoteDate"],
                                     DB.curRow["AddedBy"],
                                     DB.curRow["NoteType"],
                                     DB.curRow["CustomerID"],
                                     DB.curRow["LoginID"],
                                     dstr,
                                     text
                                    );
        }
    }

    // Add the totals.
    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected  - Gets called when the user double clicks on an
**                     entry.  Pulls up the LoginTypeReport for the
**                     clicked on login type.
*/

void CallLogReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem) {
        if (atoi(curItem->key(3,0))) {
            emit (openCustomer(atol(curItem->key(3,0))));
        }
    }
}
