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

#ifndef PKG_General_included
#define PKG_General_included

#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>

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

// vim: expandtab
