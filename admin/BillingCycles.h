/*
** $Id: $
**
***************************************************************************
**
** BillingCycles - Billing Cycle Lists.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: $
**
*/


#ifndef BillingCycles_included
#define BillingCycles_included

#include <TAAWidget.h>
#include <qlistview.h>
#include <qstrlist.h>

class BillingCycles : public TAAWidget
{
    Q_OBJECT

public:

    BillingCycles
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BillingCycles();

public slots:
    virtual void refreshList(int);

protected slots:
    virtual void Hide();
    virtual void newCycle();
    virtual void editCycle();
    virtual void deleteCycle();
    virtual void editCycleL(int msg = 0);

protected:
    QListView   *list;
};
#endif // BillingCycles_included
