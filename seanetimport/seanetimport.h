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
#include <qdatetime.h>
#include <qptrlist.h>

#define STARTING_CUSTOMERID 100000
#define ARACCOUNT    "1110-07-00"
#define SALESACCOUNT "2740-07-00"

struct billingCycleRecord {
    long    cycleID;
    int     period;
};

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

struct loginRecord {
    QString regNumber;
    int     loginTypeID;
    int     planStatus;
    QString serviceStart;
    QString serviceType;
    QString userName;
    QString password;
    QString virtDomain;
    QString virtDomain2;
    QString autoAssign;
    QString dateAssigned;
    QString netmask;
    QString ipAddr;
    QString mailType;
    QString provider;
    QString circuitID;
    QString vpi;
    QString vci;
    QString cpeName;
    QString requirePVC;
    QString nailedType;
    QString routeSwitch;
    QString portDLCI;
    QString spamFilter;
    QString hostingUserName;
    QString webSvcType;
    QString virtUser;
    QString virtDest;
    int     foundMatch;
};

struct billableRecord {
    long    billableItemID;
    QString description;
    QString endsOn;
    int     subscriptionActive;
    QDate   endsOnDate;
    QDate   lastDate;
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
    QDate   accountOpened;
    QDate   nextBillDate;
    int     billingPeriod;
    long    billingCycleID;
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
    QPtrList<billableRecord> billableList;
    QPtrList<loginRecord> loginList;
};

extern QPtrList<customerRecord> customerList;
extern QPtrList<domainRecord> domainListFull;
extern QPtrList<loginRecord> loginListFull;
extern QPtrList<billingCycleRecord> billingCycleList;

int main( int argc, char ** argv );
void csvImport(const char *, const char *, char delim = ',');
void cleanDatabase();
void loadAccountTypes();
void loadChartOfAccounts();
void loadBillingCycles();
long getBillingCycleID(int period);
void loadDomains();
void loadWebSet();
void loadVirtualSet(const char *fName);
void loadDialupStatic();
void loadDialupDynamic();
void loadDSLAccessSet();
void loadNailedSet();
void addLoginType(const char *loginType);
void addLoginTypeFlag(const char *loginType, const char *flagName);
void setLoginFlag(const QString userName, const char *flagName, const QString val);
void importLoginTypes();
void loadCustomers();
void importCustomers();
void saveCustomer(customerRecord *);
void loadCustomerCharges();
void loadCustomerPayments();
const QDate dateConvert(const QString src);

#endif

