/**********************************************************************

	--- Dlgedit generated file ---

	File: BillingCycleEdit.h
	Last generated: Sat Sep 20 13:11:15 1997

 *********************************************************************/

#ifndef BillingCycleEdit_included
#define BillingCycleEdit_included

#include "BillingCycleEditData.h"

class BillingCycleEdit : public BillingCycleEditData
{
    Q_OBJECT

public:

    BillingCycleEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        const char* CycleID = NULL
    );

    virtual ~BillingCycleEdit();

public slots:
    void refreshCycleList(int);
    
signals:
    void refresh(int);
    
private slots:
    virtual void saveCycle();
    virtual void cancelCycle();

private:    
    char myCycleID[20];
    int  EditingCycle;
};
#endif // BillingCycleEdit_included
