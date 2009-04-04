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


#include "CCBatch.h"

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Qt includes
#include <QtCore/QString>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3progressdialog.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/QApplication>
#include <QtCore/QRegExp>
#include <QtGui/QLayout>
//Added by qt3to4:
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3Frame>
#include <QtGui/QLabel>

// Blarg includes
#include <BlargDB.h>
#include <BString.h>
#include <AcctsRecv.h>
#include <ParseFile.h>
#include <FParse.h>
#include <Cfg.h>
#include <ADB.h>



CCBatch::CCBatch
(
	QWidget* parent,
	const char* name
) : TAAWidget (parent)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
	setCaption( "Credit Card Batch Processing" );

    // Create our widgets and our layout

    // The total number of cards in the batch
    QLabel  *batchSizeLabel = new QLabel(this);
    batchSizeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    batchSizeLabel->setText("Cards in batch:");

    batchSize = new QLabel(this);
    batchSize->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    batchSize->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The total batch amount
    QLabel *batchAmountLabel = new QLabel(this);
    batchAmountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    batchAmountLabel->setText("Total batch amount:");

    amount = new QLabel(this);
    amount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    amount->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The number of approved cards
    QLabel *approvedCountLabel = new QLabel(this);
    approvedCountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    approvedCountLabel->setText("Approved cards:");
    
    approvedCount = new QLabel(this);
    approvedCount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    approvedCount->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The approved total
    QLabel *approvedTotalLabel = new QLabel(this);
    approvedTotalLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    approvedTotalLabel->setText("Approved total:");
    
    approvedTotal = new QLabel(this);
    approvedTotal->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    approvedTotal->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The number of declined cards
    QLabel *declinedCountLabel = new QLabel(this);
    declinedCountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    declinedCountLabel->setText("Declined cards:");
    
    declinedCount = new QLabel(this);
    declinedCount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    declinedCount->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);

    // The declined total
    QLabel *declinedTotalLabel = new QLabel(this);
    declinedTotalLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    declinedTotalLabel->setText("Declined total:");
    
    declinedTotal = new QLabel(this);
    declinedTotal->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    declinedTotal->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);


    // Our list of transactions.
    list = new Q3ListView(this);
    list->addColumn("Charge Date");
    list->addColumn("Cust ID");
    list->addColumn("Cardholder Name");
    list->addColumn("Card Type");
    list->addColumn("Amount");
    list->addColumn("Entered By");
    list->addColumn("ID");

    list->setColumnAlignment(1, Qt::AlignRight);
    list->setColumnAlignment(4, Qt::AlignRight);
    list->setColumnAlignment(6, Qt::AlignRight);

    // Export button...
    exportButton = new QPushButton(this);
    exportButton->setText("&Export");
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportPressed()));

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
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    
    // Now, the layout for status labels.
    Q3GridLayout *stl = new Q3GridLayout(2, 6, 2);
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
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(exportButton, 0);
    bl->addWidget(finishedButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    
	totRows     = 0;
	dbRows      = 0;
	batchAmount = 0.00;
	declCount   = 0;
	declAmount  = 0.00;
	
	finishedButton->setEnabled(FALSE);
	
	list->setMultiSelection(TRUE);
	list->setAllColumnsShowFocus(TRUE);
	fillList();
	updateStatus();

    connect(list, SIGNAL(selectionChanged()), SLOT(updateStatus()));	
    connect(list, SIGNAL(doubleClicked(Q3ListViewItem *)), SLOT(custDoubleClicked(Q3ListViewItem *)));

    QApplication::restoreOverrideCursor();
}


CCBatch::~CCBatch()
{
}


/*
** fillList - Gets called on startup and fills the list with up to
**            (for now) 250 entries.
*/

void CCBatch::fillList()
{
    ADB             DB;
    CCTransDB       CCTDB;
    char            cardType[1024];
    char            theDate[64];
    QDate           today;
    int             maxCCBatchSize;

    maxCCBatchSize = atoi(cfgVal("MaxCCBatchSize"));
    if (!maxCCBatchSize) maxCCBatchSize = 250;
    
    today = QDate::currentDate();
    QDatetomyDate(theDate, today);
    
    DB.query("select InternalID from CCTrans where BPSExported = 0 and TransDate <= '%s'", theDate);
    
    dbRows = DB.rowCount;

    while (DB.getrow()) {
        CCTDB.get(atol(DB.curRow["InternalID"]));
        
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


        // Fill the list...
        new Q3ListViewItem(list, 
          CCTDB.getStr("TransDate"),
          CCTDB.getStr("CustomerID"),
          CCTDB.getStr("Name"),
          cardType,
          CCTDB.getStr("Amount"),
          CCTDB.getStr("EnteredBy"),
          DB.curRow["InternalID"]
        );
        
        batchAmount += CCTDB.getFloat("Amount");

        totRows++;
        
        if (totRows >= maxCCBatchSize) break;
    }    

}

/*
** updateStatus  - Fills in the text labels with the status information.
*/

void CCBatch::updateStatus()
{
    char    tmpstr[1024];
    
    Q3ListViewItem   *curItem;
    
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
    
    // The batch size
    sprintf(tmpstr, "%ld (of %ld)", totRows, dbRows);
    batchSize->setText(tmpstr);
    
    // The batch amount
    sprintf(tmpstr, "$%.2f", batchAmount);
    amount->setText(tmpstr);
    
    // The number of approved cards.
    sprintf(tmpstr, "%ld", totRows - declCount);
    approvedCount->setText(tmpstr);
    
    // The number of declined cards
    sprintf(tmpstr, "%ld", declCount);
    declinedCount->setText(tmpstr);
    
    // The declined amount
    sprintf(tmpstr, "$%.2f", declAmount);
    declinedTotal->setText(tmpstr);

    // The Approved amount
    sprintf(tmpstr, "$%.2f", batchAmount - declAmount);
    approvedTotal->setText(tmpstr);
}


/*
** cancelPressed - Gets called when the user selects the Cancel button.
*/

void CCBatch::cancelPressed()
{
    // We should add some validation here since we're now doing this
    // in mutiple steps...An are you sure you wish to exit if the
    // batch has been exported would be good...
    close();
}

/*
** finishedPressed - Gets called when the user selects the Finished button.
*/

void CCBatch::finishedPressed()
{
    Q3ListViewItem   *curItem;
    QDate           Today;
    char            theDate[16];
    ADB             DB;
    long            counter = 0;
    char            DeclFile[1024];
    char            RcptFile[1024];
    char            whichFile[1024];
    int             cardStatus = CCSTATUS_EXPORTED; // Exported, unknown acceptance status.
    
    Today   = QDate::currentDate();
    QDatetomyDate(theDate, Today);
    
    strcpy(DeclFile, cfgVal("CCDeclinedTemplate"));
    strcpy(RcptFile, cfgVal("CCReceiptTemplate"));
    
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //QProgressDialog progress("Saving payments and sending messages...", "Abort", dbRows, 0, "Export Credit Cards");
    //progress.show();

    curItem = list->firstChild();
    emit(setStatus("Processing credit card notices..."));
    emit(setProgress(0, totRows));
    while (curItem != NULL) {
        //FParser     VPF;
        CustomersDB CDB;

        CDB.get(atol(curItem->key(1, 1)));

        //VPF.set("PrimaryLogin", CDB.getStr("PrimaryLogin"));
        //VPF.set("CardType",     curItem->key(3,1));
        //VPF.set("AccountBalance", CDB.getStr("CurrentBalance"));
        //VPF.set("ChargeAmount",   curItem->key(4, 1));
    
        ++counter;
        emit(setProgress(counter, totRows));
        
        //progress.setProgress(++counter);
        //fprintf(stderr, "Processed %ld of %ld...", counter, dbRows);
        //progress.setLabelText(tmpStr);
        //progress.repaint();

        if (curItem->isSelected()) {
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
            AR.ARDB->setValue("TransType",  TRANSTYPE_PAYMENT);
            AR.ARDB->setValue("Price",      (float) 0.0);
            AR.ARDB->setValue("Amount",     (float) 0.0);
            AR.ARDB->setValue("Memo",       memoStr);
            AR.SaveTrans();
            cardStatus  = CCSTATUS_DECLINED;    // Declined.
        } else {
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
            AR.ARDB->setValue("TransType",  TRANSTYPE_PAYMENT);
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
        }
        
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
        QStringList addrs = CDB.getStatementEmailList();
        for ( QStringList::Iterator it = addrs.begin(); it != addrs.end(); ++it ) {
            QString toAddr = *it;
            parseEmail(whichFile, atol(curItem->key(1,1)), CDB.getStr("PrimaryLogin"), cfgVal("EmailDomain"), 0, cfgVal("StatementFromAddress"), toAddr.ascii(), "", extVars);
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
    emit(setProgress(dbRows, dbRows));

    emit finished();
    close();
}

/*
** exportPressed - Gets called when the user selects the Export button.
*/

void CCBatch::exportPressed()
{

    QString fileName = Q3FileDialog::getSaveFileName("/public/qbpro", "*.txt", this);

    if (fileName.length()) {
	    FILE *fp;
	    CCTransDB   CCTDB;
	    char        theDate[16];
	    QDate       Today;
	    char        tmpstr[1024];
	    char        addrnum[1024];
        long        counter = 0;
        QString     workStr;
	    Q3ListViewItem *curItem;
	    
	    Today = QDate::currentDate();
	    QDatetomyDate(theDate, Today);
	    
        emit(setStatus("Exporting credit cards..."));
	    fp = fopen((const char *) fileName, "w");
	    if (fp != NULL) {
            QApplication::setOverrideCursor(Qt::WaitCursor);

            // Loop through our list and export only those shown.
            emit(setProgress(0, totRows));

	        curItem = list->firstChild();
	        while (curItem != NULL) {
	            CCTDB.get(atol(curItem->key(6,1)));      // The internal ID
		        
                emit(setProgress(++counter, totRows));

				strcpy(tmpstr, CCTDB.getStr("Address"));
				for (unsigned int i = 0; i < sizeof(addrnum); i++) {
					addrnum[i] = '\0';
				}
				for (unsigned int i = 0; i < strlen(tmpstr); i++) {
					int n = tmpstr[i];
					if (isdigit(n)) {
						if (strlen(addrnum) < 5) 
						  addrnum[strlen(addrnum)] = n;
					}
				}

                // Strip any spaces out of the card number
                workStr = CCTDB.getStr("CardNo");
                workStr.replace(QRegExp(" "), "");
					
				fprintf(fp, "\"%s\",\"%s\",\"%s\",\"%.2f\",\"%s\",\"%s\",\"%s\",\"%s\",\"%.2f\"\n",
					(const char *) workStr,
					(const char *) CCTDB.getStr("ExpDate"),
					"P",		// Purchase
					CCTDB.getFloat("Amount"),
					addrnum,
					(const char *) CCTDB.getStr("ZIP"),
					(const char *) CCTDB.getStr("Name"),
					(const char *) CCTDB.getStr("CustomerID"),
					0.00
				);
				
				curItem = curItem->itemBelow();
	        }
	        
	        fclose(fp);
	        finishedButton->setEnabled(TRUE);
	        QApplication::restoreOverrideCursor();
            emit(setProgress(0, 0));
            emit(setStatus(""));

        } else {
            QMessageBox::critical(this, "Export Credit Card Batch", "Error opening file.  Export aborted.");
        }
    }
}


/*
** custDoubleClicked  - Opens a customer window.
*/

void CCBatch::custDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem) {
        emit(openCustomer(atol(curItem->key(1,0))));
    }
}


// vim: expandtab

