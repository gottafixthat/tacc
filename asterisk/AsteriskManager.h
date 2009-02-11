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

#ifndef ASTERISKMANAGER_H
#define ASTERISKMANAGER_H

// FIXME: These should all come out of a config file.
//#define ASTERISKHOST "asterisk.corp.blarg.net"
//#define ASTERISKPORT 1234
//#define ASTERISKUSER "tacc"
//#define ASTERISKPASS "VKF3i6iz"

// FIXME: This should be in a config file
#define ASTEXT_PARK "700"

#define ASTMAXKEYLENGTH 256
#define ASTMAXVALLENGTH 256

#define ASTCMD_QUEUESTATUS      1

#include <qobject.h>
#include <qstring.h>
#include <qsocket.h>
#include <qvaluevector.h>

// Define the key/value entries
struct astKVPair {
    char        key[ASTMAXKEYLENGTH];
    char        val[ASTMAXVALLENGTH];
};

enum astMsgTypes {Unknown = 0, Event = 1, Response = 2};

// Define the event structure that will be passed in our signal
struct astEventRecord {
    astMsgTypes             msgType;
    char                    msgVal[ASTMAXVALLENGTH];
    int                     responseID;
    int                     count;
    QValueVector<astKVPair> data;
};

class AsteriskManager : public QObject
{
    Q_OBJECT

public:
    AsteriskManager(QObject * parent = 0, const char * name = 0);
    virtual ~AsteriskManager();

signals:
    void eventReceived(const char *);
    void eventReceived(const astEventRecord);

public slots:
    // Commands
    void simpleCommand(int cmd);
    void goOnBreak();
    void signMeIn();
    void signMeOut();
    void setAgentStatus(int ext, int agent, int state, const char *queue);
    void transferCall(const char *chan, const char *exten, const char *context, int pri);
    void queueStatus();

protected slots:
    void reconnect();
    void disconnected();
    void socketError(int);
    void checkConnection();
    void authenticate();
    void checkManager();

    //void socketError();

protected:
    void            parseLine(const char *);
    void            clearEvent();

private:
    int             curResponseID;
    QSocket         *mySocket;
    QString         dataBuffer;
    astEventRecord  event;
};

#endif // ASTERISKMANAGER_H

