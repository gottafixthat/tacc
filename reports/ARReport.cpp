/*
** ARReport - Gives a summary of the Accounts Receivable for a month.
*/

#include "ARReport.h"

#include <stdio.h>
#include <stdlib.h>
#include <qlistview.h>
#include <qapplication.h>

#include <ADB.h>
#include <BString.h>

ARReport::ARReport
(
	QWidget* parent,
	const char* name
)
	: Report( parent, name )
{
	setCaption( "Accounts Receivable Report" );
	setTitle("Accounts Receivable");
	
	list->setColumnText(0, "Account");  list->setColumnAlignment(0, AlignLeft);
	list->addColumn("Amount");          list->setColumnAlignment(1, AlignRight);
	
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

    list->clear();
    
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
    (void) new QListViewItem(list, "Amount Billed", tmpSt);
    total += tmpFloat;
    
    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s' and Amount <= 0.00 and TransType = 0", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new QListViewItem(list, "Adjustments/credits", tmpSt);
    total += tmpFloat;
    
    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s' and TransType = 2", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new QListViewItem(list, "Payments Received", tmpSt);
    total += tmpFloat;

    tmpFloat = DB.sumFloat("select SUM(Amount) from AcctsRecv where TransDate >= '%s' and TransDate <= '%s'", sDate, eDate);
    sprintf(tmpSt, "%.2f", tmpFloat);
    (void) new QListViewItem(list, "Balance", tmpSt);

    sprintf(tmpSt, "%.2f", total);
    (void) new QListViewItem(list, "Total", tmpSt);
    
    QApplication::restoreOverrideCursor();
}
