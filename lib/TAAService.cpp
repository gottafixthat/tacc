/**
 * TAAService.cpp - Base class for generic services.  Other servies, i.e.
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

#include <TAAService.h>

/**
 * TAAService()
 *
 * Constructor.
 */
TAAService::TAAService()
{
}

/**
 * ~TAAService()
 *
 * Destructor.
 */
TAAService::~TAAService()
{
}


/**
 * provision()
 *
 * Virtual function that will get called when the service needs to be
 * provisioned.  This should be overridden.
 */
int TAAService::provision()
{
    // Return false, because we should be overridden.
    return 0;
}

/**
 * setServiceID()
 *
 * Sets the service ID for this service.
 */
void TAAService::setServiceID(QString svcID)
{
    mySvcID = svcID.ascii();
}

/**
 * serviceID()
 *
 * Returns the service ID.
 */
const QString TAAService::serviceID()
{
    return mySvcID;
}

/**
 * addFlag()
 *
 * Adds a Key->Value pair flag to the list of flags for this service.
 *
 * DUPLICATES ARE ALLOWED!
 */
void TAAService::addFlag(QString k, QString v, bool replace)
{
    ServiceFlag *flag;

    // If replace = true, find any matching flags and delete it/them
    if (replace) {
        for(uint i = 0; i < svcFlags.count(); ++i )
        if (svcFlags.at(i)) {
            if (!k.compare(k, svcFlags.at(i)->key)) {
                svcFlags.remove(i);
            }
        }
    }


    flag = new ServiceFlag;
    flag->key = k;
    flag->val = v;
    svcFlags.append(flag);
}

/**
 * setLastStatus()
 *
 * Sets what will be returned on a lastStatus call.
 */
void TAAService::setLastStatus(QString stat)
{
    myLastStatus = stat.ascii();
}

void TAAService::setLastStatus(const char *stat)
{
    myLastStatus = stat;
}
/**
 * lastStatus()
 *
 * Returns the text from the last status.  This is typically
 * set by the provision() call, and can return any errors
 * or success messages back to the caller.
 */
const QString TAAService::lastStatus()
{
    return myLastStatus;
}

/**
 * setServerHostname()
 *
 * Sets the hostname to connect to when provisioning the service.
 */
void TAAService::setServerHostname(QString newHost)
{
    myServerHost = newHost.ascii();
}

/**
 * setServerUsername()
 *
 * Sets the username to use when provisioning the service.
 */
void TAAService::setServerUsername(QString newUser)
{
    myServerUser = newUser.ascii();
}

/**
 * setServerPassword()
 *
 * Sets the password to use when provisioning the service.
 */
void TAAService::setServerPassword(QString newPass)
{
    myServerPass = newPass.ascii();
}

/**
 * setServerPort()
 *
 * Sets the port to use when provisioning the service.
 */
void TAAService::setServerPort(uint newPort)
{
    myServerPort = newPort;
}

/**
 * serverHostname()
 *
 * Returns the hostname we'll use when provisining the service.
 */
const QString TAAService::serverHostname()
{
    return myServerHost;
}

/**
 * serverUsername()
 *
 * Returns the username we'll use when provisining the service.
 */
const QString TAAService::serverUsername()
{
    return myServerUser;
}

/**
 * serverPassword()
 *
 * Returns the password we'll use when provisining the service.
 */
const QString TAAService::serverPassword()
{
    return myServerPass;
}

/**
 * serverPort()
 *
 * Returns the port we'll use when provisining the service.
 */
uint TAAService::serverPort()
{
    return myServerPort;
}

