/*
** $Id: $
**
***************************************************************************
**
** BillingCycleEdit - The widget responsible for editing a billing
**                    cycle definition.
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


#ifndef BILLINGCYCLEEDIT_H
#define BILLINGCYCLEEDIT_H

#include <TAAWidget.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qlabel.h>
#include <qcombobox.h>


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
