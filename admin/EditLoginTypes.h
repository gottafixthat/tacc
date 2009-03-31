/*
** $Id: EditLoginTypes.h,v 1.3 2003/12/24 01:09:22 marc Exp $
**
***************************************************************************
**
** EditLoginTypes - Allows the administrator to edit the login types.
**                  Creates the main window for this and loads the list
**                  of login types to be edited.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: EditLoginTypes.h,v $
** Revision 1.3  2003/12/24 01:09:22  marc
** Minor changes
**
** Revision 1.2  2003/12/21 20:11:11  marc
** Removed QTArchitecht created dialogs and did them by hand.
**
**
*/


#ifndef EditLoginTypes_included
#define EditLoginTypes_included


#include <qwidget.h>
#include <q3widgetstack.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qtabbar.h>

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
