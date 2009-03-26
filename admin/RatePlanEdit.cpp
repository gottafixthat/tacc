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

// vim: expandtab

#include <qstring.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qlayout.h>

#include <BlargDB.h>
#include <ADB.h>
#include <TAATools.h>

#include <RatePlanEdit.h>

RatePlanEdit::RatePlanEdit(QWidget* parent, const char* name, long IntID) :
	TAAWidget( parent, name )
{
    setCaption("Edit Rate Plan");

    QLabel *planTagLabel = new QLabel(this, "planTagLabel");
    planTagLabel->setAlignment(AlignRight|AlignVCenter);
    planTagLabel->setText("Tag:");

    planTag = new QLineEdit(this, "planTag");
    planTag->setMaxLength(16);

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setAlignment(AlignRight|AlignVCenter);
    descriptionLabel->setText("Description:");

    description = new QLineEdit(this, "description");
    description->setMaxLength(80);

    // Okay, we have another semi-complex layout that fits within a 
    // qbutton group.
    // Its going to look like this:
    //
    // +- Auto Conversion -------------------------------------+
    // | ( ) Do not auto-convert                               |
    // | ( ) Convert ___ days after the account is created     |
    // | ( ) Convert on this date _________                    |
    // |                                                       |
    // |  Convert to:  [List Box of RatePlans]\/               |
    // +-------------------------------------------------------+
    //
    // To do this, we'll need to create a widget within the 
    // button group and then a layout within the widget.
    
    autoConvertGroup = new QButtonGroup(1, Horizontal, "Auto Conversion", this, "autoConvertGroup");
    autoConvertGroup->setExclusive(true);

    // The autoConvertGroup widget (acgw)
    TAAWidget *acgw = new TAAWidget(autoConvertGroup, "acgw");

    // The child widgets for the acgw widget
    noAutoConvert = new QRadioButton("Do not auto-convert", acgw, "noAutoConvert");
    noAutoConvert->setChecked(true);
    connect(noAutoConvert, SIGNAL(toggled(bool)), this, SLOT(toggleAutoConvert(bool)));

    autoConvertDays = new QRadioButton("Convert", acgw, "autoConvertDays");
    connect(autoConvertDays, SIGNAL(toggled(bool)), this, SLOT(toggleConvertDays(bool)));

    convertDaysLabel = new QLabel(acgw, "convertDaysLabel");
    convertDaysLabel->setAlignment(AlignLeft|AlignVCenter);
    convertDaysLabel->setText("days after the account is created");

    convertDays = new QLineEdit(acgw, "convertDays");
    convertDays->setMaxLength(3);

    autoConvertDate = new QRadioButton("Convert on this date", acgw, "autoConvertDate");
    connect(autoConvertDate, SIGNAL(toggled(bool)), this, SLOT(toggleConvertDate(bool)));

    convertDate = new QDateEdit(QDate::currentDate(), acgw, "convertDate");

    convertToLabel = new QLabel(acgw, "convertToLabel");
    convertToLabel->setAlignment(AlignLeft|AlignVCenter);
    convertToLabel->setText("Convert to:");
    
    convertToList = new QComboBox(false, acgw, "convertToList");

    // Insert each button into the autoConvertGroup box.
    autoConvertGroup->insert(noAutoConvert);
    autoConvertGroup->insert(autoConvertDays);
    autoConvertGroup->insert(autoConvertDate);

    // Now create the layout for within acgw
    QBoxLayout *acgwl = new QBoxLayout(acgw, QBoxLayout::TopToBottom, 3);
    acgwl->addWidget(noAutoConvert, 0);
    // We need a separate layout for each of the next three widget sets
    QBoxLayout *acdaysl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    acdaysl->addWidget(autoConvertDays, 0);
    acdaysl->addWidget(convertDays, 0);
    acdaysl->addWidget(convertDaysLabel, 1);
    acgwl->addLayout(acdaysl, 0);
    
    QBoxLayout *acdatel = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    acdatel->addWidget(autoConvertDate, 0);
    acdatel->addWidget(convertDate, 0);
    acdatel->addStretch(1);
    acgwl->addLayout(acdatel, 0);

    QBoxLayout *convtol = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    convtol->addWidget(convertToLabel, 1);
    convtol->addWidget(convertToList, 1);
    acgwl->addSpacing(10);
    acgwl->addLayout(convtol, 0);
    acgwl->addStretch(1);

    // Now create the promotional plan widgets
    promoPlan = new QCheckBox(this, "promoPlan");
    promoPlan->setText("Promotional Plan");
    connect(promoPlan, SIGNAL(toggled(bool)), this, SLOT(togglePromoPlan(bool)));

    promoEndsOnLabel = new QLabel(this, "promoEndsOnLabel");
    promoEndsOnLabel->setAlignment(AlignRight|AlignVCenter);
    promoEndsOnLabel->setText("Promotion Ends On:");

    promoEndsOn = new QDateEdit(QDate::currentDate(), this, "promoEndsOn");

    // Our save and cancel buttons
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveRatePlan()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelRatePlan()));

    // Finally, create our main layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    // A grid layout for the top two rows.
    QGridLayout *gl = new QGridLayout(2, 2);
    int curRow = 0;
    gl->addWidget(planTagLabel,     curRow, 0);
    gl->addWidget(planTag,          curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(descriptionLabel, curRow, 0);
    gl->addWidget(description,      curRow, 1);
    gl->setRowStretch(curRow, 0);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addWidget(autoConvertGroup, 0);

    // Another layout for our promo info
    QBoxLayout *pl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    pl->addStretch(1);
    pl->addWidget(promoPlan, 0);
    pl->addStretch(1);
    pl->addWidget(promoEndsOnLabel, 0);
    pl->addWidget(promoEndsOn, 0);
    pl->addStretch(1);

    ml->addLayout(pl, 0);
    ml->addStretch(1);

    // Finally our button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);


	ADB         DB;
	RatePlansDB RPDB;

	myIntID = IntID;
	planIndex = NULL;
	
	// Fill in the combo box with the plans we can convert to.
	DB.query("select InternalID, Description from RatePlans where InternalID <> %ld and AutoConvert = 0", myIntID);
	if (DB.rowCount) {
		planIndex = new(long[DB.rowCount +1]);
		int indexPtr = 0;
		while(DB.getrow()) {
			planIndex[indexPtr++] = atol(DB.curRow["InternalID"]);
			convertToList->insertItem(DB.curRow["Description"]);
		}
	} else {
		autoConvertDays->setEnabled(FALSE);
		convertDaysLabel->setEnabled(FALSE);
		autoConvertDate->setEnabled(FALSE);
	}
	
	if (myIntID) {
		setCaption( "Edit Rate Plan" );
		RPDB.get(myIntID);
		planTag->setText(RPDB.getStr("PlanTag"));
		description->setText(RPDB.getStr("Description"));
		switch (RPDB.getInt("AutoConvert")) {
			case 1:
				noAutoConvert->setChecked(FALSE);
				autoConvertDays->setChecked(TRUE);
				convertDays->setText(RPDB.getStr("ConvertDays"));
				promoPlan->setChecked(RPDB.getInt("PromoPlan"));
				if (promoPlan->isChecked()) {
                    QDate   tmpDate;
                    myDateToQDate(RPDB.getStr("PromoEnds"), tmpDate);
                    promoEndsOn->setDate(tmpDate);
					//promoEndsOn->setText(RPDB.getStr("PromoEnds"));
				}
				toggleConvertDays(TRUE);
				break;
				
			case 2:
				noAutoConvert->setChecked(FALSE);
				autoConvertDate->setChecked(TRUE);
                {
                    QDate   tmpDate;
                    myDateToQDate(RPDB.getStr("ConvertDate"), tmpDate);
                    convertDate->setDate(tmpDate);
                }
				//convertDate->setText(RPDB.getStr("ConvertDate"));
				promoPlan->setChecked(RPDB.getInt("PromoPlan"));
				if (promoPlan->isChecked()) {
                    QDate   tmpDate;
                    myDateToQDate(RPDB.getStr("PromoEnds"), tmpDate);
                    promoEndsOn->setDate(tmpDate);
					//promoEndsOn->setText(RPDB.getStr("PromoEnds"));
				}
				toggleConvertDate(TRUE);
				break;
				
			default:
				toggleAutoConvert(TRUE);
				break;
		}
	} else {
		toggleAutoConvert(TRUE);
		setCaption( "New Rate Plan" );
	}
	
	planTag->setFocus();
}


RatePlanEdit::~RatePlanEdit()
{
	if (planIndex != NULL) {
		delete(planIndex);
	}
}


/*
** toggleAutoConvert - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleAutoConvert(bool newState)
{
    if (newState) {
    	// Disable the appropriate fields since we're not auto-converting
    	convertDays->setEnabled(FALSE);
    	convertDate->setEnabled(FALSE);
    	convertToLabel->setEnabled(FALSE);
    	convertToList->setEnabled(FALSE);
    	promoPlan->setEnabled(FALSE);
	    promoEndsOnLabel->setEnabled(FALSE);
    	promoEndsOn->setEnabled(FALSE);
    }
}

/*
** toggleConvertDays - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleConvertDays(bool newState)
{
    if (newState) {
    	// Enable/Disable the appropriate fields to auto convert 
    	// in a specified number of days
    	convertDays->setEnabled(TRUE);
    	convertDate->setEnabled(FALSE);
    	convertToLabel->setEnabled(TRUE);
    	convertToList->setEnabled(TRUE);
    	promoPlan->setEnabled(TRUE);
	    promoEndsOnLabel->setEnabled(promoPlan->isChecked());
    	promoEndsOn->setEnabled(promoPlan->isChecked());
    }
}

/*
** toggleConvertDays - Toggles the availability of the various input fields,
**                     based on the radio button that the user has selected.
**
*/

void RatePlanEdit::toggleConvertDate(bool newState)
{
    if (newState) {
    	// Enable/Disable the appropriate fields to auto convert 
    	// in a specified number of days
    	convertDays->setEnabled(FALSE);
    	convertDate->setEnabled(TRUE);
    	convertToLabel->setEnabled(TRUE);
    	convertToList->setEnabled(TRUE);
    	promoPlan->setEnabled(TRUE);
	    promoEndsOnLabel->setEnabled(promoPlan->isChecked());
    	promoEndsOn->setEnabled(promoPlan->isChecked());
    }
}

/*
** togglePromoPlan - Toggles the availability of the various input fields,
**                   based on the checkbox that the user has selected.
**
*/

void RatePlanEdit::togglePromoPlan(bool newState)
{
    promoEndsOnLabel->setEnabled(newState);
   	promoEndsOn->setEnabled(newState);
}

/*
** cancelRatePlan - Closes the window and destroys itself.
**
*/

void RatePlanEdit::cancelRatePlan()
{
	close();
	delete this;
}

/*
** saveRatePlan - Saves the data, closes the window and destroys itself.
**
*/

void RatePlanEdit::saveRatePlan()
{
	RatePlansDB RPDB;
	
	// Check to make sure that the planTag has something in it...
	if (!strlen(planTag->text())) {
        QMessageBox::critical(this, "Save Rate Plan", "A Rate Plan must have a tag.");
		planTag->setFocus();
		return;
	}
	
	// Check to make sure the description has something in it.
	if (!strlen(description->text())) {
        QMessageBox::critical(this, "Save Rate Plan", "A Rate Plan must have a description.");
		description->setFocus();
		return;
	}
	
	RPDB.setValue("PlanTag", planTag->text());
	RPDB.setValue("Description", description->text());
	if (noAutoConvert->isChecked()) {
		RPDB.setValue("AutoConvert", (int) 0);
		RPDB.setValue("ConvertDays", (int) 0);
		RPDB.setValue("ConvertDate", "0000-00-00");
		RPDB.setValue("PromoPlan", (int) 0);
		RPDB.setValue("PromoEnds", "0000-00-00");
	} else {
		if (autoConvertDays->isChecked()) {
			RPDB.setValue("AutoConvert", (int) 1);
			RPDB.setValue("ConvertDays", convertDays->text());
			RPDB.setValue("ConvertDate", "0000-00-00");
		} else {
			RPDB.setValue("AutoConvert", (int) 2);
			RPDB.setValue("ConvertDays", (int) 0);
			RPDB.setValue("ConvertDate", convertDays->text());
		}
		RPDB.setValue("PromoPlan", (int) promoPlan->isChecked());
		if (promoPlan->isChecked()) {
			RPDB.setValue("PromoPlan", promoEndsOn->date().toString("yyyy-MM-dd").ascii());
		} else {
			RPDB.setValue("PromoPlan", "0000-00-00");
		}
	}
	
	if (!myIntID) {
		myIntID = RPDB.ins();
	} else {
		RPDB.setValue("InternalID", (long) myIntID);
		RPDB.upd();
	}
    emit(ratePlanSaved((int) myIntID));
	
	delete this;
}
