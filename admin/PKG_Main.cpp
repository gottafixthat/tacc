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

#include <stdio.h>
#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QLayout>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3tabdialog.h>
#include <Qt3Support/q3widgetstack.h>

#include <ADB.h>

#include "PKG_Main.h"

using namespace Qt;

PKG_Main::PKG_Main
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Edit Packages" );
    
    // Create our widgets
    packageList = new Q3ListView(this, "packageList");
    packageList->addColumn("Package Name");
    packageList->addColumn("A");
    packageList->setAllColumnsShowFocus(true);
    connect(packageList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(packageSelected(Q3ListViewItem *)));

    theTabBar = new QTabBar(this);
    theTabBar->setMinimumSize(0, 25);
    theTabBar->setMaximumSize(4096, 25);
    theTabBar->addTab("&General");
    theTabBar->addTab("C&onversion");
    theTabBar->addTab("&Prices");
    theTabBar->addTab("&Billables");

    // Create a button group to hold the widget stack and other widgets
    Q3ButtonGroup    *bGroup = new Q3ButtonGroup(this, "bGroup");

    // Our widget stack
    qws = new Q3WidgetStack(bGroup, "WidgetStack");
    connect(theTabBar, SIGNAL(selected(int)), qws, SLOT(raiseWidget(int)));
    
    // Now add widgets to the stack
    tgeneral = new PKG_General(qws);
    qws->addWidget(tgeneral, 0);

    tconversion = new PKG_Conversion(qws);
    qws->addWidget(tconversion, 1);

    tpricing = new PKG_Pricing(qws);
    qws->addWidget(tpricing, 2);

    tbillables = new PKG_Billables(qws);
    qws->addWidget(tbillables, 3);

    // Our action buttons
    QPushButton *newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newPackage()));

    deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePackage()));
    
    QPushButton *closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    
    // Our layout.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3BoxLayout *wl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    wl->addWidget(packageList, 0);

    Q3BoxLayout *sl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
    sl->addWidget(theTabBar, 0);

    Q3BoxLayout *bgl = new Q3BoxLayout(bGroup, Q3BoxLayout::TopToBottom, 2);
    bgl->addWidget(qws, 1);
    sl->addWidget(bGroup, 1);

    wl->addLayout(sl, 1);
    ml->addLayout(wl, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addWidget(newButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);




    loadPackages();

    tgeneral->setPackageID((long) 0);
    tconversion->setPackageID((long) 0);
    tpricing->setPackageID((long) 0);
    tbillables->setPackageID((long) 0);
    connect(tgeneral, SIGNAL(packageChanged()), SLOT(updatePackage()));

}


PKG_Main::~PKG_Main()
{
}


/*
** refreshPackages - Calls packageSelected which causes a refresh on the
**                   tabs.
*/

void PKG_Main::refreshPackages(void)
{
    packageSelected(packageList->currentItem());
}

/*
** loadPackages - Loads the list of packages and fills the list with
**                them.
*/

void PKG_Main::loadPackages()
{
    ADB     DB;
    char    tmpStr[1024];

    packageList->clear();    
    DB.query("select InternalID, PackageTag, Active from Packages");
    
    if (DB.rowCount) while (DB.getrow()) {
        if (atoi(DB.curRow["Active"])) {
            sprintf(tmpStr, "Y");
        } else {
            sprintf(tmpStr, "N");
        }
        
        (void) new Q3ListViewItem(packageList, DB.curRow["PackageTag"], tmpStr, DB.curRow["InternalID"]);
    }
    

    for (unsigned int i = 0; i < 4 ; i++) {
        theTabBar->setTabEnabled(i, FALSE);
    }

    theTabBar->setCurrentTab(0);
    deleteButton->setEnabled(FALSE);

}


/*
** packageSelected - Calls each of the tabs so that they load the correct
**                     information when a login type is selected.
*/

void PKG_Main::packageSelected(Q3ListViewItem *curItem)
{
    long    pkgID = 0;
    if (curItem) {
        for (unsigned int i = 0; i < 4 ; i++) {
            theTabBar->setTabEnabled(i, TRUE);
        }
        pkgID = atol(curItem->key(2, 0));
        deleteButton->setEnabled(TRUE);
    } else {
        for (unsigned int i = 0; i < 4 ; i++) {
            theTabBar->setTabEnabled(i, FALSE);
        }
        deleteButton->setEnabled(FALSE);
    }
    // theTabBar->setCurrentTab(0);
    tgeneral->setPackageID(pkgID);
    tconversion->setPackageID(pkgID);
    tpricing->setPackageID(pkgID);
    tbillables->setPackageID(pkgID);
}

/*
** newPackage - Inserts a new package entry in the database and makes
**              it the currently selected one.
*/

void PKG_Main::newPackage()
{
    Q3ListViewItem   *newItem;
    ADBTable    PTDB;
    PTDB.setTableName("Packages");
    
    PTDB.setValue("InternalID",  (long) 0);
    PTDB.setValue("PackageTag",   "New Package");
    PTDB.setValue("Description",  "New Package");
    PTDB.setValue("PackageType",  0);
    PTDB.setValue("AutoConvert",  0);
    PTDB.setValue("ConvertDays",  0);
    PTDB.setValue("ConvertDate",  "0000-00-00");
    PTDB.setValue("ConvertToID",  (long) 0);
    PTDB.setValue("PromoPackage", 0);
    PTDB.setValue("PromoEnds",    "0000-00-00");
    PTDB.setValue("Active",       0);
    
    PTDB.ins();
    
    newItem = new Q3ListViewItem(packageList, PTDB.getStr("PackageTag"), "N", PTDB.getStr("InternalID"));
    packageList->setSelected(newItem, TRUE);
    packageList->setCurrentItem(newItem);
    
}


/*
** deletePackage - Checks to see if the package is in use.  If not
**                 it prompts to verify that the user wishes to delete it.
**                 if it is in use, it tells the user that it cannot be
**                 deleted until it is no longer in use.
*/

void PKG_Main::deletePackage()
{
    Q3ListViewItem   *curItem = packageList->currentItem();
    // First, make sure that our current item is not null
    if (curItem == NULL) return;
    
    // Okay, now make sure that it is not in use.
    QApplication::setOverrideCursor(waitCursor);
    ADB     DB;
    long    myIntID = atol(curItem->key(2, 0));
    DB.query("select InternalID from Subscriptions where PackageNo = %ld", myIntID);
    QApplication::restoreOverrideCursor();
    
    if (DB.rowCount) {
        // This login type is in use.  Let the user know.
        QMessageBox::critical(this, "Delete Package", "The package is in use by customers, and may not be deleted.\n\nUpdate the customer subscriptions before proceeding.");
    } else {
        switch(QMessageBox::information(this, "Delete Package", 
          "Are you sure you wish to delete\nthe currently selected package?",
          "&No", "&Yes", "&Cancel", 0, 2)) {
          
            case 1:     // Delete it.
                QApplication::setOverrideCursor(waitCursor);
                DB.dbcmd("delete from Packages where InternalID = %ld", myIntID);
                DB.dbcmd("delete from PackageContents where PackageID = %ld", myIntID);
                DB.dbcmd("delete from PackagesData where PackageID = %ld", myIntID);
                
                // Refresh our list
                loadPackages();
                packageSelected(NULL);
                QApplication::restoreOverrideCursor();
                break;
                
            default:    // Don't delete it
                break;
        }
    }
}


/*
** updatePackage - A slot that gets called when the General tab emits
**                packageChanged.  This way we can refresh the current
**                item without reloading the list.
*/

void PKG_Main::updatePackage()
{
    Q3ListViewItem   *curItem = packageList->currentItem();
    
    if (curItem != NULL) {
        char        tmpStr[1024];
		ADBTable    PDB;
		PDB.setTableName("Packages");

	    PDB.get(atol(curItem->key(2,0)));
	    
        curItem->setText(0, PDB.getStr("PackageTag"));
        if (PDB.getInt("Active")) {
            strcpy(tmpStr, "Y");
        } else {
            strcpy(tmpStr, "N");
        }
        curItem->setText(1, tmpStr);
    
    }
}


// vim: expandtab
