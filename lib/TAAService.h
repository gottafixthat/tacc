/**
 * TAAService.h - Base class for generic services.  Other servies, i.e.
 * BRASS based, Domain based, etc., will inherit this class.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef TAASERVICE_H
#define TAASERVICE_H

#include <qstring.h>
#include <qptrlist.h>

struct ServiceFlag {
    QString key;
    QString val;
};

typedef QPtrList<ServiceFlag> ServiceFlagList;

class TAAService {

public:
    TAAService();
    virtual ~TAAService();

    virtual void    setServiceID(QString svcID);
    const QString   serviceID();
    virtual void    addFlag(QString k, QString v, bool replace = false);

    void            setServerHostname(QString newHost);
    void            setServerUsername(QString newUser);
    void            setServerPassword(QString newPass);
    void            setServerPort(uint newPort);
    const QString   serverHostname();
    const QString   serverUsername();
    const QString   serverPassword();
    uint            serverPort();

    virtual int     provision();
    const QString   lastStatus();

protected:
    QString         mySvcID;
    ServiceFlagList svcFlags;

    void            setLastStatus(QString stat);
    void            setLastStatus(const char *stat);

private:
    QString         myLastStatus;
    QString         myServerHost;
    QString         myServerUser;
    QString         myServerPass;
    uint            myServerPort;
};


#endif

