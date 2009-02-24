/**
 * EmailMessage.h - A utility class that encapsulates the mimetic
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

#ifndef EMAILMESSAGE_H
#define EMAILMESSAGE_H

#include <qstring.h>
#include <qptrlist.h>
#include <qcstring.h>

struct emailAttachmentRecord {
    QString     filename;
    QString     mimeFilename;
    QString     mimeType;
    int         isTempFile;
};

class EmailMessage {

public:
    EmailMessage();
    virtual ~EmailMessage();

    void        clear();
    void        setFrom(const char *);
    void        setFrom(const QString);
    void        setTo(const char *);
    void        setTo(const QString);
    void        setSubject(const char *);
    void        setSubject(const QString);
    void        setBody(const char *);
    void        setBody(const QString);
    void        addAttachment(const char *, const char *);
    void        addAttachment(const QString, const QString);
    void        addAttachment(const QByteArray, const QString, const QString);

    int         send();

protected:
    QString                         myFrom;
    QString                         myTo;
    QString                         mySubject;
    QString                         myBody;
    QPtrList<emailAttachmentRecord>    attachmentList;
};

#endif

