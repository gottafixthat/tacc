/*
** $Id: ELT_Flags.h,v 1.2 2003/12/22 23:46:48 marc Exp $
**
***************************************************************************
**
** ELT_Flags - Edit Login Types - Flags tab.
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
** $Log: ELT_Flags.h,v $
** Revision 1.2  2003/12/22 23:46:48  marc
** Removed QTArchitect created files from ELT_Flags
**
**
*/


#ifndef ELT_Flags_included
#define ELT_Flags_included

#include <qwidget.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qstring.h>
#include <TAAWidget.h>

class ELT_Flags : public TAAWidget
{
    Q_OBJECT

public:

    ELT_Flags
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ELT_Flags();

    void    setLoginTypeID(long newLoginTypeID);
    
protected:
    QListView       *flagList;
    QLineEdit       *flagValue;
    QPushButton     *setValueButton;
    QPushButton     *addButton;
    QPushButton     *removeButton;

protected slots:
    virtual void removeFlag();
    virtual void addFlag();
    virtual void itemSelected(QListViewItem *curItem);
    virtual void flagValueChanged(const QString &);
    virtual void setValue();
    
private:
    long    myLoginTypeID;

};
#endif // ELT_Flags_included
