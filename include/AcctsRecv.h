/* $Id: AcctsRecv.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** AcctsRecv.h    - A unified, objectified interface into the general
**                  ledger.  Manages both the transaction index as well
**                  as individual split lines.
*/

#ifndef ACCTSRECV_H
#define ACCTSRECV_H

#define MAXFIELDS    99

#include "BlargDB.h"
#include "GenLedger.h"
#include <QtCore/QList>
#include <QtCore/QString>

#define TRANSTYPE_CUSTCHARGE 1

struct customerARAgingRecord {
    long        customerID;
    double      currentBalance;
    double      totalOverdue;
    double      currentDue;
    double      overdue;
    double      overdue30;
    double      overdue60;
    double      overdue90;
};

const customerARAgingRecord getCustomerAgingData(long custID);

class AcctsRecv
{

public:
    AcctsRecv(void);
    ~AcctsRecv();
    
    long		InternalID;
    AcctsRecvDB	*ARDB;
    
    // Transaction functions
    int			GetTrans(long IntID);
    long		SaveTrans(void);
    int         setGLAccount(const char *acctNo);
    int         setGLAccount(int intAcctNo);
    
private:
	GenLedger	*GL;
    int         myIntAcctNo;
};

void applyCredits(long CustomerID);


#endif // ACCTSRECV_H
