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


#ifndef QueueMonitor_included
#define QueueMonitor_included

#include <QtGui/QWidget>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3popupmenu.h>
#include <TAAWidget.h>
#include "AsteriskManager.h"

enum QMColumnNames { Channel = 0, ChanState = 1, CallerID= 2, QueueName = 3, Position= 4, Location = 5, Agent = 6, Timer = 7, TimerHidden = 8};

class QueueMonitor : public TAAWidget
{
    Q_OBJECT

public:

    QueueMonitor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~QueueMonitor();

signals:
    void        phoneNumberSelected(const char *);
    void        transferCall(const char *, const char *, const char *, int);

protected:
    Q3ListView       *callq;
    Q3PopupMenu      *menu;
    Q3ListViewItem   *findChannel(const char *chan);
    void            removeChannel(const char *chan);
    void            setColumnText(const char *chan, QMColumnNames col, const char *newText);

protected slots:
    void        asteriskEvent(const astEventRecord);
    void        callPopupMenu(Q3ListViewItem *, const QPoint &, int);
    void        callDoubleClicked(Q3ListViewItem *);
    void        menuItemSelected(int);
    void        refreshTimers();
};
#endif // QueueMonitor_included

// vim: expandtab

