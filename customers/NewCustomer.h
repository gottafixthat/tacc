/**********************************************************************

	--- Dlgedit generated file ---

	File: NewCustomer.h
	Last generated: Sun Nov 2 15:45:15 1997

 *********************************************************************/

#ifndef NewCustomer_included
#define NewCustomer_included

#include "NewCustomerData.h"

class NewCustomer : public NewCustomerData
{
    Q_OBJECT

public:

    NewCustomer
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~NewCustomer();

signals:
	void refreshCustList();
    
private:
	virtual void addCustomer();
	
	int checkPhone(char * phone);

};
#endif // NewCustomer_included
