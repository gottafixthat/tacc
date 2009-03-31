/*
** $Id: TicketManager.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** TicketManager.h - A widget that manages the list of open tickets.
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
** $Log: TicketManager.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef TicketManager_included
#define TicketManager_included

#include <TAAWidget.h>

#include <qdatetm.h>
#include <calendar.h>
#include <qpainter.h>
#include <q3listview.h>
#include <q3textview.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3popupmenu.h>


class TicketManager : public TAAWidget
{
    Q_OBJECT

public:

    TicketManager(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TicketManager();

    Q3PopupMenu  *menu();

    int         ticketCount();

public slots:
    void        refreshTicketList();
    void        editTicket(Q3ListViewItem *);

protected slots:
    void        showPersonalSelected();
    void        showAllPersonalSelected();

protected:
    Q3ListView   *ticketList;
    Q3PopupMenu  *ticketMenu;
    int         showPersonalMenuID;
    int         showAllPersonalMenuID;
    int         myTicketCount;
};
#endif // TicketManager_included
