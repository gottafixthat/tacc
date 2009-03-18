/**
 * CCRefund.h - A widget that allows a manager to issue a refund
 * to a previously used credit card.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qregexp.h>

#include <mcve.h>

#include <Cfg.h>
#include <BlargDB.h>
#include <ADB.h>
#include <TAAStructures.h>
#include <TAATools.h>
#include <AcctsRecv.h>
#include <CCRefund.h>

/**
 * CCRefund()
 *
 * Constructor.
 */
CCRefund::CCRefund(QWidget *parent, const char *name) :
    TAAWidget(parent, name)
{
    setCaption("Issue Credit Card Refund");

    QLabel *customerIDLabel = new QLabel(this, "customerIDLabel");
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);
    customerIDLabel->setText("Customer ID:");

    customerID = new QLabel(this, "customerID");
    customerID->setAlignment(AlignLeft|AlignVCenter);

    QLabel *customerNameLabel = new QLabel(this, "customerNameLabel");
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);
    customerNameLabel->setText("Customer Name:");

    customerName = new QLabel(this, "customerName");
    customerName->setAlignment(AlignLeft|AlignVCenter);

    QLabel *cardListLabel = new QLabel(this, "cardListLabel");
    cardListLabel->setAlignment(AlignRight|AlignVCenter);
    cardListLabel->setText("Card to refund:");

    cardList = new QComboBox(false, this, "cardList");

    QLabel *refundAmountLabel = new QLabel(this, "refundAmountLabel");
    refundAmountLabel->setAlignment(AlignRight|AlignVCenter);
    refundAmountLabel->setText("Refund Amount:");

    refundAmount = new QLineEdit(this, "refundAmount");
    refundAmount->setAlignment(AlignRight);

    processButton = new QPushButton(this, "processButton");
    processButton->setText("&Process");
    connect(processButton, SIGNAL(clicked()), this, SLOT(processRefund()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    QGridLayout *gl = new QGridLayout(2, 2);

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
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
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

    myCustID = custID;

    // If we're visible, hide.
    hide();

    QApplication::setOverrideCursor(waitCursor);
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

    QApplication::setOverrideCursor(waitCursor);
    // Figure out how many cards we could *potentially* have.
    // Sucks to do two queries, but they should be small.
    int idxCount = 0;
    DB.query("select InternalID from CCTrans where CustomerID = %ld order by TransDate desc", myCustID);
    idxCount += DB.rowCount;
    // Now for the second query, we'll pull from the AutoPayments table and just loop
    // through it.  We'll re-run the first query for the CCTrans table afterward
    DB.query("select InternalID from AutoPayments where CustomerID = %ld and PaymentType = 1 order by AuthorizedOn desc", myCustID);
    idxCount += DB.rowCount;
    // Create the index pointer with a bit of padding
    cardListIDX = new CCRefundCardRecord[idxCount+2];
    cardListCount = 0;

    // Now walk through the AutoPayment list, adding any cards we find.
    if (DB.rowCount) while(DB.getrow()) {
        APDB.get(atol(DB.curRow["InternalID"]));
        // Check our list for this card
        foundIt = false;
        if (cardListCount) {
            for (int i = 0; i < cardListCount; i++) {
                if (!cardListIDX[i].cardNo.compare(cardListIDX[i].cardNo, APDB.getStr("AcctNo"))) foundIt = true;
            }
        }
        if (!foundIt) {
            // Check to see if the card is expired.
            QDate   expDate;
            QDate   today = QDate::currentDate();
            myDateToQDate(APDB.getStr("ExpDate"), expDate);
            if (today.daysTo(expDate) >= 0) {
                // Add it to the list.
                cardListIDX[cardListCount].cardholder   = APDB.getStr("Name");
                cardListIDX[cardListCount].addr         = APDB.getStr("Address");
                cardListIDX[cardListCount].zip          = APDB.getStr("ZIP");
                cardListIDX[cardListCount].cardType     = APDB.getInt("CardType");
                cardListIDX[cardListCount].cardNo       = APDB.getStr("AcctNo");
                cardListIDX[cardListCount].expDate      = expDate.toString("MMyy").ascii();
                cardListIDX[cardListCount].ccv          = APDB.getStr("SecurityCode");

                // Add it to the combo box.
                cardListCount++;
            }
        }
    }


    // Do the CCTrans query again, getting whatever cards are there.
    DB.query("select InternalID from CCTrans where CustomerID = %ld order by TransDate desc", myCustID);
    if (DB.rowCount) while(DB.getrow()) {
        CCDB.get(atol(DB.curRow["InternalID"]));
        // Check our list for this card
        foundIt = false;
        if (cardListCount) {
            for (int i = 0; i < cardListCount; i++) {
                if (!cardListIDX[i].cardNo.compare(cardListIDX[i].cardNo, CCDB.getStr("CardNo"))) foundIt = true;
            }
        }
        if (!foundIt) {
            // Check to see if the card is expired.
            QString tmpExp = CCDB.getStr("ExpDate");
            int     month = tmpExp.left(2).toInt();
            int     year  = tmpExp.right(2).toInt();
            if (year < 70) year += 2000;
            else year += 1900;
            QDate   expDate;
            expDate.setYMD(year, month, 1);
            expDate.setYMD(year, month, expDate.daysInMonth());
            QDate   today = QDate::currentDate();
            if (today.daysTo(expDate) >= 0) {
                // Add it to the list.
                cardListIDX[cardListCount].cardholder   = CCDB.getStr("Name");
                cardListIDX[cardListCount].addr         = CCDB.getStr("Address");
                cardListIDX[cardListCount].zip          = CCDB.getStr("ZIP");
                cardListIDX[cardListCount].cardType     = CCDB.getInt("CardType");
                cardListIDX[cardListCount].cardNo       = CCDB.getStr("CardNo");
                cardListIDX[cardListCount].expDate      = CCDB.getStr("ExpDate");
                cardListIDX[cardListCount].ccv          = CCDB.getStr("SecurityCode");

                // Add it to the combo box.
                cardListCount++;
            }
        }
    }

    QApplication::restoreOverrideCursor();
    if (!cardListCount) {
        QMessageBox::critical(this, myMsgBoxTitle, "No valid credit cards were found for this customer.\n\nUnable to issue a refund.");
        delete this;
        return;
    }

    // Walk through the card list, set the display strings and fill our list.
    for (int i = 0; i < cardListCount; i++) {
        switch (cardListIDX[i].cardType) {
            case CCTYPE_MC:
                cardListIDX[i].cardTypeStr = "Mastercard";
                break;
            case CCTYPE_VISA:
                cardListIDX[i].cardTypeStr = "Visa";
                break;
            case CCTYPE_AMEX:
                cardListIDX[i].cardTypeStr = "AmEx";
                break;
            case CCTYPE_DINERS:
                cardListIDX[i].cardTypeStr = "Diners";
                break;
            case CCTYPE_DISC:
                cardListIDX[i].cardTypeStr = "Discover";
                break;
            default:
                cardListIDX[i].cardTypeStr = "Unknown";
                break;
        }
        QString dispStr;
        dispStr = cardListIDX[i].cardTypeStr;
        dispStr += " ";
        dispStr += cardListIDX[i].cardNo.left(1);
        dispStr += "...";
        dispStr += cardListIDX[i].cardNo.right(4);
        dispStr += " Exp ";
        dispStr += cardListIDX[i].expDate;
        cardList->insertItem(dispStr);
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
    emit(setStatus("Connecting to Monetra server..."));

    MCVE_CONN   mcvec;
    long        mcveid;
    int         ret_code;
    char        mcveHost[1024];

    MCVE_InitEngine("CAFile.pem");
    MCVE_InitConn(&mcvec);

    strcpy(mcveHost, cfgVal("MCVEHost"));

    processButton->setEnabled(false);
    QApplication::setOverrideCursor(waitCursor);
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

