/**********************************************************************

	--- Dlgedit generated file ---

	File: AccountsEdit.h
	Last generated: Fri Sep 19 17:56:49 1997

 *********************************************************************/

#ifndef AccountsEdit_included
#define AccountsEdit_included

#include "AccountsEditData.h"

class AccountsEdit : public AccountsEditData
{
    Q_OBJECT

public:

    AccountsEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int AcctNo = 0
    );

    virtual ~AccountsEdit();

signals:
    void refresh(int);

private slots:
    virtual void cancelAccount();
    virtual void saveAccount();
    
private:
    int AccountNo;
    int *AcctNoIndex;      // for our combo box.
    int IdxID;
};
#endif // AccountsEdit_included
