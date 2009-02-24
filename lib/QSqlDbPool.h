/**
 * QSqlDbPool.h
 *
 * Class definitions to manage a pool of QSqlDatabase connections and make
 * them reusable without dealing with naming, conflicts or loops with
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

#ifndef QSQLDBPOOL_H
#define QSQLDBPOOL_H

#include <qsqldatabase.h>

struct sql_Connection_Info {
    int             inUse;
    int             connectionID;
    QSqlDatabase    *db;
};

class QSqlDbPool : public QObject
{
    Q_OBJECT

public:
    QSqlDbPool(
            const char *dbName = NULL,
            const char *dbUser = NULL,
            const char *dbPass = NULL,
            const char *dbHost = NULL,
            const char *dbDriver = NULL,
            const char *dbOpts = NULL
            );
    virtual ~QSqlDbPool();

    static const char *defaultDriver();
    static  void       setDefaultDriver(const char *newDriver);
    static const char *defaultHost();
    static  void       setDefaultHost(const char *newHost);
    static const char *defaultName();
    static  void       setDefaultName(const char *newName);
    static const char *defaultUser();
    static  void       setDefaultUser(const char *newUser);
    static const char *defaultPass();
    static  void       setDefaultPass(const char *newPass);
    static const char *defaultOpts();
    static  void       setDefaultOpts(const char *newOpts);

    QSqlDatabase    *qsqldb();

    int              activeConnectionCount();

protected:
    sql_Connection_Info *findAvailableConnection();
    void  createNewConnection();

    char    *mydbName;
    char    *mydbUser;
    char    *mydbPass;
    char    *mydbHost;
    char    *mydbDriver;
    char    *mydbOpts;
    char    *myConnectionName;
    int     myConnectionID;
    sql_Connection_Info *myConnection;

};

#endif
