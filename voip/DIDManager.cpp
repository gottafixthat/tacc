/*
***************************************************************************
**
** DIDManager - An interface that allows an admin to manage DID's for
** our VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include <TAA.h>
#include "DIDManager.h"
//#include "LogVoiceMail.h"

DIDManager::DIDManager
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "DID Manager" );

}

DIDManager::~DIDManager()
{
}


/**
  * DIDManagerAdd - Constructor for the DID Manager's Add DID class.
  */
DIDManagerAdd::DIDManagerAdd
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "DID Manager - Add DIDs" );

    QApplication::setOverrideCursor(Qt::waitCursor);

    ADB     DB;

    QLabel *vendorListLabel = new QLabel(this, "vendorListLabel");
    vendorListLabel->setText("Vendor:");
    vendorListLabel->setAlignment(Qt::AlignRight);

    vendorList = new QComboBox(this, "vendorList");
    DB.query("select Vendors.CompanyName, VoIP_Origination.OriginationID from Vendors, VoIP_Origination where Vendors.VendorID = VoIP_Origination.VendorID group by VoIP_Origination.VendorID order by Vendors.CompanyName");
    if (DB.rowCount) while (DB.getrow()) {
        vendorList->insertItem(DB.curRow["CompanyName"]);
    }
    
    QLabel *groupListLabel = new QLabel(this, "groupListLabel");
    groupListLabel->setText("Origination Group:");
    groupListLabel->setAlignment(Qt::AlignRight);

    groupList = new QComboBox(this, "groupList");
    vendorSelected(vendorList->currentText());
    connect(vendorList, SIGNAL(activated(const QString &)), this, SLOT(vendorSelected(const QString &)));

    QLabel *countryListLabel = new QLabel(this, "countryListLabel");
    countryListLabel->setText("Country:");
    countryListLabel->setAlignment(Qt::AlignRight);

    countryList = new QComboBox(this, "countryList");
    DB.query("select distinct(Country) from DID_Rate_Centers order by Country");
    if (DB.rowCount) while (DB.getrow()) {
        countryList->insertItem(DB.curRow["Country"]);
    }
    connect(countryList, SIGNAL(activated(const QString &)), this, SLOT(countrySelected(const QString &)));

    QLabel *stateListLabel = new QLabel(this, "stateListLabel");
    stateListLabel->setText("State:");
    stateListLabel->setAlignment(Qt::AlignRight);

    stateList = new QComboBox(this, "stateList");
    DB.query("select distinct(State) from DID_Rate_Centers where Country = '%s' order by State", (const char *)countryList->currentText());
    if (DB.rowCount) while (DB.getrow()) {
        stateList->insertItem(DB.curRow["State"]);
    }
    connect(stateList, SIGNAL(activated(const QString &)), this, SLOT(stateSelected(const QString &)));

    QLabel *cityListLabel = new QLabel(this, "cityListLabel");
    cityListLabel->setText("City:");
    cityListLabel->setAlignment(Qt::AlignRight);

    cityList = new QComboBox(this, "cityList");
    DB.query("select distinct(City) from DID_Rate_Centers where Country = '%s' and State = '%s' order by City", (const char *)countryList->currentText(), (const char *)stateList->currentText());
    if (DB.rowCount) while (DB.getrow()) {
        cityList->insertItem(DB.curRow["City"]);
    }

    QLabel *npaLabel = new QLabel(this, "npaLabel");
    npaLabel->setText("NPA/NXX:");
    npaLabel->setAlignment(AlignRight|AlignVCenter);

    npa = new QLineEdit(this, "npa");
    npa->setInputMask("999");
    npa->setMaxLength(3);
    npa->setMaximumWidth(npa->minimumSizeHint().width() * 2);

    QLabel *nxxLabel = new QLabel(this, "nxxLabel");
    nxxLabel->setText("/");
    nxxLabel->setAlignment(AlignRight|AlignVCenter);

    nxx = new QLineEdit(this, "nxx");
    nxx->setMaxLength(3);
    nxx->setInputMask("999");
    nxx->setMaximumWidth(nxx->minimumSizeHint().width() * 2);

    QLabel *startLabel = new QLabel(this, "startLabel");
    startLabel->setText("Starting Number:");

    numStart = new QLineEdit(this, "numStart");
    numStart->setInputMask("9999");
    numStart->setMaxLength(4);
    numStart->setMaximumWidth(numStart->minimumSizeHint().width() * 2);

    QLabel *endLabel = new QLabel(this, "endLabel");
    endLabel->setText("Ending Number:");

    numStop = new QLineEdit(this, "numStop");
    numStop->setInputMask("9999");
    numStop->setMaxLength(4);
    numStop->setMaximumWidth(numStop->minimumSizeHint().width() * 2);

    QLabel *reservedLabel = new QLabel(this, "reservedLabel");
    reservedLabel->setText("Reserve Status:");

    reserved = new QComboBox(this, "reserved");
    reserved->insertItem("Not Reserved");
    reserved->insertItem("Reserved - Manual Selection Only");
    reserved->insertItem("Reserved Until Specified Date");

    QLabel *reserveDateLabel = new QLabel(this, "reserveDateLabel");
    reserveDateLabel->setText("Reserve Date:");

    reserveDate = new QDateEdit(this);
    reserveDate->setDate(QDate::currentDate().addDays(90));
    reserveDate->setEnabled(false);
    connect(reserved, SIGNAL(activated(int)), this, SLOT(reserveTypeChanged(int)));

    messageArea = new QLabel(this, "messageArea");

    addButton = new QPushButton(this);
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    doneButton = new QPushButton(this);
    doneButton->setText("&Done");
    connect(doneButton, SIGNAL(clicked()), this, SLOT(doneClicked()));

    // Setup our layout now.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QBoxLayout  *npanxxb = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    npanxxb->addWidget(npa, 0);
    npanxxb->addWidget(nxxLabel, 0);
    npanxxb->addWidget(nxx, 0);
    npanxxb->addStretch(1);

    QGridLayout *gl = new QGridLayout(3, 5);
    int rowNum = 0;
    gl->addWidget(vendorListLabel,      rowNum, 0);
    gl->addWidget(vendorList,           rowNum, 1);
    rowNum++;
    gl->addWidget(groupListLabel,       rowNum, 0);
    gl->addWidget(groupList,            rowNum, 1);
    rowNum++;
    gl->addWidget(countryListLabel,     rowNum, 0);
    gl->addWidget(countryList,          rowNum, 1);
    rowNum++;
    gl->addWidget(stateListLabel,       rowNum, 0);
    gl->addWidget(stateList,            rowNum, 1);
    rowNum++;
    gl->addWidget(cityListLabel,        rowNum, 0);
    gl->addWidget(cityList,             rowNum, 1);
    rowNum++;
    gl->addWidget(npaLabel,             rowNum, 0);
    gl->addLayout(npanxxb,              rowNum, 1);
    rowNum++;
    gl->addWidget(startLabel,           rowNum, 0);
    gl->addWidget(numStart,             rowNum, 1);
    rowNum++;
    gl->addWidget(endLabel,             rowNum, 0);
    gl->addWidget(numStop,              rowNum, 1);
    rowNum++;
    gl->addWidget(reservedLabel,        rowNum, 0);
    gl->addWidget(reserved,             rowNum, 1);
    rowNum++;
    gl->addWidget(reserveDateLabel,     rowNum, 0);
    gl->addWidget(reserveDate,          rowNum, 1);
    rowNum++;

    ml->addLayout(gl, 1);

    // Add the button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addWidget(messageArea, 0);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(doneButton, 0);

    ml->addLayout(bl, 0);

    errorBoxText = "Unable to save DIDs";
    QApplication::restoreOverrideCursor();
}

/**
  * ~DIDManagerAdd - Destructor.
  */
DIDManagerAdd::~DIDManagerAdd()
{
}

/**
  * vendorSelected - When the user selects a vendor from the list, we need
  * to refresh the origination group list to show the ones associated
  * with this vendor.
  */
void DIDManagerAdd::vendorSelected(const QString &newVendor)
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     DB;
    long    vendorID;
    groupList->clear();
    // Get the Vendor ID
    DB.query("select VendorID from Vendors where CompanyName = '%s'", (const char *)newVendor);
    if (DB.rowCount) {
        DB.getrow();
        vendorID = atol(DB.curRow["VendorID"]);
        DB.query("select Tag from VoIP_Origination where VendorID = %ld order by Tag", vendorID);
        if (DB.rowCount) while (DB.getrow()) {
            groupList->insertItem(DB.curRow["Tag"]);
        }
    }
    QApplication::restoreOverrideCursor();
}


/**
  * countrySelected - When the customer picks a country, we need to fill in
  * the states for that country.
  */
void DIDManagerAdd::countrySelected(const QString &newCountry)
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     DB;
    stateList->clear();
    DB.query("select distinct(State) from DID_Rate_Centers where Country = '%s' order by State", (const char *)newCountry);
    if (DB.rowCount) while (DB.getrow()) {
        stateList->insertItem(DB.curRow["State"]);
    }
    QApplication::restoreOverrideCursor();
    // Now reset the state list.
    stateSelected(stateList->currentText());
}


/**
  * stateSelected - When the customer picks a state, we need to fill in
  * the cities for that state.
  */
void DIDManagerAdd::stateSelected(const QString &newState)
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     DB;
    cityList->clear();
    DB.query("select distinct(City) from DID_Rate_Centers where Country = '%s' and State = '%s' order by City", (const char *)countryList->currentText(), (const char *)newState);
    if (DB.rowCount) while (DB.getrow()) {
        cityList->insertItem(DB.curRow["City"]);
    }
    QApplication::restoreOverrideCursor();
}

/**
  * reserveTypeChanged - Gets called when the user changes
  * the reserved setting on the DID's.  This enables or disables
  * the date entry box according to how we reserve it.
  */
void DIDManagerAdd::reserveTypeChanged(int newReserve)
{
    switch (newReserve) {
        case 0:
        case 1:
            reserveDate->setEnabled(false);
            break;
        default:
            reserveDate->setEnabled(true);
            break;
    }
}


/**
  * doneClicked - Gets called when the user hits the "Done" button.
  */
void DIDManagerAdd::doneClicked()
{
    delete this;
}

/**
  * addClicked - Validates the form information and adds the DID's into
  * the database.
  */
void DIDManagerAdd::addClicked()
{
    // Validate the form data.  We have mostly combo boxes, so validation
    // is mostly making sure that the NPA/NXX and number ranges are valid.
    // We also need to make sure that we haven't added any of these DID's
    // before.
    int     numNPA = 0;
    int     numNXX = 0;
    int     rangeStart = 0;
    int     rangeStop = 0;
    char    resDateStr[1024];

    if (!npa->hasAcceptableInput()) {
        QMessageBox::critical(this, errorBoxText, "The NPA is not valid.");
        npa->setFocus();
        return;
    } else {
        numNPA = npa->text().toInt();
        if (numNPA < 100 || numNPA > 999) {
            QMessageBox::critical(this, errorBoxText, "The NPA is not valid.");
            npa->setFocus();
            return;
        }
    }

    if (!nxx->hasAcceptableInput()) {
        QMessageBox::critical(this, errorBoxText, "The NXX is not valid.");
        nxx->setFocus();
        return;
    } else {
        numNXX = nxx->text().toInt();
        if (numNXX < 100 || numNXX > 999) {
            QMessageBox::critical(this, errorBoxText, "The NXX is not valid.");
            nxx->setFocus();
            return;
        }
    }

    if (!numStart->hasAcceptableInput()) {
        QMessageBox::critical(this, errorBoxText, "The starting number is not valid.");
        numStart->setFocus();
        return;
    } else {
        rangeStart = numStart->text().toInt();
        if (rangeStart < 0 || rangeStart > 9999) {
            QMessageBox::critical(this, errorBoxText, "The starting number is not valid.");
            numStart->setFocus();
            return;
        }
    }

    if (!numStop->hasAcceptableInput()) {
        QMessageBox::critical(this, errorBoxText, "The ending number is not valid.");
        numStop->setFocus();
        return;
    } else {
        rangeStop = numStop->text().toInt();
        if (rangeStart < 0 || rangeStart > 9999) {
            QMessageBox::critical(this, errorBoxText, "The ending number is not valid.");
            numStop->setFocus();
            return;
        }
    }

    if (rangeStop < rangeStart) {
        QMessageBox::critical(this, errorBoxText, "The starting number is greater than the ending number.");
        numStart->setFocus();
        return;
    }

    // If we made to this point, we have validated that all of the inputs are
    // correct.  Now make sure we don't have any overlapping numbers.
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADB     DB;
    char    didStr[1024];
    char    startDIDStr[1024];
    char    stopDIDStr[1024];
    sprintf(startDIDStr, "%03d%03d%04d", numNPA, numNXX, rangeStart);
    sprintf(stopDIDStr,  "%03d%03d%04d", numNPA, numNXX, rangeStop);
    long    matchCount = 0;
    DB.query("select count(DID) from DID_Inventory where DID >= %s and DID <= %s", startDIDStr, stopDIDStr);
    DB.getrow();
    QApplication::restoreOverrideCursor();
    matchCount = atol(DB.curRow[0]);
    if (matchCount > 0) {
        char    tmpStr[1024];
        sprintf(tmpStr, "Duplicate DIDs already found in inventory.\n%ld DIDs match.", matchCount);
        QMessageBox::critical(this, errorBoxText, tmpStr);
        return;
    }

    // We made it to this point, get the supporting ID's from the rate centers
    // and origination provider tables and then save the DIDs one by one.
    long    rateCenterID = 0;
    long    originationID = 0;
    long    vendorID = 0;

    // Get the Origination ID.
    DB.query("select VendorID from Vendors where CompanyName = '%s'", (const char *)vendorList->currentText());
    if (!DB.rowCount) {
        QMessageBox::critical(this, errorBoxText, "Unable to locate the vendor for this origination group.\n\nPerhaps add an origination group?");
        return;
    }
    DB.getrow();
    vendorID = atol(DB.curRow["VendorID"]);

    DB.query("select OriginationID from VoIP_Origination where VendorID = %ld and Tag = '%s'", vendorID, (const char *)groupList->currentText());
    if (!DB.rowCount) {
        QMessageBox::critical(this, errorBoxText, "Unable to locate the origination group.\n\nPerhaps add an origination group first?");
        return;
    }
    DB.getrow();
    originationID = atol(DB.curRow["OriginationID"]);

    // Get the rate center ID
    DB.query("select RateCenterID from DID_Rate_Centers where Country = '%s' and State = '%s' and City = '%s'", (const char *)countryList->currentText(), (const char *)stateList->currentText(), (const char *)cityList->currentText());
    if (!DB.rowCount) {
        QMessageBox::critical(this, errorBoxText, "Unable to locate the rate center.\n\nPerhaps add a rate center first?");
        return;
    }
    DB.getrow();
    rateCenterID = atol(DB.curRow["RateCenterID"]);

    // Okay, we have everything we need now.  Walk through the
    // list one by one and add them in.
    QApplication::setOverrideCursor(Qt::waitCursor);
    emit(setStatus("Adding DIDs..."));

    // Set the reserve date string
    switch(reserved->currentItem()) {
        case 0:
        case 1:
            strcat(resDateStr, "NOW()");
            break;
        default:
            sprintf(resDateStr, "'%04d-%02d-%02d'", reserveDate->date().year(), reserveDate->date().month(), reserveDate->date().day());
            break;
    }

    int curCount = 0;
    int endCount = rangeStop - rangeStart;
    for (long i = rangeStart; i < rangeStop + 1; i++) {
        sprintf(didStr, "%03d%03d%04d", numNPA, numNXX, i);
        DB.dbcmd("insert into DID_Inventory (DID, OriginationID, RateCenterID, Reserved, ReserveDate, AddDate) values (%s, %ld, %ld, %d, '%s', NOW())",
                didStr, originationID, rateCenterID, reserved->currentItem(), resDateStr);
        emit(setProgress(++curCount, endCount));
    }

    emit(setStatus(""));
    QApplication::restoreOverrideCursor();

    delete this;

}

