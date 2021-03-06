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
#include <stdio.h>

#include <QtCore/QRegExp>
#include <QtGui/QApplication>
#include <QtGui/QBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

#include "Customers.h"
#include "NewCustomer.h"
#include <BlargDB.h>
#include "EditCustomer.h"
#include "ReceivePayment.h"
#include "NoteEdit.h"
#include "LogCall.h"
#include <Cfg.h>
#include <DomainAliases.h>

#include <TAATools.h>

using namespace Qt;

Customers::Customers ( QWidget* parent, const char* name) 
  : TAAWidget (parent)
{
	setCaption( "Customer List" );

    QPushButton *newButton = new QPushButton("&New", this);
    connect(newButton, SIGNAL(clicked()), this, SLOT(newCustomer()));

    QPushButton *editButton = new QPushButton("&Edit", this);
    editButton->setText("&Edit");
    connect(editButton, SIGNAL(clicked()), this, SLOT(editCustomer()));

    QPushButton *addNoteButton = new QPushButton("&Add Note", this);
    connect(addNoteButton, SIGNAL(clicked()), this, SLOT(addCustNote()));

    QPushButton *takeCallButton = new QPushButton("&Take Call", this);
    connect(takeCallButton, SIGNAL(clicked()), this, SLOT(takeCall()));

    queryLabel = new QLabel(this, "QueryLabel");
    queryLabel->setText( "&Search:" );
    queryLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    queryLabel->setMargin( -1 );

    listQuery = new QLineEdit(this, "QueryEntry");
    listQuery->setText( "" );
    listQuery->setMaxLength( 32767 );
    listQuery->setFrame( QLineEdit::Normal );
    listQuery->setFrame( TRUE );
    listQuery->setAlignment( AlignLeft );
    connect(listQuery, SIGNAL(returnPressed()), SLOT(listQueryS()));

    QPushButton *searchButton = new QPushButton(this, "Search");
    searchButton->setText("Search");
    connect(searchButton, SIGNAL(clicked()), this, SLOT(listQueryS()));

    QPushButton *clearButton = new QPushButton("C&lear", this);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearQuery()));

    autoOpenCustomer = new QCheckBox(this, "AutoOpenCheckbox");
    autoOpenCustomer->setText( "&Auto open if only one match." );

    // Phone Number Information
    phoneNumberLabel   = new QLabel(this, "phoneNumberLabel");
    phoneNumberLabel->setAlignment(AlignRight|AlignVCenter);
    phoneNumberLabel->setText("TN:");

    phoneNumber = new QLabel(this, "phoneNumber");
    phoneNumber->setAlignment(AlignLeft|AlignVCenter);

    telcoNameLabel   = new QLabel(this, "telcoNameLabel");
    telcoNameLabel->setAlignment(AlignRight|AlignVCenter);
    telcoNameLabel->setText("Telco:");

    telcoName = new QLabel(this, "telcoName");
    telcoName->setAlignment(AlignLeft|AlignVCenter);

    LATALabel   = new QLabel(this, "LATALabel");
    LATALabel->setAlignment(AlignRight|AlignVCenter);
    LATALabel->setText("LATA:");

    LATA = new QLabel(this, "LATA");
    LATA->setAlignment(AlignLeft|AlignVCenter);

    serviceAreaLabel   = new QLabel(this, "serviceAreaLabel");
    serviceAreaLabel->setAlignment(AlignRight|AlignVCenter);
    serviceAreaLabel->setText("Svc:");

    serviceArea = new QLabel(this, "serviceArea");
    serviceArea->setAlignment(AlignLeft|AlignVCenter);

    dslQualLabel   = new QLabel(this, "dslQualLabel");
    dslQualLabel->setAlignment(AlignRight|AlignVCenter);
    dslQualLabel->setText("DSL:");

    dslQual = new QLabel(this, "dslQual");
    dslQual->setAlignment(AlignLeft|AlignVCenter);

    iHostLabel   = new QLabel(this, "iHostLabel");
    iHostLabel->setAlignment(AlignRight|AlignVCenter);
    iHostLabel->setText("Ihost:");

    iHost = new QLabel(this, "iHost");
    iHost->setAlignment(AlignLeft|AlignVCenter);

    locationLabel   = new QLabel(this, "locationLabel");
    locationLabel->setAlignment(AlignRight|AlignVCenter);
    locationLabel->setText("CO:");

    location = new QLabel(this, "location");
    location->setAlignment(AlignLeft|AlignVCenter);

    // Put the telephone number info into a layout
	QBoxLayout *phoneNumberLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    phoneNumberLayout->addWidget(phoneNumberLabel, 0);
    phoneNumberLayout->addWidget(phoneNumber,      1);
    phoneNumberLayout->addWidget(telcoNameLabel,   1);
    phoneNumberLayout->addWidget(telcoName,        1);
    phoneNumberLayout->addWidget(LATALabel,        1);
    phoneNumberLayout->addWidget(LATA,             1);
    phoneNumberLayout->addWidget(serviceAreaLabel, 1);
    phoneNumberLayout->addWidget(serviceArea,      1);
    phoneNumberLayout->addWidget(iHostLabel,       1);
    phoneNumberLayout->addWidget(iHost,            1);
    phoneNumberLayout->addWidget(dslQualLabel,     1);
    phoneNumberLayout->addWidget(dslQual,          0);

	QBoxLayout *locationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    locationLayout->addWidget(locationLabel, 0);
    locationLayout->addWidget(location,      1);

    list = new QTreeWidget(this);
    list->setAllColumnsShowFocus(true);
    list->setColumnCount(6);
    QStringList headers;
    headers += "Customer Name";
    headers += "CID";
    headers += "Username";
    headers += "Balance";
    headers += "A";
    headers += "Type";
    list->setHeaderLabels(headers);
    list->headerItem()->setTextAlignment(3, AlignRight);
    list->headerItem()->setTextAlignment(4, AlignCenter);
    list->setRootIsDecorated(true);
    //list->setSortingEnabled(true);
    //list->header()->setClickable(true);
    //list->header()->setSortIndicatorShown(true);
    
	connect(list, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(custSelected(QTreeWidgetItem *, int)));
	//connect(list, SIGNAL(returnPressed(Q3ListViewItem *)), SLOT(custSelected(Q3ListViewItem *)));

	QBoxLayout *ml = new QBoxLayout(QBoxLayout::TopToBottom);
    ml->setMargin(2);
    ml->setSpacing(2);

    QHBoxLayout *bl = new QHBoxLayout();
    bl->addWidget(newButton);
    bl->addWidget(editButton);
    bl->addWidget(addNoteButton);
    bl->addWidget(takeCallButton);
    ml->addLayout(bl);

    QBoxLayout *ql = new QBoxLayout(QBoxLayout::LeftToRight);
    ql->addWidget(queryLabel);
    ql->addWidget(listQuery, 1);
    ql->addWidget(searchButton);
    ql->addWidget(clearButton);
    ml->addLayout(ql);

    ml->addWidget(list, 1);
    ml->addLayout(phoneNumberLayout);
    ml->addLayout(locationLayout);

    setLayout(ml);
    /*
    resize(500,345);
    setMinimumSize(300, 300);
    setMaximumSize(32767, 32767);
    */

    // Setup our actions.
    actNew = new QAction(tr("&New"), this);
    actNew->setShortcut(tr("Ctrl+N"));
    connect(actNew, SIGNAL(triggered()), this, SLOT(newCustomer()));

    actEdit = new QAction(tr("&Edit"), this);
    actEdit->setShortcut(tr("Ctrl+E"));
    connect(actEdit, SIGNAL(triggered()), this, SLOT(editCustomer()));

    actAddNote = new QAction(tr("&Add Note"), this);
    actAddNote->setShortcut(tr("Ctrl+A"));
    connect(actAddNote, SIGNAL(triggered()), this, SLOT(addCustNote()));

    actReceivePayment = new QAction(tr("&Receive Payment"), this);
    actReceivePayment->setShortcut(tr("Ctrl+R"));
    connect(actReceivePayment, SIGNAL(triggered()), this, SLOT(receivePayment()));

    actDomainAliases = new QAction(tr("D&omain Aliases (Global)"), this);
    connect(actDomainAliases, SIGNAL(triggered()), this, SLOT(globalAliases()));

    // Set up the menu...
    optionsMenu = new QMenu("&Options");
    optionsMenu->addAction(actNew);
    optionsMenu->addAction(actEdit);
    optionsMenu->insertSeparator();
    optionsMenu->addAction(actAddNote);
    optionsMenu->addAction(actReceivePayment);
    if (isManager()) {
        optionsMenu->insertSeparator();
        optionsMenu->addAction(actDomainAliases);
    }

    
    listQuery->setFocus();
    
    queryLabel->setBuddy(listQuery);

    
    autoOpenCustomer->hide();
    // We only want to show the phone number data if the user enters a phone number
    phoneNumber->hide();
    phoneNumberLabel->hide();
    telcoName->hide();
    telcoNameLabel->hide();
    LATA->hide();
    LATALabel->hide();
    serviceArea->hide();
    serviceAreaLabel->hide();
    dslQual->hide();
    dslQualLabel->hide();
    iHost->hide();
    iHostLabel->hide();
    location->hide();
    locationLabel->hide();
}


Customers::~Customers()
{
}


/*
** refreshList - Searches the database for the contents of 'query'.
**
**               Several different queries are performed to try and find
**               matches.  The first query that gets a match wins.
**                 1.  Search for an exact numeric match on CustomerID
**                 2.  Exact match on a LoginID
**                 3.  Matches in Customers.FullName or Customers.ContactName
**                 4.  Matches in Logins.LoginID or Logins.ContactName
**                 5.  No matches found...
*/

void Customers::refreshList(long)
{
    ADB         DB;
    ADB         DB2;
    ADBTable    custDB("Customers");
    ADB         ltypeDB;
    char        tmpstr[4096]; 
    char        ltypeStr[4096];
    int         rowcnt = 0;
    int         curRow = 0;
    long        curCustID = 0;
    bool        allLoaded = false;
    bool        matchFound = false;
    QString     tmpsub;
    tmpsub = "";
    char    *isActive  = new char[64];
    
    // Change the cursor to an hourglass
    QApplication::setOverrideCursor(waitCursor);
    emit setStatus("Searching...");
    
    // Save the state of the list.
    QTreeWidgetItem *tmpCur;
    tmpCur  = list->currentItem();
    
    list->clear();

    // Clear our map of loaded customers as well.
    loadedCusts.clear();

    tmpsub = listQuery->text().replace(QRegExp("[^a-zA-Z0-9\\,.\\-_ /]"), "").simplifyWhiteSpace();
    listQuery->setText(tmpsub);

    emit setStatus("");
    QApplication::restoreOverrideCursor();
    // If there is no query at all, simply return since the
    // list is now empty.
    if (!tmpsub.length()) return;
    QApplication::setOverrideCursor(waitCursor);
    emit setStatus("Searching...");

    phoneNumber->setText("");
    telcoName->setText("");
    LATA->setText("");
    serviceArea->setText("");
    dslQual->setText("");
    iHost->setText("");
    location->setText("");

    phoneNumber->hide();
    phoneNumberLabel->hide();
    telcoName->hide();
    telcoNameLabel->hide();
    LATA->hide();
    LATALabel->hide();
    serviceArea->hide();
    serviceAreaLabel->hide();
    dslQual->hide();
    dslQualLabel->hide();
    iHost->hide();
    iHostLabel->hide();
    location->hide();
    locationLabel->hide();
    
    // First query.  CustomerID
    if (tmpsub.toLong()) {
        // They specified a login ID.  Check for it
        DB.query("select CustomerID from Customers where CustomerID = %ld", tmpsub.toLong());
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    } 
    
    // Check to see if the user specified a search string.
    // If they didn't, load the whole list.
    if (!rowcnt) {
        if (!strlen((const char *)tmpsub)) {
            DB.query("select CustomerID from Customers");
            rowcnt = DB.rowCount;
            if (rowcnt) {
                while (DB.getrow()) {
                    curCustID = atol(DB.curRow["CustomerID"]);
                    // Load them into our loadedCusts map
                    if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
                }
            }
            allLoaded = true;
        }
    }

    // Second query.  LoginID
    if (!allLoaded) {
        DB.query("select CustomerID from Logins where LoginID = '%s'", (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }
    
    // Third query.  Customers.FullName or Customers.ContactName
    if (!allLoaded) {
        DB.query("select distinct CustomerID from Customers where FullName LIKE '%%%%%s%%%%' or ContactName LIKE '%%%%%s%%%%'", (const char *) tmpsub, (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }

    // Fourth query.  Logins.LoginID or Logins.ContactName
    if (!allLoaded) {
        DB.query("select distinct CustomerID from Logins where LoginID LIKE '%%%%%s%%%%' or ContactName LIKE '%%%%%s%%%%'", (const char *) tmpsub, (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }

    // Fifth query.  Customers.RegNum - An ISOMEDIA-ism
    if (!allLoaded) {
        DB.query("select distinct CustomerID from Customers where RegNum LIKE '%%%s%%'", (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }

    // Sixth query.  LoginFlagValues Logins.LoginID or Logins.ContactName
    if (!allLoaded) {
        DB.query("select distinct Logins.CustomerID from Logins, LoginFlagValues where Logins.LoginID = LoginFlagValues.LoginID and LoginFlagValues.FlagValue LIKE '%%%s%%'", (const char *) tmpsub);
        rowcnt = DB.rowCount;
        if (rowcnt) {
            while (DB.getrow()) {
                curCustID = atol(DB.curRow["CustomerID"]);
                // Load them into our loadedCusts map
                if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
            }
        }
    }


    // Sixth query.  Phone number(s).  This one is harder because it is
    // free form.  We need to account for the following cases:
    // nnnDnnnDnnnn, nnnnnnnnnn, nnnDnnnn, and nnnnnnn where "D" is any
    // delimiter.  Not pretty.
    if (!allLoaded && tmpsub.length() <= 12) {
        QString tmpWorkStr;
        QString numbersOnly;
        QString NPA;
        QString NXX;
        QString fmtPhone;
        // Extract the numbers only out of the string.
        tmpWorkStr  = tmpsub;
        numbersOnly = tmpWorkStr.replace(QRegExp("[^0-9]"), "");
        //fprintf(stderr, "numbersOnly = '%s'\n", (const char *) numbersOnly);
        // Okay, now that we have only numbers in our string, if it is
        // either 7 or 10 digits long, check for a phone number.
        if (numbersOnly.length() == 7 || numbersOnly.length() == 10) {
            // If the string is 7 digits long, we want to search only
            tmpWorkStr = "";
            if (numbersOnly.length() == 7) {
                tmpWorkStr += "%%";
                tmpWorkStr += numbersOnly.mid(0,3);
                tmpWorkStr += "%%";
                tmpWorkStr += numbersOnly.mid(3,4);
                tmpWorkStr += "%%";
            } else {
                NPA = numbersOnly.mid(0,3);
                NXX = numbersOnly.mid(3,3);
                fmtPhone += numbersOnly.mid(0,3);
                fmtPhone += ".";
                fmtPhone += numbersOnly.mid(3,3);
                fmtPhone += ".";
                fmtPhone += numbersOnly.mid(6,4);

                tmpWorkStr += "%%";
                tmpWorkStr += numbersOnly.mid(0,3);
                tmpWorkStr += "%%";
                tmpWorkStr += numbersOnly.mid(3,3);
                tmpWorkStr += "%%";
                tmpWorkStr += numbersOnly.mid(6,4);
                tmpWorkStr += "%%";
            }
            DB.query("select distinct CustomerID from CustomerContacts where PhoneNumber LIKE '%s'", (const char *) tmpWorkStr);
            rowcnt = DB.rowCount;
            if (rowcnt) {
                while (DB.getrow()) {
                    curCustID = atol(DB.curRow["CustomerID"]);
                    // Load them into our loadedCusts map
                    if (loadedCusts[curCustID] != curCustID) loadedCusts.insert(curCustID, curCustID);
                }
            }
            // Check for an NPA/NXX Qwest Ihost lata lookup
            if (NPA.length() == 3 && NXX.length() == 3) {
                phoneNumberLabel->show();
                phoneNumber->show();
                phoneNumber->setText(fmtPhone);
                telcoNameLabel->show();
                telcoName->show();
                telcoName->setText("Unk");
                LATALabel->show();
                LATA->show();
                LATA->setText("Unk");
                serviceAreaLabel->show();
                serviceArea->show();
                serviceArea->setText("Unk");
                dslQualLabel->show();
                dslQual->show();
                dslQual->setText("Unk");
                iHostLabel->show();
                iHost->show();
                iHost->setText("Unk");

                DB.query("select NPA_NXX.LATA, NPA_NXX.Service, NPA_NXX.TelcoID, NPA_NXX.City, NPA_NXX.State, Telcos.TelcoName, Telcos.HasIhosts from NPA_NXX, Telcos where NPA_NXX.NPA = %d and NPA_NXX.NXX = %d and Telcos.TelcoID = NPA_NXX.TelcoID", NPA.toInt(), NXX.toInt());
                if (DB.rowCount) {
                    // Get the Telco Name and LATA
                    DB.getrow();
                    telcoName->setText(DB.curRow["TelcoName"]);
                    LATA->setText(DB.curRow["LATA"]);
                    if (DB.curRow.col("Service")->toInt()) {
                        serviceArea->setText("Yes");
                    } else {
                        serviceArea->setText("No");
                    }
                    dslQual->setText("Unk");
                    iHost->setText("N/A");

                    QString tmpLoc;
                    if (strlen(DB.curRow["City"])) {
                        tmpLoc += DB.curRow["City"];
                    }
                    if (strlen(DB.curRow["State"])) {
                        if (tmpLoc.length()) tmpLoc += ", ";
                        tmpLoc += DB.curRow["State"];
                    }
                    if (tmpLoc.length()) {
                        locationLabel->show();
                        location->show();
                        location->setText(tmpLoc);
                    }

                    // Check if we have Ihosts for this area.
                    if (DB.curRow.col("HasIhosts")->toInt()) {
                        QString tmpIHost;
                        DB2.query("select IhostName, IhostPass, VPI_Blarg from Qwest_Ihosts where LATA = %d and TelcoID = %d", DB.curRow.col("LATA")->toInt(), DB.curRow.col("TelcoID")->toInt());
                        if (DB2.rowCount) {
                            DB2.getrow();
                            tmpIHost += DB2.curRow["IhostName"];
                            if (atoi(DB2.curRow["VPI_Blarg"]) >= 0) {
                                tmpIHost += " VPI ";
                                tmpIHost += DB2.curRow["VPI_Blarg"];
                            }
                            iHost->setText(tmpIHost);
                        }
                    } 

                    // Check to see if this number qualifies for DSL
                    DB2.query("select Qualifies, QualDate from DSL_Quals where PhoneNumber = %s", (const char *) numbersOnly);
                    if (DB2.rowCount) {
                        DB2.getrow();
                        if (atoi(DB2.curRow["Qualifies"])) {
                            dslQual->setText("Yes");
                        } else {
                            dslQual->setText("No");
                        }
                    }
                }
            }
        }
    }
    
    if (loadedCusts.count()) {
        // One of our queries matched.  DB will have a query full of
        // Customer IDs.  We run several other queries on this one
        // to get the entire customer record for them.
        emit setStatus("Loading matching customers...");
        loadedCustMap::Iterator it;
        for (it = loadedCusts.begin(); it != loadedCusts.end(); ++it) {
            emit setProgress(++curRow, rowcnt);
            custDB.get(it.data());
            
            sprintf(tmpstr, "%s", custDB.getStr("FullName"));
            if (strlen(custDB.getStr("ContactName"))) {
                strcat(tmpstr, "/");
                strcat(tmpstr, custDB.getStr("ContactName"));
            }
            if (custDB.getInt("Active")) strcpy(isActive, "Yes");
            else strcpy(isActive, "No");

            // Get the login type for the parent record.
            strcpy(ltypeStr, "[Unknown]");
            ltypeDB.query("select LoginTypes.LoginType, Logins.LoginID from LoginTypes, Logins where Logins.LoginID = '%s' and LoginTypes.InternalID = Logins.LoginType", custDB.getStr("PrimaryLogin"));
            if (ltypeDB.rowCount) {
                ltypeDB.getrow();
                strcpy(ltypeStr, ltypeDB.curRow["LoginType"]);
            }


            // Create the parent record.
            tmpCur = new QTreeWidgetItem(list);
            tmpCur->setText(0, tmpstr);
            tmpCur->setText(1, custDB.getStr("CustomerID"));
            tmpCur->setText(2, custDB.getStr("PrimaryLogin"));
            tmpCur->setText(3, custDB.getStr("CurrentBalance"));
            tmpCur->setText(4, isActive);
            tmpCur->setText(5, ltypeStr);
            if (!strcmp(custDB.getStr("PrimaryLogin"), tmpsub)) {
                list->setCurrentItem(tmpCur);
                list->scrollToItem(tmpCur);
                matchFound = true;
            }
            tmpCur->setTextAlignment(1, Qt::AlignRight);
            tmpCur->setTextAlignment(3, Qt::AlignRight);
            tmpCur->setTextAlignment(4, Qt::AlignCenter);

            // Get any child records.
            DB2.query("select Logins.ContactName, Logins.LoginID, Logins.Active, LoginTypes.LoginType from Logins, LoginTypes where Logins.CustomerID = %ld and LoginTypes.InternalID = Logins.LoginType", custDB.getLong("CustomerID"));
            if (DB2.rowCount) while (DB2.getrow()) {
                // Make sure that we're not re-loading the primary login
                if (strcmp(custDB.getStr("PrimaryLogin"), DB2.curRow["LoginID"])) {
                    if (atoi(DB2.curRow["Active"])) strcpy(isActive, "Yes");
                    else strcpy(isActive, "No");
                    QTreeWidgetItem *tmpChild = new QTreeWidgetItem(tmpCur);
                    tmpChild->setText(0, DB2.curRow["ContactName"]);
                    tmpChild->setText(1, custDB.getStr("CustomerID"));
                    tmpChild->setText(2, DB2.curRow["LoginID"]);
                    tmpChild->setText(3, "");
                    tmpChild->setText(4, isActive);
                    tmpChild->setText(5, DB2.curRow["LoginType"]);
                    tmpChild->setTextAlignment(1, Qt::AlignRight);
                    tmpChild->setTextAlignment(3, Qt::AlignRight);
                    tmpChild->setTextAlignment(4, Qt::AlignCenter);
                    if (!strcmp(DB2.curRow["LoginID"], tmpsub)) {
                        tmpCur->setExpanded(true);
                        list->setCurrentItem(tmpChild);
                        list->scrollToItem(tmpChild);
                        matchFound = true;
                    }
                }
            }
        }

        // Make sure that our selected item is visible
        //if (!matchFound && list->topLevelItem(0)) list->setCurrentItem(list->topLevelItem(0));

        /*
        if (list->currentItem()) {
            if (list->currentItem()->itemBelow()) {
                list->scrollToItem(list->currentItem()->itemBelow());
            }
        }
        if (list->currentItem()) {
            if (list->currentItem()->itemAbove()) {
                list->scrollToItem(list->currentItem()->itemAbove());
            }
        }
        */
        for (int i = 0; i < list->columnCount(); i++) {
            list->resizeColumnToContents(i);
        }
        list->sortItems(0, Qt::AscendingOrder);
        list->scrollToItem(list->currentItem());

        emit setStatus("");
    }

    emit setStatus("");    // Searching...
    
    emit refreshTabs(1);
    
    QApplication::restoreOverrideCursor();

}




void Customers::refreshListV()
{
    refreshList(0);
}



void Customers::listQueryS()
{
    refreshList(0);
    list->setFocus();
    if ((list->topLevelItemCount() == 1) && autoOpenCustomer->isChecked()) {
    	editCustomer();
    }
}



/**
 ** newCustomer - The slot for adding a new customer.  Brings up the
 **               New Customer Dialog box and exits.
 **/ 

void Customers::newCustomer()
{
	NewCustomer *newCust;
	newCust = new NewCustomer();
	newCust->show();
	connect(newCust, SIGNAL(refreshCustList()), SLOT(refreshListV()));
}

/*
** editCustomer - The slot for editing a customer from the list.
*/ 

void Customers::editCustomer()
{
	long CustID = -1;
//	int tmpIDX = 0; //list->currentItem();
	QTreeWidgetItem *tmpItem;
	tmpItem = list->currentItem();

    emit setStatus("Loading customer information...");
	if (tmpItem != NULL) {
		//CustID = atol(tmpItem->key(1, 1));
		CustID = tmpItem->text(1).toInt();
        fprintf(stderr, "emitting openCustomer(%ld)...\n", CustID);
        emit openCustomer(CustID);
        /*
		EditCustomer *custEdit;
		custEdit = new EditCustomer(0, "", CustID);
		custEdit->show();
		connect(custEdit, SIGNAL(refreshCustList()), SLOT(refreshListV()));
        */
		listQuery->setFocus();
	}
    emit setStatus("");
}

/*
** addCustNote - Allows a quick way to add a note to a customer record.
*/ 

void Customers::addCustNote()
{
	int CustID = -1;
//	int tmpIDX = 0; //list->currentItem();
	QTreeWidgetItem  *tmpItem;
	tmpItem = list->currentItem();

    emit setStatus("Loading customer information...");
	if (tmpItem != NULL) {
		//CustID = atol(tmpItem->key(1, 1));
		CustID = tmpItem->text(1).toInt();
		NoteEdit *custNote;
		custNote = new NoteEdit(0, "", CustID, 0);
		custNote->show();
		// connect(custNote, SIGNAL(refreshCustList()), SLOT(refreshListV()));
		listQuery->setFocus();
	}
    emit setStatus("");
}


/**
 ** receivePayment - The slot for receiving a payment from a customer
 **/ 

void Customers::receivePayment()
{
	int CustID = -1;
	QTreeWidgetItem  *tmpItem;
	tmpItem = list->currentItem();
	
	if (tmpItem != NULL) {
		//CustID = atol(tmpItem->key(1,1));
		CustID = tmpItem->text(1).toInt();
		ReceivePayment *recPay;
		recPay = new ReceivePayment(0, "", CustID);
		recPay->show();
		listQuery->setFocus();
	}
}



/*
** custSelected  - A slot that will get called when the user double clicks
**                 on an item in the customer list.
*/

void Customers::custSelected(QTreeWidgetItem *sitem, int)
{
    emit setStatus("Loading customer information...");
    if (sitem != NULL) {
        //long tmpCustID = atol(sitem->key(1,1));
		long tmpCustID = sitem->text(1).toInt();
        emit openCustomer(tmpCustID);
        /*
        custEdit = new EditCustomer(0, "", atol(sitem->key(1,1)));
        EditCustomer    *custEdit;
        custEdit->show();
        connect(custEdit, SIGNAL(refreshCustList()), SLOT(refreshListV()));
        */
        listQuery->setFocus();
    }
    emit setStatus("");
}

/*
** clearQuery - Clears the query and sets the focus to the query box.
*/

void Customers::clearQuery()
{
    listQuery->setText("");
    listQuery->setFocus();
}


/*
** takeCall - The slot for taking a phone call.
*/ 

void Customers::takeCall()
{
	LogCall *call;
	call = new LogCall();
	call->show();
	
	QTreeWidgetItem  *tmpItem;
	tmpItem = list->currentItem();
	
	if (tmpItem != NULL) {
		//call->setLoginID((const char *) tmpItem->key(2,1));
		call->setLoginID(tmpItem->text(2).ascii());
	}
	
	// connect(newCust, SIGNAL(refreshCustList()), SLOT(refreshListV()));
}

/*
** refreshCustomer - Gets called automatically by other widgets and the
**                   mainWin.  It will contain the ID of a customer that 
**                   has changed.  We check to see if we have it loaded
**                   and if so, we refresh that entry.
*/

void Customers::refreshCustomer(long custID)
{
    QApplication::setOverrideCursor(waitCursor);
    // We already have all of the loaded customer ID's in a map,
    // so checking for this customer is trivial.
    if (loadedCusts[custID] == custID) {
        // We have it loaded.  Find it in the list
        ADBTable    custDB("Customers");
        custDB.get(custID);
        QTreeWidgetItem        *curItem;
        char                    tmpStr[1024];
        for (int i = 0; i < list->topLevelItemCount(); i++) {
            curItem = list->topLevelItem(i);
		    if (curItem->text(1).toInt() == custID) {
                sprintf(tmpStr, "%s", custDB.getStr("FullName"));
                if (strlen(custDB.getStr("ContactName"))) {
                    strcat(tmpStr, "/");
                    strcat(tmpStr, custDB.getStr("ContactName"));
                }
                curItem->setText(0, tmpStr);
                curItem->setText(3, custDB.getStr("CurrentBalance"));
                strcpy(tmpStr, "No");
                if (custDB.getInt("Active")) strcpy(tmpStr, "Yes");
                curItem->setText(4, tmpStr);
            }
        }
    }
    QApplication::restoreOverrideCursor();
}

/*
** globalAliases - Loads up the DomainAliases widget, but does not specify
**                 a customer ID.  This will cause it to work on the global
**                 level, giving the user the option to put in any combination
**                 of domain and username.
*/

void Customers::globalAliases()
{
    DomainAliases   *da = new DomainAliases();
    da->setCustomerID(0);
    da->show();
}

/** customerSearch - Clears the query, sets it to the selected value, and
  * searches for the customer.
  */
void Customers::customerSearch(const char *searchStr)
{
    fprintf(stderr, "customerSearch('%s')\n", searchStr);
    listQuery->setText(searchStr);
    refreshList(0);
}


// vim: expandtab
