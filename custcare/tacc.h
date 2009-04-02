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

#ifndef TACC_H 
#define TACC_H

#include <QtCore/QTimer>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QTabBar>
#include <Qt3Support/Q3PopupMenu>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3progressbar.h>

#include <TAAWidget.h>
#include <TAATools.h>
#include <TicketManager.h>
#include <Customers.h>
#include <ProcessVoiceMail.h>
#include <AsteriskManager.h>
#include <QueueMonitor.h>
#include <AgentStatus.h>
#include <Customers.h>

class CustomerCareStack : public TAAWidget
{
    Q_OBJECT

public:
    CustomerCareStack ( AsteriskManager *astmgr, QWidget *parent = NULL, const char *name = NULL);
    virtual ~CustomerCareStack();
    
    TicketManager       *tickets;
    Customers           *custs;
    ProcessVoiceMail    *processVM;
    //Administration      *adm;
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
    Q3WidgetStack    *qws;
};


class CustomerCare : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCare();
    virtual ~CustomerCare();

signals:
    // mainWin() relay signals
    void            refreshCustomer(long);
    void            refreshTicket(long);
    void            openCustomer(long);

public slots:
    void            takeCall();
    void            customerList();

    // mainWin() relay slots
    void            setStatusMW(const char *);
    void            setStatusMW(const char *, int);
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
    void            loginflaglist();
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
    void            emailTemplateList();
    void            toggleCallQueue();
    void            toggleAgentStatus();
    void            settings();

    void            asteriskEvent(const astEventRecord);

protected:
    CustomerCareStack   *ccStack;
    QTabBar         *tabs;
    QTab            *ticketTab;
    QTab            *customerTab;
    Q3WidgetStack    *qws;
    QLabel          *clockLabel;
    QTimer          *clockTimer;
    QLabel          *countLabel;
    Q3ProgressBar    *progressMeter;
    bool            realTimeProgress;
    AsteriskManager *am;
    Customers       *clist;
    Q3PopupMenu      *viewMenu;
    int             showQueueMenuItem;
    int             showAgentsMenuItem;
};


int main(int argc, char **argv);

#endif // TACC_H

// vim: expandtab
