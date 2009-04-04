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

#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QFile>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/q3textstream.h>

#include <TE_Message.h>

using namespace Qt;

TE_Message::TE_Message(QWidget* parent, const char* name) :
	TAAWidget(parent)
{
	setCaption( "Message" );

    QLabel *fromLineLabel = new QLabel(this, "fromLineLabel");
    fromLineLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    fromLineLabel->setText("From:");

    fromLine = new QLineEdit(this, "fromLine");
    fromLine->setMaxLength(80);

    QLabel *msgSubjectLabel = new QLabel(this, "msgSubjectLabel");
    msgSubjectLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    msgSubjectLabel->setText("Subject:");

    msgSubject = new QLineEdit(this, "msgSubject");
    msgSubject->setMaxLength(80);

    QLabel *msgTextLabel = new QLabel(this, "msgTextLabel");
    msgTextLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);
    msgTextLabel->setText("Message Text:");

    msgText = new Q3MultiLineEdit(this, "msgText");

    // A single grid layout is all we need for this widget.
    Q3GridLayout *ml = new Q3GridLayout(this, 3, 2, 3);
    int curRow = 0;
    ml->addWidget(fromLineLabel,    curRow, 0);
    ml->addWidget(fromLine,         curRow, 1);
    ml->setRowStretch(curRow, 0);

    curRow++;
    ml->addWidget(msgSubjectLabel,  curRow, 0);
    ml->addWidget(msgSubject,       curRow, 1);
    ml->setRowStretch(curRow, 0);

    curRow++;
    ml->addWidget(msgTextLabel,     curRow, 0);
    ml->addWidget(msgText,          curRow, 1);
    ml->setRowStretch(curRow, 1);
    ml->setColStretch(0, 0);
    ml->setColStretch(1, 1);
}


TE_Message::~TE_Message()
{
}


/*
** createMessage  - Creates a parsable message from the contents of the
**                  edit fields.  Returns the name of the file as a char*
*/

char * TE_Message::createMessage()
{
    char    *RetVal = new char[1024];
    
    strcpy(RetVal, tmpnam(NULL));
    
    QFile   qfp(RetVal);
    if (qfp.open(QIODevice::WriteOnly)) {
        Q3TextStream ts(&qfp);
        
        ts << "To: {LoginID}@avvanta.com" << endl;
        ts << "From: " << fromLine->text() << endl;
        ts << "Subject: " << msgSubject->text() << endl;
        ts << endl;
        ts << msgText->text() << endl;
        
        qfp.close();


    } else {
        delete RetVal;
        RetVal = NULL;
    }

    return RetVal;
}

/*
** messageOk - Returns 1 if the message is okay (has a from, subject and body)
**             to send.  Returns 0 if not.
*/

int TE_Message::messageOk()
{
    int RetVal = 1;
    
    if (!strlen(fromLine->text())) RetVal = 0;
    if (!strlen(msgSubject->text())) RetVal = 0;
    if (!strlen(msgText->text())) RetVal = 0;
    
    return RetVal;
}



// vim: expandtab

