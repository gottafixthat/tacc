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
    QLabel      *custNameLabel;
    QComboBox   *loginList;
    QComboBox   *itemList;
    QLineEdit   *quantity;
    QLineEdit   *price;
    QLabel      *ratePlanLabel;
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

private:
    long	    myCustID;
    long	    myTransID;
    long	    myRatePlan;
    long	    myCycleID;

	GenLedger	*myGL;
	long		*itemIDX;
	
	virtual	void quantityChanged(const QString &);

protected slots:
    virtual void cancelPressed() =0;
    virtual void priceChanged(const QString&) =0;
    virtual void saveCharge() =0;

private slots:
	virtual void itemChanged(int newItemNumber);

};
#endif // CustChargeEdit_included
