/**
 * GLAccountEditor.h - Allows a manager or administrator to edit or create
 * a new account in the Chart of Accounts.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#ifndef GLACCOUNTEDITOR_H
#define GLACCOUNTEDITOR_H

#include <qlineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>

#include <TAAWidget.h>

class GLAccountEditor : public TAAWidget
{
    Q_OBJECT

public:

    GLAccountEditor
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int AcctNo = 0
    );

    virtual ~GLAccountEditor();

signals:
    void refresh(int);

protected:
    QLineEdit   *acctName;
    QComboBox   *subAcctOf;
    QComboBox   *acctType;
    QSpinBox    *acctNumber;
    QLabel      *balance;
    QLabel      *transCount;
    QLineEdit   *taxLine;

    void        setAccountNo(int);

private slots:
    void         acctTypeChanged(int newIDX);
    virtual void cancelAccount();
    virtual void saveAccount();
    
private:
    int myAccountNo;
    int *AcctNoIndex;      // for our combo box.
    int IdxID;
};
#endif
