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

#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>

#include <TE_LoginTypes.h>

using namespace Qt;

TE_LoginTypes::TE_LoginTypes(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{

    allLoginsButton = new QCheckBox("Send to all Login Types", this, "allLoginsButton");
    connect(allLoginsButton, SIGNAL(clicked()), this, SLOT(allLoginsClicked()));

    includeInactive = new QCheckBox("Include inactive accounts", this, "includeInactive");

    primaryOnlyCheckbox = new QCheckBox("Send to Primary Logins only", this, "primaryOnlyChckebox");

    loginTypeList = new Q3ListView(this, "loginTypeList");
    loginTypeList->addColumn("Type");
    loginTypeList->addColumn("Description");
    loginTypeList->addColumn("Inactive");
    loginTypeList->setColumnAlignment(2, Qt::AlignRight);
    loginTypeList->addColumn("Active");
    loginTypeList->setColumnAlignment(3, Qt::AlignRight);


    // Create the layout.  A simple box layout will do.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 1, 1);
    ml->addWidget(allLoginsButton, 0);
    ml->addWidget(includeInactive, 0);
    ml->addWidget(primaryOnlyCheckbox, 0);
    ml->addWidget(loginTypeList, 1);

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
    QApplication::setOverrideCursor(Qt::waitCursor);
    
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
        
        (void) new Q3ListViewItem(loginTypeList,
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
    Q3ListViewItem   *curItem = NULL;
    
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



// vim: expandtab
