/**
 * ChartofAccounts.h - Shows the user the Chart of Accounts
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#ifndef CHARTOFACCOUNTS_H
#define CHARTOFACCOUNTS_H

#include <qlistview.h>
#include <qmenubar.h>

#include <TAAWidget.h>
#include <GenLedger.h>

class ChartOfAccounts : public TAAWidget
{
    Q_OBJECT

public:

    ChartOfAccounts
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ChartOfAccounts();
    
signals:
    void refreshAccountsList(int);
    
public slots:
    void Hide();
    void refreshList(int);

private slots:
    void newAccount();
    void editAccount();
    void deleteAccount();
    void editAccountTypes();

protected:
    QMenuBar    *menu;
    QListView   *list;

    GLAccountTypeList   glAcctTypes;

private:
    int addToList(int ParentID, QListViewItem *parent);
    int intAcctNoCol;
    // int *acctNoIndex;
    // int indexPtr;
};

#endif
