/*
** $Id$
**
***************************************************************************
**
** StatementView - Views a statement.
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
** $Log: StatementView.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "StatementView.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include "RunStatements.h"
#include <ADB.h>
#include "TAATools.h"
#include <qapplication.h>

#define Inherited StatementViewData

StatementView::StatementView
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{

	setCaption( "Statement View" );
	
	list->addColumn("Internal ID");
	list->addColumn("Trans Date");
	list->addColumn("Login ID");
	list->addColumn("Start Date");
	list->addColumn("End Date");
	list->addColumn("Quantity");
	list->addColumn("Price");
	list->addColumn("Amount");
	list->addColumn("Description");

	list->setAllColumnsShowFocus(TRUE);

    if (!isManager()) {
        printButton->hide();
    }

}


StatementView::~StatementView()
{
}


/*
** loadStatement - Loads and displays the specified statement.
*/

void StatementView::loadStatement(long stNo)
{
    if (!stNo) {
        close();
    } else {
        QApplication::setOverrideCursor(waitCursor);
        StatementsDB        STDB;
        StatementsDataDB    STDDB;
        ADB                 DB;
        char                tmpStr[1024];
        QListViewItem       *curItem;
        
        STDB.get(stNo);
        custName->setText(STDB.getStr("CustName"));
        custAddr1->setText(STDB.getStr("CustAddr1"));
        custAddr2->setText(STDB.getStr("CustAddr2"));
        custAddr3->setText(STDB.getStr("CustAddr3"));
        custAddr4->setText(STDB.getStr("CustAddr4"));
        
        sprintf(tmpStr, "%09ld", stNo);
        statementNo->setText(tmpStr);
        statementDate->setText(STDB.getStr("StatementDate"));
        chargesThrough->setText(STDB.getStr("ChargesThrough"));
        dueDate->setText(STDB.getStr("DueDate"));
        terms->setText(STDB.getStr("Terms"));
        
        prevBalance->setText(STDB.getStr("PrevBalance"));
        credits->setText(STDB.getStr("Credits"));
        newCharges->setText(STDB.getStr("NewCharges"));
        financeCharge->setText(STDB.getStr("FinanceCharge"));
        totalDue->setText(STDB.getStr("TotalDue"));
        
        sprintf(tmpStr, "%s on %s", (const char *) STDB.getStr("EmailedTo"), (const char *) STDB.getStr("EmailedOn"));
        emailedToLabel->setText(tmpStr);
        
        if (!strcmp("0000-00-00", (const char *) STDB.getStr("PrintedOn"))) {
            printedOnLabel->setText("Not printed");
        } else {
            printedOnLabel->setText(STDB.getStr("PrintedOn"));
        }

        // Load the table with the line items.
        DB.query("select InternalID from StatementsData where StatementNo = %ld", stNo);
        while (DB.getrow()) {
            STDDB.get(atol(DB.curRow["InternalID"]));
            
            curItem = new QListViewItem(list,
              STDDB.getStr("InternalID"),
              STDDB.getStr("TransDate"),
              STDDB.getStr("LoginID"),
              STDDB.getStr("StartDate"),
              STDDB.getStr("EndDate"),
              STDDB.getStr("Quantity"),
              STDDB.getStr("Price"),
              STDDB.getStr("Amount")
              // STDDB.getStr("Description")
            );
            curItem->setText(8, STDDB.getStr("Description"));
        }
        
        myStNo = stNo;
        QApplication::restoreOverrideCursor();
    }
}

/*
** closeView - Closes the statement viewer window.
*/

void StatementView::closeView()
{
    close();
}


/*
** reEmailStatement - Sends a duplicate statement to the user.
*/

void StatementView::reEmailStatement()
{
    QApplication::setOverrideCursor(waitCursor);
    emailStatement(myStNo, 1, 1);
    QApplication::restoreOverrideCursor();
}

/*
** rePrintStatement - Prints a duplicate statement to the user.
*/

void StatementView::rePrintStatement()
{
    QApplication::setOverrideCursor(waitCursor);
    printStatement(myStNo, 1, 1);
    QApplication::restoreOverrideCursor();
}

