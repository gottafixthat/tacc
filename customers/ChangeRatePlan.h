/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangeRatePlan.h
	Last generated: Thu Jul 23 19:19:55 1998

 *********************************************************************/

#ifndef ChangeRatePlan_included
#define ChangeRatePlan_included

#include "ChangeRatePlanData.h"

class ChangeRatePlan : public ChangeRatePlanData
{
    Q_OBJECT

public:

    ChangeRatePlan
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );
    virtual ~ChangeRatePlan();

    virtual void saveRatePlanChange();
    virtual void cancelRatePlanChange();

protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }

private:
    long    myCustID;

signals:
    void customerChanged(long);

};
#endif // ChangeRatePlan_included
