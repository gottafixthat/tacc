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

#ifndef CustomerTickets_included
#define CustomerTickets_included

#include <QtCore/QDateTime>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>

#include <TAAWidget.h>
#include <calendar.h>


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
    Q3ListView   *ticketList;

protected slots:
    void        ticketSelected(Q3ListViewItem *);

private:
    long        myCustomerID;
    int         myHideClosed;
};

#endif  // CustomerTickets_included 


// vim: expandtab
