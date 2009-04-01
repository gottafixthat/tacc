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

#ifndef CHANGERATEPLAN_H
#define CHANGERATEPLAN_H

#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3datetimeedit.h>

#include <TAAWidget.h>

class ChangeRatePlan : public TAAWidget
{
    Q_OBJECT

public:

    ChangeRatePlan(QWidget* parent = NULL, const char* name = NULL, long CustID = 0);
    virtual ~ChangeRatePlan();

protected slots:
    virtual void saveRatePlanChange();
    virtual void cancelRatePlanChange();
    virtual void QCloseEvent(QEvent *)              { delete this; }

protected:
    long        myCustID;
    QComboBox   *ratePlanList;
    Q3DateEdit   *effectiveDate;
    QLabel      *customer;

signals:
    void customerChanged(long);

};
#endif

// vim: expandtab

