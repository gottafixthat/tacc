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

#ifndef BI_General_included
#define BI_General_included

#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>

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


// vim: expandtab
