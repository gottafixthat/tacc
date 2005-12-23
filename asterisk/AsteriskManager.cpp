/** AsteriskManager.h - Provides a class library that will signal events
  * from the Asterisk Manager interface.
  *
  * This file doesn't have any GUI widgets in it at all, it only connects
  * to the Asterisk Manager and emits signals based on incoming events.
  ************************************************************************
  * Written by R. Marc Lewis, 
  *   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
  *   All Rights Reserved.
  *
  *  Unpublished work.  No portion of this file may be reproduced in whole
  *  or in part by any means, electronic or otherwise, without the express
  *  written consent of Blarg! Online Services and R. Marc Lewis.
  */

#include "AsteriskManager.h"
#include <qtimer.h>
#include <qdatetime.h>
#include "TAATools.h"

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
    
    mySocket = new QSocket(this, "Asterisk Socket Connection");
    connect(mySocket, SIGNAL(connected()), this, SLOT(authenticate()));
    mySocket->connectToHost(ASTERISKHOST, ASTERISKPORT);
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(checkManager()));
}

/** ~AsteriskManager - Destructor
  */
AsteriskManager::~AsteriskManager()
{
}

/** authenticate() - After we are connected, this authenticates us.
  */
void AsteriskManager::authenticate()
{
    char    authStr[2048];
    sprintf(authStr, "Action: login\nUsername: %s\nSecret: %s\n\n", ASTERISKUSER, ASTERISKPASS);
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

    //fprintf(stderr, "Key: '%s', Val: '%s'\n", (const char *)key, (const char *)val);
    
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
    sprintf(authStr, "Action: QueuePause\nQueue: %s\nInterface: %s\nPaused: true\n\n", "BlargQueue", curUser().agentID);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** signMeIn() - Signs the current user in.
  */
void AsteriskManager::signMeIn()
{
    char    authStr[2048];
    sprintf(authStr, "Action: QueuePause\nQueue: %s\nInterface: %s\nPaused: false\n\n", "BlargQueue", curUser().agentID);
    mySocket->writeBlock(authStr, strlen(authStr));
    sprintf(authStr, "Action: AgentCallbackLogin\nQueue: %s\nAgent: %s\nExten: %s\nPaused: false\n\n", "BlargQueue", curUser().extension, curUser().extension);
    mySocket->writeBlock(authStr, strlen(authStr));
}

/** signMeOut() - Signs the current user in.
  */
void AsteriskManager::signMeOut()
{
    char    authStr[2048];
    sprintf(authStr, "Action: AgentLogoff\nQueue: %s\nAgent: %s\n\n", "BlargQueue", curUser().extension);
    mySocket->writeBlock(authStr, strlen(authStr));
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

