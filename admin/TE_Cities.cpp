/*
** $Id: TE_Cities.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** TE_Cities - Targeted Email - City selection.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: TE_Cities.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "TE_Cities.h"

#include <qapplication.h>

#include "BlargDB.h"

#define Inherited TE_CitiesData

TE_Cities::TE_Cities
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    
    loadCities();
    
    cityList->setAllColumnsShowFocus(TRUE);
    cityList->setMultiSelection(TRUE);
    
    allCitiesButton->setChecked(TRUE);
    allCitiesClicked();
}


TE_Cities::~TE_Cities()
{
}


/*
** loadCities - Loads the list of cities into memory.
*/

void TE_Cities::loadCities()
{
    QApplication::setOverrideCursor(waitCursor);
    
    ADB     DB;
    
    DB.query("select distinct City, State from Addresses where RefFrom = %d", REF_CUSTOMER);
    if (DB.rowCount) while (DB.getrow()) {
        (void) new QListViewItem(cityList, DB.curRow["City"], DB.curRow["State"]);
    }
    
    QApplication::restoreOverrideCursor();
}


/*
** allCitiesClicked  - Gets called when the AllCities button is clicked.
*/

void TE_Cities::allCitiesClicked()
{
    if (allCitiesButton->isChecked()) {
        cityList->setEnabled(FALSE);
    } else {
        cityList->setEnabled(TRUE);
    }
}

/*
** allCities - Returns 1 if all cities are selected, else, 0.
*/

int TE_Cities::allCities()
{
    return (int) allCitiesButton->isChecked();
}

/*
** isIncluded  - Returns 1 if the specified city is to be mailed.
*/

int TE_Cities::isIncluded(const char *City)
{
    int             RetVal = 0;
    QListViewItem   *curItem = NULL;

    if (allCitiesButton->isChecked()) {
        RetVal = 1;
    } else {
        // Scan through the list of cities and see if we should send 
        // email to this customer.
        for (curItem = cityList->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
            if (!strcmp(curItem->key(0, 0), City)) {
                if (curItem->isSelected()) RetVal = 1;
            }
        }
    }
    
    return RetVal;
}



