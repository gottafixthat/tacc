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

#include <QtCore/QDateTime>
#include <QtGui/QApplication>

#include <ADB.h>
#include <Report.h>
#include "LoginTypeReport.h"

using namespace Qt;

/*
** LoginTypeReport()  - The constructor.  Hides the appropriate buttons
**                      and sets up the titles, etc.
*/

LoginTypeReport::LoginTypeReport(QWidget *parent, const char *name)
    : Report(parent, name)
{
    setCaption("Login Type Report");
    setTitle("Login Type Report");

    allowDates(REP_NODATES);
    allowFilters(0);
    
    repBody->setColumnText(0, "Login ID");
    repBody->addColumn("Customer Name");
    repBody->addColumn("Last Modified");
    repBody->addColumn("Active");
    repBody->addColumn("Primary");
    repBody->addColumn("CustomerID");
    
    // This needs to be updated if the list layout ever changes.
    custIDColumn = 5;

    loginType = 0;
    showActive      = true;
    showInactive    = false;
    showWiped       = false;
}

LoginTypeReport::~LoginTypeReport()
{
}


/*
** refreshReport - Called automagically when the report needs to be
**                 refreshed.
*/

void LoginTypeReport::refreshReport()
{
    emit(setStatus("Generating report..."));
    QApplication::setOverrideCursor(waitCursor);

    ADB         DB;
    char        modDate[128];
    char        isActive[128];
    char        isPrimary[128];
    QDateTime   tmpQDT;
    bool        showIt = false;

    repBody->clear();

    DB.query("select Logins.LoginID, Customers.FullName, Logins.LastModified, Logins.Active, Logins.CustomerID, Customers.PrimaryLogin from Logins, Customers where Logins.LoginType = %d and Customers.CustomerID = Logins.CustomerID", loginType);

    if (DB.rowCount) while (DB.getrow()) {
        if (!strcmp(DB.curRow["LoginID"], DB.curRow["PrimaryLogin"])) strcpy(isPrimary, "Yes");
        else strcpy(isPrimary, "No");
        if (atoi(DB.curRow["Active"])) strcpy(isActive, "Yes");
        else strcpy(isActive, "No");

        tmpQDT = DB.curRow.col("LastModified")->toQDateTime();
        sprintf(modDate, "%04d-%02d-%02d", tmpQDT.date().year(), tmpQDT.date().month(), tmpQDT.date().day());

        showIt = false;
        if ((atoi(DB.curRow["Active"])) && showActive) showIt = true;
        if ((!atoi(DB.curRow["Active"])) && showInactive) showIt = true;

        if (showIt) {
            (void) new Q3ListViewItem(repBody,
                                     DB.curRow["LoginID"],
                                     DB.curRow["FullName"],
                                     modDate,
                                     isActive,
                                     isPrimary,
                                     DB.curRow["CustomerID"]
                                    );
        };
    }

    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
}


/*
** setLoginType  - Sets the login type we are looking at and refreshes
**                 the report data.
*/

void LoginTypeReport::setLoginType(int newLoginType)
{
    // Get the login type from the database and set the title.
    ADBTable    ltDB("LoginTypes");
    ltDB.get(newLoginType);
    setTitle(ltDB.getStr("Description"));
    loginType = newLoginType;
    refreshReport();
}

/*
** listItemSelected  - Gets called when a user double clicks a list item.
*/

void LoginTypeReport::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem) {
        emit(openCustomer(atol(curItem->key(custIDColumn,0))));
    }
}




// vim: expandtab
