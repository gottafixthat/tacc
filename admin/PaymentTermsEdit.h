/**********************************************************************

	--- Dlgedit generated file ---

	File: PaymentTermsEdit.h
	Last generated: Wed Sep 17 21:15:14 1997

 *********************************************************************/

#ifndef PaymentTermsEdit_included
#define PaymentTermsEdit_included

#include "PaymentTermsEditData.h"

class PaymentTermsEdit : public PaymentTermsEditData
{
    Q_OBJECT

public:

    PaymentTermsEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int InternalID = 0
    );

    virtual ~PaymentTermsEdit();

public slots:
    void refreshVendList(int);
    
signals:
    void refresh(int);
    
private slots:
    virtual void savePaymentTerms();
    virtual void cancelPaymentTerms();

private:
    int *intIDIndex;      // for our combo box.
    
    int IntID;

};
#endif // PaymentTermsEdit_included

