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

#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <ADB.h>
#include <BlargDB.h>
#include <Cfg.h>
#include <GenLedger.h>
#include <QListViewPrint.h>
#include <TAATools.h>

#include "MakeDeposits.h"

using namespace Qt;

MakeDeposits::MakeDeposits
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
	setCaption( "Make Deposits" );

    // Create our widgets.
    paymentList = new Q3ListView(this, "paymentList");
    int curCol = 1;
    paymentList->addColumn("Date");             curCol++;
    paymentList->addColumn("Cust ID");          curCol++;
    paymentList->addColumn("Customer Name");    curCol++;
    paymentList->addColumn("Check No");         curCol++;
    paymentList->addColumn("Amount");           curCol++;
    paymentList->addColumn("IntID");
    paymentList->setColumnAlignment(curCol++, AlignRight);
    paymentList->setAllColumnsShowFocus(true);
    paymentList->setMultiSelection(true);
    connect(paymentList, SIGNAL(selectionChanged()), this, SLOT(itemSelected()));
    connect(paymentList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));

    QLabel  *selectedItemCountLabel = new QLabel(this, "selectedItemCountLabel");
    selectedItemCountLabel->setText("Items Selected:");
    selectedItemCountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    selectedItemCount = new QLabel(this, "selectedItemCount");
    selectedItemCount->setText("0");
    selectedItemCount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel  *undepositedAmountLabel = new QLabel(this, "undepositedAmountLabel");
    undepositedAmountLabel->setText("Undeposted amount:");
    undepositedAmountLabel->setAlignment(AlignRight | AlignVCenter);

    undepositedAmount = new QLabel(this, "undepositedAmount");
    undepositedAmount->setText("$0.00");
    undepositedAmount->setAlignment(AlignRight | AlignVCenter);

    QLabel  *amountSelectedLabel = new QLabel(this, "amountSelectedLabel");
    amountSelectedLabel->setText("Amount selected:");
    amountSelectedLabel->setAlignment(AlignRight | AlignVCenter);

    amountSelected = new QLabel(this, "amountSelected");
    amountSelected->setText("$0.00");
    amountSelected->setAlignment(AlignRight | AlignVCenter);

    QLabel  *targetAccountLabel = new QLabel(this, "targetAccountLabel");
    targetAccountLabel->setText("Target Account:");
    targetAccountLabel->setAlignment(AlignRight | AlignVCenter);

    targetAccountList = new QComboBox(false, this, "targetAccountList");

    QLabel  *transDateLabel = new QLabel(this, "transDateLabel");
    transDateLabel->setText("Desposit Date:");
    transDateLabel->setAlignment(AlignRight | AlignVCenter);

    transDate = new Q3DateEdit(QDate::currentDate(), this, "transDate");

    // Our buttons now.
    QPushButton *printButton = new QPushButton(this, "printButton");
    printButton->setText("&Print");
    connect(printButton, SIGNAL(clicked()), this, SLOT(printSelected()));
    
    QPushButton *selectAllButton = new QPushButton(this, "selectAllButton");
    selectAllButton->setText("Select &All");
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));

    QPushButton *selectNoneButton = new QPushButton(this, "selectNoneButton");
    selectNoneButton->setText("Select &None");
    connect(selectNoneButton, SIGNAL(clicked()), this, SLOT(selectNone()));

    QPushButton *continueButton = new QPushButton(this, "continueButton");
    continueButton->setText("C&ontinue");
    connect(continueButton, SIGNAL(clicked()), this, SLOT(processSelections()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelSelected()));

    // Create our layout.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    ml->addWidget(paymentList, 1);

    // A grid for our labels and account selection.
    Q3GridLayout *asl = new Q3GridLayout(3, 2);
    asl->setColStretch(0, 0);
    asl->setColStretch(1, 1);
    int curRow = 0;

    asl->addWidget(selectedItemCountLabel,          curRow, 0);
    asl->addWidget(selectedItemCount,               curRow, 1);
    asl->setRowStretch(curRow++, 0);
    asl->addWidget(undepositedAmountLabel,          curRow, 0);
    asl->addWidget(undepositedAmount,               curRow, 1);
    asl->setRowStretch(curRow++, 0);
    asl->addWidget(amountSelectedLabel,             curRow, 0);
    asl->addWidget(amountSelected,                  curRow, 1);
    asl->setRowStretch(curRow++, 0);
    asl->addWidget(targetAccountLabel,              curRow, 0);
    asl->addWidget(targetAccountList,               curRow, 1);
    asl->setRowStretch(curRow++, 0);
    asl->addWidget(transDateLabel,                  curRow, 0);
    asl->addWidget(transDate,                       curRow, 1);
    asl->setRowStretch(curRow++, 0);

    // Now our button layout
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(printButton,      0);
    bl->addWidget(selectAllButton,  0);
    bl->addWidget(selectNoneButton, 0);
    bl->addWidget(continueButton,   0);
    bl->addWidget(cancelButton,     0);

    // Add the buttons to our other widget layout
    //owl->addLayout(bl, 0);

    // Add the other widget layout to our main layout.
    ml->addLayout(asl, 0);
    ml->addWidget(new HorizLine(this), 0);
    ml->addLayout(bl, 0);

    // Done with the layout.

    // Fill our account list.
    ADB     DB;
    int     tmpPos = 0;
    DB.query("select IntAccountNo, AccountNo, AcctName from Accounts order by AccountNo");
    accountIDX = new long[DB.rowCount+2];
    accountIDX[0] = 0;
    QString tmpStr;
    if (DB.rowCount) while (DB.getrow()) {
        accountIDX[tmpPos++] = atol(DB.curRow["IntAccountNo"]);
        tmpStr = DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        targetAccountList->insertItem(tmpStr);
    }
    
    selTotal = 0.00;
    fillDepositList();
}


MakeDeposits::~MakeDeposits()
{
}


/*
** fillDepositList  - Does just that.  Fills the list of undeposited items.
*/

void MakeDeposits::fillDepositList()
{
    QApplication::setOverrideCursor(waitCursor);
    ADB     DB;
    char    tmpStr[1024];
    float   totalFunds = 0.00;
    
    strcpy(tmpStr, cfgVal("UndepositedFundsAccount"));
    
    emit(setStatus("Creating undeposited funds list..."));
    DB.query("select GL.InternalID, GL.TransDate, GL.Amount, AcctsRecv.RefNo, AcctsRecv.CustomerID, Customers.FullName from GL, AcctsRecv, Customers where GL.IntAccountNo = %d and GL.TransType = 2 and AcctsRecv.InternalID = GL.TransTypeLink and AcctsRecv.RefNo > 0 and Customers.CustomerID = AcctsRecv.CustomerID and GL.Cleared = 0", atoi(cfgVal("UndepositedFundsAcct")));
    //DB.query("select * from GL where AccountNo = %d and LinkedTrans = 0", atoi(tmpStr));
    
    if (DB.rowCount) while (DB.getrow()) {
        (void) new Q3ListViewItem(paymentList, 
          DB.curRow["TransDate"],
          DB.curRow["CustomerID"], 
          DB.curRow["FullName"],
          DB.curRow["RefNo"],
          DB.curRow["Amount"],
          DB.curRow["InternalID"]
        );
        totalFunds += atof(DB.curRow["Amount"]);
    }

    sprintf(tmpStr, "$%.2f", totalFunds);
    undepositedAmount->setText(tmpStr);
    
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
    
    
}

/*
** processSelections - Does the GL entry and deposits all of the selected
**                     payments.
*/

void MakeDeposits::processSelections()
{
    if (selTotal <= 0.00) return;

    GenLedger   *GL = new GenLedger();
    int         srcAcct, dstAcct = 0;
    ADB         DB;
    QDate       selDate = transDate->date();
    char        tDate[1024];

    strcpy(tDate, selDate.toString("yyyy-MM-dd").ascii());

    // Setup the base GL transaction
    GL->BaseDesc = "Deposit";

    // Get the source and destination accounts.
    // The source will be our Undeposited Funds account.
    srcAcct = atol(cfgVal("UndepositedFundsAcct"));
    dstAcct = accountIDX[targetAccountList->currentItem()];

    // Now, setup the transaction.
    // Start by walking through each of the undeposited items and
    // then adding a split item for it.
    Q3ListViewItem   *curItem;
    //int nameCol  = 2;
    //int numCol   = 3;
    int amtCol   = 4;
    int intIDCol = 5;
    char tmpStr[1024];
    selTotal = 0.00;
    curItem = paymentList->firstChild();
    while (curItem != NULL) {
        if (curItem->isSelected()) {
            selTotal += atof(curItem->key(amtCol,0));
        }
        curItem = curItem->itemBelow();
    }
    // Add the first half of our split
    GL->AddSplit();
    GL->CurrentSplit->IntAccountNo.setNum(srcAcct);
    GL->CurrentSplit->Amount.setNum(selTotal * -1.0);
    GL->CurrentSplit->TransType.setNum(TRANSTYPE_DEPOSIT);
    GL->CurrentSplit->TransTypeLink.setNum(0);
    GL->CurrentSplit->TransDate = selDate.toString("yyyy-MM-dd");
    GL->CurrentSplit->BilledDate = selDate.toString("yyyy-MM-dd");
    GL->CurrentSplit->DueDate = selDate.toString("yyyy-MM-dd");
    GL->CurrentSplit->Cleared.setNum(0);
    GL->CurrentSplit->Number.setNum(0);
    GL->CurrentSplit->NumberStr = "";
    GL->CurrentSplit->ItemID.setNum(0);
    GL->CurrentSplit->Quantity.setNum(1);
    GL->CurrentSplit->Price.setNum(selTotal);
    sprintf(tmpStr, "Deposit of $%.2f on %s", selTotal, selDate.toString("yyyy-MM-dd").ascii());
    GL->CurrentSplit->Memo = tmpStr;

    // Now, setup the other "half" of our transaction.
    GL->AddSplit();
    GL->CurrentSplit->IntAccountNo.setNum(dstAcct);
    GL->CurrentSplit->Amount.setNum(selTotal);
    GL->CurrentSplit->TransType.setNum(TRANSTYPE_DEPOSIT);
    GL->CurrentSplit->TransTypeLink.setNum(0);
    GL->CurrentSplit->TransDate = selDate.toString("yyyy-MM-dd").ascii();
    GL->CurrentSplit->BilledDate = selDate.toString("yyyy-MM-dd").ascii();
    GL->CurrentSplit->DueDate = selDate.toString("yyyy-MM-dd").ascii();
    GL->CurrentSplit->Cleared.setNum(0);
    GL->CurrentSplit->Number.setNum(0);
    GL->CurrentSplit->NumberStr = "0";
    GL->CurrentSplit->ItemID.setNum(0);
    GL->CurrentSplit->Quantity.setNum(1);
    GL->CurrentSplit->Price.setNum(selTotal);
    sprintf(tmpStr, "Deposit of $%.2f on %s", selTotal, selDate.toString("yyyy-MM-dd").ascii());
    GL->CurrentSplit->Memo = tmpStr;

    // Finally, save the entire transaction.
    GL->SaveTrans();

    // Now, walk throught the list one more time and update the 
    // undeposited funds with the linked transaction and set them to be
    // cleared.

    curItem = paymentList->firstChild();
    while (curItem != NULL) {
        if (curItem->isSelected()) {
            DB.dbcmd("update GL set LinkedTrans = %ld, Cleared = 1 where InternalID = %ld", GL->TransID, atol(curItem->key(intIDCol, 0)));
        }
        curItem = curItem->itemBelow();
    }

    // All done, close the window.
    close();

}

/*
** itemSelected - Changes the totals when the customer selects or deselects
**                an item.
*/

void MakeDeposits::itemSelected()
{
    Q3ListViewItem   *curItem;
    int amtCol = 4;
    int selCount = 0;
    selTotal = 0.00;
    curItem = paymentList->firstChild();
    while (curItem != NULL) {
        if (curItem->isSelected()) {
            selCount++;
            selTotal += atof(curItem->key(amtCol,0));
        }
        curItem = curItem->itemBelow();
    }
    char    tmpStr[1024];
    sprintf(tmpStr, "$%.2f", selTotal);
    amountSelected->setText(tmpStr);
    sprintf(tmpStr, "%d", selCount);
    selectedItemCount->setText(tmpStr);
}

/*
** itemDoubleClicked - When an item is double clicked, we emit a signalt
**                     to open the customer window.
*/

void MakeDeposits::itemDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem == NULL) return;
    emit(openCustomer(atol(curItem->key(1,0))));
}

/*
** selectNone - Deselects all of the entries in the paymentList.
*/

void MakeDeposits::selectNone()
{
    paymentList->selectAll(false);
    itemSelected();
}


/*
** selectAll - Selects all of the entries in the paymentList.
*/

void MakeDeposits::selectAll()
{
    paymentList->selectAll(true);
    itemSelected();
}

/**
 * printSelected()
 *
 * Brings up the printer dialog and prints the *selected*
 * checks.
 */
void MakeDeposits::printSelected()
{
    QString         dateStr;
    QListViewPrint  lvPrn(paymentList);

    dateStr = selectedItemCount->text();
    dateStr += " items totalling ";
    dateStr += amountSelected->text();

    lvPrn.setTitle("Deposited Item Summary");
    lvPrn.setDateLine(dateStr);
    lvPrn.setPrintSelectedOnly(1);
    lvPrn.print();
}


// vim: expandtab
