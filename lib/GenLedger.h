/**
 * GenLedger .h - A unified, objectivied interface into the general
 * ledger.  Manages both the transaction index as well as individual
 * split lines.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef GENLEDGER_H
#define GENLEDGER_H

#define MAXFIELDS    99

#include <ADB.h>
#include <qlist.h>
#include <qstring.h>
#include <qstringlist.h>

#define TRANSTYPE_CUSTCHARGE    1
#define TRANSTYPE_PAYMENT       2
#define TRANSTYPE_CCPAYMENT     3
#define TRANSTYPE_DEPOSIT       4

// DON'T EVER CHANGE THIS!
static QStringList GLAccountTypes = QStringList()
    << "Asset" << "Liability" << "Equity" << "Income"
    << "Cost of Goods Sold" << "Expense" << "Other Income" << "Other Expense";

// General Ledger (GL) layout

class GLDB
{
public:
    GLDB(void);
    ~GLDB();

    int get(long IntID);
    long ins(void);
    long insTrans(void);
    int  upd(void);
    int del(long TransID);


	// The GL table
    long    InternalID;
    QString TransID;
    QString AccountNo;
    QString Amount;
    QString LinkedTrans;
    QString TransType;
    QString	TransTypeLink;
    QString	TransDate;
    QString BilledDate;
    QString DueDate;
    QString	Cleared;
    QString	Number;
    QString	NumberStr;
    QString	ItemID;
    QString	Quantity;
    QString	Price;
    QString	Memo;
    
    // The GLIndex
    QString	IDXTransID;
    QString	SplitCount;
    QString	BaseDesc;
    
    
};


class GenLedger
{

public:
    GenLedger(void);
    ~GenLedger();
    
    long		TransID;
    int			SplitCount;
    QString		BaseDesc;
    
    uint		CurSplitNo;
    GLDB		*CurrentSplit;
    
    // Transaction functions
    int			GetTrans(long TransNo);
    long		SaveTrans(void);
    
    int			TransCheck(void);
    
    // Split Functions
    int			GetSplit(int SplitNo);
    int			AddSplit(void);
    int			DelSplit(int SplitNo);

private:
	QList<GLDB> Splits;
};


#endif // GENLEDGER_H
