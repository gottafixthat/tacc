/**
 * LoginService.cpp - Class for handling "Login" types, which are the
 * standard Blarg/Avvanta types.
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
#include <ADB.h>
#include <TAATools.h>

#include <LoginService.h>

/**
 * LoginService()
 *
 * Constructor.
 */
LoginService::LoginService()
    : BRASSService()
{
}

/**
 * ~LoginService()
 *
 * Destructor.
 */
LoginService::~LoginService()
{
}


/**
 * loadFlags()
 *
 * Loads all of the flags from the database for this user
 * into memory.
 */
int LoginService::loadFlags()
{
    if (!serviceID().length()) {
        setLastStatus("ServiceID not set");
        return 0;
    }

    ADB     DB;

    // Clear any existing flags that have been set.
    
    svcFlags.clear();

    // First, get the standard flags for this login type.
    DB.query("select LoginTypeFlags.Tag, LoginTypeFlags.Value from LoginTypeFlags, Logins where LoginTypeFlags.LoginTypeID = Logins.LoginType and Logins.LoginID = '%s'", serviceID().ascii());
    if (DB.rowCount) {
        while(DB.getrow()) {
            addFlag(QString(DB.curRow["Tag"]), QString(DB.curRow["Value"]));
        }
    }

    // Now, grab any from the LoginFlagValues table
    DB.query("select LoginFlag, FlagValue from LoginFlagValues where LoginID = '%s'", serviceID().ascii());
    if (DB.rowCount) {
        while(DB.getrow()) {
            // Set replace = true so we override any standard flag values.
            addFlag(QString(DB.curRow["LoginFlag"]), QString(DB.curRow["FlagValue"]), true);
        }
    }
    return 1;
}


