/*
** $Id: Tab_Notes.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Tab_Notes - Controls user note entries.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Tab_Notes.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef Tab_Notes_included
#define Tab_Notes_included

#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qtextview.h>
#include <qstrlist.h>
#include <qpainter.h>
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
    QListView   *list;
    QTextView   *noteText;
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
	virtual void showNoteDetail(QListViewItem *newItem);
	virtual void newNote();
	virtual void hideAccountingChanged(bool);
    virtual void printNotes();
    void         ticketButtonPressed();
    virtual void refreshTicket(long);

protected slots:
    virtual void refreshCustomer(long custID);
    void         noteDoubleClicked(QListViewItem *);

private:
	long myCustID;
    void printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont);
    void printFooter(QPainter *p, int PageNo);
    void noteHeader(QPainter *p);
};

#endif // Tab_Notes_included
