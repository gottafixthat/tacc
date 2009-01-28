/**
 * QSqlDbPool.cpp
 *
 * Class to manage a pool of QSqlDatabase connections and make them
 * reusable without dealing with naming, conflicts or loops with
 * multiple database accesses coliding.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved.
 *
 * Unpublished work.  No portion of this file may be reproduced in whole
 * or in part by any means, electronic or otherwise, without the express
 * written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <QSqlDbPool.h>
#include <qsqldatabase.h>
#include <qptrlist.h>

static QPtrList<sql_Connection_Info> connectionPool;

// Our default/global connection information.
static char *poolDefDriver  = NULL;
static char *poolDefName    = NULL;
static char *poolDefUser    = NULL;
static char *poolDefPass    = NULL;
static char *poolDefHost    = NULL;
static char *poolDefOpts    = NULL;
static int  poolCounter    = 0;
static bool poolCloseOnRelease = true;

QSqlDbPool::QSqlDbPool(
            const char *dbName,
            const char *dbUser,
            const char *dbPass,
            const char *dbHost,
            const char *dbDriver,
            const char *dbOpts
            )
{
    // Set the database name or use the default
    if (dbName) {
        mydbName = new char[strlen(dbName)+2];
        strcpy(mydbName, dbName);
    } else {
        mydbName = new char[strlen(defaultName())+2];
        strcpy(mydbName, defaultName());
    }

    // Set the database user or use the default
    if (dbUser) {
        mydbUser = new char[strlen(dbUser)+2];
        strcpy(mydbUser, dbUser);
    } else {
        mydbUser = new char[strlen(defaultUser())+2];
        strcpy(mydbUser, defaultUser());
    }

    // Set the database password or use the default
    if (dbPass) {
        mydbPass = new char[strlen(dbPass)+2];
        strcpy(mydbPass, dbPass);
    } else {
        mydbPass = new char[strlen(defaultPass())+2];
        strcpy(mydbPass, defaultPass());
    }

    // Set the database host or use the default
    if (dbHost) {
        mydbHost = new char[strlen(dbHost)+2];
        strcpy(mydbHost, dbHost);
    } else {
        mydbHost = new char[strlen(defaultHost())+2];
        strcpy(mydbHost, defaultHost());
    }

    // Set the database host or use the default
    if (dbDriver) {
        mydbDriver = new char[strlen(dbDriver)+2];
        strcpy(mydbDriver, dbDriver);
    } else {
        mydbDriver = new char[strlen(defaultDriver())+2];
        strcpy(mydbDriver, defaultDriver());
    }

    // Set the database host or use the default
    if (dbOpts) {
        mydbOpts = new char[strlen(dbOpts)+2];
        strcpy(mydbOpts, dbOpts);
    } else {
        mydbOpts = new char[strlen(defaultOpts())+2];
        strcpy(mydbOpts, defaultOpts());
    }

    myConnection = NULL;
    myConnectionID = 0;

    createNewConnection();

}

/**
 * ~QSqlDbPool()
 *
 * Destructor.  Removes our database from the use count and optionally closes it.
 */
QSqlDbPool::~QSqlDbPool()
{
    sql_Connection_Info *connInfo;
    for (connInfo = connectionPool.first(); connInfo; connInfo = connectionPool.next()) {
        if (connInfo->connectionID == myConnectionID) {
            // Found it.  Set it to not be in use.
            connInfo->inUse = 0;
            if (poolCloseOnRelease) connInfo->db->close();
        }
    }

    fprintf(stderr, "QSqlDbPool Connection count is now %d/%d\n", activeConnectionCount(), connectionPool.count());
}

/**
 * defaultDriver()
 *
 * Returns the default database driver we will use.
 */
const char *QSqlDbPool::defaultDriver()
{
    if (!poolDefDriver) return "QMYSQL3";
    else return poolDefDriver;
}

/**
 * setDefaultDriver(const char *)
 *
 * Sets the default database host name to connect to.
 */
void QSqlDbPool::setDefaultDriver(const char *newDriver)
{
    if ((newDriver) && strlen(newDriver)) {
        // Make sure the driver they are requesting is available
        if (QSqlDatabase::isDriverAvailable(newDriver)) {
            if (poolDefHost != NULL) delete poolDefHost;
            poolDefDriver = new char[strlen(newDriver)+2];
            strcpy(poolDefDriver, newDriver);
        } else {
            fprintf(stderr, "QSqlDbPool::setDefaultDriver() - Unable to set default driver to '%s'.  Qt reports driver not available.\n", newDriver);
        }
    } else {
        fprintf(stderr, "QSqlDbPool::setDefaultDriver() - Unable to set default driver to NULL\n");
    }
}

/**
 * defaultHost()
 *
 * Returns the default database host name we will use.
 */
const char *QSqlDbPool::defaultHost()
{
    if (!poolDefHost) return "";
    else return poolDefHost;
}

/**
 * setDefaultHost(const char *)
 *
 * Sets the default database host name to connect to.
 */
void QSqlDbPool::setDefaultHost(const char *newHost)
{
    if ((newHost) && strlen(newHost)) {
        if (poolDefHost != NULL) delete poolDefHost;
        poolDefHost = new char[strlen(newHost)+2];
        strcpy(poolDefHost, newHost);
    } else {
        fprintf(stderr, "QSqlDbPool::setDefaultHost() - Unable to set default host to NULL\n");
    }
}

/**
 * defaultName()
 *
 * Returns the default database name we will use.
 */
const char *QSqlDbPool::defaultName()
{
    if (!poolDefName) return "";
    else return poolDefName;
}

/**
 * setDefaultName(const char *)
 *
 * Sets the default database name to connect to.
 */
void QSqlDbPool::setDefaultName(const char *newName)
{
    if ((newName) && strlen(newName)) {
        if (poolDefName != NULL) delete poolDefName;
        poolDefName = new char[strlen(newName)+2];
        strcpy(poolDefName, newName);
    } else {
        fprintf(stderr, "QSqlDbPool::setDefaultName() - Unable to set default database name to NULL\n");
    }
}

/**
 * defaultUser()
 *
 * Returns the default database username we will use.
 */
const char *QSqlDbPool::defaultUser()
{
    if (!poolDefUser) return "";
    else return poolDefUser;
}

/**
 * setDefaultUser(const char *)
 *
 * Sets the default database username to connect to.
 */
void QSqlDbPool::setDefaultUser(const char *newUser)
{
    if ((newUser) && strlen(newUser)) {
        if (poolDefUser != NULL) delete poolDefUser;
        poolDefUser = new char[strlen(newUser)+2];
        strcpy(poolDefUser, newUser);
    } else {
        fprintf(stderr, "QSqlDbPool::setDefaultUser() - Unable to set default user name to NULL\n");
    }
}

/**
 * defaultPass()
 *
 * Returns the default database password we will use.
 */
const char *QSqlDbPool::defaultPass()
{
    if (!poolDefPass) return "";
    else return poolDefPass;
}

/**
 * setDefaultPass(const char *)
 *
 * Sets the default database username to connect to.
 */
void QSqlDbPool::setDefaultPass(const char *newPass)
{
    if (poolDefPass != NULL) delete poolDefPass;
    poolDefPass = new char[strlen(newPass)+2];
    strcpy(poolDefPass, newPass);
}

/**
 * defaultOpts()
 *
 * Returns the default database options we will use.
 */
const char *QSqlDbPool::defaultOpts()
{
    if (!poolDefOpts) return "";
    else return poolDefOpts;
}

/**
 * setDefaultOpts(const char *)
 *
 * Sets the default database options.
 */
void QSqlDbPool::setDefaultOpts(const char *newOpts)
{
    if (poolDefOpts != NULL) delete poolDefOpts;
    poolDefOpts = new char[strlen(newOpts)+2];
    strcpy(poolDefOpts, newOpts);
}

QSqlDatabase *QSqlDbPool::qsqldb()
{
    return myConnection->db;
}

/**
 * activeConnectionCount()
 *
 * Returns the number of QSqlDatabase objects in use.
 */
int QSqlDbPool::activeConnectionCount()
{
    int retVal = 0;

    if (connectionPool.isEmpty()) return 0;

    sql_Connection_Info *connInfo;
    for (connInfo = connectionPool.first(); connInfo; connInfo = connectionPool.next()) {
        if (connInfo->inUse) {
            retVal++;
        }
    }

    return retVal;
}

/**
 * findAvailableConnection()
 *
 * Returns an available connection or NULL if none are
 * available.
 */
sql_Connection_Info *QSqlDbPool::findAvailableConnection()
{
    sql_Connection_Info *retVal = NULL;

    if (connectionPool.isEmpty()) return retVal;

    sql_Connection_Info *connInfo;
    for (connInfo = connectionPool.first(); connInfo; connInfo = connectionPool.next()) {
        if (!connInfo->inUse) {
            // This one is not in use, check to see if the connection
            // info matches the database info we're looking for.
            if (
                    connInfo->db->databaseName() == QString(mydbName) &&
                    connInfo->db->userName() == QString(mydbUser) &&
                    connInfo->db->password() == QString(mydbPass) &&
                    connInfo->db->hostName() == QString(mydbHost) &&
                    connInfo->db->driverName() == QString(mydbDriver)
                ) {
                retVal = connInfo;
                break;
            }
        }
    }
    return retVal;
}

/**
 * createNewConnection()
 *
 * Finds an available database connection or creates a new one.
 * It stores the result in myConnection which can then be returned
 * by qsqldb().
 */
void QSqlDbPool::createNewConnection()
{
    sql_Connection_Info *conn = NULL;

    conn = findAvailableConnection();
    if (!conn) {
        // No available connecitons, create one.
        conn = new sql_Connection_Info;
        conn->inUse = 0;
        poolCounter++;
        char dbName[256];
        sprintf(dbName, "dbpool%06d", poolCounter);
        conn->connectionID = poolCounter;
        conn->db = QSqlDatabase::addDatabase(mydbDriver, dbName);
        conn->db->setHostName(mydbHost);
        conn->db->setDatabaseName(mydbName);
        conn->db->setUserName(mydbUser);
        conn->db->setPassword(mydbPass);
        conn->db->setConnectOptions(mydbOpts);

        connectionPool.append(conn);
    }

    conn->inUse = 1;
    myConnectionID = conn->connectionID;
    if (!conn->db->isOpen()) {
        conn->db->open();
    }

    myConnection = conn;

    fprintf(stderr, "QSqlDbPool Connection count is now %d/%d\n", activeConnectionCount(), connectionPool.count());
}

