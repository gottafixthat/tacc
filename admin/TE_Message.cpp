/*
** $Id$
**
***************************************************************************
**
** TE_Message - Targeted Email - Message widget.
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
** $Log: TE_Message.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "TE_Message.h"

#include <stdio.h>
#include <stdlib.h>

#include <qfile.h>
#include <qtextstream.h>

#define Inherited TE_MessageData

TE_Message::TE_Message
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Message" );
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
    if (qfp.open(IO_WriteOnly)) {
        QTextStream ts(&qfp);
        
        ts << "To: {LoginID}@blarg.net" << endl;
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



