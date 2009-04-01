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

#ifndef ELT_General_included
#define ELT_General_included

#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>

#include <TAAWidget.h>

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
    Q3MultiLineEdit  *description;
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

// vim: expandtab
