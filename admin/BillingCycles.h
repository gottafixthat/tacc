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


#ifndef BillingCycles_included
#define BillingCycles_included

#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3strlist.h>

#include <TAAWidget.h>

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
    virtual void closeClicked();
    virtual void newCycle();
    virtual void editCycle();
    virtual void deleteCycle();

protected:
    Q3ListView   *list;
};
#endif // BillingCycles_included

// vim: expandtab
