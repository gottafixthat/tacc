/**********************************************************************

	--- Qt Architect generated file ---

	File: RunSubscriptions.h
	Last generated: Wed Aug 26 15:05:42 1998

 *********************************************************************/

#ifndef RunSubscriptions_included
#define RunSubscriptions_included

#include "RunSubscriptionsData.h"

class RunSubscriptions : public RunSubscriptionsData
{
    Q_OBJECT

public:

    RunSubscriptions
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RunSubscriptions();

protected slots:
    virtual void cancelAction();
    virtual void processSelections();

private:
    void fillList(void);

    long    totSubscriptions;
    long    totActiveLogins;
    
    int     ARAcct;
    
    float   startingAR;
    float   totCharged;
};
#endif // RunSubscriptions_included
