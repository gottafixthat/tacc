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
    me.header().from(myFrom.ascii());
    me.header().to(myTo.ascii());
    me.header().subject(mySubject.ascii());

    if (!attachmentList.count()) {
        // No attachments, simply set the body.
        if (myBody.length()) me.body().assign(myBody.ascii());
    } else {
        // We have attachments.  Add the body part first.
        if (myBody.length()) {
            TextPlain   *pTextPlain = new TextPlain(myBody.ascii());
            me.body().parts().push_back(pTextPlain);
        }

        // Now add the attachments one by one.
        for (uint i = 0; i < attachmentList.count(); i++) {
            emailAttachmentRecord   *rec = attachmentList.at(i);

            // Can't use the Attachment convenience class since 
            // there is no way to override the filename.
            MimeEntity  *mAttach = new MimeEntity();
            ContentType act(rec->mimeType.ascii());
            Base64::Encoder b64;

            Header &h = mAttach->header();
            // Set the content type
            h.contentType(act);
            h.contentType().paramList().push_back(ContentType::Param("name", rec->mimeFilename.ascii()));
            // Set the content-transer-encoding
            h.contentTransferEncoding().mechanism(b64.name());

            // The disposition
            h.contentDisposition().type("attachment");
            h.contentDisposition().paramList().push_back(ContentDisposition::Param("filename", rec->mimeFilename.ascii()));
            mAttach->body().load(rec->filename.ascii(), b64);

            if (me.header().contentType().isMultipart() == true) {
                me.body().parts().push_back(mAttach);
            } else {
                //MimeEntity  *mm = new MimeEntity;
                me.body().preamble("This is a multi-part message in MIME format.");
                ContentType::Boundary boundary;
                ContentType ct("multipart", "mixed");
                ct.paramList().push_back(ContentType::Param("boundary", boundary));
                me.header().contentType(ct);
                //pAttach->header().contentType().paramList().remove(pAttach->header().contentType().paramList().find("name"));
                me.body().parts().push_back(mAttach);
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


