/*
** $Id: DomainAdd.h,v 1.2 2004/01/10 21:59:03 marc Exp $
**
***************************************************************************
**
** DomainAdd - Allows the user to add a new domain entry for the customer.
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
** $Log: DomainAdd.h,v $
** Revision 1.2  2004/01/10 21:59:03  marc
** More changes to DomainAdd.  Auto widget enables, etc.  Not much left to do.
**
** Revision 1.1  2004/01/09 01:55:05  marc
** New widget to include the framework for instant domain, email and web
** hosting setups.
**
**
*/


#ifndef DomainAdd_included
#define DomainAdd_included

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <TAAWidget.h>

class DomainAdd : public TAAWidget
{
    Q_OBJECT

public:

    DomainAdd
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0
    );

    virtual ~DomainAdd();
    
public slots:
    virtual void saveDomain();
    virtual void cancelDomain();
    
protected slots:
    void        typeChanged(int);

protected:
    QLabel      *customerID;
    QLabel      *customerName;
    QComboBox   *domainType;
    QComboBox   *loginID;
    QLineEdit   *domainName;
    QCheckBox   *autoDNS;
    QCheckBox   *autoAliases;
    QCheckBox   *autoHosting;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void        setMostPopular();

private:
    long        myCustID;
    int         *domainTypeIDX;
    
signals:
    void        domainSaved(int);

};

#endif // DomainAdd_included
