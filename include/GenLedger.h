/* $Id: GenLedger.h,v 1.3 2004/01/22 21:00:15 marc Exp $
**
** GenLedger.h    - A unified, objectified interface into the general
**                  ledger.  Manages both the transaction index as well
**                  as individual split lines.
*/

#ifndef GENLEDGER_H
#define GENLEDGER_H

#define MAXFIELDS    99

#include "BlargDB.h"
#include <qlist.h>
#include <qstring.h>

#define TRANSTYPE_CUSTCHARGE    1
#define TRANSTYPE_PAYMENT       2
#define TRANSTYPE_CCPAYMENT     3
#define TRANSTYPE_DEPOSIT       4

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
