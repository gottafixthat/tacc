/**********************************************************************

	--- Qt Architect generated file ---

	File: ExtendGraceDate.h
	Last generated: Wed Feb 24 13:08:55 1999

 *********************************************************************/

#ifndef ExtendGraceDate_included
#define ExtendGraceDate_included

#include "ExtendGraceDateData.h"

#include <qdatetm.h>

class ExtendGraceDate : public ExtendGraceDateData
{
    Q_OBJECT

public:

    ExtendGraceDate
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0
    );

    virtual ~ExtendGraceDate();

protected slots:
    virtual void daysChanged(int newDays);
    virtual void cancelGrace();
    virtual void saveGrace();

signals:
    void        customerChanged(long);
    
private:
    long        myCustID;
    QDate       origGrace;
    QDate       newGrace;
};
#endif // ExtendGraceDate_included
