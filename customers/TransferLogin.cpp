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

#include <qapplication.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <ADB.h>
#include "TransferLogin.h"

TransferLogin::TransferLogin(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
	setCaption( "Transfer Login" );
    
    // Create the widgets
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(AlignCenter|AlignVCenter);

    custSearch = new CustomerSearch(this, "custSearch");

    QLabel *effectiveDateLabel = new QLabel("Effective Date:", this);
    effectiveDateLabel->setAlignment(AlignRight|AlignVCenter);

    effectiveDate = new QDateEdit(QDate::currentDate(), this, "effectiveDate");

    QPushButton *transferButton = new QPushButton("&Transfer", this, "transferButton");
    connect(transferButton, SIGNAL(clicked()), this, SLOT(doTransfer()));

    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelTransfer()));

    // Now the layout.  Made much simpler by using the CustomerSearch widget.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);
    ml->addWidget(messageLabel, 0);
    ml->addWidget(custSearch, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addSpacing(10);
    bl->addWidget(effectiveDateLabel, 0);
    bl->addWidget(effectiveDate, 0);
    bl->addSpacing(10);
    bl->addStretch(1);
    bl->addWidget(transferButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}


TransferLogin::~TransferLogin()
{
}

/*
** setSourceLogin  - Gives us the information that we need to finish the
**                   transfer.
*/

void TransferLogin::setSourceLogin(long custID, const char * LoginID)
{
    char    tmpSt[1024];
    
    myCustID    = custID;
    sourceLogin = LoginID;
    
    sprintf(tmpSt, "Select the target customer for transferring the login '%s'.", LoginID);
    messageLabel->setText(tmpSt);
}

/*
** doTransfer - A slot that does the actual transfer and emits the
**              refresh signal.
**
**              For now, it only moves the login records, the subscription
**              records and puts a note in both accounts advising the
**              transfer.
**
*/

void TransferLogin::doTransfer()
{
    long    DestCustID  = custSearch->currentCustomer();
    if (!DestCustID) return;
    
    QApplication::setOverrideCursor(waitCursor);

    ADB             DB;
    NotesDB         NDB;
    CustomersDB     CDB;
    char            tmpStr[1024];
    
    CDB.get(DestCustID);
    
    // Add a note into the old customer's records first.
    sprintf(tmpStr, "Transferred login %s to Customer %ld (%s)", sourceLogin.ascii(), DestCustID, (const char *) CDB.getStr("FullName"));
    NDB.setValue("CustomerID", myCustID);
    NDB.setValue("LoginID", sourceLogin.ascii());
    NDB.setValue("NoteType", "Operator");
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();
    
    // Update the subscriptions...
    DB.dbcmd("update Subscriptions set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin.ascii());

    // Update the Logins...
    DB.dbcmd("update Logins set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin.ascii());

    // Update the Domains...
    DB.dbcmd("update Domains set CustomerID = %ld where CustomerID = %ld and LoginID = '%s'", DestCustID, myCustID, sourceLogin.ascii());

    // And now put a note into the destination account.
    CDB.get(myCustID);
    sprintf(tmpStr, "Transferred login %s from Customer %ld (%s)", sourceLogin.ascii(), myCustID, (const char *) CDB.getStr("FullName"));
    NDB.setValue("CustomerID", DestCustID);
    NDB.setValue("LoginID", sourceLogin.ascii());
    NDB.setValue("NoteType", "Operator");
    NDB.setValue("NoteText", tmpStr);
    NDB.ins();

    emit(customerUpdated(myCustID));
    emit(customerUpdated(DestCustID));

    QApplication::restoreOverrideCursor();

    QMessageBox::information(this, "Login Transfer Warning", "Note that login ID transfers are not\nrerated automatically.  You will need to\nmake any necessary adjustments to the\ncustomers register manually.");

    close();
}



/*
** cancelTransfer - A slot that closes the window.
*/

void TransferLogin::cancelTransfer()
{
    close();
}


// vim: expandtab

