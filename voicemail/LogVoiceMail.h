/*
** $Id: LogVoiceMail.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** LogVoiceMail - A voice mail logging tool.
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
** $Log: LogVoiceMail.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef LogVoiceMail_included
#define LogVoiceMail_included

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmultilinedit.h>

#include <TAAWidget.h>
#include <qdatetimeedit.h>

class LogVoiceMail : public TAAWidget
{
    Q_OBJECT

public:

    LogVoiceMail(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~LogVoiceMail();

protected:
    QDateTimeEdit   *msgDateTime;
    QMultiLineEdit  *message;
    QPushButton     *saveButton;
    QPushButton     *closeButton;
    QLabel          *statusLabel;

protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }
    virtual void saveMessage();
    virtual void closeClicked()                     { close(); }
};
#endif // LogVoiceMail_included
