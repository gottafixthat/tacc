/**
 * CityFilters.h - Filters for the Customer City Report.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qbuttongroup.h>
#include <qlayout.h>

#include <CityFilters.h>

CityFilters::CityFilters(QWidget* parent, const char* name) : 
    ReportFilter( parent, name )
{
	setCaption( "Customer City Report Filters" );

    QButtonGroup    *bgroup = new QButtonGroup(1, Qt::Horizontal, "Active Customer Selection", this, "bgroup");

    activeOnlyButton = new QRadioButton(bgroup, "activeOnlyButton");
    activeOnlyButton->setText("Active Only");
    activeOnlyButton->setChecked(true);

    inactiveOnlyButton = new QRadioButton(bgroup, "inactiveOnlyButton");
    inactiveOnlyButton->setText("Inactive Only");

    bothButton = new QRadioButton(bgroup, "bothButton");
    bothButton->setText("Both");

    bgroup->insert(activeOnlyButton);
    bgroup->insert(inactiveOnlyButton);
    bgroup->insert(bothButton);


    ml->addWidget(bgroup, 1);
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


