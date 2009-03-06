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
        QString intAcctNo = curItem->key(4,0);
        if (intAcctNo.toInt()) {
            GeneralLedgerDetailReport   *detRep = new GeneralLedgerDetailReport();
            detRep->setStartDate(startDate());
            detRep->setEndDate(endDate());
            detRep->setIntAccountNo(intAcctNo.toInt());
            detRep->show();
        }
    }
}

/**
 * GeneralLedgerDetailReport()
 *
 * Constructor.
 */
GeneralLedgerDetailReport::GeneralLedgerDetailReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption("General Ledger Detail");
	setTitle("General Ledger Detail");

    // Set all of the available columns we have for the report.
    availableColumns.insert("TransDate",        "Date");
    availableColumns.insert("AccountNo",        "Account Number");
    availableColumns.insert("AcctName",         "Account Name");
    availableColumns.insert("Amount",           "Amount");
    availableColumns.insert("TransType",        "Type");
    availableColumns.insert("BilledDate",       "Bill Date");
    availableColumns.insert("DueDate",          "Due Date");
    availableColumns.insert("Cleared",          "Cleared");
    availableColumns.insert("Number",           "Number");
    availableColumns.insert("Quantity",         "Quantity");
    availableColumns.insert("Price",            "Price");
    availableColumns.insert("Memo",             "Memo");
    availableColumns.insert("CustomerID",       "Customer ID");
    availableColumns.insert("LoginID",          "Login ID");
    availableColumns.insert("StatementNo",      "Statement");
    availableColumns.insert("StartDate",        "StartDate");
    availableColumns.insert("EndDate",          "EndDate");
    availableColumns.insert("CycleID",          "Billing Cycle");
    availableColumns.insert("PlanTag",          "Rate Plan");
    availableColumns.insert("FullName",         "Full Name");
    availableColumns.insert("ContactName",      "Contact Name");
    availableColumns.insert("ContactName",      "Contact Name");
    availableColumns.insert("BillableItemID",   "Billable Name");
    availableColumns.insert("BillablesDesc",    "Billable Desc");
    availableColumns.insert("PackageTag",       "Package Name");
    availableColumns.insert("PackageDesc",      "Package Desc");

    QStringMap  tmpMap;
    tmpMap.insert("TransDate",  "Date");
    tmpMap.insert("PackageTag", "Package Name");
    
   
    filters = new GeneralLedgerDetailFilters();
    filters->setAvailableColumns(availableColumns);
    filters->setDisplayColumns(availableColumns);
    connect(filters, SIGNAL(optionsUpdated()), this, SLOT(refreshReport()));

	repBody->setColumnText(0, "Date");  repBody->setColumnAlignment(0, AlignLeft);
	
    // By default use the current month.
    setDateRange(d_thisMonth);
    
    allowDates(REP_ALLDATES);
    allowFilters(1);
}

/**
 * ~GeneralLEdgerDetailReport()
 *
 * Destructor.
 */
GeneralLedgerDetailReport::~GeneralLedgerDetailReport()
{
    delete filters;
}

/**
 * refreshReport()
 *
 * Refreshes the report.
 */
void GeneralLedgerDetailReport::refreshReport()
{
    ADB         DB;
    QStringMap  dispCols = filters->displayColumns();

    // Set the report columns.  This will also clear the list.
    setReportColumns();

    // Now, for the mother of all queries...
    DB.query("select GL.InternalID, GL.TransID, GL.IntAccountNo, GL.Amount, GL.LinkedTrans, GL.TransType, GL.TransTypeLink, GL.TransDate, GL.BilledDate, GL.DueDate, GL.Cleared, GL.NumberStr, GL.ItemID, GL.Quantity, GL.Price, GL.Memo, Accounts.AccountNo, Accounts.AcctName, AcctsRecv.TransType, AcctsRecv.RefNo, AcctsRecv.CustomerID, AcctsRecv.LoginID, AcctsRecv.StatementNo, AcctsRecv.StartDate, AcctsRecv.EndDate, AcctsRecv.PackageItem, AcctsRecv.Memo, BillingCycles.CycleID, RatePlans.PlanTag, Customers.FullName, Customers.ContactName, Billables.ItemID as BillableItemID, Billables.Description as BillablesDescription, Packages.PackageTag, Packages.Description as PackageDesc from GL left join Accounts on Accounts.IntAccountNo = GL.IntAccountNo left join AcctsRecv on AcctsRecv.GLTransID = GL.TransID left join BillingCycles on BillingCycles.InternalID = AcctsRecv.BillingCycle left join RatePlans on RatePlans.InternalID = AcctsRecv.RatePlan left join Customers on Customers.CustomerID = AcctsRecv.CustomerID left join Billables on Billables.ItemNumber = AcctsRecv.ItemID left join Packages on Packages.InternalID = AcctsRecv.PackageItem where Accounts.IntAccountNo = GL.IntAccountNo and GL.IntAccountNo = %d and GL.TransDate >= '%s' and GL.TransDate <= '%s'",
      myIntAccountNo,
      startDate().toString("yyyy-MM-dd").ascii(), 
      endDate().toString("yyyy-MM-dd").ascii());

    int curCol;
    if (DB.rowCount) while (DB.getrow()) {
        curCol = 0;
        QListViewItem   *curItem = new QListViewItem(repBody);

        QStringMap::Iterator it;
        for (it = dispCols.begin(); it != dispCols.end(); ++it) {
            curItem->setText(curCol, DB.curRow[it.key().ascii()]);
            curCol++;
        }
        repBody->insertItem(curItem);
    }
}

/**
 * setReportColumns()
 *
 * Sets the column headers for the report.
 */
void GeneralLedgerDetailReport::setReportColumns()
{
    repBody->clear();
    while(repBody->columns()) repBody->removeColumn(0);

    myCustomerIDCol = -1;

    // Load the columns from the filters.
    QStringMap dispCols = filters->displayColumns();

    QStringMap::Iterator it;
    uint curCol = 0;
    for (it = dispCols.begin(); it != dispCols.end(); ++it) {
        repBody->addColumn(it.data());
        repBody->setColumnAlignment(curCol, Qt::AlignLeft);
        if (it.key() == "Amount") repBody->setColumnAlignment(curCol, AlignRight);
        if (it.key() == "Quantity") repBody->setColumnAlignment(curCol, AlignRight);
        if (it.key() == "Price") repBody->setColumnAlignment(curCol, AlignRight);
        if (it.key() == "CustomerID") myCustomerIDCol = curCol;
        curCol++;
    }
}

/**
 * setIntAccountNo()
 *
 * Sets the account we're going to pull a report on.
 */
void GeneralLedgerDetailReport::setIntAccountNo(int intAccountNo)
{
    ADB     DB;
    myIntAccountNo = intAccountNo;

    DB.query("select * from Accounts where IntAccountNo = %d", myIntAccountNo);
    if (DB.rowCount) {
        DB.getrow();
        QString tmpStr = "General Ledger Detail\n";
        tmpStr += DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        setTitle(tmpStr);
    } else {
	    setTitle("General Ledger Detail");
    }

    refreshReport();
}

/**
 * listItemSelected()
 *
 * Gets called when the user double clicks on a list item.
 */
void GeneralLedgerDetailReport::listItemSelected(QListViewItem *curItem)
{
    if (!curItem) return;
    if (myCustomerIDCol < 0) return;
    QString custID = curItem->key(myCustomerIDCol, 0);
    if (custID.toInt()) emit(openCustomer(custID.toInt()));
}

/**
 * editFilters()
 *
 * Called when the user clicks the "Filters" button.
 */
void GeneralLedgerDetailReport::editFilters()
{
    filters->show();
}


/**
 * GeneralLedgerDetailFilters()
 *
 * Constructor.
 */
GeneralLedgerDetailFilters::GeneralLedgerDetailFilters
( QWidget* parent, const char* name) : TAAWidget( parent, name )
{
    setCaption("Filters - General Ledger Detail Report");

    QLabel *columnListLabel = new QLabel(this, "columnListLabel");
    columnListLabel->setText("Display Columns:");
    columnListLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    columnList = new QListBox(this, "columnList");
    columnList->setSelectionMode(QListBox::Multi);

    QPushButton *updateButton = new QPushButton(this, "updateButton");
    updateButton->setText("&Update");
    connect(updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));

    QPushButton *closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(columnListLabel,      curRow, 0);
    gl->addWidget(columnList,           curRow, 1);
    gl->setRowStretch(curRow, 1);
    curRow++;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 1);

    // Buttons
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(updateButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);
    
}

/**
 * ~GeneralLedgerDetailFilters()
 *
 * Destructor.
 */
GeneralLedgerDetailFilters::~GeneralLedgerDetailFilters()
{
}

/**
 * setAvailableColumns()
 *
 * Defines the list of columns that are available to display
 * to the user.
 */
void GeneralLedgerDetailFilters::setAvailableColumns(QStringMap &columnNames)
{
    myColumnNames = columnNames;
    columnList->clear();

    QStringMap::Iterator it;
    for (it = columnNames.begin(); it != columnNames.end(); ++it) {
        columnList->insertItem(it.data());
    }
}

/**
 * setDisplayColumns()
 *
 * Sets the selected items in our QListBox.
 */
void GeneralLedgerDetailFilters::setDisplayColumns(QStringMap dispCols)
{
    columnList->clearSelection();
    QStringMap::Iterator it;
    for (it = dispCols.begin(); it != dispCols.end(); ++it) {
        // Walk through the list now, and if we find an item with a matching
        // description, set it to be selected.
        for (uint i = 0; i < columnList->count(); i++) {
            QListBoxItem *itm = columnList->item(i);
            if (!itm->text().compare(it.data())) {
                columnList->setSelected(i, true);
            }
        }
    }
}

/**
 * displayColumns()
 *
 * Returns the items selected in the QListBox.
 */
const QStringMap GeneralLedgerDetailFilters::displayColumns()
{
    QStringMap  retMap;

    for (uint i = 0; i < columnList->count(); i++) {
        if (columnList->isSelected(i)) {
            // This one is selected, add it to our return value.
            QListBoxItem *itm = columnList->item(i);
            QString keyStr = itm->text();
            QStringMap::Iterator it;
            for (it = myColumnNames.begin(); it != myColumnNames.end(); ++it) {
                if (it.data() == itm->text()) {
                    // This item matches, add it to our return map
                    retMap.insert(it.key(), it.data());
                }
            }
        }
    }
    return retMap;
}

/**
 * updateClicked()
 *
 * The user clicked "Update" on the filter list.
 */
void GeneralLedgerDetailFilters::updateClicked()
{
    emit(optionsUpdated());
    hide();
}

/**
 * closeClicked()
 *
 * The user clicked the "Close" button.
 */
void GeneralLedgerDetailFilters::closeClicked()
{
    hide();
}


