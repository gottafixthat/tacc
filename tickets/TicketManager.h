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

#ifndef TicketManager_included
#define TicketManager_included

#include <QtCore/QDateTime>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>

#include <calendar.h>
#include <TAAWidget.h>


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

// vim: expandtab
