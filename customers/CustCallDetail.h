/**********************************************************************

	--- Qt Architect generated file ---

	File: CustCallDetail.h
	Last generated: Tue May 11 16:12:35 1999

 *********************************************************************/

#ifndef CustCallDetail_included
#define CustCallDetail_included

#include "CustCallDetailData.h"

class CustCallDetail : public CustCallDetailData
{
    Q_OBJECT

public:

    CustCallDetail
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CustCallDetail();

    void    setCustID(long custID);
    void    setCallNumber(int callNo);

protected:
    virtual void nextCall();
    virtual void previousCall();
    virtual void closeClicked();
    
protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }

private:

    void    loadCallRecords(void);

    long    myCustID;
    int     curCallNo;

    int     callCount;
    long    *callList;    

};
#endif // CustCallDetail_included
