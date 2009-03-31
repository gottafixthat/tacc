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
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/Q3Frame>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <calendar.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>

class ProcessVoiceMail : public TAAWidget
{
    Q_OBJECT

public:

    ProcessVoiceMail(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~ProcessVoiceMail();

    Q3PopupMenu      *menu();
    int             count();

public slots:
    virtual void    refreshList();
    virtual void    listItemChanged(Q3ListViewItem *);
    virtual void    logVM();

protected slots:
    virtual void loginIDChanged(const QString &);
    virtual void QCloseEvent(QEvent *)      { delete this; }
    virtual void saveClicked();

protected:
    Q3ListView       *vmList;
    QLabel          *msgDate;
    QLabel          *msgTime;
    Q3Frame          *msgTimeArea;
    QLabel          *loggedBy;
    QLabel          *loggedAt;
    Q3TextView       *message;
    QPushButton     *saveButton;
    Q3MultiLineEdit  *followUp;
    QComboBox       *disposition;
    QLineEdit       *loginID;
    QLabel          *customerID;
    QLabel          *customerName;
    QCheckBox       *processed;
    Q3PopupMenu      *vmMenu;
    long            myCustID;
    int             myVMCount;
};
#endif // ProcessVoiceMail_included
