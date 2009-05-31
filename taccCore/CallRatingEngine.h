/**************************************************************************
**
** CalLRatingEngine.h - Class definitions for the CDR rating engine.
**
***************************************************************************
** Written by R. Marc Lewis <marc@avvatel.com>
**   (C)opyright 2008, R. Marc Lewis
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of R. Marc Lewis or Avvatel Corporation.
***************************************************************************/

#ifndef CALLRATINGENGINE_H
#define CALLRATINGENGINE_H

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QString>

#define RATEOP_EQ   0
#define RATEOP_LE   1
#define RATEOP_LT   2
#define RATEOP_GE   3
#define RATEOP_GT   4
#define RATEOP_MA   5

#define RATEACT_CF  0
#define RATEACT_AF  1
#define RATEACT_RT  2
#define RATEACT_AP  3

enum cre_CallDirection {
    INBOUND     = 0,
    OUTBOUND    = 1
};

struct CallRatePlanRule {
    int     seqno;
    int     rateop;
    double  opamt;
    int     mact;
    double  mamt;
};

class CallRatingEngine 
{
public:
    CallRatingEngine();
    ~CallRatingEngine();

    int     setVerbosity(int);
    int     initDB(QString dbtype, QString dbhost, QString dbname, QString dbuser, QString dbpass);
    bool    setCarrier(long);
    int     setRatePlan(long);
    double  rateCalc(double rate, int duration, int min, int incr);
    double  rateCall(int callDir, QString src, QString dst, int duration);

    double  getTFRate(QString npanxx);

protected:
    double  adjustRate(double rate, int mact, double mamt);

private:
    int                     verbosity;
    long                    myCarrier;
    long                    termvers;
    long                    origvers;
    long                    tforigvers;
    long                    myRatePlanID;
    QString                 myDBType;
    QString                 myDBHost;
    QString                 myDBName;
    QString                 myDBUser;
    QString                 myDBPass;
    int                     minSec;
    int                     incSec;
    QHash<QString, double>  rateCache;
    QList<CallRatePlanRule> rules;

};

#endif

