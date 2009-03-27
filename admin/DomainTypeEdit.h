/*
** $Id: DomainTypeEdit.h,v 1.2 2004/01/10 01:53:15 marc Exp $
**
***************************************************************************
**
** DomainTypeEdit - Allows the admin to edit the domain types.
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
** $Log: DomainTypeEdit.h,v $
** Revision 1.2  2004/01/10 01:53:15  marc
** Re-written Domain type editor.  Still needs some work, but is getting there.
**
**
*/

#ifndef DomainTypeEdit_Included 
#define DomainTypeEdit_Included

#include <qwidget.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <TAAWidget.h>

class DomainTypeEdit : public TAAWidget
{
    Q_OBJECT

public:

    DomainTypeEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~DomainTypeEdit();

public slots:
    virtual void refreshList(void);
    virtual void refreshBillablesList(int);

protected slots: 
    virtual void itemSelected(QListViewItem *);
    virtual void addBillable();
    virtual void removeBillable();
    virtual void editDomainType()               {};
    virtual void newDomainType()                {};
    virtual void deleteDomainType()             {};
    virtual void saveDomainType();

protected:
    QListView   *domainTypeList;
    QLineEdit   *domainType;
    QListBox    *billablesList;
    QPushButton *newButton;
    QPushButton *delButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QCheckBox   *allowEmail;
    QCheckBox   *allowHosting;
    QComboBox   *dnsTemplate;
    QPushButton *saveButton;

    long        *templateIDX;

};
#endif // DomainTypeEdit_included
