/*
** $Id: MakeDeposits.h,v 1.2 2004/01/22 20:59:17 marc Exp $
**
***************************************************************************
**
** MakeDeposits  - Allows the user to select money that is currently
**                 sitting in the undeposited funds account and transfer
**                 it into a checking account.
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
** $Log: MakeDeposits.h,v $
** Revision 1.2  2004/01/22 20:59:17  marc
** Functional.
**
**
*/


#ifndef MakeDeposits_included
#define MakeDeposits_included

#include <qdialog.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <TAAWidget.h>

class MakeDeposits : public TAAWidget
{
    Q_OBJECT

public:

    MakeDeposits
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~MakeDeposits();
    
    void    fillDepositList();

protected:
    QLabel      *undepositedAmount;
    QLabel      *amountSelected;
    QListView   *paymentList;
    QComboBox   *targetAccountList;
    float       selTotal;

protected slots:
    virtual void cancelSelected()                       { close();     }
    virtual void processSelections();
    virtual void itemSelected();
    virtual void itemDoubleClicked(QListViewItem *);
    virtual void selectNone();
    virtual void selectAll();

private:
    long    *accountIDX;

};
#endif // MakeDeposits_included
