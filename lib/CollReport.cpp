/*
***************************************************************************
**
** CollReport.cpp - Prints a collection report for the specified customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2005, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/


#include "CollReport.h"

#include <qmessagebox.h>
#include <qprogressbar.h>
#include <qdatetm.h>
#include <qapplication.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <AcctsRecv.h>
#include <BlargDB.h>
#include <BString.h>
#include <BrassClient.h>
#include "ParseFile.h"


// The print class itself...

CollReport::CollReport()
{
    // The positions of our columns
    transDateX1     = 20;
    transDateX2     = transDateX1 + 44;
    dueDateX1       = transDateX2 + 1;
    dueDateX2       = dueDateX1 + 44;
    descriptionX1   = dueDateX2 + 1;
    descriptionX2   = descriptionX1 + 399;
    amountX1        = descriptionX2 + 1;
    amountX2        = amountX1 + 39;
    balanceX1       = amountX2 + 1;
    balanceX2       = balanceX1 + 39;

    //fprintf(stderr, "descriptionX1 = %d, descriptionX2 = %d\n", descriptionX1, descriptionX2);
}

CollReport::~CollReport()
{
}

/*
** printHeader - Puts a header on the page containing the Blarg address,
**               the date, etc.
*/

void CollReport::printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance)
{
    QDate       theDate;
    QRect       rect;
    QString     tmpSt;
    int         yPos;
    int         pOfs = 36;
    long        CustID = cust->getLong("CustomerID");

    theDate = QDate::currentDate();

    char        addrFont[1024];
    char        infoFont[1024];

    strcpy(addrFont, "b&h lucida");
    strcpy(infoFont, "b&h lucida");
    
    // p->rotate(55);
    yPos = pOfs;
    p->setFont(QFont(addrFont, 10, QFont::Bold));
    p->drawText(10, yPos, "Blarg! Online Services, Inc.");
    p->setFont(QFont(addrFont, 10, QFont::Normal));
    yPos += 10;
    p->drawText(10, yPos, "PO Box 1827");
    yPos += 10;
    p->drawText(10, yPos, "Bellevue, WA 98009-1827");
    yPos += 10;
    p->drawText(10, yPos, "Phone:  425.818.6500");
    yPos += 10;
    p->drawText(10, yPos, "Fax: 425.401.9741");

    rect.setCoords(450, pOfs, 600, pOfs+15);
    p->setFont(QFont(addrFont, 14, QFont::Bold));
    p->drawText(rect, Qt::AlignCenter, "Collections Report");

    rect.setCoords(450, pOfs+15, 600, pOfs+25);
    p->setFont(QFont(addrFont, 10, QFont::Normal));
    p->drawText(rect, Qt::AlignCenter, theDate.toString());
    p->drawLine(  0,  78, 692, 78);

    p->setFont(QFont(addrFont, 10, QFont::Normal));
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



    p->setFont(QFont(infoFont, 10, QFont::Normal));
    rect.setCoords(350,80,450, 93);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Customer ID:");
    rect.setCoords(460,80,600, 93);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, cust->getStr("CustomerID"));

    rect.setCoords(350,90,450,103);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Unpaid Balance:");
    rect.setCoords(460,90,600, 103);
    tmpSt.sprintf("$%.2f", Balance);
    p->setFont(QFont(infoFont, 10, QFont::Bold));
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, tmpSt);
    p->setFont(QFont(infoFont, 10, QFont::Normal));

    PhoneNumbersDB  PDB;

    rect.setCoords(350,100,450,113);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Day Phone:");
    rect.setCoords(460,100,600,113);
    PDB.get(REF_CUSTOMER, CustID, "Daytime");
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, PDB.PhoneNumber);
    rect.setCoords(350,110,450,123);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Eve Phone:");
    PDB.get(REF_CUSTOMER, CustID, "Evening");
    rect.setCoords(460,110,600,123);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, PDB.PhoneNumber);

}

/*
** printFooter - Puts a footer on the page containing the page number
*/

void CollReport::printFooter(QPainter *p, int PageNo)
{
    char    tmpSt[1024];
    char    addrFont[1024];

    strcpy(addrFont, "b&h lucida");
    
    sprintf(tmpSt, "Page %d", PageNo);

    p->setFont(QFont(addrFont, 10, QFont::Normal));
    p->drawLine(  0, 740, 692,740);
    p->drawText(290, 760, tmpSt);
}

/*
** registerHeader - Puts a register header on the page.
*/

void CollReport::registerHeader(QPainter *p)
{
    QRect       rect;
    QString     tmpSt;
    QBrush      bbrush;
    char        headFont[1024];

    strcpy(headFont, "b&h lucida");

    p->setFont(QFont(headFont, 8, QFont::Normal));
//    p->drawRect(40, 150, 525, 15);
    bbrush.setStyle(Qt::SolidPattern);
    bbrush.setColor(Qt::black);

    //p->setBackgroundMode(Qt::OpaqueMode);
    p->setPen(Qt::white);

    rect.setCoords(transDateX1, 150, transDateX2, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Trans Date");

    rect.setCoords(dueDateX1, 150, dueDateX2, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Due Date");
    p->drawLine(dueDateX1, 150, dueDateX1, 165);
        
    /*
    rect.setCoords(140, 150, 200, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Login ID");
    p->drawLine(140, 150, 140, 165);
    */
        
    rect.setCoords(descriptionX1, 150, descriptionX2, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Description");
    p->drawLine(descriptionX1, 150, descriptionX1, 165);

    rect.setCoords(amountX1, 150, amountX2, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Amount");
    p->drawLine(amountX1, 150, amountX1, 165);

    rect.setCoords(balanceX1, 150, balanceX2, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Balance");
    p->drawLine(balanceX1, 150, balanceX1, 165);

    // Reset our pen back to a transparent background and black letters.
    p->setBackgroundMode(Qt::TransparentMode);
    p->setPen(Qt::black);
}

void CollReport::printReport(long CustID)
{
    QDate       theDate;
    QPrinter    prn(QPrinter::PrinterResolution);
    QPainter    p;
    QRect       rect;
    ADBTable    cust;
    ADBTable    cont;
    ADB         DB;
    QString     tmpSt;
    QDate       tmpDate1;
    char        tmpStr[1024];
    char        tStr[1024];
    int         yPos;
    int         pageNo = 1;
    float       Balance = 0.00;
    float       EndingBalance;
    CustomersDB CDB;
    AddressesDB addrDB;
    char        transFont[1024];
    strcpy(transFont, "b&h lucida");
    
    CDB.get(CustID);
    addrDB.get(REF_CUSTOMER, CustID, "Billing");
    
    theDate = QDate::currentDate();
    
    sprintf(tStr, "/tmp/collreport-%09ld.ps", CustID);
    // prn.setPrintProgram("ghostview");
    //prn.setPrinterName("PostScript");
    //prn.setOutputFileName(tStr);
    prn.setOutputToFile(false);
    prn.setPageSize(QPrinter::Letter);
    prn.setFullPage(true);
    prn.setDocName("Collections Report");
    prn.setCreator("Total Accountability");
    
    p.begin(&prn);
    
    EndingBalance = DB.sumFloat("select SUM(Amount) from AcctsRecv where CustomerID = %ld", CustID);
    
    // Put the Blarg header and contact information on the page.
    printHeader(&p, &CDB, &addrDB, EndingBalance);
    
    // Put the register header on the page.
    registerHeader(&p);
    
    // Now, get the register information from the database.
    DB.query("select TransDate, DueDate, LoginID, Memo, Amount from AcctsRecv where CustomerID = %ld order by TransDate, LoginID", CustID);
    
    yPos = 165;
    p.setFont(QFont(transFont, 8, QFont::Normal));
    QFontMetrics fm(p.fontMetrics());
    while (DB.getrow()) {
        //int Lines = (int) (strlen(DB.curRow["Memo"]) / 52) + 1;
        //int RowHeight = 15 * Lines;
        int Lines = (int) (fm.width(DB.curRow["Memo"]) / (descriptionX2 - descriptionX1 - 2)) + 1;
        int RowHeight = (fm.height()+2) * Lines;
        
        // Check to see if we have enough room on the page left for this
        // line.
        if (yPos+RowHeight >= 740) {
            printFooter(&p, pageNo++);
            prn.newPage();
            printHeader(&p, &CDB, &addrDB, EndingBalance);
            registerHeader(&p);
            yPos = 165;
            p.setFont(QFont(transFont, 8, QFont::Normal));
        } 
    
        // The transaction date.
        myDatetoQDate(DB.curRow["TransDate"], &tmpDate1);
        sprintf(tmpStr, "%02d/%02d/%02d", tmpDate1.month(), tmpDate1.day(), tmpDate1.year()%100);
        rect.setCoords(transDateX1, yPos, transDateX2, yPos + RowHeight-1);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, tmpStr);
        
        // The Due Date
        myDatetoQDate(DB.curRow["DueDate"], &tmpDate1);
        sprintf(tmpStr, "%02d/%02d/%02d", tmpDate1.month(), tmpDate1.day(), tmpDate1.year()%100);
        rect.setCoords(dueDateX1, yPos, dueDateX2, yPos + RowHeight-1);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, tmpStr);
        
        // The Login ID
        /*
        rect.setCoords(140, yPos, 199, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, DB.curRow["LoginID"]);
        */
        
        // The description...
        //fprintf(stderr, "descriptionX1 = %d, descriptionX2 = %d\n", descriptionX1, descriptionX2);
        //fprintf(stderr, "description = '%s'\n", DB.curRow["Memo"]);
        rect.setCoords(descriptionX1, yPos, descriptionX2, yPos + RowHeight-1);
        p.drawRect(rect);
        rect.setCoords(descriptionX1+1, yPos, descriptionX2, yPos + RowHeight);
        p.drawText(rect, Qt::WordBreak|Qt::AlignLeft|Qt::AlignVCenter, DB.curRow["Memo"]);
        
        // The amount.
        rect.setCoords(amountX1, yPos, amountX2, yPos + RowHeight-1);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, DB.curRow["Amount"]);
        
        // The balance.
        Balance += atof(DB.curRow["Amount"]);
        sprintf(tStr, "%.2f", Balance);
        if (Balance == 0.0) strcpy(tStr, "0.00");
        rect.setCoords(balanceX1, yPos, balanceX2, yPos + RowHeight-1);
        p.drawRect(rect);
        p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tStr);
        
        yPos += RowHeight;
    }

    // Put the footer on the page.
    printFooter(&p, pageNo);
    
    // prn.newPage();
    
    // p.drawText(300, 600, "Page 2");
    
    p.end();
}



