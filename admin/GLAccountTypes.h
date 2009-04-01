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

#ifndef GLACCOUNTTYPES_H
#define GLACCOUNTTYPES_H

#include <QtGui/QMenuBar>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include <GenLedger.h>

class GLAccountTypeEditor : public TAAWidget
{
    Q_OBJECT

public:
    GLAccountTypeEditor
    (
     int acctType,
     QWidget *parent = NULL,
     const char *name = NULL
    );
    virtual ~GLAccountTypeEditor();

    void        setAccountType(int);

signals:
    void    refresh(int);

protected slots:
    void        saveClicked();
    void        cancelClicked();

protected:
    QLineEdit   *description;
    int         myAccountType;

};

class GLAccountTypes : public TAAWidget
{
    Q_OBJECT

public:

    GLAccountTypes
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~GLAccountTypes();
    
signals:
    void refreshAcctTypeList(int);
    
public slots:
    void refreshList(int);

protected slots:
    void newAcctType();
    void editAcctType();
    void deleteAcctType();
    void closeClicked();

protected:
    int         accountTypeCol;
    QMenuBar    *menu;
    Q3ListView   *acctTypeList;

    GLAccountTypeList   glAcctTypes;
};

#endif

// vim: expandtab
