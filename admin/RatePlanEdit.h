/**********************************************************************

	--- Dlgedit generated file ---

	File: RatePlanEdit.h
	Last generated: Tue Oct 14 17:00:47 1997

 *********************************************************************/

#ifndef RatePlanEdit_included
#define RatePlanEdit_included

#include "RatePlanEditData.h"

class RatePlanEdit : public RatePlanEditData
{
    Q_OBJECT

public:

    RatePlanEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long IntID = 0
    );

    virtual ~RatePlanEdit();
    
public slots:
	virtual void toggleAutoConvert(bool newState);
	virtual void toggleConvertDays(bool newState);
	virtual void toggleConvertDate(bool newState);
	virtual void togglePromoPlan(bool newState);
	virtual void cancelRatePlan();
	virtual void saveRatePlan();

private:
	long	myIntID;
	long    *planIndex;
};
#endif // RatePlanEdit_included
