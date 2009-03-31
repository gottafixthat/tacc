/*
** $Id: PKG_General.h,v 1.3 2004/02/27 01:33:33 marc Exp $
**
***************************************************************************
**
** PKG_General.h - Package Editor general tab.
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
** $Log: PKG_General.h,v $
** Revision 1.3  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.2  2003/12/29 23:11:32  marc
** Removed QTArch dialogs, doing them manually.
**
**
*/


#ifndef PKG_General_included
#define PKG_General_included

#include <qwidget.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3multilineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <TAAWidget.h>

class PKG_General : public TAAWidget
{
    Q_OBJECT

public:

    PKG_General
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_General();

    void    setPackageID(long newPackageID);

protected:
    QLineEdit       *itemName;
    Q3MultiLineEdit  *description;
    QCheckBox       *isActive;
    QCheckBox       *showIncluded;
    QCheckBox       *isPrivate;
    QComboBox       *glAccount;
    QSpinBox        *numMailboxes;
    QPushButton     *saveButton;

protected slots:
    void            checkForSave(void);
    virtual void    nameChanged(const QString &)    { checkForSave(); }
    virtual void    descriptionChanged()            { checkForSave(); }
    virtual void    activeChanged(bool)             { checkForSave(); }
    virtual void    privateChanged(bool)            { checkForSave(); }
    virtual void    showIncludedChanged(bool)       { checkForSave(); }
    virtual void    glAccountChanged(int)           { checkForSave(); }
    virtual void    save();
    void            numMailboxesChanged(int)        { checkForSave(); }
    
private:
    long        myPackageID;
    char        *intItemName;
    char        *intItemDesc;
    int         intItemActive;
    int         intItemPrivate;
    int         intShowIncluded;
    int         intGLAccount;
    int         intNumMailboxes;
    int         *glAcctIDX;
    

signals:
    void        packageChanged();

};
#endif // PKG_General_included
