/*
** $Id: CreateCustTicket.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** CreateCustTicket.h - A widget that allows the user to create a customer
**                      ticket.
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

#ifndef CreateCustTicket_included
#define CreateCustTicket_included

#include <TAAWidget.h>

#include <qdatetm.h>
#include <calendar.h>
#include <qpainter.h>
#include <q3listview.h>
#include <q3textview.h>
#include <q3textedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3multilineedit.h>
#include <qcheckbox.h>

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
