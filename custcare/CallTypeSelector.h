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

#ifndef CALLTYPESELECTOR_H 
#define CALLTYPESELECTOR_H

#include <QtCore/QTimer>
#include <QtGui/QRadioButton>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include <CustomerSearch.h>
#include <CustomerTickets.h>

class CallTypeSelector : public TAAWidget
{
    Q_OBJECT

public:
    CallTypeSelector( QWidget *parent = NULL, const char *name = NULL);
    virtual ~CallTypeSelector();

protected slots:
    void            delayedRefresh(long);
    void            refresh();

protected:
    long            refreshCustID;
    QTimer          *refreshTimer;
    QRadioButton    *typeUnsolicited;
    QRadioButton    *typeSupport;
    QRadioButton    *typeSales;
    QRadioButton    *typeAddService;
    QRadioButton    *typeAccounting;
    QRadioButton    *typeTicket;

    CustomerSearch  *custSearch;

    Q3ListView       *contactList;
    //QListView       *ticketList;
    CustomerTickets *ticketList;
};

#endif // CALLTYPESELECTOR_H

// vim: expandtab
