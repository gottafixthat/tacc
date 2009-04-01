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

#include <QtGui/QToolTip>
#include <QtGui/QCursor>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPainter>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/q3listview.h>

#include <ADB.h>
#include <TAATools.h>
#include <RListViewItem.h>

#include "CustChargeEdit.h"
#include "CustRegister.h"

using namespace Qt;

CustRegister::CustRegister
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
	setCaption( "Register" );
	
	myCustID = CustID;

    // Create our labels
    QLabel  *custNameLabel = new QLabel(this);
    custNameLabel->setText("Customer Name:");
    custNameLabel->setAlignment(AlignRight);

    customerName = new QLabel(this);
    customerName->setAlignment(AlignLeft);
    
    QLabel  *customerIDLabel = new QLabel(this);
    customerIDLabel->setText("Customer ID:");
    customerIDLabel->setAlignment(AlignRight);

    custIDLabel = new QLabel(this);
    custIDLabel->setAlignment(AlignLeft);
    
    QLabel  *customerBalanceLabel = new QLabel(this);
    customerBalanceLabel->setText("Current Balance:");
    customerBalanceLabel->setAlignment(AlignRight);

    currentBalanceArea = new QLabel(this);
    currentBalanceArea->setAlignment(AlignLeft);
    
    QLabel  *customerStatusLabel = new QLabel(this);
    customerStatusLabel->setText("Customer Status:");
    customerStatusLabel->setAlignment(AlignRight);

    customerStatus = new QLabel(this);
    customerStatus->setAlignment(AlignLeft);
    
    list = new Q3ListView(this);
    list->addColumn( "Date" );
    list->addColumn( "Login ID" );
    list->addColumn( "Item ID" );
    list->addColumn( "Qty" );
    list->setColumnAlignment(3, Qt::AlignRight);
    list->addColumn( "Amount" );
    list->setColumnAlignment(4, Qt::AlignRight);
    list->addColumn( "Total" );
    list->setColumnAlignment(5, Qt::AlignRight);
    list->addColumn( "Description" );
    //list->addColumn( "TrnsID" );

    QPushButton *printButton = new QPushButton(this);
    printButton->setText("&Print");
    connect(printButton, SIGNAL(clicked()), this, SLOT(printRegister()));

    voidChargeButton = new QPushButton(this);
    voidChargeButton->setText("&Void");
    connect(voidChargeButton, SIGNAL(clicked()), this, SLOT(voidCustCharge()));
    voidChargeButton->setEnabled(false);

    delChargeButton = new QPushButton(this);
    delChargeButton->setText("&Delete");
    connect(delChargeButton, SIGNAL(clicked()), this, SLOT(delCustCharge()));

    editChargeButton = new QPushButton(this);
    editChargeButton->setText("&Edit");
    connect(editChargeButton, SIGNAL(clicked()), this, SLOT(editCustCharge()));
    editChargeButton->setEnabled(false);

    newChargeButton = new QPushButton(this);
    newChargeButton->setText("&New");
    connect(newChargeButton, SIGNAL(clicked()), this, SLOT(newCustCharge()));

    QPushButton *closeButton = new QPushButton(this);
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    // Now, create our layouts
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    // The text label layouts
    Q3GridLayout *tl = new Q3GridLayout(2, 4, 3);
    tl->setColStretch(0, 0);
    tl->setColStretch(1, 1);
    tl->setColStretch(2, 0);
    tl->setColStretch(3, 1);
    tl->setRowStretch(0, 0);
    tl->setRowStretch(1, 0);
    tl->addWidget(custNameLabel,        0, 0);
    tl->addWidget(customerName,         0, 1);
    tl->addWidget(customerBalanceLabel, 0, 2);
    tl->addWidget(currentBalanceArea,   0, 3);
    tl->addWidget(customerIDLabel,      1, 0);
    tl->addWidget(custIDLabel,          1, 1);
    tl->addWidget(customerStatusLabel,  1, 2);
    tl->addWidget(customerStatus,       1, 3);

    ml->addLayout(tl, 0);

    ml->addWidget(list, 1);

    // Now, our button area
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(printButton, 0);
    bl->addWidget(voidChargeButton, 0);
    bl->addWidget(delChargeButton, 0);
    bl->addWidget(editChargeButton, 0);
    bl->addWidget(newChargeButton, 0);
    bl->addSpacing(10);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);


    
    
    CustomersDB     CDB;
    
    char    tmpStr[1024];
    
    CDB.get(myCustID);
    customerName->setText(CDB.getStr("FullName"));
    custIDLabel->setText(CDB.getStr("CustomerID"));
    sprintf(tmpStr, "$%.2f", CDB.getFloat("CurrentBalance"));
    currentBalanceArea->setText(tmpStr);
    if (CDB.getInt("Active")) {
        strcpy(tmpStr, "Active");
    } else {
        strcpy(tmpStr, "Inactive");
    }
    customerStatus->setText(tmpStr);
    
    sprintf(tmpStr, "Register for %s (%ld)", (const char *) CDB.getStr("FullName"), myCustID);
    setCaption(tmpStr);

	if (!isManager()) {
		newChargeButton->hide();
		editChargeButton->hide();
		delChargeButton->hide();
		voidChargeButton->hide();
	}


    //list->setSorting(7, TRUE);  // Sort by the internal transaction ID to
                                // make sure that our total works.
    
    // The numeric columns should be aligned to the right.
    
    list->setAllColumnsShowFocus(TRUE);

	refreshRegister(1);
	

}


CustRegister::~CustRegister()
{
    delete (list);
}


/*
** refreshRegister - The routine that actually fills the customer register
**                   with their transactions.
*/

void CustRegister::refreshRegister(int)
{
    QApplication::setOverrideCursor(waitCursor);
	char	tmpItem[1024];
	ADB     DB;
	ADB     DB2;
	float	runningTotal = 0.00;
	char    tmpTransID[64];
	char    totalStr[64];
	char    tmpQty[64];
	char    tmpAmount[64];
	RListViewItem   *curItem = NULL;
	int     isOdd = 1;
	
	DB.query("select InternalID, TransDate, LoginID, ItemID, Quantity, Amount, Memo, PackageItem from AcctsRecv where CustomerID = %ld order by TransDate, InternalID", myCustID);
	
    list->clear();

	QString s ;
	while (DB.getrow()) {
	    sprintf(tmpTransID, "%08ld", atol(DB.curRow["InternalID"]));
		sprintf(tmpQty, "%.2f", atof(DB.curRow["Quantity"]));
		sprintf(tmpAmount, "%.2f", atof(DB.curRow["Amount"]));
		runningTotal += atof(DB.curRow["Amount"]);
		sprintf(totalStr, "%.2f", runningTotal);

        // Get the Item ID
        strcpy(tmpItem, "");
        if (atoi(DB.curRow["ItemID"])) {
			DB2.query("select ItemID from Billables where ItemNumber = %d", atoi(DB.curRow["ItemID"]));
			if (DB2.rowCount) {
				DB2.getrow();
				strcpy(tmpItem, DB2.curRow["ItemID"]);
			}
	    } else {
            if (atoi(DB.curRow["PackageItem"])) {
                DB2.query("select PackageTag from Packages where InternalID = %d", atoi(DB.curRow["PackageItem"]));
                if (DB2.rowCount) {
                    DB2.getrow();
                    sprintf(tmpItem, "[%s]", DB2.curRow["PackageTag"]);
                }
            }
        }

		curItem = new RListViewItem(list, 
		  isOdd,
		  DB.curRow["TransDate"],
		  DB.curRow["LoginID"],
		  tmpItem,
          DB.curRow["Quantity"],
		  tmpAmount,
		  totalStr,
		  DB.curRow["Memo"],
		  tmpTransID
		);
		
		if (isOdd) {
		    isOdd--;
		} else {
		    isOdd++;
		}
    }

    // Hilight the last item in the list, which will be curItem
    if (curItem != NULL) {
        list->setCurrentItem(curItem);
        list->setSelected(curItem, TRUE);
        list->ensureItemVisible(curItem);
    }
    QApplication::restoreOverrideCursor();
}

/*
** refreshRegisterS - A slot that we will use to connect internally.
**                    When called, it will update the local register, and
**                    emit a signal to refresh the other customer data.
*/

void CustRegister::refreshRegisterS(int)
{
//	refreshRegister(1);
	emit(customerUpdated(myCustID));
}

/*
** newCustCharge - Brings up the customer charge edit window so the user
**                 can be charged with additional items.
*/

void CustRegister::newCustCharge()
{
	CustChargeEdit	*chargeWin;
	chargeWin = new CustChargeEdit(0, "", myCustID);
	chargeWin->show();
	connect(chargeWin, SIGNAL(chargeSaved(int)), SLOT(refreshRegister(int)));
}

/*
** editCustCharge - Brings up the customer charge edit window so the user
**                  can edit the charge.
*/

void CustRegister::editCustCharge()
{
	CustChargeEdit	*chargeWin;
	chargeWin = new CustChargeEdit(0, "", myCustID);
	chargeWin->show();
	connect(chargeWin, SIGNAL(chargeSaved(int)), SLOT(refreshRegister(int)));
}

/*
** delCustCharge - Deletes an entry in the customer register after making
**				   sure the user wants to do this.
*/

void CustRegister::delCustCharge()
{
	AcctsRecvDB	ARDB;
	
	// Make sure that the item selected is okay to delete.
    if (QMessageBox::warning(this, "Delete Transaction", "Are you sure you wish to delete\nthe currently selected transaction?", "&Yes", "&No", 0, 1) == 0) {
	    QApplication::setOverrideCursor(waitCursor);
	    Q3ListViewItem   *tmpItem;
	    tmpItem = list->currentItem();
	    ARDB.get(atol(tmpItem->key(7, TRUE)));
	    ARDB.del(atol(tmpItem->key(7, TRUE)));
		QApplication::restoreOverrideCursor();
        refreshRegister(1);
	    emit(customerUpdated(myCustID));
	}
}

/*
** voidCustCharge - Sets the quantity and amount to zero in the customer
**                  register, essentially voiding the transaction.
*/

void CustRegister::voidCustCharge()
{
	CustChargeEdit	*chargeWin;
	chargeWin = new CustChargeEdit(0, "", myCustID);
	chargeWin->show();
	connect(chargeWin, SIGNAL(chargeSaved(int)), SLOT(refreshRegister(int)));
}

/*
** refreshCustomer - A virtual slot that will get called when any other
**                   widget updates a customer record.
*/

void CustRegister::refreshCustomer(long CustID)
{
    if (CustID == myCustID) refreshRegister(1);
}

/*
** printRegister  - Sets up the printer widget and prints the register.
*/

void CustRegister::printRegister()
{
    QDate       theDate;
    QPrinter    prn;
    QPainter    p;
    QRect       rect;
    ADBTable    cust;
    ADBTable    cont;
    ADB         DB;
    QString     tmpSt;
    char        tStr[1024];
    int         yPos;
    int         pageNo = 1;
    float       Balance = 0.00;
    float       EndingBalance;
    CustomersDB CDB;
    AddressesDB addrDB;
    
    CDB.get(myCustID);
    addrDB.get(REF_CUSTOMER, myCustID, "Billing");
    
    theDate = QDate::currentDate();
    
    // prn.setPrintProgram("ghostview");
    prn.setPrinterName("PostScript");
    // prn.setOutputFileName("/home/marc/test.ps");
    // prn.setOutputToFile(TRUE);
    prn.setPageSize(QPrinter::Letter);
    
    prn.setDocName("Register Listing");
    prn.setCreator("Blarg! Online Services, Inc.");
    
    if (!prn.setup()) return;
    
    p.begin(&prn);
    
    EndingBalance = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld", myCustID);
    
    // Put the Blarg header and contact information on the page.
    printHeader(&p, &CDB, &addrDB, EndingBalance);
    
    // Put the register header on the page.
    registerHeader(&p);
    
    // Now, get the register information from the database.
    DB.query("select TransDate, DueDate, LoginID, Memo, Amount from AcctsRecv where CustomerID = %ld order by TransDate, LoginID", myCustID);
    
    yPos = 165;
    p.setFont(QFont("courier", 8, QFont::Normal));
    while (DB.getrow()) {
        int Lines = (int) (strlen(DB.curRow["Memo"]) / 52) + 1;
        int RowHeight = 15 * Lines;
        
        // Check to see if we have enough room on the page left for this
        // line.
        if (yPos+RowHeight >= 740) {
            printFooter(&p, pageNo++);
            prn.newPage();
            printHeader(&p, &CDB, &addrDB, EndingBalance);
            registerHeader(&p);
            yPos = 165;
            p.setFont(QFont("courier", 8, QFont::Normal));
        } 
    
        // The transaction date.
        rect.setCoords(20, yPos, 79, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, DB.curRow["TransDate"]);
        
        // The Due Date
        rect.setCoords(80, yPos, 139, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, DB.curRow["DueDate"]);
        
        // The Login ID
        rect.setCoords(140, yPos, 199, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, DB.curRow["LoginID"]);
        
        // The description...
        rect.setCoords(200, yPos, 419, yPos + RowHeight);
        p.drawRect(rect);
        rect.setCoords(203, yPos, 419, yPos + RowHeight);
        p.drawText(rect, WordBreak|AlignLeft|AlignVCenter, DB.curRow["Memo"]);
        
        // The amount.
        rect.setCoords(420, yPos, 479, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignRight|AlignVCenter, DB.curRow["Amount"]);
        
        // The balance.
        Balance += atof(DB.curRow["Amount"]);
        sprintf(tStr, "%.2f", Balance);
        if (Balance == 0.0) strcpy(tStr, "0.00");
        rect.setCoords(480, yPos, 539, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignRight|AlignVCenter, tStr);
        
        yPos += RowHeight;
    }

    // Put the footer on the page.
    printFooter(&p, pageNo);
    
    // prn.newPage();
    
    // p.drawText(300, 600, "Page 2");
    
    p.end();
}


/*
** printHeader - Puts a header on the page containing the Blarg address,
**               the date, etc.
*/

void CustRegister::printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance)
{
    QDate       theDate;
    QRect       rect;
    QString     tmpSt;
    int         yPos;

    theDate = QDate::currentDate();
    
    // p->rotate(55);
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(10, 30, "Blarg! Online Services, Inc.");
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText(10, 40, "P.O. Box 7385");
    p->drawText(10, 50, "Bellevue, WA 98008-1385");
    p->drawText(10, 60, "Phone:  425/401-9821");
    p->drawText(10, 70, "Fax: 425/401-9741");

    p->setFont(QFont("helvetica", 14, QFont::Bold));
    p->drawText(450, 40, "Customer Register");
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(470, 60, theDate.toString());
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawLine(  0,  75, 692, 75);

    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText( 20, 90, "Customer:");
    yPos = 90;
    if (strlen(cust->getStr("FullName"))) {
        p->drawText( 70, yPos, cust->getStr("FullName"));
        yPos += 10;
    }
    if (strlen(cust->getStr("ContactName"))) {
        p->drawText( 70, yPos, cust->getStr("ContactName"));
        yPos += 10;
    }
    if (strlen(cont->Address1)) {
        p->drawText( 70, yPos, cont->Address1);
        yPos += 10;
    }
    if (strlen(cont->Address2)) {
        p->drawText( 70, yPos, cont->Address2);
        yPos += 10;
    }

    tmpSt = cont->City;
    tmpSt.append(", ");
    tmpSt.append(cont->State);
    tmpSt.append(" ");
    tmpSt.append(cont->ZIP);
    p->drawText( 70,yPos, tmpSt);
    yPos += 10;



    p->setFont(QFont("courier", 10, QFont::Normal));
    rect.setCoords(350,80,450, 93);
    p->drawText(rect, AlignRight|AlignVCenter, "Customer ID:");
    rect.setCoords(460,80,600, 93);
    p->drawText(rect, AlignLeft|AlignVCenter, cust->getStr("CustomerID"));

    rect.setCoords(350,90,450,103);
    p->drawText(rect, AlignRight|AlignVCenter, "Balance:");
    rect.setCoords(460,90,600, 103);
    tmpSt.sprintf("$%.2f", Balance);
    p->setFont(QFont("courier", 10, QFont::Bold));
    p->drawText(rect, AlignLeft|AlignVCenter, tmpSt);
    p->setFont(QFont("courier", 10, QFont::Normal));

    //PhoneNumbersDB  PDB;
    // FIXME:  This should list all available phone numbers for this customer

    rect.setCoords(350,100,450,113);
    p->drawText(rect, AlignRight|AlignVCenter, "Day Phone:");
    rect.setCoords(460,100,600,113);
    //PDB.get(REF_CUSTOMER, CustID, "Daytime");
    //p->drawText(rect, AlignLeft|AlignVCenter, PDB.PhoneNumber);
    rect.setCoords(350,110,450,123);
    p->drawText(rect, AlignRight|AlignVCenter, "Eve Phone:");
    //PDB.get(REF_CUSTOMER, CustID, "Evening");
    rect.setCoords(460,110,600,123);
    //p->drawText(rect, AlignLeft|AlignVCenter, PDB.PhoneNumber);

}

/*
** printFooter - Puts a footer on the page containing the page number
*/

void CustRegister::printFooter(QPainter *p, int PageNo)
{
    char    tmpSt[1024];
    
    sprintf(tmpSt, "Page %d", PageNo);

    p->drawLine(  0, 740, 692,740);
    p->drawText(290, 760, tmpSt);
}

/*
** registerHeader - Puts a register header on the page.
*/

void CustRegister::registerHeader(QPainter *p)
{
    QRect       rect;
    QString     tmpSt;
    QBrush      bbrush;

    p->setFont(QFont("helvetica", 10, QFont::Normal));
//    p->drawRect(40, 150, 525, 15);
    bbrush.setStyle(SolidPattern);
    bbrush.setColor(black);

    p->setBackgroundMode(OpaqueMode);
    p->setPen(white);

    rect.setCoords(20, 150, 80, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Trans Date");

    rect.setCoords(80, 150, 140, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Due Date");
    p->drawLine(80, 150, 80, 165);
        
    rect.setCoords(140, 150, 200, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Login ID");
    p->drawLine(140, 150, 140, 165);
        
    rect.setCoords(200, 150, 420, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Description");
    p->drawLine(200, 150, 200, 165);

    rect.setCoords(420, 150, 480, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Amount");
    p->drawLine(420, 150, 420, 165);

    rect.setCoords(480, 150, 540, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Balance");
    p->drawLine(480, 150, 480, 165);

    // Reset our pen back to a transparent background and black letters.
    p->setBackgroundMode(TransparentMode);
    p->setPen(black);
}


// vim: expandtab
