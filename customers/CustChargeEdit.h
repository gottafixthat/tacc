/*
** $Id: CustChargeEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** CustChargeEdit - Allows the user to enter charges directly for a customer.
*/

#ifndef CustChargeEdit_included
#define CustChargeEdit_included

#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qmultilinedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qdatetimeedit.h>
#include "GenLedger.h"
#include <TAAWidget.h>

class CustChargeEdit : public TAAWidget
{
    Q_OBJECT

public:

    CustChargeEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        const long CustID = 0,
        const long TransID = 0
    );

    virtual ~CustChargeEdit();
    
signals:
	void chargeSaved(int);

protected:
    QLabel      *custNameDispLabel;
    QLabel      *custNameDisp;
    QLabel      *custIDDispLabel;
    QLabel      *custIDDisp;
    QComboBox   *loginList;
    QComboBox   *itemList;
    QLineEdit   *quantity;
    QLineEdit   *price;
    QLabel      *ratePlanDispLabel;
    QLabel      *ratePlanLabel;
    QLabel      *cycleDispLabel;
    QLabel      *cycleLabel;
    QLineEdit   *units;
    QLabel      *totalCharge;
    QCheckBox   *taxable;
    QMultiLineEdit *memo;
    QDateEdit   *chargeDate;
    QDateEdit   *startDate;
    QDateEdit   *stopDate;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void         calculateQuantity();

private:
    long	    myCustID;
    long	    myTransID;
    long	    myRatePlan;
    long	    myCycleID;

	GenLedger	*myGL;
	long		*itemIDX;
	

protected slots:
    virtual void cancelPressed();
    virtual void priceChanged(const QString&);
    virtual void saveCharge();
	virtual	void quantityChanged(const QString &);
	virtual void itemChanged(int newItemNumber);
    void         periodChanged(const QDate &);

};
#endif // CustChargeEdit_included
