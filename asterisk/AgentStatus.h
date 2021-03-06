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


#ifndef AgentStatus_included
#define AgentStatus_included

#define AGENT_STATUS_SIGNEDOUT 0
#define AGENT_STATUS_AVAILABLE 1
#define AGENT_STATUS_ONBREAK 2

#include <sys/time.h>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3ptrlist.h>
#include <Qt3Support/q3popupmenu.h>

#include <TAAWidget.h>
#include "AsteriskManager.h"

struct statusInfoStruct {
    time_t      lastchange;
    int         paused;
    int         status;
};

// An individual agent's widget.  The AgentStatus will create a list
// of these widgets, one for each agent.
class AgentInfo : public TAAWidget
{
    Q_OBJECT

public:
    AgentInfo(QWidget *parent = NULL, const char *name = NULL);
    virtual ~AgentInfo();

    const char  *agentName();
    const char  *agentLocation();
    bool        isExpanded();

    void        setAgentName(const char *);
    void        setLocation(const char *);
    void        setStatus(int, const char *);
    int         getStatus();
    void        setCallsTaken(int);
    void        setLastCallTime(int);
    void        setLastStatusChange(time_t);
    time_t      getLastStatusChange();
    void        setTimeInState(int, int, int);

public slots:
    void        setExpanded(bool);

protected:
    QLabel      *agentname;
    QLabel      *location;
    QLabel      *status;
    QLabel      *timeinstate;
    QLabel      *guistatus;
    QLabel      *callstaken;
    QLabel      *lastcall;
    QPushButton *expandButton;
    time_t      lastchange;
    int         curstatus;

    bool        intIsExpanded;

    void        updateExpandedPrefs();

protected slots:
    void        expandedClicked();
};

// The main agent status widget, shows all the other widgets.
class AgentStatus : public TAAWidget
{
    Q_OBJECT

public:

    AgentStatus(
        AsteriskManager *mgrptr,
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~AgentStatus();

signals:
    void                sendAsteriskCommand(int);

protected:
    int                 agentCount;
    AsteriskManager     *am;
    /*
    QPtrList<QLabel>    names;
    QPtrList<QLabel>    locations;
    QPtrList<QLabel>    status;
    QPtrList<QLabel>    timeinstate;
    QPtrList<QLabel>    guistatus;
    QPtrList<QLabel>    callstaken;
    QPtrList<QLabel>    lastcall;
    QPtrList<statusInfoStruct>  statusinfo;
    */
    Q3PtrList<AgentInfo> agentList;
    QPushButton         *setStatusButton;
    Q3PopupMenu          *setStatusMenu;              
    int                 myPosition;

protected slots:
    void        asteriskEvent(const astEventRecord);
    void        refreshAgentList();
    void        agentSetStatus(int);
    void        updateTimeInState();
};

#endif // AgentStatus_included

// vim: expandtab

