/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangeBillingCycle.h
	Last generated: Tue Jul 28 12:55:25 1998

 *********************************************************************/

#ifndef ChangeBillingCycle_included
#define ChangeBillingCycle_included

#include "ChangeBillingCycleData.h"

class ChangeBillingCycle : public ChangeBillingCycleData
{
    Q_OBJECT

public:

    ChangeBillingCycle
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~ChangeBillingCycle();

    virtual void saveBillingCycleChange();
    virtual void cancelBillingCycleChange();

protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }
    
private:
    long    myCustID;
    
signals:
    void customerChanged(long);

};
#endif // ChangeBillingCycle_included
