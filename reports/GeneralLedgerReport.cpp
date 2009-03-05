/**
 * GeneralLedgerREport.cpp - General Ledger Report
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>
#include <qmap.h>

#include <GeneralLedgerReport.h>
#include <ADB.h>
#include <TAATools.h>
#include <BString.h>
#include <Cfg.h>
#include <TAAStructures.h>

/**
 * GeneralLedgerReport()
 *
 * Constructor.
 */
GeneralLedgerReport::GeneralLedgerReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption("General Ledger");
	setTitle("General Ledger");
	
	repBody->setColumnText(0, "Account");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Name");            repBody->setColumnAlignment(1, AlignLeft);
	repBody->addColumn("Type");            repBody->setColumnAlignment(2, AlignLeft);
	repBody->addColumn("Amount");          repBody->setColumnAlignment(3, AlignRight);
	
    // By default use the current month.
    setDateRange(d_thisMonth);
    
    allowDates(REP_ALLDATES);
    allowFilters(0);
	refreshReport();
}


/**
 * ~GeneralLedgerReport()
 * 
 * Destructor.
 */
GeneralLedgerReport::~GeneralLedgerReport()
{
}

/**
 * refreshReport()
 *
 * A virtual function that gets called when the user changes
 * one of the dates or otherwise needs to refresh the report.
 */
void GeneralLedgerReport::refreshReport()
{
    // Create a structure that we can create a hash into
    typedef QMap<int, glAccountRecord> acctMap;

    acctMap     map;

    ADB         DB;
    QString     amount;
    int         intAcct;
    QString     query;

    QApplication::setOverrideCursor(waitCursor);
    repBody->clear();

    // Load the list of accounts into the map
    DB.query("select Accounts.IntAccountNo, Accounts.AccountNo, Accounts.AcctName, Accounts.AccountType, GLAccountTypes.Description from Accounts, GLAccountTypes where Accounts.AccountType = GLAccountTypes.AccountType");
    while (DB.getrow()) {
        glAccountRecord rec;
        rec.intAccountNo    = atoi(DB.curRow["IntAccountNo"]);
        rec.accountNo       = DB.curRow["AccountNo"];
        rec.acctName        = DB.curRow["AcctName"];
        rec.accountType     = atoi(DB.curRow["AccountType"]);
        rec.accountTypeName = DB.curRow["Description"];
        rec.balance         = 0.00;
        map[rec.intAccountNo] = rec;
    }

    // Format our dates
    QString     sDate = startDate().toString("yyyy-MM-dd");
    QString     eDate = endDate().toString("yyyy-MM-dd");

    // Create our query.
    DB.query("select * from GL where TransDate >= '%s' and GL.TransDate <= '%s'", sDate.ascii(), eDate.ascii());

    if (DB.rowCount) {
        while(DB.getrow()) {
            intAcct = atoi(DB.curRow["IntAccountNo"]);
            amount = DB.curRow["Amount"];

            map[intAcct].balance += amount.toDouble();

            /*
            total += amount.toDouble();
            amount = amount.sprintf("%.2f", amount.toDouble());

            customerName = "";
            companyName  = "";
            // Check for a company name.
            if (strlen(DB.curRow["ContactName"])) {
                customerName = DB.curRow["ContactName"];
                companyName  = DB.curRow["FullName"];
            } else {
                customerName = DB.curRow["FullName"];
            }

            (void) new QListViewItem(repBody, 
                    DB.curRow["TransDate"],
                    amount,
                    DB.curRow["CustomerID"],
                    customerName,
                    companyName,
                    DB.curRow["Memo"]);
                */
        }
    }

    // Walk through the map and add them to the list.
    acctMap::Iterator it;
    for ( it = map.begin(); it != map.end(); ++it ) {
        QString intAcctNo;
        QString balance;
        intAcctNo.setNum(it.data().intAccountNo);
        balance.setNum(it.data().balance);
        balance.sprintf("%.2f", balance.toDouble());
        new QListViewItem(repBody,
                it.data().accountNo,
                it.data().acctName,
                it.data().accountTypeName,
                balance,
                intAcctNo);
    }

    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report, opening a ccPaymentDetails report for the specified
 * day.
 */
void GeneralLedgerReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        // If they double click, open the customer's window.
        QString custID = curItem->key(2,0);
        if (custID.toInt()) {
            emit(openCustomer(custID.toInt()));
        }
    }
}


