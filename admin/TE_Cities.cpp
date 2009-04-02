/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>

#include <BlargDB.h>

#include <TE_Cities.h>

using namespace Qt;

TE_Cities::TE_Cities(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
    allCitiesButton = new QCheckBox("Send to customers in all cities", this, "allCitiesButton");
    connect(allCitiesButton, SIGNAL(clicked()), this, SLOT(allCitiesClicked()));

    cityList = new Q3ListView(this, "cityList");
    cityList->addColumn("City");
    cityList->addColumn("State");
    cityList->setAllColumnsShowFocus(TRUE);
    cityList->setMultiSelection(TRUE);
    
    // Basic layout, box top to bottom.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(allCitiesButton, 0);
    ml->addWidget(cityList, 1);


    loadCities();
    
    
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
    QApplication::setOverrideCursor(Qt::waitCursor);
    
    ADB     DB;
    
    DB.query("select distinct City, State from Addresses where RefFrom = %d", REF_CUSTOMER);
    if (DB.rowCount) while (DB.getrow()) {
        (void) new Q3ListViewItem(cityList, DB.curRow["City"], DB.curRow["State"]);
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
    Q3ListViewItem   *curItem = NULL;

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



// vim: expandtab
