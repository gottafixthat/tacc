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

#ifndef CustChargeEdit_included
#define CustChargeEdit_included

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3datetimeedit.h>

#include <TAAWidget.h>
#include <GenLedger.h>

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
    Q3MultiLineEdit *memo;
    Q3DateEdit   *chargeDate;
    Q3DateEdit   *startDate;
    Q3DateEdit   *stopDate;
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

// vim: expandtab
