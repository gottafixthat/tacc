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


#ifndef GLACCOUNTEDITOR_H
#define GLACCOUNTEDITOR_H

#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>

#include <TAAWidget.h>
#include <GenLedger.h>

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

    QLineEdit   *accountNo;
    QLineEdit   *acctName;
    QComboBox   *parentAcct;
    QComboBox   *accountType;
    QLineEdit   *providerAcctNo;
    QLineEdit   *taxLine;
    QLabel      *balance;
    QLabel      *transCount;

    void        setIntAccountNo(int);
    void        loadParentList();

private slots:
    void         accountTypeChanged(int newIDX);
    virtual void cancelAccount();
    virtual void saveAccount();
    
private:
    GLAccountTypeList   glAcctTypes;
    int *acctTypeIDX;
    int myIntAccountNo;
    int *intAcctNoIndex;        // for our combo box.
    int *parentIDX;             // For our combo box.
    int IdxID;
};
#endif

// vim: expandtab
