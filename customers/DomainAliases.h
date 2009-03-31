/*
** $Id: DomainAliases.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** DomainAliases - Allows the operator to view and change the aliases for
**                 a customer's domains.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2002, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DomainAliases.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef DomainAliases_included
#define DomainAliases_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <q3listview.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>

class DomainAliases : public TAAWidget
{
    Q_OBJECT

public:
    DomainAliases (QWidget *parent = NULL, const char *name = NULL);
    ~DomainAliases();

    void setCustomerID(long);

protected:
    Q3ListView   *aliasList;
    QLineEdit   *address;
    QComboBox   *domainList;
    QComboBox   *loginList;
    QPushButton *addButton;
    QPushButton *refreshButton;
    QPushButton *autoButton;
    QPushButton *delButton;
    QPushButton *closeButton;
    long        myCustID;
    int         virtIDCol;

protected slots:
    void        refreshAlias();
    void        addAlias();
    void        delAlias();
    void        autoAlias();
};


#endif // DomainAliases_included

