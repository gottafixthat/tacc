/*
** $Id: ELT_General.h,v 1.3 2004/02/27 01:33:33 marc Exp $
**
***************************************************************************
**
** ELT_General - The Edit Login Types general tab.
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
** $Log: ELT_General.h,v $
** Revision 1.3  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.2  2003/12/21 18:32:29  marc
** Removed the old dialog editor layout code and did the layout manually.
** Added the ability to edit the LoginClass.
**
**
*/


#ifndef ELT_General_included
#define ELT_General_included

#include <TAAWidget.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qmultilineedit.h>

class ELT_General : public TAAWidget
{
    Q_OBJECT

public:

    ELT_General
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ELT_General();

    void    setLoginTypeID(long newLoginTypeID);

protected slots:
    void    checkForSave(void);
    virtual void    nameChanged(const QString &)    { checkForSave(); }
    virtual void    descriptionChanged()            { checkForSave(); }
    virtual void    activeChanged(bool)             { checkForSave(); }
    virtual void    diskSpaceChanged(int)           { checkForSave(); }
    virtual void    dialupChannelsChanged(int)      { checkForSave(); }
    virtual void    additionalMailboxesChanged(int) { checkForSave(); }
    virtual void    diskBillableChanged(int)        { checkForSave(); }
    virtual void    channelBillableChanged(int)     { checkForSave(); }
    virtual void    loginClassChanged(int)          { checkForSave(); }

    virtual void    save();
    
    void            loadBillableLists();
   
protected:
    QMultiLineEdit  *description;
    QCheckBox       *isActive;
    QSpinBox        *diskSpace;
    QSpinBox        *dialupChannels;
    QSpinBox        *additionalMailboxes;
    QPushButton     *saveButton;
    QLineEdit       *itemName;
    QComboBox       *diskBillableList;
    QComboBox       *channelBillableList;
    QComboBox       *loginClassList;

private:
    long    myLoginTypeID;

    char            *intItemName;
    char            *intItemDesc;
    int             intItemActive;
    int             intItemSpace;
    int             intItemChan;
    int             intAddMail;
    long            intDiskBill;
    long            intChanBill;
    char            intLoginClass[256];

    long            *bListIDX;
    

signals:
    void    loginTypeChanged();

};
#endif // ELT_General_included
