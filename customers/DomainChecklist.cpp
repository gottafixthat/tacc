/*
** $Id$
**
***************************************************************************
**
** DomainChecklist - Gives the user a checklist of domain tasks.
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
** $Log: DomainChecklist.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include <stdio.h>
#include <stdlib.h>
#include "DomainChecklist.h"
#include "BlargDB.h"
#include <ADB.h>

#define Inherited DomainChecklistData

DomainChecklist::DomainChecklist
(
	QWidget* parent,
	const char* name,
	long DomainID
)
	:
	Inherited( parent, name )
{
	setCaption( "Domain Checklist" );
	
	myDomainID = DomainID;
	
	if (!DomainID) return;
	
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

