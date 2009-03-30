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

#include <qdatetime.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <BString.h>
#include <TAATools.h>

#include "ChangeRatePlan.h"

ChangeRatePlan::ChangeRatePlan(QWidget* parent, const char* name, long CustID) :
	TAAWidget( parent, name )
{
	setCaption( "Edit Customer Rate Plan" );
	
	if (!CustID) return;
	myCustID    = CustID;

    // Create our widgets
    QLabel *customerLabel = new QLabel("Customer:", this, "customerLabel");
    customerLabel->setAlignment(AlignRight|AlignVCenter);

    customer = new QLabel(this, "customer");
    customer->setAlignment(AlignLeft|AlignVCenter);

    QLabel *ratePlanLabel = new QLabel("Customer:", this, "ratePlanLabel");
    ratePlanLabel->setAlignment(AlignRight|AlignVCenter);

    ratePlanList = new QComboBox(false, this, "ratePlanList");

    QLabel *effectiveDateLabel = new QLabel("Effective Date:", this, "effectiveDateLabel");
    effectiveDateLabel->setAlignment(AlignRight|AlignVCenter);

    effectiveDate = new QDateEdit(QDate::currentDate(), this, "effectiveDate");

    QPushButton *saveButton = new QPushButton("&Save", this, "saveButton");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveRatePlanChange()));

    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelRatePlanChange()));

    // Our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(2, 3);
    int curRow = 0;
    gl->addWidget(customerLabel,        curRow, 0);
    gl->addWidget(customer,             curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(ratePlanLabel,        curRow, 0);
    gl->addWidget(ratePlanList,         curRow, 1);
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
	QDate       tmpDate;
    QString     tmpStr;
	
	CDB.get(CustID);
	
    tmpStr = tmpStr.sprintf("%s (%ld)", (const char *) CDB.getStr("FullName"), myCustID);
	customer->setText(tmpStr);
	
	DB.query("select PlanTag,InternalID from RatePlans order by PlanTag");
	while (DB.getrow()) {
	    ratePlanList->insertItem(DB.curRow["PlanTag"]);
	    if (atol(DB.curRow["InternalID"]) == CDB.getLong("RatePlan")) {
	        ratePlanList->setCurrentItem(ratePlanList->count()-1);
	    }
	}
	
}


ChangeRatePlan::~ChangeRatePlan()
{
}

/*
** saveRatePlanChange - Saves the changes...
*/

void ChangeRatePlan::saveRatePlanChange()
{
    CustomersDB CDB;
    ADB         DB;
    
    QApplication::setOverrideCursor(waitCursor);
    CDB.get(myCustID);

    DB.query("select InternalID from RatePlans where PlanTag = '%s'", (const char *) ratePlanList->text(ratePlanList->currentItem()));
    DB.getrow();

    CDB.setValue("RatePlanDate", effectiveDate->date().toString("yyyy-MM-dd").ascii());
    CDB.setValue("RatePlan", atol(DB.curRow["InternalID"]));
    CDB.upd();
    QApplication::restoreOverrideCursor();
    emit(customerUpdated(myCustID));
    close();
}

/*
** cancelRatePlanChange - Closes the window...
*/

void ChangeRatePlan::cancelRatePlanChange()
{
    close();
}

// vim: expandtab
