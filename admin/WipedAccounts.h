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


#ifndef WIPEDACCOUNTS_H
#define WIPEDACCOUNTS_H

#include <QtCore/QDateTime>
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <Qt3Support/q3listview.h>

#include <BlargDB.h>
#include <TAAWidget.h>

class WipedAccounts : public TAAWidget
{
    Q_OBJECT

public:

    WipedAccounts(QWidget* parent = NULL, const char* name = NULL);
    virtual ~WipedAccounts();
    void    fillList(void);

protected:
    long    totAccts;
    long    selAccts;
    float   totOverdue;
    float   totSelected;
    
    int             myAccountTypes;
    Q3ListView       *list;
    QLabel          *accountsOverdue;
    QLabel          *accountsSelected;
    QLabel          *selectedOverdue;
    QLabel          *amountOverdue;
    QRadioButton    *noAction;
    QRadioButton    *sendFinal;
    QRadioButton    *sendToCollection;

protected slots:
    virtual void processSelections();
    virtual void cancelOverdue();
    virtual void selectNone();
    virtual void selectAll();
    virtual void editCustomer(Q3ListViewItem *curItem);
    virtual void updateTotals();
    virtual void sendToCollections(void);
    
};


#endif

// vim: expandtab
