/*
** $Id$
**
***************************************************************************
**
** TE_LoginTypes - Targeted Email - Login Types Filter
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
** $Log: TE_LoginTypes.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "TE_LoginTypes.h"

#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#define Inherited TE_LoginTypesData

TE_LoginTypes::TE_LoginTypes
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    loadLoginTypes();
    loginTypeList->setAllColumnsShowFocus(TRUE);
    loginTypeList->setMultiSelection(TRUE);
    
    allLoginsButton->setChecked(TRUE);
    allLoginsClicked();
}


TE_LoginTypes::~TE_LoginTypes()
{
}

/*
** loadLoginTypes - This loads the login type list into memory so the
**                  user can select which login types to send to.
*/

void TE_LoginTypes::loadLoginTypes()
{
    QApplication::setOverrideCursor(waitCursor);
    
    ADB     DB;
    ADB     DB2;
    
    char    iCount[1024];
    char    aCount[1024];
    
    DB.query("select InternalID, LoginType, Description from LoginTypes");
    if (DB.rowCount) while (DB.getrow()) {
        DB2.query("select InternalID from Logins where LoginType = %ld and Active = 0 and Wiped < '1970-01-01'", atol(DB.curRow["InternalID"]));
        sprintf(iCount, "%ld", DB2.rowCount);
        DB2.query("select InternalID from Logins where LoginType = %ld and Active = 1", atol(DB.curRow["InternalID"]));
        sprintf(aCount, "%ld", DB2.rowCount);
        
        (void) new QListViewItem(loginTypeList,
          DB.curRow["LoginType"],
          DB.curRow["Description"],
          iCount,
          aCount,
          DB.curRow["InternalID"]
        );
    }
    
    QApplication::restoreOverrideCursor();
}

/*
** allLoginsClicked - A slot that gets called when the Send to All login
**                    types button is checked.
*/

void TE_LoginTypes::allLoginsClicked()
{
    if (allLoginsButton->isChecked()) {
        loginTypeList->setEnabled(FALSE);
    } else {
        loginTypeList->setEnabled(TRUE);
    }
}


/*
** isIncluded  - Returns 1 if we should send to this login type, or
**               0 if we should not.
*/

int TE_LoginTypes::isIncluded(long LoginType, int Active)
{
    int             RetVal = 0;
    QListViewItem   *curItem = NULL;
    
    if (allLoginsButton->isChecked()) {
        if (!Active) {
            if (includeInactive->isChecked()) RetVal = 1; 
        } else {
            RetVal = 1;
        }
    } else {
        // Scan through the list and see if we should send mail to this
        // login type.
        for (curItem = loginTypeList->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
            if (atol(curItem->key(4,0)) == LoginType) {
                if (curItem->isSelected()) RetVal = 1;
            }
        }
    }
    
    return RetVal;
}



