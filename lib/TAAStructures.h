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

#include <qstring.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qlist.h>

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

#endif
