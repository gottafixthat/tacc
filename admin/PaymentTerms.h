/**
 * PaymentTerms.h - Class definition for PaymentTerms which allows the
 * user to view and edit the payment terms in TACC.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef PaymentTerms_included
#define PaymentTerms_included

#include <qlistview.h>
#include <qmenubar.h>

#include <TAAWidget.h>

class PaymentTerms : public TAAWidget
{
    Q_OBJECT

public:

    PaymentTerms(QWidget* parent = NULL, const char* name = NULL);
    virtual ~PaymentTerms();

signals:
    void refreshPaymentTermsList(int);
    
public slots:
    virtual void closeClicked();
    virtual void refreshList(int);

protected:
    QListView       *termsList;
    QMenuBar        *menu;

protected slots:
    virtual void newPaymentTerms();
    virtual void editPaymentTerms();
    virtual void editPaymentTermsL(int msg = 0);
    virtual void deletePaymentTerms();
    virtual void editTerms(QListViewItem *);

private:
    int     intIDCol;

};

#endif
