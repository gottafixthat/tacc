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
#include <qlist.h>
#include <qstring.h>

#define TRANSTYPE_CUSTCHARGE 1

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
    
private:
	GenLedger	*GL;
};

void applyCredits(long CustomerID);


#endif // ACCTSRECV_H
