/*
** $Id$
**
***************************************************************************
**
** CCMonetra - Runs a credit card batch through the Monetra Payment
**             processing gateway.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2003, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CCMonetra.cpp,v $
** Revision 1.5  2004/01/22 01:09:43  marc
** Updated to use the TRANSTYPE_CCPAYMENT account
**
** Revision 1.4  2004/01/13 01:15:35  marc
** Added CV Processing if available.
**
** Revision 1.3  2004/01/13 00:07:20  marc
** Added AVS to the transactions
**
** Revision 1.2  2003/12/30 18:44:32  marc
** Updated to use the new setProgressRT call for realtime updates.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CCMonetra.h"

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Qt includes
#include <qstring.h>
#include <qmessagebox.h>
#include <qprogressdialog.h>
#include <qlistview.h>
#include <qfiledialog.h>
#include <qeventloop.h>
#include <qregexp.h>
#include <qlayout.h>

// Blarg includes
#include <BlargDB.h>
#include <BString.h>
#include <AcctsRecv.h>
#include <ParseFile.h>
#include <FParse.h>
#include <Cfg.h>
#include <ADB.h>

// MCVE
#include <mcve.h>



CCMonetra::CCMonetra
(
	QWidget* parent,
	const char* name
) : TAAWidget ( parent, name )
{
    QApplication::setOverrideCursor(waitCursor);
	setCaption( "Credit Card Batch Processing (Monetra)" );

    QString tmpReceipts = cfgVal("GenerateCCReceipts");
    doReceipts = tmpReceipts.toInt();

    // Create our widgets and our layout

    // The total number of cards in the batch
    QLabel  *batchSizeLabel = new QLabel(this);
    batchSizeLabel->setAlignment(AlignRight|AlignVCenter);
    batchSizeLabel->setText("Cards in batch:");

    batchSize = new QLabel(this);
    batchSize->setAlignment(AlignRight|AlignVCenter);
    batchSize->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    // The total batch amount
    QLabel *batchAmountLabel = new QLabel(this);
    batchAmountLabel->setAlignment(AlignRight|AlignVCenter);
    batchAmountLabel->setText("Total batch amount:");

    amount = new QLabel(this);
    amount->setAlignment(AlignRight|AlignVCenter);
    amount->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    // The number of approved cards
    QLabel *approvedCountLabel = new QLabel(this);
    approvedCountLabel->setAlignment(AlignRight|AlignVCenter);
    approvedCountLabel->setText("Approved cards:");
    
    approvedCount = new QLabel(this);
    approvedCount->setAlignment(AlignRight|AlignVCenter);
    approvedCount->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    // The approved total
    QLabel *approvedTotalLabel = new QLabel(this);
    approvedTotalLabel->setAlignment(AlignRight|AlignVCenter);
    approvedTotalLabel->setText("Approved total:");
    
    approvedTotal = new QLabel(this);
    approvedTotal->setAlignment(AlignRight|AlignVCenter);
    approvedTotal->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    // The number of declined cards
    QLabel *declinedCountLabel = new QLabel(this);
    declinedCountLabel->setAlignment(AlignRight|AlignVCenter);
    declinedCountLabel->setText("Declined cards:");
    
    declinedCount = new QLabel(this);
    declinedCount->setAlignment(AlignRight|AlignVCenter);
    declinedCount->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    // The declined total
    QLabel *declinedTotalLabel = new QLabel(this);
    declinedTotalLabel->setAlignment(AlignRight|AlignVCenter);
    declinedTotalLabel->setText("Declined total:");
    
    declinedTotal = new QLabel(this);
    declinedTotal->setAlignment(AlignRight|AlignVCenter);
    declinedTotal->setFrameStyle(QFrame::Panel|QFrame::Sunken);


    // Our list of transactions.
    list = new QListView(this);
    list->addColumn("Charge Date");
    list->addColumn("Cust ID");
    list->addColumn("Cardholder Name");
    list->addColumn("Card Type");
    list->addColumn("Amount");
    list->addColumn("Entered By");
    list->addColumn("ID");
    list->addColumn("Status");

    list->setColumnAlignment(1, AlignRight);
    list->setColumnAlignment(4, AlignRight);
    list->setColumnAlignment(6, AlignRight);

    // Export button...
    startButton = new QPushButton(this);
    startButton->setText("&Start Processing");
    connect(startButton, SIGNAL(clicked()), this, SLOT(processPressed()));

    // Finished button...
    finishedButton = new QPushButton(this);
    finishedButton->setText("&Finished");
    connect(finishedButton, SIGNAL(clicked()), this, SLOT(finishedPressed()));

    // Cancel button...
    cancelButton = new QPushButton(this);
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    
    // Now, create our layout, and add the widgets into it.

    // Our main layout first.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    
    // Now, the layout for status labels.
    QGridLayout *stl = new QGridLayout(2, 6, 2);
    stl->addWidget(batchSizeLabel,      0,0);
    stl->addWidget(batchSize,           0,1);
    stl->addWidget(batchAmountLabel,    1,0);
    stl->addWidget(amount,              1,1);

    stl->addWidget(approvedCountLabel,  0,2);
    stl->addWidget(approvedCount,       0,3);
    stl->addWidget(approvedTotalLabel,  1,2);
    stl->addWidget(approvedTotal,       1,3);

    stl->addWidget(declinedCountLabel,  0,4);
    stl->addWidget(declinedCount,       0,5);
    stl->addWidget(declinedTotalLabel,  1,4);
    stl->addWidget(declinedTotal,       1,5);

    ml->addLayout(stl, 0);
    ml->addWidget(list, 1);

    // Add the action buttons
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(startButton, 0);
    bl->addWidget(finishedButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    
	totRows     = 0;
	dbRows      = 0;
	batchAmount = 0.00;
	apprCount   = 0;
	apprAmount  = 0.00;
	declCount   = 0;
	declAmount  = 0.00;
	
	finishedButton->setEnabled(FALSE);
	
	list->setMultiSelection(TRUE);
	list->setAllColumnsShowFocus(TRUE);
	fillList();
	updateStatus();

    connect(list, SIGNAL(selectionChanged()), SLOT(updateStatus()));	
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), SLOT(custDoubleClicked(QListViewItem *)));

    QApplication::restoreOverrideCursor();

}


CCMonetra::~CCMonetra()
{
}


/*
** fillList - Gets called on startup and fills the list with up to
**            (for now) 250 entries.
*/

void CCMonetra::fillList()
{
    ADB             DB;
    CCTransDB       CCTDB;
    char            cardType[1024];
    char            theDate[64];
    QDate           today;
    int             maxCCMonetraSize;
    char            mcveStatus[1024];

    maxCCMonetraSize = atoi(cfgVal("MCVEMaxTrans"));
    if (!maxCCMonetraSize) maxCCMonetraSize = 250;

    today = QDate::currentDate();
    QDatetomyDate(theDate, today);
    
    DB.query("select InternalID from CCTrans where BPSExported = 0 and TransDate <= '%s' LIMIT %d", theDate, maxCCMonetraSize);
    
    dbRows = DB.rowCount;

    while (DB.getrow()) {
        CCTDB.get(atol(DB.curRow["InternalID"]));

        // Check for this transaction in MCVE
        
        
        // Card Type
        switch (CCTDB.getInt("CardType")) {
            case CCTYPE_MC:
                sprintf(cardType, "Mastercard");
                break;
            
            case CCTYPE_VISA:
                sprintf(cardType, "Visa");
                break;
                
            case CCTYPE_AMEX:
                sprintf(cardType, "American Express");
                break;
                
            default:
                sprintf(cardType, "???");
                break;
        }

        // Our own status code.
        switch(CCTDB.getInt("Status")) {
            case TAACC_SENT:
                strcpy(mcveStatus, "Sent");
                break;

            case TAACC_AUTH:
                strcpy(mcveStatus, "Approved");
                break;

            case TAACC_DECL:
                strcpy(mcveStatus, "Declined");
                break;

            case TAACC_INVALID:
                strcpy(mcveStatus, "Invalid");
                break;

            case TAACC_EXPIRED:
                strcpy(mcveStatus, "Expired");
                break;

            case TAACC_RETRY:
                strcpy(mcveStatus, "MCVE Retry");
                break;

            case TAACC_UNPROCESSED:
            default:
                strcpy(mcveStatus, "Unprocessed");
                break;
        }


        // Fill the list...
        new QListViewItem(list, 
          CCTDB.getStr("TransDate"),
          CCTDB.getStr("CustomerID"),
          CCTDB.getStr("Name"),
          cardType,
          CCTDB.getStr("Amount"),
          CCTDB.getStr("EnteredBy"),
          DB.curRow["InternalID"],
          mcveStatus
        );
        
        batchAmount += CCTDB.getFloat("Amount");

        totRows++;
        emit(setProgressRT(totRows, dbRows));
    }    

}

/*
** updateStatus  - Fills in the text labels with the status information.
*/

void CCMonetra::updateStatus()
{
    char    tmpstr[1024];
    
    /*
    CCTransDB       CCTDB;
    QListViewItem   *curItem;
    
    apprCount  = 0;
    apprAmount = 0.00;
    declCount  = 0;
    declAmount = 0.00;
    
    curItem = list->firstChild();
    while (curItem != NULL) {
        if (curItem->isSelected()) {
            declCount++;
            declAmount += atof(curItem->key(4,1));
        }
        curItem = curItem->itemBelow();
    }
    */
    
    // The batch size
    sprintf(tmpstr, "%ld (of %ld)", totRows, dbRows);
    batchSize->setText(tmpstr);
    
    // The batch amount
    sprintf(tmpstr, "$%.2f", batchAmount);
    amount->setText(tmpstr);
    
    // The number of approved cards.
    sprintf(tmpstr, "%ld", apprCount);
    approvedCount->setText(tmpstr);
    
    // The number of declined cards
    sprintf(tmpstr, "%ld", declCount);
    declinedCount->setText(tmpstr);
    
    // The declined amount
    sprintf(tmpstr, "$%.2f", declAmount);
    declinedTotal->setText(tmpstr);

    // The Approved amount
    sprintf(tmpstr, "$%.2f", apprAmount);
    approvedTotal->setText(tmpstr);
}


/*
** cancelPressed - Gets called when the user selects the Cancel button.
*/

void CCMonetra::cancelPressed()
{
    // We should add some validation here since we're now doing this
    // in mutiple steps...An are you sure you wish to exit if the
    // batch has been exported would be good...
    close();
}

/*
** finishedPressed - Gets called when the user selects the Finished button.
*/

void CCMonetra::finishedPressed()
{
    QListViewItem   *curItem;
    QDate           Today;
    char            theDate[16];
    //char            tmpStr[1024];
    ADB             DB;
    long            counter = 0;
    char            DeclFile[1024];
    char            RcptFile[1024];
    char            whichFile[1024];
    int             cardStatus = CCSTATUS_EXPORTED; // Exported, unknown acceptance status.
    int             mcveStatus;
    CCTransDB       CCTDB;
    
    Today   = QDate::currentDate();
    QDatetomyDate(theDate, Today);
    
    strcpy(DeclFile, cfgVal("CCDeclinedTemplate"));
    strcpy(RcptFile, cfgVal("CCReceiptTemplate"));
    
    QApplication::setOverrideCursor(waitCursor);
    //QProgressDialog progress("Saving payments and sending messages...", "Abort", dbRows, 0, "Export Credit Cards");
    //progress.show();

    curItem = list->firstChild();
    emit(setStatus("Processing credit card notices..."));
    emit(setProgressRT(0, totRows));
    while (curItem != NULL) {
        //FParser     VPF;
        CustomersDB CDB;

        CCTDB.get(atol(curItem->key(6,1)));      // The internal ID
        CDB.get(atol(curItem->key(1, 1)));

        //VPF.set("PrimaryLogin", CDB.getStr("PrimaryLogin"));
        //VPF.set("CardType",     curItem->key(3,1));
        //VPF.set("AccountBalance", CDB.getStr("CurrentBalance"));
        //VPF.set("ChargeAmount",   curItem->key(4, 1));
    
        ++counter;
        emit(setProgressRT(counter, totRows));
        
        //progress.setProgress(++counter);
        //fprintf(stderr, "Processed %ld of %ld...", counter, dbRows);
        //progress.setLabelText(tmpStr);
        //progress.repaint();

        mcveStatus = CCTDB.getInt("Status");

        if (mcveStatus == TAACC_DECL || mcveStatus == TAACC_INVALID ||
                mcveStatus == TAACC_EXPIRED) {
        //if (curItem->isSelected()) {
            // Card was declined.
            // Process a declined message and put a declined message
            // in their register.
            /*
            FILE    *ofp;
            sprintf(tmpStr, "/var/spool/taamail/ccdeclined-%s.txt", (const char *) curItem->key(1, 1));
            ofp = fopen(tmpStr, "w");
            VPF.parseFile(DeclFile, ofp);
            fclose(ofp);
            */

            //fprintf(stderr, "Calling parseEmail()\n");
            strcpy(whichFile, DeclFile);
            //parseEmail("CCDeclined", atol(curItem->key(1,1)), CDB.getStr("PrimaryLogin"), "", 1, "Avvanta Accounting <accounting@avvanta.com>", "", "", extVars);

            AcctsRecv   AR;
            char        memoStr[512];
            float       tmpAmount;
            
            tmpAmount = atof(curItem->key(4, 1));
            sprintf(memoStr, "Credit card (%s) payment for $%.2f declined", (const char *) curItem->key(3,1), tmpAmount);
            
            AR.ARDB->setValue("TransDate",  theDate);
            AR.ARDB->setValue("CustomerID", atol(curItem->key(1, 1)));
            AR.ARDB->setValue("TransType",  TRANSTYPE_CCPAYMENT);
            AR.ARDB->setValue("Price",      (float) 0.0);
            AR.ARDB->setValue("Amount",     (float) 0.0);
            AR.ARDB->setValue("Memo",       memoStr);
            AR.SaveTrans();
            cardStatus  = CCSTATUS_DECLINED;    // Declined.
        } else if (mcveStatus == TAACC_AUTH) {
            // Card was accepted.
            // Add in the transaction into the customer's register.
            //fprintf(stderr, "Creating an AR connection...\n");
            AcctsRecv   AR;
            char        memoStr[512];
            float       tmpAmount;
            
            tmpAmount = atof(curItem->key(4, 1)) * -1.0;
            sprintf(memoStr, "Payment Received (%s)", (const char *) curItem->key(3,1));
            
            AR.ARDB->setValue("TransDate",  theDate);
            AR.ARDB->setValue("CustomerID", atol(curItem->key(1, 1)));
            AR.ARDB->setValue("TransType",  TRANSTYPE_CCPAYMENT);
            AR.ARDB->setValue("Price",      tmpAmount);
            AR.ARDB->setValue("Amount",     tmpAmount);
            AR.ARDB->setValue("Memo",       memoStr);
            //fprintf(stderr, "Inserting the AR transaction...\n");
            AR.SaveTrans();
            
            // Now, process a receipt for them.
            // Reload the customer data first so we have the correct 
            // balance.
            //fprintf(stderr, "Getting customer record...\n");
            strcpy(whichFile, RcptFile);
            /*
	        CDB.get(atol(curItem->key(1, 1)));
	        VPF.set("AccountBalance", CDB.getStr("CurrentBalance"));
            //fprintf(stderr, "Parsing reciept file...\n");
            //VPF.parseFile(RcptFile);
            //fprintf(stderr, "Storing reciept file...\n");
            sprintf(tmpStr, "/var/spool/taamail/ccreceipt-%s.txt", (const char *) curItem->key(1, 1));
            
            FILE    *ofp;
            ofp = fopen(tmpStr, "w");
            VPF.parseFile(RcptFile, ofp);
            fclose(ofp);
            */

            //VPF.writeParsedFile(tmpStr);
            cardStatus  = CCSTATUS_APPROVED;    // Accepted.
            //fprintf(stderr, "Done.\n");
        } else {
            // Retry, timeout, etc.  Put the card back in the batch
            cardStatus = CCSTATUS_PENDING;

        }
        
        if (cardStatus != CCSTATUS_PENDING) {
            CDB.get(atol(curItem->key(1, 1)));

            //fprintf(stderr, "Setting extra parse variables....\n");
            // Set our extra variables to pass to parseEmail
            SElement    *extVars = new(SElement[10]);
            int j = 0;
            extVars[j].Name  = new(char[256]);
            strcpy(extVars[j].Name, "PrimaryLogin");
            extVars[j].Value = new(char[strlen(CDB.getStr("PrimaryLogin"))+10]);
            strcpy(extVars[j].Value, CDB.getStr("PrimaryLogin"));
            j++;

            extVars[j].Name  = new(char[256]);
            strcpy(extVars[j].Name, "CardType");
            extVars[j].Value = new(char[strlen(curItem->key(3,1))+10]);
            strcpy(extVars[j].Value, curItem->key(3,1));
            j++;
            
            extVars[j].Name  = new(char[256]);
            strcpy(extVars[j].Name, "AccountBalance");
            extVars[j].Value = new(char[strlen(CDB.getStr("CurrentBalance"))+10]);
            strcpy(extVars[j].Value, CDB.getStr("CurrentBalance"));
            j++;

            extVars[j].Name  = new(char[256]);
            strcpy(extVars[j].Name,"ChargeAmount");
            extVars[j].Value = new(char[strlen(curItem->key(4,1))+10]);
            strcpy(extVars[j].Value, curItem->key(4,1));
            j++;
            
            extVars[j].Name  = new(char[256]);
            strcpy(extVars[j].Name, "");
            extVars[j].Value = new(char[256]);
            strcpy(extVars[j].Value, "");
                
            //fprintf(stderr, "Calling parseEmail()\n");
            // Finally, send it to them.
            // Get the list of email addresses to send it to.
            if (doReceipts) {
                QStringList addrs = CDB.getStatementEmailList();
                for ( QStringList::Iterator it = addrs.begin(); it != addrs.end(); ++it ) {
                    QString toAddr = *it;
                    parseEmail(whichFile, atol(curItem->key(1,1)), CDB.getStr("PrimaryLogin"), cfgVal("EmailDomain"), 0, cfgVal("StatementFromAddress"), toAddr.ascii(), "", extVars);
                }
            }
            
            emit(customerUpdated(atol(curItem->key(6,1))));
        }

        // Regardless of whether or not the card was accepted,
        // we update it in the batch so we don't run it again.
        DB.dbcmd("update CCTrans set BPSExported = %d, BPSEDate = '%s' where InternalID = %ld",
          cardStatus,
          theDate,
          atol(curItem->key(6,1))
        );
            
        curItem = curItem->itemBelow();
    }
    QApplication::restoreOverrideCursor();

    emit(setStatus(""));
    emit(setProgressRT(dbRows, dbRows));

    emit finished();
    close();
}

/*
** processPressed - This prompts the user to make sure they want to do this
**                  and if so, sends all of the credit cards to the
**                  credit card processing server for processing.
*/

void CCMonetra::processPressed()
{

    if (QMessageBox::warning(this, "Process Credit Cards", "Once this operation is started, it cannot be aborted.\n\nAre you sure you want to start processing?", "&Yes", "&No", 0, 1) == 1) {
        return;
    }
    
    // MCVE Variables
    MCVE_CONN       mcvec;
    long            mcveid;
    //long            transStat;
    int             ret_code;
    char            mcveHost[1024];
    char            tmpStr[4096];
    QListViewItem   *curItem;
    long            counter = 0;
    CCTransDB       CCTDB;
    char            addrnum[1024];
    char            avszip[1024];
    QString         workStr;

    strcpy(mcveHost, cfgVal("MCVEHost"));

    startButton->setEnabled(false);
    cancelButton->setEnabled(false);

    // Init our MCVE connection
    MCVE_InitEngine("CAfile.pem");
    MCVE_InitConn(&mcvec);
    
    // Initialize the connection.
    if (!MCVE_SetIP(&mcvec, mcveHost, atoi(cfgVal("MCVEPort")))) {
        QMessageBox::critical(this, "Process Credit Cards", "Unable to connect to the MCVE server.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // We put it into blocking mode since bad things might happen if we
    // don't.  This way, once we get something back from Monetra, we're
    // 100% sure that transaction went through one way or another.
    // the server at once, then we'll loop through until we're finished.
    if (!MCVE_SetBlocking(&mcvec, 1)) {
        QMessageBox::critical(this, "Process Credit Cards", "Unable to set MCVE into blocking mode.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // Set a maximum timeout per transaction of 30 seconds.
    if (!MCVE_SetTimeout(&mcvec, 30)) {
        QMessageBox::critical(this, "Process Credit Cards", "Unable to set MCVE transaction timeout.");
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // Finally, connect to the server
    if (!MCVE_Connect(&mcvec)) {
        sprintf(tmpStr, "Connection to MCVE Server failed.\n%s", MCVE_ConnectionError(&mcvec));
        QMessageBox::critical(this, "Process Credit Cards", tmpStr);
        MCVE_DestroyConn(&mcvec);
        return;
    }

    // If we made it to this point, we have a connection and can start
    // pumping in transactions.
    emit(setStatus("Sending cards to MCVE engine..."));
    QApplication::setOverrideCursor(waitCursor);

    curItem = list->firstChild();
    while (curItem != NULL) {
        list->ensureItemVisible(curItem);
        list->setCurrentItem(curItem);
        CCTDB.get(atol(curItem->key(6,1)));      // The internal ID
        
        strcpy(tmpStr, CCTDB.getStr("Address"));
        for (unsigned int i = 0; i < sizeof(addrnum); i++) {
            addrnum[i] = '\0';
        }
        for (unsigned int i = 0; i < strlen(tmpStr); i++) {
            int n = tmpStr[i];
            if (isdigit(n)) {
                if (strlen(addrnum) < 5) 
                  addrnum[strlen(addrnum)] = n;
            }
        }

        // Copy the zip code and truncate it at 5 digits.
        strcpy(avszip, CCTDB.getStr("ZIP"));
        if (strlen(avszip) > 5) {
            avszip[5] = '\0';
        }


        // Strip any spaces out of the card number
        workStr = CCTDB.getStr("CardNo");
        workStr.replace(QRegExp(" "), "");
            
        // Send it to MCVE
        // Setup a new transaction
        mcveid = MCVE_TransNew(&mcvec);
        fprintf(stderr, "MCVE Transaction ID: %ld, InternalID: %ld\n", mcveid, CCTDB.getLong("InternalID"));
        CCTDB.setValue("Status",  TAACC_SENT);
        CCTDB.setValue("MCVE_ID", mcveid);
        CCTDB.upd();

        // Set the transaction details
        MCVE_TransParam(&mcvec, mcveid, MC_USERNAME, cfgVal("MCVEUser"));
        MCVE_TransParam(&mcvec, mcveid, MC_PASSWORD, cfgVal("MCVEPass"));
        MCVE_TransParam(&mcvec, mcveid, MC_TRANTYPE, MC_TRAN_SALE);
        MCVE_TransParam(&mcvec, mcveid, MC_ACCOUNT,  (const char *) workStr);
        MCVE_TransParam(&mcvec, mcveid, MC_EXPDATE,  (const char *) CCTDB.getStr("ExpDate"));
        MCVE_TransParam(&mcvec, mcveid, MC_AMOUNT,   CCTDB.getFloat("Amount"));
        MCVE_TransParam(&mcvec, mcveid, MC_PTRANNUM, CCTDB.getLong("InternalID"));
        MCVE_TransParam(&mcvec, mcveid, MC_STREET,   addrnum);
        MCVE_TransParam(&mcvec, mcveid, MC_ZIP,      avszip);
        // Do we have CV Info for this card?
        if (strlen(CCTDB.getStr("SecurityCode"))) {
            MCVE_TransParam(&mcvec, mcveid, MC_CV,   CCTDB.getStr("SecurityCode"));
        }

        curItem->setText(7, "Sent");
        emit(setProgressRT(counter, totRows));
        // Send the transaction
        if (!MCVE_TransSend(&mcvec, mcveid)) {
            sprintf(tmpStr, "Transaction failed.");
            QMessageBox::critical(this, "Process Credit Cards", tmpStr);
        } else {
            // Successfully sent.  Update the database with the MCVEID
            curItem->setText(7, "Sent");
        }

        ret_code = MCVE_ReturnCode(&mcvec, CCTDB.getLong("MCVE_ID"));
        CCTDB.setValue("MCVE_Status", ret_code);
        switch(ret_code) {
            case MCVE_RETRY:
                CCTDB.setValue("Status", TAACC_RETRY);
                curItem->setText(7, "Retry");
                break;

            case MCVE_TIMEOUT:
                CCTDB.setValue("Status", TAACC_RETRY);
                curItem->setText(7, "Timeout");
                break;

            case MCVE_AUTH:
                apprCount++;
                apprAmount += CCTDB.getFloat("Amount");
                CCTDB.setValue("MCVE_TTID", MCVE_TransactionItem(&mcvec, CCTDB.getLong("MCVE_ID")));
                CCTDB.setValue("MCVE_BatchID", MCVE_TransactionBatch(&mcvec, CCTDB.getLong("MCVE_ID")));
                CCTDB.setValue("Status", TAACC_AUTH);
                curItem->setText(7, "Approved");
                break;

            case MCVE_PKUP:
                declCount++;
                declAmount += CCTDB.getFloat("Amount");
                CCTDB.setValue("Status", TAACC_INVALID);
                curItem->setText(7, "Invalid");
                break;

            case MCVE_FAIL:
            case MCVE_DENY:
            case MCVE_CALL:
            case MCVE_DUPL:
            case MCVE_SETUP:
                declCount++;
                declAmount += CCTDB.getFloat("Amount");
                CCTDB.setValue("Status", TAACC_DECL);
                curItem->setText(7, "Declined");
                break;

            default:
                CCTDB.setValue("Status", TAACC_RETRY);
                curItem->setText(7, "Unknown");
                break;

        }
        CCTDB.upd();
        //curItem->setText(7, CCTDB.getStr("MCVE_Status"));
        if (ret_code == MCVE_SUCCESS) {
            // This was a successful transaction.
            
        }
        
        //MCVE_DeleteTrans(&conn, mcveid);

        updateStatus();
        emit(setProgressRT(++counter, totRows));
        curItem = curItem->itemBelow();

    }
    


    emit(setStatus(""));
    QApplication::restoreOverrideCursor();
    finishedButton->setEnabled(true);
    fprintf(stderr, "Deleting connection to MCVE....\n");
    MCVE_DestroyConn(&mcvec);
    MCVE_DestroyEngine();

}


/*
** custDoubleClicked  - Opens a customer window.
*/

void CCMonetra::custDoubleClicked(QListViewItem *curItem)
{
    if (curItem) {
        emit(openCustomer(atol(curItem->key(1,0))));
    }
}

