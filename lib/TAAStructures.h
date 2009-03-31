/**
 * TAAStructures.h - Structure definitions for the various data components
 * found in TAA.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef TAASTRUCTURES_H
#define TAASTRUCTURES_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QMap>

// Credit card statuses
#define TAACC_UNPROCESSED 0
#define TAACC_SENT        1
#define TAACC_AUTH        2
#define TAACC_DECL        3
#define TAACC_INVALID     4
#define TAACC_EXPIRED     5
#define TAACC_RETRY       6

typedef QMap<QString, QString> QStringMap;

enum customerContactFlags { None = 0, Billing = 1 };

struct customerContactRecord {
    uint        contactID;
    uint        customerID;
    QString     tag;
    QString     name;
    QString     access;
    uint        international;
    QString     phoneNumber;
    QString     emailAddress;
    uint        active;
    uint        flags;
    uint        sendStatements;
    QStringList flagList;           // Read-only, set by datbase
    QString     lastModifiedBy;
    QDateTime   lastModified;
};

typedef QList<customerContactRecord> customerContactList;

// Chart of accounts
struct glAccountRecord {
    int     intAccountNo;
    QString accountNo;
    QString acctName;
    int     parentID;
    int     accountType;
    QString accountTypeName;
    QString providerAccountNo;
    QString taxLine;
    double  balance;
    ulong   transCount;
};

#endif
