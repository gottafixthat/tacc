/*
** $Id: CustRegister.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** CustRegister   - Class definitions for CustRegister.
**
**                  CustRegister loads and allows the manipulation of
**                  a customers register directly.
*/

#ifndef CustRegister_included
#define CustRegister_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <qlistview.h>
#include <qpainter.h>
#include <BlargDB.h>
#include <qpushbutton.h>
#include <qlabel.h>

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
    QListView *list;
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


