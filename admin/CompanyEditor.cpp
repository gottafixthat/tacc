/*
***************************************************************************
**
** CompanyEditor - An interface that allows an admin to manage the
** companies that TACC manages.
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
#include "CompanyEditor.h"

CompanyList::CompanyList
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Manage Companies" );

    coList = new QListView(this, "Company List");
    coList->setAllColumnsShowFocus(true);
    coList->setRootIsDecorated(true);
    coList->addColumn("Company Name");
    coList->addColumn("Active");
    connect(coList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(itemDoubleClicked(QListViewItem *)));

    idColumn        = 2;

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
    ml->addWidget(coList, 1);

    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(editButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);

    refreshList();
}

CompanyList::~CompanyList()
{
}

/**
  * refreshList - Refreshes the company list.
  */

void CompanyList::refreshList()
{
    ADB     myDB;

    QApplication::setOverrideCursor(Qt::waitCursor);
    coList->clear();
    // Get the list of companies
    myDB.query("select CompanyID, CompanyName, Active from Companies");
    if (myDB.rowCount) {
        while (myDB.getrow()) {
            QListViewItem   *tmpItem = new QListViewItem(coList, myDB.curRow["CompanyName"], myDB.curRow["Active"], myDB.curRow["CompanyID"]);
        }
    }

    QApplication::restoreOverrideCursor();
}

/** refreshCompany - A slot that connects to the company editor so the
  * company that was affected can be refreshed after an edit.
  */
void CompanyList::refreshCompany(long companyID)
{
    refreshList();
}

/**
  * addClicked - Gets called when the user clicks on the Add button.
  */

void CompanyList::addClicked()
{
    CompanyEditor   *ce;
    ce = new CompanyEditor();
    ce->show();
    connect(ce, SIGNAL(companySaved(long)), this, SLOT(refreshCompany(long)));
}

/**
  * editClicked - Gets called when the user clicks on the Edit button.
  */

void CompanyList::editClicked()
{
    QListViewItem   *curItem;
    curItem = coList->currentItem();
    if (curItem) {
        long    tmpID;
        if (curItem->key(idColumn,0).length()) {
            tmpID = atoi(curItem->key(idColumn, 0));
            if (tmpID) {
                CompanyEditor *ce;
                ce = new CompanyEditor();
                ce->setCompanyID(tmpID);
                ce->show();
                connect(ce, SIGNAL(companySaved(long)), this, SLOT(refreshCompany(long)));
            }
        }
    }
}

/**
  * deleteClicked - Gets called when the user clicks on the Delete button.
  */

void CompanyList::deleteClicked()
{
}

/**
  * closeClicked - Gets called when the user clicks on the Close button.
  */

void CompanyList::closeClicked()
{
    delete this;
}

/**
  * itemDoubleClicked - When the user double clicks on an origination provider
  * this opens the edit window.
  */
void CompanyList::itemDoubleClicked(QListViewItem *curItem)
{
    if (curItem) editClicked();
}

/**
  * CompanyEditor - Allows the user to add or edit a company for TACC to
  * manage.
  */

CompanyEditor::CompanyEditor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Company Add" );

    companyID = 0;

    ADB     DB;

    QLabel  *companyTagLabel = new QLabel(this, "companyTagLabel");
    companyTagLabel->setText("Tag:");
    companyTagLabel->setAlignment(Qt::AlignRight);

    companyTag = new QLineEdit(this, "companyTag");

    QLabel *companyNameLabel = new QLabel(this, "companyNameLabel");
    companyNameLabel->setText("Company Name:");
    companyNameLabel->setAlignment(Qt::AlignRight);

    companyName = new QLineEdit(this, "companyName");

    QLabel *address1Label = new QLabel(this, "address1Label");
    address1Label->setText("Address 1:");
    address1Label->setAlignment(Qt::AlignRight);

    address1 = new QLineEdit(this, "address1");

    QLabel *address2Label = new QLabel(this, "address2Label");
    address2Label->setText("Address 2:");
    address2Label->setAlignment(Qt::AlignRight);

    address2 = new QLineEdit(this, "address2");

    QLabel *cityLabel = new QLabel(this, "cityLabel");
    cityLabel->setText("City:");
    cityLabel->setAlignment(Qt::AlignRight);

    city = new QLineEdit(this, "city");

    QLabel *stateLabel = new QLabel(this, "stateLabel");
    stateLabel->setText("State:");
    stateLabel->setAlignment(Qt::AlignRight);

    state = new QLineEdit(this, "state");

    QLabel *zipLabel = new QLabel(this, "zipLabel");
    zipLabel->setText("ZIP:");
    zipLabel->setAlignment(Qt::AlignRight);

    zip = new QLineEdit(this, "zip");

    QLabel *mainPhoneLabel = new QLabel(this, "mainPhoneLabel");
    mainPhoneLabel->setText("Main Phone:");
    mainPhoneLabel->setAlignment(Qt::AlignRight);

    mainPhone = new QLineEdit(this, "mainPhone");

    QLabel *altPhoneLabel = new QLabel(this, "altPhoneLabel");
    altPhoneLabel->setText("Alternate Phone:");
    altPhoneLabel->setAlignment(Qt::AlignRight);

    altPhone = new QLineEdit(this, "altPhone");

    QLabel *faxPhoneLabel = new QLabel(this, "faxPhoneLabel");
    faxPhoneLabel->setText("Fax Number:");
    faxPhoneLabel->setAlignment(Qt::AlignRight);

    faxPhone = new QLineEdit(this, "faxPhone");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *gl = new QGridLayout();
    int curRow = 0;
    gl->addWidget(companyTagLabel,          curRow, 0);
    gl->addWidget(companyTag,               curRow, 1);
    curRow++;
    gl->addWidget(companyNameLabel,         curRow, 0);
    gl->addWidget(companyName,              curRow, 1);
    curRow++;
    gl->addWidget(address1Label,            curRow, 0);
    gl->addWidget(address1,                 curRow, 1);
    curRow++;
    gl->addWidget(address2Label,            curRow, 0);
    gl->addWidget(address2,                 curRow, 1);
    curRow++;
    gl->addWidget(cityLabel,                curRow, 0);
    gl->addWidget(city,                     curRow, 1);
    curRow++;
    gl->addWidget(stateLabel,               curRow, 0);
    gl->addWidget(state,                    curRow, 1);
    curRow++;
    gl->addWidget(zipLabel,                 curRow, 0);
    gl->addWidget(zip,                      curRow, 1);
    curRow++;
    gl->addWidget(mainPhoneLabel,           curRow, 0);
    gl->addWidget(mainPhone,                curRow, 1);
    curRow++;
    gl->addWidget(altPhoneLabel,            curRow, 0);
    gl->addWidget(altPhone,                 curRow, 1);
    curRow++;
    gl->addWidget(faxPhoneLabel,            curRow, 0);
    gl->addWidget(faxPhone,                 curRow, 1);
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
  * ~CompanyEditor - Destructor.
  */
CompanyEditor::~CompanyEditor()
{
}

/**
  * setCompanyID - Activates the "edit" mode of the editor.
  * Loads up the specified company and puts its values into
  * the form.
  * Returns true if the operation was successful, otherwise false.
  */
int CompanyEditor::setCompanyID(long newID)
{
    int     retVal = 0;

    ADBTable    DB("Companies");
    if (DB.get(newID) == newID) {
        ADB     myDB;

        companyTag->setText(DB.getStr("CompanyTag"));
        companyName->setText(DB.getStr("CompanyName"));
        address1->setText(DB.getStr("Address1"));
        address2->setText(DB.getStr("Address2"));
        city->setText(DB.getStr("City"));
        state->setText(DB.getStr("State"));
        zip->setText(DB.getStr("ZIP"));
        mainPhone->setText(DB.getStr("MainPhone"));
        altPhone->setText(DB.getStr("altPhone"));
        faxPhone->setText(DB.getStr("FaxPhone"));

        retVal = 1;
        companyID = newID;
    }

    setCaption( "Company Edit" );
    return retVal;
}

/**
  * saveClicked - Validates the form and saves the record.
  */
void CompanyEditor::saveClicked()
{
    // Validate the form data.
    ADB     DB;
    long    vendorID = 0;
    long    serverGroupID = 0;

    // Validate the tag line.
    if (companyTag->text().length() < 2 || companyTag->text().length() > 64) {
        QMessageBox::critical(this, "Unable to save Company", "Tag line must be between 2 and 64 characters long.");
        return;
    }

    DB.query("select CompanyID from Companies where CompanyTag = '%s' and CompanyID <> %ld", (const char *)companyTag->text(), companyID);
    if (DB.rowCount) {
        QMessageBox::critical(this, "Unable to save Company", "Tag lines must be unique.");
        return;
    }

    // Now check to see if we are updating or adding.
    ADBTable    coDB("Companies");
    if (companyID) {
        coDB.get(companyID);
    }

    coDB.setValue("CompanyTag",     (const char *)companyTag->text());
    coDB.setValue("CompanyName",    (const char *)companyName->text());
    coDB.setValue("Address1",       (const char *)address1->text());
    coDB.setValue("Address2",       (const char *)address2->text());
    coDB.setValue("City",           (const char *)city->text());
    coDB.setValue("State",          (const char *)state->text());
    coDB.setValue("ZIP",            (const char *)zip->text());
    coDB.setValue("mainPhone",      (const char *)mainPhone->text());
    coDB.setValue("altPhone",       (const char *)altPhone->text());
    coDB.setValue("faxPhone",       (const char *)faxPhone->text());

    if (companyID) {
        coDB.upd();
    } else {
        coDB.ins();
        companyID = coDB.getLong("CompanyID");
    }

    emit(companySaved(companyID));

    delete this;
}

/**
  * cancelClicked - Closes the editor.
  */
void CompanyEditor::cancelClicked()
{
    delete this;
}

