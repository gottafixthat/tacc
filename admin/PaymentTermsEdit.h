/**
 * PaymentTermsEdit.h - Class definition to for the payment terms edit
 * widget.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef PaymentTermsEdit_included
#define PaymentTermsEdit_included

#include <qlineedit.h>
#include <qspinbox.h>
#include <TAAWidget.h>

class PaymentTermsEdit : public TAAWidget
{
    Q_OBJECT

public:

    PaymentTermsEdit(QWidget* parent = NULL, const char* name = NULL, int InternalID = 0);
    virtual ~PaymentTermsEdit();

public slots:
    void refreshVendList(int);
    
signals:
    void refresh(int);
    
protected slots:
    virtual void savePaymentTerms();
    virtual void cancelPaymentTerms();

protected:
    QLineEdit   *desc;
    QLineEdit   *dueDays;
    QLineEdit   *graceDays;
    QLineEdit   *financePct;
    QLineEdit   *financeDays;
    QLineEdit   *discountPct;
    QLineEdit   *discountDays;

private:
    int         myIntID;

};
#endif // PaymentTermsEdit_included

