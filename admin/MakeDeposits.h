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


#ifndef MakeDeposits_included
#define MakeDeposits_included

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3datetimeedit.h>

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

// vim: expandtab
