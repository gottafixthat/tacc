/**************************************************************************
**
** CallRatingEngine.cpp - CDR rating classes.
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

#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include <ADB.h>

#include "CallRatingEngine.h"

// CallRatingEngine - Constructor
CallRatingEngine::CallRatingEngine()
{
    verbosity    = 0;  // Run silent
    myCarrier    = 0;  // Start with no carrier
    termvers     = 0;
    origvers     = 0;
    tforigvers   = 0;
}

// CallRatingEngine - Destructor
CallRatingEngine::~CallRatingEngine()
{
}

// initDB - Sets up the database connection for the rating engine.
//
// @param dbtype - The type of database engine for Qt
// @param dbhost - The database host
// @param dbname - The database name
// @param dbuser - The database user name
// @param dbpass - The database password
//
// @return 1 on successful connection to the database
int CallRatingEngine::initDB(QString dbtype, QString dbhost, QString dbname, QString dbuser, QString dbpass)
{
    int     retVal = 1;

    myDBType    = dbtype;
    myDBHost    = dbhost;
    myDBName    = dbname;
    myDBUser    = dbuser;
    myDBPass    = dbpass;

    return retVal;
}

// setCarrier - Set the carrier that we will use to rate calls with
//
// @param newCarrier - The ID of the new carrier to use
bool CallRatingEngine::setCarrier(long newCarrier)
{
    // Check if the carrier is different, if so clear our cache
    if (myCarrier != newCarrier) rateCache.clear();

    ADB     db(myDBName.toAscii(), myDBUser.toAscii(), myDBPass.toAscii(), myDBHost.toAscii());

    db.query("select * from Carriers where CarrierID = %ld", newCarrier);
    if (!db.rowCount) {
        return false;
    }
    db.getrow();
    myCarrier = newCarrier;
    termvers    = atoi(db.curRow["termvers"]);
    origvers    = atoi(db.curRow["origvers"]);
    tforigvers  = atoi(db.curRow["tforigvers"]);

}

// setRatePlan - Sets the CallRatePlan ID.
//
// This loads the rate plan data into memory.
//
// @param planid - The crpid to load from the database.
//
// @return true if it loaded the rate plan, false otherwise.
int CallRatingEngine::setRatePlan(long crpid)
{
    int retVal = 0;

    ADB     db(myDBName.toAscii(), myDBUser.toAscii(), myDBPass.toAscii(), myDBHost.toAscii());

    db.query("select * from rate_plans where id = %ld", crpid);
    if (db.rowCount) {
        db.getrow();
        myRatePlanID = crpid;
        minSec = atoi(db.curRow["minsec"]);
        incSec = atoi(db.curRow["incsec"]);

        // Now, get the rules.
        rules.clear();
        db.query("select * from rate_plan_rules where rateplan_id = %ld order by seqno", myRatePlanID);
        if (db.rowCount) {
            while(db.getrow()) {
                CallRatePlanRule    rule;
                rule.seqno = atoi(db.curRow["seqno"]);
                rule.rateop = RATEOP_EQ;        // Default to equal
                if (!strcmp(db.curRow["rateop"], "eq")) rule.rateop = RATEOP_EQ;
                else if (!strcmp(db.curRow["rateop"], "le")) rule.rateop = RATEOP_LE;
                else if (!strcmp(db.curRow["rateop"], "lt")) rule.rateop = RATEOP_LT;
                else if (!strcmp(db.curRow["rateop"], "ge")) rule.rateop = RATEOP_GE;
                else if (!strcmp(db.curRow["rateop"], "gt")) rule.rateop = RATEOP_GT;
                else if (!strcmp(db.curRow["rateop"], "ma")) rule.rateop = RATEOP_MA;
                rule.opamt = atof(db.curRow["opamt"]);
                rule.mact = RATEACT_CF;         // Default to charge fixed
                if (!strcmp(db.curRow["mact"], "cf")) rule.mact = RATEACT_CF;
                else if (!strcmp(db.curRow["mact"], "af")) rule.mact = RATEACT_AF;
                else if (!strcmp(db.curRow["mact"], "rt")) rule.mact = RATEACT_RT;
                else if (!strcmp(db.curRow["mact"], "ap")) rule.mact = RATEACT_AP;
                rule.mamt = atof(db.curRow["mamt"]);
                rules.append(rule);
                //fprintf(stderr, "rateop = '%s' (%d), mamt = %f\n", db.curRow["rateop"], rule.rateop, atof(db.curRow["mamt"]));
            }
        }
        fprintf(stdout, "Loaded rateplan with %d calling rules:\n", rules.count());

    }

    return retVal;
}

// rateCalc - Do the calculations and return the rate for a call
//
// @param rate - The actual rate per minute to use.
// @param duration - The billable seconds.
// @param min - The minimum number of seconds that we will bill for.
// @param incr - The increment step.  The duration will be rounded up to the
//               nearest increment.
//
// @return The charge for the call.
double CallRatingEngine::rateCalc(double rate, int duration, int min, int incr)
{
    double callrate = 0.0;

    // If there are no billable seconds, return 0.
    if (duration == 0) return callrate;

    // If there are billable seconds, make sure its at least our minimum
    if (duration < min) duration = min;

    // Round to the nearest increment
    int rest = duration - min;
    if (rest % incr) rest += incr - (rest % incr);
    duration = min + rest;


    //fprintf(stderr, "Duration is now %d (min = %d, incr = %d)\n", duration, min, incr);


    // First pass, get the raw rate for this call
    callrate = rate * ((double)duration / 60);

    // Now round up to the next penny.
    long intval = (long) (callrate * 10000);
    intval += 100 - (intval % 100);

    // Reset our return value to the rounded one.
    callrate = (double) intval / 10000;

    return callrate;
}

// rateCall - Rate a call
//
// @param dir - The call direction
// @param src - The calling number to rate
// @param dst - The destination number
// @param duration - The billable seconds.
// @param min - The minimum number of seconds that we will bill for.
// @param incr - The increment step.  The duration will be rounded up to the
//               nearest increment.
//
// @return The charge for the call.
double CallRatingEngine::rateCall(int callDir, QString src, QString dst, int duration)
{
    double  retVal = 0.0;
    double  rate = 0.039;
    int     isTF = 0;
    QRegExp tfr("^(8(00|55|66|77|88)[2-9]\\d{6})$");

    QString cDir = "Inbound  ";
    if (callDir == OUTBOUND) cDir = "Outbound ";
    // Check for toll free...
    if (callDir == INBOUND && tfr.indexIn(dst) >= 0) {
        isTF = 1;
        cDir = "Toll Free";
    }

    // Get the rates for this carrier/call
    if (isTF) {
        rate = getTFRate(src.left(6));
    }

    // Now, walk through our rate list and find a match.
    bool matchFound = false;
    for (int i = 0; i < rules.size(); i++) {
        CallRatePlanRule rule = rules.at(i);
        if (!matchFound) {
            //fprintf(stderr, "Examining rule %d, seqno %d, rateop = %d, mamt = %f\n", i, rule.seqno, rule.rateop, rule.mamt);
            if (rule.rateop == RATEOP_EQ && rate == rule.opamt) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            } else if (rule.rateop == RATEOP_LE && rate <= rule.opamt) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            } else if ((rule.rateop == RATEOP_LT) && (rate < rule.opamt)) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            } else if (rule.rateop == RATEOP_GE && rate >= rule.opamt) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            } else if (rule.rateop == RATEOP_GT && rate > rule.opamt) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            } else if (rule.rateop == RATEOP_MA) {
                rate = adjustRate(rate, rule.mact, rule.mamt);
                matchFound = true;
                continue;
            }
        }
    }

    retVal = rateCalc(rate, duration, minSec, incSec);

    fprintf(stdout, "%s Call from %10s to %10s for %4ds rated at $%f/min for $%7.3f\n", cDir.toAscii().data(), src.toAscii().data(), dst.toAscii().data(), duration, rate, retVal);


    return retVal;
}

//
// getTFRate() - Given an NPA/NXX this gets a toll free rate.
//
double CallRatingEngine::getTFRate(QString npanxx)
{
    double  retVal = -1.0;

    ADB     db(myDBName.toAscii(), myDBUser.toAscii(), myDBPass.toAscii(), myDBHost.toAscii());

    db.query("select tforigrates.rate, voiplatas.lata from tforigrates, voiplatas where tforigrates.CarrierID = %ld and tforigrates.tforigvers = %ld and tforigrates.lata = voiplatas.lata and voiplatas.npanxx = '%s'", myCarrier, tforigvers, npanxx.toAscii().data());
    if (!db.rowCount) {
        // Get the rate for lata '99999' "all others"
        db.query("select tforigrates.rate from tforigrates where lata = '99999' and CarrierID = %ld and tforigvers = %ld", myCarrier, tforigvers);
        if (!db.rowCount) {
            fprintf(stderr, "Unable to locate rate for Carrier ID %ld from NPA/NXX '%s'\n", myCarrier, npanxx.toAscii().data());
        } else {
            db.getrow();
            retVal = atof(db.curRow["rate"]);
        }
    } else {
        db.getrow();
        retVal = atof(db.curRow["rate"]);
    }

    return retVal;
}

/**
 * adjustRate() - Adjusts the rate of a call based on the action.
 *
 * Given a rate, an action and an action amount, this adjusts the
 * rate accordingly.
 */
double CallRatingEngine::adjustRate(double rate, int mact, double mamt)
{
    double retVal = rate;
    //fprintf(stderr, "adjustRate(%f, %d, %f)\n", rate, mact, mamt);

    switch(mact) {
        case RATEACT_CF:    // Charge Fixed
            //printf("Charge Fixed: Ajusting rate to %f\n", mamt);
            retVal = mamt;
            break;
        case RATEACT_AF:    // Add Fixed
            //printf("Add Fixed: Ajusting rate to %f\n", rate+mamt);
            retVal = rate + mamt;
            break;
        case RATEACT_RT:    // FIXME:  This is rate table lookup
            //printf("Rate Table: Ajusting rate to %f\n", rate+mamt);
            retVal = rate + mamt;
            break;
        case RATEACT_AP:    // Add Percent
            //printf("Add Percent: Ajusting rate to %f\n", rate+(rate * mamt));
            retVal += (rate * mamt);
            break;
    }

    return retVal;
}
