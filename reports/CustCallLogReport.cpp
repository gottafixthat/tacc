/*
** CustCallLogReport - Gives a summary of the Accounts Receivable for a month.
*/

#include "CustCallLogReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>
#include <qdict.h>
#include <qstrlist.h>

#include <BlargDB.h>
#include <ADB.h>
#include <BString.h>


CustCallLogReport::CustCallLogReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
    myCustID = 0;
    
	setCaption( "Customer Call Log Report" );
	setTitle("Customer Call Log Report");
	
	repBody->setColumnText(0, "Date");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Time");         repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Taken By");     repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Duration");     repBody->setColumnAlignment(3, AlignLeft);
	repBody->addColumn("LoginID");      repBody->setColumnAlignment(4, AlignLeft);
	repBody->addColumn("Category");     repBody->setColumnAlignment(5, AlignLeft);
	repBody->addColumn("Topic");        repBody->setColumnAlignment(6, AlignLeft);

    repBody->setRootIsDecorated(TRUE);
    repBody->setAllColumnsShowFocus(TRUE);

    connect(repBody, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(showCall(QListViewItem *)));
    // connect(list, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(showCall(QListViewItem *)));

    if (parent) {
        userButton->show();
        userButton->setText("&Browse");
    }

	refreshReport();
}


CustCallLogReport::~CustCallLogReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void CustCallLogReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);
    repBody->clear();
    userText->setText("");
    userText->hide();
    ADB             DB;
    QListViewItem   *curItem;
    char            sDate[20];
    char            eDate[20];
    char    *query = new char[4096];
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d-%02d-%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d-%02d-%02d", endDate().year(), endDate().month(), endDate().day());

    if (myCustID) {
        sprintf(query, "select StartDate, StartTime, TakenBy, Duration, LoginID, Category, SubCategory, CallID from CallLog where FollowupOf = 0 and CustomerID = %ld and StartDate >= '%s' and StartDate <= '%s' order by StartDate, StartTime", myCustID, sDate, eDate);
    } else {
        sprintf(query, "select StartDate, StartTime, TakenBy, Duration, LoginID, Category, SubCategory, CallID from CallLog where FollowupOf = 0 and StartDate >= '%s' and StartDate <= '%s' order by StartDate, StartTime", sDate, eDate);
    }
    DB.query(query);
    if (DB.rowCount) while (DB.getrow()) {
        // Okay, we're here....
     
     // Get the elapsed time.
     QTime           tmpTime(0,0,0,0);
     QTime           elapsed(0,0,0,0);
     char            elStr[80];
     elapsed = tmpTime.addSecs(atoi(DB.curRow["Duration"]));
     sprintf(elStr, "%02d:%02d:%02d", elapsed.hour(), elapsed.minute(), elapsed.second());

        curItem = new QListViewItem(repBody, 
            DB.curRow["StartDate"],
            DB.curRow["StartTime"],
            DB.curRow["TakenBy"],
            elStr,                          // Duration
            DB.curRow["LoginID"],
            DB.curRow["Category"],
            DB.curRow["SubCatetory"],
            DB.curRow["CallID"]             // hidden
        );
        
        addFollowups(curItem, atol(DB.curRow["CallID"]));  // The Call ID
    }
    
    delete query;
    
    repBody->setSorting(1,0);
    repBody->setSorting(0,0);
    
    QApplication::restoreOverrideCursor();
}

void CustCallLogReport::addFollowups(QListViewItem *curItem, long CallID)
{
    ADB     DB;
    QListViewItem   *newItem;
    char            *query = new char[4096];
    char            sDate[20];
    char            eDate[20];
    
    // Get the starting and ending dates.
    sprintf(sDate, "%04d-%02d-%02d", startDate().year(), startDate().month(), startDate().day());
    sprintf(eDate, "%04d-%02d-%02d", endDate().year(), endDate().month(), endDate().day());
    
    if (myCustID) {
        sprintf(query, "select StartDate, StartTime, TakenBy, Duration, LoginID, Category, SubCategory, CallID from CallLog where FollowupOf = %ld and CustomerID = %ld and StartDate >= '%s' and StartDate <= '%s' order by StartDate, StartTime", CallID, myCustID, sDate, eDate);
    } else {
        sprintf(query, "select StartDate, StartTime, TakenBy, Duration, LoginID, Category, SubCategory, CallID from CallLog where FollowupOf = %ld and StartDate >= '%s' and StartDate <= '%s' order by StartDate, StartTime", CallID, sDate, eDate);
    }
    
    DB.query(query);
    if (DB.rowCount) while (DB.getrow()) {
        // Okay, we're here....
        
        // Get the elapsed time.
	    QTime           tmpTime(0,0,0,0);
	    QTime           elapsed(0,0,0,0);
	    char            elStr[80];
        elapsed = tmpTime.addSecs(atoi(DB.curRow["Duration"]));
        sprintf(elStr, "%02d:%02d:%02d", elapsed.hour(), elapsed.minute(), elapsed.second());

        newItem = new QListViewItem(curItem, 
            DB.curRow["StartDate"],
            DB.curRow["StartTime"],
            DB.curRow["TakenBy"],
            elStr,                          // Duration
            DB.curRow["LoginID"],
            DB.curRow["Category"],
            DB.curRow["SubCatetory"],
            DB.curRow["CallID"]             // hidden
        );
        curItem->setOpen(TRUE);
        addFollowups(newItem, atol(DB.curRow["CallID"]));  // The Call ID
    }
    
    delete query;
}

/*
** setCustID  - Sets the customer ID for the report.
*/

void CustCallLogReport::setCustID(long CustID)
{
    myCustID = CustID;
    if (myCustID) {
	    char        tmpStr[1024];
	    CustomersDB CDB;
	    if (CDB.get(myCustID)) {
	        sprintf(tmpStr, "Customer Call Log\n%s (Cust ID %ld)", (const char *) CDB.getStr("FullName"), myCustID);
	        setTitle(tmpStr);
	    }
    } else {
        setTitle("Customer Call Log\nAll Customers");
    }
    refreshReport();
}

/*
** showCall - Displays the detail of a logged call.
*/

void CustCallLogReport::showCall(QListViewItem *curItem)
{
    if (curItem) {
        ADBTable    callDB("CallLog");
        callDB.get(atol(curItem->key(7,0)));
        userText->setText(callDB.getStr("Notes"));
        userText->setTextFormat(RichText);
        userText->show();
    } else {
        userText->setText("");
        userText->hide();
    }
}

