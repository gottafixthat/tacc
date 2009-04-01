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

#ifndef CHANGEBILLINGCYCLE_H
#define CHANGEBILLINGCYCLE_H

#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3datetimeedit.h>

#include <TAAWidget.h>

class ChangeBillingCycle : public TAAWidget
{
    Q_OBJECT

public:

    ChangeBillingCycle(QWidget* parent = NULL, const char* name = NULL, long CustID = 0);

    virtual ~ChangeBillingCycle();

protected slots:
    virtual void saveBillingCycleChange();
    virtual void cancelBillingCycleChange();
    virtual void QCloseEvent(QEvent *)              { delete this; }
    
protected:
    QLabel      *customer;
    QComboBox   *cycleList;
    Q3DateEdit   *effectiveDate;

    long    myCustID;

};
#endif

// vim: expandtab
