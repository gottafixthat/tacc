/*
** $Id: CustomerTickets.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** CustomerTickets.h - A widget that manages the list of open for a
**                     particular customer.
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
*/

#ifndef CustomerTickets_included
#define CustomerTickets_included

#include <TAAWidget.h>

#include <qdatetm.h>
#include <calendar.h>
#include <qpainter.h>
#include <qlistview.h>
#include <qtextview.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>


class CustomerTickets : public TAAWidget
{
    Q_OBJECT

public:

    CustomerTickets(QWidget* parent = NULL, const char* name = NULL);
    virtual ~CustomerTickets();

    void        setCustomerID(long);

public slots:
    void        refreshTicketList();

protected:
    QListView   *ticketList;

protected slots:
    void        ticketSelected(QListViewItem *);

private:
    long        myCustomerID;
    int         myHideClosed;
};

#endif  // CustomerTickets_included 

