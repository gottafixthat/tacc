/**
 * CCEntry.h - Class definition for taking a credit card payment from a
 * customer.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CCEntry_included
#define CCEntry_included

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qspinbox.h>

#include <TAAWidget.h>

class CCEntry : public TAAWidget
{
    Q_OBJECT

public:

    CCEntry
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~CCEntry();
    
protected slots:
    void        saveCCard();
    void        cancelCCard();
    void        autoPayChanged(bool);

protected:
    QLineEdit   *custName;
    QLineEdit   *addr1;
    QLineEdit   *addr2;
    QLineEdit   *city;
    QLineEdit   *state;
    QLineEdit   *zip;
    QLineEdit   *phone;
    QLineEdit   *chargeAmount;
    QCheckBox   *autoPay;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QLabel      *dateLabel;
    QLabel      *balance;
    QComboBox   *cardType;
    QLineEdit   *cardNumber;
    QLineEdit   *expDate;
    QLineEdit   *cvInfo;
    QSpinBox    *chargeDay;

private:
    long    myCustID;

signals:
    void    customerChanged(long);
};
#endif // CCEntry_included
