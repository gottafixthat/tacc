/*
** $Id: PaymentTermsEdit.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** PaymentTermsEdit - Allows the user to edit payment terms.
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
** $Log: PaymentTermsEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "PaymentTermsEdit.h"
#include "BlargDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <qregexp.h>
#include <ADB.h>


#define Inherited PaymentTermsEditData

PaymentTermsEdit::PaymentTermsEdit
(
	QWidget* parent,
	const char* name,
	int InternalID
)
	:
	Inherited( parent, name )
{
	setCaption( "Edit Payment Terms" );

    IntID  = InternalID;
    
    if (InternalID) {
    	setCaption("Edit Payment Terms");
    } else {
        setCaption("New Payment Terms");
    }

    // Clear our mappings to internal ID's
    intIDIndex = NULL;
    
    // We're editing one, so get the data for it.
    if (IntID) {
        PaymentTermsDB PTDB;
        if (PTDB.get(IntID)) {
            desc->setText(PTDB.getStr("TermsDesc"));
            dueDays->setText(PTDB.getStr("DueDays"));
            graceDays->setText(PTDB.getStr("GraceDays"));
            financeDays->setText(PTDB.getStr("FinanceDays"));
            financePct->setText(PTDB.getStr("FinancePercent"));
            discountPct->setText(PTDB.getStr("DiscountPercent"));
            discountDays->setText(PTDB.getStr("DiscountDays"));
        } else {
            // Switch it so that it seems like we're adding...
            IntID = 0;
        }
    }
    
    desc->setFocus();

}


PaymentTermsEdit::~PaymentTermsEdit()
{
    if (intIDIndex != NULL) {
        free(intIDIndex);
    }
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
    
    if (!IntID) {
        // New record...
        // PTDB.InternalID = 0;
        PTDB.ins();
    } else {
        // Updating a record...
        PTDB.setValue("InternalID", IntID);
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

