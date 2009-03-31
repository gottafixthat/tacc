/**
 * BillingCycleEdit.cpp - Allows an administrator to edit a billing cycle.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <BillingCycleEdit.h>
#include <BlargDB.h>
#include <qlabel.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3BoxLayout>
#include <ADB.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>

BillingCycleEdit::BillingCycleEdit
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("New Billing Cycle");
    myCycleID = "";

    // Create our widgets
    QLabel  *cycleIDLabel = new QLabel(this, "cycleIDLabel");
    cycleIDLabel->setText("Cycle ID:");
    cycleIDLabel->setAlignment(Qt::AlignRight);

    cycleID = new QLineEdit(this, "cycleID");
    cycleID->setMaxLength(16);

    QLabel *cycleTypeLabel = new QLabel(this, "cycleTypeLabel");
    cycleTypeLabel->setText("Cycle Type:");
    cycleTypeLabel->setAlignment(Qt::AlignRight);

    cycleType = new QComboBox(false, this, "cycleType");
    cycleType->insertItem("Day");
    cycleType->insertItem("Anniversary");
    connect(cycleType, SIGNAL(activated(int)), this, SLOT(cycleTypeChanged(int)));

    QLabel *descriptionLabel = new QLabel(this, "descriptionLabel");
    descriptionLabel->setText("Description:");
    descriptionLabel->setAlignment(Qt::AlignRight);

    description = new QLineEdit(this, "description");
    description->setMaxLength(80);

    defaultCycle= new QCheckBox("Default for new customers", this, "defaultCycle");

    // Stuff for CycleType == Day
    dayOfMonthLabel = new QLabel(this, "dayOfMonthLabel");
    dayOfMonthLabel->setText("Day of month:");
    dayOfMonthLabel->setAlignment(Qt::AlignRight);

    dayOfMonth = new QSpinBox(1,31,1,this,"dayOfMonth");

    monthsLabel = new QLabel(this, "monthsLabel");
    monthsLabel->setText("Months processed:");
    monthsLabel->setAlignment(Qt::AlignRight);

    january     = new QCheckBox("January",   this, "january");
    february    = new QCheckBox("February",  this, "february");
    march       = new QCheckBox("March",     this, "march");
    april       = new QCheckBox("April",     this, "april");
    may         = new QCheckBox("May",       this, "may");
    june        = new QCheckBox("June",      this, "june");
    july        = new QCheckBox("July",      this, "july");
    august      = new QCheckBox("August",    this, "august");
    september   = new QCheckBox("September", this, "september");
    october     = new QCheckBox("October",   this, "october");
    november    = new QCheckBox("November",  this, "november");
    december    = new QCheckBox("December",  this, "december");

    // Stuff for CycleType == Anniversary
    anniversaryPeriodLabel = new QLabel(this, "anniversaryPeriodLabel");
    anniversaryPeriodLabel->setText("Period (months):");
    anniversaryPeriodLabel->setAlignment(Qt::AlignRight);

    anniversaryPeriod = new QSpinBox(1,12,1,this,"anniversaryPeriod");

    // Create our buttons now.
    QPushButton *saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    // Create the layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    
    // Create the main grid layout
    Q3GridLayout *gl = new Q3GridLayout();
    int curRow = 0;
    gl->addWidget(cycleIDLabel,     curRow, 0);
    gl->addWidget(cycleID,          curRow, 1);
    curRow++;
    gl->addWidget(cycleTypeLabel,   curRow, 0);
    gl->addWidget(cycleType,        curRow, 1);
    curRow++;
    gl->addWidget(descriptionLabel, curRow, 0);
    gl->addWidget(description,      curRow, 1);
    curRow++;
    gl->addWidget(defaultCycle,     curRow, 1);
    curRow++;
    gl->addWidget(dayOfMonthLabel,  curRow, 0);
    gl->addWidget(dayOfMonth,       curRow, 1);
    curRow++;

    // Create another grid for our months.
    Q3GridLayout *mgrid = new Q3GridLayout();
    int mRow = 0;
    mgrid->addWidget(january,       mRow, 0);
    mgrid->addWidget(february,      mRow, 1);
    mgrid->addWidget(march,         mRow, 2);
    mRow++;
    mgrid->addWidget(april,         mRow, 0);
    mgrid->addWidget(may,           mRow, 1);
    mgrid->addWidget(june,          mRow, 2);
    mRow++;
    mgrid->addWidget(july,          mRow, 0);
    mgrid->addWidget(august,        mRow, 1);
    mgrid->addWidget(september,     mRow, 2);
    mRow++;
    mgrid->addWidget(october,       mRow, 0);
    mgrid->addWidget(november,      mRow, 1);
    mgrid->addWidget(december,      mRow, 2);
    mRow++;
    // Add the months to our main grid.
    gl->addWidget(monthsLabel,      curRow, 0);
    gl->addLayout(mgrid,            curRow, 1);
    curRow++;
    // Add the anniversary info to the main grid.
    gl->addWidget(anniversaryPeriodLabel, curRow, 0);
    gl->addWidget(anniversaryPeriod, curRow, 1);
    curRow++;

    // Add the grid to our main box layout
    ml->addLayout(gl, 0);
    ml->addStretch(1);
    // Create our button layout.
    Q3BoxLayout  *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    // Finally, add the button layout to the main layout.
    ml->addLayout(bl, 0);

    EditingCycle = 0;
    cycleTypeChanged(0);
}

BillingCycleEdit::~BillingCycleEdit()
{
}

/**
 * setCycleID()
 * 
 * Sets the cycle ID for editing.
 */
void BillingCycleEdit::setCycleID(const QString newCycleID)
{
    BillingCyclesDB BCDB;

    myCycleID = newCycleID;

    EditingCycle = 1;

    setCaption("Edit Billing Cycle");
    BCDB.getByCycleID(myCycleID);

    cycleID->setText(BCDB.CycleID);
    cycleID->setEnabled(FALSE);
    cycleType->setCurrentText(BCDB.CycleType);
    description->setText(BCDB.Description);
    january->setChecked(BCDB.Jan.toInt());
    february->setChecked(BCDB.Feb.toInt());
    march->setChecked(BCDB.Mar.toInt());
    april->setChecked(BCDB.Apr.toInt());
    may->setChecked(BCDB.May.toInt());
    june->setChecked(BCDB.Jun.toInt());
    july->setChecked(BCDB.Jul.toInt());
    august->setChecked(BCDB.Aug.toInt());
    september->setChecked(BCDB.Sep.toInt());
    october->setChecked(BCDB.Oct.toInt());
    november->setChecked(BCDB.Nov.toInt());
    december->setChecked(BCDB.Dece.toInt());
    dayOfMonth->setValue(BCDB.Day.toInt());
    defaultCycle->setChecked(BCDB.DefaultCycle.toInt());
    anniversaryPeriod->setValue(BCDB.AnniversaryPeriod.toInt());
    description->setFocus();
    cycleTypeChanged(cycleType->currentItem());
}

// The user clicked on the close button...

void BillingCycleEdit::cancelClicked()
{
    delete this;
}

// The user clicked on the save button...

void BillingCycleEdit::saveClicked()
{
    BillingCyclesDB BCDB;
    int             Unique = 1;

    // First things first.  Check to see if we're editing one or creating 
    // a new one.  If we're creating a new one, make sure that the CycleID
    // is unique.
    if (!EditingCycle) {
        // We're Creating a new one, verify the CycleID is unique.
        QString tmpStr = cycleID->text();
        tmpStr.stripWhiteSpace();
        tmpStr.truncate(16);
        if (BCDB.getByCycleID(tmpStr)) {
            Unique = 0;
            QMessageBox::critical(this, "Save Billing Cycle", "The billing cycle could not be saved.\nThe Cycle ID must be unique.");
        }
    } else {
    	BCDB.getByCycleID(cycleID->text());
    }
    
    if (Unique) {
        BCDB.CycleID     = cycleID->text();
        BCDB.CycleType   = cycleType->currentText();
        BCDB.Description = description->text();
        BCDB.Jan.setNum((int)january->isChecked());
        BCDB.Feb.setNum((int)february->isChecked());
        BCDB.Mar.setNum((int)march->isChecked());
        BCDB.Apr.setNum((int)april->isChecked());
        BCDB.May.setNum((int)may->isChecked());
        BCDB.Jun.setNum((int)june->isChecked());
        BCDB.Jul.setNum((int)july->isChecked());
        BCDB.Aug.setNum((int)august->isChecked());
        BCDB.Sep.setNum((int)september->isChecked());
        BCDB.Oct.setNum((int)october->isChecked());
        BCDB.Nov.setNum((int)november->isChecked());
        BCDB.Dece.setNum((int)december->isChecked());
        BCDB.Day = dayOfMonth->text();
        BCDB.AnniversaryPeriod = anniversaryPeriod->text();
        BCDB.DefaultCycle.setNum((int)defaultCycle->isChecked());
        if (EditingCycle) {
            BCDB.upd();
        } else {
            // Last check.  Make sure the user put something in the CycleID
            if (!BCDB.CycleID.length()) {
                QMessageBox::critical(this, "Save Billing Cycle", "The Billing Cycle ID must not be blank.");
                return;
            } else {
                BCDB.ins();
            }
        }
    }
    emit refresh(1);
    delete this;
}

/**
 * cycleTypeChanged()
 *
 * Gets called when the user changes the type of billing cycle.
 * This shows the fields that are appropriate for this type of
 * billing cycle and hides the rest.
 */
void BillingCycleEdit::cycleTypeChanged(int newCycleType)
{
    int hidemonthly = 0;
    int hideanniversary = 1;

    if (newCycleType == 1) {
        hidemonthly = 1;
        hideanniversary = 0;
    }

    dayOfMonthLabel->setHidden(hidemonthly);
    dayOfMonth->setHidden(hidemonthly);
    monthsLabel->setHidden(hidemonthly);
    january->setHidden(hidemonthly);
    february->setHidden(hidemonthly);
    march->setHidden(hidemonthly);
    april->setHidden(hidemonthly);
    may->setHidden(hidemonthly);
    june->setHidden(hidemonthly);
    july->setHidden(hidemonthly);
    august->setHidden(hidemonthly);
    september->setHidden(hidemonthly);
    october->setHidden(hidemonthly);
    november->setHidden(hidemonthly);
    december->setHidden(hidemonthly);

    anniversaryPeriodLabel->setHidden(hideanniversary);
    anniversaryPeriod->setHidden(hideanniversary);
}
