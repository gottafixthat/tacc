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

#ifndef CreateCustTicket_included
#define CreateCustTicket_included

#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3textedit.h>

#include <calendar.h>
#include <TAAWidget.h>

#include "Ticket.h"
#include "TicketEditor.h"


// The TicketLogAnnotater class
// Allows for annotating a note onto a ticket.
class CreateCustTicket : public TAAWidget
{
    Q_OBJECT

public:
    CreateCustTicket(QWidget* parent = NULL, const char* name = NULL, long custID = 0);
    virtual ~CreateCustTicket();

    void            setCustomerID(long);

protected:
    QLabel          *customerIDText;
    QLabel          *customerName;
    QComboBox       *ticketType;
    QLineEdit       *ticketSummary;
    Q3TextEdit       *ticketNotes;

    QPushButton     *customerButton;
    QPushButton     *saveTicketButton;
    QPushButton     *cancelButton;

protected slots:
    void            saveTicket();
    void            customerClicked();
    void            cancelClicked();

private:
    long            myCustID;

};


#endif // CreateCustTicket_included

// vim: expandtab
