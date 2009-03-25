/**
 * PaymentTermsEdit.cpp - Class file to edit payment terms.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qregexp.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include <BlargDB.h>
#include <ADB.h>

#include <PaymentTermsEdit.h>

PaymentTermsEdit::PaymentTermsEdit(QWidget* parent, const char* name, int InternalID) :
	TAAWidget( parent, name )
{
	setCaption( "Edit Payment Terms" );

    QLabel  *descLabel = new QLabel(this, "descLabel");
    descLabel->setAlignment(AlignRight|AlignVCenter);
    descLabel->setText("Description:");

    desc = new QLineEdit(this, "desc");
    desc->setMaxLength(60);

    QLabel *dueDaysPrefix = new QLabel(this, "dueDaysPrefix");
    dueDaysPrefix->setAlignment(AlignRight|AlignVCenter);
    dueDaysPrefix->setText("Net due in");

    dueDays = new QLineEdit(this, "dueDays");
    dueDays->setMaxLength(3);
    QSize   tmpSize = dueDays->minimumSizeHint();
    int     charWidth = tmpSize.width();
    dueDays->setMaximumWidth(3 * charWidth);

    QLabel *dueDaysSuffix = new QLabel(this, "dueDaysSuffix");
    dueDaysSuffix->setAlignment(AlignLeft|AlignVCenter);
    dueDaysSuffix->setText("days.");

    QLabel *graceDaysPrefix = new QLabel(this, "graceDaysPrefix");
    graceDaysPrefix->setAlignment(AlignRight|AlignVCenter);
    graceDaysPrefix->setText("Grace period of");

    graceDays = new QLineEdit(this, "graceDays");
    graceDays->setMaxLength(3);
    graceDays->setMaximumWidth(3 * charWidth);

    QLabel *graceDaysSuffix = new QLabel(this, "graceDaysSuffix");
    graceDaysSuffix->setAlignment(AlignLeft|AlignVCenter);
    graceDaysSuffix->setText("days.");

    QLabel *financePctPrefix = new QLabel(this, "financePctPrefix");
    financePctPrefix->setAlignment(AlignRight|AlignVCenter);
    financePctPrefix->setText("Charge finance charge of");
    
    financePct = new QLineEdit(this, "financePct");
    financePct->setMaxLength(5);
    financePct->setMaximumWidth(5 * charWidth);
    
    QLabel *financePctSuffix = new QLabel(this, "financePctSuffix");
    financePctSuffix->setAlignment(AlignLeft|AlignVCenter);
    financePctSuffix->setText("percent, starting");

    financeDays = new QLineEdit(this, "financeDays");
    financeDays->setMaxLength(3);
    financeDays->setMaximumWidth(3 * charWidth);
    
    QLabel *financeDaysSuffix = new QLabel(this, "financeDaysSuffix");
    financeDaysSuffix->setAlignment(AlignLeft|AlignVCenter);
    financeDaysSuffix->setText("days after grace period.");

    QLabel *discountPctPrefix = new QLabel(this, "discountPctPrefix");
    discountPctPrefix->setAlignment(AlignRight|AlignVCenter);
    discountPctPrefix->setText("Auto apply discount of");

    discountPct = new QLineEdit(this, "discountPct");
    discountPct->setMaxLength(5);
    discountPct->setMaximumWidth(5 * charWidth);
    
    QLabel *discountPctSuffix = new QLabel(this, "discountPctSuffix");
    discountPctSuffix->setAlignment(AlignLeft|AlignVCenter);
    discountPctSuffix->setText("percent if paid within");

    discountDays = new QLineEdit(this, "discountPct");
    discountDays->setMaxLength(3);
    discountDays->setMaximumWidth(3 * charWidth);

    QLabel *discountDaysSuffix = new QLabel(this, "discountDaysSuffix");
    discountDaysSuffix->setAlignment(AlignLeft|AlignVCenter);
    discountDaysSuffix->setText("days before due date.");

    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savePaymentTerms()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPaymentTerms()));

    // Create our layout
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(5, 5);
    int curRow = 0;
    gl->addWidget(descLabel,            curRow, 0);
    gl->addMultiCellWidget(desc,        curRow, curRow, 1, 4);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(dueDaysPrefix,        curRow, 0);
    gl->addWidget(dueDays,              curRow, 1);
    gl->addWidget(dueDaysSuffix,        curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(graceDaysPrefix,      curRow, 0);
    gl->addWidget(graceDays,            curRow, 1);
    gl->addWidget(graceDaysSuffix,      curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(financePctPrefix,     curRow, 0);
    gl->addWidget(financePct,           curRow, 1);
    gl->addWidget(financePctSuffix,     curRow, 2);
    gl->addWidget(financeDays,          curRow, 3);
    gl->addWidget(financeDaysSuffix,    curRow, 4);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(discountPctPrefix,    curRow, 0);
    gl->addWidget(discountPct,          curRow, 1);
    gl->addWidget(discountPctSuffix,    curRow, 2);
    gl->addWidget(discountDays,         curRow, 3);
    gl->addWidget(discountDaysSuffix,   curRow, 4);
    gl->setRowStretch(curRow, 0);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 0);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 0);
    gl->setColStretch(4, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    myIntID  = InternalID;
    
    if (InternalID) {
    	setCaption("Edit Payment Terms");
    } else {
        setCaption("New Payment Terms");
    }
    
    // We're editing one, so get the data for it.
    if (myIntID) {
        PaymentTermsDB PTDB;
        if (PTDB.get(myIntID)) {
            desc->setText(PTDB.getStr("TermsDesc"));
            dueDays->setText(PTDB.getStr("DueDays"));
            graceDays->setText(PTDB.getStr("GraceDays"));
            financeDays->setText(PTDB.getStr("FinanceDays"));
            financePct->setText(PTDB.getStr("FinancePercent"));
            discountPct->setText(PTDB.getStr("DiscountPercent"));
            discountDays->setText(PTDB.getStr("DiscountDays"));
        } else {
            // Switch it so that it seems like we're adding...
            myIntID = 0;
        }
    }
    
    desc->setFocus();

}


PaymentTermsEdit::~PaymentTermsEdit()
{
}


    
    
// The user clicked on the close button...

void PaymentTermsEdit::cancelPaymentTerms()
{
    close();
    delete this;
}

// The user clicked on the save button...

void PaymentTermsEdit::savePaymentTerms()
{
    PaymentTermsDB  PTDB;

    // Get the data from the dialog
    PTDB.setValue("TermsDesc",       desc->text());
    PTDB.setValue("DueDays",         dueDays->text());
    PTDB.setValue("GraceDays",       graceDays->text());
    PTDB.setValue("FinanceDays",     financeDays->text());
    PTDB.setValue("FinancePercent",  financePct->text());
    PTDB.setValue("DiscountPercent", discountPct->text());
    PTDB.setValue("DiscountDays",    discountDays->text());
    
    if (!myIntID) {
        // New record...
        // PTDB.InternalID = 0;
        PTDB.ins();
    } else {
        // Updating a record...
        PTDB.setValue("InternalID", myIntID);
        PTDB.upd();
    }
    
    emit refresh(1);
    close();
//    delete this;
}

// A public slot for saving...

void PaymentTermsEdit::refreshVendList(int)
{
}

