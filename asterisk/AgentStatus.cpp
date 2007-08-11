/** AgentStatus - Monitors the Blarg Sales/Support queue(s)
  ***************************************************************************
  * Written by R. Marc Lewis, 
  *   (C)opyright 1998-2006, R. Marc Lewis and Blarg! Oline Services, Inc.
  *   All Rights Reserved.
  *
  *  Unpublished work.  No portion of this file may be reproduced in whole
  *  or in part by any means, electronic or otherwise, without the express
  *  written consent of Blarg! Online Services and R. Marc Lewis.
  ***************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <qlayout.h>
#include <qlistview.h>

#include "ColorListViewItem.h"

#include "AgentStatus.h"
#include "Cfg.h"

#include <TAAWidget.h>
#include <TAATools.h>
#include <TAAPixmaps.h>
#include <qsqldatabase.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <ADB.h>

/**
  * AgentInfo is the container for a single agent's status.
  */
AgentInfo::AgentInfo
(
    QWidget *parent,
    const char *name
) : TAAWidget(parent, name)
{
    agentname = new QLabel(this, "agentname");
    location = new QLabel(this, "location");
    status = new QLabel(this, "status");
    timeinstate = new QLabel(this, "timeinstate");
    guistatus = new QLabel(this, "guistatus");
    callstaken = new QLabel(this, "callstaken");
    lastcall = new QLabel(this, "lastcall");
    expandButton = new QPushButton(this, "expandButton");
    expandButton->setFlat(true);
    connect(expandButton, SIGNAL(clicked()), this, SLOT(expandedClicked()));
    //expandButton->setPixmap(down_xpm);

    location->setText("Location");
    status->setText("Signed Out");
    timeinstate->setText("00:00");
    callstaken->setText("0 Total");
    lastcall->setText("Last: N/A");

    // By default we don't want to show everything,
    // only the basics.
    intIsExpanded = true;
    setExpanded(false);


    // Create the layouts for it.
    //QBoxLayout *ml = new QBoxLayout(QBoxLayout::TopToBottom, 4);
    
    // Create a layout for each agent
    QGridLayout *wl = new QGridLayout(this, 3, 4, 2, 1);
    wl->setColStretch(0, 0);
    wl->setColStretch(1, 1);
    wl->setColStretch(2, 1);
    wl->setColStretch(3, 0);
    wl->setColSpacing(2, 5);
    int curRow = 0;
    wl->addMultiCellWidget(guistatus,   curRow, curRow+1, 0, 0, AlignLeft|AlignTop);
    wl->addWidget(agentname,            curRow, 1, AlignLeft|AlignVCenter);
    wl->addWidget(location,             curRow, 2, AlignRight|AlignVCenter);
    wl->addMultiCellWidget(expandButton,curRow, curRow+1, 3, 3, AlignLeft|AlignTop);
    curRow++;
    wl->addMultiCellWidget(status,      curRow, curRow, 1, 1, AlignLeft|AlignTop);
    wl->addMultiCellWidget(timeinstate, curRow, curRow, 2, 2, AlignRight|AlignTop);
    curRow++;
    wl->addMultiCellWidget(lastcall,    curRow, curRow, 1, 1, AlignLeft|AlignTop);
    wl->addMultiCellWidget(callstaken,  curRow, curRow, 2, 2, AlignRight|AlignTop);
    curRow++;

    HorizLine *tmpLine = new HorizLine(this);
    wl->addMultiCellWidget(tmpLine, curRow,curRow,0,3);
    curRow++;
    //ml->addLayout(wl, 0);
    //ml->addStretch(1);

}
  
AgentInfo::~AgentInfo()
{
}

/**
  * agentName - Returns the name of the agent.
  */
const char *AgentInfo::agentName()
{
    return (const char *) agentname->text();
}

/**
  * agentLocation - Returns the location of the agent.
  */
const char *AgentInfo::agentLocation()
{
    return (const char *) location->text();
}

/**
  * isExpanded - Returns true or false depending on whether
  * or not this agent's information is expanded.
  */
bool AgentInfo::isExpanded()
{
    return intIsExpanded;
}

/**
  * setAgentName - Sets the text in the agent name box.
  */
void AgentInfo::setAgentName(const char *newName)
{
    agentname->setText(newName);
}

/**
  * setLocation - Sets the text in the agent location box.
  */
void AgentInfo::setLocation(const char *newLoc)
{
    location->setText(newLoc);
}

/**
  * setCallsTaken - Sets the number of calls the the agent has taken.
  */
void AgentInfo::setCallsTaken(int calls)
{
    char    tmpStr[1024];
    sprintf(tmpStr, "%d Total", calls);
    callstaken->setText(tmpStr);
}

/**
  * setLastCallTime - Sets the last time that the agent took a call.
  */
void AgentInfo::setLastCallTime(int last)
{
    QDateTime   tmpTime;
    char        tmpStr[1024];
    tmpTime.setTime_t(last);
    sprintf(tmpStr, "Last: %s", (const char *)tmpTime.toString("h:mmap"));
    lastcall->setText(tmpStr);
}

/**
  * setLastStatusChange - Sets the last time the status of this user
  * changed.
  */
void AgentInfo::setLastStatusChange(time_t tmpTime)
{
    lastchange = tmpTime;
}

/**
  * getLastStatusChange - Returns the last status change.
  */
time_t AgentInfo::getLastStatusChange()
{
    return lastchange;
}

/**
  * setTimeInState - Updates the time in state label.
  */
void AgentInfo::setTimeInState(int h, int m, int s)
{
    char    tmpStr[1024];
    sprintf(tmpStr, "%d:%02d:%02d", h, m, s);
    timeinstate->setText(tmpStr);
}

/**
  * setStatus - Sets the status of the agent.
  */
void AgentInfo::setStatus(int newStatus, const char *textStatus)
{
    QPixmap signedout(signedout_xpm);
    QPixmap avail(available_xpm);
    QPixmap onbreak(onbreak_xpm);
    QPixmap user_icon(user_icon_xpm);
    QPixmap busy_icon(busy_xpm);
    switch (newStatus) {
        case AGENT_STATUS_AVAILABLE:
            guistatus->setPixmap(avail);
            break;
        case AGENT_STATUS_ONBREAK:
            guistatus->setPixmap(onbreak);
            break;
        case AGENT_STATUS_SIGNEDOUT:
        default:
            guistatus->setPixmap(signedout);
            break;
    }
    status->setText(textStatus);
}

/**
  * getStatus - Returns the status of the agent.
  */
int AgentInfo::getStatus()
{
    return curstatus;
}

/**
  * setExpanded - Expands or shrinks the status by hiding certain widgets.
  */
void AgentInfo::setExpanded(bool newVal)
{
    if (newVal == intIsExpanded) return;

    QPixmap downmap(down_xpm);
    QPixmap upmap(up_xpm);
    bool    newHidden = true;
    if (newVal) {
        newHidden = false;
        expandButton->setPixmap(upmap);
    } else {
        expandButton->setPixmap(downmap);
    }

    status->setHidden(newHidden);
    timeinstate->setHidden(newHidden);
    callstaken->setHidden(newHidden);
    lastcall->setHidden(newHidden);


    // Update the user preferences

    intIsExpanded = newVal;

    updateExpandedPrefs();
}

/**
  * updateExpandedPrefs
  */
void AgentInfo::updateExpandedPrefs()
{
    if (!agentname->text().length()) return;
    char    tmpSt[1024];
    int tmpExp = 0;
    if (intIsExpanded) {
        tmpExp = 1;
    }
    sprintf(tmpSt, "%d", tmpExp);
    setUserPref("AgentListExpanded", (const char *)agentname->text(), tmpSt);
}

/**
  * expandedClicked - The slot that gets called when the user wants to 
  * expand or condense the view.
  */
void AgentInfo::expandedClicked()
{
    if (intIsExpanded) setExpanded(false);
    else setExpanded(true);
    //setExpanded(false ? intIsExpanded : true);
}


/**
  * AgentStatus is the main widget container and controller
  * for all of the AgentInfo widgets.
  */

AgentStatus::AgentStatus
(
    AsteriskManager *mgrptr,
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Queue Monitor" );

    agentCount = 0;

    am = mgrptr;

    // Connect to the database and get the agents
    QSqlDatabase        *myDB;
    myDB = QSqlDatabase::addDatabase("QMYSQL3","asteriskagentstatus");
    myDB->setHostName(cfgVal("TAAMySQLHost"));
    myDB->setDatabaseName(cfgVal("TAAMySQLDB"));
    myDB->setUserName(cfgVal("TAAMySQLUser"));
    myDB->setPassword(cfgVal("TAAMySQLPass"));

    if (!myDB->open()) {
        fprintf(stderr, "AgentStatus::AgentStatus() Unable to connect to the databse!\n");
        return;
    }

    // Get the agents out of the database.
    QSqlQuery query(myDB);
    query.exec("select LoginID, AgentID, Extension, QueueName from Staff where Active > 0 and AgentID <> ''");
    if (query.size() < 1) {
        fprintf(stderr, "AgentStatus::AgentStatus() No agents found!\n");
        return;
    }

    ADB     DB;

    agentCount = query.size();
    // Create our widgets.
    QPixmap signedout(signedout_xpm);
    QPixmap avail(available_xpm);
    QPixmap onbreak(onbreak_xpm);
    QPixmap user_icon(user_icon_xpm);
    QPixmap busy_icon(busy_xpm);
    int i = 0;
    myPosition = 0;
    while (query.next()) {
        AgentInfo   *tmpAgent = new AgentInfo(this);
        tmpAgent->setAgentName(query.value(0).toString());
        tmpAgent->setStatus(AGENT_STATUS_SIGNEDOUT, "Signed Out");
        tmpAgent->setLocation("Agent/" + query.value(2).toString());
        tmpAgent->setLastStatusChange(QDateTime::currentDateTime().toTime_t());
        // Check to see if we prefer this agent expanded or not.
        QString tmpQSt = getUserPref("AgentListExpanded",  (const char *)query.value(0).toString());
        if (tmpQSt.length()) {
            tmpAgent->setExpanded(tmpQSt.toInt());
        }
        agentList.append(tmpAgent);

        if (!strcmp(curUser().agentID, query.value(1).toString())) {
            myPosition = i;
        }
        i++;
    }

    QBoxLayout *cl = new QBoxLayout(this, QBoxLayout::LeftToRight, 0, 0);
    VertLine *vertBar = new VertLine(this);
    cl->addWidget(vertBar, 0);

    QBoxLayout *ml = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    
    // Create a layout for each agent
    QGridLayout *wl = new QGridLayout(2, agentCount * 3, 2);
    int curRow = 0;
    for (i = 0; i < agentCount; i++) {
        wl->addMultiCellWidget(agentList.at(i), curRow, curRow, 0, 2);
        curRow++;

    }
    ml->addLayout(wl, 0);
    ml->addStretch(1);

    setStatusMenu = new QPopupMenu(this);
    setStatusMenu->insertItem(avail, "Available", am, SLOT(signMeIn()));
    setStatusMenu->insertItem(onbreak, "On Break", am, SLOT(goOnBreak()));
    setStatusMenu->insertItem(signedout, "Signed Out", am, SLOT(signMeOut()));
    if (isAdmin()) {
        setStatusMenu->insertSeparator();
        QPopupMenu *agentsMenu = new QPopupMenu(this);
        ADB     db;
        db.query("select * from Staff where Active > 0 and Extension <> '' order by LoginID");
        if (db.rowCount) {
            // Add in all agents but ourselves.
            while (db.getrow()) if (strcmp(curUser().userName, db.curRow["LoginID"])) {
                int i;
                QPopupMenu *agent = new QPopupMenu(this);

                i = agent->insertItem(avail, "Available", atoi(db.curRow["Extension"])+10000);
                agent->connectItem(i, this, SLOT(agentSetStatus(int)));
                agent->setItemParameter(i, i);

                i = agent->insertItem(onbreak, "On Break", atoi(db.curRow["Extension"])+20000);
                agent->connectItem(i, this, SLOT(agentSetStatus(int)));
                agent->setItemParameter(i, i);

                i = agent->insertItem(signedout, "Signed Out", atoi(db.curRow["Extension"])+30000);
                agent->connectItem(i, this, SLOT(agentSetStatus(int)));
                agent->setItemParameter(i, i);

                agentsMenu->insertItem(user_icon, db.curRow["LoginID"], agent);
            }
        }
        // Create another menu for the admin to log people in and out
        setStatusMenu->insertItem(user_icon, "Agents", agentsMenu);
    }
    setStatusButton = new QPushButton(signedout, "Signed Out", this);
    setStatusButton->setPopup(setStatusMenu);
    ml->addWidget(setStatusButton, 0);

    cl->addLayout(ml, 1);

    QTimer::singleShot(1000, this, SLOT(refreshAgentList()));

    QTimer  *timeInStateTimer = new QTimer(this);
    connect(timeInStateTimer, SIGNAL(timeout()), this, SLOT(updateTimeInState()));
    timeInStateTimer->start(1000, false);   // Update once per second

    // Make our connections to the Asterisk Manager
    connect(am, SIGNAL(eventReceived(const astEventRecord)), this, SLOT(asteriskEvent(const astEventRecord)));

    // Done.
}

AgentStatus::~AgentStatus()
{
}

/** asteriskEvent - This is a slot that should be connected to the
  * AsteriskManager.  It will take an event from the system and
  * update the queue accordingly.
  */
void AgentStatus::asteriskEvent(const astEventRecord event)
{
    QColor  rgb(153,204,204);
    QString     loc;
    QString     newStatus;
    int         newGuiStatus;
    int         memberStatus = -1;
    int         calls = -1;
    uint        lastcalltime = 0;
    char        tmpStr[1024];
    QPixmap     avail(available_xpm);
    QPixmap     onbreak(onbreak_xpm);
    QPixmap     signedout(signedout_xpm);
    QPixmap     user_icon(user_icon_xpm);
    QDateTime   curDateTime = QDateTime::currentDateTime();

    if (!strcasecmp(event.msgVal, "QueueMemberAdded")) {
        // An agent is joining the call queue
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Location")) loc = event.data[i].val;
            else if (!strcasecmp(event.data[i].key, "Status")) memberStatus = atoi(event.data[i].val);
        }
        newGuiStatus = AGENT_STATUS_AVAILABLE;
        newStatus = "Available";
        //newGuiStatus = &avail;
    } else if (!strcasecmp(event.msgVal, "QueueMemberRemoved")) {
        // An agent is signing out.
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Location")) loc = event.data[i].val;
            else if (!strcasecmp(event.data[i].key, "Status")) memberStatus = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallsTaken")) calls = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "LastCall")) lastcalltime = atoi(event.data[i].val);
        }
        newStatus = "Signed Out";
        newGuiStatus = AGENT_STATUS_SIGNEDOUT;
        //newGuiStatus = signedout;
        //newGuiStatus.loadFromData(onbreak_xpm, strlen(onbreak_xpm));
    } else if (!strcasecmp(event.msgVal, "QueueMemberPaused")) {
        // An agent is signing out.
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Location")) loc = event.data[i].val;
            else if (!strcasecmp(event.data[i].key, "Paused")) {
                if (!strcmp(event.data[i].val, "0")) {
                    newStatus = "Available";
                    newGuiStatus = AGENT_STATUS_AVAILABLE;
                } else {
                    newStatus = "On Break";
                    newGuiStatus = AGENT_STATUS_ONBREAK;
                }
            } else if (!strcasecmp(event.data[i].key, "Status")) memberStatus = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallsTaken")) calls = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "LastCall")) lastcalltime = atoi(event.data[i].val);
        }
        //newGuiStatus = signedout;
        //newGuiStatus.loadFromData(onbreak_xpm, strlen(onbreak_xpm));
    } else if (!strcasecmp(event.msgVal, "QueueMember") || !strcasecmp(event.msgVal, "QueueMemberStatus")) {
        // An agent is signing out.
        int isPaused = 0;
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Location")) loc = event.data[i].val;
            else if (!strcasecmp(event.data[i].key, "Paused")) {
                if (!strcmp(event.data[i].val, "1")) {
                    isPaused = 1;
                }
            } else if (!strcasecmp(event.data[i].key, "Status")) memberStatus = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallsTaken")) calls = atoi(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "LastCall")) lastcalltime = atoi(event.data[i].val);
        }
        if (memberStatus > 3 && memberStatus < 6) {
            newGuiStatus = AGENT_STATUS_SIGNEDOUT;
        } else {
            newGuiStatus = AGENT_STATUS_AVAILABLE;
            if (isPaused) {
                newStatus = "On Break";
                newGuiStatus = AGENT_STATUS_ONBREAK;
            }
        }
        //newGuiStatus.loadFromData(onbreak_xpm, strlen(onbreak_xpm));
    } else if (!strcasecmp(event.msgVal, "Agents")) {
        // Agent status update
        int isPaused = 0;
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Agent")) {
                sprintf(tmpStr, "Agent/%s", (const char *) event.data[i].val);
                loc = tmpStr;
            }
            else if (!strcasecmp(event.data[i].key, "Status")) {
                if (!strcasecmp(event.data[i].val, "AGENT_LOGGEDOFF")) {
                    memberStatus = 5;
                } else if (!strcasecmp(event.data[i].val, "AGENT_IDLE")) {
                    memberStatus = 1;
                } else if (!strcasecmp(event.data[i].val, "AGENT_ONCALL")) {
                    memberStatus = 2;
                } else {
                    memberStatus = 0;
                }
            } 
        }
    } else if (!strcasecmp(event.msgVal, "Agentcallbacklogoff")) {
        // Agent status update
        int isPaused = 0;
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Agent")) {
                sprintf(tmpStr, "Agent/%s", (const char *) event.data[i].val);
                loc = tmpStr;
            }
            memberStatus = 5;
        }
    }

    if (memberStatus >= 0) {
        switch(memberStatus) {
            case 1:
                newStatus = "Available";
                break;
            case 2:
                newStatus = "On Call";
                break;
            case 3:
                newStatus = "Busy";
                break;
            case 4:
                newStatus = "Device Invalid";
                break;
            case 5:
                newStatus = "Unavailable";
                break;
            case 6:
                newStatus = "Ringing";
                break;
            default:
                newStatus = "Unknown";
        }
    }

    // Now, walk through the list of agents and find the one who's status we're changing.
    // But only if we have a status and a location.
    if (loc.isEmpty()) return;
    if (newStatus.isEmpty()) return;

    for (int i = 0; i < agentCount; i++) {
        // Update the agent class
        AgentInfo *tmpAgent = agentList.at(i);
        if (!strcasecmp(tmpAgent->agentLocation(), loc)) {
            // Found them.
            tmpAgent->setStatus(newGuiStatus, (const char *)newStatus);
            if (calls >= 0) {
                tmpAgent->setCallsTaken(calls);
            }
            if (lastcalltime > 0) {
                tmpAgent->setLastCallTime(lastcalltime);
            }
            // Reset the time, but only if its not us.
            if ((newGuiStatus != tmpAgent->getStatus()) && (i != myPosition)) {
                //statusinfo.at(i)->status = newGuiStatus;
                tmpAgent->setLastStatusChange(curDateTime.toTime_t());
            }

        }
    }

    // Check to see if the status was our own extension
    //fprintf(stderr, "My Location: '%s', Examining Location: '%s'\n", curUser().location, (const char *)loc);
    if (!strcasecmp(curUser().agentID, loc)) {
        // It was.  Set our status.
        switch (newGuiStatus) {
            case AGENT_STATUS_AVAILABLE:
                setStatusButton->setText("Available");
                setStatusButton->setIconSet(avail);
                break;
            case AGENT_STATUS_ONBREAK:
                setStatusButton->setText("On Break");
                setStatusButton->setIconSet(onbreak);
                break;
            case AGENT_STATUS_SIGNEDOUT:
            default:
                newGuiStatus = AGENT_STATUS_SIGNEDOUT;
                setStatusButton->setText("Signed Out");
                setStatusButton->setIconSet(signedout);
                break;
        }

        //fprintf(stderr, "Checking to see if I need to update myself, myPosition = %d.\n", myPosition);
        if (newGuiStatus != agentList.at(myPosition)->getStatus()) {
        //if (newGuiStatus != statusinfo.at(myPosition)->status) {
            // Update the database with this change.
            //fprintf(stderr, "Inserting agent change into the database...\n");
            QSqlDatabase        *myDB;
            myDB = QSqlDatabase::database("asteriskagentstatus");
            myDB->setHostName(cfgVal("TAAMySQLHost"));
            myDB->setDatabaseName(cfgVal("TAAMySQLDB"));
            myDB->setUserName(cfgVal("TAAMySQLUser"));
            myDB->setPassword(cfgVal("TAAMySQLPass"));

            if (!myDB->open()) {
                fprintf(stderr, "AgentStatus::AgentStatus() Unable to connect to the databse!\n");
            } else {
                QSqlQuery query(myDB);

                QDateTime   startTime;
                //startTime.setTime_t(statusinfo.at(myPosition)->lastchange);
                startTime.setTime_t(agentList.at(myPosition)->getLastStatusChange());
                char        tmpStr[1024];
                sprintf(tmpStr, "replace into AgentStatus (LoginID, State, StartTime, EndTime, ElapsedSeconds) values ('%s', %d, '%s', '%s', %ld)",
                        curUser().userName,
                        agentList.at(myPosition)->getStatus(),
                        (const char *) startTime.toString("yyyy-MM-dd hh:mm:ss"),
                        (const char *) curDateTime.toString("yyyy-MM-dd hh:mm:ss"),
                        startTime.secsTo(curDateTime));
                query.exec(tmpStr);
            }

            //fprintf(stderr, "Finished inserting agent change into the database...\n");

            agentList.at(myPosition)->setStatus(newGuiStatus, newStatus);
            agentList.at(myPosition)->setLastStatusChange(curDateTime.toTime_t());
        }
    }
}

/** refreshAgentList - Calls the AsteriskManager QUEUESTATUS to get the
  * list of queue agents and their status.  It only sends it, it doesn't
  * wait.  The events from the response will automatically cause the
  * refreshes.
  */
void AgentStatus::refreshAgentList()
{
    am->simpleCommand(ASTCMD_QUEUESTATUS);
    //emit(sendAsteriskCommand(ASTCMD_QUEUESTATUS));
}

/** agentSetStatus - Given the Extension some number over 1000 we will
  * attempt to set the status of the agent.
  */
void AgentStatus::agentSetStatus(int ext)
{
    int     state  = 0;
    //fprintf(stderr, "AgentStatus::agentSetStatus(%d) called.\n", ext);
    while (ext > 10000) {
        state++;
        ext = ext - 10000;
    }
    // Get the queue name out of the database.

    ADB     db;
    char    queue[1024];
    strcpy(queue, "");

    db.query("select QueueName from Staff where AgentID = %d", ext);
    if (db.rowCount) {
        db.getrow();
        strcpy(queue, db.curRow["QueueName"]);
    }

    am->setAgentStatus(ext, ext, state, queue);
}

/** updateTimeInState - Refreshes the time in state for each of the agents.
  */
void AgentStatus::updateTimeInState()
{
    QDateTime   startTime;
    QDateTime   curTime = QDateTime::currentDateTime();
    char        tmpSt[1024];
    int         h, m, s, secs;
    for (int i = 0; i < agentCount; i++) {
        AgentInfo *tmpAgent = agentList.at(i);
        startTime.setTime_t(tmpAgent->getLastStatusChange());
        h = m = s = secs = 0;
        secs = startTime.secsTo(curTime);

        while (secs >= 3600) {
            h++;
            secs = secs - 3600;
        }
        while (secs >= 60) {
            m++;
            secs = secs - 60;
        }
        s = secs;
        sprintf(tmpSt, "%d:%02d:%02d", h, m, s);
        tmpAgent->setTimeInState(h, m, s);
    }
}

