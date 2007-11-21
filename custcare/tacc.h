/*
** $Id: tacc.h,v 1.5 2004/02/27 01:33:33 marc Exp $
**
***************************************************************************
**
** tacc - Total Accountability Customer Care interface.
**        This is the main program that Blarg employees will use.  It
**        encapsulates the main menu, the Main Window, a Take Call
**        interface, and pulls in the active ticket list.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#ifndef TACC_H 
#define TACC_H

#include <qwidget.h>
#include <qmainwindow.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qwidgetstack.h>
#include <qtabbar.h>
#include <qprogressbar.h>
#include "TAATools.h"
#include "TicketManager.h"
#include "Customers.h"
#include "ProcessVoiceMail.h"
#include <admin.h>
#include "AsteriskManager.h"
#include "QueueMonitor.h"
#include "AgentStatus.h"
#include "Customers.h"

class CustomerCareStack : public TAAWidget
{
    Q_OBJECT

public:
    CustomerCareStack ( AsteriskManager *astmgr, QWidget *parent = NULL, const char *name = NULL);
    virtual ~CustomerCareStack();
    
    TicketManager       *tickets;
    Customers           *custs;
    ProcessVoiceMail    *processVM;
    Administration      *adm;
    QueueMonitor        *qMon;
    AgentStatus         *agents;

public slots:
    void            raiseTab1();
    void            raiseTab2();
    void            raiseTab3();

protected slots:
    void            tabClicked(int);

protected:
    QTabBar         *tabs;
    QTab            *ticketTab;
    QTab            *customerTab;
    QTab            *voicemailTab;
    QTab            *adminTab;
    QWidgetStack    *qws;
};


class CustomerCare : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCare ( QWidget *parent = NULL, const char *name = NULL);
    virtual ~CustomerCare();
    QApplication    *appRef;

signals:
    // mainWin() relay signals
    void            refreshCustomer(long);
    void            refreshTicket(long);

public slots:
    void            takeCall();
    void            customerList();

    // mainWin() relay slots
    void            setStatusMW(const char *);
    void            setProgressMW(int, int);
    void            setProgressMWRT(int, int);
    void            openCustomerMW(long);
    void            openTicketMW(long);
    void            refreshCustomerMW(long);
    void            customerChanged(long);
    void            refreshTicketMW(long);
    void            ticketChanged(long);

protected slots:
    void            quitSelected();
    void            updateClock();
    void            about()             { showAbout(); };
    void            staffEditor();
    void            run_subscriptions();
    void            process_ccbatch();
    void            export_ccbatch();
    void            create_statements();
    void            billableitemslist();
    void            packagelist();
    void            paymenttermslist();
    void            logintypelist();
    void            domaintypelist();
    void            dnstemplates();
    void            processOverdueAccounts();
    void            processWipedAccounts();
    void            printStatements();
    void            sendTargetedEmail();
    void            settleCCBatch();
    void            accountlist();
    void            make_deposits();
    void            checkDBIntegrity();
    void            updateAllMaxMailboxes();
    void            ratePlanList();
    void            serverGroupList();
    void            billingCycleList();
    void            vendorList();
    void            vendorTypeList();
    void            voipAddDIDs();
    void            voipOriginationProviderList();
    void            voipServiceTypeList();
    void            manageRateCenters();
    void            openCompanyList();
    void            settings();

    void            asteriskEvent(const astEventRecord);

protected:
    CustomerCareStack   *ccStack;
    QTabBar         *tabs;
    QTab            *ticketTab;
    QTab            *customerTab;
    QWidgetStack    *qws;
    QLabel          *clockLabel;
    QTimer          *clockTimer;
    QLabel          *countLabel;
    QProgressBar    *progressMeter;
    bool            realTimeProgress;
    AsteriskManager *am;
    Customers       *clist;
};


int main(int argc, char **argv);

#endif // TACC_H
