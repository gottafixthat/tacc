/*
** $Id: DomainAdd.cpp,v 1.5 2004/01/22 00:02:10 marc Exp $
**
***************************************************************************
**
** DomainAdd - Allows a customer to add a new domain.
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
** $Log: DomainAdd.cpp,v $
** Revision 1.5  2004/01/22 00:02:10  marc
** Virtual Hosting database access is now complete in a basic form.
**
** Revision 1.4  2004/01/21 22:33:00  marc
** Will now add a VHost record into the database as well.
**
** Revision 1.3  2004/01/11 16:57:53  marc
** Will now optionally create DNS and Email Alias/Hosting records
**
** Revision 1.2  2004/01/10 21:59:03  marc
** More changes to DomainAdd.  Auto widget enables, etc.  Not much left to do.
**
** Revision 1.1  2004/01/09 01:55:05  marc
** New widget to include the framework for instant domain, email and web
** hosting setups.
**
**
*/


#include "DomainAdd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "BlargDB.h"
#include "BString.h"
#include "ParseFile.h"
#include <Cfg.h>
#include <ADB.h>
#include <DNSUtils.h>
#include <VHostUtils.h>
#include <BrassClient.h>
#include <qmessagebox.h>
#include <qlayout.h>

DomainAdd::DomainAdd
(
	QWidget* parent,
	const char* name,
	long CustomerID
) : TAAWidget( parent, name )
{
	setCaption( "Add Domain" );

    // Create our widgets.
    QLabel  *customerIDLabel = new QLabel(this, "customerIDLabel");
    customerIDLabel->setText("Customer ID:");
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);

    customerID = new QLabel(this, "customerID");
    customerID->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *customerNameLabel = new QLabel(this, "customerNameLabel");
    customerNameLabel->setText("Customer Name:");
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);

    customerName = new QLabel(this, "customerName");
    customerName->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *domainTypeLabel = new QLabel(this, "domainTypeLabel");
    domainTypeLabel->setText("Domain Type:");
    domainTypeLabel->setAlignment(AlignRight|AlignVCenter);

    domainType = new QComboBox(false, this, "domainType");
    connect(domainType, SIGNAL(activated(int)), this, SLOT(typeChanged(int)));

    QLabel  *loginIDLabel = new QLabel(this, "loginIDLabel");
    loginIDLabel->setText("Login ID:");
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);

    loginID = new QComboBox(false, this, "loginID");

    QLabel  *domainNameLabel = new QLabel(this, "domainNameLabel");
    domainNameLabel->setText("Domain Name:");
    domainNameLabel->setAlignment(AlignRight|AlignVCenter);

    domainName = new QLineEdit(this, "domainName");
    domainName->setMaxLength(64);

    autoDNS = new QCheckBox(this, "autoDNS");
    autoDNS->setChecked(true);
    autoDNS->setEnabled(false);
    autoDNS->setText("Automatically Setup DNS Records");

    autoAliases = new QCheckBox(this, "autoAliases");
    autoAliases->setChecked(true);
    autoAliases->setEnabled(false);
    autoAliases->setText("Automatically Setup Mail Aliases");

    autoHosting = new QCheckBox(this, "autoHosting");
    autoHosting->setChecked(true);
    //autoHosting->setEnabled(false);
    autoHosting->setText("Automatically Setup Web Hosting");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDomain()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelDomain()));


    // Create our layout.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // Most of the widgets go into the grid layout.
    QGridLayout *gl = new QGridLayout(8, 2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;
    gl->addWidget(customerIDLabel,              curRow, 0);
    gl->addWidget(customerID,                   curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(customerNameLabel,            curRow, 0);
    gl->addWidget(customerName,                 curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(domainTypeLabel,              curRow, 0);
    gl->addWidget(domainType,                   curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(loginIDLabel,                 curRow, 0);
    gl->addWidget(loginID,                      curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(domainNameLabel,              curRow, 0);
    gl->addWidget(domainName,                   curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(autoDNS,                      curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(autoAliases,                  curRow, 1);
    gl->setRowStretch(curRow++, 0);
    gl->addWidget(autoHosting,                  curRow, 1);
    gl->setRowStretch(curRow++, 0);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

    // Create the layout for our buttons.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    // Done with layout.

	
	myCustID = CustomerID;
	
	CustomersDB CDB;
	CDB.get(myCustID);
	customerID->setText(CDB.getStr("CustomerID"));
	customerName->setText(CDB.getStr("FullName"));
	
	ADB         DB;
	// Fill the domain type list.
	DB.query("select InternalID, DomainType from DomainTypes order by DomainType");
	domainTypeIDX = new(int[DB.rowCount + 1]);
	int idxPtr = 0;
	while (DB.getrow()) {
	    domainTypeIDX[idxPtr] = atoi(DB.curRow["InternalID"]);
	    domainType->insertItem(DB.curRow["DomainType"]);
	    idxPtr++;
	}
    domainTypeIDX[idxPtr] = 0;
	
	// Fill the login list for the logins on this customer.
	DB.query("select LoginID from Logins where CustomerID = %ld and Active = 1 order by LoginID", myCustID);
	while (DB.getrow()) {
	    loginID->insertItem(DB.curRow["LoginID"]);
	}
    // Grab the primary login from the customer record and set that as
    // the default login ID.
    DB.query("select PrimaryLogin from Customers where CustomerID = %ld", myCustID);
    if (DB.rowCount) {
        DB.getrow();
        loginID->setCurrentText(DB.curRow["PrimaryLogin"]);
    }

    setMostPopular();
}


DomainAdd::~DomainAdd()
{
    if (domainTypeIDX != NULL) delete(domainTypeIDX);
}


/*
** saveDomain   - The slot that gets called when the user clicks the save
**                button.
*/

void DomainAdd::saveDomain()
{
    // Validate the domain name before we save.
    QString     tmpqstr;
    QString     tmpqstr2;
    QString     ext;
    QStrList    parts;
    QStrList    validtlds;
    char        tmpstr[1024];
    char        dstFile[1024];
    char        tmpLogin[1024];
    char        tmpDomain[1024];
    DomainsDB   DDB;
    ADB         DB;
    char        today[64];
    QTime       theTime;
    QString     domainpart;
    QString     tldpart;
    
    char        HostmasterAddress[128];
    
    strcpy(HostmasterAddress, cfgVal("HostmasterAccount"));
    splitString(domainName->text(), '.', parts);

    if (parts.count() != 2) {
        sprintf(tmpstr, "The invalid domain name '%s'.\nValid domainnames are 'domain.name'.", (const char *)domainName->text());
        QMessageBox::critical(this, "Invalid Domain Name", tmpstr, "&Ok");
        return;
    }
    domainpart  = parts.at(0);
    tldpart     = parts.at(1);
    
    QDatetomyDate(today, QDate::currentDate());
    theTime = QTime::currentTime();
    
    tmpqstr = domainName->text();
    tmpqstr2 = tmpqstr.right(4);
    ext = tmpqstr2.lower();

    // Check our list of TLD's and make sure that this one is valid.
    splitString(cfgVal("ValidTLDs"), ':', validtlds);
    int notValid = 1;
    for (unsigned int i = 0; i < validtlds.count(); i++) {
        if (!strcmp(validtlds.at(i), tldpart)) notValid = 0;
    }
    
    /*
    int notValid = 1;
    if (!strcmp(".com", ext)) notValid = 0;
    if (!strcmp(".org", ext)) notValid = 0;
    if (!strcmp(".net", ext)) notValid = 0;
    */
    if (notValid) {
        // Invalid extension.
        sprintf(tmpstr, "The extension '%s', is not a valid\nextension for a domain.", (const char *)ext);
        QMessageBox::critical(this, "Invalid Domain Name", tmpstr, "&Ok");
        return;
    }
    
    // Process the mail to hostmaster...
    tmpnam(dstFile);
    strcpy(tmpLogin, loginID->text(loginID->currentItem()));
    strcpy(tmpDomain, domainName->text());
    parseFile("/usr/local/lib/taa/domain-template.txt", dstFile, myCustID, tmpLogin, tmpDomain);
    sprintf(tmpstr, "cp %s /var/spool/taamail/new-domain-%s-%s", dstFile, tmpDomain, tmpLogin);
    system(tmpstr);
    unlink(dstFile);            // Clean up our tmpfile.
    
    // Now, lets save this bad boy.
    DDB.setValue("CustomerID",  myCustID);
    DDB.setValue("LoginID",     tmpLogin);
    DDB.setValue("DomainType",  domainTypeIDX[domainType->currentItem()]);
    DDB.setValue("DomainName",  tmpDomain);
    DDB.setValue("DateCreated", today);
    DDB.setValue("Active",      1);
    theTime = QTime::currentTime();
    sprintf(tmpstr, "%s %s", today, (const char *) theTime.toString());
    DDB.setValue("LocalSubmit", tmpstr);
    DDB.ins();


    // Check to see if we need to create the DNS records.
    if (autoDNS->isEnabled() && autoDNS->isChecked()) {
        emit(setStatus("Creating DNS entries..."));
        DB.query("select TemplateID from DomainTypes where InternalID = %d", domainTypeIDX[domainType->currentItem()]);
        DB.getrow();

        // Check to see if we're already doing DNS for this domain
        if (hasDNSRecord(tmpDomain)) {
            QMessageBox::critical(this, "DNS Error", "DNS Records are already present for this domain.\nSkipping DNS record creation.");
        } else {
            setupDNSFromTemplate(atoi(DB.curRow["TemplateID"]), tmpDomain, myCustID, tmpLogin);
        }

        emit(setStatus(""));
    }
    
    // Check to see if we need to create the email aliases now.
    if (autoAliases->isEnabled() && autoAliases->isChecked()) {
        emit(setStatus("Setting up mail server aliases..."));
        createDomainAliases(myCustID, 1, tmpDomain);
        emit(setStatus(""));
    }

    // Check to see if we need to create the hosting entry.
    if (autoHosting->isEnabled() && autoHosting->isChecked()) {
        emit(setStatus("Setting up virtual hosting..."));
        if (hasVHostRecord(tmpDomain)) {
            QMessageBox::critical(this, "VHost Error", "There is already a virtual host entry for this domain.\nVirtual Hosting Entry not setup.");
        } else {
            if (!createVHostRecord(tmpDomain, myCustID, tmpLogin)) {
                QMessageBox::critical(this, "VHost Error", "There was an error creating the Virtual Hosting entry.\nVirtual Hosting Entry not setup.");
            }
        }
        emit(setStatus(""));
    }

    emit(domainSaved(1));
    close();
}

/*
** cancelDomain - The slot that gets called when the user clicks the cancel
**                button.
*/

void DomainAdd::cancelDomain()
{
    close();
}

/*
** typeChanged - Gets called when the domain type selection changes.
**               It updates the checkboxes for the allowed actions on
**               the domain.
*/

void DomainAdd::typeChanged(int newType)
{
    ADB     DB;

    // Load the selected domain type and check to see what is allowed
    // for this type of domain.
    DB.query("select * from DomainTypes where InternalID = %d", domainTypeIDX[newType]);
    if (DB.rowCount) {
        DB.getrow();
        autoDNS->setEnabled(atoi(DB.curRow["TemplateID"]));
        autoAliases->setEnabled(atoi(DB.curRow["AllowAliases"]));
        autoHosting->setEnabled(atoi(DB.curRow["AllowWebHosting"]));
    } else {
        autoDNS->setEnabled(false);
        autoAliases->setEnabled(false);
    }
}

/*
** setMostPopular - Gets the most popular domain type and sets our list to it.
*/

void DomainAdd::setMostPopular()
{
    ADB     DB;
    ADB     DB2;
    int     tmpIDX = 0;
    unsigned long    lastNum = 0;

    while(domainTypeIDX[tmpIDX]) {
        DB.query("select InternalID from Domains where DomainType = %d and Active > 0", domainTypeIDX[tmpIDX]);
        if (DB.rowCount > lastNum) {
            lastNum = DB.rowCount;
            domainType->setCurrentItem(tmpIDX);
        }
        tmpIDX++;
    }
}
