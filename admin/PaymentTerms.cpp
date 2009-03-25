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

#include <qlayout.h>

#include "PaymentTerms.h"
#include "PaymentTermsEdit.h"
#include "BlargDB.h"
#include <stdlib.h>
#include <stdio.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <ADB.h>


PaymentTerms::PaymentTerms(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
	setCaption( "Payment Terms" );

    menu = new QMenuBar(this, "menu");

    termsList = new QListView(this, "termsList");
    termsList->addColumn("Terms");
    intIDCol = 1;
    connect(termsList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(editTerms(QListViewItem *)));

    QPushButton *newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newPaymentTerms()));

    QPushButton *editButton = new QPushButton(this, "editButton");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editPaymentTerms()));

    QPushButton *deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePaymentTerms()));

    QPushButton *closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    
    // Setup the layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);
    ml->addWidget(menu, 0);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addWidget(newButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);
    bl->addStretch(1);

    ml->addLayout(bl, 0);
    ml->addWidget(termsList, 1);

    // Set up the menu...
    
    QPopupMenu * options = new QPopupMenu(this);
    CHECK_PTR( options );
    options->insertItem("New", this, SLOT(newPaymentTerms()), CTRL+Key_N);
    options->insertItem("Edit", this, SLOT(editPaymentTerms()), CTRL+Key_E);
    options->insertItem("Delete", this, SLOT(deletePaymentTerms()), CTRL+Key_D);
    options->insertSeparator();
    options->insertItem("Close", this, SLOT(closeClicked()), CTRL+Key_C);
    
    menu->insertItem("&Options", options);
    
    termsList->setFocus();

}


PaymentTerms::~PaymentTerms()
{
}

void PaymentTerms::closeClicked()
{
    delete this;
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
    termsList->clear();
    DB.query("select * from PaymentTerms order by TermsDesc");
    if (DB.rowCount) while(DB.getrow()) {
        (void) new QListViewItem(termsList, DB.curRow["TermsDesc"], DB.curRow["InternalID"]);
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
    QListViewItem   *curItem = termsList->currentItem();
    editTerms(curItem);
}

/**
 * editTerms()
 *
 * Gets called when the user double clicks on a list item.
 * It brings up the payment terms editor.
 */
void PaymentTerms::editTerms(QListViewItem *curItem)
{
    if (curItem) {
        int     tmpID = curItem->key(intIDCol, 0).toInt();
        PaymentTermsEdit    *newPaymentTerms;
        newPaymentTerms = new PaymentTermsEdit(0, "", tmpID);
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
    QListViewItem   *curItem;
    char   tmpstr[256];
    ADB    DB;
    
    curItem = termsList->currentItem();
    if (curItem) {
        DB.query("select TermsDesc from PaymentTerms where InternalID = %d", curItem->key(intIDCol, 0).toInt());
        DB.getrow();
        sprintf(tmpstr, "Are you sure you wish to delete the payment\nterms '%s'?", DB.curRow["TermsDesc"]);
        if (QMessageBox::warning(this, "Delete Payment Terms", tmpstr, "&Yes", "&No", 0, 1) == 0) {
            PaymentTermsDB PTDB;
            if (PTDB.del(curItem->key(intIDCol, 0).toInt())) {
                QMessageBox::critical(this, "Delete Payment Terms", "Unable to delete payment terms.\nIt has been assigned to one or more customers or vendors.");
            } else {
                refreshList(1);
            }
        }
    }
}

