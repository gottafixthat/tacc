/**********************************************************************

	--- Qt Architect generated file ---

	File: TE_LoginTypes.h
	Last generated: Tue Jul 27 12:19:36 1999

 *********************************************************************/

#ifndef TE_LoginTypes_included
#define TE_LoginTypes_included

#include "TE_LoginTypesData.h"

class TE_LoginTypes : public TE_LoginTypesData
{
    Q_OBJECT

public:

    TE_LoginTypes
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TE_LoginTypes();

    void    loadLoginTypes();
    
    int     isIncluded(long LoginType, int Active);
    
    bool    primaryOnly() { return primaryOnlyCheckbox->isChecked(); }

protected slots:
    virtual void allLoginsClicked();
    virtual void closeEvent(QCloseEvent *)       { delete this; }


};
#endif // TE_LoginTypes_included
