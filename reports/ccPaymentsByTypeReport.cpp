/**
 * ccPaymentsByType.cpp - Credit Card Payments by type
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

#include <ccPaymentsByTypeReport.h>
#include <ccPaymentDetailReport.h>
#include <ADB.h>
#include <CCValidate.h>
#include <mcve.h>
#include <TAATools.h>
#include <BString.h>

/**
 * ccPaymentsByTypeReport()
 *
 * Constructor.
 */
ccPaymentsByTypeReport::ccPaymentsByTypeReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption("Credit Card Transactions By Type");
	setTitle("Credit Card Transactions By Type");
	
	repBody->setColumnText(0, "Date");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Visa/MC");      repBody->setColumnAlignment(1, AlignRight);
	repBody->addColumn("AmEx");         repBody->setColumnAlignment(2, AlignRight);
	repBody->addColumn("Discover");     repBody->setColumnAlignment(3, AlignRight);
	repBody->addColumn("Other");        repBody->setColumnAlignment(4, AlignRight);
	repBody->addColumn("Total");        repBody->setColumnAlignment(5, AlignRight);
	
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
    QApplication::setOverrideCursor(waitCursor);
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
        (void) new QListViewItem(repBody, strDate, txtVisa, txtAmex, txtDisc, txtOther, txtTotal);
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
    (void) new QListViewItem(repBody, "Total", txtVisa, txtAmex, txtDisc, txtOther, txtTotal);
    QApplication::restoreOverrideCursor();
}


/**
 * listItemSelected()
 *
 * When an item is double clicked, this is called.  It drills down
 * in the report, opening a ccPaymentDetails report for the specified
 * day.
 */
void ccPaymentsByTypeReport::listItemSelected(QListViewItem *curItem)
{
    if (curItem != NULL) {
        // Only run the report if its not the total line
        if (strcmp(curItem->key(0,0), "Total")) {
            QDate   dateSel;
            myDatetoQDate(curItem->key(0,0), &dateSel);
            ccPaymentDetailReport   *rep = new ccPaymentDetailReport();
            rep->setStartDate(dateSel);
            rep->setEndDate(dateSel);
            rep->refreshReport();
            rep->show();
        }
    }
}


