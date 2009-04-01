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


#ifndef EditLoginTypes_included
#define EditLoginTypes_included


#include <QtGui/QPushButton>
#include <QtGui/QTabBar>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include <ELT_General.h>
#include <ELT_Flags.h>
#include <ELT_Billables.h>

class EditLoginTypes : public TAAWidget 
{
    Q_OBJECT

public:

    EditLoginTypes
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~EditLoginTypes();

protected slots:
    virtual void loginTypeSelected(Q3ListViewItem *curItem);
    void    updateLoginType();

    virtual void newLoginType();
    virtual void deleteLoginType();    

protected:
    QPushButton     *deleteButton;
    Q3ListView       *loginTypeList;
    QTabBar         *theTabBar;
    Q3WidgetStack    *qws;

private:
    ELT_General     *tgeneral;
    ELT_Flags       *tflags;
    ELT_Billables   *tbillables;

    void            loadLoginTypes();

};
#endif // EditLoginTypes_included

// vim: expandtab
