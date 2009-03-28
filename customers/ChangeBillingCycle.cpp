/*
** $Id$
**
***************************************************************************
**
** ChangeBillingCycle - Allows the user to change the billing cycle of
**                      a customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: ChangeBillingCycle.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include <stdlib.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <BString.h>
#include <TAATools.h>
#include <ADB.h>

#include "ChangeBillingCycle.h"

ChangeBillingCycle::ChangeBillingCycle(QWidget* parent, const char* name, long CustID) :
	TAAWidget( parent, name )
{
	setCaption( "Edit Customer Billing Cycle" );

	if (!CustID) return;
	myCustID    = CustID;

    // Create the widgets
    QLabel *customerLabel = new QLabel("Customer:", this, "customerLabel");
    customerLabel->setAlignment(AlignRight|AlignVCenter);

    customer = new QLabel(this, "customer");
    customer->setAlignment(AlignLeft|AlignVCenter);

    QLabel *cycleListLabel = new QLabel("New Billing Cycle:", this, "cycleListLabel");
    cycleListLabel->setAlignment(AlignRight|AlignVCenter);

    cycleList = new QComboBox(false, this, "cycleList");

    QLabel *effectiveDateLabel = new QLabel("Customer:", this, "effectiveDateLabel");
    effectiveDateLabel->setAlignment(AlignRight|AlignVCenter);

    effectiveDate = new QDateEdit(QDate::currentDate(), this, "effectiveDate");

    QPushButton *saveButton = new QPushButton("&Save", this, "saveButton");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveBillingCycleChange()));

    QPushButton *cancelButton = new QPushButton("&Save", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelBillingCycleChange()));

    // Our layout.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(2, 3);
    int curRow = 0;
    gl->addWidget(customerLabel,        curRow, 0);
    gl->addWidget(customer,             curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(cycleListLabel,       curRow, 0);
    gl->addWidget(cycleList,            curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(effectiveDateLabel,   curRow, 0);
    gl->addWidget(effectiveDate,        curRow, 1);
    gl->setRowStretch(curRow, 0);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

	
	// Load the list of rate plans.
	ADB         DB;
	CustomersDB CDB;
	char        tmpStr[1024];
	
	CDB.get(CustID);
	
	sprintf(tmpStr, "%s (%ld)", (const char *) CDB.getStr("FullName"), myCustID);
	customerLabel->setText(tmpStr);
	
	DB.query("select CycleID,InternalID from BillingCycles order by CycleID");
	while (DB.getrow()) {
	    cycleList->insertItem(DB.curRow["CycleID"]);
	    if (atol(DB.curRow["InternalID"]) == CDB.getLong("BillingCycle")) {
	        cycleList->setCurrentItem(cycleList->count()-1);
	    }
	}
	

}


ChangeBillingCycle::~ChangeBillingCycle()
{
}

/*
** saveBillingCycleChange - Saves the changes...
*/

void ChangeBillingCycle::saveBillingCycleChange()
{
    CustomersDB CDB;
    ADB         DB;
    
    QApplication::setOverrideCursor(waitCursor);
    CDB.get(myCustID);

    DB.query("select InternalID from BillingCycles where CycleID = '%s'", (const char *) cycleList->text(cycleList->currentItem()));
    DB.getrow();

    CDB.setValue("BillingCycleDate", effectiveDate->date().toString("yyyy-MM-dd").ascii());
    CDB.setValue("BillingCycle", atol(DB.curRow["InternalID"]));
    CDB.upd();
    QApplication::restoreOverrideCursor();
    emit(customerUpdated(myCustID));
    
    QMessageBox::information(this, "Rerate warning", "Note that billing cycle changes are not\nrerated automatically.  You will need to\nmake any necessary adjustments to the\ncustomers register manually.");
    
    close();
}

/*
** cancelBillingCycleChange - Closes the window...
*/

void ChangeBillingCycle::cancelBillingCycleChange()
{
    close();
}



