/*
** $Id: UserPrivs.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** UserPrivs.h   - UserPrivs is the widget that will allow the system
**                 administrator to create and delete accounts, as well as
**                 change their passwords and such.
**
***************************************************************************
** Written by R. Marc Lewis, 
**     under contract to The Public Defender Association
***************************************************************************
*/


#ifndef UserPrivs_included
#define UserPrivs_included

#include <qevent.h>
#include <qwidget.h>
#include <q3widgetstack.h>
#include <q3listview.h>
#include <TAATools.h>
#include <TAAWidget.h>

#include "UserEditor.h"

class UserPrivs : public TAAWidget
{
    Q_OBJECT

public:

    UserPrivs(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~UserPrivs();
    
    void    init();

protected slots:
    virtual void userSelected(Q3ListViewItem*);
    virtual void refreshList();
    virtual void userUpdated(long);

protected:
    Q3ListView       *userList;
    UserEditor      *editArea;    

    long            myCurrentID;

};
#endif // UserPrivs_included


