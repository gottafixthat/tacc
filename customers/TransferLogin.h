/**********************************************************************

	--- Qt Architect generated file ---

	File: TransferLogin.h
	Last generated: Mon Sep 14 17:28:19 1998

 *********************************************************************/

#ifndef TransferLogin_included
#define TransferLogin_included

#include "TransferLoginData.h"

class TransferLogin : public TransferLoginData
{
    Q_OBJECT

public:

    TransferLogin
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TransferLogin();

    void    setSourceLogin(long custID, const char * LoginID);

    virtual void doTransfer();
    virtual void cancelTransfer();
    virtual void doQuery();
        

signals:
    void    refreshLoginList(int);
    void    refreshCust(int);

private:
    char    sourceLogin[64];
    long    myCustID;

};
#endif // TransferLogin_included
