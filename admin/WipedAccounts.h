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

#include <BlargDB.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qfont.h>
#include <qdatetime.h>
#include <qrect.h>
#include <qcolor.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qradiobutton.h>

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
    QListView       *list;
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
    virtual void editCustomer(QListViewItem *curItem);
    virtual void updateTotals();
    virtual void sendToCollections(void);
    
};


#endif

// vim: expandtab
