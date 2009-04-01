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


#ifndef Tab_Notes_included
#define Tab_Notes_included

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QPainter>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3strlist.h>
#include <Qt3Support/q3listview.h>

#include <BlargDB.h>
#include <TAAWidget.h>

class Tab_Notes : public TAAWidget
{
    Q_OBJECT

public:

    Tab_Notes
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_Notes();

protected:
    QPushButton *newButton;
    QPushButton *ticketButton;
    QCheckBox   *hideAccounting;
    Q3ListView   *list;
    Q3TextView   *noteText;
    QLineEdit   *searchText;
    QLabel      *openTicketCount;
    QLabel      *addedBy;
    QLabel      *noteType;
    QLabel      *subject;
    QLabel      *noteDate;
    QLabel      *loginID;
    QLabel      *callDurationLabel;
    QLabel      *callDuration;

public slots:
	void         refreshNotesList(int);
    void         startSearch();
	virtual void showNoteDetail(Q3ListViewItem *newItem);
	virtual void newNote();
	virtual void hideAccountingChanged(bool);
    virtual void printNotes();
    void         ticketButtonPressed();
    virtual void refreshTicket(long);

protected slots:
    virtual void refreshCustomer(long custID);
    void         noteDoubleClicked(Q3ListViewItem *);

private:
	long myCustID;
    void printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont);
    void printFooter(QPainter *p, int PageNo);
    void noteHeader(QPainter *p);
};

#endif // Tab_Notes_included

// vim: expandtab

