/**
 * TAATools.h - Common global functions and tools.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#ifndef TAATools_included
#define TAATools_included

#include <qwidget.h>
#include <qstrlist.h>
#include <qlistview.h>
#include <qdatetm.h>
#include <time.h>

#define SCHEMA_VERSION_REQUIRED 7

enum t_TimeFormat {
    YYYY_MM_DD_HH_MM     = 0,
    MM_DD_YY_HHa         = 1,
    YYYY_MM_DD_HH_MM_SS  = 2,
    MM_DD_YY_HH_MMa_NA   = 3,
    Mmm_DD_YYYY_HH_MMa   = 4,
    YYYY_MM_DD           = 5
};

enum AccessLevels {Admin = 1, Manager = 2, Staff = 3};

struct taa_User {
    char            userName[64];
    char            location[64];       // Telephone Extension, i.e. SIP/x113
    char            agentID[64];        // Agent ID
    char            extension[64];      // Telephone Extension, numeric, i.e. 113
    char            queue[64];
    AccessLevels    accessLevel;
};


QWidget    *mainWin();
void        setMainWin(QWidget *newWinPtr);
bool        loadTAAConfig(bool isCritical = true);
int         schemaVersion();
bool        schemaValid();

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

void        emailAdmins(const char *subj, const char *body);

uint        QListViewToCSV(QListView *qlist, const char *fName, bool forceQuotes = false);

void        setDebugLevel(int newLevel);
void        debug(int level, const char *format, ... );

const char *stripHTML(const char *src);

const QString latexEscapeString(QString s);

#endif // TAATools_included


