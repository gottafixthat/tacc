/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef CustCallLogReport_included
#define CustCallLogReport_included

#include "Report.h"

class CustCallLogReport : public Report
{
    Q_OBJECT

public:

    CustCallLogReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CustCallLogReport();

    virtual void refreshReport();

    virtual void setCustID(long CustID);

protected slots:
    virtual void showCall(QListViewItem *);

private:
    long    myCustID;
    
    void    addFollowups(QListViewItem *curItem, long CallID);
};
#endif // CustCallLogReport_included
