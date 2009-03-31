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
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <Qt3Support/Q3PtrList>

#define TRANSTYPE_CUSTCHARGE    1
#define TRANSTYPE_PAYMENT       2
#define TRANSTYPE_CCPAYMENT     3
#define TRANSTYPE_DEPOSIT       4
#define TRANSTYPE_CCREFUND      5

struct GLAccountTypeRecord {
    int     accountType;
    QString description;
};

typedef Q3PtrList<GLAccountTypeRecord> GLAccountTypeList;

// GL Account Types
class GLAccountTypesDB
{
public:
    GLAccountTypesDB();
    virtual ~GLAccountTypesDB();

    static GLAccountTypeList   getAccountTypeList();
};

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
    QString IntAccountNo;
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
    
    int		    CurSplitNo;
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
	QList<GLDB *> Splits;
};


#endif // GENLEDGER_H
