/*
** $Id: ADB.cpp,v 1.19 2002/09/29 18:43:47 marc Exp $
**
**  ADB.cpp - Generic Database Routines for queries containing specific
**            columns or joins.
**
*****************************************************************************
**
**  (C)opyright 1998, 1999 by R. Marc Lewis and Blarg! Online Services, Inc.
**  All Rights Reserved
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
**
*****************************************************************************
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <ADB.h>
#include <mysql/mysql.h>
#include <syslog.h>
#ifdef USEDES
#include "bdes.h"
#endif

// The global database connection information.  Used by all classes if
// the information is not specified in the class declaration.
static  char    *OGHost  = NULL;
static  char    *OGDBase = NULL;
static  char    *OGUser  = NULL;
static  char    *OGPass  = NULL;
static  int     ADBDebug = 0;
static  int     ADBRetry = 3;
static  bool    ADBUseSyslog = true;
static  bool    ADBUseStdErr = true;
static  bool    ADBLogUpdates = false;
static  bool    ADBEmptyDatesAsNULL = false;


/*
** ADBLogMsg    - Logs a message from one of the database modules.
**                Will determine where to send the message and then send it.
*/

void ADBLogMsg(int priority, const char *format, ... )
{
    // Make the query string from the variable arguments...
    va_list ap;
    va_start(ap, format);
    char    *outstr = new char[265536];
    vsprintf(outstr, format, ap);

    if (ADBUseSyslog) {
        syslog(priority, "%s", outstr);
    }
    
    if (ADBUseStdErr) {
        fprintf(stderr, "%s\n", outstr);
        fflush(stderr);
    }

    delete outstr;
}

/*
** ADBDebugMsg - Logs a message from one of the database modules.
**               Will determine where to send the message and then send it.
*/

void ADBDebugMsg(int level, const char *format, ... )
{
    // Make the query string from the variable arguments...
    va_list ap;
    va_start(ap, format);
    char    *outstr = new char[265536];
    vsprintf(outstr, format, ap);

    if (ADBDebug >= level) {
        ADBLogMsg(LOG_DEBUG, outstr);
    }

    delete outstr;
}


ADB::ADB(
  const char *Name,
  const char *User,
  const char *Pass,
  const char *Host
)
{
    
    // Set the debug level
    debugLevel = ADBDebug;
    curRow.setDebugLevel(debugLevel);
    curRow.setZeroDatesAsNULL(ADBEmptyDatesAsNULL);
    
    // Setup the DBHost value, based on passed in arguments or global settings.
    if (Host == NULL) {
        if (OGHost == NULL) {
            ADBLogMsg(LOG_CRIT, "ADB::ADB() - No database host specified!");
            exit(-1);
        } else {
            DBHost  = new char[strlen(OGHost)+2];
            strcpy(DBHost, OGHost);
        }
    } else {
        DBHost  = new char[strlen(Host)+2];
        strcpy(DBHost, Host);
    }

    // Setup the DBName value, based on passed in arguments or global settings.
    if (Name == NULL) {
        if (OGDBase == NULL) {
            ADBLogMsg(LOG_CRIT, "ADB::ADB() - No database name specified!");
            exit(-1);
        } else {
            DBName = new char[strlen(OGDBase)+2];
            strcpy(DBName, OGDBase);
        }
    } else {
        DBName = new char[strlen(Name)+2];
        strcpy(DBName, Name);
    }

    // Setup the DBUser value, based on passed in arguments or global settings.
    if (User == NULL) {
        if (OGUser == NULL) {
            ADBLogMsg(LOG_CRIT, "ADB::ADB() - No user name specified!");
            exit(-1);
        } else {
            DBUser = new char[strlen(OGUser)+2];
            strcpy(DBUser, OGUser);
        }
    } else {
        DBUser = new char[strlen(User)+2];
        strcpy(DBUser, User);
    }

    // Setup the DBUser value, based on passed in arguments or global settings.
    if (Pass == NULL) {
        if (OGPass == NULL) {
            ADBLogMsg(LOG_ERR, "ADB::ADB() - No database password specified!");
            // exit(-1);
        } else {
            DBPass = new char[strlen(OGPass)+2];
            strcpy(DBPass, OGPass);
        }
    } else {
        DBPass = new char[strlen(Pass)+2];
        strcpy(DBPass, Pass);
    }
    
    // Initialize the MySQL structure
    mysql_init(&MyConn);

    int tryNo = 0;
    while (tryNo < ADBRetry) {
        connected = 0;
        ADBDebugMsg(1, "ADB: Connecting to %s as %s, pw %s...", DBHost, DBUser, DBPass);
        if (!(MySock = mysql_real_connect(&MyConn, DBHost, DBUser, DBPass, NULL,0,NULL,0))) {
            ADBLogMsg(LOG_ERR, "ADB: Unable to connect to the database server on %s as %s.", DBHost, DBUser);
            // exit(-1);
        } else {
            // Connect to the requested datase now.
            ADBDebugMsg(1, "ADB: Selecting database '%s'...", DBName);
            if (mysql_select_db(MySock, DBName) == -1) {
                ADBLogMsg(LOG_ERR, "ADB: Unable to connect to database '%s'", DBName);
                // exit(-1);
            } else {
                connected = 1;
                tryNo = ADBRetry + 1;
            }
        }
        if (tryNo < ADBRetry) {
            tryNo++;
            sleep(tryNo);   // Sleep for tryNo seconds before trying again
                            // To add a bit of time in case the host thought
                            // we were synflooding, or to give the host we
                            // are on to get past any possible blocked ports.
        }
    }
    
    // connected  = 1;
    // Set our initial pointers to NULL
    queryRes   = NULL;
    
    // Setup our escape string so we can free() it safely.
    escWorkStr = (char *) calloc(16, sizeof(char));

}

// Destrcutor.  Doesn't need to do anything right now, as it should delete
// itself...

ADB::~ADB()
{
    ADBDebugMsg(7, "ADB: Freeing query results...");
    // Free the last query result if it exists...
    if (queryRes != NULL) { 
       mysql_free_result(queryRes);
    }
    
    ADBDebugMsg(7, "ADB: closing MySQL socket...");
	if (connected) {
	    // And disconnect from the database.
    	mysql_close(MySock);
    }
    
    // Free our escape work string.
    ADBDebugMsg(7, "ADB: freeing escWorkStr...");
    free(escWorkStr);
}

const char *ADB::defaultHost()
{
    if (!OGHost) return "";
    else return OGHost;
}

/*
** setDefaultHost - If the user doesn't specify a host name when creating
**                  the DB Object, the ADB Global Host will be used instead.
**                  This function sets that host name.
*/

void ADB::setDefaultHost(const char *GlobHost)
{
    if ((GlobHost) && strlen(GlobHost)) {
        if (OGHost != NULL) delete OGHost;
        OGHost = new char[strlen(GlobHost)+2];
        strcpy(OGHost, GlobHost);
    } else {
        ADBLogMsg(LOG_ERR, "ADB::setDefaultHost() - Unable to set default host to NULL");
    }
}

const char *ADB::defaultDBase()
{
    if (!OGDBase) return "";
    else return OGDBase;
}
/*
** setDefaultDBase - If the user doesn't specify a database name when creating
**                   the DB Object, the ADB Global Database will be used instead.
**                   This function sets that database name.
*/

void ADB::setDefaultDBase(const char *GlobDBase)
{
    if ((GlobDBase) && strlen(GlobDBase)) {
        if (OGDBase != NULL) delete OGDBase; 
        OGDBase = new char[strlen(GlobDBase)+2];
        strcpy(OGDBase, GlobDBase);
    } else {
        ADBLogMsg(LOG_ERR, "ADB::setDefaultDBase() - Unable to set default database name to NULL");
    }
}

const char *ADB::defaultUser()
{
    if (!OGUser) return "";
    else return OGUser;
}

/*
** setDefaultUser  - If the user doesn't specify a user name when creating
**                   the DB Object, the ADB Global User will be used instead.
**                   This function sets that user name.
*/

void ADB::setDefaultUser(const char *GlobUser)
{
    if ((GlobUser) && strlen(GlobUser)) {
        if (OGUser != NULL) delete OGUser; 
        OGUser = new char[strlen(GlobUser)+2];
        strcpy(OGUser, GlobUser);
    } else {
        ADBLogMsg(LOG_ERR, "ADB::setDefaultUser() - Unable to set default user name to NULL");
    }
}

const char *ADB::defaultPass()
{
    if (!OGPass) return "";
    else return OGPass;
}

/*
** setDefaultPass  - If the user doesn't specify a password when creating
**                   the DB Object, the ADB Global Password will be used instead.
**                   This function sets that password.
*/

void ADB::setDefaultPass(const char *GlobPass)
{
    if ((GlobPass) && strlen(GlobPass)) {
        if (OGPass != NULL) delete OGPass; 
        OGPass = new char[strlen(GlobPass)+2];
        strcpy(OGPass, GlobPass);
    } else {
        ADBLogMsg(LOG_ERR, "ADB::setDefaultPass() - Unable to set default password to NULL");
    }
}

/*
** setDebugLevel - Sets the debug level for the database
*/

void ADB::setDebugLevel(int newDebugLevel)
{
    ADBDebug = newDebugLevel;
}

/*
** useSyslog  - Determines whether or not we will be logging to syslog.
*/

void ADB::useSyslog(bool newVal)
{
    ADBUseSyslog = newVal;
}

/*
** recordUpdates - Determines whether or not we send a message to syslog when
**                 issuing a command that will modify the database.
*/

void ADB::recordUpdates(bool newVal)
{
    ADBLogUpdates = newVal;
}

/*
** returnEmptyDatesAsNULL - Determines whether we will return '0' dates
**                          as '0000-00-00' or as ''.
*/

void ADB::returnEmptyDatesAsNULL(bool newVal)
{
    ADBEmptyDatesAsNULL = newVal;
}

/*
** Connected   - Returns 0 if we are not connected, 1 if we are.
*/

int ADB::Connected(void)
{
    return connected;
}


// query - Do a query on the database we're connected to.

int ADB::query(const char *format, ... )
{
    // Make the query string from the variable arguments...
    int     retVal = 0;
    va_list ap;
    va_start(ap, format);
    char    *querystr = new char[265536];
    vsprintf(querystr, format, ap);

    // Free the last query result.
    if (queryRes != NULL) { 
        mysql_free_result(queryRes);
    }
    
    // Clear our current row...
    curRow.clearRow();
    
    ADBDebugMsg(2, "ADB: Performing query '%s'", querystr);

    // Do the query.
    if (mysql_query(MySock, querystr) || !(queryRes = mysql_store_result(MySock))) {
        ADBLogMsg(LOG_ERR, "ADB: MySQL error on query.  Query: '%s', Error: '%s'", querystr, mysql_error(MySock));
        // exit(-1);
    } else {
        rowCount  = mysql_num_rows(queryRes);
        ADBDebugMsg(1, "ADB: query returned %ld rows.", rowCount);
        retVal = 1;
    }
    delete querystr;
    return retVal;
}


/*
** sumFloat - Do a query on the database which will return a "SUM()".
**            This is seperate because of the way that the new MySQL
**            versions process them.  If no matching records are found,
**            it returns NULL instead of 0.00.
*/

float ADB::sumFloat(const char *format, ... )
{
    // Make the query string from the variable arguments...
    va_list ap;
    va_start(ap, format);
    char    *querystr = new char[265536];
    vsprintf(querystr, format, ap);

    // Free the last query result.
    if (queryRes != NULL) { 
        mysql_free_result(queryRes);
    }

    // Do the query.
    if (mysql_query(MySock, querystr) || !(queryRes = mysql_store_result(MySock))) {
        ADBLogMsg(LOG_ERR, "ADB: MySQL error on query.  Query: '%s', Error: '%s'", querystr, mysql_error(MySock));
        // exit(-1);
    } else {
        rowCount  = mysql_num_rows(queryRes);
        ADBDebugMsg(1, "ADB: query returned %ld rows.", rowCount);
    }
    delete querystr;
    
    float   RetVal = 0.00;
    getrow();
    if ((curRow[0]) && strlen(curRow[0])) {
        RetVal = atof(curRow[0]);
    }
    
    return RetVal;
}


// getrow - Loads a row from the database into memory.

int ADB::getrow(void)
{
    int RetVal = curRow.loadRow(queryRes);
    ADBDebugMsg(1, "ADB::getrow() returning %d", RetVal);
    return RetVal;
}

// dbcmd - Execute a command on the database we're connected to.

long ADB::dbcmd(const char *format, ... )
{
    // Make the query string from the variable arguments...
    va_list ap;
    va_start(ap, format);
    char    *cmdstr = new char[265536];
    vsprintf(cmdstr, format, ap);

    ADBDebugMsg(1, "ADB: command = '%s'", cmdstr);

	long	Ret = 0;

    // Do the command
    if (ADBLogUpdates) syslog(LOG_DEBUG, "ADB::dbcmd[%s]: %s", DBUser, cmdstr);
    mysql_query(MySock, cmdstr);
    Ret = mysql_insert_id(MySock);
    if (Ret < 0) {
        ADBLogMsg(LOG_ERR, "ADB: MySQL error on command.  Command: '%s', Error: '%s'", cmdstr, mysql_error(MySock));
    }
    
    ADBDebugMsg(1, "ADB: command returning value %ld", Ret);
    delete cmdstr;
    return(Ret);
}

/*
** escapeString  - An interface into the MySQL escape_string function.
*/

const char *ADB::escapeString(const char *src, int truncLen)
{
    int tmpLen = 50;
    if ((src) && strlen(src)) tmpLen = strlen(src);
    free(escWorkStr);
    escWorkStr = (char *) calloc((tmpLen+1) * 2, sizeof(char));
    if (truncLen < tmpLen) tmpLen = truncLen;
    if (src) mysql_escape_string(escWorkStr, src, tmpLen);
    return escWorkStr;
}

