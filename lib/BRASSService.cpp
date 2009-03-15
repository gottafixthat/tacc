/**
 * BRASSService.cpp - Base class for BRASS based services.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <Cfg.h>
#include <TAATools.h>

#include <BRASSService.h>

/**
 * BRASSService()
 *
 * Constructor.
 */
BRASSService::BRASSService()
    : TAAService()
{
    // Make sure our client connection is NULL
    BC = NULL;

    // Set our default BRASS credentials.
    setServerHostname(cfgVal("BrassHost"));
    setServerUsername(cfgVal("BrassUser"));
    setServerPassword(cfgVal("BrassPass"));
    QString portStr = cfgVal("BrassPort");
    setServerPort(portStr.toInt());
}

/**
 * ~BRASSService()
 *
 * Destructor.
 */
BRASSService::~BRASSService()
{
    if (BC) {
        delete BC;
        BC = NULL;
    }
}


/**
 * provision()
 *
 * Provision the FLAGS ONLY!
 *
 * Other BRASS functions are done elsewhere.
 */
int BRASSService::provision()
{
    // If we don't have a service ID set, abort.
    if (!serviceID().length()) {
        setLastStatus(QString("Service ID Not Set"));
        return 0;
    }

    // We don't have a brass connection at this point.
    // Create one
    if (!brassConnect()) return 0;

    // Provision the service now.  This is FLAGS ONLY!
    BC->ZapFlags(serviceID().ascii());

    if (svcFlags.count()) {
        ServiceFlag     *flag;
        for (flag = svcFlags.first(); flag; flag = svcFlags.next()) {
            debug(3, "BRASSService::provision() - SetFlag('%s', '%s', '%s')\n", serviceID().ascii(), flag->key.ascii(), flag->val.ascii());
            if (flag->key.length() && flag->val.length()) {
                BC->SetFlag(serviceID().ascii(), flag->key.ascii(), flag->val.ascii());
            }
        }
    }

    return 1;
}

/**
 * loginExists()
 *
 * Returns 1 if serviceID() exists, else 0.
 */
int BRASSService::loginExists()
{
    // If we don't have a service ID set, abort.
    if (!serviceID().length()) {
        setLastStatus(QString("Service ID Not Set"));
        return 0;
    }

    if (!brassConnect()) return 0;

    if (BC->LoginExists(serviceID().ascii())) {
        setLastStatus("Login ID exists.");
        return 1;
    }

    setLastStatus("Login ID not found");
    return 0;
}


/**
 * createLogin()
 *
 * Creates a new username.
 *
 * Returns 1 if successful, 0 if not.
 */
int BRASSService::createLogin(QString loginID)
{
    if (!brassConnect()) return 0;

    if (!BC->ValidateLogin(loginID.ascii())) {
        setLastStatus("Login ID contains invalid characters.");
        return 0;
    }

    if (BC->LoginExists(loginID.ascii())) {
        setLastStatus("Login ID already exists or is invalid.");
        return 0;
    }

    if (BC->AddLogin(loginID.ascii())) {
        setLastStatus("Login created successfully.");
        setServiceID(loginID);
        return 1;
    } else {
        setLastStatus("An unknown error occurred while adding this Login ID.");
    }
    return 0;
}

/**
 * setLoginPassword()
 *
 * Sets the password for the login ID.
 */
int BRASSService::setPassword(QString loginID, QString pass)
{
    if (!brassConnect()) return 0;

    if (!BC->SetPassword(loginID.ascii(), pass.ascii())) {
        setLastStatus("Error changing password.");
        return 0;
    }
    return 1;
}

/**
 * setFingerName()
 *
 * Sets the password for the login ID.
 */
int BRASSService::setFingerName(QString loginID, QString gecos)
{
    if (!brassConnect()) return 0;

    if (!BC->SetFingerName(loginID.ascii(), gecos.ascii())) {
        setLastStatus("Error changing password.");
        return 0;
    }
    return 1;
}


/**
 * brassConnect()
 *
 * Connects and authenticates to the configured BRASS daemon.
 */
int BRASSService::brassConnect()
{
    if (BC) return 1;   // We're already connected.

    BC = new BrassClient();
    // FIXME:  We should set our connection paramaters
    // but this will require modification to the BrassClient class first.
    debug(3, "brassConnect() - Connecting....\n");
    if (!BC->Connect()) {
        debug(3, "brassConnect() - Connection failure....\n");
        QString tmpStr;
        tmpStr = "Error connecting to BRASS server ";
        tmpStr += serverHostname();
        setLastStatus(tmpStr);
        delete BC;
        BC = NULL;
        return 0;
    }

    // Now authenticate.
    debug(3, "brassConnect() - Authenticating....\n");
    if (!BC->Authenticate()) {
        debug(3, "brassConnect() - Authentication failure....\n");
        QString tmpStr;
        tmpStr = "Error authenticating with BRASS server ";
        tmpStr += serverHostname();
        setLastStatus(tmpStr);
        delete BC;
        BC = NULL;
        return 0;
    }

    debug(3, "brassConnect() - Authentication success....\n");
    return 1;
}

