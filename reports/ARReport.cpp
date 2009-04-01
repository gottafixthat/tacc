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
#include <Qt3Support/q3listview.h>

#include <ADB.h>
#include <BString.h>

#include "ARReport.h"

using namespace Qt;

ARReport::ARReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Accounts Receivable Report" );
	setTitle("Accounts Receivable");
	
	repBody->setColumnText(0, "Account");  repBody->setColumnAlignment(0, AlignLeft);
	repBody->addColumn("Amount");          repBody->setColumnAlignment(1, AlignRight);
	
	refreshReport();
}


ARReport::~ARReport()
{
}


/*
** refreshReport   - A virtual function that gets called when the user
**                   changes one of the dates.
*/

void ARReport::refreshReport()
{
    QApplication::setOverrideCursor(waitCursor);

    repBody->clear();
    
    char    sDate[64];
    char    eDate[64];
    char    tmpSt[1024];
    float   total = 0.00;
    float   tmpFloat;
    ADB     DB;
    
    QDatetomyDate(sDate, startDate());
    QDatetomyDate(eDate, endDate());
    
    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s' and Amount >= 0.00 and TransType = 0", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new Q3ListViewItem(repBody, "Amount Billed", tmpSt);
    total += tmpFloat;
    
    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s' and Amount <= 0.00 and TransType = 0", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new Q3ListViewItem(repBody, "Adjustments/credits", tmpSt);
    total += tmpFloat;
    
    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s' and TransType = 2", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new Q3ListViewItem(repBody, "Payments Received", tmpSt);
    total += tmpFloat;

    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s'", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new Q3ListViewItem(repBody, "Balance", tmpSt);

    sprintf(tmpSt, "%.2f", total);
    (void) new Q3ListViewItem(repBody, "Total", tmpSt);
    
    QApplication::restoreOverrideCursor();
}

// vim: expandtab
