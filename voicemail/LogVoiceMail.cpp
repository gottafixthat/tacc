/*
** $Id: LogVoiceMail.cpp,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** LogVoiceMail - A voicemail logging tool.
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
** $Log: LogVoiceMail.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "LogVoiceMail.h"

#include <qlabel.h>
#include <qdatetimeedit.h>
#include <qlayout.h>

#include <TAAWidget.h>
#include <TAATools.h>
#include <qregexp.h>

#include <ADB.h>

LogVoiceMail::LogVoiceMail
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "Voice Mail Logging" );

    // Create the widgets
    QLabel *dateLabel = new QLabel(this);
    dateLabel->setAlignment(AlignRight|AlignVCenter);
    dateLabel->setText("D&ate/Time:");

    msgDateTime = new QDateTimeEdit(QDateTime::currentDateTime(), this, "MsgDateTime");
    dateLabel->setBuddy(msgDateTime);
    
    QLabel *messageLabel = new QLabel(this);
    messageLabel->setAlignment(AlignRight|AlignTop);
    messageLabel->setText("M&essage:");

    message = new QMultiLineEdit(this);
    message->setWordWrap(QMultiLineEdit::WidgetWidth);
    messageLabel->setBuddy(message);

    saveButton = new QPushButton(this);
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveMessage()));

    closeButton = new QPushButton(this);
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    statusLabel = new QLabel(this);
    statusLabel->setAlignment(AlignLeft|AlignVCenter);
    statusLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statusLabel->setMinimumSize(0, 20);

    // Now, create our layouts
    // Date/time area.
    QGridLayout *dtl = new QGridLayout(2, 4, 3);
    dtl->setColStretch(0, 0);
    dtl->setColStretch(1, 1);
    dtl->setColStretch(2, 0);
    dtl->setColStretch(3, 1);
    dtl->setRowStretch(0, 0);
    dtl->setRowStretch(1, 1);

    dtl->addWidget(dateLabel,               0, 0);
    dtl->addMultiCellWidget(msgDateTime,    0, 0, 1, 3);
    dtl->addWidget(messageLabel,            1, 0);
    dtl->addMultiCellWidget(message,        1, 1, 1, 3);

    // The action button layout.
    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 5);
    abl->addStretch(1);
    abl->addWidget(saveButton, 0);
    abl->addWidget(closeButton, 0);
    abl->addSpacing(5);

    // The main layout.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 0, 0);
    ml->addSpacing(3);
    ml->addLayout(dtl, 1);
    ml->addSpacing(3);
    ml->addLayout(abl, 0);
    ml->addSpacing(3);
    ml->addWidget(statusLabel, 0);

    // Done.

    msgDateTime->setFocus();

    resize(350, 180);
}

LogVoiceMail::~LogVoiceMail()
{
}

void LogVoiceMail::saveMessage()
{
    if (message->text().stripWhiteSpace().length()) {
        // Validate the date and time, and that a message exists.
        ADBTable        vmDB("VoiceMail");
        QDate           tmpDate = msgDateTime->dateTime().date();
        QTime           tmpTime = msgDateTime->dateTime().time();
        
        QDateTime       tmpDateTime;
        QDateTime       curDateTime;
        long            msgID = 0;
        char            tmpStr[1024];

        tmpDateTime.setDate(tmpDate);
        tmpDateTime.setTime(tmpTime);
        curDateTime.setDate(QDate::currentDate());
        curDateTime.setTime(QTime::currentTime());

        vmDB.setValue("MsgDateTime", tmpDateTime);
        vmDB.setValue("LoggedBy",    curUser().userName);
        vmDB.setValue("LoggedAt",    curDateTime);
        vmDB.setValue("Message", message->text().replace(QRegExp("\n"), "<BR>\n"));

        msgID = vmDB.ins();

        sprintf(tmpStr, "Message #%ld saved.", msgID);
        statusLabel->setText(tmpStr);
        message->clear();
        msgDateTime->setFocus();
    } else {
        statusLabel->setText("The message body may not be blank.");
        message->setFocus();
    }
}

