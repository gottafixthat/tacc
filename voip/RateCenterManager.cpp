/*
***************************************************************************
**
** RateCenterManager - An interface that allows an admin to manage the
** list of rate centers that we provide DID's in.
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
#include <Cfg.h>

#include <TAA.h>
#include "RateCenterManager.h"
//#include "LogVoiceMail.h"

RateCenterManager::RateCenterManager
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Rate Center Manager" );

    rcList = new QListView(this, "Rate Center Manager");
    rcList->setAllColumnsShowFocus(true);
    rcList->setRootIsDecorated(true);
    rcList->addColumn("Country/State/City");
    rcList->addColumn("Active");
    rcList->addColumn("Avail");
    rcList->addColumn("Total");

    activeColumn    = 1;
    availColumn     = 2;
    totalColumn     = 3;
    idColumn        = 4;

    rcList->setColumnAlignment(activeColumn, Qt::AlignRight);
    rcList->setColumnAlignment(availColumn,  Qt::AlignRight);
    rcList->setColumnAlignment(totalColumn,  Qt::AlignRight);


    addButton = new QPushButton(this, "Add Button");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    editButton = new QPushButton(this, "Edit Button");
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editClicked()));

    deleteButton = new QPushButton(this, "Delete Button");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    deleteButton->setEnabled(false);

    closeButton = new QPushButton(this, "Close Button");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(rcList, 1);

    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

RateCenterManager::~RateCenterManager()
{
}

/**
  * refreshList - Refreshes the rate center list.
  */

void RateCenterManager::refreshList()
{
    ADB     myDB;
    ADB     myDB2;
    ADB     myDB3;

    long    curID = -1;
    // Get the ID of the currently hilighted item.
    if (rcList->currentItem() != 0) {
        QListViewItem *tmpCur = rcList->currentItem();
        if (strlen(tmpCur->key(idColumn,0))) {
            curID = atol(tmpCur->key(idColumn,0));
        }
    }

    QApplication::setOverrideCursor(Qt::waitCursor);
    rcList->clear();
    // Get the list of countries
    myDB.query("select distinct(Country) from DID_Rate_Centers");
    if (myDB.rowCount) {
        // Walk through the list of countries and get the states
        while(myDB.getrow()) {
            QListViewItem   *countryItem = new QListViewItem(rcList, myDB.curRow["Country"]);
            myDB2.query("select distinct(State) from DID_Rate_Centers where Country = '%s' order by State", myDB.curRow["Country"]);
            // No need to check if they exist since a state can't exist without a country.
            while (myDB2.getrow()) {
                QListViewItem   *stateItem = new QListViewItem(countryItem, myDB2.curRow["State"]);
                myDB3.query("select City, RateCenterID from DID_Rate_Centers where Country = '%s' and State = '%s' order by City", myDB.curRow["Country"], myDB2.curRow["State"]);
                while (myDB3.getrow()) {
                    QListViewItem *curItem = new QListViewItem(stateItem, myDB3.curRow["City"], "", "", "", myDB3.curRow["RateCenterID"]);
                    fprintf(stderr, "Checking RateCenterID against '%ld'\n", curID); fflush(stderr);
                    if (atol(myDB3.curRow["RateCenterID"]) == curID) {
                        countryItem->setOpen(true);
                        stateItem->setOpen(true);
                        rcList->setCurrentItem(curItem);
                        rcList->ensureItemVisible(curItem);
                    }
                    fprintf(stderr, "Done checking RateCenterID against '%ld'\n", curID); fflush(stderr);
                }
            }
        }
    }

    // Check to see if we had a hilighted item, if so, find it again.


    QApplication::restoreOverrideCursor();
}

/** refreshProvider - A slot that connects to the provider editor so the
  * provider that was affected can be refreshed after an edit.
  */
void RateCenterManager::refreshRateCenter(long rateCenterID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void RateCenterManager::addClicked()
{
    RateCenterEditor   *rce;
    rce = new RateCenterEditor();
    rce->show();
    connect(rce, SIGNAL(rateCenterSaved(long)), this, SLOT(refreshRateCenter(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void RateCenterManager::editClicked()
{
    QListViewItem   *curItem;
    curItem = rcList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                RateCenterEditor   *rce;
                rce = new RateCenterEditor();
                rce->setRateCenterID(tmpID);
                rce->show();
                connect(rce, SIGNAL(rateCenterSaved(long)), this, SLOT(refreshRateCenter(long)));
            }
        }
    }
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void RateCenterManager::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void RateCenterManager::closeClicked()
{
    delete this;
}


/**
  * RateCenterEditor - Allows the user to add or edit an
  * origination provider.
  */

RateCenterEditor::RateCenterEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Rate Center Add" );

    rateCenterID = 0;

    ADB     DB;

    QLabel  *countryListLabel = new QLabel(this, "countryListLabel");
    countryListLabel->setText("Country:");
    countryListLabel->setAlignment(Qt::AlignRight);

    countryList = new QComboBox(true, this, "countryList");
    countryList->setAutoCompletion(true);
    // Fill the state list.
    DB.query("select distinct(Country) from DID_Rate_Centers order by Country");
    if (DB.rowCount) while (DB.getrow()) {
        countryList->insertItem(DB.curRow["Country"]);
    }
    connect(countryList, SIGNAL(activated(const QString &)), this, SLOT(countrySelected(const QString &)));

    QLabel  *stateListLabel = new QLabel(this, "stateListLabel");
    stateListLabel->setText("State:");
    stateListLabel->setAlignment(Qt::AlignRight);

    stateList = new QComboBox(true, this, "stateList");
    stateList->setAutoCompletion(true);
    // Fill the state list.
    DB.query("select distinct(State) from DID_Rate_Centers where Country = '%s' order by State", (const char *)countryList->currentText());
    if (DB.rowCount) while (DB.getrow()) {
        stateList->insertItem(DB.curRow["State"]);
    }

    QLabel *cityLabel = new QLabel(this, "cityLabel");
    cityLabel->setText("City:");
    cityLabel->setAlignment(Qt::AlignRight);

    city = new QLineEdit(this, "city");


    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(countryListLabel,          curRow, 0);
    gl->addWidget(countryList,               curRow, 1);
    curRow++;
    gl->addWidget(stateListLabel,            curRow, 0);
    gl->addWidget(stateList,                 curRow, 1);
    curRow++;
    gl->addWidget(cityLabel,                 curRow, 0);
    gl->addWidget(city,                      curRow, 1);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~RateCenterEditor - Destructor.
  */
RateCenterEditor::~RateCenterEditor()
{
}

/**
  * setProviderID - Activates the "edit" mode of the editor.
  * Loads up the specified provider ID and puts its values into
  * the form.
  * Returns true if the operation was successful, otherwise false.
  */
int RateCenterEditor::setRateCenterID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("DID_Rate_Centers");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        countryList->setCurrentText(DB.getStr("Country"));
        countrySelected(DB.getStr("Country"));
        stateList->setCurrentText(DB.getStr("State"));
        city->setText(DB.getStr("City"));

        retVal = 1;
        rateCenterID = newID;
    }

    setCaption( "Rate Center Edit" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void RateCenterEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;

    // Validate the country
    if (countryList->currentText().length() < 2 || countryList->currentText().length() > 64) {
        QMessageBox::critical(this, "Unable to save Rate Center", "Country must be between 2 and 64 characters long.");
        return;
    }

    // Validate the state
    if (stateList->currentText().length() < 2 || stateList->currentText().length() > 64) {
        QMessageBox::critical(this, "Unable to save Rate Center", "State must be between 2 and 64 characters long.");
        return;
    }

    // Finally, validate the city
    if (city->text().length() < 2 || city->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Rate Center", "City must be between 2 and 64 characters long.");
        return;
    }

    // Now make sure that this combination is already unique.
    DB.query("select RateCenterID from DID_Rate_Centers where Country = '%s' and State = '%s' and City = '%s' and RateCenterID <> %ld",
            (const char *)countryList->currentText(),
            (const char *)stateList->currentText(),
            (const char *)city->text(),
            rateCenterID);

    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Rate Center", "The specified rate center already exists.");
        return;
    }

    // Now check to see if we are updating or adding.
    ADBTable    rcDB("DID_Rate_Centers");
    if (rateCenterID) {
        rcDB.get(rateCenterID);
    }

    rcDB.setValue("Country",        (const char *)countryList->currentText());
    rcDB.setValue("State",          (const char *)stateList->currentText());
    rcDB.setValue("City",           (const char *)city->text());

    if (rateCenterID) {
        rcDB.upd();
    } else {
        rcDB.ins();
        rateCenterID = rcDB.getLong("RateCenterID");
    }

    emit(rateCenterSaved(rateCenterID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void RateCenterEditor::cancelClicked()
{
    delete this;
}

/**
  * countrySelected - Refreshes the list of states based on the country
  * that the user has selected in the country list.
  */
void RateCenterEditor::countrySelected(const QString &tmpCountry)
{
    ADB     DB;
    stateList->clear();
    DB.query("select distinct(State) from DID_Rate_Centers where Country = '%s' order by State", (const char *)tmpCountry);
    while (DB.getrow()) {
        stateList->insertItem(DB.curRow["State"]);
    }
}

