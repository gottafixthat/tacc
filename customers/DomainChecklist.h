/**********************************************************************

	--- Qt Architect generated file ---

	File: DomainChecklist.h
	Last generated: Sat Jun 13 17:28:35 1998

 *********************************************************************/

#ifndef DomainChecklist_included
#define DomainChecklist_included

#include "DomainChecklistData.h"

class DomainChecklist : public DomainChecklistData
{
    Q_OBJECT

public:

    DomainChecklist
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long DomainID = 0
    );

    virtual ~DomainChecklist();

    void refreshCheckList();
    
    void saveChecklist();
    
public slots:
    virtual void cancelClicked();
    virtual void saveClicked();
    virtual void updateClicked();

private:
    long    myDomainID;

};
#endif // DomainChecklist_included
