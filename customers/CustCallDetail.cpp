/*
** $Id$
**
***************************************************************************
**
** CustCallDetail.cpp  - Shows the detail of a customer call record.
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
** $Log: CustCallDetail.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CustCallDetail.h"

#include <BlargDB.h>
#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>

#define Inherited CustCallDetailData

CustCallDetail::CustCallDetail
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption("Customer Call Browser");
    myCustID    = 0;
    curCallNo   = 0;
    callList    = NULL;
    callCount   = 0;
}


CustCallDetail::~CustCallDetail()
{
}

/*
** setCustID  - Sets the customer ID for the call detail report.
**              This actually loads up the customer records for the calls
**              and allows the user to browse the calls.
*/

void CustCallDetail::setCustID(long custID)
{
    myCustID = custID;
    if (myCustID) {
        CustomersDB CDB;
        CDB.get(myCustID);
        char    tmpStr[2048];
        sprintf(tmpStr, "Customer Call Detail\n%s (%ld)", (const char *)CDB.getStr("FullName"), myCustID);
        titleLabel->setText(tmpStr);
    } else {
        titleLabel->setText("Customer Call Detail\nAll Customers");
    }
    loadCallRecords();
    setCallNumber(1);
}

/*
** setCallNumber - Changes the currently displayed call, by number.
*/

void CustCallDetail::setCallNumber(int callNo)
{
    if (callNo <= callCount && callCount > 0) {
        ADBTable    CLDB("Notes");
        CLDB.get(callList[callNo - 1]);
        
        // Okay, we've got the record.  Fill in the labels
        takenBy->setText(CLDB.getStr("AddedBy"));
        loginID->setText(CLDB.getStr("LoginID"));
        callDate->setText(CLDB.getDate("NoteDate").toString());
        stopDate->setText(CLDB.getDate("StopDate").toString());
        
        QTime   tmpTime(0,0,0,0);
        QTime   elapsed(0,0,0,0);
        char    elStr[80];
        elapsed = tmpTime.addSecs(CLDB.getInt("Duration"));
        sprintf(elStr, "%02d:%02d:%02d", elapsed.hour(), elapsed.minute(), elapsed.second());
        callDuration->setText(elStr);

        category->setText(CLDB.getStr("Category"));
        topic->setText(CLDB.getStr("SubCategory"));
        solution->setText(CLDB.getStr("NoteText"));
        
        
        char    tmpStr[1024];
        sprintf(tmpStr, "%d of %d", callNo, callCount);
        callNumber->setText(tmpStr);
        
        curCallNo = callNo;
    } else {
        takenBy->setText("");
        loginID->setText("");
        callDate->setText("");
        callStartTime->setText("");
        stopDate->setText("");
        stopTime->setText("");
        callDuration->setText("");
        category->setText("");
        topic->setText("");
        solution->setText("No call selected");
        
        
        callNumber->setText("None");
    }
}

/*
** loadCallRecords - Loads all of the calls for the selected customer ID
**                   into memory so we can display them.
*/

void CustCallDetail::loadCallRecords(void)
{
    ADB     DB;
    char    *query = new char[32768];
    
    if (callCount) {
        callCount = 0;
        delete callList;
        callList = NULL;
    }
    
    if (myCustID) {
        sprintf(query, "select CallID from CallLog where CustomerID = %ld", myCustID);
    } else {
        sprintf(query, "select CallID from CallLog order by CallID");
    }
    DB.query("%s", query);
    if (DB.rowCount) {
        callCount = DB.rowCount;
        callList  = new long[callCount+1];
        int         tmpPos = 0;
        while (DB.getrow()) {
            callList[tmpPos++] = atol(DB.curRow["CallID"]);
        }
    }
    delete query;
}

/*
** nextCall - Increments the next call in the list.
*/

void CustCallDetail::nextCall()
{
    if (curCallNo < callCount) setCallNumber(curCallNo + 1);
}

/*
** prevCall - Decrements the call number in the list.
*/

void CustCallDetail::previousCall()
{
    if (curCallNo > 1) setCallNumber(curCallNo - 1);
}


/*
** closeClicked - Closes the window.
*/

void CustCallDetail::closeClicked()
{
    delete this;
}

