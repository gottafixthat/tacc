/*
** $Id: TAATools.h,v 1.3 2004/02/27 01:33:33 marc Exp $
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
** $Log: TAATools.h,v $
** Revision 1.3  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.2  2004/01/22 21:00:55  marc
** Added new dateToStr format
**
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef TAATools_included
#define TAATools_included

#include <qwidget.h>
#include <qstrlist.h>
#include <qdatetm.h>
#include <time.h>


enum t_TimeFormat {
    YYYY_MM_DD_HH_MM     = 0,
    MM_DD_YY_HHa         = 1,
    YYYY_MM_DD_HH_MM_SS  = 2,
    MM_DD_YY_HH_MMa_NA   = 3,
    Mmm_DD_YYYY_HH_MMa   = 4,
    YYYY_MM_DD           = 5
};

typedef enum AccessLevels {Admin = 1, Manager = 2, Staff = 3};

typedef struct taa_User {
    char            userName[64];
    AccessLevels    accessLevel;
};


QWidget    *mainWin();
void        setMainWin(QWidget *newWinPtr);

bool        dateValid(const char *tDate, bool showMessage = true);
void        myDateToQDate(const char *srcDate, QDate &dstDate);

const char *timeToStr(time_t, t_TimeFormat);
time_t      rightNow();
time_t      ymdhmToTime(int y, int m, int d, int h, int min, int s);

bool        isAdmin();
bool        isManager();
taa_User    curUser();
void        setCurUser(taa_User newInfo);

void        recountAllowedMailboxes(long customerID);

void        showAbout();

const char *stripHTML(const char *src);

#endif // TAATools_included


