/*
** $Id: BI_Prices.cpp,v 1.2 2003/12/24 01:08:54 marc Exp $
**
***************************************************************************
**
** BI_Prices - Billable Items Pricing Page
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
** $Log: BI_Prices.cpp,v $
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "BI_Prices.h"

#include <stdio.h>
#include <stdlib.h>

#include <BlargDB.h>
#include <ADB.h>

#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>

BI_Prices::BI_Prices
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Create our widgets.
    pricingList = new QListView(this, "pricingList");
    pricingList->addColumn("Rate Plan");
    pricingList->addColumn("Billling Cycle");
    pricingList->addColumn("Pri. Price");
    pricingList->addColumn("Sec. Price");
    pricingList->addColumn("Units");
    pricingList->addColumn("Trial");
    pricingList->setColumnAlignment(2, AlignRight);
    pricingList->setColumnAlignment(3, AlignRight);
    pricingList->setColumnAlignment(5, AlignRight);
    connect(pricingList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(listItemSelected(QListViewItem *)));

    QLabel *primaryPriceLabel = new QLabel(this, "primaryPriceLabel");
    primaryPriceLabel->setText("Primary Price:");
    primaryPriceLabel->setAlignment(AlignRight|AlignVCenter);

    primaryPrice = new QLineEdit(this, "primaryPrice");
    primaryPrice->setMaxLength(10);
    connect(primaryPrice, SIGNAL(textChanged(const QString &)), this, SLOT(primaryPriceChanged(const QString &)));

    QLabel *secondaryPriceLabel = new QLabel(this, "secondaryPriceLabel");
    secondaryPriceLabel->setText("Secondary Price:");
    secondaryPriceLabel->setAlignment(AlignRight|AlignVCenter);

    secondaryPrice = new QLineEdit(this, "secondaryPrice");
    secondaryPrice->setMaxLength(10);
    connect(secondaryPrice, SIGNAL(textChanged(const QString &)), this, SLOT(secondaryPriceChanged(const QString &)));

    QLabel *unitsListLabel = new QLabel(this, "unitsListLabel");
    unitsListLabel->setText("Units:");
    unitsListLabel->setAlignment(AlignRight|AlignVCenter);
    
    unitsList = new QComboBox(this, "unitsList");
    connect(unitsList, SIGNAL(activated(int)), this, SLOT(unitsChanged(int)));

    QLabel *trialPeriodLabel = new QLabel(this, "trialPeriodLabel");
    trialPeriodLabel->setText("Trial Period:");
    trialPeriodLabel->setAlignment(AlignRight|AlignVCenter);
    
    trialPeriod = new QSpinBox(this, "trialPeriod");
    trialPeriod->setRange(0, 90);
    trialPeriod->setSuffix(" days");
    connect(trialPeriod, SIGNAL(valueChanged(int)), this, SLOT(trialPeriodChanged(int)));

    QLabel *additionalDescriptionLabel = new QLabel(this, "additionalDescriptionLabel");
    additionalDescriptionLabel->setText("Additional Description:");
    additionalDescriptionLabel->setAlignment(AlignRight|AlignTop);
    
    additionalDescription = new QMultiLineEdit(this, "additionalDescription");
    connect(additionalDescription, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Create our layout....
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(pricingList, 1);

    // Most of the rest of the widgets will go into the grid.
    QGridLayout *gl = new QGridLayout(3, 4);
    int curRow = 0;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);

    gl->addWidget(primaryPriceLabel,        curRow, 0);
    gl->addWidget(primaryPrice,             curRow, 1);
    gl->addWidget(secondaryPriceLabel,      curRow, 2);
    gl->addWidget(secondaryPrice,           curRow, 3);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(unitsListLabel,           curRow, 0);
    gl->addWidget(unitsList,                curRow, 1);
    gl->addWidget(trialPeriodLabel,         curRow, 2);
    gl->addWidget(trialPeriod,              curRow, 3);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(additionalDescriptionLabel,curRow, 0);
    gl->addMultiCellWidget(additionalDescription,   curRow, curRow, 1, 3);
    gl->setRowStretch(curRow++, 1);

    // The grid gets stretch, too.
    ml->addLayout(gl, 1);

    // Our box layout for our list of button
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // Done with the layout.


    setItemNumber((long) 0);
    pricingList->setAllColumnsShowFocus(TRUE);
    myAddDesc = NULL;
}


BI_Prices::~BI_Prices()
{
}

/*
** setItemNumber - Sets the currently selected item number.  Loads the
**                 prices from the database and fills the list with them.
*/

void BI_Prices::setItemNumber(long newItemNumber)
{
    myItemNumber    = newItemNumber;

    QApplication::setOverrideCursor(waitCursor);
    
    pricingList->clear();

    if (newItemNumber) {
        // Fill the list with all of the rate plans and billing cycles,
        // regardless of whether or not we have something defined for 
        // them.
        ADB     BDDB;
        ADB     DB;
        char    thePrice[1024];
        char    secPrice[1024];
        char    theUnits[1024];
        char    theTrial[1024];
        char    theIntID[1024];

        // First, fill our "Units" combo box with the available units.
        unitsList->clear();
        DB.query("select distinct Units from BillablesData order by Units");
        if (DB.rowCount) while (DB.getrow()) {
            unitsList->insertItem(DB.curRow["Units"]);
        }
        
        QListViewItem *curItem;
        DB.query("select RatePlans.InternalID, RatePlans.PlanTag, BillingCycles.InternalID, BillingCycles.CycleID from RatePlans, BillingCycles order by RatePlans.PlanTag, BillingCycles.CycleID");
        if (DB.rowCount) while (DB.getrow()) {
            BDDB.query("select Price, SecondaryPrice, Units, TrialPeriod, InternalID from BillablesData where ItemNumber = %ld and RatePlanID = %ld and CycleID = %ld", myItemNumber, atol(DB.curRow[0]), atol(DB.curRow[2]));
            if (BDDB.rowCount) {
                BDDB.getrow();
                sprintf(thePrice, "%10.2f", atof(BDDB.curRow["Price"]));
                sprintf(secPrice, "%10.2f", atof(BDDB.curRow["SecondaryPrice"]));
                sprintf(theUnits, "%s", BDDB.curRow["Units"]);
                sprintf(theTrial, "%3d", atoi(BDDB.curRow["TrialPeriod"]));
                strcpy(theIntID,  BDDB.curRow["InternalID"]);
            } else {
                sprintf(thePrice, "%10.2f", 0.00);
                sprintf(secPrice, "%10.2f", 0.00);
                strcpy(theUnits, "");
                sprintf(theTrial, "%3d", 0);
                strcpy(theIntID, "0");
            }
                
            // Okay, we're here.
            curItem = new QListViewItem(pricingList, 
              DB.curRow["PlanTag"], 
              DB.curRow["CycleID"], 
              thePrice, 
              secPrice, 
              theUnits,
              theTrial,
              theIntID,
              DB.curRow[0]          //  RatePlans.InternalID
            );
            curItem->setText(8, DB.curRow[2]);  // BillingCycles.InternalID

        }
        
    } else {
    }
    saveButton->setEnabled(true);

    // listItemSelected(pricingList->currentItem());
    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected  - When the user selects a list item, this function
**                     gets called.  We fill in the prices and such into
**                     the editable spaces.
*/

void BI_Prices::listItemSelected(QListViewItem *curItem)
{
    if (curItem == NULL) return;
    QApplication::setOverrideCursor(waitCursor);
    
    QString tmpPrice;
    QString tmpSecPrice;
    QString tmpStr;
    long        tmpIntID;
    ADBTable    BDDB;
    BDDB.setTableName("BillablesData");
    
    if (myAddDesc != NULL) delete myAddDesc;
    tmpIntID = atol(curItem->key(6,0));
    if (tmpIntID) {
        BDDB.get(tmpIntID);
        myAddDesc = new char[strlen(BDDB.getStr("Description")) +2 ];
        strcpy(myAddDesc, BDDB.getStr("Description"));
    } else {
        myAddDesc = new char[2];
        strcpy(myAddDesc, "");
    }
    additionalDescription->setText(myAddDesc);
    
    tmpStr = curItem->key(2,0);
    tmpPrice = tmpStr.stripWhiteSpace();
    tmpStr = curItem->key(3,0);
    tmpSecPrice = tmpStr.stripWhiteSpace();

    primaryPrice->setText(tmpPrice);
    secondaryPrice->setText(tmpSecPrice);
    trialPeriod->setValue(atoi(curItem->key(5,0)));
    
    // Get our units and select it in the combo box.
    for (int i = 0; i < unitsList->count(); i++) {
        if (!strcmp(unitsList->text(i), curItem->key(4,0))) unitsList->setCurrentItem(i);
    }
    checkForSave();
    QApplication::restoreOverrideCursor();
}

/*
** checkForSave   - Checks to see if we should activate the save button or not.
*/

void BI_Prices::checkForSave()
{
    // First, check to see if we even have an item selected.  If we don't,
    // then set the save button to disabled and return.
    QListViewItem   *curItem = pricingList->currentItem();
    if (curItem == NULL) {
        saveButton->setEnabled(FALSE);
        return;
    } else {
        saveButton->setEnabled(true);
        return;
    }
    
    // Okay, we're not null.  Do the comparisons.
    int     needSave = 0;
    float   tmpF1;
    float   tmpF2;
    
    // Check the primary price
    tmpF1 = atof(curItem->key(2,0));
    tmpF2 = atof(primaryPrice->text());
    if (tmpF1 != tmpF2) needSave++;
    
    // The secondary price.
    tmpF1 = atof(curItem->key(3,0));
    tmpF2 = atof(secondaryPrice->text());
    if (tmpF1 != tmpF2) needSave++;
    
    // The units.
    if (strcmp(curItem->key(4,0), unitsList->text(unitsList->currentItem()))) needSave++;
    
    // The trial period.
    if (trialPeriod->cleanText().toInt() != atoi(curItem->key(5,0))) needSave++;
    
    // The additional description.
    if (strcmp(additionalDescription->text(), myAddDesc)) needSave++;
    
    saveButton->setEnabled(needSave);
}

/*
** save      - Saves the changes and updates our list.
*/

void BI_Prices::save()
{
    QListViewItem   *curItem = pricingList->currentItem();
    if (curItem != NULL) {
        char        tmpStr[1024];
        ADBTable    BDDB;
        BDDB.setTableName("BillablesData");
        
        long    tmpIntID = atol(curItem->key(6,0));
        if (tmpIntID) {
            BDDB.get(tmpIntID);
        }
        BDDB.setValue("ItemNumber",     myItemNumber);
        BDDB.setValue("RatePlanID",     atol(curItem->key(7,0)));
        BDDB.setValue("CycleID",        atol(curItem->key(8,0)));
        BDDB.setValue("TrialPeriod",    trialPeriod->cleanText().toInt());
        BDDB.setValue("Price",          atof(primaryPrice->text()));
        BDDB.setValue("SecondaryPrice", atof(secondaryPrice->text()));
        BDDB.setValue("Units",          unitsList->text(unitsList->currentItem()));
        BDDB.setValue("Description",    additionalDescription->text());
        
        if (myAddDesc != NULL) delete myAddDesc;
        myAddDesc = new char[strlen(BDDB.getStr("Description")) +2 ];
        strcpy(myAddDesc, BDDB.getStr("Description"));
        
        if (tmpIntID) BDDB.upd();
        else {
            BDDB.ins();
            curItem->setText(6, BDDB.getStr("InternalID"));
        }
        
        // Now, update our list item.
        sprintf(tmpStr, "%10.2f", BDDB.getFloat("Price"));
        curItem->setText(2, tmpStr);
        
        sprintf(tmpStr, "%10.2f", BDDB.getFloat("SecondaryPrice"));
        curItem->setText(3, tmpStr);
        
        curItem->setText(4, BDDB.getStr("Units"));
        
        sprintf(tmpStr, "%3d", BDDB.getInt("TrialPeriod"));
        curItem->setText(5, tmpStr);
        
        pricingList->setCurrentItem(curItem->itemBelow());
        pricingList->setSelected(curItem->itemBelow(), TRUE);
        
        checkForSave();
    }
}



