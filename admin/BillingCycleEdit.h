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


#ifndef BILLINGCYCLEEDIT_H
#define BILLINGCYCLEEDIT_H

#include <QtCore/QString>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

#include <TAAWidget.h>

class BillingCycleEdit : public TAAWidget
{
    Q_OBJECT

public:

    BillingCycleEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BillingCycleEdit();

    void    setCycleID(const QString);

signals:
    void refresh(int);
    
private slots:
    virtual void cycleTypeChanged(int);
    virtual void saveClicked();
    virtual void cancelClicked();

protected:    
    QString     myCycleID;
    int         EditingCycle;
    QLineEdit   *cycleID;
    QLineEdit   *description;

    QLabel      *dayOfMonthLabel;
    QSpinBox    *dayOfMonth;
    QLabel      *monthsLabel;
    QCheckBox   *january;
    QCheckBox   *february;
    QCheckBox   *march;
    QCheckBox   *april;
    QCheckBox   *may;
    QCheckBox   *june;
    QCheckBox   *july;
    QCheckBox   *august;
    QCheckBox   *september;
    QCheckBox   *october;
    QCheckBox   *november;
    QCheckBox   *december;
    QCheckBox   *defaultCycle;
    QComboBox   *cycleType;
    QLabel      *anniversaryPeriodLabel;
    QSpinBox    *anniversaryPeriod;
};
#endif // BillingCycleEdit_included
// vim: expandtab

