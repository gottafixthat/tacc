/**
 * MakeDeposits.h - Allows the user to select funds that are currently
 * sitting in the Undeposited Funds account and transfer it into a cash
 * account.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */


#ifndef MakeDeposits_included
#define MakeDeposits_included

#include <qdialog.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qcombobox.h>
#include <q3datetimeedit.h>
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
    QLabel      *selectedItemCount;
    QLabel      *undepositedAmount;
    QLabel      *amountSelected;
    Q3ListView   *paymentList;
    Q3DateEdit   *transDate;
    QComboBox   *targetAccountList;
    float       selTotal;

protected slots:
    virtual void cancelSelected()                       { close();     }
    virtual void processSelections();
    virtual void itemSelected();
    virtual void itemDoubleClicked(Q3ListViewItem *);
    virtual void selectNone();
    virtual void selectAll();
    virtual void printSelected();

private:
    long    *accountIDX;

};
#endif // MakeDeposits_included
