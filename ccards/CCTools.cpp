/**
 * CCTools.h - Some tools for working with credit card data.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qstring.h>

#include <ADB.h>
#include <BlargDB.h>
#include <TAATools.h>

#include <CCTools.h>

/**
 * getCreditCardList()
 *
 * Given a customer ID and a pointer to a list of CreditCardRecord structures
 * this will get the list of credit cards for said customer and return them
 * via the pointer and return the number of records we found.
 */
CreditCardList getCreditCardList(long custID)
{
    ADB                 DB;
    AutoPaymentsDB      APDB;
    CCTransDB           CTDB;
    CustomersDB         CDB;
    CreditCardList      cards;
    bool                foundIt;
    int                 idx = 0;
    QDate               today = QDate::currentDate();

    DB.query("select InternalID from AutoPayments where CustomerID = %ld and PaymentType = 1 order by AuthorizedOn desc", custID);
    if (DB.rowCount) while (DB.getrow()) {
        foundIt = false;
        APDB.get(atol(DB.curRow["InternalID"]));
        // Walk through the list of cards to see if we already have this one.
        CreditCardRecord *tmpCard;
        if (!cards.isEmpty()) for (tmpCard = cards.first(); tmpCard; tmpCard = cards.next()) {
            if (!tmpCard->cardNo.compare(tmpCard->cardNo, APDB.getStr("AcctNo"))) foundIt = true;
        }

        if (!foundIt) {
            // Verify this card isn't expired.
            QDate   expDate;
            myDateToQDate(APDB.getStr("ExpDate"), expDate);
            if (today.daysTo(expDate) >= 0) {
                // Not expired.  Add it to the list.
                CreditCardRecord    *card = new CreditCardRecord;
                card->idx           = idx;
                card->cardholder    = APDB.getStr("Name");
                card->addr          = APDB.getStr("Address");
                card->zip           = APDB.getStr("ZIP");
                card->cardType      = APDB.getInt("CardType");
                card->cardNo        = APDB.getStr("AcctNo");
                card->expDate       = expDate.toString("MMyy").ascii();
                card->ccv           = APDB.getStr("SecurityCode");

                idx++;
                cards.append(card);
            }
        }
    }

    // Now grab the cards from CCTrans and do the same for them.
    DB.query("select InternalID from CCTrans where CustomerID = %ld order by TransDate desc", custID);
    if (DB.rowCount) while(DB.getrow()) {
        CTDB.get(atol(DB.curRow["InternalID"]));
        // Check our list for this card
        foundIt = false;
        CreditCardRecord *tmpCard;
        if (!cards.isEmpty()) for (tmpCard = cards.first(); tmpCard; tmpCard = cards.next()) {
            if (!tmpCard->cardNo.compare(tmpCard->cardNo, CTDB.getStr("CardNo"))) foundIt = true;
        }

        if (!foundIt) {
            // Check to see if the card is expired.
            QString tmpExp = CTDB.getStr("ExpDate");
            int     month = tmpExp.left(2).toInt();
            int     year  = tmpExp.right(2).toInt();
            if (year < 70) year += 2000;
            else year += 1900;
            QDate   expDate;
            expDate.setYMD(year, month, 1);
            expDate.setYMD(year, month, expDate.daysInMonth());
            if (today.daysTo(expDate) >= 0) {
                // Add it to the list.
                CreditCardRecord    *card = new CreditCardRecord;
                card->idx           = idx;
                card->cardholder    = CTDB.getStr("Name");
                card->addr          = CTDB.getStr("Address");
                card->zip           = CTDB.getStr("ZIP");
                card->cardType      = CTDB.getInt("CardType");
                card->cardNo        = CTDB.getStr("CardNo");
                card->expDate       = expDate.toString("MMyy").ascii();
                card->ccv           = CTDB.getStr("SecurityCode");

                idx++;
                cards.append(card);
            }
        }
    }

    // Walk through the list again and set the card type string and the 
    // text that functions can use to insert them into lists.
    if (!cards.isEmpty()) {
        CreditCardRecord *card;
        if (!cards.isEmpty()) for (card = cards.first(); card; card = cards.next()) {
            switch (card->cardType) {
                case CCTYPE_MC:
                    card->cardTypeStr = "Mastercard";
                    break;
                case CCTYPE_VISA:
                    card->cardTypeStr = "Visa";
                    break;
                case CCTYPE_AMEX:
                    card->cardTypeStr = "AmEx";
                    break;
                case CCTYPE_DINERS:
                    card->cardTypeStr = "Diners";
                    break;
                case CCTYPE_DISC:
                    card->cardTypeStr = "Discover";
                    break;
                default:
                    card->cardTypeStr = "Unknown";
                    break;
            }

            QString dispStr;
            dispStr = card->cardTypeStr;
            dispStr += " ";
            dispStr += card->cardNo.left(1);
            dispStr += "...";
            dispStr += card->cardNo.right(4);
            dispStr += " Exp ";
            dispStr += card->expDate;
            card->listText = dispStr;
        }
    }

    return cards;
}

