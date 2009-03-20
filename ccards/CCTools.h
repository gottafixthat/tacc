/**
 * CCTools.h - Some tools for working with credit card data.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CCTOOLS_H
#define CCTOOLS_H

#include <qstring.h>
#include <qptrlist.h>

struct CreditCardRecord {
    int     idx;
    QString cardholder;
    QString addr;
    QString zip;
    int     cardType;
    QString cardTypeStr;
    QString cardNo;
    QString expDate;
    QString ccv;
};

typedef QPtrList<CreditCardRecord> CreditCardList;

CreditCardList getCreditCardList(long);

#endif

