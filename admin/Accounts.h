/**********************************************************************

	--- Dlgedit generated file ---

	File: Accounts.h
	Last generated: Thu Sep 18 19:35:23 1997

 *********************************************************************/

#ifndef Accounts_included
#define Accounts_included

#include "AccountsData.h"
#include <qlistview.h>

class Accounts : public AccountsData
{
    Q_OBJECT

public:

    Accounts
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~Accounts();
    
signals:
    void refreshAccountsList(int);
    
public slots:
    virtual void Hide();
    virtual void refreshList(int);
    virtual void editAccountL(int msg = 0);

private slots:
    virtual void newAccount();
    virtual void editAccount();
    virtual void deleteAccount();

private:
    void addToList(int AcctNo, QListViewItem *parent);
    // int *acctNoIndex;
    // int indexPtr;
};
#endif // Accounts_included
