/**********************************************************************

	--- Dlgedit generated file ---

	File: BillingCycles.h
	Last generated: Sat Sep 20 13:18:44 1997

 *********************************************************************/

#ifndef BillingCycles_included
#define BillingCycles_included

#include "BillingCyclesData.h"
#include <qstrlist.h>

class BillingCycles : public BillingCyclesData
{
    Q_OBJECT

public:

    BillingCycles
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BillingCycles();

public slots:
    virtual void Hide();
    virtual void refreshList(int);

private slots:
    virtual void newCycle();
    virtual void editCycle();
    virtual void editCycleL(int msg = 0);
    virtual void deleteCycle();
    
};
#endif // BillingCycles_included
