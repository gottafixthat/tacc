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

#include <QtCore/QRegExp>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3GridLayout>

#include <ADB.h>
#include <BlargDB.h>
#include <TAATools.h>
#include <TAAWidget.h>
#include <Cfg.h>
#include <StrTools.h>

#include "DomainAliases.h"

using namespace Qt;

DomainAliases::DomainAliases(QWidget *parent, const char *name)
   : TAAWidget(parent, name)
{
    setCaption("Email Alias List");
    // Create the widgets we'll need
    aliasList = new Q3ListView(this, "Alias List");
    aliasList->addColumn("Address");
    aliasList->addColumn("Domain");
    aliasList->addColumn("Mailbox");
    aliasList->addColumn("Time Stamp");
    aliasList->addColumn("Added By");
    aliasList->setAllColumnsShowFocus(true);
    virtIDCol = 5;

    address = new QLineEdit(this, "Address Input");
    address->setMaxLength(63);

    QLabel  *atLabel = new QLabel(this, "At label");
    atLabel->setText("@");

    QLabel  *mapsToLabel = new QLabel(this, "Maps To label");
    mapsToLabel->setText("maps to");

    domainList = new QComboBox(this, false);
    loginList = new QComboBox(this, false);


    addButton = new QPushButton(this, "Add Button");
    addButton->setText("&Add");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addAlias()));

    refreshButton = new QPushButton(this, "Refresh Button");
    refreshButton->setText("&Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshAlias()));

    autoButton = new QPushButton(this, "Auto Button");
    autoButton->setText("A&uto");
    connect(autoButton, SIGNAL(clicked()), this, SLOT(autoAlias()));

    delButton = new QPushButton(this, "Delete Button");
    delButton->setText("&Delete");
    connect(delButton, SIGNAL(clicked()), this, SLOT(delAlias()));
    
    closeButton = new QPushButton(this, "Close Button");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    HorizLine *hline1 = new HorizLine(this);

    // Our layout now.  Very simple top down box.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(aliasList, 1);

    // Add in our "add" line.
    Q3BoxLayout *addl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    addl->addWidget(address, 1);
    addl->addWidget(atLabel, 0);
    addl->addWidget(domainList, 0);
    addl->addWidget(mapsToLabel, 0);
    addl->addWidget(loginList, 0);
    addl->addWidget(addButton, 0);

    ml->addLayout(addl, 0);
    ml->addWidget(hline1, 0);

    Q3GridLayout *bl = new Q3GridLayout(1, 4);
    int curCol = 0;
    bl->addWidget(refreshButton,    0, curCol++);
    bl->addWidget(autoButton,       0, curCol++);
    bl->addWidget(delButton,        0, curCol++);
    bl->addWidget(closeButton,      0, curCol++);
    for (int i = 0; i < curCol; i++) bl->setColStretch(i, 1);
    bl->setRowStretch(0, 0);

    // Now, the "button bar layout" to keep things spaced nicely.
    Q3BoxLayout  *bbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 0);
    bbl->addStretch(1);
    bbl->addLayout(bl, 0);
    ml->addLayout(bbl);
} 

DomainAliases::~DomainAliases()
{
}

/*
** setCustomerID - Sets the customer ID and loads the list of aliases
**                 from the database for them.
*/

void DomainAliases::setCustomerID(long custID)
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    myCustID = custID;
    char    AddrPart[1024];
    char    DomainPart[1024];
    __gnu_cxx::StrSplitMap parts;
    ADB     domainDB;
    ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
    aliasList->clear();
    if (mailDB.Connected()) {
        if (!myCustID) mailDB.query("select * from Virtual");
        else {
            char    sqlstr[65536];
            sprintf(sqlstr, "select * from Virtual where CustomerID = %ld", myCustID);
            // Now, walk through the domains for this customer and get any
            // mailboxes from other customers that might be mapped.
            domainDB.query("select Domains.DomainName from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", myCustID);
            if (domainDB.rowCount) while (domainDB.getrow()) {
                strcat(sqlstr, " or Address LIKE '%%@");
                strcat(sqlstr, domainDB.curRow["DomainName"]);
                strcat(sqlstr, "'");
            }

            mailDB.query(sqlstr);
        }
        if (mailDB.rowCount) while (mailDB.getrow()) {
            char mbstr[1024];
            StrSplit(mailDB.curRow["Address"], "@", parts);
            strcpy(AddrPart, parts[0].c_str());
            strcpy(DomainPart, parts[1].c_str());
            if (!strlen(AddrPart)) strcpy(AddrPart, "[Any Address]");
            // Check to see if the mailbox is a different customer, if so
            // note that in the list
            strcpy(mbstr, mailDB.curRow["Mailbox"]);
            if (atol(mailDB.curRow["CustomerID"]) != custID) {
                strcat(mbstr, " (");
                strcat(mbstr, mailDB.curRow["CustomerID"]);
                strcat(mbstr, ")");
            }
            // Add it to the list now.
            // Make sure it isn't the "special" mapped so Postfix knows
            // how to handle the domain and not give "mail loops back to
            // myself" errors.
            if (strcmp(mailDB.curRow["Mailbox"], mailDB.curRow["Address"])) {
                (void) new Q3ListViewItem(aliasList,
                                         AddrPart,
                                         DomainPart,
                                         mbstr,
                                         mailDB.curRow["AliasDate"],
                                         mailDB.curRow["AddedBy"],
                                         mailDB.curRow["VirtualID"]
                                        );
            }
        }
    }

    // Now that the list is populated, populate the list of domains
    // for this customer.
    domainList->clear();
    if (!myCustID) domainDB.query("select Domains.DomainName from Domains, DomainTypes where Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName");
    else domainDB.query("select Domains.DomainName from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", myCustID);
    if (domainDB.rowCount) while (domainDB.getrow()) {
        domainList->insertItem(domainDB.curRow["DomainName"]);
    }

    // Now populate the list of logins.
    loginList->clear();
    if (!myCustID) domainDB.query("select * from Logins where Active > 0 order by LoginID");
    else domainDB.query("select * from Logins where CustomerID = %ld and Active > 0 order by LoginID", myCustID);
    if (domainDB.rowCount) while (domainDB.getrow()) {
        loginList->insertItem(domainDB.curRow["LoginID"]);
    }

    // If we don't have a customer ID, disable the Auto button
    if (!myCustID) autoButton->setEnabled(false);
    else autoButton->setEnabled(true);

    QApplication::restoreOverrideCursor();
}

/*
** addAlias - Adds the alias the user has entered into the database.
**            Only the characters [a-z,A-Z,0-9,-,_,.] are allowed.
*/

void DomainAliases::addAlias()
{
    // First, make sure that there are no invalid characters in the
    // address.
    QRegExp rexp("[^a-z0-9_.-]", false);
    if (rexp.indexIn(address->text()) >= 0) {
        // Found a bad character.
        QMessageBox::warning(this, "Domain Alias", "Invalid character in address.\nOnly the characters a-z, A-Z, 0-9, '-', '_' and '.' are allowed.");
        return;
    }

    // The address must start with an alpha-numeric character
    rexp.setPattern("^[a-z0-9]+[^a-z0-9_.-]*");
    if (address->text().length() && rexp.indexIn(address->text())) {
        // Found a bad character.
        QMessageBox::warning(this, "Domain Alias", "The first character of an\nalias must be alphanumeric.");
        return;
    }


    // Now, make sure the address isn't blank.
    if (address->text().length() == 0) {
        // The address is empty.  Ask the user if they want to do an
        // all-mail/default setup.
        char    tmpStr[1024];
        sprintf(tmpStr, "An address was not specified.\n\nDid you want to set a default entry so any non-specific\nmatching address for %s\nwill be delivered to %s?", (const char *) domainList->currentText(), (const char *) loginList->currentText());
        if (QMessageBox::warning(this, "Domain Alias", tmpStr, "&Yes", "&No", 0, 1)) return;
        // Make sure there is not already a default entry...
    }

    char    fullAddr[1024];
    char    tmpAddr[1024];
    if (address->text().length()) sprintf(fullAddr, "%s@%s", (const char *)address->text(), (const char *)domainList->currentText());
    else sprintf(fullAddr, "@%s", (const char *)domainList->currentText());

    // Make sure this alias doesn't already exist.
    QApplication::setOverrideCursor(Qt::waitCursor);
    ADBTable     mailDB("Virtual", cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
    mailDB.query("select * from Virtual where Address = '%s'", fullAddr);
    QApplication::restoreOverrideCursor();
    if (mailDB.rowCount) {
        // That alias already exists.
        QMessageBox::warning(this, "Domain Alias", "That address is already mapped.");
        return;
    }

    // If we made it here, we're good to go.
    char    timedate[128];
    long    tmpCustID = myCustID;
    // Check to see if we need to lookup the customer ID.  This will
    // be keyed on the login.
    if (!tmpCustID) {
        ADB     tmpDB;
        tmpDB.query("select CustomerID from Logins where LoginID = '%s'", (const char *) loginList->currentText());
        tmpDB.getrow();
        tmpCustID = atol(tmpDB.curRow["CustomerID"]);
    }
    QDateTime   cd = QDateTime::currentDateTime();
    sprintf(timedate, "%04d-%02d-%02d %02d:%02d:%02d",
            cd.date().year(),
            cd.date().month(),
            cd.date().day(),
            cd.time().hour(),
            cd.time().minute(),
            cd.time().second()
           );
    mailDB.clearData();
    mailDB.setValue("CustomerID",   tmpCustID);
    mailDB.setValue("AliasDate",    timedate);
    mailDB.setValue("AddedBy",      curUser().userName);
    mailDB.setValue("Address",      fullAddr);
    mailDB.setValue("Mailbox",      (const char *) loginList->currentText());
    QApplication::setOverrideCursor(Qt::waitCursor);
    mailDB.ins();

    if (address->text().length()) strcpy(tmpAddr, (const char *) address->text());
    else strcpy(tmpAddr, "[Any Address]");
    (void) new Q3ListViewItem(aliasList,
                             tmpAddr,
                             domainList->currentText(),
                             loginList->currentText(),
                             timedate,
                             curUser().userName,
                             mailDB.getStr("VirtualID")
                            );
    address->clear();
    address->setFocus();

    // We'll sneak this in here, too.  We need to have an entry in there for
    // the domain by itself so that Postfix knows that the domain is okay to
    // accept email for.  We need to get all of the domains that we take email
    // for and add them into the system.
    ADB DB;
    char    addedBy[1024];
    sprintf(addedBy, "%s (Auto)", curUser().userName);
    DB.query("select Domains.DomainName from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", myCustID);
    if (DB.rowCount) while (DB.getrow()) {
        mailDB.query("select * from Virtual where Address = '%s' and Mailbox = '%s'", DB.curRow["DomainName"], DB.curRow["DomainName"]);
        if (!mailDB.rowCount) {
            // Wasn't found.  Add it.
            mailDB.clearData();
            mailDB.setValue("CustomerID",   myCustID);
            mailDB.setValue("AliasDate",    timedate);
            mailDB.setValue("AddedBy",      addedBy);
            mailDB.setValue("Address",      DB.curRow["DomainName"]);
            mailDB.setValue("Mailbox",      DB.curRow["DomainName"]);
            mailDB.ins();
        }
    }
    
    QApplication::restoreOverrideCursor();
}

/*
** delAlias - Allows the user to delete an alias.
*/

void DomainAliases::delAlias()
{
    Q3ListViewItem   *curItem = aliasList->currentItem();
    if (!curItem) return;

    // Prompt the user and make sure they want to delete the selected
    // alias.
    if (QMessageBox::warning(this, "Domain Aliases", "Are you sure you want to\ndelete the selected alias?", "&Yes", "&No", 0, 1) == 0) {
        QApplication::setOverrideCursor(Qt::waitCursor);
        ADB     mailDB(cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
        mailDB.dbcmd("delete from Virtual where VirtualID = %s", (const char *) curItem->key(virtIDCol, 0));
        QApplication::restoreOverrideCursor();
        delete curItem; // aliasList->takeItem(curItem);
    }

}

/*
** autoAlias - Verifies that the user wants to setup the auto-aliases for the
**             customer and whether or not they want to do the webmaster,
**             sales, support, info aliases also.
*/

void DomainAliases::autoAlias()
{
    // Check to make sure there is a customer ID.  If not, we do not want to
    // do auto domains.
    if (!myCustID) {
        // Found a bad character.
        QMessageBox::warning(this, "Domain Alias", "Auto setup can't be done unless a\nspecific customer/domain is selected.");
        return;
    }

    // First, make sure they really want to do this.
    if (QMessageBox::warning(this, "Domain Aliases", "Are you sure you want to auto-create aliases?", "&Yes", "&No", 0, 1)) {
        return;
    }

    int    doStockAliases = 1;
    // Now, find out if they want to do the "standard" aliases as well.
    if (QMessageBox::warning(this, "Domain Aliases", "Do you wish to create the standard\ninfo, sales, support, webmaster aliases?", "&Yes", "&No", 0, 1)) {
        doStockAliases = 0;
    }

    QApplication::setOverrideCursor(Qt::waitCursor);
    emit(setStatus("Creating aliases..."));

    // Call the library function to do the creation.
    createDomainAliases(myCustID, doStockAliases);
    
    // Now, clear the aliasList and reload them.
    setCustomerID(myCustID);
    
    emit(setStatus(""));
    QApplication::restoreOverrideCursor();

}

/*
** refreshAlias - Simply calls setCustomerID to refresh our list.
*/

void DomainAliases::refreshAlias()
{
    setCustomerID(myCustID);
}

// vim: expandtab
