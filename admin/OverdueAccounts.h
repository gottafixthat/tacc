/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#ifndef OVERDUEACCOUNTS_H
#define OVERDUEACCOUNTS_H

#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class OverdueAccounts : public TAAWidget
{
    Q_OBJECT

public:

    OverdueAccounts(QWidget* parent = NULL, const char* name = NULL, int AccountTypes = 0);
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
    virtual void editCustomer(Q3ListViewItem *curItem);
    virtual void updateTotals();
    virtual void QCloseEvent(QEvent *)              { delete this; }
    
protected:
    QRadioButton    *noAction;
    QRadioButton    *sendReminder;
    QRadioButton    *suspendAccount;
    QLabel          *accountsOverdue;
    QLabel          *accountsSelected;
    QLabel          *amountOverdue;
    QLabel          *selectedOverdue;
    Q3ListView       *list;
};

#endif

// vim: expandtab
