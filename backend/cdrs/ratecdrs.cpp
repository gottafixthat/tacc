/**************************************************************************
**
** FileName     - Overview of its main function.
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

#include <CallRatingEngine.h>
#include <Cfg.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariant>
#include <QtCore/QRegExp>

#include "ratecdrs.h"

// rateCDRs - Rates CDRs that have not yet been rated.

void rateCDRs(void)
{
    CallRatingEngine cre;
    int fnSrcNumber;
    int fnDstNumber;
    int fnProfile;
    int fnBillsec;

    if (cre.initDB(cfgVal("TAASQLDriver"), cfgVal("TAAMySQLHost"), cfgVal("TAAMySQLDB"), cfgVal("TAAMySQLUser"), cfgVal("TAAMySQLPass")) != 1) {
        fprintf(stderr, "Unable to connect to the database to rate calls!\n");
        return;
    }

    // Start rating the calls.
    QSqlDatabase rDB;
    rDB = QSqlDatabase::addDatabase(cfgVal("CDRDBType"), "rating");
    rDB.setHostName(cfgVal("CDFDBHost"));
    rDB.setDatabaseName(cfgVal("CDRDBName"));
    rDB.setUserName(cfgVal("CDRDBUser"));
    rDB.setPassword(cfgVal("CDRDBPass"));

    if (!rDB.open()) {
        fprintf(stderr, "rateCDRs - Unable to connect to the database to rate calls!\n");
        return;
    }

    // FIXME:  We need to get the rate plan for this customer.
    cre.setRatePlan(1);

    // FIXME:  This should actually be a bit more particular about what calls
    // we're going to rate.  Right now we'll just hit them all.
    QSqlQuery query(rDB);
    query.prepare("select * from cdr where normalized = 0 and UserID > 0 and dstnumber in('8882461945','8773840808','8669473688', '8663840808') order by callstart");
    if (!query.exec()) {
        fprintf(stderr, "rateCDRs - Error performing query!\n");
        return;
    }

    if (query.size() == 0) {
        fprintf(stdout, "rateCDRs - No calls are set to be normalized.\n");
        return;
    }

    fnSrcNumber  = query.record().indexOf("srcuser");
    fnDstNumber  = query.record().indexOf("dstuser");
    fnProfile    = query.record().indexOf("sipprofile");
    fnBillsec = query.record().indexOf("billsec");
    float total = 0.0;
    while(query.next()) {
        float   amount = 0.0;
        int     callDir = OUTBOUND;

        cre.setCarrier(2);
        // FIXME:  There should be a better way of determining the direction of the call.
        if (!strcmp("external", query.value(fnProfile).toString())) {
            callDir = INBOUND;
        }
        amount = cre.rateCall(callDir, query.value(fnSrcNumber).toString(), query.value(fnDstNumber).toString(), query.value(fnBillsec).toInt());
        //fprintf(stdout, "Call to %10s for %4ds rated at $%7.3f\n", query.value(fnNumber).toString().toAscii().data(), query.value(fnBillsec).toInt(), amount);
        total += amount;
    }
    fprintf(stdout, "Total billed amount: %.2f\n", total);

    fprintf(stdout, "rateCDRs - Found %d calls to normalize and rate.\n", query.size());


}


