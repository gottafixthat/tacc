/*
** $Id: ProcessVoiceMail.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** ProcessVoiceMail - A voice mail call returning interface.
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
** $Log: ProcessVoiceMail.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef ProcessVoiceMail_included
#define ProcessVoiceMail_included

#include "TAATools.h"
#include "TAAWidget.h"
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qmultilineedit.h>
#include <calendar.h>
#include <qtextview.h>
#include <qpopupmenu.h>

class ProcessVoiceMail : public TAAWidget
{
    Q_OBJECT

public:

    ProcessVoiceMail(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~ProcessVoiceMail();

    QPopupMenu      *menu();
    int             count();

public slots:
    virtual void    refreshList();
    virtual void    listItemChanged(QListViewItem *);
    virtual void    logVM();

protected slots:
    virtual void loginIDChanged(const QString &);
    virtual void QCloseEvent(QEvent *)      { delete this; }
    virtual void saveClicked();

protected:
    QListView       *vmList;
    QLabel          *msgDate;
    QLabel          *msgTime;
    QFrame          *msgTimeArea;
    QLabel          *loggedBy;
    QLabel          *loggedAt;
    QTextView       *message;
    QPushButton     *saveButton;
    QMultiLineEdit  *followUp;
    QComboBox       *disposition;
    QLineEdit       *loginID;
    QLabel          *customerID;
    QLabel          *customerName;
    QCheckBox       *processed;
    QPopupMenu      *vmMenu;
    long            myCustID;
    int             myVMCount;
};
#endif // ProcessVoiceMail_included
