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

#include <stdlib.h>

#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <Qt3Support/Q3GridLayout>

#include <ADB.h>
#include <TAATools.h>
#include <BString.h>
#include <Cfg.h>
#include <TAAStructures.h>

#include <GeneralLedgerReport.h>

using namespace Qt;

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
	
	repBody->setColumnText(0, "Account");  repBody->setColumnAlignment(0, Qt::AlignLeft);
	repBody->addColumn("Name");            repBody->setColumnAlignment(1, Qt::AlignLeft);
	repBody->addColumn("Type");            repBody->setColumnAlignment(2, Qt::AlignLeft);
	repBody->addColumn("Amount");          repBody->setColumnAlignment(3, Qt::AlignRight);
	
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

    QApplication::setOverrideCursor(Qt::waitCursor);
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
        new Q3ListViewItem(repBody,
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
void GeneralLedgerReport::listItemSelected(Q3ListViewItem *curItem)
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
    availableColumns.insert("Date",             "TransDate");
    availableColumns.insert("Account Number",   "AccountNo");
    availableColumns.insert("Account Name",     "AcctName");
    availableColumns.insert("Amount",           "Amount");
    availableColumns.insert("Type",             "TransType");
    availableColumns.insert("Bill Date",        "BilledDate");
    availableColumns.insert("Due Date",         "DueDate");
    availableColumns.insert("Cleared",          "Cleared");
    availableColumns.insert("Number",           "Number");
    availableColumns.insert("Quantity",         "Quantity");
    availableColumns.insert("Price",            "Price");
    availableColumns.insert("Memo",             "Memo");
    availableColumns.insert("Customer ID",      "CustomerID");
    availableColumns.insert("Login ID",         "LoginID");
    availableColumns.insert("Statement",        "StatementNo");
    availableColumns.insert("Start Date",       "StartDate");
    availableColumns.insert("End Date",         "End Date");
    availableColumns.insert("Billing Cycle",    "CycleID");
    availableColumns.insert("Rate Plan",        "PlanTag");
    availableColumns.insert("Full Name",        "FullName");
    availableColumns.insert("Contact Name",     "ContactName");
    availableColumns.insert("Contact Name",     "ContactName");
    availableColumns.insert("Billable Name",    "BillableItemID");
    availableColumns.insert("Billable Desc",    "BillablesDesc");
    availableColumns.insert("Package Name",     "PackageTag");
    availableColumns.insert("Package Desc",     "PackageDesc");

    // Note that all of the names listed above have to be listed here as well.
    columnNames.append("Date");
    columnNames.append("Account Number");
    columnNames.append("Account Name");
    columnNames.append("Amount");
    columnNames.append("Type");
    columnNames.append("Bill Date");
    columnNames.append("Due Date");
    columnNames.append("Cleared");
    columnNames.append("Number");
    columnNames.append("Quantity");
    columnNames.append("Price");
    columnNames.append("Memo");
    columnNames.append("Customer ID");
    columnNames.append("Login ID");
    columnNames.append("Statement");
    columnNames.append("Start Date");
    columnNames.append("End Date");
    columnNames.append("Billing Cycle");
    columnNames.append("Rate Plan");
    columnNames.append("Full Name");
    columnNames.append("Contact Name");
    columnNames.append("Contact Name");
    columnNames.append("Billable Name");
    columnNames.append("Billable Desc");
    columnNames.append("Package Name");
    columnNames.append("Package Desc");

    // Pick a few of the available for our starting columns
    // if the user hasn't saved a report.
    QStringList startingCols;
    startingCols += "Date";
    startingCols += "Amount";
    startingCols += "Customer ID";
    startingCols += "Full Name";
    startingCols += "Contact Name";
    startingCols += "Statement";
    startingCols += "Billable Name";
    startingCols += "Package Name";

   
    filters = new GeneralLedgerDetailFilters();
    filters->setAvailableColumns(columnNames);
    filters->setDisplayColumns(startingCols);
    connect(filters, SIGNAL(optionsUpdated()), this, SLOT(refreshReport()));

	repBody->setColumnText(0, "Date");  repBody->setColumnAlignment(0, Qt::AlignLeft);
	
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
    QStringList dispCols = filters->displayColumns();

    QApplication::setOverrideCursor(Qt::waitCursor);
    emit(setStatus("Generating report..."));
    // Set the report columns.  This will also clear the list.
    setReportColumns();

    // Now, for the mother of all queries...
    DB.query("select GL.InternalID, GL.TransID, GL.IntAccountNo, GL.Amount, GL.LinkedTrans, GL.TransType, GL.TransTypeLink, GL.TransDate, GL.BilledDate, GL.DueDate, GL.Cleared, GL.NumberStr, GL.ItemID, GL.Quantity, GL.Price, GL.Memo, Accounts.AccountNo, Accounts.AcctName, AcctsRecv.TransType, AcctsRecv.RefNo, AcctsRecv.CustomerID, AcctsRecv.LoginID, AcctsRecv.StatementNo, AcctsRecv.StartDate, AcctsRecv.EndDate, AcctsRecv.PackageItem, AcctsRecv.Memo, BillingCycles.CycleID, RatePlans.PlanTag, Customers.FullName, Customers.ContactName, Billables.ItemID as BillableItemID, Billables.Description as BillablesDescription, Packages.PackageTag, Packages.Description as PackageDesc from GL left join Accounts on Accounts.IntAccountNo = GL.IntAccountNo left join AcctsRecv on AcctsRecv.GLTransID = GL.TransID left join BillingCycles on BillingCycles.InternalID = AcctsRecv.BillingCycle left join RatePlans on RatePlans.InternalID = AcctsRecv.RatePlan left join Customers on Customers.CustomerID = AcctsRecv.CustomerID left join Billables on Billables.ItemNumber = AcctsRecv.ItemID left join Packages on Packages.InternalID = AcctsRecv.PackageItem where Accounts.IntAccountNo = GL.IntAccountNo and GL.IntAccountNo = %d and GL.TransDate >= '%s' and GL.TransDate <= '%s'",
      myIntAccountNo,
      startDate().toString("yyyy-MM-dd").ascii(), 
      endDate().toString("yyyy-MM-dd").ascii());

    QString colName;
    int     curCol;
    if (DB.rowCount) while (DB.getrow()) {
        curCol = 0;
        Q3ListViewItem   *curItem = new Q3ListViewItem(repBody);

        QStringList::Iterator it;
        for (it = dispCols.begin(); it != dispCols.end(); ++it) {
            colName = *it;
            curItem->setText(curCol, DB.curRow[availableColumns[colName].ascii()]);
            curCol++;
        }
        repBody->insertItem(curItem);
    }
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
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
    QStringList dispCols = filters->displayColumns();

    QString colName;
    uint curCol = 0;
    QStringList::Iterator it;
    for (it = dispCols.begin(); it != dispCols.end(); ++it) {
        colName = *it;
        repBody->addColumn(colName);
        repBody->setColumnAlignment(curCol, Qt::AlignLeft);
        if (colName == "Amount") repBody->setColumnAlignment(curCol, Qt::AlignRight);
        if (colName == "Quantity") repBody->setColumnAlignment(curCol, Qt::AlignRight);
        if (colName == "Price") repBody->setColumnAlignment(curCol, Qt::AlignRight);
        if (colName == "Customer ID") myCustomerIDCol = curCol;
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
void GeneralLedgerDetailReport::listItemSelected(Q3ListViewItem *curItem)
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
GeneralLedgerDetailFilters::GeneralLedgerDetailFilters(QWidget* parent, const char* name) : 
    ReportFilter( parent, name )
{
    setCaption("Filters - General Ledger Detail Report");

    QLabel *columnListLabel = new QLabel(this, "columnListLabel");
    columnListLabel->setText("Display Columns:");
    columnListLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    columnList = new Q3ListBox(this, "columnList");
    columnList->setSelectionMode(Q3ListBox::Multi);

    QPushButton *upButton = new QPushButton(this, "upButton");
    upButton->setText("Move Up");
    connect(upButton, SIGNAL(clicked()), this, SLOT(upClicked()));
    
    QPushButton *dnButton = new QPushButton(this, "dnButton");
    dnButton->setText("Move Down");
    connect(dnButton, SIGNAL(clicked()), this, SLOT(downClicked()));

    // Our main layout is already created for us
    Q3GridLayout *gl = new Q3GridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(columnListLabel,      curRow, 0);
    gl->addWidget(columnList,           curRow, 1);
    gl->setRowStretch(curRow, 1);
    curRow++;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 1);

    // Our button layout is already created for us, so insert
    // our buttons into it, but since we want to do them on the
    // left, insert them in reverse order.
    bl->insertSpacing(0, 20);
    bl->insertWidget(0, dnButton, 0);
    bl->insertWidget(0, upButton, 0);
    
    // Tell the base class we can save.
    setAllowSave(1);
    setReportName("GeneralLedgerDetailReport");
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
void GeneralLedgerDetailFilters::setAvailableColumns(const QStringList columnNames)
{
    //myColumnNames = columnNames;
    columnList->clear();
    columnList->insertStringList(columnNames);
}

/**
 * setDisplayColumns()
 *
 * Sets the selected items in our QListBox.
 */
void GeneralLedgerDetailFilters::setDisplayColumns(QStringList dispCols)
{
    columnList->clearSelection();
    QStringList::Iterator it;
    for (it = dispCols.begin(); it != dispCols.end(); ++it) {
        for (uint i = 0; i < columnList->count(); i++) {
            Q3ListBoxItem *itm = columnList->item(i);
            if (!itm->text().compare(*it)) {
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
QStringList GeneralLedgerDetailFilters::displayColumns()
{
    QStringList  retList;

    for (uint i = 0; i < columnList->count(); i++) {
        if (columnList->isSelected(i)) {
            // This one is selected, add it to our return value.
            Q3ListBoxItem *itm = columnList->item(i);
            retList += itm->text();
        }
    }
    return retList;
}

/**
 * upClicked()
 *
 * Moves the current item in the columnList up by one.
 */
void GeneralLedgerDetailFilters::upClicked()
{
    uint curIdx = columnList->currentItem();
    if (curIdx > 0) {
        Q3ListBoxItem    *curItem = columnList->item(curIdx);
        columnList->takeItem(curItem);
        columnList->insertItem(curItem, curIdx-1);
        columnList->setCurrentItem(curItem);
    }
}

/**
 * downClicked()
 *
 * Moves the current item in the columnList down by one.
 */
void GeneralLedgerDetailFilters::downClicked()
{
    uint curIdx = columnList->currentItem();
    if (curIdx < columnList->count() - 1) {
        Q3ListBoxItem    *curItem = columnList->item(curIdx);
        columnList->takeItem(curItem);
        columnList->insertItem(curItem, curIdx+1);
        columnList->setCurrentItem(curItem);
    }
}

/**
 * saveFilters()
 *
 * Slot that gets called when the user clicks Save to save
 * the report filters.
 */
void GeneralLedgerDetailFilters::saveFilters()
{
    // First, check to see if the report name is valid.
    if (!saveList->currentText().length()) {
        QMessageBox::critical(this, "Invalid Save Name", "You must specify a report name before saving.");
        return;
    }

    ADB         DB;
    ADBTable    srDB("SavedReports");
    long        savedReportID = 0;
    // Check to see if this report name exists and if so if they want
    // to overwrite it.
    DB.query("select SavedReportID from SavedReports where LoginID = '%s' and ReportName = '%s' and SaveName = '%s'", curUser().userName, myReportName.ascii(), saveList->currentText().ascii());
    if (DB.rowCount) {
        DB.getrow();
        savedReportID = atol(DB.curRow["SavedReportID"]);
    }
    if (savedReportID) {
        // Load it from the datbase while we're here.
        srDB.get(savedReportID);
        if (QMessageBox::warning(this, "Saved Report Exists", "A saved report with this name\nalready exists.  Do you want to overwrite it?", "&Yes", "&No", 0, 1) == 1) {
            return;
        }
    }

    // If we made it here, we're good to save.
    // Set the preliminary items.
    srDB.setValue("LoginID",        curUser().userName);
    srDB.setValue("ReportName",     myReportName.ascii());
    srDB.setValue("SaveName",       saveList->currentText().ascii());
    
    // Now, create the report data.
    QString repData;
    repData = "ColumnOrder=";
    // Walk through the list box and add the columns in the order they appear.
    QStringList  tmpList;
    for (uint i = 0; i < columnList->count(); i++) {
        Q3ListBoxItem *itm = columnList->item(i);
        tmpList += itm->text();
    }
    repData += tmpList.join(":");
    repData += ";";
    // Now walk through the list box and add the selected columns.
    repData += "SelectedColumns=";
    tmpList.clear();
    for (uint i = 0; i < columnList->count(); i++) {
        // This one is selected, add it to our return value.
        Q3ListBoxItem *itm = columnList->item(i);
        if (itm->isSelected()) tmpList += itm->text();
    }
    repData += tmpList.join(":");

    // Finally, set the report data and save.
    srDB.setValue("ReportData", repData.ascii());
    if (savedReportID) srDB.upd();
    else srDB.ins();
    emit (setStatus("Report saved", 5000));
}

/**
 * loadFilters()
 *
 * Loads a set of saved filters for this report from the database.
 */
void GeneralLedgerDetailFilters::loadFilters(const QString repName)
{
    //debug(1, "Loading report filter '%s'\n", repName.ascii());
    if (!repName.length()) return;
    ADB     DB;
    
    //debug(1, "Loading report filter '%s' from database...\n", repName.ascii());
    DB.query("select ReportData from SavedReports where LoginID = '%s' and ReportName = '%s' and SaveName = '%s'", curUser().userName, myReportName.ascii(), repName.ascii());
    if (!DB.rowCount) return;
    DB.getrow();

    //debug(1, "Parsing report filter '%s'...\n", repName.ascii());
    QStringMap  settingsMap;
    QStringList repSettings;
    repSettings = repSettings.split(";", DB.curRow["ReportData"]);
    // Now, walk through the settings and put each of the key/value pairs
    // into our settings map for further processing.
    for (QStringList::Iterator it = repSettings.begin(); it != repSettings.end(); ++it) {
        QString tmpStr = *it;
        QStringList kvPairs;
        kvPairs = kvPairs.split("=", tmpStr);
        if (kvPairs.count() == 2) {
            settingsMap[kvPairs[0]] = kvPairs[1];
            //debug(1, "Key = '%s', Value = '%s'\n\n", kvPairs[0].ascii(), kvPairs[1].ascii());
        } else {
            debug(0, "Malformed report setting: '%s'\n", tmpStr.ascii());
        }
    }

    // Now, go through our settings.
    if (settingsMap["ColumnOrder"].length()) {
        //debug(1, "Setting available columns for report filter '%s'\n", repName.ascii());
        QStringList tmpList;
        tmpList = tmpList.split(":", settingsMap["ColumnOrder"]);
        // Now, compare the available columns from our list
        // To do this, we'll walk through both lists adding any
        // in our availableColumns list that don't exist in the
        // column order to it and then replace our availableColumns
        // list with the new list.
        QStringList dispCols = displayColumns();
        QString it1, it2;
        for (QStringList::Iterator dcit = dispCols.begin(); dcit != dispCols.end(); ++dcit) {
            bool    foundIt = false;
            it1 = *dcit;
            for (QStringList::Iterator tlit = tmpList.begin(); tlit != tmpList.end(); ++tlit) {
                it2 = *tlit;
                if (!it1.compare(it2)) foundIt = true;
            }
            if (!foundIt) tmpList.append(it1);
        }
        setAvailableColumns(tmpList);
    }

    if (settingsMap["SelectedColumns"].length()) {
        //debug(1, "Setting selected columns for report filter '%s'\n", repName.ascii());
        QStringList tmpList;
        tmpList = tmpList.split(":", settingsMap["SelectedColumns"]);
        setDisplayColumns(tmpList);
    }

    emit(optionsUpdated());
}


// vim: expandtab
