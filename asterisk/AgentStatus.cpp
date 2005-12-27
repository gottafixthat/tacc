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
#include <qsqldatabase.h>
#include <qpixmap.h>
#include <qtimer.h>

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
    query.exec("select LoginID, AgentID from Staff where Active > 0 and AgentID <> ''");
    if (query.size() < 1) {
        fprintf(stderr, "AgentStatus::AgentStatus() No agents found!\n");
        return;
    }
    
    agentCount = query.size();
    // Create our widgets.
    QPixmap signedout(signedout_xpm);
    QPixmap avail(available_xpm);
    QPixmap onbreak(onbreak_xpm);
    int i = 0;
    while (query.next()) {
        QLabel  *tmpName = new QLabel(this);
        tmpName->setText(query.value(0).toString());
        names.append(tmpName);

        QLabel *tmpLoc = new QLabel(this);
        tmpLoc->setText(query.value(1).toString());
        locations.append(tmpLoc);

        QLabel *tmpGuiStatus = new QLabel(this);
        tmpGuiStatus->setPixmap(signedout);
        guistatus.append(tmpGuiStatus);

        QLabel *tmpStatus = new QLabel(this);
        tmpStatus->setText("Signed Out");
        status.append(tmpStatus);
        
        QLabel *tmpCallsTaken = new QLabel(this);
        tmpCallsTaken->setAlignment(AlignVCenter);
        callstaken.append(tmpCallsTaken);

        QLabel *tmpLastCall = new QLabel(this);
        tmpLastCall->setAlignment(AlignVCenter);
        lastcall.append(tmpLastCall);

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
        wl->addMultiCellWidget(guistatus.at(i), curRow, curRow+1, 0, 0, AlignLeft|AlignVCenter);
        wl->addWidget(names.at(i),          curRow, 1, AlignLeft|AlignTop);
        wl->addWidget(locations.at(i),      curRow, 2, AlignLeft|AlignTop);
        curRow++;
        wl->addMultiCellWidget(status.at(i), curRow,curRow,1,2, AlignLeft|AlignTop);
        curRow++;
        wl->addMultiCellWidget(callstaken.at(i),    curRow, curRow, 1, 2, AlignLeft|AlignTop);
        curRow++;
        wl->addMultiCellWidget(lastcall.at(i),      curRow, curRow, 1, 2, AlignLeft|AlignTop);
        curRow++;
        HorizLine *tmpLine = new HorizLine(this);
        wl->addMultiCellWidget(tmpLine, curRow,curRow,0,2);
        curRow++;
    }
    ml->addLayout(wl, 0);
    ml->addStretch(1);

    setStatusMenu = new QPopupMenu(this);
    setStatusMenu->insertItem(avail, "Available", am, SLOT(signMeIn()));
    setStatusMenu->insertItem(onbreak, "On Break", am, SLOT(goOnBreak()));
    setStatusMenu->insertItem(signedout, "Signed Out", am, SLOT(signMeOut()));
    setStatusButton = new QPushButton(signedout, "Signed Out", this);
    setStatusButton->setPopup(setStatusMenu);
    ml->addWidget(setStatusButton, 0);

    cl->addLayout(ml, 1);

    QTimer::singleShot(1000, this, SLOT(refreshAgentList()));

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
        if (isPaused) {
            newStatus = "On Break";
            newGuiStatus = AGENT_STATUS_ONBREAK;
        } else {
            if (memberStatus > 3 && memberStatus < 6) {
                newGuiStatus = AGENT_STATUS_SIGNEDOUT;
            } else {
                newGuiStatus = AGENT_STATUS_AVAILABLE;
            }
        }
        //newGuiStatus.loadFromData(onbreak_xpm, strlen(onbreak_xpm));
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
        if (!strcasecmp(locations.at(i)->text(), loc)) {
            // Found them.
            status.at(i)->setText(newStatus);
            switch (newGuiStatus) {
                case AGENT_STATUS_AVAILABLE:
                    guistatus.at(i)->setPixmap(avail);
                    break;
                case AGENT_STATUS_ONBREAK:
                    guistatus.at(i)->setPixmap(onbreak);
                    break;
                case AGENT_STATUS_SIGNEDOUT:
                default:
                    guistatus.at(i)->setPixmap(signedout);
                    break;
            }
            if (calls >= 0) {
                sprintf(tmpStr, "Calls Taken: %d", calls);
                callstaken.at(i)->setText(tmpStr);
            }
            if (lastcalltime > 0) {
                QDateTime   tmpTime;
                tmpTime.setTime_t(lastcalltime);
                sprintf(tmpStr, "Last Call: %s", (const char *)tmpTime.toString("h:mm:ssap"));
                lastcall.at(i)->setText(tmpStr);
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
                setStatusButton->setText("Signed Out");
                setStatusButton->setIconSet(signedout);
                break;
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

