/*
** $Id: CallTypeSelector.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CallTypeSelector - A set of widgets that gets what type of incoming
**                    call is being taken.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CallTypeSelector.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef CALLTYPESELECTOR_H 
#define CALLTYPESELECTOR_H

#include <TAAWidget.h>
#include <CustomerSearch.h>
#include <qwidget.h>
#include <qradiobutton.h>
#include <qlistview.h>
#include <qtimer.h>
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

    QListView       *contactList;
    //QListView       *ticketList;
    CustomerTickets *ticketList;
};

#endif // CALLTYPESELECTOR_H
