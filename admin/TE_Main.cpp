/*
** $Id: TE_Main.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** TE_Main - Targeted Email - main window.
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
** $Log: TE_Main.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "TE_Main.h"

#define Inherited TE_MainData

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <qapplication.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>

#include "BlargDB.h"
#include "ParseFile.h"
#include <ADB.h>


TE_Main::TE_Main
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Targeted Email" );
    
    // qws is our QWidgetStack
    tmessage = new TE_Message(qws);
    qws->addWidget(tmessage, 0);
    
    tlogins = new TE_LoginTypes(qws);
    qws->addWidget(tlogins, 1);
    
    tcities = new TE_Cities(qws);
    qws->addWidget(tcities, 2);
    
    connect(theTabBar, SIGNAL(selected(int)), qws, SLOT(raiseWidget(int)));
}


TE_Main::~TE_Main()
{
}


/*
** sendMessage - Processes the selections and sends the message away...
*/

void TE_Main::sendMessage()
{
    int doIt = 0;
    
    if (!tmessage->messageOk()) {
        QMessageBox::information(this, "Targeted Email", "The message is not complete.\nComplete the message before sending.");
        return;
    }

    // Find out if the user is really ready to do this...
    switch(QMessageBox::information(this, 
      "Send Targeted Email",
      "Are you sure you wish to send this email?",
      "&Yes", "&No", "&Cancel",
      0,            // Enter == button 0, yes
      2             // Escape == cancel, button 2
    )) {
        case 0:     // Yes, do it.
            doIt  = 1;
            break;
            
        case 1:
            doIt = 0;
            break;
            
        default:        // Cancel, close the window.
            close();
            break;
    }
    
    
    QApplication::setOverrideCursor(waitCursor);

    if (doIt) {
        char        srcfile[1024];
        char        dstfile[1024];
        ADB         DB;
        ADB         DB2;
        CustomersDB CDB;
        int         curStep = 0;
        int         hitCount = 0;
        int         reqHits  = 3;
        
        DB.query("select LoginID, LoginType, CustomerID, Active from Logins where Wiped < '1970-01-01'");
        if (DB.rowCount) {
            strcpy(srcfile, tmessage->createMessage());
            QProgressDialog     prog("Targeted Email", "Cancel", DB.rowCount);
            while (DB.getrow()) {
                prog.setProgress(curStep++);
                
                hitCount = 0;
                hitCount += tlogins->isIncluded(atol(DB.curRow["LoginType"]), atoi(DB.curRow["Active"]));
                
                if (tcities->allCities()) hitCount++;
                else {
                    // Load the City for the customer.
                    DB2.query("select City from Addresses where RefFrom = %d and RefID = %ld", REF_CUSTOMER, atol(DB.curRow["CustomerID"]));
                    if (DB2.rowCount) {
                        DB2.getrow();
                        hitCount += tcities->isIncluded(DB2.curRow["LoginID"]);
                    }
                }
                
                // Check for primary login only.
                if (tlogins->primaryOnly()) {
                    CDB.get(atol(DB.curRow["CustomerID"]));
                    if (!strcmp(CDB.getStr("PrimaryLogin"), DB.curRow["LoginID"])) hitCount++;
                } else {
                    // All logins are being sent this email.
                    hitCount++;
                }

                if (hitCount == reqHits) {
                    // Send the message
                    sprintf(dstfile, "/var/spool/taamail/TE-%s-%s-%s", DB.curRow["LoginID"], DB.curRow["LoginType"], DB.curRow["CustomerID"]);
                    parseFile(srcfile, dstfile, atol(DB.curRow["CustomerID"]), DB.curRow["LoginID"], "");
                }
            }
            
            // Remove the source file.
            unlink(srcfile);
        }
       
        
        QApplication::restoreOverrideCursor();
        close();
    }
}


