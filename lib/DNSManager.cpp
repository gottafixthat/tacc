/*
** $Id$
**
***************************************************************************
**
** DNSManager - A tool that allows a TAA user to manage DNS entries for a
**              customer or a template.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2003, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNSManager.cpp,v $
** Revision 1.6  2004/03/15 22:33:50  marc
** Fixed a bad escape sequence in a regex
**
** Revision 1.5  2004/01/12 22:57:54  marc
** Updated updateSOA to also remove the TemplateID from the ZoneInfo
**
** Revision 1.4  2004/01/04 20:01:02  marc
** Added a bit more the the DNS Templates/Managed DNS tools/widgets
**
** Revision 1.3  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.2  2004/01/02 14:59:01  marc
** DNSTemplatePicker complete, added DNSUtils
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <DNSManager.h>
#include <TAATools.h>
#include <TAAWidget.h>
#include <ADB.h>
#include <Cfg.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qregexp.h>


// TAA DNS Manager class/widget

DNSManager::DNSManager(QWidget *parent, const char *name)
    : TAAWidget(parent, name)
{
    setCaption("DNS Manager");

    currentZone = 0;

    QLabel *domainNameLabel = new QLabel(this);
    domainNameLabel->setAlignment(AlignRight);
    domainNameLabel->setText("Domain Name:");
    domainName = new QLabel(this);
    domainName->setText("domainname.com");
    
    QLabel *domainTypeLabel = new QLabel(this);
    domainTypeLabel->setAlignment(AlignRight);
    domainTypeLabel->setText("Domain Type:");
    domainType = new QLabel(this);
    //domainType->setText("std/soho/etc");
    
    QLabel *lastModifiedLabel = new QLabel(this);
    lastModifiedLabel->setAlignment(AlignRight);
    lastModifiedLabel->setText("Last Updated:");
    lastModified = new QLabel(this);
    lastModified->setText("11/28/2003 02:14p");
    
    recordList = new QListView(this);
    recordList->addColumn("Host");
    recordList->addColumn("Type");
    recordList->addColumn("Pri");
    recordList->addColumn("IP Address/FQDN");
    recordList->addColumn("Act");
    recordList->addColumn("ID");
    recordList->setAllColumnsShowFocus(true);

    // New entry options
    hostBoxLabel = new QLabel(this);
    hostBoxLabel->setAlignment(AlignRight|AlignVCenter);
    hostBoxLabel->setText("Host:");

    hostBox = new QLineEdit(this);
    hostBox->setMaxLength(64);

    typeListLabel = new QLabel(this);
    typeListLabel->setAlignment(AlignRight|AlignVCenter);
    typeListLabel->setText("Type:");

    typeList = new QComboBox(this);
    typeList->insertItem("A");
    typeList->insertItem("CNAME");
    typeList->insertItem("MX");
    typeList->insertItem("NS");
    typeList->insertItem("PTR");

    priSpinnerLabel = new QLabel(this);
    priSpinnerLabel->setAlignment(AlignRight|AlignVCenter);
    priSpinnerLabel->setText("Priority:");

    priSpinner = new QSpinBox(this);
    priSpinner->setMinValue(0);
    priSpinner->setMaxValue(1000);
    priSpinner->setValue(10);

    addressBoxLabel = new QLabel(this);
    addressBoxLabel->setAlignment(AlignRight|AlignVCenter);
    addressBoxLabel->setText("IP/FQHN:");

    addressBox = new QLineEdit(this);
    addressBox->setMaxLength(64);

    addButton = new QPushButton(this);
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addRR()));

    
    // Our "action" buttons
    //QPushButton *save = new QPushButton(this);
    //save->setText("&Save");
    QPushButton *delZoneButton = new QPushButton(this);
    delZoneButton->setText("Delete &Zone");
    connect(delZoneButton, SIGNAL(clicked()), this, SLOT(delZone()));


    QPushButton *delButton = new QPushButton(this);
    delButton->setText("&Delete Record");
    connect(delButton, SIGNAL(clicked()), this, SLOT(delRR()));

    QPushButton *cancel = new QPushButton(this);
    cancel->setText("&Close");
    connect(cancel, SIGNAL(clicked()), SLOT(close()));



    // Create our layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QGridLayout *ll = new QGridLayout(2,6,2);
    ll->addWidget(domainNameLabel,          0, 0);
    ll->addWidget(domainName,               0, 1);
    ll->addWidget(domainTypeLabel,          1, 0);
    ll->addWidget(domainType,               1, 1);
    ll->addWidget(lastModifiedLabel,        2, 0);
    ll->addWidget(lastModified,             2, 1);

    ml->addLayout(ll, 0);

    ml->addWidget(recordList, 1);

    QBoxLayout *actl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    actl->addWidget(hostBoxLabel,   0);
    actl->addWidget(hostBox,        1);
    actl->addWidget(typeListLabel,  0);
    actl->addWidget(typeList,       0);
    actl->addWidget(priSpinnerLabel,0);
    actl->addWidget(priSpinner,     0);
    actl->addWidget(addressBoxLabel,0);
    actl->addWidget(addressBox,     1);
    actl->addWidget(addButton,      0);

    ml->addLayout(actl, 0);


    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    //bl->addWidget(save, 0);
    bl->addWidget(delZoneButton,0);
    bl->addWidget(delButton,    0);
    bl->addWidget(cancel,       0);

    ml->addLayout(bl, 0);
}

DNSManager::~DNSManager()
{
}

/*
** setDomainName - Sets the domain name we are working with.
**                 This sets the various labels and loads in the records
**                 from the master SQL server.
*/

int DNSManager::setDomainName(const char *domain)
{
    int     retVal = 1;     // 1/true = Success
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char    recType[1024];
    char    priority[1024];
    char    recAction[1024];

    dnsDB.query("select * from soa where origin = '%s.'", dnsDB.escapeString(domain));
    if (!dnsDB.rowCount) return 0;

    dnsDB.getrow();
    currentZone = atol(dnsDB.curRow["id"]);

    domainName->setText(dnsDB.curRow["origin"]);
    /*
    switch (atoi(dnsDB.curRow["DomainType"])) {
        case 0:
            domainType->setText("Template");
            break;

        case 1:
            domainType->setText("Secondary/Slave");
            break;

        case 2:
            domainType->setText("SOHO Secondary/Slave");
            break;

        case 3:
            domainType->setText("SOHO");
            break;

        case 4:
            domainType->setText("Standard (from Template)");
            break;

        case 5:
            domainType->setText("Standard");
            break;

        case 6:
            domainType->setText("Reverse Records");
            break;

        default:
            domainType->setText("Uknown!");
            break;
    }
    */

    // Fill in the list.
    dnsDB.query("select * from rr where zone = %ld", currentZone);
    if (dnsDB.rowCount) while (dnsDB.getrow()) {
        strcpy(priority,    "");
        strcpy(recAction,   "");
        strcpy(recType,     dnsDB.curRow["type"]);
        if (!strcasecmp("mx", dnsDB.curRow["type"])) {
            strcpy(priority, dnsDB.curRow["aux"]);
        }
        (void) new QListViewItem(recordList,
                                 dnsDB.curRow["name"],
                                 recType,
                                 priority,
                                 dnsDB.curRow["data"],
                                 recAction,
                                 dnsDB.curRow["id"]);

    }

    return retVal;
}

/*
** updateSOASerial - Updates the serial number for the zone.
*/

void DNSManager::updateSOASerial()
{
    ADB     soaDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char    curDate[1024];
    char    newSer[1024];
    QDate   tmpDate(QDate::currentDate());
    QString tmpQS;

    soaDB.query("select serial from soa where id = %ld", currentZone);
    if (soaDB.rowCount) { 
        soaDB.getrow();
        tmpQS = soaDB.curRow["serial"];
        sprintf(curDate, "%04d%02d%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
        sprintf(newSer, "%s01", curDate);
        if (!strcmp(curDate, tmpQS.left(8))) {
            // The serial number is from today.
            // Increment the last two digits by one.
            sprintf(newSer, "%s%02d", curDate, atoi(tmpQS.right(2))+1);
        }
        soaDB.dbcmd("update soa set serial = '%s' where id = %ld", newSer, currentZone);

        // Since this is only called when an RR is changed, it means
        // that it no longer matches any templates.  Set the Template ID
        // to 0 to make sure automatic template updates don't wipe out the
        // custom changes.
        soaDB.dbcmd("update ZoneInfo set TemplateID = 0 where zoneid = %ld", currentZone);
    }
    
}

/*
** addRR - Adds a resource record based on the user's input.
**         we'll try to do some basic validation on it before we add it.
*/

int DNSManager::addRR()
{
    if (!currentZone) return 0;
    int retVal = 1;
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
    // In order to put this into a switch, lets turn the type into an int.
    if (!strcasecmp("A", typeList->currentText())) tmpType = 1;
    else if (!strcasecmp("CNAME", typeList->currentText())) tmpType = 3;
    else if (!strcasecmp("MX", typeList->currentText())) tmpType = 5;
    else if (!strcasecmp("NS", typeList->currentText())) tmpType = 6;
    else if (!strcasecmp("PTR", typeList->currentText())) tmpType = 7;
    
    if (!tmpType) return 0;

    if (!strlen((const char *) addressBox->text())) {
        strcat(errText, "You must include a FQDN or IP address.\n");
        canAdd = 0;
    }

    switch (tmpType) {
        case 1:                         // A
        case 3:                         // CNAME
        case 7:                         // PTR
            // Host and data need to be filled in.
            if (!strlen((const char *) hostBox->text())) {
                strcat(errText, "Host field must be filled in.\n");
                canAdd = 0;
            }
            break;
            
        default:
            break;
    }

    // Check the address box to see if it appears to be an IP address.
    // If its not, then we need to add a "." to the end of it.
    qreg.setPattern("^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$");
    if (qreg.search(addressBox->text()) < 0) {
        //fprintf(stderr, "Not an IP address.\n");
        // Since its not an IP address, check to see if it is a FQDN.
        if (addressBox->text().find(".") < 0) {
            strcat(errText, "You must enter an IP address or fully qualified host name (i.e. www.avvanta.com).\n");
            canAdd = 0;
            // Its a bare host name or not valid.
        } else {
            strcpy(myAddr, addressBox->text());
            if (myAddr[strlen(myAddr)-1] != '.') strcat(myAddr, ".");
        }
    } else {
        addrIsIP = 1;
        strcpy(myAddr, addressBox->text());
        //fprintf(stderr, "IS an IP address.\n");
    }

    // Check to see if host is a FQDN.  If it is, we add a "." to it.
    strcpy(myName, hostBox->text());
    if (hostBox->text().find(".") >= 0) {
        // Yes, it was a FQDN.
        // add a "." to the end of it, but only if it doesn't already have one.
        if (strlen(myName)) {
            if (myName[strlen(myName)-1] != '.') strcat(myName, ".");
        }
    }

    if (!canAdd) {
        strcat(errText, "\nCorrect these errors and try again.");
        QMessageBox::critical(this, "Unable to add record", errText);
        retVal = 0;
    } else {
        // Add the record.
        ADBTable    dnsDB("rr", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
        
        dnsDB.setValue("zone",      currentZone);
        dnsDB.setValue("name",      myName);
        dnsDB.setValue("type",      (const char *)typeList->currentText());
        dnsDB.setValue("data",      myAddr);
        dnsDB.setValue("aux",       priSpinner->value());
        dnsDB.setValue("ttl",       86400);
        dnsDB.ins();

        if (!strcasecmp("mx", dnsDB.curRow["type"])) strcpy(myPri, dnsDB.curRow["aux"]);
        (void) new QListViewItem(recordList,
                                 dnsDB.curRow["name"],
                                 dnsDB.curRow["type"],
                                 myPri,
                                 dnsDB.curRow["data"],
                                 "",
                                 dnsDB.curRow["id"]);

        // Update the serial number for the domain.
        updateSOASerial();
        
    }

    return retVal;
}

/*
** delRR - Allows the user to remove an RR.
*/

int DNSManager::delRR()
{
    if (recordList->currentItem() == NULL) return 0;
    if (QMessageBox::warning(this, "Delete DNS Entry", "There is no way to undo this action.\n\nAre you sure you want to delete this record?", "&Yes", "&No", 0, 1) == 1) {
        return 0;
    }

    // If we made it here, we're good to delete it.
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));

    //fprintf(stderr, "delete from rr where zone = %ld and id = %ld\n", currentZone, atol(recordList->currentItem()->key(5,0)));
    dnsDB.dbcmd("delete from rr where zone = %ld and id = %ld", currentZone, atol(recordList->currentItem()->key(5,0)));
    delete recordList->currentItem();
    updateSOASerial();

    return 0;
}

/*
** delZone - Allows the user to remove the entire zone.
*/

int DNSManager::delZone()
{
    if (QMessageBox::warning(this, "Delete DNS Zone", "There is no way to undo this action.\n\nAre you sure you want to delete this zone?", "&Yes", "&No", 0, 1) == 1) {
        return 0;
    }

    if (QMessageBox::warning(this, "Delete DNS Zone", "We weren't joking.  There really is no way to undo this.\nThis domain will be a smoking crater if you select yes here.\n\nAre you *really* sure you want to delete this zone?", "&Yes", "&No", 0, 1) == 1) {
        return 0;
    }

    // If we made it here, we're good to delete it.
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));

    // Get the customer ID from the ZoneInfo database so we can emit
    // the fact that the info has changed and cause any open Customer ID
    // windows to refresh.
    long myCustID = 0;
    dnsDB.query("select CustomerID from ZoneInfo where zoneid = %ld", currentZone);
    if (dnsDB.rowCount) {
        dnsDB.getrow();
        myCustID = atol(dnsDB.curRow["CustomerID"]);
    }
    //fprintf(stderr, "delete from rr where zone = %ld and id = %ld\n", currentZone, atol(recordList->currentItem()->key(5,0)));
    dnsDB.dbcmd("delete from rr where zone = %ld", currentZone);
    dnsDB.dbcmd("delete from soa where id = %ld", currentZone);
    dnsDB.dbcmd("delete from ZoneInfo where zoneid = %ld", currentZone);
    if (myCustID) emit(customerUpdated(myCustID));

    QMessageBox::critical(this, "Zone Deleted", "All that remains of this domain's\nDNS records are memories.");

    close();

    return 0;
}
