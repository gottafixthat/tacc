/**
 * EmailMessage.cpp - A utility class that encapsulates the mimetic
 * MimeEntity funcitons into an interface that is much easier to
 * use within TAA.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <EmailMessage.h>


/** 
 * EmailMessage()
 *
 * Contstructor.
 */
EmailMessage::EmailMessage()
{
}

/**
 * ~EmailMessage()
 *
 * Destructor.
 */
EmailMessage::~EmailMessage()
{
    clear();
}

/**
 * clear()
 *
 * Empties all of our structures and resets to handle a new
 * message.
 */
void EmailMessage::clear()
{
    myFrom      = "";
    myTo        = "";
    mySubject   = "";
    myBody      = "";
    attachmentList.clear();
}

/**
 * setFrom()
 *
 * Sets the from address in the message.
 */
void EmailMessage::setFrom(const char *fromAddr)
{
    myFrom = fromAddr;
}

/**
 * setFrom() 
 *
 * Sets the from address in the message.
 */
void EmailMessage::setFrom(const QString fromAddr)
{
    setFrom(fromAddr.ascii());
}

/**
 * setTo()
 *
 * Sets the to address in the message.
 */
void EmailMessage::setTo(const char *toAddr)
{
    myTo = toAddr;
}

/**
 * setTo() 
 *
 * Sets the to address in the message.
 */
void EmailMessage::setTo(const QString toAddr)
{
    setTo(toAddr.ascii());
}

/**
 * setSubject()
 *
 * Sets the subject of the message
 */
void EmailMessage::setSubject(const char *msgSubject)
{
    mySubject = msgSubject;
}

/**
 * setSubject() 
 *
 * Sets the subject of the message.
 */
void EmailMessage::setSubject(const QString msgSubject)
{
    setSubject(msgSubject.ascii());
}

/**
 * setBody()
 *
 * Sets the body of the message
 */
void EmailMessage::setBody(const char *msgBody)
{
    myBody = msgBody;
}

/**
 * setBody() 
 *
 * Sets the body of the message.
 */
void EmailMessage::setBody(const QString msgBody)
{
    setBody(msgBody.ascii());
}

/**
 * addAttachment()
 *
 * Given a file name and a mime type, this adds an attachment
 * to the message.
 */
void EmailMessage::addAttachment(const char *fName, const char *mimeType)
{
    emailAttachmentRecord *atch = new emailAttachmentRecord;
    atch->filename = fName;
    atch->mimeFilename = fName;
    atch->mimeType = mimeType;
    atch->isTempFile = 0;
    attachmentList.append(atch);
}

/**
 * addAttachment()
 *
 * Given a file name and a mime type, this adds an attachment
 * to the message.
 */
void EmailMessage::addAttachment(const QString fName, const QString mimeType)
{
    addAttachment(fName.ascii(), mimeType.ascii());
}

/**
 * addAttachment()
 *
 * Given a buffer in memory, suggested file name and a mime type,
 * this adds an attachment to the message.
 */
void EmailMessage::addAttachment(const QByteArray buf, const QString fName, const QString mimeType)
{
}

/**
 * send()
 *
 * Assembles and queues the message for sending.
 */
int EmailMessage::send()
{
}


