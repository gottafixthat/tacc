/*
** $Id$
**
***************************************************************************
**
** PaymentTerms - A list of payment terms.
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
** $Log: PaymentTerms.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "PaymentTerms.h"
#include "PaymentTermsEdit.h"
#include "BlargDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <ADB.h>

#define Inherited PaymentTermsData

PaymentTerms::PaymentTerms
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Payment Terms" );
    // Set up the menu...
    
    QPopupMenu * options = new QPopupMenu();
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newPaymentTerms()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editPaymentTerms()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deletePaymentTerms()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(Hide()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);
    
    list->setFocus();

    intIDIndex = NULL;    
    indexPtr   = 0;
}


PaymentTerms::~PaymentTerms()
{
}

void PaymentTerms::Hide()
{
    hide();
}

//
// refreshList    - Connects to the database and refreshes the Payment Terms
//                  list

void PaymentTerms::refreshList(int)
{
    ADB     DB;
    char    tmpstr[128]; 
    int     tmpTop, tmpCur;
    int     rowcnt;
    
    // Save the state of the list.
    tmpTop = list->topItem();
    tmpCur = list->currentItem();
    
    list->setAutoUpdate(FALSE);
    list->clear();
    DB.query("select * from PaymentTerms order by TermsDesc");
    if (intIDIndex != NULL) {
        delete(intIDIndex);
    }
    intIDIndex = new(int[DB.rowCount + 1]);
    indexPtr = -1;
    rowcnt = DB.rowCount;
    while(DB.getrow()) {
        indexPtr++;
        QString tmpStr1 = DB.curRow["InternalID"];
        intIDIndex[indexPtr] = tmpStr1.toInt(); //atoi(DB.curRow[0]);
        sprintf (tmpstr, "%-60s", DB.curRow["TermsDesc"]);
        list->insertItem(tmpstr);
    }
    list->setAutoUpdate(TRUE);
    list->repaint();
    
    rowcnt = DB.rowCount - 1;
    // Restore the state of the list.
    if (tmpTop > -1) {
        while(tmpTop > rowcnt) tmpTop--;
        while(tmpCur > rowcnt) tmpCur--;
        list->setCurrentItem(tmpCur);
        list->setTopItem(tmpTop);
    }

}

//
// newPaymentTerms  - Allows the user to create a new PaymentTerms
//

void PaymentTerms::newPaymentTerms()
{
    PaymentTermsEdit * newPaymentTerms;
    newPaymentTerms = new PaymentTermsEdit();
    newPaymentTerms->show();
    connect(newPaymentTerms, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    
}

//
// editPaymentTerms  - Allows the user to edit a PaymentTerms
//

void PaymentTerms::editPaymentTerms()
{
    int itemNo = -1;
    PaymentTermsEdit * newPaymentTerms;
    itemNo = list->currentItem();
    if (itemNo > -1) {
        newPaymentTerms = new PaymentTermsEdit(0, "", intIDIndex[itemNo]);
        newPaymentTerms->show();
        connect(newPaymentTerms, SIGNAL(refresh(int)),this, SLOT(refreshList(int)));
    }
}

//
// editPaymentTermsL  - Allows the user to edit a new PaymentTerms from the list
//

void PaymentTerms::editPaymentTermsL(int msg)
{
    msg = 0;
    editPaymentTerms();
}

//
// deletePaymentTerms  - Allows the user to delete a new PaymentTerms
//

void PaymentTerms::deletePaymentTerms()
{
    int    CurItem;
    char   tmpstr[256];
    ADB    DB;
    
    CurItem = list->currentItem();
    if (CurItem > -1) {
        DB.query("select TermsDesc from PaymentTerms where InternalID = %d", intIDIndex[CurItem]);
        DB.getrow();
        sprintf(tmpstr, "Are you sure you wish to delete the payment\nterms '%s'?", DB.curRow["TermsDesc"]);
        if (QMessageBox::warning(this, "Delete Payment Terms", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            PaymentTermsDB PTDB;
            if (PTDB.del(intIDIndex[CurItem])) {
                QMessageBox::critical(this, "Delete Payment Terms", "Unable to delete payment terms.\nIt has been assigned to one or more customers or vendors.");
            } else {
                refreshList(1);
            }
        };
    }
}

//
// IntIDfromList  - Gets a PaymentTerms InternalID from a list entry.
//

int PaymentTerms::IntIDfromList(int ListNum)
{
    int Ret = 0;

    QString tmpStr1;
    QString tmpStr2;    
    
    if (ListNum > -1) {
        tmpStr1 = list->currentItem();
    }
    return (Ret);
}
