/*
** $Id$
**
***************************************************************************
**
** CCValidate.cpp - Credit card validation functions.
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
** $Log: CCValidate.cpp,v $
** Revision 1.1.1.1  2001/03/01 01:19:04  marc
** First checked in version of the BTools library
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
#include <string>

#include "CCValidate.h"

/*
** creditCardValid - Determines if the currently loaded credit card is valid.
**                   Returns one of the CCARD_* response values.
*/

int creditCardValid(int cardType, const char *cardNo, const char *expDate)
{
	int	                RetVal = CCARD_OK;
	string	            tmpStr1;
	string              tmpStr2;
    string::size_type   curPos;
	
	tmpStr2 = cardNo;
    while ((curPos = tmpStr2.find(" ", 0)) != string::npos) {
        tmpStr2.replace(curPos, 1, "");
    }
	tmpStr1 = tmpStr2;
	
	int     tmpMonth = 0;
	int     tmpYear  = 0;
    int     tmpInt;
    int     digitCount = strlen(tmpStr1.c_str());
    time_t  curTime  = time(NULL);
    struct  tm *lTime = localtime(&curTime);
    int     curYear     = lTime->tm_year + 1900;
    int     curMonth    = lTime->tm_mon  + 1;
	
	// First, verify the card type vs. the number
	switch(cardType) {
	    case CCTYPE_UNKNOWN:
	        break;
	        
		case CCTYPE_MC:
			// MasterCard prefixes must be between 51 and 55.
			tmpStr2 = tmpStr1.substr(0,2);
            tmpInt  = atoi(tmpStr2.c_str());
			if (tmpInt < 51 || tmpInt > 55) {
				RetVal = CCARD_INVPRE; 		// Invalid MC number
			} else {
			    // Check the length of the card number.  Must be 16
			    if (digitCount != 16) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_VISA:
			// VISA prefixes must be the number 4
			tmpStr2 = tmpStr1.substr(0,1);
            tmpInt  = atoi(tmpStr2.c_str());
			if (tmpInt != 4) {
				RetVal = CCARD_INVPRE; 		// Invalid VISA number
			} else {
			    // Check the length of the card number.  Must be 13 or 16
			    if (digitCount != 16 && digitCount != 13) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_AMEX:
			// American Express prefixes must be the number 34 or 37
			tmpStr2 = tmpStr1.substr(0,2);
            tmpInt  = atoi(tmpStr2.c_str());
			if (tmpInt != 34 && tmpInt != 37) {
				RetVal = CCARD_INVPRE; 		// Invalid AmEx number
			} else {
			    // Check the length of the card number.  Must be 15
			    if (digitCount != 15) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_DINERS:
			// Diners Club prefixes must be 300-305, 36, or 38
			tmpStr2 = tmpStr1.substr(0,2);
            tmpInt  = atoi(tmpStr2.c_str());
			if (tmpInt != 36 && tmpInt != 38) {
				// Not 36 or 38, try the 300-305 range
				tmpStr2 = tmpStr1.substr(0,3);
                tmpInt  = atoi(tmpStr2.c_str());
				if (tmpInt < 300 && tmpInt > 305) {
					RetVal = CCARD_INVPRE; 		// Invalid AmEx number
				}
			} else {
			    // Check the length of the card number.  Must be 14
			    if (digitCount != 14) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_DISC:
			// Discover card prefixes must be the number 6011
            tmpStr2 = tmpStr1.substr(0,4);
            tmpInt  = atoi(tmpStr2.c_str());
			if (tmpInt != 6011) {
				RetVal = CCARD_INVPRE; 		// Invalid AmEx number
			} else {
			    // Check the length of the card number.  Must be 16
			    if (digitCount != 16) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;

		default:
			RetVal	= 0;
	}	
	
	if (RetVal) {
		// Card seems to pass the prefix test, continue with the MOD10
		// validation of it.
		// Simple formula:
		//    1. Double the value of alternate digits of the account/card
		//       number beginning with the second digit from the right.
		//    2. Add the numbers together and divide by 10.  If there
		//       is no remainder, the card is "good", or at least it passes
		//       the MOD 10 test...
		int total = 0;
		int counter = 0;
		int curnum = 0;
		for (int i = strlen(tmpStr1.c_str()); i > 0; i--) {
			counter++;
			tmpStr2 = tmpStr1.substr(i-1, 1);
			curnum = atoi(tmpStr2.c_str());
			if (!(counter % 2)) {
				curnum += curnum;
				if (curnum > 9) {
					curnum -= 9;
				}
			}
			total += curnum;
		}
		
		if (total % 10) {
			RetVal = CCARD_INVMOD;
		}
	}
	
	// Finally, check the expiration date.  It should be no more than
	// 10 years from now.
	if (RetVal == CCARD_OK) {
		tmpStr1 = expDate;
        while ((curPos = tmpStr1.find(" ", 0)) != string::npos) {
            tmpStr1.replace(curPos, 1, "");
        }
        while ((curPos = tmpStr1.find("/", 0)) != string::npos) {
            tmpStr1.replace(curPos, 1, "");
        }
		if (strlen(tmpStr1.c_str()) == 4) {
			tmpStr2 = tmpStr1.substr(0,2);
			tmpMonth = atoi(tmpStr2.c_str());
			if (tmpMonth < 1 || tmpMonth > 12) {
				// Invalid month.
		    	RetVal = CCARD_INVEXP;
			} else {
				tmpStr2 = tmpStr1.substr(2,2);
				tmpYear = atoi(tmpStr2.c_str());
                if (tmpYear < 100) tmpYear += 2000;
                if (tmpYear < curYear || tmpYear > curYear + 8) RetVal = CCARD_INVEXP;
                /*
				if (tmpYear <= 9) tmpYear += 2000;
				if (tmpYear < 98 || tmpYear > 109) {
					RetVal = CCARD_INVEXP;
				}
                */
			}
		} else {
			// Too long or too short of an expiration date.
			RetVal = CCARD_INVEXP;
		}
		
		// Now, check to make sure that the card hasn't expired.
		if (RetVal == CCARD_OK) {
		    tmpYear += 1900;
		    if (curYear > tmpYear) {
		        // Card has expired already.
		        RetVal = CCARD_EXP;
		    } else if (tmpYear == curYear) {
		        // The card expires this year, check the month.
		        if (tmpMonth < curMonth) {
		            // Card has expired.
		            RetVal = CCARD_EXP;
		        }
		    }
		}
	}
	
	
	return(RetVal);
}

const char *cardTypeString(int cardType)
{
    if (cardType == CCTYPE_MC) return "Mastercard";
    else if (cardType == CCTYPE_VISA) return "Visa";
    else if (cardType == CCTYPE_AMEX) return "American Express";
    else if (cardType == CCTYPE_DISC) return "Discover";
    else if (cardType == CCTYPE_DINERS) return "Diners";
    else return "Unknown";
}

