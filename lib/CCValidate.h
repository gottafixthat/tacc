/*
** $Id: CCValidate.h,v 1.1.1.1 2001/03/01 01:19:04 marc Exp $
**
***************************************************************************
**
** CCValidate - Credit card validation functions.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CCValidate.h,v $
** Revision 1.1.1.1  2001/03/01 01:19:04  marc
** First checked in version of the BTools library
**
**
*/

#ifndef CCVALIDATE_H
#define CCVALIDATE_H

// Credit card response codes.
#define CCARD_UNK        0          // Unknown error
#define CCARD_OK         1          // All is fine
#define CCARD_INVPRE    -1          // Invalid card number (prefix error)
#define CCARD_INVNUM    -2          // Invalid card number (length)
#define CCARD_INVMOD    -3          // Failed the MOD10 test.
#define CCARD_EXP       -4          // Card has expired
#define CCARD_INVEXP    -5          // Card has an invalid expiration date.

// Credit card types
#define CCTYPE_UNKNOWN  -1          // Don't do specific validation
#define CCTYPE_MC       0           // MasterCard
#define CCTYPE_VISA     1           // Visa
#define CCTYPE_AMEX     2           // American Express
#define CCTYPE_DINERS   3           // Diners Club
#define CCTYPE_DISC     4           // Discover


int  creditCardValid(int cardType, const char *cardNo, const char *expDate);
const char *cardTypeString(int cardType);


#endif  // CCVALIDATE_H

