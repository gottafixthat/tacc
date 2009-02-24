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
#include <TAATools.h>

#include <mimetic/mimetic.h>
#include <sstream>
#include <string>

#include <qfile.h>

using namespace std;
using namespace mimetic;

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
void EmailMessage::addAttachment(const char *fName, const char *mimeType, const char *msgName)
{
    emailAttachmentRecord *atch = new emailAttachmentRecord;
    atch->filename = fName;
    atch->mimeFilename = msgName;
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
void EmailMessage::addAttachment(const QString fName, const QString mimeType, const QString msgName)
{
    addAttachment(fName.ascii(), mimeType.ascii(), msgName.ascii());
}

/**
 * addAttachment()
 *
 * Given a buffer in memory, suggested file name and a mime type,
 * this adds an attachment to the message.
 */
void EmailMessage::addAttachment(const QByteArray buf, const QString fName, const QString mimeType)
{
    // Create a temp file for the attachment.
    QString tmpFile;
    tmpFile = makeTmpFileName("/tmp/msgAttachment-XXXXXX");
    QFile   oFile(tmpFile);
    if (oFile.open(IO_WriteOnly)) {
        QDataStream oStrm(&oFile);
        oStrm << buf;
        oFile.close();

        // Add it to our list now.
        emailAttachmentRecord *atch = new emailAttachmentRecord;
        atch->filename = tmpFile;
        atch->mimeFilename = fName;
        atch->mimeType = mimeType;
        atch->isTempFile = 1;
        attachmentList.append(atch);
    }
    
}

/**
 * send()
 *
 * Assembles and queues the message for sending.
 */
int EmailMessage::send()
{
    QString msgFile;
    QString toMailbox;
    QString frMailbox;

    toMailbox = myTo.left(myTo.find('@'));
    frMailbox = myFrom.left(myFrom.find('@'));
    msgFile = msgFile.sprintf("/var/spool/taamail/Email-%s-%s-XXXXXX", frMailbox.ascii(), toMailbox.ascii());

    msgFile = makeTmpFileName(msgFile.ascii());

    // Create the mimetic MimeEntity
    MimeEntity  me;
    me.header().from(myTo.ascii());
    me.header().to(myTo.ascii());
    me.header().subject(mySubject.ascii());

    if (!attachmentList.count()) {
        // No attachments, simply set the body.
        me.body().assign(myBody.ascii());
    } else {
        // We have attachments.  Add the body part first.
        TextPlain   *pTextPlain = new TextPlain(myBody.ascii());
        me.body().parts().push_back(pTextPlain);

        // Now add the attachments one by one.
        for (uint i = 0; i < attachmentList.count(); i++) {
            emailAttachmentRecord   *rec = attachmentList.at(i);

            Attachment  *pAttach = NULL;
            Base64::Encoder b64;
            ContentType act(rec->mimeType.ascii());
            //act.param("filename", rec->mimeFilename.ascii());
            //pAttach = new Attachment(rec->filename.ascii(), rec->mimeType.ascii(), b64);
            pAttach = new Attachment(rec->filename.ascii(), act, b64);
            if (me.header().contentType().isMultipart() == true) {
                me.body().parts().push_back(pAttach);
            } else {
                //MimeEntity  *mm = new MimeEntity;
                me.body().preamble("This is a multi-part message in MIME format.");
                ContentType::Boundary boundary;
                ContentType ct("multipart", "mixed");
                ct.paramList().push_back(ContentType::Param("boundary", boundary));
                me.header().contentType(ct);
                //pAttach->header().contentType().paramList().remove(pAttach->header().contentType().paramList().find("name"));
                me.body().parts().push_back(pAttach);
            }
        }
    }

    // We should now have a message.  Output it to file.
    string  s;
    stringstream    oStr;
    oStr << me;
    s = oStr.str();

    QFile   oFile(msgFile);
    if (oFile.open(IO_WriteOnly)) {
        QTextStream ofStr(&oFile);
        ofStr << s.c_str();
        oFile.close();
    }

}


