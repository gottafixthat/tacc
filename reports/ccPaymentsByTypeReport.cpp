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

#include <mcve.h>

#include <ADB.h>
#include <CCValidate.h>
#include <TAATools.h>
#include <BString.h>

#include <ccPaymentDetailReport.h>
#include <ccPaymentsByTypeReport.h>

using namespace Qt;

/**
 * ccPaymentsByTypeReport()
 *
 * Constructor.
 */
ccPaymentsByTypeReport::ccPaymentsByTypeReport(QWidget* parent)
	: TACCReport(parent)
{
	setWindowTitle("Credit Card Transactions By Type");
	setTitle("Credit Card Transactions By Type");

    QStringList headers;
    headers += "Date";
    headers += "Visa/MC";
    headers += "AmEx";
    headers += "Discover";
    headers += "Other";
    headers += "Total";
    repBody->setColumnCount(6);
    repBody->setHeaderLabels(headers);
	
    //setStartDate(QDate(2007,6,1));
    //setEndDate(QDate(2007,6,30));
    allowDates(REP_ALLDATES);
    allowFilters(0);
	refreshReport();
}


/**
 * ~ccPaymentsByTypeReport()
 * 
 * Destructor.
 */
ccPaymentsByTypeReport::~ccPaymentsByTypeReport()
{
}

/**
 * refreshReport()
 *
 * A virtual function that gets called when the user changes
 * one of the dates or otherwise needs to refresh the report.
 */
void ccPaymentsByTypeReport::refreshReport()
{
    ADB     DB;
    ADB     DB2;
    QDate   curDate;
    QString strDate;
    QString txtVisa;
    QString txtAmex;
    QString txtDisc;
    QString txtOther;
    QString txtTotal;
    float   visaTot, amexTot, discTot, otherTot, dateTot;
    float   visaGT, amexGT, discGT, otherGT, grandTot;
    visaTot = amexTot = discTot = otherTot = dateTot = 0.00;
    visaGT = amexGT = discGT = otherGT = grandTot = 0.00;
    int     pmCur = 0;
    int     pmTot = startDate().daysTo(endDate()) + 1;

    repBody->clear();
    curDate = startDate();
    QApplication::setOverrideCursor(Qt::waitCursor);
    while(curDate < endDate().addDays(1)) {
        emit(setProgressRT(pmCur, pmTot));
        pmCur++;
        visaTot = amexTot = discTot = otherTot = dateTot = 0.00;
        // This is easier to just have MySQL do the math.  More queries, but
        // less handling of results.
        strDate = curDate.toString("yyyy-MM-dd");
        
        // Visa/MC
        DB.query("select sum(Amount) from CCTrans where TransDate = '%s' and CardType in (%d,%d) and MCVE_Status = %d", strDate.ascii(), CCTYPE_VISA, CCTYPE_MC, MCVE_AUTH);
        if (DB.rowCount) while(DB.getrow()) {
            visaTot += atof(DB.curRow[0]);
        }

        // Amex
        DB.query("select sum(Amount) from CCTrans where TransDate = '%s' and CardType = %d and MCVE_Status = %d", strDate.ascii(), CCTYPE_AMEX, MCVE_AUTH);
        if (DB.rowCount) while(DB.getrow()) {
            amexTot += atof(DB.curRow[0]);
        }

        // Discover
        DB.query("select sum(Amount) from CCTrans where TransDate = '%s' and CardType = %d and MCVE_Status = %d", strDate.ascii(), CCTYPE_DISC, MCVE_AUTH);
        if (DB.rowCount) while(DB.getrow()) {
            discTot += atof(DB.curRow[0]);
        }

        // NOT Visa, MC, Amex or Discover
        DB.query("select sum(Amount) from CCTrans where TransDate = '%s' and CardType not in (%d,%d,%d,%d) and MCVE_Status = %d", strDate.ascii(), CCTYPE_VISA, CCTYPE_MC, CCTYPE_AMEX, CCTYPE_DISC, MCVE_AUTH);
        if (DB.rowCount) while(DB.getrow()) {
            otherTot += atof(DB.curRow[0]);
        }

        dateTot = visaTot + amexTot + discTot + otherTot;

        visaGT   += visaTot;
        amexGT   += amexTot;
        discGT   += discTot;
        otherGT  += otherTot;
        grandTot += dateTot;

        // Add this record.
        txtVisa = txtVisa.sprintf("%.2f", visaTot);
        txtAmex = txtAmex.sprintf("%.2f", amexTot);
        txtDisc = txtDisc.sprintf("%.2f", discTot);
        txtOther = txtOther.sprintf("%.2f", otherTot);
        txtTotal = txtTotal.sprintf("%.2f", dateTot);
        txtVisa = txtVisa.rightJustify(10, ' ');
        TACCReportItem *curItem = new TACCReportItem(repBody);
        curItem->setText(0, strDate);
        curItem->setText(1, txtVisa);
        curItem->setText(2, txtAmex);
        curItem->setText(3, txtDisc);
        curItem->setText(4, txtOther);
        curItem->setText(5, txtTotal);
        curItem->setSortType(0, TACCReportItem::rString);
        curItem->setSortType(1, TACCReportItem::rDouble);
        curItem->setSortType(2, TACCReportItem::rDouble);
        curItem->setSortType(3, TACCReportItem::rDouble);
        curItem->setSortType(4, TACCReportItem::rDouble);
        curItem->setSortType(5, TACCReportItem::rDouble);
        curItem->setTextAlignment(0, Qt::AlignLeft);
        curItem->setTextAlignment(1, Qt::AlignRight);
        curItem->setTextAlignment(2, Qt::AlignRight);
        curItem->setTextAlignment(3, Qt::AlignRight);
        curItem->setTextAlignment(4, Qt::AlignRight);
        curItem->setTextAlignment(5, Qt::AlignRight);
        curDate = curDate.addDays(1);
    }
    // One last one to make our bar go away...
    emit(setProgressRT(pmCur, pmTot));

    // Grand total
    txtVisa = txtVisa.sprintf("%.2f", visaGT);
    txtAmex = txtAmex.sprintf("%.2f", amexGT);
    txtDisc = txtDisc.sprintf("%.2f", discGT);
    txtOther = txtOther.sprintf("%.2f", otherGT);
    txtTotal = txtTotal.sprintf("%.2f", grandTot);

    TACCReportItem *curItem = new TACCReportItem(repBody);
    curItem->setText(0, "Total");
    curItem->setText(1, txtVisa);
    curItem->setText(2, txtAmex);
    curItem->setText(3, txtDisc);
    curItem->setText(4, txtOther);
    curItem->setText(5, txtTotal);
    curItem->setSortType(0, TACCReportItem::rString);
    curItem->setSortType(1, TACCReportItem::rDouble);
    curItem->setSortType(2, TACCReportItem::rDouble);
    curItem->setSortType(3, TACCReportItem::rDouble);
    curItem->setSortType(4, TACCReportItem::rDouble);
    curItem->setSortType(5, TACCReportItem::rDouble);
    curItem->setTextAlignment(0, Qt::AlignLeft);
    curItem->setTextAlignment(1, Qt::AlignRight);
    curItem->setTextAlignment(2, Qt::AlignRight);
    curItem->setTextAlignment(3, Qt::AlignRight);
    curItem->setTextAlignment(4, Qt::AlignRight);
    curItem->setTextAlignment(5, Qt::AlignRight);
    curItem->setIsTotalLine(true);
    QFont tmpFont = curItem->font(0);
    tmpFont.setWeight(QFont::Bold);
    curItem->setFont(0, tmpFont);
    curItem->setFont(1, tmpFont);
    curItem->setFont(2, tmpFont);
    curItem->setFont(3, tmpFont);
    curItem->setFont(4, tmpFont);
    curItem->setFont(5, tmpFont);

    for (int i = 0; i < repBody->columnCount(); i++) repBody->resizeColumnToContents(i);
    repBody->sortItems(0, Qt::AscendingOrder);
    repBody->setSortingEnabled(true);

    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report, opening a ccPaymentDetails report for the specified
 * day.
 */
void ccPaymentsByTypeReport::listItemSelected(QTreeWidgetItem *curItem, int)
{
    if (curItem != NULL) {
        // Only run the report if its not the total line
        if (curItem->text(0).compare("Total")) {
            QDate   dateSel;
            myDatetoQDate(curItem->text(0), &dateSel);
            ccPaymentDetailReport   *rep = new ccPaymentDetailReport();
            rep->setStartDate(dateSel);
            rep->setEndDate(dateSel);
            rep->refreshReport();
            rep->show();
        }
    }
}



// vim: expandtab
