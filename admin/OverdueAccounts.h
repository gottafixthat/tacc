/**********************************************************************

	--- Qt Architect generated file ---

	File: OverdueAccounts.h
	Last generated: Fri Aug 14 17:32:03 1998

 *********************************************************************/

#ifndef OverdueAccounts_included
#define OverdueAccounts_included

#include "OverdueAccountsData.h"
#include <qlistview.h>

class OverdueAccounts : public OverdueAccountsData
{
    Q_OBJECT

public:

    OverdueAccounts
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int AccountTypes = 0
    );

    virtual ~OverdueAccounts();
    void    fillList(void);

private:
    long    totAccts;
    long    selAccts;
    float   totOverdue;
    float   totSelected;
    
    int     myAccountTypes;
    
    void    sendReminders(void);
    void    lockAccounts(void);

protected slots:
    virtual void processSelections();
    virtual void cancelOverdue();
    virtual void selectNone();
    virtual void selectAll();
    virtual void editCustomer(QListViewItem *curItem);
    virtual void updateTotals();
    virtual void QCloseEvent(QEvent *)              { delete this; }
    
};
#endif // OverdueAccounts_included
