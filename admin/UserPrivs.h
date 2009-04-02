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

#ifndef UserPrivs_included
#define UserPrivs_included

#include <QtCore/QEvent>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3listview.h>

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



// vim: expandtab

