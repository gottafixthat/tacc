/**********************************************************************

	--- Qt Architect generated file ---

	File: DomainTypeBillablesAdd.h
	Last generated: Mon May 25 13:09:21 1998

 *********************************************************************/

#ifndef DomainTypeBillablesAdd_included
#define DomainTypeBillablesAdd_included

#include "DomainTypeBillablesAddData.h"

class DomainTypeBillablesAdd : public DomainTypeBillablesAddData
{
    Q_OBJECT

public:

    DomainTypeBillablesAdd
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int DomainTypeID = 0
    );

    virtual ~DomainTypeBillablesAdd();

private:
    int             myDomainTypeID;
    int             *billableIDX;
    
private slots:
    virtual void    addBillableItem();

};
#endif // DomainTypeBillablesAdd_included
