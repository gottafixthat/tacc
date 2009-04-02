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
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3BoxLayout>

#include <calendar.h>
#include <BlargDB.h>
#include <ADB.h>

#include "PKG_Conversion.h"

using namespace Qt;

PKG_Conversion::PKG_Conversion
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    // Create our widgets.
    autoConvert = new QCheckBox(this, "autoConvert");
    autoConvert->setText("Auto Convert");
    connect(autoConvert, SIGNAL(clicked()), this, SLOT(checkForSave()));

    // We need a button group to hold certain widgets for us.
    Q3ButtonGroup *bGroup = new Q3ButtonGroup(this, "bGroup");

    cvtDaysButton = new QRadioButton(bGroup, "cvtDaysButton");
    cvtDaysButton->setText("Convert");
    cvtDaysButton->setChecked(true);
    connect(cvtDaysButton, SIGNAL(toggled(bool)), this, SLOT(cvtDaysButtonChanged(bool)));

    daysSpinner = new QSpinBox(bGroup, "daysSpinner");
    daysSpinner->setRange(1, 999);
    daysSpinner->setSuffix(" days");
    connect(daysSpinner, SIGNAL(valueChanged(int)), this, SLOT(cvtDaysChanged(int)));

    QLabel  *daysLabel = new QLabel(bGroup, "daysLabel");
    daysLabel->setText("after creation");
    daysLabel->setAlignment(AlignLeft|AlignVCenter);

    cvtOnDateButton = new QRadioButton(bGroup, "cvtOnDateButton");
    cvtOnDateButton->setText("Convert on");
    cvtOnDateButton->setChecked(false);
    connect(cvtOnDateButton, SIGNAL(toggled(bool)), this, SLOT(cvtOnButtonChanged(bool)));

    QLabel  *cvtToLabel = new QLabel(bGroup, "cvtToLabel");
    cvtToLabel->setText("Convert to:");
    cvtToLabel->setAlignment(AlignRight|AlignVCenter);

    cvtToList = new QComboBox(false, bGroup, "cvtToList");

    QDate   tmpDate = QDate::currentDate().addDays(30);
    cvtDate = new DateInput(bGroup, tmpDate);
    connect(cvtDate, SIGNAL(dateChanged()), this, SLOT(checkForSave()));

    isPromotional = new QCheckBox(this, "isPromotional");
    isPromotional->setText("Promotional");
    connect(isPromotional, SIGNAL(toggled(bool)), this, SLOT(promotionalChanged(bool)));
    
    QLabel  *promoDateLabel = new QLabel(bGroup, "promoDateLabel");
    promoDateLabel->setText("Promotion ends on:");
    promoDateLabel->setAlignment(AlignRight|AlignVCenter);

    promoDate = new DateInput(this, tmpDate);
    connect(promoDate, SIGNAL(dateChanged()), this, SLOT(checkForSave()));

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // Create our layout now.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(autoConvert, 0);

    // The bGroup layout.
    Q3BoxLayout *bgl = new Q3BoxLayout(bGroup, Q3BoxLayout::TopToBottom, 3, 3);

    Q3BoxLayout *bgl1 = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bgl1->addWidget(cvtDaysButton, 0);
    bgl1->addWidget(daysSpinner, 0);
    bgl1->addWidget(daysLabel, 0);
    bgl1->addStretch(1);
    bgl->addLayout(bgl1, 0);

    Q3BoxLayout *bgl2 = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bgl2->addWidget(cvtOnDateButton, 0);
    bgl2->addWidget(cvtDate, 0);
    bgl2->addStretch(1);
    bgl->addLayout(bgl2, 0);

    Q3BoxLayout *bgl3 = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bgl3->addWidget(cvtToLabel, 1);
    bgl3->addWidget(cvtToList, 0);
    bgl3->addStretch(1);
    bgl->addLayout(bgl3, 0);

    ml->addWidget(bGroup, 0);

    ml->addWidget(new HorizLine(this), 0);
    ml->addWidget(isPromotional, 0);

    Q3BoxLayout  *pl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    pl->addWidget(promoDateLabel, 1);
    pl->addWidget(promoDate, 0);
    pl->addStretch(1);

    ml->addLayout(pl, 0);
    ml->addStretch(1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);

    ml->addLayout(bl, 0);

    // Done with the layout.


    myPackageID = 0;

    setPackageID((long) 0);

    
    
    cvtToIDX = NULL;
}


PKG_Conversion::~PKG_Conversion()
{
}


/*
** setPackageID - Sets the current package ID.
**                Loads the information from the database.
*/

void PKG_Conversion::setPackageID(long newPackageID)
{
    if (newPackageID) {
        ADBTable    PDB;
        ADB         DB;
        int         tmpPos = 0;
        int         tmpCur = 0;
        QDate       tmpDate = QDate::currentDate();
        PDB.setTableName("Packages");
        if (PDB.get(newPackageID)) {
            myPackageID = newPackageID;
	        autoConvert->setChecked(PDB.getInt("AutoConvert"));
	        autoConvert->setEnabled(TRUE);
	        
	        intAutoConvert = autoConvert->isChecked();
	        
	        intConvertDays = PDB.getInt("ConvertDays");
	        intConvertTo   = PDB.getLong("ConvertToID");
	        daysSpinner->setValue(intConvertDays);
	        if (intConvertDays) {
	            cvtOnDateButton->setChecked(FALSE);
	            cvtDaysButton->setChecked(TRUE);
	            
	            intConvertDate = QDate::currentDate().addDays(30);
	            
	            // Set the date in the spinbox.
	            // Set it to default at 30 days from today.
	            tmpDate = QDate::currentDate().addDays(30);
	            cvtDate->setDate(tmpDate);
	        } else {
	            cvtDaysButton->setChecked(FALSE);
	            cvtOnDateButton->setChecked(TRUE);
	            
	            // Set the date in the spinbox.
	            if (strcmp(PDB.getStr("ConvertDate"), "0000-00-00")) {
    	            tmpDate = PDB.getDate("ConvertDate");
    	        } else {
    	            tmpDate = QDate::currentDate().addDays(30);
    	        }
  	            intConvertDate = tmpDate;
	            cvtDate->setDate(tmpDate);
	        }

            // Check for promo items.
            if (intAutoConvert) {
                isPromotional->setChecked(PDB.getInt("PromoPackage"));
                intIsPromo = isPromotional->isChecked();
                if (intIsPromo) {
		            if (strcmp(PDB.getStr("PromoEnds"), "0000-00-00")) {
	    	            tmpDate = PDB.getDate("PromoEnds");
	    	        } else {
	    	            tmpDate = QDate::currentDate().addDays(30);
	    	        }
	  	            intPromoDate = tmpDate;
		            promoDate->setDate(tmpDate);
                } else {
    	            tmpDate = QDate::currentDate().addDays(30);
	  	            intPromoDate = tmpDate;
		            promoDate->setDate(tmpDate);
                }
            } else {
                isPromotional->setChecked(0);
                intIsPromo      = 0;
   	            tmpDate = QDate::currentDate().addDays(30);
  	            intPromoDate = tmpDate;
	            promoDate->setDate(tmpDate);
            }	        
	        
	        
            checkForSave();
            saveButton->setEnabled(FALSE);
            
            // Now we need to fill our list of convert to entries.
            cvtToList->clear();
            if (cvtToIDX != NULL) delete cvtToIDX;
            DB.query("select InternalID, PackageTag from Packages where Active <> 0 and InternalID <> %ld order by PackageTag", myPackageID);
            cvtToIDX = new long[DB.rowCount + 2];
            cvtToList->insertItem("None/Remove");
            cvtToIDX[tmpPos++] = (long) 0;
            
            if (DB.rowCount) while (DB.getrow()) {
                cvtToList->insertItem(DB.curRow["PackageTag"]);
                cvtToIDX[tmpPos] = atol(DB.curRow["InternalID"]);
                if (cvtToIDX[tmpPos] == intConvertTo) tmpCur = tmpPos;
                tmpPos++;
            }
            
            cvtToList->setCurrentItem(tmpCur);

        } else {
            myPackageID = 0;
        }
    } else {
        myPackageID = 0;
    }
    
    if (!myPackageID) {
        autoConvert->setChecked(FALSE);
        autoConvert->setEnabled(FALSE);
        cvtOnDateButton->setChecked(TRUE);
        cvtDaysButton->setChecked(FALSE);
        cvtOnDateButton->setEnabled(FALSE);
        cvtDaysButton->setEnabled(FALSE);
        daysSpinner->setValue(0);
        daysSpinner->setEnabled(FALSE);
        // cvtDate->setEnabled(FALSE);
        cvtToList->setCurrentItem(0);
        cvtToList->setEnabled(FALSE);
        isPromotional->setChecked(FALSE);
        isPromotional->setEnabled(FALSE);
        if (isVisible()) {
            cvtDate->setEnabled(FALSE);
            promoDate->setEnabled(FALSE);
        }
    }
}


/*
** save - Saves the widget.
*/

void PKG_Conversion::save()
{
    if (myPackageID) {
        ADBTable    PDB;
        PDB.setTableName("Packages");
    
        PDB.get(myPackageID);
        
        PDB.setValue("AutoConvert",  (int) autoConvert->isChecked());
        PDB.setValue("PromoPackage", (int) isPromotional->isChecked());
        PDB.setValue("PromoEnds",    promoDate->getDate());

        if (cvtDaysButton->isChecked()) {
            PDB.setValue("ConvertDays", atoi(daysSpinner->cleanText()));
        } else {
            PDB.setValue("ConvertDays", 0);
            PDB.setValue("ConvertDate", cvtDate->getQDate());
        }
        
        if (isPromotional->isChecked()) {
            PDB.setValue("PromoPackage", 1);
            PDB.setValue("PromoEnds", promoDate->getQDate());
        } else {
            PDB.setValue("PromoPackage", 0);
        }
        
        PDB.setValue("ConvertToID", cvtToIDX[cvtToList->currentItem()]);
        
        PDB.upd();
        
        // Reload the package info.
        setPackageID(myPackageID);
    } 
}


/*
** checkForSave - Serves two purposes.  First, it checks to see if the
**                save button should be enabled (any data has changed).
**                second, it enables or disables all of the appropriate
**                entry widgets based on other dependancies.
*/

void PKG_Conversion::checkForSave()
{


    // Check to see which buttons/widgets should be enabled.
    if (autoConvert->isChecked()) {
        // Okay, we autoconvert, so check to see what other things need
        // to be enabled or disabled...
        cvtDaysButton->setEnabled(TRUE);
        cvtOnDateButton->setEnabled(TRUE);
        isPromotional->setEnabled(TRUE);
        if (cvtDaysButton->isChecked()) { 
            daysSpinner->setEnabled(TRUE);
            cvtDate->setEnabled(FALSE);
        } else {
            daysSpinner->setEnabled(FALSE);
            cvtDate->setEnabled(TRUE);
        }
        cvtToList->setEnabled(TRUE);
        
        if (isPromotional->isChecked()) {
            promoDate->setEnabled(TRUE);
        } else {
            promoDate->setEnabled(FALSE);
        }
    } else {
        // If auto convert is not checked, everything else is disabled.
        cvtOnDateButton->setChecked(TRUE);
        cvtDaysButton->setChecked(FALSE);
        cvtOnDateButton->setEnabled(FALSE);
        cvtDaysButton->setEnabled(FALSE);
        daysSpinner->setValue(0);
        daysSpinner->setEnabled(FALSE);
        // cvtDate->setEnabled(FALSE);
        cvtToList->setCurrentItem(0);
        cvtToList->setEnabled(FALSE);
        isPromotional->setChecked(FALSE);
        isPromotional->setEnabled(FALSE);
        if (isVisible()) {
            cvtDate->setEnabled(FALSE);
            promoDate->setEnabled(FALSE);
        }
    }


    if (isVisible()) {
	    // Now, compare the data in the widget with the data that we stored
	    // to see if our save button should be enabled.
	    int     allowSave = 0;
	    if (myPackageID) {
		    
		    if (intAutoConvert != autoConvert->isChecked()) allowSave++;
		    if (intConvertDays != atoi(daysSpinner->cleanText())) allowSave++;
		    if (intConvertDate != cvtDate->getQDate()) allowSave++;
		    if (cvtToIDX != NULL) {
	            if (intConvertTo   != cvtToIDX[cvtToList->currentItem()]) allowSave++;
	        }
	        if (intIsPromo     != isPromotional->isChecked()) allowSave++;
	        if (intPromoDate   != promoDate->getQDate()) allowSave++;
        }
        
        saveButton->setEnabled(allowSave);
    }

}


// vim: expandtab
