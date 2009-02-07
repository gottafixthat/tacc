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

typedef struct serviceRecord {
    char    loginID[64];
    long    loginTypeID;
    QString endsOn;
    QString closeDate;
};

typedef struct billableRecord {
    long    billableItemID;
    QString endsOn;
    QString closeDate;
};


typedef struct customerRecord {
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
};


int main( int argc, char ** argv );
void importLoginTypes();
void importCustomers();
void saveCustomer(customerRecord *);

#endif

