/** QueueMonitor - Monitors the Blarg Sales/Support queue(s)
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
#include <qregexp.h>
#include <qdatetime.h>
#include <qtimer.h>

#include "ColorListViewItem.h"

#include "QueueMonitor.h"

#include <TAAWidget.h>
#include <TAATools.h>
#include <ADB.h>

QueueMonitor::QueueMonitor
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Queue Monitor" );

    // Create the widgets
    callq = new QListView(this, "Queue");
    callq->setAllColumnsShowFocus(true);
    callq->setMinimumHeight(75);
    callq->addColumn("Channel");
    callq->addColumn("State");
    callq->addColumn("Caller ID");
    callq->addColumn("Queue");
    callq->addColumn("Position");
    callq->addColumn("Location");
    callq->addColumn("Agent");
    callq->addColumn("Time");
    callq->setMinimumHeight(100);
    connect(callq, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(callDoubleClicked(QListViewItem *)));

    callq->setColumnAlignment((QMColumnNames)Position, AlignRight);

    // Attach a context menu to the queue list.
    menu = new QPopupMenu(callq);
    connect(menu, SIGNAL(activated(int)), this, SLOT(menuItemSelected(int)));
        
    connect(callq, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint& , int ) ),
                        this, SLOT( callPopupMenu( QListViewItem *, const QPoint &, int ) ) );

    // The main layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 0, 0);
    ml->addWidget(callq, 1);

    // Create a timer to update the "time" column.
    QTimer  *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(refreshTimers()));
    t->start(1000, false);

    // Done.
}

QueueMonitor::~QueueMonitor()
{
}

/** asteriskEvent - This is a slot that should be connected to the
  * AsteriskManager.  It will take an event from the system and
  * update the queue accordingly.
  */
void QueueMonitor::asteriskEvent(const astEventRecord event)
{
    char    callqueue[ASTMAXVALLENGTH];
    char    pos[ASTMAXVALLENGTH];
    char    cid[ASTMAXVALLENGTH];
    char    state[ASTMAXVALLENGTH];
    char    chan[ASTMAXVALLENGTH];
    char    member[ASTMAXVALLENGTH];
    char    agent[ASTMAXVALLENGTH];
    char    tmpStr[2048];
    strcpy(callqueue,   "");
    strcpy(pos,         "");
    strcpy(state,       "");
    strcpy(cid,         "");
    strcpy(chan,        "");
    strcpy(member,      "");
    strcpy(agent,       "");
    QColor  queued_rgb(153,204,204);
    QColor  connect_rgb(200,200,200);
    QColor  newcall_rgb(128,128,128);
    /*if (!strcasecmp(event.msgVal, "NewChannel")) {
        // A new incoming call is being made
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "CallerID")) strcpy(cid, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel")) strcpy(chan, event.data[i].val);
        }
        QRegExp localexp("^Local/.*");
        //QRegExp agentexp("^Agent/.*");
        if (localexp.search(chan) < 0) { // && agentexp.search(chan) < 0) {
            removeChannel(chan);
            QListViewItem *newItem = new ColorListViewItem(callq, newcall_rgb, chan, state, cid, callqueue, pos, member, agent);
        }
    } else if (!strcasecmp(event.msgVal, "Newstate")) {
        // A channel has changed its status
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "State")) strcpy(state, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallerID")) strcpy(cid, event.data[i].val);
        }
        setColumnText(chan, ChanState, state);
        setColumnText(chan, CallerID,  cid);
    } else if (!strcasecmp(event.msgVal, "Newexten")) {
        // A channel has changed its extension
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Application")) strcpy(state, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallerID")) strcpy(cid, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "AppData")) strcpy(callqueue, event.data[i].val);
        }
        setColumnText(chan, ChanState, state);
        setColumnText(chan, CallerID,  cid);
        setColumnText(chan, QueueName, callqueue);
    } else */ if (!strcasecmp(event.msgVal, "join")) {
        // Someone is joining the queue.
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "callerid")) strcpy(cid, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "channel")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "position")) strcpy(pos, event.data[i].val);
        }
        removeChannel(chan);
        debug(2, "QueueMonitor::asteriskEvent('%s') - Adding channel for queue '%s', channel '%s', position '%s', member '%s', agent '%s'\n", event.msgVal, callqueue, chan, pos, member, agent);
        QListViewItem *newItem = new ColorListViewItem(callq, queued_rgb, chan, state, cid, callqueue, pos, member, agent, "0:00");
        sprintf(tmpStr, "%ld", QDateTime::currentDateTime().toTime_t());
        newItem->setText((QMColumnNames)TimerHidden, tmpStr);
    } else if (!strcasecmp(event.msgVal, "leave")) {
        // Someone is leaving the queue.
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "channel")) strcpy(chan, event.data[i].val);
        }
        // Now, remove the channel we found from the list.
        if (strlen(chan)) {
            debug(2, "QueueMonitor::asteriskEvent('%s') - Removing channel for queue '%s', channel '%s'\n", event.msgVal, callqueue, chan);
            removeChannel(chan);
        }
    } else if (!strcasecmp(event.msgVal, "AgentCalled")) {
        // An agent is being offered a queue call
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "ChannelCalling")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "AgentCalled")) strcpy(member, event.data[i].val);
        }
        setColumnText(chan, ChanState, "Offering");
        setColumnText(chan, Location, member);
    } else if (!strcasecmp(event.msgVal, "AgentConnect")) {
        // A call is being linked to an agent.
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "channel")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "member")) strcpy(member, event.data[i].val);
        }
        strcpy(state, "Answered");
        // Get the name of the agent.
        ADB db;
        db.query("select * from Staff where Active > 0 and AgentID = '%s'", member);
        if (db.rowCount) while (db.getrow()) {
            strcpy(agent, db.curRow["LoginID"]);
        }

        removeChannel(chan);
        debug(2, "QueueMonitor::asteriskEvent('%s') - Adding channel for queue '%s', channel '%s', position '%s', member '%s', agent '%s'\n", event.msgVal, callqueue, chan, pos, member, agent);
        QListViewItem *newItem = new ColorListViewItem(callq, connect_rgb, chan, state, cid, callqueue, pos, member, agent, "0:00");
        sprintf(tmpStr, "%ld", QDateTime::currentDateTime().toTime_t());
        newItem->setText((QMColumnNames)TimerHidden, tmpStr);
    } else if (!strcasecmp(event.msgVal, "AgentComplete")) {
        // A queue call is being ended.  Find it in the list and remove it.
        // Walk through the event records first
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "channel")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "member")) strcpy(member, event.data[i].val);
        }
        // Now, remove the channel we found from the list.
        if (strlen(chan)) {
            debug(2, "QueueMonitor::asteriskEvent('%s') - Removing channel for queue '%s', channel '%s', member '%s'\n", event.msgVal, callqueue, chan, member);
            removeChannel(chan);
        }
    } else if (!strcasecmp(event.msgVal, "Unlink")) {
        // A queue call is being ended.  Find it in the list and remove it.
        // Walk through the event records first
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel1")) removeChannel(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel2")) removeChannel(event.data[i].val);
        }
    } else if (!strcasecmp(event.msgVal, "Link")) {
        // A call is being connected (Linked) see if it is one of the calls
        // in our list.  If it is, update a couple of the columns.
        // Walk through the event records first
        // The agent channel will be Channel2
        char    chan1[ASTMAXVALLENGTH];
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "queue")) strcpy(callqueue, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel1")) strcpy(chan1, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel2")) strcpy(chan, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "CallerID1")) strcpy(cid, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "member")) strcpy(member, event.data[i].val);
        }
        // Now walk through the list and remove the one we're looking at.
        QListViewItem           *curItem = findChannel(chan);
        if (curItem) {
            setColumnText(chan, CallerID, cid);
            setColumnText(chan, Channel, chan1);
        }
    } else if (!strcasecmp(event.msgVal, "Hold")) {
        // Someone is putting someone else on hold.
        // Walk through the event records first
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel")) setColumnText(event.data[i].val, ChanState, "On Hold");
        }
    } else if (!strcasecmp(event.msgVal, "Unhold")) {
        // Someone is putting someone else on hold.
        // Walk through the event records first
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel")) setColumnText(event.data[i].val, ChanState, "Answered");
        }
    } else if (!strcasecmp(event.msgVal, "Hangup")) {
        // A queue call is being ended.  Find it in the list and remove it.
        // Walk through the event records first
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Channel")) removeChannel(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel1")) removeChannel(event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Channel2")) removeChannel(event.data[i].val);
        }
    } else if (!strcasecmp(event.msgVal, "Rename")) {
        // A queue call is being ended.  Find it in the list and remove it.
        // Walk through the event records first
        char    oldname[ASTMAXVALLENGTH];
        char    newname[ASTMAXVALLENGTH];
        strcpy(oldname, "");
        strcpy(newname, "");
        for (int i = 0; i < event.count; i++) {
            if (!strcasecmp(event.data[i].key, "Oldname")) strcpy(oldname, event.data[i].val);
            else if (!strcasecmp(event.data[i].key, "Newname")) strcpy(newname, event.data[i].val);
        }
        QListViewItem   *tmpItem = findChannel(oldname);
        if (tmpItem) setColumnText(oldname, Channel, newname);
    } else {
        debug(2, "QueueMonitor::asteriskEvent('%s') Ignoring Event:\n", event.msgVal);
        for (int i = 0; i < event.count; i++) {
            debug(2, "\t%s: %s\n", event.data[i].key, event.data[i].val);
        }
    }
    /*
    for (int i = 0; i < event.count; i++) {
        fprintf(stderr, "QueueMonitor::asteriskEvent() - Key: '%s', Val: '%s'\n",
                event.data[i].key, event.data[i].val);
    }
    */
}

/** findChannel - Given a channel name, this returns a pointer to
  * its entry in the list, or NULL if its not found.
  */
QListViewItem *QueueMonitor::findChannel(const char *chan)
{
    QListViewItemIterator   it(callq);
    QListViewItem           *curItem;
    QListViewItem           *retVal = NULL;
    for (; it.current(); ++it) {
        curItem = it.current();
        if (!strcasecmp(curItem->key((QMColumnNames) Channel, 0), chan)) {
            // Found it.  Remove it from the list.
            retVal = curItem;
        }
    }
    return retVal;
}


/** removeChannel - Removes the specified channel from our callq.
  */
void QueueMonitor::removeChannel(const char *chan)
{
    QListViewItem           *curItem;
    curItem = findChannel(chan);
    while (curItem) {
        debug(2, "QueueMonitor::removeChannel() - Removing Channel:%s State:%s CID: %s Queue:%s Position:%s Location:%s Agent:%s\n",
                (const char *)curItem->text(0),(const char *)curItem->text(1),(const char *)curItem->text(2),(const char *)curItem->text(3),(const char *)curItem->text(4),(const char *)curItem->text(5),(const char *)curItem->text(6));
        callq->removeItem(curItem);
        curItem = findChannel(chan);
    }
}

/** setColumnText - Sets the text in the speicified column for the specified
  * channel.
  */
void QueueMonitor::setColumnText(const char *chan, QMColumnNames col, const char *newText)
{
    QListViewItem           *curItem;
    curItem = findChannel(chan);
    if (curItem) curItem->setText(col, newText);
}

/** callPopupMenu - Gets called when the user right mouse clicks
  * on a list item.  This adjusts the menu items based on the
  * item passed in then pops up so the user can select an action.
  */
void QueueMonitor::callPopupMenu( QListViewItem* item, const QPoint & point, int )
{
    int i;
    if(!item ) return;

    ADB     db;

    
    menu->clear();
    i = menu->insertItem(item->key((QMColumnNames) Channel, 0));
    menu->setItemEnabled(i, false);
    menu->insertSeparator();

    // Give them the ability to park.
    i = menu->insertItem("Park Call");
    if (item->key((QMColumnNames) Location,0).isEmpty()) menu->setItemEnabled(i, false);

    // Load the list of people we can transfer to.
    db.query("select * from Staff where Active > 0 and Extension <> '' order by LoginID");
    QPopupMenu *xferTargets = new QPopupMenu(menu);
    QPopupMenu *vmTargets = new QPopupMenu(menu);
    if (db.rowCount) while (db.getrow()) {
        i = xferTargets->insertItem(db.curRow["LoginID"], atoi(db.curRow["Extension"]));
        xferTargets->connectItem(i, this, SLOT(menuItemSelected(int)));
        xferTargets->setItemParameter(i, i);
        /*
        if (!strcasecmp(curUser().userName, db.curRow["LoginID"])) {
            // We can't transfer to ourselves.
            xferTargets->setItemEnabled(i, false);
        }
        */
        // Now for voice mail targets
        i = vmTargets->insertItem(db.curRow["LoginID"], atoi(db.curRow["Extension"]));
        vmTargets->connectItem(i, this, SLOT(menuItemSelected(int)));
        vmTargets->setItemParameter(i, i+1000);
    }
    i = menu->insertItem("Transfer Call", xferTargets);
    i = menu->insertItem("Send to Voice Mail", vmTargets);
    menu->setItemEnabled(i, true);
    // Check to see if we own this call.
    /*
    if (!strcasecmp(curUser().location, item->key((QMColumnNames) Location,0)) || curUser().accessLevel == Admin || curUser().accessLevel == Manager) {
        // We own it or we're a supervisor, allow the transfer.
        // But only if it is a connected call.
        if (!item->key((QMColumnNames)Location,0).isEmpty()) menu->setItemEnabled(i, true);
    }
    */

    menu->popup( point );
}

/** callDoubleClicked - When the user double clicks on a call in the callq
  * list, this gets called.  We extract the phone number and emit it in
  * a signal which will later be connected to the customer window.
  */
void QueueMonitor::callDoubleClicked(QListViewItem *item)
{
    if (!item) return;
    //fprintf(stderr, "Emitting phoneNumberSelected(%s)...\n", (const char *)item->key(2,0));
    emit(phoneNumberSelected((const char *)item->key((QMColumnNames)CallerID,0)));
}

/** menuItemSelected - Gets called by the popup menu when the user selects
  * an option from it.
  */
void QueueMonitor::menuItemSelected(int menuID)
{
    //fprintf(stderr, "menuItemSelected(%d) called...\n", menuID);
    // First, check for a transfer.
    if (menuID >= 100 && menuID <= 199) {
        // Its a transfer and the extension is the menu ID
        QListViewItem *curItem = callq->currentItem();
        if (curItem) {
            char    extStr[2048];
            sprintf(extStr, "%d", menuID);
            emit(transferCall(curItem->key((QMColumnNames)Channel, 0), extStr, "default", 1));
        }
    }
    if (menuID >= 1100 && menuID <= 1199) {
        // Its a transfer and the extension is the menu ID
        QListViewItem *curItem = callq->currentItem();
        if (curItem) {
            char    extStr[2048];
            sprintf(extStr, "*%d", menuID-1000);
            emit(transferCall(curItem->key((QMColumnNames)Channel, 0), extStr, "default", 1));
        }
    }
}

/** refreshTimers - Walks through the items in the callq list and updates the timers.
  * Gets called automatically once per second.
  */
void QueueMonitor::refreshTimers()
{
    if (!callq->childCount()) return;
    QListViewItemIterator   it(callq);
    QListViewItem           *curItem;
    QDateTime               tmpTime = QDateTime::currentDateTime();
    QDateTime               startTime;
    char                    tmpSt[1024];
    int                     h, m, s, secs;
    uint                    utime;
    for (; it.current(); ++it) {
        curItem = it.current();
        utime = atoi(curItem->key((QMColumnNames) TimerHidden, 0));
        h = m = s = secs = 0;
        startTime.setTime_t(utime);
        secs = startTime.secsTo(tmpTime);
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
        curItem->setText((QMColumnNames) Timer, tmpSt);
    }
}
