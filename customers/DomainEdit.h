/* $Id: DomainEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** DomainEdit  - Allows editing domain information for a customer.
*/

#ifndef DomainEdit_included
#define DomainEdit_included

#include "DomainEditData.h"

class DomainEdit : public DomainEditData
{
    Q_OBJECT

public:

    DomainEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0
    );

    virtual ~DomainEdit();
    
public slots:
    virtual void saveDomain();
    virtual void cancelDomain();
    
private:
    long        myCustID;
    int         *domainTypeIDX;
    
signals:
    void        domainSaved(int);

};
#endif // DomainEdit_included
