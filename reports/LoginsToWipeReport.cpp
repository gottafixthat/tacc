/*
** LoginsToWipeReport - Gives a summary of the Logins that have been inactive
**                      for 90 days or more.
*/

#include "LoginsToWipeReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>

#include <ADB.h>
#include <BString.h>

LoginsToWipeReport::LoginsToWipeReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "List of Expired Accounts" );
	setTitle("List of Expired Accounts\n(Accounts to wipe)");

	list->setColumnText(0, "Customer Name");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Customer ID");           list->setColumnAlignment(1, AlignRight);
	list->addColumn("Login ID");              list->setColumnAlignment(2, AlignLeft);
	list->addColumn("Lock Date");             list->setColumnAlignment(3, AlignLeft);
	list->addColumn("Balance");               list->setColumnAlignment(4, AlignRight);
	
	// Set the sorting to a hidden column.
    list->setSorting(3);
    list->setAllColumnsShowFocus(true);

    resize(675, 400);

    // This is a snapshot report.  We don't want to see the dates or filters.
    allowDates(REP_GENERATE);
    allowFilters(0);

	refreshReport();
	
}


LoginsToWipeReport::~LoginsToWipeReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void LoginsToWipeReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    char    custName[1024];
    char    custBalance[1024];
    QDateTime   qDT;
    char    dStr[1024];

    QApplication::setOverrideCursor(waitCursor);

	// Set the sorting to a hidden column.
    list->setSorting(0);
    
    list->clear();
    QDateTime   tmpTime = QDateTime::currentDateTime().addDays(-90);
    DB.query("select InternalID, CustomerID, LoginID, LastModified from Logins where Wiped = 0 and Active = 0 and LastModified < '%04d%02d%02d%02d%02d%02d'",
      tmpTime.date().year(), tmpTime.date().month(), tmpTime.date().day(), 
      tmpTime.time().hour(), tmpTime.time().minute(), tmpTime.time().second());
    if (DB.rowCount) while (DB.getrow()) {
        DB2.query("select FullName, CurrentBalance from Customers where CustomerID = %ld", atol(DB.curRow[1]));
        if (DB2.rowCount) {
            DB2.getrow();
            strcpy(custName, DB2.curRow["FullName"]);
            sprintf(custBalance, "%8.2f", atof(DB2.curRow["CurrentBalance"]));
        } else {
            strcpy(custName, "Undefined!");
            sprintf(custBalance, "%8.2f", 0.0);
        }

        sprintf(dStr, "%04d-%02d-%02d %02d:%02d", 
                qDT.date().year(),
                qDT.date().month(),
                qDT.date().day(),
                qDT.time().hour(),
                qDT.time().minute()
        );
        
        myDateStamptoQDateTime(DB.curRow["LastModified"], &qDT);
        (void) new QListViewItem(list,
          custName,
          DB.curRow["CustomerID"],
          DB.curRow["LoginID"],
          dStr,
          custBalance
        );

    }
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected   - Gets called when the user double-clicks on a report
**                      item.  It will bring up the customer in question.
*/

void LoginsToWipeReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        long    CustID = atol(curItem->key(1,0));
        emit(openCustomer(CustID));
    }
}

/*
** refreshCustomer - A slot that gets called automatically when a customer
**                   gets changed.  We will call refreshReport to make sure
**                   we have a fresh list.
*/

void LoginsToWipeReport::refreshCustomer(long)
{
    refreshReport();
}

