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

#ifndef ReceivePayment_included
#define ReceivePayment_included

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class ReceivePayment : public TAAWidget
{
    Q_OBJECT

public:

    ReceivePayment
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~ReceivePayment();

public slots:
    void         loadCustInfo();
    virtual void refreshCustomer(long);
    
protected slots:
    virtual void cancelPressed();
    virtual void savePayment();

signals:
    void customerChanged(long);
	
protected:
    QLabel      *customerIDLabel;
    QLabel      *customerNameLabel;
    QLabel      *addressLabel1;
    QLabel      *addressLabel2;
    QLabel      *phoneLabel;
    QLabel      *dateLabel;
    QLabel      *balanceLabel;
    QLineEdit   *checkNumber;
    QLineEdit   *amount;
    Q3ListView   *chargeList;
    QPushButton *saveButton;
    QPushButton *cancelButton;

private:
    void fillUncleared(void);
    
    long    myCustID;    
    float   balanceDue;

};
#endif // ReceivePayment_included

// vim: expandtab
