/**********************************************************************

	--- Qt Architect generated file ---

	File: CityFilters.cpp
	Last generated: Sun Apr 11 13:58:51 1999

 *********************************************************************/

#include "CityFilters.h"

#define Inherited CityFiltersData

CityFilters::CityFilters
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Customer City Report Filters" );
}


CityFilters::~CityFilters()
{
}

/*
** setActiveStatus - Sets which button is checked in the button group.
*/

void CityFilters::setActiveStatus(int newStatus)
{
    switch (newStatus) {
        case    1:
            activeOnlyButton->setChecked(0);
            inactiveOnlyButton->setChecked(1);
            bothButton->setChecked(0);
            break;
            
        case    2:
            activeOnlyButton->setChecked(0);
            inactiveOnlyButton->setChecked(0);
            bothButton->setChecked(1);
            break;
            
        default:
            activeOnlyButton->setChecked(1);
            inactiveOnlyButton->setChecked(0);
            bothButton->setChecked(0);
            break;
    }
}

/*
** getActiveStatus - Returns 0, 1, 2 depending on ActiveOnly, InactiveOnly
**                   or Both.
*/

int CityFilters::getActiveStatus(void)
{
    int RetVal  = 0;
    
    if (inactiveOnlyButton->isChecked()) RetVal = 1;
    if (bothButton->isChecked()) RetVal = 2;
    
    return RetVal;
}


/*
** cancelFilters - Closes the filter window and does nothing else.
*/

void CityFilters::cancelFilters()
{
    close();
}


/*
** applyFilters - Emits the updateReport signal and nothing else.
*/

void CityFilters::applyFilters()
{
    emit(filtersChanged(getActiveStatus()));
}


/*
** applyAndExit - Emits the updateReport signal and closes the filters
**                dialog.
*/

void CityFilters::applyAndExit()
{
    emit(filtersChanged(getActiveStatus()));
    close();
}


