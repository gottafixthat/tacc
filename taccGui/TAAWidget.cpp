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

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <Qt3Support/Q3Frame>

#include <TAATools.h>
#include "TAAWidget.h"
#include <ADB.h>


// TAAWidget

TAAWidget::TAAWidget(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    // Connect the relay slots to the main window.
    connect(this, SIGNAL(openCustomer(long)), mainWin(), SLOT(openCustomerMW(long)));
    connect(this, SIGNAL(openTicket(long)), mainWin(), SLOT(openTicketMW(long)));
    connect(this, SIGNAL(setStatus(const char *)), mainWin(), SLOT(setStatusMW(const char *)));
    connect(this, SIGNAL(setStatus(const char *, int)), mainWin(), SLOT(setStatusMW(const char *, int)));
    connect(this, SIGNAL(setProgress(int, int)), mainWin(), SLOT(setProgressMW(int, int)));
    connect(this, SIGNAL(setProgressRT(int, int)), mainWin(), SLOT(setProgressMWRT(int, int)));

    // Connect our signals to top level widgets.
    /*
    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
        qDebug() << "Top widget name = '" << widget << "'";
        if (!strcmp(widget->className(), "CustomerCare")) {
            qDebug() << "Connecting signal to " << widget->className();
            connect(this, SIGNAL(openCustomer(long)), widget, SLOT(openCustomerMW(long)));
            //connect(this, SIGNAL(openCustomer(long)), widget->centralWidget(), SLOT(openCustomerMW(long)));
        }
    }
    this->dumpObjectInfo();
    */

    // Relay slots.  Individual TAAWidgets will emit customerUpdated() whenever
    // they update customer information.  This, in turn, will call the 
    // mainWin() member function refreshCustomerMW().  refreshCustomerMW()
    // will then emit refreshCustomer(), which is a virtual function contained
    // by every TAAWidget instance.  This makes it so any window in TAA
    // can cause updates to every other open window in the system without
    // keeping a list of windows.
    connect(this, SIGNAL(customerUpdated(long)), mainWin(), SLOT(refreshCustomerMW(long)));
    connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
    connect(this, SIGNAL(ticketUpdated(long)), mainWin(), SLOT(refreshTicketMW(long)));
    connect(mainWin(), SIGNAL(refreshTicket(long)), this, SLOT(refreshTicket(long)));
}

TAAWidget::~TAAWidget()
{
}

/** updateUserPref - Updates the database with the user preferences.
  */
void TAAWidget::setUserPref(const char *key, const char *subkey, const char *val)
{
    ADB     DB;
    DB.query("select PrefData from UserPrefs where LoginID = '%s' and PrefKey = '%s' and PrefSubKey = '%s'", curUser().userName, key, subkey);
    if (DB.rowCount) {
        DB.dbcmd("update UserPrefs set PrefData = '%s' where LoginID = '%s' and PrefKey = '%s' and PrefSubKey = '%s'", val, curUser().userName, key, subkey);
    } else {
        DB.dbcmd("insert into UserPrefs (LoginID, PrefKey, PrefSubKey, PrefData) values('%s', '%s', '%s', '%s')", curUser().userName, key, subkey, val);
    }
}

/** getUserPref - Returns the requested user preference
  */
const char *TAAWidget::getUserPref(const char *key, const char *subkey)
{
    char    *retStr;
    ADB     DB;
    DB.query("select PrefData from UserPrefs where LoginID = '%s' and PrefKey = '%s' and PrefSubKey = '%s'", curUser().userName, key, subkey);
    if (DB.rowCount) {
        DB.getrow();
        retStr = new(char[strlen(DB.curRow["PrefData"]) + 2]);
        strcpy(retStr, DB.curRow["PrefData"]);
    } else {
        retStr = new(char[2]);
        strcpy(retStr, "");
    }
    return (const char *) retStr;
}
    

HorizLine::HorizLine(QWidget *parent, const char *name, Qt::WFlags f)
    : QLabel(parent, name, f)
{
    setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    setMinimumSize(0,4);
    setMaximumSize(32767, 4);
}

HorizLine::~HorizLine()
{
}

VertLine::VertLine(QWidget *parent, const char *name, Qt::WFlags f)
    : QLabel(parent, name, f)
{
    setFrameStyle(Q3Frame::VLine|Q3Frame::Sunken);
    setMinimumSize(3,0);
    setMaximumSize(3,32767);
}

VertLine::~VertLine()
{
}


// vim: expandtab
