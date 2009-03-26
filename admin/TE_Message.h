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

#ifndef TE_MESSAGE_H
#define TE_MESSAGE_H

#include <qlineedit.h>
#include <qmultilineedit.h>

#include <TAAWidget.h>

/*!
 * \brief A widget to compose a simple message.
 *
 * This widget allows the user to enter a simple message by providing who
 * the message is from, the subject and the message body.
 * It is used by the TE_Main class to send a targeted email.
 */
class TE_Message : public TAAWidget
{
    Q_OBJECT

public:

    TE_Message(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TE_Message();
    
    char *createMessage();
    
    int messageOk();

protected slots:
    virtual void    closeEvent(QCloseEvent *)       { delete this; }

protected:
    QLineEdit       *fromLine;
    QLineEdit       *msgSubject;
    QMultiLineEdit  *msgText;

};
#endif // TE_Message_included
// vim: expandtab
