/*
** $Id: CustChargeEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** CustChargeEdit - Allows the user to enter charges directly for a customer.
*/

#ifndef CustChargeEdit_included
#define CustChargeEdit_included

#include "CustChargeEditData.h"
#include "GenLedger.h"

class CustChargeEdit : public CustChargeEditData
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

private:
    long	    myCustID;
    long	    myTransID;
    long	    myRatePlan;
    long	    myCycleID;

	GenLedger	*myGL;
	long		*itemIDX;
	
	virtual void saveCharge();
	virtual void cancelPressed();
	virtual	void quantityChanged(const QString &);
	virtual	void priceChanged(const QString &);

private slots:
	virtual void itemChanged(int newItemNumber);

};
#endif // CustChargeEdit_included
