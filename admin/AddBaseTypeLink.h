/**********************************************************************

	--- Qt Architect generated file ---

	File: AddBaseTypeLink.h
	Last generated: Sat Dec 13 05:27:12 1997

 *********************************************************************/

#ifndef AddBaseTypeLink_included
#define AddBaseTypeLink_included

#include "AddBaseTypeLinkData.h"

class AddBaseTypeLink : public AddBaseTypeLinkData
{
    Q_OBJECT

public:

    AddBaseTypeLink
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int ItemNo = 0
    );

    virtual ~AddBaseTypeLink();

private:
	int		*baseItemIDX;
	int		myItemNo;
	
private slots:
	virtual void	itemSelected(int ItemNo);
	virtual void	acceptSelected();

};
#endif // AddBaseTypeLink_included
