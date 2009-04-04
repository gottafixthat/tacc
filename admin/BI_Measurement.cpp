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
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <BlargDB.h>
#include <ADB.h>

#include "BI_Measurement.h"

using namespace Qt;

BI_Measurement::BI_Measurement
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
{
    // Create all our widgets.
    measureList = new Q3ListView(this, "measureList");
    measureList->addColumn("Rate Plan");
    measureList->addColumn("Billing Cycle");
    measureList->addColumn("Measurement");
    measureList->addColumn("Price");
    measureList->addColumn("Base");
    measureList->addColumn("Increment");
    measureList->setColumnAlignment(3, AlignRight);
    measureList->setColumnAlignment(4, AlignRight);
    measureList->setColumnAlignment(5, AlignRight);
    connect(measureList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(listItemSelected(Q3ListViewItem *)));

    isMeasured = new QCheckBox(this, "isMeasured");
    isMeasured->setText("Measured");
    connect(isMeasured, SIGNAL(clicked()), this, SLOT(checkForSave()));

    QLabel *measuredFromLabel = new QLabel(this, "measuredFromLabel");
    measuredFromLabel->setText("Measurement:");
    measuredFromLabel->setAlignment(AlignRight|AlignVCenter);

    measuredFrom = new QComboBox(false, this, "measuredFrom");
    connect(measuredFrom, SIGNAL(activated(int)), this, SLOT(measuredFromChanged(int)));

    QLabel *additionalPriceLabel = new QLabel(this, "additionalPriceLabel");
    additionalPriceLabel->setText("Additional Price:");
    additionalPriceLabel->setAlignment(AlignRight|AlignVCenter);

    additionalPrice = new QLineEdit(this, "additionalPrice");
    connect(additionalPrice, SIGNAL(textChanged(const QString &)), this, SLOT(priceChanged(const QString &)));

    QLabel *baseQtyLabel = new QLabel(this, "baseQtyLabel");
    baseQtyLabel->setText("Base Quantity:");
    baseQtyLabel->setAlignment(AlignRight|AlignVCenter);

    baseQty = new QLineEdit(this, "baseQty");
    connect(baseQty, SIGNAL(textChanged(const QString &)), this, SLOT(baseQtyChanged(const QString &)));

    isIncremental = new QCheckBox(this, "isIncremental");
    isIncremental->setText("Incremental");
    connect(isIncremental, SIGNAL(clicked()), this, SLOT(isIncrementalChanged()));

    QLabel *minIncrementLabel = new QLabel(this, "minIncrementLabel");
    minIncrementLabel->setText("Min. Increment:");
    minIncrementLabel->setAlignment(AlignRight|AlignVCenter);

    minIncrement = new QLineEdit(this, "minIncrement");
    connect(minIncrement, SIGNAL(textChanged(const QString &)), this, SLOT(minIncrementChanged(const QString &)));


    QLabel *additionalDescriptionLabel = new QLabel(this, "additionalDescriptionLabel");
    additionalDescriptionLabel->setText("Additional Description:");
    additionalDescriptionLabel->setAlignment(AlignRight|AlignTop);

    additionalDescription = new Q3MultiLineEdit(this, "additionalDescription");
    connect(additionalDescription, SIGNAL(textChanged()), this, SLOT(additionalDescriptionChanged()));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // All done creating widgets, create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(measureList, 1);

    // Create a grid which will hold most of the other widgets.
    Q3GridLayout *gl = new Q3GridLayout(4, 4);
    int curRow = 0;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 1);

    gl->addWidget(isMeasured,                       curRow, 0);
    gl->addWidget(measuredFromLabel,                curRow, 2);
    gl->addWidget(measuredFrom,                     curRow, 3);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(additionalPriceLabel,             curRow, 0);
    gl->addWidget(additionalPrice,                  curRow, 1);
    gl->addWidget(baseQtyLabel,                     curRow, 2);
    gl->addWidget(baseQty,                          curRow, 3);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(isIncremental,                    curRow, 1);
    gl->addWidget(minIncrementLabel,                curRow, 2);
    gl->addWidget(minIncrement,                     curRow, 3);
    gl->setRowStretch(curRow++, 0);

    gl->addWidget(additionalDescriptionLabel,       curRow, 0);
    gl->addMultiCellWidget(additionalDescription,   curRow, curRow, 1, 3);
    gl->setRowStretch(curRow++, 1);

    // The grid gets stretch, too.
    ml->addLayout(gl, 1);

    // Create the layout for our row of button.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // Done with our layout.


    setItemNumber((long) 0);
    myAddDesc = NULL;
    
    // Fill our measurement list.
    measuredFrom->clear();
    measuredFrom->insertItem("Seconds (Modems)");
    measuredFrom->insertItem("Minutes (Modems)");
    measuredFrom->insertItem("Hours (Modems)");
    measuredFrom->insertItem("Bytes (Traffic)");
    measuredFrom->insertItem("KBytes (Traffic)");
    measuredFrom->insertItem("MBytes (Traffic)");
    measuredFrom->insertItem("GBytes (Traffic)");
    
    measureList->setAllColumnsShowFocus(TRUE);
}


BI_Measurement::~BI_Measurement()
{
}

/*
** setItemNumber - Sets our internal item number and fills the list with
**                 our rate plans and billing cycles.
*/

void BI_Measurement::setItemNumber(long newItemNumber)
{
    myItemNumber = newItemNumber;
    
    QApplication::setOverrideCursor(waitCursor);
    
    measureList->clear();
    
    if (newItemNumber) {
        // Fill our list with the measurement information for each combination
        // of rate plan and billing cycle, regardless of whether or not
        // there are actual entries for them.
        ADB         RPBCDB;
        ADB         BDDB;
        ADBTable    BDB;
        ADB         DB;
        BDB.setTableName("Billables");
        Q3ListViewItem   *curItem;
        
        int         myIsMeasured;
        char        theMeasurement[1024];
        char        thePrice[1024];
        char        theBase[1024];
        char        theIncrement[1024];
        char        theIntID[1024];
        
        // Load all of the Rate Plans and Billing Cycles and loop through them
        // one by one.
        // This query will load all of the rate plans and billing cycles in one shot.
        RPBCDB.query("select RatePlans.InternalID, RatePlans.PlanTag, BillingCycles.InternalID, BillingCycles.CycleID from RatePlans, BillingCycles order by RatePlans.PlanTag, BillingCycles.CycleID");
        if (RPBCDB.rowCount) while (RPBCDB.getrow()) {
            BDDB.query("select Measured, Measurement, BaseQuantity, AdditionalPrice, Incremental, MinIncrement, InternalID from BillablesData where ItemNumber = %ld and RatePlanID = %ld and CycleID = %ld", myItemNumber, atol(RPBCDB.curRow[0]), atol(RPBCDB.curRow[2]));
            if (BDDB.rowCount) {
                BDDB.getrow();
                myIsMeasured = atoi(BDDB.curRow["Measured"]);
                if (myIsMeasured) {
                    strcpy(theMeasurement, measuredFrom->text(atoi(BDDB.curRow["Measurement"])));
                    sprintf(thePrice, "%10.2f", atof(BDDB.curRow["AdditionalPrice"]));
                    strcpy(theBase, BDDB.curRow["BaseQuantity"]);
                    if (atoi(BDDB.curRow["Incremental"])) {
                        strcpy(theIncrement, BDDB.curRow["MinIncrement"]);
                    } else {
                        strcpy(theIncrement, "N/A");
                    }
                } else {
                    strcpy(theMeasurement, "N/A");
                    strcpy(thePrice, "N/A");
                    strcpy(theBase, "N/A");
                    strcpy(theIncrement, "N/A");
                }
                strcpy(theIntID, BDDB.curRow["InternalID"]);
            } else {
                // No BillablesData found for this RatePlan/BillingCycle Combo.
                strcpy(theMeasurement, "N/A");
                strcpy(thePrice, "N/A");
                strcpy(theBase, "N/A");
                strcpy(theIncrement, "N/A");
                strcpy(theIntID, "0");
            }
            curItem = new Q3ListViewItem(measureList,
              RPBCDB.curRow["PlanTag"],         // RatePlans PlanTag
              RPBCDB.curRow["CycleID"],         // Billing Cycles CycleID
              theMeasurement,
              thePrice,
              theBase,
              theIncrement,
              theIntID,
              RPBCDB.curRow[0]                  // RatePlans.InternalID
            );
            curItem->setText(8, RPBCDB.curRow[2]);  // BillingCycles.InternalID
        }
    }
    listItemSelected(NULL);
    
    QApplication::restoreOverrideCursor();
}

/*
** listItemSelected - Gets called when the user selects an item from the
**                    meausreList.  We fill in the editable fields with 
**                    the selected item's details.
*/

void BI_Measurement::listItemSelected(Q3ListViewItem *curItem)
{
    // If the item is NULL, disable all of the edit widgets and clear
    // them.
    if (curItem == NULL) {
        isMeasured->setEnabled(FALSE);
        measuredFrom->setEnabled(FALSE);
        additionalPrice->setEnabled(FALSE);
        baseQty->setEnabled(FALSE);
        isIncremental->setEnabled(FALSE);
        minIncrement->setEnabled(FALSE);
        additionalDescription->setEnabled(FALSE);
        saveButton->setEnabled(true);   
        isMeasured->setChecked(FALSE);
        measuredFrom->setCurrentItem(0);
        additionalPrice->setText("");
        baseQty->setText("");
        isIncremental->setChecked(FALSE);
        minIncrement->setText("");
        additionalDescription->setText("");
        return;
    }
    
    QApplication::setOverrideCursor(waitCursor);
    
    long        myIntID;
    ADBTable    BDDB;
    BDDB.setTableName("BillablesData");
    
    myIntID = atol(curItem->key(6,0));
    
    if (myAddDesc == NULL) delete myAddDesc;
    
    if (myIntID) {
        BDDB.get(myIntID);
        isMeasured->setEnabled(TRUE);
        isMeasured->setChecked(BDDB.getInt("Measured"));
        measuredFrom->setCurrentItem(BDDB.getInt("Measurement"));
        additionalPrice->setText(BDDB.getStr("AdditionalPrice"));
        baseQty->setText(BDDB.getStr("BaseQuantity"));
        isIncremental->setChecked(BDDB.getInt("Incremental"));
        minIncrement->setText(BDDB.getStr("MinIncrement"));
        additionalDescription->setText(BDDB.getStr("MeasuredDesc"));
        
        myIsMeasured        = BDDB.getInt("Measured");
        myMeasuredFrom      = BDDB.getInt("Measurement");
        myAdditionalPrice   = BDDB.getFloat("AdditionalPrice");
        myBaseQuantity      = BDDB.getLLong("BaseQuantity");
        myIsIncremental     = BDDB.getInt("Incremental");
        myMinIncrement      = BDDB.getLLong("MinIncrement");
        
    } else {
        isMeasured->setChecked(FALSE);
        isMeasured->setEnabled(TRUE);
        measuredFrom->setCurrentItem(0);
        additionalPrice->setText("");
        baseQty->setText("");
        isIncremental->setChecked(FALSE);
        minIncrement->setText("");
        additionalDescription->setText("");
        
        myIsMeasured        = 0;
        myMeasuredFrom      = 0;
        myAdditionalPrice   = 0.0;
        myBaseQuantity      = 0;
        myIsIncremental     = 0;
        myMinIncrement      = 0;
    }
    
    myAddDesc = new char[strlen(additionalDescription->text())+2];
    strcpy(myAddDesc, additionalDescription->text());
    
    // Enable/disable all of our appropriate buttons and fields.
    checkForSave();

    QApplication::restoreOverrideCursor();
}

/*
** checkForSave  - Enables/disables buttons based on whether or not
**                 the user has changed anything.
*/

void BI_Measurement::checkForSave()
{
    // First, make sure that there is an item selected, if we don't have
    // one selected, set the save button to disabled and return.
    Q3ListViewItem   *curItem = measureList->currentItem();
    if (curItem == NULL) {
        saveButton->setEnabled(false);
        return;
    } else {
        saveButton->setEnabled(true);
        //return;
    }
    
    int     needSave = 0;

    // First, make sure that the appropriate things are enabled/disabled
    // based on what the user has selected.
    if (isMeasured->isChecked()) {
        measuredFrom->setEnabled(true);
        additionalPrice->setEnabled(true);
        baseQty->setEnabled(true);
        isIncremental->setEnabled(true);
        if (isIncremental->isChecked()) {
            minIncrement->setEnabled(true);
        } else {
            minIncrement->setEnabled(false);
        }
        additionalDescription->setEnabled(true);
    } else {
        measuredFrom->setEnabled(false);
        additionalPrice->setEnabled(false);
        baseQty->setEnabled(false);
        isIncremental->setEnabled(false);
        minIncrement->setEnabled(false);
        additionalDescription->setEnabled(false);
    }
    
    if (isMeasured->isChecked() != myIsMeasured) needSave++;
    if (measuredFrom->currentItem() != myMeasuredFrom) needSave++;
    if (atof(additionalPrice->text()) != myAdditionalPrice) needSave++;
    if (atoll(baseQty->text()) != myBaseQuantity) needSave++;
    if (isIncremental->isChecked() != myIsIncremental) needSave++;
    //if (strcmp((const char *) additionalDescription->text(), myAddDesc)) needSave++;
    
    saveButton->setEnabled(needSave);
}


/*
** save  - Saves the changes into the database.
*/

void BI_Measurement::save()
{
    Q3ListViewItem   *curItem = measureList->currentItem();
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
        BDDB.setValue("Measured",       (int) isMeasured->isChecked());
        BDDB.setValue("Measurement",    measuredFrom->currentItem());
        BDDB.setValue("BaseQuantity",   atoll(baseQty->text()));
        BDDB.setValue("AdditionalPrice",atof(additionalPrice->text()));
        BDDB.setValue("Incremental",    (int) isIncremental->isChecked());
        BDDB.setValue("MinIncrement",   atoll(minIncrement->text()));
        BDDB.setValue("MeasuredDesc",   additionalDescription->text());
        
        if (tmpIntID) BDDB.upd();
        else {
            BDDB.ins();
            curItem->setText(6, BDDB.getStr("InternalID"));
        }
        
        // Now, update our internal variables...
        isMeasured->setChecked(BDDB.getInt("Measured"));
        measuredFrom->setCurrentItem(BDDB.getInt("Measurement"));
        additionalPrice->setText(BDDB.getStr("AdditionalPrice"));
        baseQty->setText(BDDB.getStr("BaseQuantity"));
        isIncremental->setChecked(BDDB.getInt("Incremental"));
        minIncrement->setText(BDDB.getStr("MinIncrement"));
        additionalDescription->setText(BDDB.getStr("MeasuredDesc"));
        
        myIsMeasured        = BDDB.getInt("Measured");
        myMeasuredFrom      = BDDB.getInt("Measurement");
        myAdditionalPrice   = BDDB.getFloat("AdditionalPrice");
        myBaseQuantity      = BDDB.getLLong("BaseQuantity");
        myIsIncremental     = BDDB.getInt("Incremental");
        myMinIncrement      = BDDB.getLLong("MinIncrement");
        if (myAddDesc != NULL) delete myAddDesc;
        myAddDesc = new char[strlen(BDDB.getStr("MeasuredDesc"))+2];
        strcpy(myAddDesc, BDDB.getStr("MeasuredDesc"));
        
        // Now that all of our variables have been reset, update the
        // list item itself.
        if (myIsMeasured) {
            curItem->setText(2, measuredFrom->text(myMeasuredFrom));

            sprintf(tmpStr, "%10.2f", myAdditionalPrice);
            curItem->setText(3, tmpStr);
        
            sprintf(tmpStr, "%qd", myBaseQuantity);
            curItem->setText(4, tmpStr);
        
            if (myIsIncremental) {
                sprintf(tmpStr, "%qd", myMinIncrement);
            } else {
                strcpy(tmpStr, "N/A");
            }
            curItem->setText(5, tmpStr);
        } else {
            curItem->setText(2, "N/A");
            curItem->setText(3, "N/A");
            curItem->setText(4, "N/A");
        }
    
    }
}


// vim: expandtab
