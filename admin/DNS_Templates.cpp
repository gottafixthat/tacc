/*
** $Id: DNS_Templates.cpp,v 1.2 2004/01/04 18:59:02 marc Exp $
**
***************************************************************************
**
** DNS_Templates - A DNS template editor for SQL based DNS entries.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNS_Templates.cpp,v $
** Revision 1.2  2004/01/04 18:59:02  marc
** delTemplate now checks to make sure the template is not in use before deleting.
**
** Revision 1.1  2003/12/31 21:35:26  marc
** Fully functional.  Could use a bit more polish, but then what software couldn't.
**
**
*/

#include <stdlib.h>

#include "DNS_Templates.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qapplication.h>
#include <ADB.h>
#include <Cfg.h>

// The column our TemplateID is in in the templateList
#define TID_COLUMN 2

DNS_Templates::DNS_Templates(QWidget *parent, const char *name)
    : TAAWidget(parent, name)
{
    setCaption("DNS Template Editor");
    // Create our widgets.
    templateList = new QListView(this, "templateList");
    templateList->addColumn("Template Name");
    templateList->addColumn("Act");
    templateList->setColumnAlignment(1, AlignCenter);
    templateList->setAllColumnsShowFocus(true);
    connect(templateList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(templateSelected(QListViewItem *)));
    connect(templateList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(templateSelected(QListViewItem *)));

    // Our edit widgets.
    QLabel  *templateNameLabel = new QLabel(this, "templateNameLabel");
    templateNameLabel->setText("Name:");
    templateNameLabel->setAlignment(AlignRight|AlignVCenter);

    templateName = new QLineEdit(this, "templateName");
    templateName->setMaxLength(60);

    QLabel  *refreshSpinLabel = new QLabel(this, "refreshSpinLabel");
    refreshSpinLabel->setText("Refresh Time:");
    refreshSpinLabel->setAlignment(AlignRight|AlignVCenter);

    refreshSpin = new QSpinBox(this, "refreshSpin");
    refreshSpin->setRange(1, 999999);

    QLabel  *retrySpinLabel = new QLabel(this, "retrySpinLabel");
    retrySpinLabel->setText("Retry Time:");
    retrySpinLabel->setAlignment(AlignRight|AlignVCenter);

    retrySpin = new QSpinBox(this, "retrySpin");
    retrySpin->setRange(1, 999999);

    QLabel  *expireSpinLabel = new QLabel(this, "expireSpinLabel");
    expireSpinLabel->setText("Expire Time:");
    expireSpinLabel->setAlignment(AlignRight|AlignVCenter);

    expireSpin = new QSpinBox(this, "expireSpin");
    expireSpin->setRange(1, 999999);

    QLabel  *minimumSpinLabel = new QLabel(this, "minimumSpinLabel");
    minimumSpinLabel->setText("Minimum Time:");
    minimumSpinLabel->setAlignment(AlignRight|AlignVCenter);

    minimumSpin = new QSpinBox(this, "minimumSpin");
    minimumSpin->setRange(1, 999999);

    QLabel  *ttlSpinLabel = new QLabel(this, "ttlSpinLabel");
    ttlSpinLabel->setText("Max TTL:");
    ttlSpinLabel->setAlignment(AlignRight|AlignVCenter);

    ttlSpin = new QSpinBox(this, "ttlSpin");
    ttlSpin->setRange(1, 999999);

    allowCustEdit = new QCheckBox(this, "allowCustEdit");
    allowCustEdit->setText("Allow Customer Editing");
    
    propogate = new QCheckBox(this, "propogate");
    propogate->setText("Propogate zone");

    isActive = new QCheckBox(this, "isActive");
    isActive->setText("Active");

    saveSOAButton = new QPushButton(this, "saveSOAButton");
    saveSOAButton->setText("&Save SOA");
    connect(saveSOAButton, SIGNAL(clicked()), this, SLOT(saveSOA()));

    // Create our resource record edit area.
    rrList = new QListView(this, "rrList");
    rrList->addColumn("Host");
    rrList->addColumn("Type");
    rrList->addColumn("Pri");
    rrList->addColumn("IP Address/FQDN");
    rrList->addColumn("TTL");
    rrList->setAllColumnsShowFocus(true);

    QLabel *hostNameLabel = new QLabel(this, "hostNameLabel");
    hostNameLabel->setText("Host:");
    hostNameLabel->setAlignment(AlignRight|AlignVCenter);

    hostName = new QLineEdit(this, "hostName");
    hostName->setMaxLength(64);

    typeList = new QComboBox(false, this, "typeList");
    typeList->insertItem("A");
    typeList->insertItem("AAAA");
    typeList->insertItem("CNAME");
    typeList->insertItem("HINFO");
    typeList->insertItem("MX");
    typeList->insertItem("NS");
    typeList->insertItem("PTR");
    typeList->insertItem("SRV");
    typeList->insertItem("TXT");

    priSpin = new QSpinBox(this, "priSpin");
    priSpin->setRange(0, 99999);
    priSpin->setPrefix("Pri ");

    QLabel *ipAddressLabel = new QLabel(this, "ipAddressLabel");
    ipAddressLabel->setText("IP:");
    ipAddressLabel->setAlignment(AlignRight|AlignVCenter);

    ipAddress = new QLineEdit(this, "ipAddress");
    ipAddress->setMaxLength(128);

    rrTTLSpin = new QSpinBox(this, "rrTTLSpin");
    rrTTLSpin->setRange(0, 99999);
    rrTTLSpin->setPrefix("TTL ");

    addRRButton = new QPushButton(this, "addRRButton");
    addRRButton->setText("&Add");
    connect(addRRButton, SIGNAL(clicked()), this, SLOT(addRR()));

    delRRButton = new QPushButton(this, "delRRButton");
    delRRButton->setText("&Remove");
    connect(delRRButton, SIGNAL(clicked()), this, SLOT(delRR()));

    newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newTemplate()));

    deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(delTemplate()));

    closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    HorizLine   *hline1 = new HorizLine(this, "hline1");
    // Create our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // Widgets and grids
    QBoxLayout *wl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    wl->addWidget(templateList, 0);

    // Most of our edit widgets will go into a grid layout
    QGridLayout *gl = new QGridLayout(4, 4);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);
    gl->setColStretch(3, 0);
    int curRow = 0;
    gl->addWidget(templateNameLabel,                curRow, 0);
    gl->addMultiCellWidget(templateName,            curRow, curRow, 1, 3);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(refreshSpinLabel,                 curRow, 0);
    gl->addWidget(refreshSpin,                      curRow, 1);
    gl->addWidget(retrySpinLabel,                   curRow, 2);
    gl->addWidget(retrySpin,                        curRow, 3);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(expireSpinLabel,                  curRow, 0);
    gl->addWidget(expireSpin,                       curRow, 1);
    gl->addWidget(minimumSpinLabel,                 curRow, 2);
    gl->addWidget(minimumSpin,                      curRow, 3);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(ttlSpinLabel,                     curRow, 0);
    gl->addWidget(ttlSpin,                          curRow, 1);
    gl->addMultiCellWidget(allowCustEdit,           curRow, curRow, 2, 3);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(propogate,                        curRow, 1);
    gl->addWidget(isActive,                         curRow, 2);
    gl->addWidget(saveSOAButton,                    curRow, 3);
    gl->setRowStretch(curRow++, 0);

    // Create an edit layout to hold our edit grid and RR edit area.
    QBoxLayout *el = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    el->addLayout(gl, 0);
    // We need another layout to hold the save SOA button.
    /*
    QBoxLayout *soal = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    soal->addStretch(1);
    soal->addWidget(saveSOAButton, 0);
    el->addLayout(soal, 0);
    */

    el->addWidget(hline1, 0);
    // Add another two layouts for our rr list and the delete button.
    QBoxLayout *rrbl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    rrbl->addWidget(rrList, 1);
    // And another layout for the delete RR button.
    QBoxLayout *drrbl = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    drrbl->addStretch(1);
    drrbl->addWidget(delRRButton, 0);
    rrbl->addLayout(drrbl, 0);

    el->addLayout(rrbl, 1);

    // The rr edit stuff goes into a box layout as well.
    QBoxLayout *rrel = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    rrel->addWidget(hostNameLabel, 0);
    rrel->addWidget(hostName, 1);
    rrel->addWidget(typeList, 0);
    rrel->addWidget(priSpin, 0);
    rrel->addWidget(ipAddressLabel, 0);
    rrel->addWidget(ipAddress, 1);
    rrel->addWidget(rrTTLSpin, 0);
    rrel->addWidget(addRRButton, 0);
    el->addLayout(rrel, 0);
    el->addWidget(new HorizLine(this), 0);

    // Add the edit layout to our widget layout giving it stretch
    wl->addLayout(el, 1);

    ml->addLayout(wl, 1);

    // Buttons
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addWidget(newButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);
    // Done with the layout.


    curTemplateID = 0;
    refreshList();
}

DNS_Templates::~DNS_Templates()
{
}

/*
** refreshList - Loads the list of templates from the database and fills
**               templateList.
*/

void DNS_Templates::refreshList()
{
    ADB     db;
    char    actStr[1024];

    templateList->clear();
    db.query("select TemplateID, TemplateName, Active from DNS_Templates");
    if (db.rowCount) {
        while (db.getrow()) {
            strcpy(actStr, "N");
            if (atoi(db.curRow["Active"])) strcpy(actStr, "Y");
            (void) new QListViewItem(templateList,
                                     db.curRow["TemplateName"],
                                     actStr,
                                     db.curRow["TemplateID"]);
        }
    } else {
        templateSelected(NULL);
    }
}

/*
** templateSelected - Gets called when the user clicks on a template
**                    entry from the list.
*/

void DNS_Templates::templateSelected(QListViewItem *curItem)
{
    curItem = templateList->currentItem();
    if (curItem == NULL) {
        curTemplateID = 0;
        disableEditElements();
        return;
    }

    ADB     db;
    char    myPri[1024];
    db.query("select * from DNS_Templates where TemplateID = %ld", atol(curItem->key(TID_COLUMN, 0)));
    if (!db.rowCount) {
        curTemplateID = 0;
        disableEditElements();
        return;
    }

    curTemplateID = atol(curItem->key(TID_COLUMN, 0));

    db.getrow();
    templateName->setText(db.curRow["TemplateName"]);
    templateName->setEnabled(true);
    refreshSpin->setValue(atoi(db.curRow["Refresh"]));
    refreshSpin->setEnabled(true);
    retrySpin->setValue(atoi(db.curRow["Retry"]));
    retrySpin->setEnabled(true);
    expireSpin->setValue(atoi(db.curRow["Expire"]));
    expireSpin->setEnabled(true);
    minimumSpin->setValue(atoi(db.curRow["Minimum"]));
    minimumSpin->setEnabled(true);
    ttlSpin->setValue(atoi(db.curRow["TTL"]));
    ttlSpin->setEnabled(true);
    isActive->setChecked(atoi(db.curRow["Active"]));
    isActive->setEnabled(true);
    propogate->setChecked(atoi(db.curRow["Propogate"]));
    propogate->setEnabled(true);
    allowCustEdit->setChecked(atoi(db.curRow["AllowCustEdit"]));
    allowCustEdit->setEnabled(true);
    saveSOAButton->setEnabled(true);

    rrList->clear();
    rrList->setEnabled(true);
    // Load up the RR List
    db.query("select * from DNS_Templates_RR where TemplateID = %ld", curTemplateID);
    if (db.rowCount) {
        while (db.getrow()) {
            strcpy(myPri, "");
            if (!strcasecmp("mx", db.curRow["RRType"])) strcpy(myPri, db.curRow["Aux"]);
            (void) new QListViewItem(rrList,
                                     db.curRow["Name"],
                                     db.curRow["RRType"],
                                     myPri,
                                     db.curRow["Data"],
                                     db.curRow["TTL"],
                                     db.curRow["InternalID"]);
        }
    }

    hostName->setText("");
    hostName->setEnabled(true);
    typeList->setCurrentItem(0);
    typeList->setEnabled(true);
    priSpin->setValue(0);
    priSpin->setEnabled(true);
    ipAddress->setText("");
    ipAddress->setEnabled(true);
    rrTTLSpin->setValue(86400);
    rrTTLSpin->setEnabled(true);
    addRRButton->setEnabled(true);
    delRRButton->setEnabled(true);

}

/*
** disableEditElements - Clears the values in the edit area and disables
**                       the fields.
*/

void DNS_Templates::disableEditElements()
{
    templateName->setText("");
    templateName->setEnabled(false);
    refreshSpin->setValue(1);
    refreshSpin->setEnabled(false);
    retrySpin->setValue(1);
    retrySpin->setEnabled(false);
    expireSpin->setValue(1);
    expireSpin->setEnabled(false);
    minimumSpin->setValue(1);
    minimumSpin->setEnabled(false);
    ttlSpin->setValue(1);
    ttlSpin->setEnabled(false);
    isActive->setChecked(false);
    isActive->setEnabled(false);
    propogate->setChecked(false);
    propogate->setEnabled(false);
    allowCustEdit->setChecked(false);
    allowCustEdit->setEnabled(false);
    saveSOAButton->setEnabled(false);
    rrList->clear();
    rrList->setEnabled(false);
    hostName->setText("");
    hostName->setEnabled(false);
    typeList->setCurrentItem(0);
    typeList->setEnabled(false);
    priSpin->setValue(0);
    priSpin->setEnabled(false);
    ipAddress->setText("");
    ipAddress->setEnabled(false);
    rrTTLSpin->setValue(86400);
    rrTTLSpin->setEnabled(false);
    addRRButton->setEnabled(false);
    delRRButton->setEnabled(false);
}

/*
** newTemplate - Creates a new domain template entry.
*/

void DNS_Templates::newTemplate()
{
    ADBTable    soaDB("DNS_Templates");
    ADBTable    rrDB("DNS_Templates_RR");
    QListViewItem   *curItem;

    soaDB.setValue("TemplateName",      "New Template");
    soaDB.setValue("Refresh",           28000);
    soaDB.setValue("Retry",             7200);
    soaDB.setValue("Expire",            604800);
    soaDB.setValue("Minimum",           86400);
    soaDB.setValue("TTL",               86400);
    soaDB.setValue("Propogate",         1);
    soaDB.setValue("AllowCustEdit",     0);
    soaDB.setValue("Active",            0);

    soaDB.ins();

    // Now, insert our nameserver entries.
    rrDB.setValue("InternalID",     0);
    rrDB.setValue("TemplateID",     soaDB.getLong("TemplateID"));
    rrDB.setValue("Name",           "");
    rrDB.setValue("RRType",         "NS");
    rrDB.setValue("Data",           "ns1.blarg.net.");
    rrDB.setValue("Aux",            0);
    rrDB.setValue("TTL",            86400);
    rrDB.ins();
    rrDB.setValue("InternalID",     0);
    rrDB.setValue("TemplateID",     soaDB.getLong("TemplateID"));
    rrDB.setValue("Name",           "");
    rrDB.setValue("RRType",         "NS");
    rrDB.setValue("Data",           "ns2.blarg.net.");
    rrDB.setValue("Aux",            0);
    rrDB.setValue("TTL",            86400);
    rrDB.ins();

    // Now, add it into our list
    curItem = new QListViewItem(templateList, 
            soaDB.getStr("TemplateName"),
            "N",
            soaDB.getStr("TemplateID"));
    templateSelected(curItem);

}

/*
** delTemplate - Allows the user to remove an entire template.
*/

void DNS_Templates::delTemplate()
{
    if (!curTemplateID) return;
    if (QMessageBox::warning(this, "Delete Template", "There is no way to undo this action.\n\nAre you sure you want to delete this template?", "&Yes", "&No", 0, 1) == 1) {
        return;
    }
    
    // Check to see if this template is in use in the main DNS database.
    // If it is, we can't delete it.
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    QApplication::setOverrideCursor(waitCursor);
    dnsDB.query("select id from ZoneInfo where TemplateID = %ld", curTemplateID);
    QApplication::restoreOverrideCursor();
    if (dnsDB.rowCount) {
        char tmpStr[1024];
        sprintf(tmpStr, "Unable to delete template.  It is\ncurrently in use by %ld domains.", dnsDB.rowCount);
        QMessageBox::critical(this, "Delete Template", tmpStr);
        return;
    }
    
    // If we made it here, we're good to delete it.
    ADB     DB;

    //fprintf(stderr, "delete from rr where zone = %ld and id = %ld\n", currentZone, atol(recordList->currentItem()->key(5,0)));
    DB.dbcmd("delete from DNS_Templates_RR where TemplateID = %ld", curTemplateID);
    rrList->clear();
    DB.dbcmd("delete from DNS_Templates where TemplateID = %ld", curTemplateID);
    refreshList();

}

/*
** saveSOA - Validates the data on the SOA portion of the edit form and
**           saves it or spits out an error, whichever applies.
*/

void DNS_Templates::saveSOA()
{
    ADBTable    soaDB("DNS_Templates");
    ADB         db;

    // The only thing that really requires validating is the name.
    // Everything else is bound in a spinbox or a checkbox.
    if (templateName->text().isEmpty()) {
        QMessageBox::critical(this, "Save DNS Template", "You may not save a template with no name.");
        return;
    }

    // Check for a duplicate name in the database and warn the user
    // if one is found.  Not critical, but stupid to do.
    db.query("select TemplateID from DNS_Templates where TemplateName = '%s' and TemplateID <> %ld", db.escapeString(templateName->text()), curTemplateID);
    if (db.rowCount) {
        QMessageBox::warning(this, "Save DNS Template", "A duplicate name was found.  Not critical, but not a good idea.\nYou should change the name and save again.");
        return;
    }

    int tmpProp = 0;
    int tmpEdit = 0;
    int tmpAct  = 0;

    if (propogate->isChecked()) tmpProp = 1;
    if (allowCustEdit->isChecked()) tmpEdit = 1;
    if (isActive->isChecked()) tmpAct = 1;

    soaDB.get(curTemplateID);
    soaDB.setValue("TemplateName",      templateName->text());
    soaDB.setValue("Refresh",           refreshSpin->value());
    soaDB.setValue("Retry",             retrySpin->value());
    soaDB.setValue("Expire",            expireSpin->value());
    soaDB.setValue("Minimum",           minimumSpin->value());
    soaDB.setValue("TTL",               ttlSpin->value());
    soaDB.setValue("Propogate",         tmpProp);
    soaDB.setValue("AllowCustEdit",     tmpEdit);
    soaDB.setValue("Active",            tmpAct);
    soaDB.upd();

    // Now, find it in the list to update it.
    QListViewItemIterator it(templateList);
    QListViewItem   *curItem;
    for (; it.current(); it++) {
        curItem = it.current();
        if (atol(curItem->key(TID_COLUMN, 0)) == curTemplateID) {
            // Found it.
            curItem->setText(0, templateName->text());
            if (tmpAct) curItem->setText(1, "Y");
            else curItem->setText(1, "N");
        }
    }
}

/*
** addRR - Adds a resource record to the template.  We will do some
**         very basic validation on it before adding it.
*/

void DNS_Templates::addRR()
{
    if (!curTemplateID) return;

    int tmpType = 0;
    int canAdd = 1;
    char errText[2048];
    int  addrIsIP = 0;
    char    myName[1024];
    char    myAddr[1024];
    char    myPri[1024];
    strcpy(errText, "");
    strcpy(myPri, "");
    QRegExp qreg;

    // What we validate will be determined by what type of record it is.
    // The only thing that should not have a "Host" record is an NS record,
    // for exmaple.

    if (!strlen((const char *) ipAddress->text())) {
        strcat(errText, "You must include a FQDN or IP address.\n");
        canAdd = 0;
    }

    switch (typeList->currentItem()) {
        case 0:                         // A
        case 1:                         // AAAA
        case 2:                         // CNAME
        case 3:                         // HINFO
        case 6:                         // PTR
        case 7:                         // SRV
        case 8:                         // TXT
            // Host and data need to be filled in.
            if (!strlen((const char *) hostName->text())) {
                strcat(errText, "Host field must be filled in.\n");
                canAdd = 0;
            }
            break;
            
        default:
            break;
    }

    // Check the address box to see if it appears to be an IP address.
    // If its not, then we need to add a "." to the end of it.
    qreg.setPattern("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
    if (qreg.search(ipAddress->text()) < 0) {
        //fprintf(stderr, "Not an IP address.\n");
        // Since its not an IP address, check to see if it is a FQDN.
        if (ipAddress->text().find(".") < 0) {
            strcat(errText, "You must enter an IP address or fully qualified host name (i.e. www.blarg.net).\n");
            canAdd = 0;
            // Its a bare host name or not valid.
        } else {
            strcpy(myAddr, ipAddress->text());
            if (myAddr[strlen(myAddr)-1] != '.') strcat(myAddr, ".");
        }
    } else {
        addrIsIP = 1;
        strcpy(myAddr, ipAddress->text());
        //fprintf(stderr, "IS an IP address.\n");
    }

    // Check to see if host is a FQDN.  If it is, we add a "." to it.
    strcpy(myName, hostName->text());
    if (hostName->text().find(".") >= 0) {
        // Yes, it was a FQDN.
        // add a "." to the end of it, but only if it doesn't already have one.
        if (strlen(myName)) {
            if (myName[strlen(myName)-1] != '.') strcat(myName, ".");
        }
    }

    if (!canAdd) {
        strcat(errText, "\nCorrect these errors and try again.");
        QMessageBox::critical(this, "Unable to add record", errText);
    } else {
        // Add the record.
        ADBTable    rrDB("DNS_Templates_RR");
        
        rrDB.setValue("TemplateID",     curTemplateID);
        rrDB.setValue("Name",           myName);
        rrDB.setValue("RRType",         (const char *)typeList->currentText());
        rrDB.setValue("Data",           myAddr);
        rrDB.setValue("Aux",            priSpin->value());
        rrDB.setValue("TTL",            rrTTLSpin->value());
        rrDB.ins();

        if (!strcasecmp("mx", rrDB.curRow["RRType"])) strcpy(myPri, rrDB.curRow["Aux"]);
        (void) new QListViewItem(rrList,
                                 rrDB.curRow["Name"],
                                 rrDB.curRow["RRType"],
                                 myPri,
                                 rrDB.curRow["Data"],
                                 rrDB.curRow["TTL"],
                                 rrDB.curRow["InternalID"]);

    }

}

/*
** delRR - Allows the user to remove an RR.
*/

void DNS_Templates::delRR()
{
    if (!curTemplateID) return;
    if (rrList->selectedItem() == NULL) return;
    if (QMessageBox::warning(this, "Delete Resource Record", "There is no way to undo this action.\n\nAre you sure you want to delete this resource record?", "&Yes", "&No", 0, 1) == 1) {
        return;
    }

    // If we made it here, we're good to delete it.
    ADB     rrDB;

    //fprintf(stderr, "delete from rr where zone = %ld and id = %ld\n", currentZone, atol(recordList->currentItem()->key(5,0)));
    rrDB.dbcmd("delete from DNS_Templates_RR where TemplateID = %ld and InternalID = %ld", curTemplateID, atol(rrList->selectedItem()->key(5,0)));
    delete rrList->selectedItem();
}

