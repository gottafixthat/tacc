/*
** $Id$
**
***************************************************************************
**
** Tab_Domains - The main place for viewing and editing domains.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Tab_Domains.cpp,v $
** Revision 1.5  2004/01/22 00:02:10  marc
** Virtual Hosting database access is now complete in a basic form.
**
** Revision 1.4  2004/01/09 01:55:19  marc
** Minor changes here and there
**
** Revision 1.3  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.2  2004/01/02 15:00:20  marc
** Added more DNS Management functionality
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "Tab_Domains.h"
#include "BlargDB.h"
#include <qstring.h>
#include <qstrlist.h>
#include <stdlib.h>
#include <stdio.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include "DomainEdit.h"
#include "DomainAdd.h"
#include "DomainChecklist.h"
#include "DomainAliases.h"
#include <ADB.h>
#include <Cfg.h>
#include <TAATools.h>
#include <DNSManager.h>
#include <DNSTemplatePicker.h>
#include <VHostUtils.h>


Tab_Domains::Tab_Domains
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
    // Create the Widgets.
    list    = new QListView(this, "DomainList");
    list->addColumn("Login ID");
    list->addColumn("Type");
    list->addColumn("Name");
    list->addColumn("Server");
    list->addColumn("DNS");
    list->addColumn("Mail");
    list->addColumn("Web");
    list->addColumn("Active");
    list->setAllColumnsShowFocus(TRUE);

    newButton = new QPushButton(this, "New Button");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newDomain()));

    editButton = new QPushButton(this, "Edit Button");
    editButton->setText("&Edit");
    editButton->setEnabled(false);
    //connect(editButton, SIGNAL(clicked()), this, SLOT(editDomain()));
    
    checklistButton = new QPushButton(this, "Checklist Button");
    checklistButton->setText("&Checklist");
    connect(checklistButton, SIGNAL(clicked()), this, SLOT(editChecklist()));

    activeButton = new QPushButton(this, "Activate Button");
    activeButton->setText("(De)ac&tivate");
    connect(activeButton, SIGNAL(clicked()), this, SLOT(deActivateDomain()));

    vhostButton = new QPushButton(this, "VHost Button");
    vhostButton->setText("&Web Hosting");
    connect(vhostButton, SIGNAL(clicked()), this, SLOT(vhostSelected()));

    emailButton = new QPushButton(this, "Email Button");
    emailButton->setText("&Email");
    connect(emailButton, SIGNAL(clicked()), this, SLOT(emailSelected()));

    activitiesButton = new QPushButton(this, "Activities Button");
    activitiesButton->setText("&Activities");
    activitiesButton->setEnabled(false);
    //connect(activitiesButton, SIGNAL(clicked()), this, SLOT(activitiesSelected()));

    trafficButton = new QPushButton(this, "Traffic Button");
    trafficButton->setText("&Traffic");
    trafficButton->setEnabled(false);
    //connect(trafficButton, SIGNAL(clicked()), this, SLOT(trafficSelected()));

    aliasButton = new QPushButton(this, "Alias Button");
    aliasButton->setText("&Aliases");
    connect(aliasButton, SIGNAL(clicked()), this, SLOT(aliasSelected()));

    dnsButton = new QPushButton(this, "DNS Button");
    dnsButton->setText("&DNS");
    connect(dnsButton, SIGNAL(clicked()), this, SLOT(dnsSelected()));
    if (!isManager()) dnsButton->hide();

    // Do our layout.  Its a simple top-down box layout for most of it,
    // then a button group layout for the action buttons.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(list, 1);

    // Add the button layout now.
    QGridLayout *bl = new QGridLayout(2, 5);
    bl->addWidget(newButton,        0, 0);
    bl->addWidget(checklistButton,  0, 1);
    bl->addWidget(vhostButton,      0, 2);
    bl->addWidget(activitiesButton, 0, 3);
    bl->addWidget(aliasButton,      0, 4);

    bl->addWidget(editButton,       1, 0);
    bl->addWidget(activeButton,     1, 1);
    bl->addWidget(emailButton,      1, 2);
    bl->addWidget(trafficButton,    1, 3);
    bl->addWidget(dnsButton,        1, 4);

    for (int i = 0; i < 6; i++) bl->setColStretch(i, 1);
    bl->addColSpacing(5, 1);
    bl->setRowStretch(0, 0);
    bl->setRowStretch(1, 0);

    ml->addLayout(bl, 0);
    
	setCaption( "Domains" );
	
	myCustID = CustID;
	refreshDomainList(0);
}


Tab_Domains::~Tab_Domains()
{
}


/**
 ** refreshDomainsList - A public slot which will get called when an item
 **				         in the Domains list is changed.
 **
 **/

void Tab_Domains::refreshDomainList(int domainID)
{
    ADB     DB;
    char    activeStr[1024];
    char    doingEmail[1024];
    char    hasVHost[1024];
    char    doingDNS[1024];
    QListViewItem *curItem;

    // And clear the list.
    list->clear();

	// Fill in the subscriptions ListBox
    DB.query("select Domains.InternalID, Domains.LoginID, DomainTypes.DomainType, Domains.DomainName, Domains.Active, Domains.HasSSL, Domains.Server from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.DomainType = DomainTypes.InternalID order by DomainName", myCustID);
    //fprintf(stderr, "Query returned %ld rows...\n", DB.rowCount);
    QApplication::setOverrideCursor(waitCursor);
	if (DB.rowCount) {
        ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
        ADB     vhostDB(cfgVal("VHostSQLDB"), cfgVal("VHostSQLUser"), cfgVal("VHostSQLPass"), cfgVal("VHostSQLHost"));
        while(DB.getrow()) {
            //fprintf(stderr, "Internal ID = %ld...\n", DB.curRow["InternalID"]);
            // Active
            if (atoi(DB.curRow["Active"])) strcpy(activeStr, "Yes");
            else strcpy(activeStr, "No");
            
            // Are we accepting email for this domain?
            if (mailDB.query("select VirtualID from Virtual where Mailbox = '%s'", DB.curRow["DomainName"])) {
                if (mailDB.rowCount) strcpy(doingEmail, "Yes");
                else strcpy(doingEmail, "No");
            } else {
                strcpy(doingEmail, "Unk");
            }
            
            // Do we have a Virtual Hosting entry?
            if (vhostDB.query("select VHostID from VHosts where HostName = '%s'", DB.curRow["DomainName"])) {
                if (vhostDB.rowCount) strcpy(hasVHost, "Yes");
                else strcpy(hasVHost, "No");
            } else {
                strcpy(hasVHost, "Unk");
            }
            
            // Check to see if we're doing DNS for this domain
            if (hasDNSRecord(DB.curRow["DomainName"])) strcpy(doingDNS, "Yes");
            else strcpy(doingDNS, "No");
            
            //fprintf(stderr, "Inserting into the list...\n", DB.curRow["InternalID"]);
            curItem = new QListViewItem(list,
                DB.curRow["LoginID"],       // Login ID
                DB.curRow["DomainType"],    // Domain Type
                DB.curRow["DomainName"],    // Domain Name
                DB.curRow["Server"],        // Server
                doingDNS,
                doingEmail,
                hasVHost,
                activeStr);                 // Active
            curItem->setText(8, DB.curRow["InternalID"]);        // Internal ID  - not shown.
            if (atoi(DB.curRow["InternalID"]) == domainID) {
                list->setCurrentItem(curItem);
                list->setSelected(curItem, true);
                list->ensureItemVisible(curItem);
            }
        }
    }
    QApplication::restoreOverrideCursor();
}

/*
** newDomain        - A public slot that gets called when the "New" button
**                    is clicked.
*/

void Tab_Domains::newDomain()
{
    DomainAdd *AD;
    
    AD = new DomainAdd(0,"",myCustID);
    connect(AD, SIGNAL(domainSaved(int)), this, SLOT(refreshDomainList(int)));
    AD->show();
}

/*
** editChecklist    - Brings up the domain checklist for this domain.
*/

void Tab_Domains::editChecklist()
{
    if (list->currentItem() != NULL) {
        DomainChecklist  *DC;
    
        DC = new DomainChecklist(0,"", atoi(list->currentItem()->key(8,0)));
        DC->show();
    }
}

/*
** DeActivateDomain - Activates or Deactivates a domain, depending on
**                    whether or not it is currently active.
*/

void Tab_Domains::deActivateDomain()
{
    if (list->currentItem() != NULL) {
	    QApplication::setOverrideCursor(waitCursor);

	    long        domainID = atol(list->currentItem()->key(8,0));
	    DomainsDB   DDB;
	    char        tmpStr[1024];
	    
	    DDB.get(domainID);
	    QApplication::restoreOverrideCursor();
	    
	    if (DDB.getInt("Active")) {
	        // We want to de-activate this domain.
	        sprintf(tmpStr, "Deactivate the domain '%s'?", (const char *) DDB.getStr("DomainName"));
	        switch(QMessageBox::information(this, "Deactivate Domain", tmpStr, "&Yes", "&No", "&Cancel", 0, 2 )) {
	            case 0:         // yes selected
	                QApplication::setOverrideCursor(waitCursor);
	                DDB.setValue("Active", (int) 0);
	                DDB.upd();
	                refreshDomainList(domainID);
	                QApplication::restoreOverrideCursor();
                    // Now, ask the user if they want to remove the
                    // aliases for this domain.
                    if (!QMessageBox::information(this, "Deactivate Domain", "Do you wish to remove the\naliases for this domain as well?", "&Yes", "&No", 0, 1)) {
	                    QApplication::setOverrideCursor(waitCursor);
                        ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
                        mailDB.dbcmd("delete from Virtual where Address LIKE '%%@%s'", DDB.getStr("DomainName"));
                        mailDB.dbcmd("delete from Virtual where Address = '%s' and Mailbox = '%s'", DDB.getStr("DomainName"), DDB.getStr("DomainName"));
	                    QApplication::restoreOverrideCursor();
                    }
	                break;

	            default:        // Anything else.
	                break;
	        }
	    } else {
	        // We want to activate this domain.
	        sprintf(tmpStr, "Reactivate the domain '%s'?", (const char *) DDB.getStr("DomainName"));
	        switch(QMessageBox::information(this, "Reactivate Domain", tmpStr, "&Yes", "&No", "&Cancel", 0, 2 )) {
	            case 0:         // yes selected
	                QApplication::setOverrideCursor(waitCursor);
	                DDB.setValue("Active", (int) 1);
	                DDB.upd();
	                refreshDomainList(domainID);
	                QApplication::restoreOverrideCursor();
                    // Now, ask the user if they want to recreate the
                    // aliases for this domain.
                    if (!QMessageBox::information(this, "Reactivate Domain", "Do you wish to create the\naliases for this domain as well?", "&Yes", "&No", 0, 1)) {
                        // Now, find out if they want to do the "standard" aliases as well.
                        int    doStockAliases = 1;
                        if (QMessageBox::warning(this, "Domain Aliases", "Do you wish to create the standard\ninfo, sales, support, webmaster aliases?", "&Yes", "&No", 0, 1)) {
                            doStockAliases = 0;
                        }
	                    QApplication::setOverrideCursor(waitCursor);
                        createDomainAliases(myCustID, doStockAliases, DDB.getStr("DomainName"));
	                    QApplication::restoreOverrideCursor();
                    }
	                break;

	            default:        // Anything else.
	                break;
	        }
	    }
    }
}


/*
** emailSelected - Gets called when the user clicks on the Email button.
**                 This toggle the Postfix domains table and determines
**                 whether or not we will accept email for this domain
**                 on the primary mail servers.
*/

void Tab_Domains::emailSelected()
{
    // If we don't have a record selected return before doing anything.
    if (list->currentItem() == NULL) return;

    // Get the domain name
    QListViewItem *curItem = list->currentItem();

    // Now, find out if we are doing email for this domain.
    ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
    char    tmpStr[4096];

    mailDB.query("select VirtualID from Virtual where Mailbox = '%s'", (const char *)curItem->key(2,0));
    if (mailDB.rowCount) {
        // We are doing email for this domain.  Ask them if they want
        // to stop.
        sprintf(tmpStr, "Are you sure you want the system to stop\naccepting email for %s?", (const char *)curItem->key(2,0));
        switch(QMessageBox::information(this, "Deactivate Domain Email", tmpStr, "&Yes", "&No", 0)) {
            case 0:         // yes selected
                break;

            default:
                // No selected, return.
                return;
        }

        // If we made it here, we need to get rid of all of the aliases.
        mailDB.dbcmd("delete from Virtual where Mailbox = '%s'", (const char *)curItem->key(2,0));
        mailDB.dbcmd("delete from Virtual where Address LIKE '%%%%@%s'", (const char *)curItem->key(2,0));
        // Update our list entry.
        curItem->setText(5, "No");

    } else {
        // We are not doing email for this domain.  Ask them if they want
        // to set it up.
        sprintf(tmpStr, "Are you sure you want the system to\naccept email for %s?", (const char *)curItem->key(2,0));
        switch(QMessageBox::information(this, "Activate Domain Email", tmpStr, "&Yes", "&No", 0)) {
            case 0:         // yes selected
                break;

            default:
                // No selected, return.
                return;
        }
        ADBTable    mailTable("Virtual", cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));

        mailTable.clearData();
        mailTable.setValue("CustomerID",   myCustID);
        mailTable.setValue("AliasDate",    timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
        mailTable.setValue("AddedBy",      curUser().userName);
        mailTable.setValue("Address",      (const char *)curItem->key(2,0));
        mailTable.setValue("Mailbox",      (const char *)curItem->key(2,0));
        mailTable.ins();
        
        if (!QMessageBox::information(this, "Activate Domain Email", "Do you wish to create the\naliases for this domain as well?", "&Yes", "&No", 0, 1)) {
            // Now, find out if they want to do the "standard" aliases as well.
            int    doStockAliases = 1;
            if (QMessageBox::warning(this, "Activate Domain Email", "Do you wish to create the standard\ninfo, sales, support, webmaster aliases?", "&Yes", "&No", 0, 1)) {
                doStockAliases = 0;
            }
            QApplication::setOverrideCursor(waitCursor);
            createDomainAliases(myCustID, doStockAliases, (const char*)curItem->key(2,0));
            QApplication::restoreOverrideCursor();
        }

        // Update our list entry.
        curItem->setText(5, "Yes");
    }


}

/*
** aliasSelected - Loads up the DomainAlias list/editor.
*/

void Tab_Domains::aliasSelected()
{
    DomainAliases   *da = new DomainAliases();
    da->setCustomerID(myCustID);
    da->show();
}

/*
** dnsSelected - Loads up the DNS Manager for this domain.
*/

void Tab_Domains::dnsSelected()
{
    // If we don't have a record selected return before doing anything.
    if (list->currentItem() == NULL) return;

    // Get the domain name
    QListViewItem *curItem = list->currentItem();

    // Check to see if the domain exists in the DNS database
    char domainName[1024];
    strcpy(domainName, curItem->key(2,0));
    if (!hasDNSRecord(domainName)) {
        // No existing domain record.  Ask the user if they want to 
        // create one from a template.

        if (!QMessageBox::warning(this, "Domain Not Managed", "The domain was not found in the DNS database.\n\nWould you like to create a new entry for\nthis domain based on a template?", "&Yes", "&No", 0, 1) == 1) {
            createDNSFromTemplate();
            //QMessageBox::critical(this, "Create Domain From Template", "This has not yet been implemented.");
        } else {
            if (!QMessageBox::warning(this, "Domain Not Managed", "The domain was not found in the DNS database.\n\nWould you like to create a new blank entry for\nthis domain?", "&Yes", "&No", 0, 1) == 1) {
                createDNSRecord((const char *)curItem->key(2,0), myCustID, (const char *)curItem->key(0,0));
                // QMessageBox::critical(this, "Create Blank Domain", "This has not yet been implemented.");
            }
        }
    }
    // Check again for a domain entry.  If we don't have one, the user
    // elected to not create one.  We simply return.
    if (!hasDNSRecord(domainName)) return;
	curItem->setText(4, "Yes");        // Update the DNS column

	// emit(customerUpdated(myCustID));

    // Load up the DNS manager
    DNSManager   *dnsm = new DNSManager();
    dnsm->setDomainName(curItem->key(2,0));
    dnsm->show();
}

/*
** createDNSFromTemplate - Opens the DNSTemplatePicker and gets a template
**                         to create the DNS entry from.
*/

void Tab_Domains::createDNSFromTemplate()
{
    // If we don't have a record selected return before doing anything.
    if (list->currentItem() == NULL) return;

    // Get the domain name
    QListViewItem *curItem = list->currentItem();

    DNSTemplatePicker   *dtp;
    dtp = new DNSTemplatePicker(this);
    if (dtp->exec() == QDialog::Accepted) {
        setupDNSFromTemplate(dtp->getTemplateID(), (const char *)curItem->key(2,0), myCustID, (const char *)curItem->key(0,0));
    }
    delete dtp;
}

/*
** vhostSelected - Toggles Virtual Hosting settings.
*/

void Tab_Domains::vhostSelected()
{
    // If we don't have a record selected return before doing anything.
    if (list->currentItem() == NULL) return;

    // Get the domain name
    QListViewItem *curItem = list->currentItem();

    // Check to see if the domain exists in the DNS database
    char domainName[1024];
    strcpy(domainName, curItem->key(2,0));
    if (!hasVHostRecord(domainName)) {
        // No existing VHost record.  Ask the user if they want to create one
        if (!QMessageBox::warning(this, "Virtual Web Hosting", "The domain does not have a hosting entry.\n\nWould you like to create one?", "&Yes", "&No", 0, 1) == 1) {
            // Yes was selected.
            createVHostRecord(domainName, myCustID, (const char *)curItem->key(0,0));
        }
    } else {
        // They want to turn off Virtual Hosting
        if (!QMessageBox::warning(this, "Virtual Web Hosting", "Are you sure you want to disable Virtual Hosting for this domain?", "&Yes", "&No", 0, 1) == 1) {
            // Yes was selected.
            removeVHostRecord(domainName);
        }
    }

    // Check again for a VHost entry to update the list.
    if (!hasVHostRecord(domainName)) curItem->setText(6, "No");
    else curItem->setText(6, "Yes");        // Update the DNS column

    //emit(customerUpdated(myCustID));
}

/*
** refreshCustomer - Gets called automatically when another widget calls
**                   customer changed.
*/

void Tab_Domains::refreshCustomer(long custID)
{
    if (custID == myCustID) {
        if (list->currentItem() != NULL) {
            refreshDomainList(atoi((const char *)list->currentItem()->key(8,0)));
        }
    }
}
