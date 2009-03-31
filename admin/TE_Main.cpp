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

#include <stdlib.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <q3accel.h>
//Added by qt3to4:
#include <Q3BoxLayout>

#include <BlargDB.h>
#include <ParseFile.h>
#include <ADB.h>

#include <TE_Main.h>

TE_Main::TE_Main(QWidget* parent, const char* name) :
	TAAWidget( parent, name )
{
	setCaption( "Targeted Email" );

    // Create our widgets.
    theTabBar = new QTabBar(this, "theTabBar");

    // Create the tabs for the tab bar.
    theTabBar->addTab(new QTab("Message"));
    theTabBar->addTab(new QTab("Login Types"));
    theTabBar->addTab(new QTab("Cities"));

    qws = new Q3WidgetStack(this, "widgetStack");
    
    // Create the actual tabs now, using the widget stack as the parent.
    tmessage = new TE_Message(qws);
    qws->addWidget(tmessage, 0);
    
    tlogins = new TE_LoginTypes(qws);
    qws->addWidget(tlogins, 1);
    
    tcities = new TE_Cities(qws);
    qws->addWidget(tcities, 2);

    // Buttons now.
    QPushButton *sendButton = new QPushButton(this, "sendButton");
    sendButton->setText("&Send");
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    // Create our layout.
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3);
    ml->addWidget(theTabBar, 0);
    ml->addWidget(qws, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(sendButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);
    
    connect(theTabBar, SIGNAL(selected(int)), qws, SLOT(raiseWidget(int)));

    // Create our Ctrl-1 through Ctrl-3 hotkeys
    Q3Accel  *ac = new Q3Accel(this);
    ac->insertItem(Qt::CTRL+Qt::Key_1, 0);
    ac->insertItem(Qt::CTRL+Qt::Key_2, 1);
    ac->insertItem(Qt::CTRL+Qt::Key_3, 2);
    connect(ac, SIGNAL(activated(int)), this, SLOT(raiseTab(int)));
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
    
    
    if (doIt) {
        QApplication::setOverrideCursor(Qt::waitCursor);
        emit(setStatus("Sending targeted email..."));

        char        srcfile[1024];
        char        dstfile[1024];
        ADB         DB;
        ADB         DB2;
        CustomersDB CDB;
        int         curStep = 0;
        int         totSteps = 0;
        int         hitCount = 0;
        int         reqHits  = 3;
        
        DB.query("select LoginID, LoginType, CustomerID, Active from Logins where Wiped < '1970-01-01'");
        if (DB.rowCount) {
            strcpy(srcfile, tmessage->createMessage());
            totSteps = DB.rowCount;
            while (DB.getrow()) {
                emit(setProgress(curStep++, totSteps));

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
       
        
        emit(setStatus(""));
        QApplication::restoreOverrideCursor();
        close();
    }
}

/*
 * raiseTab()
 *
 * Gets called by the accelerator key actions.
 */
void TE_Main::raiseTab(int tab)
{
    theTabBar->setCurrentTab(tab);
}

// vim: expandtab

