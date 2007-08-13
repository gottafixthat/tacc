/*
** $Id$
**
***************************************************************************
**
** TAATools - A few global functions to make development of TAA easier.
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
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#include <qapp.h>
#include <qdatetm.h>
#include <qmessagebox.h>
#include <qwidget.h>

#include <hash_map.h>
#include <ADB.h>
#include <Cfg.h>
#include <StrTools.h>

#include "TAATools.h"
#include "version.h"
#include "buildnum.h"
#include "BString.h"

static char* t_MonthNames[] = { "Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug", "Sep", "Oct",
  "Nov", "Dec" 
};

taa_User        myCurrentUser;

QWidget         *intMainWin;

int      debugLevel = 0;

/*
** mainWin      - Returns a pointer to the main widget for the purpose of
**                connecting singals to it to tell it to its message and
**                status bar widgets.
*/

QWidget *mainWin()
{
    return intMainWin;
}

/*
** setMainWin   - Sets the main widget pointer so that other windows that
**                it may not know about can connect signals to it.
**
**                Note:  If this isn't set, really bad things will happen.
*/

void setMainWin(QWidget *newWinPtr)
{
    intMainWin = newWinPtr;
}

/*
** dateValid  - Tests a string to make sure that it is in YYYY-MM-DD format.
*/

bool dateValid(const char *tDate, bool showMessage)
{
    bool    retVal = false;
    int     y = 0, m = 0, d = 0;
    char    workStr[1024];
    char    *part;
    
    strcpy(workStr, tDate);
    
    if (strlen(workStr)) {
        part = strtok(workStr, "-");
        // Get the year.
        if (part && (strlen(part) == 4)) {
            y = atoi(part);
            
            // Get the month.
            part = strtok(NULL, "-");
            if (part) {
                m = atoi(part);
                
                // Finally, get the day
                part = strtok(NULL, "-");
                if (part) {
                    d = atoi(part);
                    
                    // We should have no more parts.
                    part = strtok(NULL, "-");
                    if (!part) {
                        retVal = QDate::isValid(y, m, d);
                    }
                }
            }
        }
    }
    
    if (!retVal && showMessage) {
        QMessageBox::critical(0, "TAA", "Invalid date specified.\n\nValid dates take the form of YYYY-MM-DD.");
    }
    
    return retVal;
}

/*
** myDateToQDate  - Converts a MySQL date in YYYY-MM-DD format to a QDate
**                  object.
*/

void myDateToQDate(const char *srcDate, QDate &dstDate)
{
    bool    isValid = false;
    int     y = 0, m = 0, d = 0;
    char    workStr[1024];
    char    *part;
    
    strcpy(workStr, srcDate);
    
    if (strlen(workStr)) {
        part = strtok(workStr, "-");
        // Get the year.
        if (part && (strlen(part) == 4)) {
            y = atoi(part);
            
            // Get the month.
            part = strtok(NULL, "-");
            if (part) {
                m = atoi(part);
                
                // Finally, get the day
                part = strtok(NULL, "-");
                if (part) {
                    d = atoi(part);
                    
                    // We should have no more parts.
                    part = strtok(NULL, "-");
                    if (!part) {
                        isValid = QDate::isValid(y, m, d);
                    }
                }
            }
        }
    }
    
    if (isValid) {
        dstDate.setYMD(y, m, d);
    }
}

/*
** isAdmin - Returns true if the current user is an administrator, false
**           if not.
*/

bool isAdmin()
{
    bool         retVal = false;
    if (curUser().accessLevel == Admin) retVal = true;
    return retVal;
}

/*
** isManager - Returns true if the current user is an administrator, false
**             if not.
*/

bool isManager()
{
    bool    retVal = false;
    if (curUser().accessLevel == Admin || curUser().accessLevel == Manager) retVal = true;
    return retVal;
}


/*
** curUser - Returns a taa_User structure with the current user in it.
*/

taa_User curUser()
{
    return myCurrentUser;
}

/*
** setCurUser - Sets the current user information from the passed in
**              user structure.
*/

void setCurUser(taa_User newInfo)
{
    strcpy(myCurrentUser.userName,  newInfo.userName);
    strcpy(myCurrentUser.location,  newInfo.location);
    strcpy(myCurrentUser.agentID,   newInfo.agentID);
    strcpy(myCurrentUser.extension, newInfo.extension);
    strcpy(myCurrentUser.queue,     newInfo.queue);
    myCurrentUser.accessLevel = newInfo.accessLevel;
}

/*
** myTimeStr - Given a time_t value and a format string this will return
**             a formatted time.
*/

const char *timeToStr(time_t theTime, t_TimeFormat fmt)
{
    char    *retStr = new char[256];
    memset(retStr, 0, sizeof(retStr));

    // Extract the time from the time_t value.
    time_t myTime = theTime;
    struct tm *t = localtime(&myTime);

    // Setup our common variables that will be used by one or more formats.
    int theYear = t->tm_year + 1900;
    int twoYear = t->tm_year;
    while (twoYear > 99) twoYear -= 100;
    char    ampm[64];
    strcpy(ampm, "a");
    int apHour = t->tm_hour;
    if (apHour >= 12) {
        apHour -= 12;
        strcpy(ampm, "p");
    }
    if (apHour == 0) apHour = 12;

    switch (fmt) {
        case MM_DD_YY_HHa:              // MM/DD/YY HH:MMa
            sprintf(retStr, "%02d/%02d/%02d %2d:%02d%s",
                    t->tm_mon+1, t->tm_mday, twoYear,
                    apHour, t->tm_min, ampm);
            break;

        case YYYY_MM_DD_HH_MM_SS:    // YYYY-MM-DD HH:MM:SS (suitable for DB update)
            sprintf(retStr, "%04d-%02d-%02d %02d:%02d:%02d",
                    theYear, t->tm_mon+1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);
            break;

        case MM_DD_YY_HH_MMa_NA:        // MM/DD/YY HH:MMa or "N/A" if invalid
            sprintf(retStr, "%02d/%02d/%02d %2d:%02d%s",
                    t->tm_mon+1, t->tm_mday, twoYear,
                    apHour, t->tm_min, ampm);
            if (!theTime) strcpy(retStr, "N/A");
            break;
        
        case Mmm_DD_YYYY_HH_MMa:        // Mon DD, YYYY HH:MMa 
            sprintf(retStr, "%s %02d, %04d %2d:%02d%s",
                    t_MonthNames[t->tm_mon], t->tm_mday, theYear,
                    apHour, t->tm_min, ampm);
            if (!theTime) strcpy(retStr, "N/A");
            break;


        case YYYY_MM_DD_HH_MM:          // YYYY-MM-DD HH:MM
            sprintf(retStr, "%04d-%02d-%02d %02d:%02d",
                    theYear, t->tm_mon+1, t->tm_mday,
                    t->tm_hour, t->tm_min);
            break;

        case YYYY_MM_DD:               // YYYY-MM-DD
        default:
            sprintf(retStr, "%04d-%02d-%02d",
                    theYear, t->tm_mon+1, t->tm_mday);
            break;

    }

    return retStr;
}

/*
** rightNow - Returns the current time in time_t format.
*/

time_t rightNow()
{
    time_t  retVal;
    struct  timeval tv;
    // Get GMT
    gettimeofday(&tv, 0);
    retVal = tv.tv_sec;
    // Now, convert GMT into local time
    tm *t = localtime(&retVal);
    retVal = mktime(t);
    return retVal;
}

/*
** ymdhmToTime - Converts a year, month, day, hour, minute and second to
**               a time_t variable.
*/

time_t ymdhmToTime(int y, int m, int d, int h, int min, int s)
{
    tm  t;
    t.tm_sec = s;
    t.tm_min = min;
    t.tm_hour = h;
    t.tm_mday = d;
    t.tm_mon  = m - 1;
    t.tm_year = y - 1900;

    return mktime(&t);
}

/*
** showAbout  - Creates a QMessageBox showing the information about this
**              program.
*/

void showAbout()
{
    QString     aboutText;
    
    aboutText  = "Total <I>Account</I>Ability<BR>";
    aboutText += "(C)opyright 1997-2006, R. Marc Lewis<br>";
    aboutText += "and Avvanta Communications Corp.<br>";
    aboutText += "All Rights Reserved<BR>";
    //aboutText += "Contains ADB data management software\n(C)opyright 1998-2001, R. Marc Lewis, All Rights Reserved\n";
    aboutText += "<p>Version ";
    aboutText += VERSION;
    aboutText += "<br>";
    aboutText += BUILDNUM();

    QMessageBox::about(0, "Total AccountAbility", aboutText);
}

/*
** stripHTML - Given a const char *, this will strip any HTML from it and
**             return a clean string.
*/

const char *stripHTML(const char *src)
{
    string              tmpStr   = src;
    string::size_type   startpos = string::npos;
    string::size_type   endpos   = string::npos;

    // Strip any HTML from the source
    while ((startpos = tmpStr.find("<", startpos)) != string::npos) {
        endpos = tmpStr.find(">", startpos+1);
        if (endpos != string::npos) {
            tmpStr.replace(startpos, endpos-startpos+1, "");
        } else {
            // Malformed HTML.  Just drop the opening "<"
            tmpStr.replace(startpos, 1, "");
        }
    }

    return tmpStr.c_str();
}


/*
** recountAllowedMailboxes - Tallies up the number of mailboxes a customer
**                           is allowed to have based on their current
**                           login types and packages.
*/

void recountAllowedMailboxes(long customerID)
{
    ADB     DB;
    int     loginTotal = 0;
    int     pkgTotal   = 0;
    int     subTotal   = 0;

    // Get the login counts first.
    /*
    DB.query("select  Logins.LoginID, LoginTypes.AdditionalMailboxes from Logins, LoginTypes where Logins.CustomerID = %ld and Logins.Active <> 0 and LoginTypes.InternalID = Logins.LoginType", customerID);
    if (DB.rowCount) {
        loginTotal = DB.rowCount;
        while (DB.getrow()) {
            loginTotal += atoi(DB.curRow["AdditionalMailboxes"]);
        }
    }
    */

    // Get the billables total.
    DB.query("select Subscriptions.LoginID, Billables.NumMailboxes from Subscriptions, Billables where Subscriptions.CustomerID = %ld and Subscriptions.Active <> 0 and Subscriptions.PackageNo = 0 and Subscriptions.ParentID = 0 and Billables.ItemNumber = Subscriptions.ItemNumber", customerID);
    if (DB.rowCount) {
        while (DB.getrow()) {
            subTotal += atoi(DB.curRow["NumMailboxes"]);
        }
    }

    // Now, get the package total.
    DB.query("select Subscriptions.LoginID, Packages.NumMailboxes from Subscriptions, Packages where Subscriptions.CustomerID = %ld and Subscriptions.Active <> 0 and Subscriptions.PackageNo <> 0 and Packages.InternalID = Subscriptions.PackageNo", customerID);
    if (DB.rowCount) {
        while (DB.getrow()) {
            pkgTotal += atoi(DB.curRow["NumMailboxes"]);
        }
    }

    // Okay, we have our numbers.  Pick the higher of the two.
    //if (pkgTotal > loginTotal) loginTotal = pkgTotal;
    loginTotal = subTotal + pkgTotal;
    DB.dbcmd("update Customers set MailboxesAllowed = %d where CustomerID = %ld", loginTotal, customerID);

    // Thats it, we're done.
}


/*
** emailAdmins - Sends an email message to all of the email addresses listed
**               in the AdminEmailList configuration variable with the
**               specified subject and message body.
**
**               Should be used sparingly for events that need to have 
**               special notice taken of them.
*/

void emailAdmins(const char *subj, const char *body)
{
    char    *tmpstr;
    FILE    *fp;
    char    mname[1024];
    char    tmpstr2[1024];

    QStrList    admins;         // The list of admins to mail.
    
    admins.setAutoDelete(TRUE);
    
    tmpstr  = new(char[65536]);

    strcpy(tmpstr, cfgVal("AdminEmailList"));
    splitString(tmpstr, ':', admins, 1);
    
    for (unsigned int i = 0; i < admins.count(); i++) {
        // Create the tmp files.
        tmpnam(mname);
        fp = fopen(mname, "w");
        if (fp != NULL) {
            fprintf(fp, "From: Avvanta Support <support@avvanta.com>\n");
            fprintf(fp, "To: %s@avvanta.com\n", (const char *) admins.at(i));
            fprintf(fp, "Subject: %s\n", subj);
            fprintf(fp, "\n");
            fprintf(fp, "%s\n", body);
            fprintf(fp, "\n");
            
            fclose(fp);
            
            sprintf(tmpstr2, "/var/spool/taamail/%s-XXXXXX", (const char *) admins.at(i));
            sprintf(tmpstr, "cp %s %s", mname, mktemp(tmpstr2));
            system(tmpstr);
            
            unlink(mname);
        }
    }
    
    delete(tmpstr);
}

/*
 * setDebugLevel - Sets the current debug level
 */

void setDebugLevel(int newLevel)
{
    debugLevel = newLevel;
}

/*
 * debug - Spits out debugging information if the level is high enough
 */
void debug(int level, const char *format, ...)
{
    //fprintf(stderr, "Debug Level = %d, requested level = %d\n", debugLevel, level);
    if (level <= debugLevel) {
        va_list     ap;
        va_start(ap, format);
        char    *outstr = new char[265536];
        vsprintf(outstr, format, ap);
        fprintf(stderr, "%s", outstr);
        fflush(stderr);
    }
}


