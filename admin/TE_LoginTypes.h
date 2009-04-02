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

#ifndef TE_LOGINTYPES_H
#define TE_LOGINTYPES_H

#include <QtGui/QCheckBox>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

/*!
 * \brief Part of the Targeted Email class, this allows the user to select 
 * what login types to send to.
 *
 */
class TE_LoginTypes : public TAAWidget
{
    Q_OBJECT

public:

    TE_LoginTypes(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TE_LoginTypes();

    void    loadLoginTypes();
    
    int     isIncluded(long LoginType, int Active);
    
    bool    primaryOnly() { return primaryOnlyCheckbox->isChecked(); }

protected slots:
    virtual void allLoginsClicked();
    virtual void closeEvent(QCloseEvent *)       { delete this; }

protected:
    QCheckBox   *allLoginsButton;
    QCheckBox   *includeInactive;
    QCheckBox   *primaryOnlyCheckbox;
    Q3ListView   *loginTypeList;

};

#endif
// vim: expandtab

