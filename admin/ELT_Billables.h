/*
** $Id: ELT_Billables.h,v 1.2 2003/12/23 00:29:46 marc Exp $
**
***************************************************************************
**
** ELT_Billables - Edit Login Types Billable Items tab.
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
** $Log: ELT_Billables.h,v $
** Revision 1.2  2003/12/23 00:29:46  marc
** Removed QTArchitect created dialogs and created them manually.
**
**
*/


#ifndef ELT_Billables_included
#define ELT_Billables_included

#include <qwidget.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <TAAWidget.h>

class ELT_Billables : public TAAWidget
{
    Q_OBJECT

public:

    ELT_Billables
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ELT_Billables();

    void    setLoginTypeID(long newLoginTypeID);

protected:
    Q3ListView   *billableList;
    QPushButton *addButton;
    QPushButton *removeButton;

protected slots:
    virtual void itemSelected(Q3ListViewItem *curItem);
    virtual void removeBillable();
    virtual void addBillable();
    
private:
    long    myLoginTypeID;

};
#endif // ELT_Billables_included
