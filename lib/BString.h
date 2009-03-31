// $Id: BString.h,v 1.1 2003/12/07 01:47:04 marc Exp $
//
// vendortypes header file
//

#ifndef BSTRING_H
#define BSTRING_H

#include <stddef.h>
#include <stdio.h>

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <Qt3Support/q3strlist.h>

const char * getField(char delim, int fieldNo, char * string);
QString wrapString(uint maxCols, QString string);
void myDatetoHuman(char * retStr, const char * MySQLDate);
void myDatetoQDate(const char * myDate, QDate * newDate);
void myDateStamptoQDateTime(const char * myDate, QDateTime * newDate);
void QDatetomyDate(char * myDate, QDate theDate);
void QDateTimetomyTimeStamp(char * myTimeStamp, QDateTime theDate);
void splitString(const char *src, char delim, Q3StrList &dst, int Fold = 1);
void joinString(Q3StrList *src, char *delim, char *dst);
void pwGen(char *pwstr);

char *word_wrap(char *string, size_t line_len); 
void set_tab_size(size_t size); 
void center(FILE *file, char *string, size_t width); 


// Credit Card Validation Functions

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


int  creditCardValid(int cardType, char *cardNo, char *expDate);

#endif // BSTRING_H
