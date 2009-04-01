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

#ifndef DomainTypeEdit_Included 
#define DomainTypeEdit_Included

#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3listbox.h>

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
    virtual void itemSelected(Q3ListViewItem *);
    virtual void addBillable();
    virtual void removeBillable();
    virtual void editDomainType()               {};
    virtual void newDomainType()                {};
    virtual void deleteDomainType()             {};
    virtual void saveDomainType();

protected:
    Q3ListView   *domainTypeList;
    QLineEdit   *domainType;
    Q3ListBox    *billablesList;
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

// vim: expandtab
