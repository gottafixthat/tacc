/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangePassword.h
	Last generated: Fri May 22 18:03:34 1998

 *********************************************************************/

#ifndef ChangePassword_included
#define ChangePassword_included

#include "ChangePasswordData.h"

class ChangePassword : public ChangePasswordData
{
    Q_OBJECT

public:

    ChangePassword
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0,
        const char* LoginID = NULL
    );

    virtual ~ChangePassword();
    
    virtual void generatePassword();
    virtual void savePassword();
    virtual void cancelPassword();

protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }

private:
    long    myCustID;
    char    myLoginID[64];

};
#endif // ChangePassword_included
