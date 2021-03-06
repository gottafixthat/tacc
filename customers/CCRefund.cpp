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

#include <QtCore/QRegExp>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <mcve.h>

#include <Cfg.h>
#include <BlargDB.h>
#include <ADB.h>
#include <TAAStructures.h>
#include <TAATools.h>
#include <AcctsRecv.h>
#include <ConfirmBox.h>
#include <CCTools.h>
#include <CCRefund.h>

/**
 * CCRefund()
 *
 * Constructor.
 */
CCRefund::CCRefund(QWidget *parent, const char *name) :
    TAAWidget(parent)
{
    setCaption("Issue Credit Card Refund");

    QLabel *customerIDLabel = new QLabel(this, "customerIDLabel");
    customerIDLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    customerIDLabel->setText("Customer ID:");

    customerID = new QLabel(this, "customerID");
    customerID->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *customerNameLabel = new QLabel(this, "customerNameLabel");
    customerNameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    customerNameLabel->setText("Customer Name:");

    customerName = new QLabel(this, "customerName");
    customerName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *cardListLabel = new QLabel(this, "cardListLabel");
    cardListLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    cardListLabel->setText("Card to refund:");

    cardList = new QComboBox(false, this, "cardList");

    QLabel *refundAmountLabel = new QLabel(this, "refundAmountLabel");
    refundAmountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    refundAmountLabel->setText("Refund Amount:");

    refundAmount = new QLineEdit(this, "refundAmount");
    refundAmount->setAlignment(Qt::AlignRight);

    processButton = new QPushButton(this, "processButton");
    processButton->setText("&Process");
    connect(processButton, SIGNAL(clicked()), this, SLOT(processRefund()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);

    Q3GridLayout *gl = new Q3GridLayout(2, 2);

    int curRow = 0;
    gl->addWidget(customerIDLabel,      curRow, 0);
    gl->addWidget(customerID,           curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(customerNameLabel,    curRow, 0);
    gl->addWidget(customerName,         curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(cardListLabel,        curRow, 0);
    gl->addWidget(cardList,             curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(refundAmountLabel,    curRow, 0);
    gl->addWidget(refundAmount,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);

    ml->addStretch(1);

    ml->addWidget(new HorizLine(this, "hLine"), 0);
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(processButton, 0);
    bl->addWidget(cancelButton, 0);
    ml->addLayout(bl, 0);

    // More variable inits
    cardListIDX = NULL;
    myCustID = 0;

    myMsgBoxTitle = "Issue Credit Card Refund";
}

/**
 * ~CCRefund()
 *
 * Destructor.
 */
CCRefund::~CCRefund()
{
}

/**
 * setCustomerID()
 *
 * Sets the customer ID to use for this.  Loads the customer data
 * and fills in the widget controls.
 */
void CCRefund::setCustomerID(long custID)
{
    ADB             DB;
    CustomersDB     CDB;
    CCTransDB       CCDB;
    AutoPaymentsDB  APDB;
    int             doIt = 0;
    bool            foundIt;
    CreditCardList  cards;

    myCustID = custID;

    // If we're visible, hide.
    hide();

    QApplication::setOverrideCursor(Qt::waitCursor);
	CDB.get(myCustID);
    QApplication::restoreOverrideCursor();
	if (!CDB.getFloat("CurrentBalance")) {
        if (QMessageBox::warning(this, myMsgBoxTitle, "The customer's balance is zero.\nDo you still want to issue a refund?", "&Yes", "&No", 0, 1) == 0) doIt = 1;
	} else {
        if (CDB.getFloat("CurrentBalance") > 0.0) {
            if (QMessageBox::warning(this, myMsgBoxTitle, "The customers does not have a credit balance.\nDo you still want to issue a refund?", "&Yes", "&No", 0, 1) == 0) doIt = 1;
        } else {
            doIt = 1;
        }
	}

    if (!doIt) delete this;

    customerID->setText(CDB.getStr("CustomerID"));
    customerName->setText(CDB.getStr("FullName"));

    QApplication::setOverrideCursor(Qt::waitCursor);

    // Get the credit card list
    cards = getCreditCardList(myCustID);

    QApplication::restoreOverrideCursor();
    if (cards.isEmpty()) {
        QMessageBox::critical(this, myMsgBoxTitle, "No valid credit cards were found for this customer.\n\nUnable to issue a refund.");
        delete this;
        return;
    }

    // Walk through the card list, set the display strings and fill our list.
    CreditCardRecord *card;
    for (card = cards.first(); card; card = cards.next()) {
        cardList->insertItem(card->listText, card->idx);
    }
    

    // Finally, if our customer has a negative balance, default the
    // refund amount to be that balance.
    if (CDB.getFloat("CurrentBalance") < 0.00) {
        QString tmpBal;
        tmpBal = tmpBal.sprintf("%.2f", CDB.getFloat("CurrentBalance") * -1.0);
        refundAmount->setText(tmpBal);
    }
    


    // Resize the window now that we've filled the widgets and show it.
    resize(sizeHint());
    show();
}

/**
 * processRefund()
 *
 * Processes the credit card refund.
 */
void CCRefund::processRefund()
{
    // Validate the balance.
    float   amount = refundAmount->text().toFloat();
    if (amount <= 0.0) {
        QMessageBox::critical(this, myMsgBoxTitle, "Unable to process refund.\n\nInvalid refund amount.");
        return;
    }

    // Okay, we have a valid card and we have a valid amount.  Issue the refund.
    // FIXME:  This should be in a library somewhere.
    processButton->setEnabled(false);

    // One last confirmation before proceeding.
    ConfirmBox  *cb = new ConfirmBox(this);
    cb->setTitle("Confirm Refund");
    QString tmpText = "Confirm that you wish to refund $";
    tmpText += refundAmount->text();
    tmpText += "\non the ";
    tmpText += cardList->currentText();
    tmpText += "\nfor ";
    tmpText += customerName->text();
    cb->setText(tmpText);
    cb->setConfirmText("Check this box to confirm the transaction");
    if (cb->exec() == QDialog::Rejected) {
        processButton->setEnabled(true);
        return;
    }

    emit(setStatus("Connecting to Monetra server..."));

    MCVE_CONN   mcvec;
    long        mcveid;
    int         ret_code;
    char        mcveHost[1024];

    MCVE_InitEngine("CAFile.pem");
    MCVE_InitConn(&mcvec);

    strcpy(mcveHost, cfgVal("MCVEHost"));



    QApplication::setOverrideCursor(Qt::waitCursor);
    if (!MCVE_SetIP(&mcvec, mcveHost, atoi(cfgVal("MCVEPort")))) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QMessageBox::critical(this, myMsgBoxTitle, "Unable to process refund.\n\nUnable to connect to MCVE server.");
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }
    
    // We put it into blocking mode since bad things might happen if we
    // don't.  This way, once we get something back from Monetra, we're
    // 100% sure that transaction went through one way or another.
    // the server at once, then we'll loop through until we're finished.
    if (!MCVE_SetBlocking(&mcvec, 1)) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QMessageBox::critical(this, myMsgBoxTitle, "Unable to set Monetra into blocking mode.");
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }

    // Set a maximum timeout per transaction of 30 seconds.
    if (!MCVE_SetTimeout(&mcvec, 30)) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QMessageBox::critical(this, myMsgBoxTitle, "Unable to set Monetra transaction timeout.");
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }

    // Finally, connect to the server
    if (!MCVE_Connect(&mcvec)) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QString tmpStr;
        tmpStr.sprintf("Connection to Monetra Server failed.\n%s", MCVE_ConnectionError(&mcvec));
        QMessageBox::critical(this, myMsgBoxTitle, tmpStr);
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }

    emit(setStatus("Processing card refund..."));

    CCRefundCardRecord  *card = &cardListIDX[cardList->currentItem()];

    // Turn the address into a max of 5 numbers only.
    card->addr.replace(QRegExp("[^0-9]"), "");
    if (card->addr.length() > 5) card->addr = card->addr.left(5);

    // Same for zip code.
    if (card->zip.length() > 5) card->zip = card->zip.left(5);
    
    // Setup a new Monetra transaction
    mcveid = MCVE_TransNew(&mcvec);

    // Transaction details.
    MCVE_TransParam(&mcvec, mcveid, MC_USERNAME, cfgVal("MCVEUser"));
    MCVE_TransParam(&mcvec, mcveid, MC_PASSWORD, cfgVal("MCVEPass"));
    MCVE_TransParam(&mcvec, mcveid, MC_TRANTYPE, MC_TRAN_RETURN);
    MCVE_TransParam(&mcvec, mcveid, MC_ACCOUNT,  card->cardNo.ascii());
    MCVE_TransParam(&mcvec, mcveid, MC_EXPDATE,  card->expDate.ascii());
    MCVE_TransParam(&mcvec, mcveid, MC_AMOUNT,   amount);
    //MCVE_TransParam(&mcvec, mcveid, MC_PTRANNUM, CCTB.getLong("InternalID"));
    MCVE_TransParam(&mcvec, mcveid, MC_STREET,   card->addr.ascii());
    MCVE_TransParam(&mcvec, mcveid, MC_ZIP,      card->zip.ascii());
    if (card->ccv.length()) {
        MCVE_TransParam(&mcvec, mcveid, MC_CV,   card->ccv.ascii());
    }

    // Send the transaction
    if (!MCVE_TransSend(&mcvec, mcveid)) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QString tmpStr;
        tmpStr.sprintf("Transaction failed.\n%s", MCVE_ConnectionError(&mcvec));
        QMessageBox::critical(this, myMsgBoxTitle, tmpStr);
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }

    ret_code = MCVE_ReturnCode(&mcvec, mcveid);

    if (ret_code != MCVE_AUTH) {
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        processButton->setEnabled(true);
        QString tmpStr;
        tmpStr.sprintf("Transaction failed.\n%s", MCVE_ConnectionError(&mcvec));
        QMessageBox::critical(this, myMsgBoxTitle, tmpStr);
        MCVE_DestroyConn(&mcvec);
        MCVE_DestroyEngine();
        return;
    }

    emit(setStatus("Updating account..."));
    // Save the credit card transaction
    CCTransDB   CCDB;
    CCDB.setValue("CustomerID",     myCustID);
    CCDB.setValue("EnteredBy",      curUser().userName);
    CCDB.setValue("TransDate",      QDate::currentDate().toString("yyyy-MM-dd").ascii());
    CCDB.setValue("Name",           card->cardholder.ascii());
    CCDB.setValue("Address",        card->addr.ascii());
    CCDB.setValue("ZIP",            card->zip.ascii());
    CCDB.setValue("BPSExported",    1);
    CCDB.setValue("BPSDate",        timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
    CCDB.setValue("ASExported",     1);
    CCDB.setValue("ASEDate",        timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
    CCDB.setValue("Confirmation",   0);
    CCDB.setValue("Amount",         (float)amount * -1.0);
    CCDB.setValue("TaxAmount",      (float)0.0);
    CCDB.setValue("CardType",       card->cardType);
    CCDB.setValue("ExpDate",        card->expDate.ascii());
    CCDB.setValue("CardNo",         card->cardNo.ascii());
    CCDB.setValue("MCVE_ID",        mcveid);
    CCDB.setValue("MCVE_Status",    ret_code);
    CCDB.setValue("MCVE_BatchID",   MCVE_TransactionBatch(&mcvec, mcveid));
    CCDB.setValue("MCVE_TTID",      MCVE_TransactionItem(&mcvec, mcveid));
    CCDB.setValue("Status",         TAACC_AUTH);
    CCDB.ins();

    // Now the AR entry.
    AcctsRecv   AR;
    QString     memo("Credit Card Refund");
    AR.ARDB->setValue("TransDate",      timeToStr(rightNow(), YYYY_MM_DD));
    AR.ARDB->setValue("CustomerID",     myCustID);
    AR.ARDB->setValue("TransType",      TRANSTYPE_CCREFUND);
    AR.ARDB->setValue("Price",          amount);
    AR.ARDB->setValue("Amount",         amount);
    AR.ARDB->setValue("Memo",           memo.ascii());
    AR.setGLAccount(atoi(cfgVal("UndepositedCCAcct")));
    AR.SaveTrans();

    emit(customerUpdated(myCustID));
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, myMsgBoxTitle, "Transaction succeeded.");

    MCVE_DestroyConn(&mcvec);
    MCVE_DestroyEngine();
    delete this;
}

/**
 * cancelClicked()
 *
 * Closes the widget.
 */
void CCRefund::cancelClicked()
{
    delete this;
}


// vim: expandtab
