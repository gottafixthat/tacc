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

#ifndef CustRegister_included
#define CustRegister_included

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3listview.h>

#include <BlargDB.h>
#include <TAAWidget.h>

class CustRegister : public TAAWidget
{
    Q_OBJECT

public:

    CustRegister
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~CustRegister();

private:
	long			myCustID;

public slots:
	virtual void refreshRegister(int);
    virtual void refreshCustomer(long);

private slots:
	virtual void refreshRegisterS(int);
	virtual void newCustCharge();
	virtual void editCustCharge();
	virtual void delCustCharge();
	virtual void voidCustCharge();
	virtual void printRegister();

protected:
    QPushButton *newChargeButton;
    QPushButton *editChargeButton;
    QPushButton *delChargeButton;
    QPushButton *voidChargeButton;
    Q3ListView *list;
    QLabel *customerName;
    QLabel *custIDLabel;
    QLabel *currentBalanceArea;
    QLabel *customerStatus;

protected slots:
    virtual void QCloseEvent(QEvent *)          { delete this; }

private:
    void printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance);
    void printFooter(QPainter *p, int PageNo);
    void registerHeader(QPainter *p);
};
#endif // CustRegister_included



// vim: expandtab

