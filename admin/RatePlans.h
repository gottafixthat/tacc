/**********************************************************************

	--- Dlgedit generated file ---

	File: RatePlans.h
	Last generated: Tue Oct 14 16:04:44 1997

 *********************************************************************/

#ifndef RatePlans_included
#define RatePlans_included

#include "RatePlansData.h"

class RatePlans : public RatePlansData
{
    Q_OBJECT

public:

    RatePlans
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RatePlans();

public slots:
    virtual void Hide();
    virtual void refreshList(int);

private slots:
    virtual void newRatePlan();
    virtual void editRatePlan();
    virtual void editRatePlanL(int msg = 0);
    virtual void deleteRatePlan();
    
private:
    int		*planIndex;

};
#endif // RatePlans_included
