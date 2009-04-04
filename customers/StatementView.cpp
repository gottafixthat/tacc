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
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3groupbox.h>

#include <BlargDB.h>
#include <StatementEngine.h>
#include <ADB.h>
#include <TAATools.h>

#include <StatementView.h>

using namespace Qt;

StatementView::StatementView(QWidget* parent, const char* name) :
	TAAWidget(parent)
{
	setCaption( "Statement View" );

    // Create our widgets.  This is a somewhat complex layout, but not overly.
    // We'll put Billed To, Date Info and Charge Info into three group boxes.
    // We'll then shove those group boxes into a box layout from left to right.
    
    Q3GroupBox   *billGrp = new Q3GroupBox(1, Qt::Horizontal, "Billed To", this, "billGrp");
    custName = new QLabel(billGrp, "custName");
    custName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    custAddr1 = new QLabel(billGrp, "custAddr1");
    custAddr1->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    custAddr2 = new QLabel(billGrp, "custAddr2");
    custAddr2->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    custAddr3 = new QLabel(billGrp, "custAddr3");
    custAddr3->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    custAddr4 = new QLabel(billGrp, "custAddr4");
    custAddr4->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    Q3GroupBox *dateGrp = new Q3GroupBox(2, Qt::Horizontal, "Date Information", this, "dateGrp");

    QLabel *statementNoLabel = new QLabel("Statement No:", dateGrp, "statementNoLabel");
    statementNoLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    statementNo = new QLabel(dateGrp, "statementNoLabel");
    statementNo->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *statementDateLabel = new QLabel("Statement Date:", dateGrp, "statementDateLabel");
    statementDateLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    statementDate = new QLabel(dateGrp, "statementDateLabel");
    statementDate->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    QLabel *chargesThroughLabel = new QLabel("Charges Through:", dateGrp, "chargesThroughLabel");
    chargesThroughLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    chargesThrough = new QLabel(dateGrp, "chargesThroughLabel");
    chargesThrough->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    QLabel *dueDateLabel = new QLabel("Due Date:", dateGrp, "dueDateLabel");
    dueDateLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    dueDate = new QLabel(dateGrp, "dueDateLabel");
    dueDate->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    QLabel *termsLabel = new QLabel("Terms:", dateGrp, "termsLabel");
    termsLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    terms = new QLabel(dateGrp, "termsLabel");
    terms->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	
    Q3GroupBox *chargeGrp = new Q3GroupBox(2, Qt::Horizontal, "Charge Information", this, "chargeGrp");
    
    QLabel *prevBalanceLabel = new QLabel("Previous Balance:", chargeGrp, "prevBalanceLabel");
    prevBalanceLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    prevBalance = new QLabel(chargeGrp, "prevBalanceLabel");
    prevBalance->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	
    QLabel *creditsLabel = new QLabel("Credits:", chargeGrp, "creditsLabel");
    creditsLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    credits = new QLabel(chargeGrp, "creditsLabel");
    credits->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	
    QLabel *newChargesLabel = new QLabel("New Charges:", chargeGrp, "newChargesLabel");
    newChargesLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    newCharges = new QLabel(chargeGrp, "newChargesLabel");
    newCharges->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel *financeChargeLabel = new QLabel("Finance Charge:", chargeGrp, "financeChargeLabel");
    financeChargeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    financeCharge = new QLabel(chargeGrp, "financeChargeLabel");
    financeCharge->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel *totalDueLabel = new QLabel("Total Due:", chargeGrp, "totalDueLabel");
    totalDueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    totalDue = new QLabel(chargeGrp, "totalDueLabel");
    totalDue->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    itemList = new Q3ListView(this, "itemList");
	itemList->addColumn("Int ID");
	itemList->addColumn("Trans Date");
	itemList->addColumn("Login ID");
	itemList->addColumn("Start Date");
	itemList->addColumn("End Date");
	itemList->addColumn("Quantity");
    itemList->setColumnAlignment(5, Qt::AlignRight);
	itemList->addColumn("Price");
    itemList->setColumnAlignment(6, Qt::AlignRight);
	itemList->addColumn("Amount");
    itemList->setColumnAlignment(7, Qt::AlignRight);
	itemList->addColumn("Description");
	itemList->setAllColumnsShowFocus(TRUE);

    QLabel *emailedToLabel = new QLabel("Emailed to:", this, "emailedToLabel");
    emailedToLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    emailedTo = new QLabel(this, "emailedTo");
    emailedToLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *printedOnLabel = new QLabel("Printed on:", this, "printedOnLabel");
    printedOnLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    printedOn = new QLabel(this, "printedOn");
    printedOnLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    emailButton = new QPushButton("&Email", this, "emailButton");
    connect(emailButton, SIGNAL(clicked()), this, SLOT(reEmailStatement()));

    printButton = new QPushButton("&Print", this, "printButton");
    connect(printButton, SIGNAL(clicked()), this, SLOT(rePrintStatement()));

    closeButton = new QPushButton("&Close", this, "closeButton");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeView()));

    // Now, create the layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);

    Q3BoxLayout *grpl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    grpl->addWidget(billGrp, 1);
    grpl->addWidget(dateGrp, 1);
    grpl->addWidget(chargeGrp, 1);

    ml->addLayout(grpl, 0);
    ml->addWidget(itemList, 1);

    Q3GridLayout *gl = new Q3GridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(emailedToLabel,   curRow, 0);
    gl->addWidget(emailedTo,        curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(printedOnLabel,   curRow, 0);
    gl->addWidget(printedOn,        curRow, 1);
    gl->setRowStretch(curRow, 0);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 0);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addSpacing(20);
    bl->addLayout(gl, 0);
    bl->addStretch(1);
    bl->addWidget(emailButton, 0);
    bl->addWidget(printButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);
    
    if (!isManager()) {
        printButton->setEnabled(false);
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
        QApplication::setOverrideCursor(Qt::waitCursor);
        emit(setStatus("Loading statement..."));
        StatementsDB        STDB;
        StatementsDataDB    STDDB;
        ADB                 DB;
        char                tmpStr[1024];
        Q3ListViewItem       *curItem;
        
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
        emailedTo->setText(tmpStr);
        
        if (!strcmp("0000-00-00", (const char *) STDB.getStr("PrintedOn"))) {
            printedOn->setText("Not printed");
        } else {
            printedOn->setText(STDB.getStr("PrintedOn"));
        }

        // Load the table with the line items.
        DB.query("select InternalID from StatementsData where StatementNo = %ld", stNo);
        while (DB.getrow()) {
            STDDB.get(atol(DB.curRow["InternalID"]));
            
            curItem = new Q3ListViewItem(itemList,
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
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
    }
    if (isHidden()) {
        QSize   mainSize = sizeHint();
        QSize   listSize = itemList->sizeHint();
        mainSize.setWidth(listSize.width()+25);
        resize(mainSize);
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
    QApplication::setOverrideCursor(Qt::waitCursor);
    StatementEngine stEng;
    stEng.emailStatement(myStNo, 1, 1);
    QApplication::restoreOverrideCursor();
}

/*
** rePrintStatement - Prints a duplicate statement to the user.
*/

void StatementView::rePrintStatement()
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    StatementEngine stEng;
    stEng.printStatement(myStNo, 1, 1);
    QApplication::restoreOverrideCursor();
}


// vim: expandtab
