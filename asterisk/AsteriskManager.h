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

#ifndef ASTERISKMANAGER_H
#define ASTERISKMANAGER_H

// FIXME: This should be in a config file
#define ASTEXT_PARK "700"

#define ASTMAXKEYLENGTH 256
#define ASTMAXVALLENGTH 256

#define ASTCMD_QUEUESTATUS      1

#include <QtCore/QObject>
#include <QtCore/QString>
#include <Qt3Support/q3socket.h>
#include <Qt3Support/q3valuevector.h>

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
    Q3ValueVector<astKVPair> data;
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
    Q3Socket         *mySocket;
    QString         dataBuffer;
    astEventRecord  event;
};

#endif // ASTERISKMANAGER_H


// vim: expandtab

