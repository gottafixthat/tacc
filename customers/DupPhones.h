/**********************************************************************

	--- Dlgedit generated file ---

	File: DupPhones.h
	Last generated: Fri Nov 7 19:41:54 1997

 *********************************************************************/

#ifndef DupPhones_included
#define DupPhones_included

#include "DupPhonesData.h"

#include <qstrlist.h>

class DupPhones : public DupPhonesData
{
    Q_OBJECT

public:

    DupPhones
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DupPhones();
    
    void AddDupeEntry(long CustID, const char * FullName, const char * DayPhone, float Balance);
    

protected slots:
	virtual void viewDupCustomer();
	virtual void viewDupCustomerL(int);

private:
	QStrList	*custIDIndex;

};
#endif // DupPhones_included
