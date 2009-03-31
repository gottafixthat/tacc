/*
** $Id: PKG_Billables.h,v 1.2 2003/12/30 01:38:28 marc Exp $
**
***************************************************************************
**
** PKG_Billables.h - Billable items included in a package.
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
** $Log: PKG_Billables.h,v $
** Revision 1.2  2003/12/30 01:38:28  marc
** Removed QTArch dialogs.
**
**
*/

#ifndef PKG_Billables_included
#define PKG_Billables_included

#include <qwidget.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <TAAWidget.h>

class PKG_Billables : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Billables
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Billables();

    void    setPackageID(long newPackageID);

protected:
    Q3ListView   *billableList;
    QComboBox   *itemList;
    QLineEdit   *description;
    QSpinBox    *qtySpin;
    QSpinBox    *groupSpin;
    QPushButton *removeButton;
    QPushButton *addButton;
    QPushButton *saveButton;

protected slots:
    void         checkForSave();
    virtual void billableSelected(Q3ListViewItem *curItem);
    virtual void save();
    virtual void addBillable();
    virtual void removeBillable();
    virtual void billableItemChanged(int)               { checkForSave(); }
    virtual void descriptionChanged(const QString &)    { checkForSave(); }
    virtual void quantityChanged(int)                   { checkForSave(); }
    virtual void groupChanged(int)                      { checkForSave(); }

private:
    long    myPackageID;
    long    *billItemIDX;
    
    long    intBillable;
    char    *intDescr;
    int     intQty;
    int     intGroup;
};
#endif // PKG_Billables_included
