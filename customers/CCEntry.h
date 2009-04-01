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

#ifndef CCEntry_included
#define CCEntry_included

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>

#include <CCTools.h>
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
    QComboBox   *cardList;
    QLineEdit   *chargeAmount;
    QLabel      *autoPayLabel;
    QCheckBox   *autoPay;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QLabel      *balance;
    QLabel      *cardTypeLabel;
    QComboBox   *cardType;
    QLabel      *cardNumberLabel;
    QLineEdit   *cardNumber;
    QLabel      *expDateLabel;
    QLineEdit   *expDate;
    QLabel      *cvInfoLabel;
    QLineEdit   *cvInfo;
    QLabel      *chargeDayLabel;
    QSpinBox    *chargeDay;

    void        fillContactInfo();

private:
    long            myCustID;
    CreditCardList  cards;

protected slots:
    void        cardListChanged(int);

signals:
    void        customerChanged(long);
};
#endif // CCEntry_included

// vim: expandtab
