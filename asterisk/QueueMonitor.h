/** QueueMonitor - Monitors the Blarg Sales/Support queue(s)
  ***************************************************************************
  * Written by R. Marc Lewis, 
  *   (C)opyright 1998-2006, R. Marc Lewis and Blarg! Oline Services, Inc.
  *   All Rights Reserved.
  *
  *  Unpublished work.  No portion of this file may be reproduced in whole
  *  or in part by any means, electronic or otherwise, without the express
  *  written consent of Blarg! Online Services and R. Marc Lewis.
  ***************************************************************************
  */


#ifndef QueueMonitor_included
#define QueueMonitor_included

#include <qwidget.h>
#include <TAAWidget.h>
#include <qlistview.h>
#include <qpopupmenu.h>
#include "AsteriskManager.h"

typedef enum QMColumnNames { Channel = 0, ChanState = 1, CallerID= 2, QueueName = 3, Position= 4, Location = 5, Agent = 6};

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
    QListView       *callq;
    QPopupMenu      *menu;
    QListViewItem   *findChannel(const char *chan);
    void            removeChannel(const char *chan);
    void            setColumnText(const char *chan, QMColumnNames col, const char *newText);

protected slots:
    void        asteriskEvent(const astEventRecord);
    void        callPopupMenu(QListViewItem *, const QPoint &, int);
    void        callDoubleClicked(QListViewItem *);
    void        menuItemSelected(int);
};
#endif // QueueMonitor_included
