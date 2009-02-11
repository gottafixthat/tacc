/***************************************************************************
** seanetimport.h - Structure and function prototypes
****************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
****************************************************************************/

#ifndef SEANETIMPORT_H
#define SEANETIMPORT_H

#include <qstring.h>
#include <qptrlist.h>

#define STARTING_CUSTOMERID 100000

struct domainRecord {
    QString regNumber;
    int     billingPeriod;
    int     planStatus;
    QString serviceStart;
    QString nextBillDate;
    QString domainName;
    int     spamFilter;
    QString mailType;
};

struct dialupRecord {
    QString regNumber;
    int     billingPeriod;
    int     planStatus;
    QString serviceStart;
    QString nextBillDate;
    QString serviceType;
    QString userName;
    QString password;
    QString virtDomain;
    int     autoAssign;
    QString dateAssigned;
    QString netmask;
    QString ipAddr;
    QString mailType;
    int     foundMatch;
};

struct serviceRecord {
    char    loginID[64];
    long    loginTypeID;
    QString loginType;
    QString loginTypeDesc;
    int     foundMatch;
    QString endsOn;
    QString closeDate;
    QString userName;
    QString password;
    QString virtDomain;
    int     autoAssign;
    QString dateAssigned;
    QString netmask;
    QString ipAddr;
    QString mailType;
};

struct billableRecord {
    long    billableItemID;
    QString endsOn;
    QString closeDate;
};


struct customerRecord {
    QString regNumber;
    uint    customerID;
    QString fullName;
    QString contactName;
    QString altContact;
    QString email;
    float   currentBalance;
    QString accountExpires;
    QString street;
    QString city;
    QString state;
    QString zip;
    QString country;
    QString homePhone;
    QString workPhone;
    QString faxPhone;
    int     statementType;
    int     paymentType;
    QString creditCard;
    int     cardExpMonth;
    int     cardExpYear;
    QString password;
    QPtrList<serviceRecord> svcList;
    QPtrList<billableRecord> billableList;
    QPtrList<dialupRecord> dialupList;
};

extern QPtrList<customerRecord> customerList;
extern QPtrList<domainRecord> domainListFull;
extern QPtrList<dialupRecord> dialupListFull;

int main( int argc, char ** argv );
void csvImport(const char *, const char *);
void loadFlags();
void loadDomains();
void loadDialupStatic();
void loadDialupDynamic();
void importLoginTypes();
void loadCustomers();
void importCustomers();
void matchDialup(serviceRecord *, dialupRecord *);
void saveCustomer(customerRecord *);

#endif

