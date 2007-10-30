/*
** Accounts.cpp - Shows the user the list of GL accounts in the system.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2007, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#ifndef Accounts_included
#define Accounts_included

#include "TAAWidget.h"
#include <qlistview.h>
#include <qmenubar.h>

class Accounts : public TAAWidget
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
    void Hide();
    void refreshList(int);
    void editAccountL(int msg = 0);

private slots:
    void newAccount();
    void editAccount();
    void deleteAccount();

protected:
    QMenuBar    *menu;
    QListView   *list;

private:
    void addToList(int AcctNo, QListViewItem *parent);
    // int *acctNoIndex;
    // int indexPtr;
};
#endif // Accounts_included
