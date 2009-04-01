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

#ifndef TicketEditor_included
#define TicketEditor_included

#include <QtCore/QDateTime>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>

#include <calendar.h>
#include <TAAWidget.h>

#include "Ticket.h"

// The TicketHeader lays out and displays common ticket elements that 
// are in all of the various widgets, including the Ticket Number, the
// owner, the customer information, department, etc.

class TicketHeader : public TAAWidget
{
    Q_OBJECT

public:

    TicketHeader(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TicketHeader();

    void        setTicketNo(long);

protected:
    QLabel      *dispTicketNo;
    QLabel      *dispCustomerID;
    QLabel      *dispName;
    QLabel      *dispOwner;
    QLabel      *dispType;
    QLabel      *dispOpened;
    QLabel      *dispClosed;
    QLabel      *dispModified;
    QLabel      *dispStatus;
    QLabel      *summary;

protected slots:
    void        refreshTicket(long);

private:
    Ticket      *myTicket;
};




class TicketEditor : public TAAWidget
{
    Q_OBJECT

public:

    TicketEditor(QWidget* parent = NULL, const char* name = NULL, long ticketNo = 0);
    virtual ~TicketEditor();

    void        setTicketNo(long);

protected:
    TicketHeader    *header;
    QComboBox       *logLevel;
    QCheckBox       *reverseLog;
    Q3TextView       *ticketLog;

    QPushButton     *customerButton;
    QPushButton     *annotateButton;
    QPushButton     *takeTicketButton;
    QPushButton     *releaseTicketButton;

protected slots:
    void        refreshTicket(long);
    void        refreshLog();
    void        setLogLevel(int);
    void        customerClicked();
    void        annotateTicket();
    void        takeTicket();
    void        releaseTicket();

private:
    bool        loggedView;
    Ticket      *myTicket;
    int         myLogLevel;

};


// The TicketLogAnnotater class
// Allows for annotating a note onto a ticket.
class TicketLogAnnotater : public TAAWidget
{
    Q_OBJECT

public:
    TicketLogAnnotater(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TicketLogAnnotater();

    void            setTicketNo(long);

protected:
    TicketHeader    *header;
    QComboBox       *newStatus;
    Q3MultiLineEdit  *noteText;
    QCheckBox       *isUrgent;

    QPushButton     *saveNoteButton;
    QPushButton     *cancelButton;

protected slots:
    void            saveNote();

private:
    Ticket              *myTicket;
    Ticket::t_Status    prevStatus;
    int                 statusMap[10];
    int                 revStatusMap[10];
};


#endif // TicketEditor_included

// vim: expandtab
