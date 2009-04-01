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

#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <BlargDB.h>
#include <ADB.h>
#include <TAATools.h>

#include "ExtendGraceDate.h"

using namespace Qt;

ExtendGraceDate::ExtendGraceDate(QWidget* parent, const char* name, long CustomerID) :
	TAAWidget( parent, name )
{
    if (!CustomerID) return;
    myCustID = CustomerID;

    // Create our widgets.
    titleLabel = new QLabel(this, "titleLabel");
    titleLabel->setAlignment(AlignCenter|AlignVCenter);

    QLabel *extendDaysLabel = new QLabel("Extend grace by:", this, "extendDaysLabel");
    extendDaysLabel->setAlignment(AlignRight|AlignVCenter);

    extendDays = new QSpinBox(1, 7, 1, this, "extendDays");
    extendDays->setSuffix("day(s)");
    connect(extendDays, SIGNAL(valueChanged(int)), this, SLOT(daysChanged(int)));

    QLabel *currentGraceLabel = new QLabel("Current Grace Ends:", this, "currentGraceLabel");
    currentGraceLabel->setAlignment(AlignRight|AlignVCenter);

    currentGraceDate = new QLabel(this, "currentGraceDate");
    currentGraceDate->setAlignment(AlignLeft|AlignVCenter);

    QLabel *newGraceLabel = new QLabel("New Grace Ends:", this, "newGraceLabel");
    newGraceLabel->setAlignment(AlignRight|AlignVCenter);

    newGraceDate = new QLabel(this, "newGraceDate");
    newGraceDate->setAlignment(AlignLeft|AlignVCenter);

    QPushButton *saveButton = new QPushButton("&Save", this, "saveButton");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveGrace()));

    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelGrace()));

    // Our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);
    ml->addWidget(titleLabel, 0);
    ml->addSpacing(5);
    
    Q3GridLayout *gl = new Q3GridLayout(2, 3);
    int curRow = 0;
    gl->addWidget(extendDaysLabel,  curRow, 0);
    gl->addWidget(extendDays,       curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(currentGraceLabel,curRow, 0);
    gl->addWidget(currentGraceDate, curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(newGraceLabel,    curRow, 0);
    gl->addWidget(newGraceDate,     curRow, 1);
    gl->setRowStretch(curRow, 0);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);
    ml->addLayout(bl, 0);

    
    CustomersDB     CDB;
    CDB.get(myCustID);
    newGrace = origGrace = CDB.getDate("GraceDate");
    daysChanged(1);

    char    tmpStr[1024];
    sprintf(tmpStr, "Extending grace period for %s", (const char *) CDB.getStr("FullName"));
    titleLabel->setText(tmpStr);
    setCaption(tmpStr);
    
    extendDays->setFocus();

    currentGraceDate->setText(CDB.getDate("GraceDate").toString());

    show();

}


ExtendGraceDate::~ExtendGraceDate()
{
}


/*
** daysChanged - Gets called when the user changes something on the
**               spinbox.
*/

void ExtendGraceDate::daysChanged(int newDays)
{
    newGrace = origGrace.addDays(newDays);
    newGraceDate->setText(newGrace.toString());
}

/*
** cancelGrace - Gets called when the user hits the cancle button.
*/

void ExtendGraceDate::cancelGrace()
{
    close();
}

/*
** saveGrace - Gets called when the user hits the cancle button.
*/

void ExtendGraceDate::saveGrace()
{
    CustomersDB     CDB;
    CDB.get(myCustID);
    
    char    tmpStr[1024];
    sprintf(tmpStr, "%04d-%02d-%02d", newGrace.year(), newGrace.month(), newGrace.day());

    CDB.setValue("GraceDate", tmpStr);
    CDB.upd();

    NotesDB NDB;
    NDB.setValue("CustomerID", myCustID);
    NDB.setValue("LoginID", CDB.getStr("PrimaryLogin"));
    NDB.setValue("NoteType", "Accounting");
    sprintf(tmpStr, "Extended grace period to %s (%d days)", (const char *) newGrace.toString(), origGrace.daysTo(newGrace));
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();

    emit(customerUpdated(myCustID));
    close();
}


// vim: expandtab

