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
    connect(cityList, SIGNAL(activated(const QString &)), this, SLOT(citySelected(const QString &)));

    messageArea = new QLabel(this, "messageArea");

    addButton = new QPushButton(this);
    addButton->setText("&Add");
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
    gl->addWidget(npaLabel,             rowNum, 0);
    gl->addLayout(npanxxb,              rowNum, 1);
    rowNum++;
    gl->addWidget(startLabel,           rowNum, 0);
    gl->addWidget(numStart,             rowNum, 1);
    rowNum++;
    gl->addWidget(endLabel,             rowNum, 0);
    gl->addWidget(numStop,              rowNum, 1);
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

    ml->addLayout(gl, 1);

    // Add the button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addWidget(messageArea, 0);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(doneButton, 0);

    ml->addLayout(bl, 0);

    QApplication::restoreOverrideCursor();
}

/**
  * ~DIDManagerAdd - Destructor.
  */
DIDManagerAdd::~DIDManagerAdd()
{
}

/**
  * doneClicked - Gets called when the user hits the "Done" button.
  */
void DIDManagerAdd::doneClicked()
{
    delete this;
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


