/*
** $Id$
**
***************************************************************************
**
** DomainEdit - The main edit window for changing a domain.
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
** $Log: DomainEdit.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "DomainEdit.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "BlargDB.h"
#include "BString.h"
#include "ParseFile.h"
#include <Cfg.h>
#include <ADB.h>
#include <qmessagebox.h>

#define Inherited DomainEditData

DomainEdit::DomainEdit
(
	QWidget* parent,
	const char* name,
	long CustomerID
)
	:
	Inherited( parent, name )
{
	setCaption( "Edit Domain" );
	
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
	
	// Fill the login list for the logins on this customer.
	DB.query("select LoginID from Logins where CustomerID = %ld and Active = 1 order by LoginID", myCustID);
	while (DB.getrow()) {
	    loginID->insertItem(DB.curRow["LoginID"]);
	}
}


DomainEdit::~DomainEdit()
{
    if (domainTypeIDX != NULL) delete(domainTypeIDX);
}


/*
** saveDomain   - The slot that gets called when the user clicks the save
**                button.
*/

void DomainEdit::saveDomain()
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
    emit(domainSaved(1));
    close();
}

/*
** cancelDomain - The slot that gets called when the user clicks the cancel
**                button.
*/

void DomainEdit::cancelDomain()
{
    close();
}

