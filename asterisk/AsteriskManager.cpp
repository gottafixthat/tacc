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

#include "AsteriskManager.h"
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include "TAATools.h"
#include "Cfg.h"

/** AsteriskManager - Contructor
  */
AsteriskManager::AsteriskManager(QObject * parent, const char * name)
    : QObject(parent, name)
{
    // Initialize our variables
    event.msgType       = Unknown;
    event.responseID    = 0;
    event.count         = 0;
    event.msgVal[0]     = '\0';
    curResponseID       = 0;
    
    mySocket = new Q3Socket(this, "Asterisk Socket Connection");
    connect(mySocket, SIGNAL(connected()),        this, SLOT(authenticate()));
    connect(mySocket, SIGNAL(readyRead()),        this, SLOT(checkManager()));
    connect(mySocket, SIGNAL(connectionClosed()), this, SLOT(disconnected()));
    connect(mySocket, SIGNAL(error(int))        , this, SLOT(socketError(int)));
    reconnect();
}

/** ~AsteriskManager - Destructor
  */
AsteriskManager::~AsteriskManager()
{
}

/** reconnect() - Reconnects to the asterisk server.
  */
void AsteriskManager::reconnect()
{
    //fprintf(stderr, "AsteriskManager::reconnect() - Attempting to reconnect to Asterisk Server...\n");
    debug(5, "AsteriskManager::reconnect() - Attempting reconnect...\n");
    mySocket->connectToHost(cfgVal("AsteriskHost"), atoi(cfgVal("AsteriskPort")));
    // Check after 5 seconds, if we're not connected try again.
    QTimer::singleShot(5000, this, SLOT(checkConnection()));
}

/** disconnected() - This gets called when the socket connection is closed.
  * We create a single shot timer and try reconnecting.
  */
void AsteriskManager::disconnected()
{
    // Try again in one second.
    QTimer::singleShot(1000, this, SLOT(reconnect()));
}

/** error() - Gets called when there is a socket error.
  * We check the state and if we're disconnected or have some
  * other type of error, we try again.
  */
void AsteriskManager::socketError(int err)
{
    // Abort everything and close the connection.
    mySocket->clearPendingData();
    mySocket->close();
    // Reconnect in one second.
    //QTimer::singleShot(1000, this, SLOT(reconnect()));
}

/** checkConnection - Checks to see if we are connected or not, if we're
  * not, close down our attempt and reconnect.
  */
void AsteriskManager::checkConnection()
{
    if (mySocket->state() != Q3Socket::Connected) {
        debug(5, "AsteriskManager::checkConnection() - Not connected.  Will attempt to reconnect...\n");
        // Abort everything and close the connection.
        mySocket->clearPendingData();
        mySocket->close();
        // Reconnect in one second.
        QTimer::singleShot(1000, this, SLOT(reconnect()));
    }
}

/** authenticate() - After we are connected, this authenticates us.
  */
void AsteriskManager::authenticate()
{
    char    authStr[2048];
    debug(5, "AsteriskManager::authenticate() - Connected.  Attempting to authenticate...\n");
    sprintf(authStr, "Action: login\nUsername: %s\nSecret: %s\n\n", cfgVal("AsteriskUser"), cfgVal("AsteriskPass"));
    //fprintf(stderr, "Sending:\n%s", authStr);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** checkManager() - Gets called automatically when the manager
  * sends us some new data.  If we are called and we still have
  * data waiting but not a full line, then we seutp a single shot
  * timer to fetch the rest shortly.
  */
void AsteriskManager::checkManager()
{
    while (mySocket->canReadLine()) {
        QString tmpLine = mySocket->readLine().stripWhiteSpace();
        //fprintf(stderr, "AsteriskManager: read '%s'\n", (const char *) tmpLine);
        // Check for a blank line, this indicates the end of a response.
        if (!tmpLine.length()) {
            QDateTime   tmpTime = QDateTime::currentDateTime();
            //fprintf(stderr, "***** Begin AsteriskManager Event %s *****\n%s\n***** End AsteriskManager Event %s *****\n", (const char *) tmpTime.toString(), (const char *) dataBuffer, (const char *) tmpTime.toString());
            emit (eventReceived((const char *)dataBuffer));
            emit (eventReceived(event));
            clearEvent();
            dataBuffer.truncate(0);
            dataBuffer.squeeze();
        } else {
            if (dataBuffer.length()) dataBuffer += "\n";
            dataBuffer += tmpLine;
            parseLine((const char *)tmpLine);
        }
    }
    if (!mySocket->atEnd()) {
        QTimer::singleShot(100, this, SLOT(checkManager()));
    }
}

/** parseLine - When a completed manager packet comes in, this function
  * parses the line and adds it to our event record.
  */
void AsteriskManager::parseLine(const char *line)
{
    QString     key;
    QString     keylower;
    QString     val;
    QString     tmpLine(line);
    int         delimPos;

    delimPos = tmpLine.find(':');
    if (delimPos < 0) return;       // Not a valid line?
    key = tmpLine.left(delimPos).stripWhiteSpace();
    val = tmpLine.mid(delimPos+1).stripWhiteSpace();
    if (key.isEmpty() || val.isEmpty()) return;     // Missing something.
    keylower = key.lower();

    debug(8, "Key: '%s', Val: '%s'\n", (const char *)key, (const char *)val);
    
    // Check for "Event" or "Response", if it is neither of those, then
    // its extra data that we add to our data list and leave up to any
    // connected classes to worry about.
    key.truncate(ASTMAXKEYLENGTH - 1);
    val.truncate(ASTMAXVALLENGTH - 1);
    if (!keylower.compare("event")) {
        event.msgType = Event;
        strcpy(event.msgVal, (const char *)val);
    } else if (!keylower.compare("response")) {
        event.msgType = Response;
        strcpy(event.msgVal, (const char *)val);
    }

    astKVPair   pair;
    strcpy(pair.key, (const char *)key);
    strcpy(pair.val, (const char *)val);
    event.data.push_back(pair);
    event.count++;
}

/** clearEvent - Clears our event record to prepare for the next event.
  * this gets called right after we emit our eventRecieved signal(s), so
  * if other classes/applications want the event, they need to act on it
  * right then or copy it.
  */
void AsteriskManager::clearEvent()
{
    // Clear them all out
    event.msgType       = Unknown;
    event.msgVal[0]     = '\0';
    event.responseID    = 0;
    event.count         = 0;
    event.data.clear();
}

/** simpleCommand - Sends a simple command (i.e. one that doesn't have any
  * paramaters) to the Asterisk Manager.
  */
void AsteriskManager::simpleCommand(int cmd)
{
    char    cmdStr[2048];
    strcpy(cmdStr, "");

    switch (cmd) {
        case ASTCMD_QUEUESTATUS:
            sprintf(cmdStr, "Action: QueueStatus\n\n");
            break;

        default:
            break;
    }
    if (strlen(cmdStr)) {
        mySocket->writeBlock(cmdStr, strlen(cmdStr));
    }
}

/** goOnBreak() - Sets the currently logged in user to "paused"
  */
void AsteriskManager::goOnBreak()
{
    char    authStr[2048];
    //sprintf(authStr, "Action: QueuePause\nQueue: %s\nInterface: %s\nPaused: true\n\n", curUser().queue, curUser().agentID);
    sprintf(authStr, "Action: QueuePause\nInterface: %s\nPaused: true\n\n", curUser().agentID);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** signMeIn() - Signs the current user in.
  */
void AsteriskManager::signMeIn()
{
    char    authStr[2048];
    //sprintf(authStr, "Action: QueuePause\nQueue: %s\nInterface: %s\nPaused: false\n\n", curUser().queue, curUser().agentID);
    sprintf(authStr, "Action: QueuePause\nInterface: %s\nPaused: false\n\n", curUser().agentID);
    mySocket->writeBlock(authStr, strlen(authStr));
    //sprintf(authStr, "Action: AgentCallbackLogin\nQueue: %s\nAgent: %s\nExten: %s\nPaused: false\nContext: %s\n\n", curUser().queue, curUser().extension, curUser().extension, cfgVal("QueueContext"));
    //sprintf(authStr, "Action: AgentCallbackLogin\nAgent: %s\nExten: %s\nPaused: false\nContext: %s\n\n", curUser().extension, curUser().extension, cfgVal("QueueContext"));
    sprintf(authStr, "Action: AgentCallbackLogin\nAgent: %s\nExten: %s\nPaused: false\n\n", curUser().extension, curUser().extension);
    debug(8, "Sending:\n%s", authStr);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** signMeOut() - Signs the current user in.
  */
void AsteriskManager::signMeOut()
{
    char    authStr[2048];
    //sprintf(authStr, "Action: AgentLogoff\nQueue: %s\nAgent: %s\n\n", curUser().queue, curUser().extension);
    sprintf(authStr, "Action: AgentLogoff\nAgent: %s\n\n", curUser().extension);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** setAgentStatus - Given an extension, an agent ID and a state
  * we will attempt to set the state.
  */
void AsteriskManager::setAgentStatus(int ext, int agent, int state, const char *queue)
{
    char    actStr[2048];

    switch (state) {
        case 1:             // Sign in the agent
            //sprintf(actStr, "Action: AgentCallbackLogin\nQueue: %s\nAgent: %d\nExten: %d\nPaused: false\nContext: %s\n\n", queue, agent, ext, cfgVal("QueueContext"));
            sprintf(actStr, "Action: AgentCallbackLogin\nAgent: %d\nExten: %d\nPaused: false\nContext: %s\n\n", agent, ext, cfgVal("QueueContext"));
    	    debug(8, "Sending:\n%s", actStr);
            mySocket->writeBlock(actStr, strlen(actStr));
            //sprintf(actStr, "Action: QueuePause\nQueue: %s\nInterface: Agent/%d\nPaused: false\n\n", queue, agent);
            sprintf(actStr, "Action: QueuePause\nInterface: Agent/%d\nPaused: false\n\n", agent);
    	    debug(8, "Sending:\n%s", actStr);
            mySocket->writeBlock(actStr, strlen(actStr));
            break;

        case 2:             // Put the agent on break
            //sprintf(actStr, "Action: QueuePause\nQueue: %s\nInterface: Agent/%d\nPaused: true\n\n", queue, agent);
            sprintf(actStr, "Action: QueuePause\nInterface: Agent/%d\nPaused: true\n\n", agent);
            mySocket->writeBlock(actStr, strlen(actStr));
            break;

        case 3:             // Sign the agent out
            //sprintf(actStr, "Action: AgentLogoff\nQueue: %s\nAgent: %d\n\n", queue, agent);
            sprintf(actStr, "Action: AgentLogoff\nAgent: %d\n\n", agent);
            mySocket->writeBlock(actStr, strlen(actStr));
            break;

        default:            // Unknown action
            debug(1, "AsteriskManager::setAgentStatus() unknown state (%d) for extension %d, agent ID %d for queue %s!\n", state, ext, agent, queue);
            break;
    }
    // After we're all done, referesh the queue again.
    //sprintf(actStr, "Action: QueueStatus\nQueue: %s\n\n", "BlargQueue");
    //mySocket->writeBlock(actStr, strlen(actStr));
    QTimer::singleShot(2000, this, SLOT(queueStatus()));
}

/** queueStatus - Calls simpleCommand(ASTCMD_QUEUESTATUS)
  */
void AsteriskManager::queueStatus()
{
    simpleCommand(ASTCMD_QUEUESTATUS);
}

/** transferCall() - Redirects a call from one extension to another.
  */
void AsteriskManager::transferCall(const char *chan, const char *exten, const char *context, int pri)
{
    QDateTime   tmpTime = QDateTime::currentDateTime();
    char    xferStr[2048];
    sprintf(xferStr, "Action: Redirect\nChannel: %s\nContext: %s\nExten: %s\nPriority: %d\n\n", chan, context, exten, pri);
    //fprintf(stderr, "***** Begin AsteriskManager Command %s *****\n%s\n***** End AsteriskManager Command %s *****\n", (const char *) tmpTime.toString(), (const char *) xferStr, (const char *) tmpTime.toString());
    mySocket->writeBlock(xferStr, strlen(xferStr));
}


// vim: expandtab

