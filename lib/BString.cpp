/*
** $Id: BString.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** BString  - A set of string functions and utilities.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qregexp.h>
#include <qdatetm.h>
#include <time.h>
#include <ctype.h>
#include <sys/timeb.h>
#include <BString.h>



/*************************************************************************
**
** Some miscellanious tools...
**
*************************************************************************/

/*
 * getField		- 	Given a delimiter, and a string, this will extract
 *					a particular field from it, starting at 1.
 *
 */
 
const char * getField(char delim, int fieldNo, char * string)
{
	QStrList	qstrptr;
	char		retStr[1024];
	
	splitString(string, delim, qstrptr, 0);
	strcpy(retStr, qstrptr.at(fieldNo-1));

	return(retStr);
}

/*
 * wrapString	- 	Given a string, and a maximum width, this function will
 *					do a word-wrap on the string and return a new one.
 *
 */
 
QString wrapString(uint maxCols, QString string)
{

    QString     retQStr;
    char        *workStr = new char[string.length()+2];
    char        *retStr  = new char[string.length()+2];
    strcpy(workStr, string);
    
    strcpy(retStr, word_wrap(workStr, maxCols));
    
    retQStr = retStr;
    return retQStr;
}

/*
** myDatetoHuman - Converts a MySQL timestamp into human readable form.
*/

void myDatetoHuman(char *retStr, const char * MySQLDate)
{
	int		tmptime;
	QString	tmpQstr;
	char	ampm[10];
	
	if (strlen(MySQLDate) != 10) {
		strcpy(retStr, "");
		return;
	}
	tmpQstr = MySQLDate;
	tmptime = atoi((const char *) tmpQstr.mid(8,2));
	if (tmptime > 11) {
		strcpy(ampm, "p");
	    tmptime -= 12;
	} else {
	    strcpy(ampm, "a");
	    if (!tmptime) {
	        tmptime = 12;
	    }
	}
	sprintf(retStr, "%s-%s-%s %2d:%s%s",
	  (const char *) tmpQstr.mid(0,4),
	  (const char *) tmpQstr.mid(4,2),
	  (const char *) tmpQstr.mid(6,2),
	  tmptime,
	  (const char *) tmpQstr.mid(10,2),
	  ampm
	);
}

/*
** myDatetoQDate - Converts a MySQL date into a QDate
*/

void myDatetoQDate(const char *myDate, QDate * newDate)
{
    QString tmpQstr;
    tmpQstr = myDate;

    #ifdef DBDEBUG
    fprintf(stderr, "myDatetoQDate: Source date is %s\n", myDate);
    #endif

    if (strcmp(myDate, "0000-00-00")) {
	    newDate->setYMD(
		  atoi((const char *) tmpQstr.mid(0,4)),
		  atoi((const char *) tmpQstr.mid(5,2)),
		  atoi((const char *) tmpQstr.mid(8,2))
		);
    } else {
        // Default to the current date if we're at zero.
        newDate->setYMD(
          QDate::currentDate().year(),
          QDate::currentDate().month(),
          QDate::currentDate().day()
        );
    }
}

/*
** myDateStamptoQDateTime - Converts a MySQL date into a QDate
*/

void myDateStamptoQDateTime(const char * myDate, QDateTime * newDate)
{
    QString tmpQstr;
    QDate   tmpDate;
    QTime   tmpTime;
    tmpQstr = myDate;
    

    #ifdef DBDEBUG
    fprintf(stderr, "myDatetoQDate: Source date is %s\n", myDate);
    #endif

	tmpDate.setYMD(
	  atoi((const char *) tmpQstr.mid(0,4)),
	  atoi((const char *) tmpQstr.mid(4,2)),
	  atoi((const char *) tmpQstr.mid(6,2))
	);

    tmpTime.setHMS(
	  atoi((const char *) tmpQstr.mid(8,2)),
	  atoi((const char *) tmpQstr.mid(10,2)),
	  atoi((const char *) tmpQstr.mid(12,2))
    );

    newDate->setDate(tmpDate);
    newDate->setTime(tmpTime);

}

/*
** QDatetomyDate - Converts a MySQL date into a QDate
*/

void QDatetomyDate(char *myDate, QDate theDate)
{
    sprintf(myDate, "%04d-%02d-%02d", theDate.year(), theDate.month(), theDate.day());
}

/*
** QDateTimetomyDateStamp - Converts a QDateTime timestamp into a MySQL TimeStamp
*/

void QDateTimetomyTimeStamp(char *myTimeStamp, QDateTime theDate)
{
    QDate   tmpDate;
    QTime   tmpTime;
    
    tmpDate = theDate.date();
    tmpTime = theDate.time();
    
    sprintf(myTimeStamp, "%04d%02d%02d%02d%02d%02d", 
      tmpDate.year(), 
      tmpDate.month(), 
      tmpDate.day(),
      tmpTime.hour(),
      tmpTime.minute(),
      tmpTime.second()
    );
}


/*
** splitString - Splits a string into multiple fields based on a 
**               delimiter
**
** Arguments - src  - char * - The source string to parse
**             delim  - char - The delimiter to split the string with.
**             dst  - QStrList * - The destination array (already
**                    initialized with new().
**             Fold - If True (1), then empty fields will not be
**                    returned.
*/

void splitString(const char *src, char delim, QStrList &dst, int Fold)
{
	char tmpstr[1024];
	int  tmpstrpos = 0;

	// Clear out the array first.
	dst.clear();

	for (unsigned int i = 0; i < strlen(src); i++) {
		if (src[i] == delim) {
			// We have a complete field now, do something with it.
			int AddIt = 1;
			if ((!strlen(tmpstr)) & (Fold)) {
				AddIt = 0;
			}
			if (AddIt) {
				dst.append(tmpstr);
			}
			tmpstrpos = 0;
			tmpstr[tmpstrpos] = '\0';
		} else {
			tmpstr[tmpstrpos++] = src[i];
			tmpstr[tmpstrpos]   = '\0';
		}
	}
	// And don't forget to add the last field, should it exist...
	if (strlen(tmpstr)) {
		dst.append(tmpstr);
	}
}

/*
** joinString - Joins a QStrList into a single string, using the specified
**              delimiter
**
** Arguments - src   - QStrList * - The source string list
**             delim - char * - The null terminated delimiter to use
**             dst   - char * - The destination string
*/

void joinString(QStrList *src, char *delim, char *dst)
{
	QString	tmpqstr;

	for (unsigned int i = 0; i < src->count(); i++) {
		tmpqstr.append(src->at(i));
		if (i < src->count() - 1) {
			tmpqstr.append(delim);
		}
	}
	strcpy(dst, tmpqstr);
}

/*
** pwGen  - Generates an 8 character password and returns it in *pwstr.
*/

void pwGen(char *pwstr)
{
    int num;
    char tmpstr[64];
    char c;
	time_t	ti = time(NULL);
	struct tm * tm = localtime(&ti);
    srand(tm->tm_sec + tm->tm_min * 60 + tm->tm_hour * 3600);

    tmpstr[0] = '\0';
    for (num = 0; num < 8; num++) {
        c = 0;
        while(!c) {
            c = 48+(int) (74.0*rand()/(RAND_MAX+1.0));
            if (c < 48) c = 0;
            if (c > 57 && c < 91) c = 0;
            if (c > 90 && c < 97) c = 0;
            if (c > 122) c = 0;
        }
        tmpstr[num] = c;
        tmpstr[num+1] = '\0';
    }

    strcpy(pwstr, tmpstr);
}



// Credit Card Functions


/*
** creditCardValid - Determines if the currently loaded credit card is valid.
**                   Returns one of the CCARD_* response values.
*/

int creditCardValid(int cardType, char *cardNo, char *expDate)
{
	int	RetVal		= CCARD_OK;
	QString	qstr1;
	QString qstr2;
	
	qstr1 = cardNo;
	qstr2 = qstr1.replace(QRegExp(" "), "");
	qstr1 = qstr2;
	
	int tmpMonth = 0;
	int tmpYear  = 0;
	QDate   theDate = QDate::currentDate();
	
	
	// First, verify the card type vs. the number
	switch(cardType) {
	    case CCTYPE_UNKNOWN:
	        break;
	        
		case CCTYPE_MC:
			// MasterCard prefixes must be between 51 and 55.
			qstr2 = qstr1.left(2);
			if (qstr2.toInt() < 51 || qstr2.toInt() > 55) {
				RetVal = CCARD_INVPRE; 		// Invalid MC number
			} else {
			    // Check the length of the card number.  Must be 16
			    if (qstr1.length() != 16) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_VISA:
			// VISA prefixes must be the number 4
			qstr2 = qstr1.left(1);
			if (qstr2.toInt() != 4) {
				RetVal = CCARD_INVPRE; 		// Invalid VISA number
			} else {
			    // Check the length of the card number.  Must be 13 or 16
			    if (qstr1.length() != 16 && qstr1.length() != 13) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_AMEX:
			// American Express prefixes must be the number 34 or 37
			qstr2 = qstr1.left(2);
			if (qstr2.toInt() != 34 && qstr2.toInt() != 37) {
				RetVal = CCARD_INVPRE; 		// Invalid AmEx number
			} else {
			    // Check the length of the card number.  Must be 15
			    if (qstr1.length() != 15) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_DINERS:
			// Diners Club prefixes must be 300-305, 36, or 38
			qstr2 = qstr1.left(2);
			if (qstr2.toInt() != 36 && qstr2.toInt() != 38) {
				// Not 36 or 38, try the 300-305 range
				qstr2 = qstr1.left(3);
				if (qstr2.toInt() < 300 && qstr2.toInt() > 305) {
					RetVal = CCARD_INVPRE; 		// Invalid AmEx number
				}
			} else {
			    // Check the length of the card number.  Must be 14
			    if (qstr1.length() != 14) {
			    	RetVal = CCARD_INVNUM;
			    }
			}
			break;
		
		case CCTYPE_DISC:
			// Discover card prefixes must be the number 6011
			qstr2 = qstr1.left(4);
			if (qstr2.toInt() != 6011) {
				RetVal = CCARD_INVPRE; 		// Invalid AmEx number
			} else {
			    // Check the length of the card number.  Must be 16
			    if (qstr1.length() != 16) {
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
		for (int i = qstr1.length(); i > 0; i--) {
			counter++;
			qstr2 = qstr1.mid(i-1, 1);
			curnum = qstr2.toInt();
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
		qstr1 = expDate;
		qstr1.replace(QRegExp("/"), "");
		if (qstr1.length() == 4) {
			qstr2 = qstr1.left(2);
			tmpMonth = qstr2.toInt();
			if (tmpMonth < 1 || tmpMonth > 12) {
				// Invalid month.
		    	RetVal = CCARD_INVEXP;
			} else {
				qstr2 = qstr1.right(2);
				tmpYear = qstr2.toInt();
				if (tmpYear < 9) tmpYear += 100;
				if (tmpYear < 98 || tmpYear > 109) {
					RetVal = CCARD_INVEXP;
				}
			}
		} else {
			// Too long or too short of an expiration date.
			RetVal = CCARD_INVEXP;
		}
		
		// Now, check to make sure that the card hasn't expired.
		if (RetVal == CCARD_OK) {
		    tmpYear += 1900;
		    if (theDate.year() > tmpYear) {
		        // Card has expired already.
		        RetVal = CCARD_EXP;
		    } else if (tmpYear == theDate.year()) {
		        // The card expires this year, check the month.
		        if (tmpMonth < theDate.month()) {
		            // Card has expired.
		            RetVal = CCARD_EXP;
		        }
		    }
		}
	}
	
	
	return(RetVal);
}




/* +++Date last modified: 07-Nov-1995 */

/* w_wrap.c */

/*
**  This is an attempt at a useful word-wrap function.  It is given an array
**  of characters ( a string ) and it modifies the string, replacing any
**  new-lines found with spaces and placing new-lines where needed to make
**  lines of the width specified, placing them only where there was previously
**  white-space. ( ie. Words are not split across lines. )  At the present
**  time it is rather stupid. 1) It doesn't know enough to split a line at an
**  existing hyphen.  2) It has no clue about how to hyphenate words.  3) It
**  makes no attempt at dealing intelligently with a singly word longer than
**  the specified line length 4) It does not deal intelligently with multiple
**  spaces new-lines, etc. ( eg. has no clue of paragraph separation, etc. )
**  OTOH, it does deal well with unformatted, left justified text.
**
**  Tabs will be considered the size specified.  Note that word_wrap() does
**  not actually expand tabs.  This is only to inform it of the number of
**  spaces the output device will expand them to, so it will know how much
**  they expand a line.  The only time word_wrap does anything with tabs, is
**  if the tab size is set to zero, in which case each tab is replaced with a
**  single space character.  This often provides the most useful output, since
**  tabs will often be in the wrong places after re-formatting, and is
**  therefor the default.
**
**
**  Publicly available contents:
**
**      char *word_wrap(char *string,long line_len);
**          Does the actual word-wrapping, as described above;
**          Parameters:
**            string:     actual string to work with
**            line_len:   length of lines for output
**          Returns:        pointer to justified string.
**
**          void set_tab_size(int size);
**            Set the number of spaces that tabs will be expanded to on output
**            default tab size is zero. (each tab replaced with a space char )
**            word_wrap does not actually expand tabs.  This only lets it keep
**            track of how many spaces they take up.  If this is set to
**            zero, each tab will be replaced with a single space.
**
**  Other procedures:
**      int get_word(char *string);
**          returns the number of characters in the next word in string,
**          including leading white-space characters.
**
**  This compiles without warnings and runs with the following compilers:
**      MS Quick C 2.51:
**      Borland C++ 2.0:            either as C or C++
**      GNU C++ 1.39, DOS port:     either as C or C++
**  As far as I know, it uses only portable, standard C constructs.  It should
**  compile and run with little or no modification under nearly any C compiler
**  and environment.
**
**
**  This code was written Nov 16, 1991 by Jerry Coffin.
**  It is hereby placed in the public domain, for free use by any and
**  all who wish to do so, for any use, public, private, or commercial.
*/

static int tab_size = 4;                  /* size to consider tabs as */

static size_t get_word(char *string);     /* returns size of next word*/

void set_tab_size(size_t size)
{
      tab_size = size;
}

char *word_wrap(char *string, size_t line_len)
{
      size_t len,                         /* length of current word */
             current_len = 0;             /* current length of line */
      size_t start_line = 0;              /* index of beginning if line */

      while (0 != (len = get_word(&string[current_len + start_line])))
      {
            if (current_len + len < line_len)
                  current_len += len;
            else
            {
                  string[start_line+current_len] = '\n';
                  start_line += current_len + 1;
                  current_len = 0;
            }
      }
      return string;
}

static size_t get_word(char *string)
{
      register int i = 0, word_len = 0;

      if (!string[0])
            return 0;
      while (isspace(string[i]))
      {
            if ('\t' == string[i])
            {
                  if (0 == tab_size)
                        string[i] = ' ';
                  else  word_len += tab_size-1;
            }
            else if ('\n' == string[i])
                  string[i]=' ';
            word_len++;
            i++;
      }
      while (string[i] && !isspace(string[i++]))
            word_len++;
      return word_len;
}

