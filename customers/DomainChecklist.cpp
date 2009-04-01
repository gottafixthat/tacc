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


#include <stdlib.h>

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <BlargDB.h>
#include <ADB.h>
#include <TAATools.h>

#include "DomainChecklist.h"

using namespace Qt;

DomainChecklist::DomainChecklist(QWidget* parent, const char* name, long DomainID) :
	TAAWidget( parent, name )
{
	setCaption( "Domain Checklist" );
	
	if (!DomainID) return;
	myDomainID = DomainID;

    // Setup the widgets.  There are a bunch.
    QLabel  *customerIDLabel = new QLabel("Customer ID:", this, "customerIDLabel");
    customerIDLabel->setAlignment(AlignRight|AlignVCenter);

    customerID = new QLabel(this, "customerID");
    customerID->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *loginIDLabel = new QLabel("Login ID:", this, "loginIDLabel");
    loginIDLabel->setAlignment(AlignRight|AlignVCenter);

    loginID = new QLabel(this, "loginID");
    loginID->setAlignment(AlignLeft|AlignVCenter);
	
    QLabel  *customerNameLabel = new QLabel("Customer Name:", this, "customerNameLabel");
    customerNameLabel->setAlignment(AlignRight|AlignVCenter);

    customerName = new QLabel(this, "customerName");
    customerName->setAlignment(AlignLeft|AlignVCenter);

    QLabel  *domainNameLabel = new QLabel("Domain Name:", this, "domainNameLabel");
    domainNameLabel->setAlignment(AlignRight|AlignVCenter);

    domainName = new QLabel(this, "domainName");
    domainName->setAlignment(AlignLeft|AlignVCenter);

    hostmasterSubmit = new QCheckBox("Request submitted to Hostmaster", this, "hostmasterSubmit");
    hostmastDate = new QLabel(this, "hostmastDate");
    hostmastDate->setAlignment(AlignRight|AlignVCenter);

    internicSubmit = new QCheckBox("Request submitted to InterNIC", this, "internicSubmit");
    nicReqDate = new QLabel(this, "nicReqDate");
    nicReqDate->setAlignment(AlignRight|AlignVCenter);

    dnsDone = new QCheckBox("DNS records created/updated", this, "dnsDone");
    dnsDate = new QLabel(this, "dnsDate");
    dnsDate->setAlignment(AlignRight|AlignVCenter);

    mailDone = new QCheckBox("Mail system updated", this, "mailDone");
    mailDate = new QLabel(this, "mailDate");
    mailDate->setAlignment(AlignRight|AlignVCenter);

    vserverDone = new QCheckBox("Virtual server setup completed", this, "vserverDone");
    vserverDate = new QLabel(this, "vserverDate");
    vserverDate->setAlignment(AlignRight|AlignVCenter);

    QLabel *serverListLabel = new QLabel("Server:", this, "serverListLabel");
    serverListLabel->setAlignment(AlignRight|AlignVCenter);

    serverList = new QComboBox(true, this, "serverList");

    QLabel *processListLabel = new QLabel("Process:", this, "processListLabel");
    processListLabel->setAlignment(AlignRight|AlignVCenter);

    processList = new QComboBox(true, this, "processList");

    QLabel *ipAddressLabel = new QLabel("IP Address:", this, "ipAddressLabel");
    ipAddressLabel->setAlignment(AlignRight|AlignVCenter);

    ipAddress = new QLineEdit(this, "ipAddress");
    ipAddress->setMaxLength(16);

    internicDone = new QCheckBox("InterNIC request completed", this, "internicDone");
    nicDoneDate = new QLabel(this, "nicDoneDate");
    nicDoneDate->setAlignment(AlignRight|AlignVCenter);

    QLabel *nicAdminIDLabel = new QLabel("NIC Admin ID:", this, "nicAdminIDLabel");
    nicAdminIDLabel->setAlignment(AlignRight|AlignVCenter);
    nicAdminID = new QLineEdit(this, "nicAdminID");
    nicAdminID->setMaxLength(16);

    QLabel *nicBillIDLabel = new QLabel("NIC Billing ID:", this, "nicBillIDLabel");
    nicBillIDLabel->setAlignment(AlignRight|AlignVCenter);
    nicBillID = new QLineEdit(this, "nicBillID");
    nicBillID->setMaxLength(16);


    domainDone = new QCheckBox("Domain released", this, "domainDone");
    doneDate = new QLabel(this, "domainDate");
    doneDate->setAlignment(AlignRight|AlignVCenter);

    // Finally, our buttons.
    QPushButton *updateButton = new QPushButton("&Update", this, "updateButton");
    connect(updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));

    QPushButton *saveButton = new QPushButton("&Save", this, "saveButton");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    QPushButton *cancelButton = new QPushButton("&Cancel", this, "cancelButton");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    // And now for the layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 5);

    // Our info grid.
    Q3GridLayout *igrid = new Q3GridLayout(3, 4, 3);
    int curRow = 0;
    igrid->addWidget(customerIDLabel,   curRow, 0);
    igrid->addWidget(customerID,        curRow, 1);
    igrid->addWidget(loginIDLabel,      curRow, 2);
    igrid->addWidget(loginID,           curRow, 3);
    igrid->setRowStretch(curRow, 0);

    curRow++;
    igrid->addWidget(customerNameLabel,     curRow, 0);
    igrid->addMultiCellWidget(customerName, curRow, curRow, 1, 3);
    igrid->setRowStretch(curRow, 0);

    curRow++;
    igrid->addWidget(domainNameLabel,       curRow, 0);
    igrid->addMultiCellWidget(domainName,   curRow, curRow, 1, 3);
    igrid->setRowStretch(curRow, 0);

    igrid->setColStretch(0, 0);
    igrid->setColStretch(1, 1);
    igrid->setColStretch(2, 0);
    igrid->setColStretch(3, 0);

    ml->addLayout(igrid, 0);
    ml->addWidget(new HorizLine(this), 0);

    Q3GridLayout *gl = new Q3GridLayout(12, 3, 3);

    curRow = 0;
    gl->addMultiCellWidget(hostmasterSubmit,    curRow, curRow, 0, 1);
    gl->addWidget(hostmastDate,                 curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(internicSubmit,      curRow, curRow, 0, 1);
    gl->addWidget(nicReqDate,                   curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(dnsDone,             curRow, curRow, 0, 1);
    gl->addWidget(dnsDate,                      curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(mailDone,            curRow, curRow, 0, 1);
    gl->addWidget(mailDate,                     curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(vserverDone,         curRow, curRow, 0, 1);
    gl->addWidget(vserverDate,                  curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(serverListLabel,              curRow, 0);
    gl->addMultiCellWidget(serverList,          curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(processListLabel,             curRow, 0);
    gl->addMultiCellWidget(processList,         curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(ipAddressLabel,               curRow, 0);
    gl->addMultiCellWidget(ipAddress,           curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(internicDone,        curRow, curRow, 0, 1);
    gl->addWidget(nicDoneDate,                  curRow, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(nicAdminIDLabel,              curRow, 0);
    gl->addMultiCellWidget(nicAdminID,          curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(nicBillIDLabel,               curRow, 0);
    gl->addMultiCellWidget(nicBillID,           curRow, curRow, 1, 2);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addMultiCellWidget(domainDone,          curRow, curRow, 0, 1);
    gl->addWidget(doneDate,                     curRow, 2);
    gl->setRowStretch(curRow, 0);

    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    gl->setColStretch(2, 0);

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    ml->addWidget(new HorizLine(this), 0);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(updateButton, 0);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);
    ml->addLayout(bl, 0);



	refreshCheckList();
}


DomainChecklist::~DomainChecklist()
{
}

/*
** refreshCheckList - Loads the data from the database and displays it
**                    on the screen.
*/

void DomainChecklist::refreshCheckList(void)
{
    ADB             DB;
    DomainsDB       DDB;
    char            tmpst[64];
    
    DDB.get(myDomainID);
    
    myCustID = DDB.getLong("CustomerID");
    customerID->setText(DDB.getStr("CustomerID"));
    loginID->setText(DDB.getStr("LoginID"));
    domainName->setText(DDB.getStr("DomainName"));
    
    if (strlen(DDB.getStr("LocalSubmit"))) {
        hostmasterSubmit->setChecked(TRUE);
        hostmasterSubmit->setEnabled(FALSE);
        hostmastDate->setText(DDB.getStr("LocalSubmit"));
    } else {
        hostmasterSubmit->setChecked(FALSE);
        hostmasterSubmit->setEnabled(TRUE);
        hostmastDate->setText("");
    }

    if (strlen(DDB.getStr("NICSubmit"))) {
        internicSubmit->setChecked(TRUE);
        internicSubmit->setEnabled(FALSE);
        nicReqDate->setText(DDB.getStr("NICSubmit"));
    } else {
        internicSubmit->setChecked(FALSE);
        internicSubmit->setEnabled(TRUE);
        nicReqDate->setText("");
    }

    if (strlen(DDB.getStr("DNSUpdated"))) {
        dnsDone->setChecked(TRUE);
        dnsDone->setEnabled(FALSE);
        dnsDate->setText(DDB.getStr("DNSUpdated"));
    } else {
        dnsDone->setChecked(FALSE);
        dnsDone->setEnabled(TRUE);
        dnsDate->setText("");
    }

    if (strlen(DDB.getStr("MailSystemUpdated"))) {
        mailDone->setChecked(TRUE);
        mailDone->setEnabled(FALSE);
        mailDate->setText(DDB.getStr("MailSystemUpdated"));
    } else {
        mailDone->setChecked(FALSE);
        mailDone->setEnabled(TRUE);
        mailDate->setText("");
    }

    if (strlen(DDB.getStr("VirtServerSetup"))) {
        vserverDone->setChecked(TRUE);
        vserverDone->setEnabled(FALSE);
        vserverDate->setText(DDB.getStr("VirtServerSetup"));
    } else {
        vserverDone->setChecked(FALSE);
        vserverDone->setEnabled(TRUE);
        vserverDate->setText("");
    }

    if (strlen(DDB.getStr("NICCompleted"))) {
        internicDone->setChecked(TRUE);
        internicDone->setEnabled(FALSE);
        nicDoneDate->setText(DDB.getStr("NICCompleted"));
    } else {
        internicDone->setChecked(FALSE);
        internicDone->setEnabled(TRUE);
        nicDoneDate->setText("");
    }

    if (strlen(DDB.getStr("Released"))) {
        domainDone->setChecked(TRUE);
        domainDone->setEnabled(FALSE);
        doneDate->setText(DDB.getStr("Released"));
    } else {
        domainDone->setChecked(FALSE);
        domainDone->setEnabled(TRUE);
        doneDate->setText("");
    }

    ipAddress->setText(DDB.getStr("IPAddress"));
    nicAdminID->setText(DDB.getStr("NICAdminID"));
    nicBillID->setText(DDB.getStr("NICBillID"));
    
    strcpy(tmpst, DDB.getStr("Server"));
    serverList->clear();
    DB.query("select distinct Server from Domains order by Server");
    if (DB.rowCount) while (DB.getrow()) {
        serverList->insertItem(DB.curRow["Server"]);
        if (!strcmp(tmpst, DB.curRow["Server"])) {
            serverList->setCurrentItem(serverList->count() - 1);
        }
    }

    strcpy(tmpst, DDB.getStr("SubServer"));
    processList->clear();
    DB.query("select distinct SubServer from Domains order by SubServer");
    if (DB.rowCount) while (DB.getrow()) {
        processList->insertItem(DB.curRow["SubServer"]);
        // printf("tmpst = '%s', DB.curRow[0] = '%s'\n", tmpst, DB.curRow[0]);
        if (!strcmp(tmpst, DB.curRow["SubServer"])) {
            processList->setCurrentItem(processList->count() - 1);
        }
    }
}

/*
** saveDomainChecklist - Saves any text fields the user may have changed.
*/

void DomainChecklist::saveChecklist()
{
    DomainsDB       DDB;
    
    DDB.get(myDomainID);
    
    QDate   theDate;
    QTime   theTime;
    char    curTimeSt[64];
    
    theDate = QDate::currentDate();
    theTime = QTime::currentTime();
    sprintf(curTimeSt, "%04d-%02d-%02d %s", theDate.year(), theDate.month(), theDate.day(), (const char *) theTime.toString());

    // Scan through the fields to see if anything different is checked.
    if (hostmasterSubmit->isEnabled() && hostmasterSubmit->isChecked()) {
        DDB.setValue("LocalSubmit", curTimeSt);
    }

    if (internicSubmit->isEnabled() && internicSubmit->isChecked()) {
        DDB.setValue("NICSubmit", curTimeSt);
    }

    if (dnsDone->isEnabled() && dnsDone->isChecked()) {
        DDB.setValue("DNSUpdated", curTimeSt);
    }

    if (mailDone->isEnabled() && mailDone->isChecked()) {
        DDB.setValue("MailSystemUpdated", curTimeSt);
    }

    if (vserverDone->isEnabled() && vserverDone->isChecked()) {
        DDB.setValue("VirtServerSetup", curTimeSt);
    }

    if (internicDone->isEnabled() && internicDone->isChecked()) {
        DDB.setValue("NICCompleted", curTimeSt);
    }

    if (domainDone->isEnabled() && domainDone->isChecked()) {
        DDB.setValue("Released", curTimeSt);
    }
    
    DDB.setValue("IPAddress", ipAddress->text());
    DDB.setValue("NICAdminID", nicAdminID->text());
    DDB.setValue("NICBillID",  nicBillID->text());
    DDB.setValue("Server", serverList->text(serverList->currentItem()));
    DDB.setValue("SubServer", processList->text(processList->currentItem()));

    DDB.upd();
    emit(customerUpdated(myCustID));
}

/*
** cancelClicked  - The slot that gets called when the user clicks the
**                  cancel button.
*/

void DomainChecklist::cancelClicked()
{
    close();
}

/*
** saveClicked  - The slot that gets called when the user clicks the
**                save button.
*/

void DomainChecklist::saveClicked()
{
    saveChecklist();
    close();
}

/*
** saveClicked  - The slot that gets called when the user clicks the
**                update button.
*/

void DomainChecklist::updateClicked()
{
    saveChecklist();
    refreshCheckList();
}


// vim: expandtab

