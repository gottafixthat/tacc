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

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>

#include "PKG_Pricing.h"

using namespace Qt;

PKG_Pricing::PKG_Pricing
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Setup our widgets.
    pricingList = new Q3ListView(this, "pricingList");
    int curCol = 0;
    pricingList->addColumn("Rate Plan");
    pricingList->setColumnAlignment(curCol++, AlignLeft);
    pricingList->addColumn("BillingCycle");
    pricingList->setColumnAlignment(curCol++, AlignLeft);
    pricingList->addColumn("Setup Item");
    pricingList->setColumnAlignment(curCol++, AlignLeft);
    pricingList->addColumn("Price");
    pricingList->setColumnAlignment(curCol++, AlignRight);
    connect(pricingList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(listItemSelected(Q3ListViewItem *)));

    QLabel  *setupItemListLabel = new QLabel(this, "setupItemListLabel");
    setupItemListLabel->setText("Setup Item:");
    setupItemListLabel->setAlignment(AlignRight|AlignVCenter);

    setupItemList = new QComboBox(false, this, "setupItemList");
    connect(setupItemList, SIGNAL(activated(int)), this, SLOT(setupItemChanged(int)));

    QLabel  *itemPriceLabel = new QLabel(this, "itemPriceLabel");
    itemPriceLabel->setText("Price:");
    itemPriceLabel->setAlignment(AlignRight|AlignVCenter);

    itemPrice = new QLineEdit(this, "itemPrice");
    itemPrice->setMaxLength(16);
    connect(itemPrice, SIGNAL(textChanged(const QString &)), this, SLOT(priceChanged(const QString &)));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));


    // Create our layout
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(pricingList, 1);

    Q3GridLayout *gl = new Q3GridLayout(2, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setRowStretch(0, 0);
    gl->setRowStretch(1, 0);
    gl->addWidget(setupItemListLabel,           0, 0);
    gl->addWidget(setupItemList,                0, 1);
    gl->addWidget(itemPriceLabel,               1, 0);
    gl->addWidget(itemPrice,                    1, 1);

    ml->addLayout(gl, 0);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // Finished with the layout



    setPackageID((long) 0);
    
    setupItemIDX = NULL;
    
    // Fill our setup item list.
    ADB     DB;
    int     tmpPos = 0;
    DB.query("select ItemNumber, ItemID from Billables where ItemType > 1 order by ItemID");
    setupItemIDX = new long[DB.rowCount+2];
    setupItemList->clear();
    setupItemList->insertItem("None");
    setupItemIDX[tmpPos++] = (long) 0;
    if (DB.rowCount) while (DB.getrow()) {
        setupItemIDX[tmpPos++] = atol(DB.curRow["ItemNumber"]);
        setupItemList->insertItem(DB.curRow["ItemID"]);
    }
    
    pricingList->setAllColumnsShowFocus(TRUE);
}


PKG_Pricing::~PKG_Pricing()
{
}



/*
** setPackageID - Loads the pricing information for the specified
**                package ID.
*/

void PKG_Pricing::setPackageID(long newPackageID)
{
    myPackageID = newPackageID;

    QApplication::setOverrideCursor(waitCursor);
    
    pricingList->clear();

    myPackageID = newPackageID;    
    if (newPackageID) {
        // Fill the list with all of the rate plans and billing cycles,
        // regardless of whether or not we have something defined for 
        // them.
        ADB         RPDB;
        ADB         BCDB;
        ADB         PDDB;
        ADBTable    BDB;
        BDB.setTableName("Billables");
        char        thePrice[1024];
        char        theSetupItem[1024];
        char        theSetupItemID[1024];
        char        theIntID[1024];
        RPDB.query("select InternalID, PlanTag from RatePlans order by PlanTag");
        if (RPDB.rowCount) while (RPDB.getrow()) {
            BCDB.query("select InternalID, CycleID from BillingCycles order by CycleID");
            if (BCDB.rowCount) while (BCDB.getrow()) {
                PDDB.query("select Price, SetupItem, InternalID from PackagesData where PackageID = %ld and RatePlanID = %ld and CycleID = %ld", myPackageID, atol(RPDB.curRow["InternalID"]), atol(BCDB.curRow["InternalID"]));
                if (PDDB.rowCount) {
                    PDDB.getrow();
                    sprintf(thePrice, "%10.2f", atof(PDDB.curRow["Price"]));
                    if (atol(PDDB.curRow["SetupItem"])) {
                        BDB.get(atol(PDDB.curRow["SetupItem"]));
                        strcpy(theSetupItem, BDB.getStr("ItemID"));
                        strcpy(theSetupItemID, PDDB.curRow["SetupItem"]);
                    } else {
                        strcpy(theSetupItem, "None");
                        strcpy(theSetupItemID, "0");
                    }
                    strcpy(theIntID, PDDB.curRow["InternalID"]);
                } else {
                    sprintf(thePrice, "%10.2f", 0.00);
                    strcpy(theSetupItem, "None");
                    strcpy(theIntID, "0");
                }
            
                // Okay, we're here.
                (void) new Q3ListViewItem(pricingList, 
                  RPDB.curRow["PlanTag"], 
                  BCDB.curRow["CycleID"], 
                  theSetupItem, 
                  thePrice, 
                  theIntID,
                  RPDB.curRow["InternalID"],
                  BCDB.curRow["InternalID"],
                  theSetupItemID        // The setup item ID.
                );
            }
        }
        
    } else {
    }
    saveButton->setEnabled(TRUE);

    // listItemSelected(pricingList->currentItem());
    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected - Gets called whenever the user clicks on a list entry.
**                    We set the contents of the edit boxes when this gets 
**                    called.
*/

void PKG_Pricing::listItemSelected(Q3ListViewItem *curItem)
{
    if (curItem != NULL) {
        QString tmpQSt1;
        QString tmpQSt2;
        tmpQSt1 = curItem->key(3,0);
        tmpQSt2 = tmpQSt1.stripWhiteSpace();
        itemPrice->setText(tmpQSt2);
        itemPrice->setEnabled(TRUE);
        setupItemList->setEnabled(TRUE);
        
        // Okay, find the setup item in the setupItemList and select it.
        int finished = 0;
        int tmpPos = 0;
        long setupItem = atol(curItem->key(7,0));
        while (!finished) {
            if (setupItemIDX[tmpPos] == setupItem) {
                setupItemList->setCurrentItem(tmpPos);
                finished = 1;
            } else {
                tmpPos++;
                if (tmpPos > setupItemList->count()) finished = 1;
            }
        }
        
    } else {
        setupItemList->setEnabled(TRUE);
        // setupItemList->setCurrentItem(0);
        itemPrice->setEnabled(TRUE);
        itemPrice->setText("");
        saveButton->setEnabled(TRUE);
    }
}



/*
** checkForSave  - Checks to see if we need to save the current item.
**                 Enables/Disables the save button appropriatly.
*/

void PKG_Pricing::checkForSave()
{
    Q3ListViewItem   *curItem = pricingList->currentItem();
    if (curItem != NULL) { 
        // Very simple checks here...
        int     needSave = 0;
        if (atof(itemPrice->text()) != atof(curItem->key(3,0))) needSave++;
        
        if (setupItemIDX[setupItemList->currentItem()] != atol(curItem->key(7,0))) needSave++;
        
        saveButton->setEnabled(TRUE);
    } else {
        saveButton->setEnabled(TRUE);
    }
}


/*
** save  - Stores the current item to the database.
*/

void PKG_Pricing::save()
{
    Q3ListViewItem   *curItem = pricingList->currentItem();
    if (curItem != NULL) {
        ADBTable    PDDB;
        PDDB.setTableName("PackagesData");
        long        myIntID = atol(curItem->key(4,0));
        long        myRPID  = atol(curItem->key(5,0));
        long        myBCID  = atol(curItem->key(6,0));
        float       myPrice = atof(itemPrice->text());
        long        mySetupItem = setupItemIDX[setupItemList->currentItem()];
        char        tmpStr[1024];
        
        PDDB.setValue("InternalID", myIntID);
        PDDB.setValue("PackageID",  myPackageID);
        PDDB.setValue("RatePlanID", myRPID);
        PDDB.setValue("CycleID",    myBCID);
        PDDB.setValue("Price",      myPrice);
        PDDB.setValue("SetupItem",  mySetupItem);
        
        if (myIntID) PDDB.upd();
        else {
            PDDB.ins();
            // It was an insert, so we need to get the new internal ID
            // and store it in our list.
            sprintf(tmpStr, "%ld", PDDB.getLong("InternalID"));
            curItem->setText(4, tmpStr);
        }
        
        sprintf(tmpStr, "%10.2f", myPrice);
        curItem->setText(3, tmpStr);
        curItem->setText(2, setupItemList->text(setupItemList->currentItem()));
        sprintf(tmpStr, "%ld", mySetupItem);
        curItem->setText(7, tmpStr);
        
        pricingList->setCurrentItem(curItem->itemBelow());
        pricingList->setSelected(curItem->itemBelow(), TRUE);
    }
    checkForSave();
}

// vim: expandtab
