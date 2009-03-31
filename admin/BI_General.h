/*
** $Id: BI_General.h,v 1.4 2004/02/27 23:11:18 marc Exp $
**
***************************************************************************
**
** BI_General - The General tab of Billable Items editor.
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
** $Log: BI_General.h,v $
** Revision 1.4  2004/02/27 23:11:18  marc
** Fixed the way we're doing mailbox counts.  It now keys off of subscriptions.
**
** Revision 1.3  2003/12/27 21:37:29  marc
** Switched a couple char variables to static sizes.
**
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
**
*/


#ifndef BI_General_included
#define BI_General_included

#include <qwidget.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <q3multilineedit.h>
#include <TAAWidget.h>

class BI_General : public TAAWidget
{
    Q_OBJECT

public:

    BI_General
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BI_General();

    void setItemNumber(long itemNumber);
    
protected slots:
    virtual void    nameChanged(const QString &)    { checkForSave(); }
    virtual void    descriptionChanged()            { checkForSave(); }
    virtual void    itemTypeChanged(int)            { checkForSave(); }
    virtual void    accountChanged(int)             { checkForSave(); }
    virtual void    setupItemChanged(int)           { checkForSave(); }
    virtual void    priorityChanged(int)            { checkForSave(); }
    virtual void    activeChanged(bool)             { checkForSave(); }
    virtual void    taxableChanged(bool)            { checkForSave(); }
    virtual void    mailboxesChanged(int)           { checkForSave(); }
    virtual void    save();

protected:
    Q3MultiLineEdit  *description;
    QCheckBox       *isActive;
    QPushButton     *saveButton;
    QLineEdit       *itemName;
    QCheckBox       *isTaxable;
    QComboBox       *accountList;
    QComboBox       *setupItemList;
    QSpinBox        *prioritySpin;
    QSpinBox        *mailboxesSpin;
    QComboBox       *itemTypeList;

    void loadItemTypeList();
    void loadAccountList();
    void loadSetupItemList();
    
    void checkForSave();

private:
    long        myItemNumber;
    long        *acctIDX;
    long        *setupIDX;
    
    char        myItemName[1024];
    char        myItemDesc[8192];
    int         myItemType;
    long        myAccount;
    long        mySetupItem;
    int         myPriority;
    int         myMailboxes;
    int         myActive;
    int         myTaxable;
    
signals:
    void        itemChanged();

};
#endif // BI_General_included

