/**********************************************************************

	--- Dlgedit generated file ---

	File: PaymentTerms.h
	Last generated: Wed Sep 17 20:41:04 1997

 *********************************************************************/

#ifndef PaymentTerms_included
#define PaymentTerms_included

#include "PaymentTermsData.h"

class PaymentTerms : public PaymentTermsData
{
    Q_OBJECT

public:

    PaymentTerms
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PaymentTerms();

signals:
    void refreshPaymentTermsList(int);
    
public slots:
    virtual void Hide();
    virtual void refreshList(int);

private slots:
    virtual void newPaymentTerms();
    virtual void editPaymentTerms();
    virtual void editPaymentTermsL(int msg = 0);
    virtual void deletePaymentTerms();
    virtual int IntIDfromList(int ListNum);

private:

    int *intIDIndex;
    int indexPtr;
};
#endif // PaymentTerms_included
