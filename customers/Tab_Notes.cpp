/*
** $Id$
**
***************************************************************************
**
** Tab_Notes - This widget displays and shows user notes.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>

#include <qprinter.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qregexp.h>
#include <qapplication.h>
#include <qlayout.h>

#include <FParse.h>

#include <CustCallLogReport.h>
#include "CustCallDetail.h"
#include "Tab_Notes.h"
#include "BlargDB.h"
#include "BString.h"
#include "NoteEdit.h"
#include <ADB.h>

#include <TAA.h>
#include <TAATools.h>
#include <Ticket.h>
#include <Cfg.h>

#define NOTEIDCOL   4
#define NOTETYPECOL 5

Tab_Notes::Tab_Notes
(
	QWidget* parent,
	const char* name,
	long CustID
) : TAAWidget( parent, name )
{
	setCaption( "Notes" );

    // Create our widgets.

    QLabel *searchLabel = new QLabel(this, "SearchLabel");
    searchLabel->setText("Searc&h:");
    searchLabel->setAlignment(AlignVCenter|AlignRight);

    searchText = new QLineEdit(this, "SearchText");
    connect(searchText, SIGNAL(returnPressed()), this, SLOT(startSearch()));

    searchLabel->setBuddy(searchText);
    
    list = new QListView(this, "Note List");
    //list->addColumn("Ticket");
    list->addColumn("Date");
    list->addColumn("User");
    //list->addColumn("Dept.");
    list->addColumn("Reason/Summary");
    list->addColumn("Note");
    list->setAllColumnsShowFocus(true);
    list->setRootIsDecorated(true);
	list->setSorting(0, false);
    list->setFixedHeight(100);
    list->setHScrollBarMode(QScrollView::AlwaysOff);
    connect(list, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(showNoteDetail(QListViewItem *)));
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(noteDoubleClicked(QListViewItem *)));

    // Add some "markers" or information labels
    QLabel  *addedByLabel  = new QLabel(this);
    addedByLabel->setAlignment(AlignVCenter|AlignRight);
    addedByLabel->setText("Added By:");

    addedBy = new QLabel(this);
    addedBy->setAlignment(AlignVCenter|AlignLeft);
    addedBy->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    QLabel  *noteTypeLabel  = new QLabel(this);
    noteTypeLabel->setAlignment(AlignVCenter|AlignRight);
    noteTypeLabel->setText("Type:");

    noteType = new QLabel(this);
    noteType->setAlignment(AlignVCenter|AlignLeft);
    noteType->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    QLabel  *subjectLabel  = new QLabel(this);
    subjectLabel->setAlignment(AlignVCenter|AlignRight);
    subjectLabel->setText("Subject:");

    subject = new QLabel(this);
    subject->setAlignment(AlignVCenter|AlignLeft);
    subject->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    subject->setMinimumWidth(180);

    QLabel  *noteDateLabel  = new QLabel(this);
    noteDateLabel->setAlignment(AlignVCenter|AlignRight);
    noteDateLabel->setText("Date:");

    noteDate = new QLabel(this);
    noteDate->setAlignment(AlignVCenter|AlignLeft);
    noteDate->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    QLabel  *loginIDLabel  = new QLabel(this);
    loginIDLabel->setAlignment(AlignVCenter|AlignRight);
    loginIDLabel->setText("Login ID:");

    loginID = new QLabel(this);
    loginID->setAlignment(AlignVCenter|AlignLeft);
    loginID->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    
    callDurationLabel  = new QLabel(this);
    callDurationLabel->setAlignment(AlignVCenter|AlignRight);
    callDurationLabel->setText("Duration:");

    callDuration = new QLabel(this);
    callDuration->setAlignment(AlignVCenter|AlignLeft);
    callDuration->setFrameStyle(QFrame::Panel|QFrame::Sunken);


    QLabel *spacerLabel = new QLabel(this);

    
    noteText = new QTextView(this);

    /*
    QLabel *hline1 = new QLabel(this);
    hline1->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    hline1->setMinimumSize(0, 4);
    hline1->setMaximumSize(32767, 4);
    */
    HorizLine   *hline1 = new HorizLine(this);

    hideAccounting = new QCheckBox(this);
    hideAccounting->setText("Hide Accounting");
    hideAccounting->setChecked(true);
    connect(hideAccounting, SIGNAL(toggled(bool)), this, SLOT(hideAccountingChanged(bool)));

    newButton = new QPushButton(this);
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newNote()));
    
    QPushButton *followUpButton = new QPushButton(this);
    followUpButton->setText("Follo&wup");
    followUpButton->setEnabled(false);
    // connect(folowUpButton, SIGNAL(clicked()), this, SLOT(newNote()));

    QPushButton *callLogButton = new QPushButton(this);
    callLogButton->setText("Call Log");
    connect(callLogButton, SIGNAL(clicked()), this, SLOT(showCallLog()));
    callLogButton->setEnabled(false);

    ticketButton= new QPushButton(this);
    ticketButton->setText("Open &Ticket");
    connect(ticketButton, SIGNAL(clicked()), this, SLOT(ticketButtonPressed()));
    ticketButton->setEnabled(false);

    QPushButton *printButton = new QPushButton(this);
    printButton->setText("&Print");
    connect(printButton, SIGNAL(clicked()), this, SLOT(printNotes()));

    /* This widget has a somewhat complex layout.  It will look something
       like this:

       +-------------------------------------------------------------+
       | +------------------+   +--------------------------------+   |
       | | Note Subj. List  |   | Selected Note's text.          |   |
       | |                  |   |                                |   |
       | |                  |   |                                |   |
       | |                  |   |                                |   | 
       | |                  |   |                                |   |
       | +------------------+   +--------------------------------+   |
       |-------------------------------------------------------------|
       | [x] Hide Accounting                [New] [Call Log] [Print] |
       +-------------------------------------------------------------+

       New layout, 2001-03-06:

       +-------------------------------------------------------------+
       |   Search: [                                      ] [Search] |
       | +--------+------+------+-------+--------------------------+ |
       | | Ticket | Date | User | Dept. | Reason/Summary           | |
       | +--------+------+------+-------+--------------------------+ |
       | | 12345  | xxxx | fred | Sales | Added personal domain    | |
       | +--------+------+------+-------+--------------------------+ |
       | +---------------------------------------------------------+ |
       | | Note text goes here.                                    | |
       | |                                                         | |
       | +---------------------------------------------------------+ |
       |                         [Add] [New] [Call] [Print] [Cancel] |
       +-------------------------------------------------------------+

    */

    // Create the main layout to hold them all.
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QBoxLayout *sal = new QBoxLayout(QBoxLayout::LeftToRight, 4);
    sal->addWidget(searchLabel, 0);
    sal->addWidget(searchText,  1);
    sal->addStretch(1);

    ml->addLayout(sal, 0);

    ml->addWidget(list, 0);
    // ml->addWidget(hline1, 0);
    // Create a grid for the Note Detail information
    QGridLayout *ndl = new QGridLayout(5, 1, 1);
    int curRow = -1;
    int curCol = 0;
    ndl->setColStretch(0,0);
    ndl->setColStretch(1,1);

    curCol = 0;
    curRow++;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(addedByLabel,    curRow, curCol++);
    ndl->addWidget(addedBy,         curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(noteTypeLabel,   curRow, curCol++);
    ndl->addWidget(noteType,        curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(subjectLabel,    curRow, curCol++);
    ndl->addWidget(subject,         curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(noteDateLabel,   curRow, curCol++);
    ndl->addWidget(noteDate,        curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(loginIDLabel,    curRow, curCol++);
    ndl->addWidget(loginID,         curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,0);
    ndl->addWidget(callDurationLabel, curRow, curCol++);
    ndl->addWidget(callDuration,      curRow, curCol++);

    curRow++;
    curCol = 0;
    ndl->setRowStretch(curRow,1);
    ndl->addWidget(spacerLabel,     curRow, curCol++);

    QBoxLayout *cndl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    cndl->addLayout(ndl, 0);
    cndl->addWidget(noteText, 1);

    //ml->addWidget(noteText, 1);
    ml->addLayout(cndl, 1);
    ml->addWidget(hline1, 0);

    // Now, create a box to hold our action buttons.
    QBoxLayout *abl = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    abl->addWidget(newButton, 0);
    abl->addWidget(followUpButton, 0);
    abl->addWidget(callLogButton, 0);
    abl->addWidget(ticketButton, 0);
    abl->addWidget(printButton, 0);
    abl->addStretch(1);
    abl->addWidget(hideAccounting, 0);

    ml->addLayout(abl, 0);
    // Done.


	
	myCustID = CustID;
	
	refreshNotesList(0);
	
}


Tab_Notes::~Tab_Notes()
{
}


/*
** refreshNotesList - A public slot which will get called when an item
**					   in the Notes list is changed.
**
*/

void Tab_Notes::refreshNotesList(int)
{
    ADB             DB;
    QString         tmpstr = "";
    char	        tmpAcctStr[128];
    char            tmpDateStamp[128] = "";
    char            ticketNo[256] = "";
    QListViewItem   *curItem;
    QListViewItem   *parent = NULL;
    long            selNoteID = 0;
    char            searchStr[4096];
    char            escStr[4096];
    string          tmpNoteText;

    if (list->currentItem()) selNoteID = atol(list->currentItem()->key(NOTEIDCOL, 0));

    list->clear();
    noteText->setText("");

    if (strlen((const char *) searchText->text())) {
        strcpy(escStr, DB.escapeString((const char *) searchText->text()));
        sprintf(searchStr, " and (Subject LIKE '%%%s%%' or NoteText LIKE '%%%s%%') ", escStr, escStr);
    } else {
        strcpy(searchStr, " ");
    }
    
	// Fill in the Notes ListBox
	strcpy(tmpAcctStr, "");
	if (hideAccounting->isChecked()) strcpy(tmpAcctStr, "and NoteType <> 'Accounting'");
    DB.query("select * from Notes where CustomerID = %ld %s %s order by ParentNoteID, NoteID", myCustID, tmpAcctStr, searchStr);
	while(DB.getrow()) {
        tmpNoteText.assign(stripHTML(DB.curRow["NoteText"]));
        while (tmpNoteText.find("\n") != string::npos) tmpNoteText.replace(tmpNoteText.find("\n"), 1, " ");
        while (tmpNoteText.find("\r") != string::npos) tmpNoteText.replace(tmpNoteText.find("\r"), 1, " ");

		sprintf(tmpDateStamp, "%04d-%02d-%02d %02d:%02d",
                DB.curRow.col("NoteDate")->toQDateTime().date().year(),
                DB.curRow.col("NoteDate")->toQDateTime().date().month(),
                DB.curRow.col("NoteDate")->toQDateTime().date().day(),
                DB.curRow.col("NoteDate")->toQDateTime().time().hour(),
                DB.curRow.col("NoteDate")->toQDateTime().time().minute()
                );

        parent = NULL;
        if (atol(DB.curRow["ParentNoteID"])) {
            // We have a parent, find it in the list.
            QListViewItemIterator   tmpIt(list);
            for (; tmpIt.current(); ++tmpIt) {
                if (atol(tmpIt.current()->key(NOTEIDCOL, 0)) == atol(DB.curRow["ParentNoteID"])) {
                    parent = tmpIt.current();
                    break;
                }
            }
        } 

        // Check the ticket number.
        if (atoi(DB.curRow["TicketNo"])) {
            strcpy(ticketNo, DB.curRow["TicketNo"]);
        } else {
            strcpy(ticketNo, "");
        }
        
        if (parent) {
            // This note has a parent ID
            curItem = new QListViewItem(parent,
                //ticketNo,
                tmpDateStamp,
                DB.curRow["LoginID"],
                DB.curRow["Category"],
                //DB.curRow["Subject"],
                //stripHTML(DB.curRow["NoteText"]),
                tmpNoteText.c_str(), 
                DB.curRow["NoteID"],
                "0"                     // Standard Note
            );
        } else {
            // Standalone, or root level entry
            curItem = new QListViewItem(list,
                //ticketNo,
                tmpDateStamp,
                DB.curRow["LoginID"],
                DB.curRow["Category"],
                //DB.curRow["Subject"],
                //stripHTML(DB.curRow["NoteText"]),
                tmpNoteText.c_str(), 
                DB.curRow["NoteID"],
                "0"                     // Standard Note
            );
        }
	}

    // Get the tickets for this customer
    DB.query("select TicketNo from Tickets where CustomerID = %ld", myCustID);
	if (DB.rowCount) while(DB.getrow()) {
        Ticket  *tmpTick = new Ticket();
        tmpTick->setTicketNo(atol(DB.curRow["TicketNo"]));
        char    loginStr[1024];
        sprintf(loginStr, "[T%ld]", tmpTick->ticketNo());

        curItem = new QListViewItem(list,
            //ticketNo,
            tmpTick->openedStr(),
            loginStr,
            tmpTick->statusStr(),
            //DB.curRow["Subject"],
            //stripHTML(DB.curRow["NoteText"]),
            tmpTick->summary(),
            DB.curRow["TicketNo"],
            "1"                     // Ticket
        );
        delete tmpTick;
    }


    // If there was already a note selected, re-select it.
    // else, select the first one.
    bool foundIt = false;
    if (selNoteID) {
        QListViewItemIterator   tmpIt(list);
        for (; tmpIt.current(); ++tmpIt) {
            if (atol(tmpIt.current()->key(NOTEIDCOL, 0)) == selNoteID) {
                QListViewItem   *tmpCur = tmpIt.current();
                list->setCurrentItem(tmpCur);
                list->ensureItemVisible(tmpCur);
                list->setSelected(tmpCur, true);
                showNoteDetail(tmpCur);
                foundIt = true;
                break;
            }
        }
    } 
    // If we were unable to find the one we were looking for, show the
    // first entry in the list.
    if (!foundIt) {
        QListViewItem *tmpCur = list->firstChild();
        if (tmpCur) {
            list->setCurrentItem(tmpCur);
            list->ensureItemVisible(tmpCur);
            list->setSelected(tmpCur, true);
            showNoteDetail(tmpCur);
        }
    }
}

/*
** startSearch      - Refreshes the note list.
*/

void Tab_Notes::startSearch()
{
    refreshNotesList(1);
}

/*
** showNoteDetail	- A public slot which will get called when an item
**					  in the Logins list is changed.
**
*/

void Tab_Notes::showNoteDetail(QListViewItem *newItem)
{
    QApplication::setOverrideCursor(waitCursor);

    if (newItem) {
        if (atoi(newItem->key(NOTETYPECOL, 0)) == 0) {
            // Standard note.
            // For speed's sake, we loaded the entire list of notes into memory.
            // Now, lets get the row that we're interested in...
            
            ADBTable    detail("Notes");
            QString     tmpText;
            char        tmpDate[1024];

            long        myID = atol(newItem->key(NOTEIDCOL, 0));

            detail.get(myID);

            FParser     fparse;

            int     year;
            int     hour;
            char    ampm[64];

            strcpy(ampm, "a");
            hour = detail.getDateTime("NoteDate").time().hour();
            if (hour >= 12) {
                if (hour > 12) hour -= 12;
                strcpy(ampm, "p");
            }
            year = detail.getDateTime("NoteDate").date().year();
            while (year >= 100) year -= 100;

            sprintf(tmpDate, "%02d/%02d/%02d %02d:%02d%s",
                    detail.getDateTime("NoteDate").date().month(),
                    detail.getDateTime("NoteDate").date().day(),
                    year,
                    hour,
                    detail.getDateTime("NoteDate").time().minute(),
                    ampm
                    );

            fparse.setPath(cfgVal("TemplatePath"));
            fparse.set("LoginID",   detail.getStr("LoginID"));
            fparse.set("AddedBy",   detail.getStr("AddedBy"));
            fparse.set("DateTime",  tmpDate);
            fparse.set("NoteType",  detail.getStr("NoteType"));
            fparse.set("Subject",   detail.getStr("Subject"));
            fparse.set("NoteText",  detail.getStr("NoteText"));
            
            addedBy->setText(detail.getStr("AddedBy"));
            noteType->setText(detail.getStr("NoteType"));
            subject->setText(detail.getStr("Subject"));
            noteDate->setText(tmpDate);
            loginID->setText(detail.getStr("LoginID"));

            if (detail.getInt("Duration")) {
                int h,m,s;
                char    tmpSt[1024];
                s = detail.getInt("Duration");
                h = s / 3600;
                s -= h * 3600;
                m = s / 60;
                s -= m * 60;
                sprintf(tmpSt, "%dh %dm %ds", h, m, s);
                callDuration->setText(tmpSt);
            } else {
                callDuration->setText("N/A");
            }


            // Create the HTML for the first entry.
            tmpText.append(fparse.parseFileToMem("customers/NoteDetail.bhtml"));

            // Now, get any child entries and process them as well.
            QListViewItem *myChild = newItem->firstChild();
            if (myChild) while (myChild) {
                tmpText.append("<HR>\n");
                detail.get(atol(myChild->key(NOTEIDCOL, 0)));

                FParser     cparse;

                int     year;
                int     hour;
                char    ampm[64];

                strcpy(ampm, "a");
                hour = detail.getDateTime("NoteDate").time().hour();
                if (hour >= 12) {
                    if (hour > 12) hour -= 12;
                    strcpy(ampm, "p");
                }
                year = detail.getDateTime("NoteDate").date().year();
                while (year >= 100) year -= 100;

                sprintf(tmpDate, "%02d/%02d/%02d %02d:%02d%s",
                        detail.getDateTime("NoteDate").date().month(),
                        detail.getDateTime("NoteDate").date().day(),
                        year,
                        hour,
                        detail.getDateTime("NoteDate").time().minute(),
                        ampm
                        );

                cparse.setPath(cfgVal("TemplatePath"));
                cparse.set("LoginID",   detail.getStr("LoginID"));
                cparse.set("AddedBy",   detail.getStr("AddedBy"));
                cparse.set("DateTime",  tmpDate);
                cparse.set("NoteType",  detail.getStr("NoteType"));
                cparse.set("Subject",   detail.getStr("Subject"));
                cparse.set("NoteText",  detail.getStr("NoteText"));
            
                // Create the HTML for the first entry.
                tmpText.append(cparse.parseFileToMem("customers/NoteDetail.bhtml"));

                myChild = myChild->nextSibling();

            }

            noteText->setText(tmpText);
            noteText->setTextFormat(AutoText);
            ticketButton->setEnabled(false);
        } else if (atoi(newItem->key(NOTETYPECOL, 0)) == 1) {
            // Its a ticket
            Ticket  tmpTick;
            tmpTick.setTicketNo(atol(newItem->key(NOTEIDCOL, 0)));
            char    tmpType[1024];
            sprintf(tmpType, "Ticket (%s)", tmpTick.statusStr());

            addedBy->setText(tmpTick.owner());
            noteType->setText(tmpType);
            subject->setText(tmpTick.summary());
            noteDate->setText(tmpTick.openedStr());
            loginID->setText("N/A");
            callDuration->setText("N/A");
            noteText->setText(tmpTick.fullLog());
            ticketButton->setEnabled(true);
        }
    } else {
        // No entry is selected.
        ticketButton->setEnabled(false);
        noteText->setText("");
        addedBy->setText("");
        noteType->setText("");
        subject->setText("");
        noteDate->setText("");
        loginID->setText("");
        callDuration->setText("");
    }
    QApplication::restoreOverrideCursor();
}


/*
** newNote	- A public slot which will get called when the user wants to
**			  create a new note.
**
*/

void Tab_Notes::newNote()
{
	NoteEdit *note;
	note = new NoteEdit(0, "", myCustID);
	connect(note, SIGNAL(refresh(int)), SLOT(refreshNotesList(int)));
}

void Tab_Notes::hideAccountingChanged(bool)
{
	refreshNotesList(1);
}

/*
** showCallLog - Displays the customer's call log.
*/

void Tab_Notes::showCallLog()
{
    CustCallLogReport   *CCLR = new CustCallLogReport();
    CCLR->setCustID(myCustID);
    CCLR->setUserButton("&Browse");
    connect(CCLR, SIGNAL(userButtonClicked()), this, SLOT(openCallLogBrowser()));
    CCLR->show();
}

/*
** openCallLogBrowser - Opens the call log browser for the current customer.
*/

void Tab_Notes::openCallLogBrowser()
{
    CustCallDetail  *CCD = new CustCallDetail();
    CCD->setCustID(myCustID);
    CCD->show();
}


/*
** printNotes - Sets up the printer widget and prints the customers notes.
*/

void Tab_Notes::printNotes()
{
    QDate       theDate;
    QDateTime   timeStamp;
    QPrinter    prn;
    QPainter    p;
    QRect       rect;
    ADBTable    cust;
    ADBTable    cont;
    ADB         DB;
    QString     tmpSt;
    int         yPos;
    int         pageNo = 1;
    CustomersDB CDB;
    AddressesDB AddrDB;
    char        tmpDate[128];
    char        tmpTime[128];
    QString     notes;
    
    CDB.get(myCustID);
    AddrDB.get(REF_CUSTOMER, myCustID, "Billing");
    
    theDate = QDate::currentDate();
    
    // prn.setPrintProgram("ghostview");
    prn.setPrinterName("PostScript");
    // prn.setOutputFileName("/home/marc/test.ps");
    // prn.setOutputToFile(TRUE);
    prn.setPageSize(QPrinter::Letter);
    
    prn.setDocName("Customer Notes");
    prn.setCreator("Blarg! Online Services, Inc.");
    
    if (!prn.setup()) return;
    
    p.begin(&prn);
    
    // Put the Blarg header and contact information on the page.
    printHeader(&p, &CDB, &AddrDB);
    
    // Put the header on the page.
    noteHeader(&p);
    
    // Now, get the register information from the database.
    DB.query("select * from Notes where CustomerID = %ld order by NoteDate", myCustID);
    
    yPos = 165;
    while (DB.getrow()) {
        tmpSt = DB.curRow["Notes"];
        notes = tmpSt.replace(QRegExp("\n"), " ");
        tmpSt = notes.simplifyWhiteSpace();
        notes = tmpSt.stripWhiteSpace();
        int Lines = (int) (notes.length() / 52) + 1;
        int RowHeight = 15 * Lines;
        
        // Check to see if we have enough room on the page left for this
        // line.
        if (yPos+RowHeight >= 740) {
            printFooter(&p, pageNo++);
            prn.newPage();
            printHeader(&p, &CDB, &AddrDB);
            noteHeader(&p);
            yPos = 165;
        } 
    
        // The date and time of the note.
        myDateStamptoQDateTime(DB.curRow["NoteDate"], &timeStamp);
        sprintf(tmpDate, "%04d-%02d-%02d", 
          timeStamp.date().year(),
          timeStamp.date().month(),
          timeStamp.date().day()
        );
        
        sprintf(tmpTime, "%02d:%02d:%02d", 
          timeStamp.time().hour(),
          timeStamp.time().minute(),
          timeStamp.time().second()
        );
        
        rect.setCoords(20, yPos, 79, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, tmpDate);
        
        // The Due Date
        rect.setCoords(80, yPos, 139, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, tmpTime);
        
        // The Login ID
        rect.setCoords(140, yPos, 199, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignVCenter|AlignHCenter, DB.curRow["LoginID"]);
        
        // The note text itself...
        rect.setCoords(200, yPos, 459, yPos + RowHeight);
        p.drawRect(rect);
        rect.setCoords(203, yPos, 459, yPos + RowHeight);
        p.drawText(rect, WordBreak|AlignLeft|AlignVCenter, notes);
        
        // The note type.
        rect.setCoords(460, yPos, 519, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignLeft|AlignVCenter, DB.curRow["NoteType"]);
        
        // Who added it.
        rect.setCoords(520, yPos, 579, yPos + RowHeight);
        p.drawRect(rect);
        p.drawText(rect, AlignLeft|AlignVCenter, DB.curRow["AddedBy"]);
        
        yPos += RowHeight;
    }

    // Put the footer on the page.
    printFooter(&p, pageNo);
    
    // prn.newPage();
    
    // p.drawText(300, 600, "Page 2");
    
    p.end();
}


/*
** printHeader - Puts a header on the page containing the Blarg address,
**               the date, etc.
*/

void Tab_Notes::printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont)
{
    QDate       theDate;
    QRect       rect;
    QString     tmpSt;
    int         yPos;
    long        CustID = cust->getLong("CustomerID");

    theDate = QDate::currentDate();
    
    // p->rotate(55);
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(10, 30, "Blarg! Online Services, Inc.");
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText(10, 40, "P.O. Box 7385");
    p->drawText(10, 50, "Bellevue, WA 98008-1385");
    p->drawText(10, 60, "Phone:  425/401-9821");
    p->drawText(10, 70, "Fax: 425/401-9741");

    p->setFont(QFont("helvetica", 14, QFont::Bold));
    p->drawText(450, 40, "Customer Notes");
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    p->drawText(470, 60, theDate.toString());
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawLine(  0,  75, 692, 75);

    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText( 20, 90, "Customer:");
    yPos = 90;
    if (strlen(cust->getStr("FullName"))) {
        p->drawText( 70, yPos, cust->getStr("FullName"));
        yPos += 10;
    }
    if (strlen(cust->getStr("ContactName"))) {
        p->drawText( 70, yPos, cust->getStr("ContactName"));
        yPos += 10;
    }
    if (strlen(cont->Address1)) {
        p->drawText( 70, yPos, cont->Address1);
        yPos += 10;
    }
    if (strlen(cont->Address2)) {
        p->drawText( 70, yPos, cont->Address2);
        yPos += 10;
    }

    tmpSt = cont->City;
    tmpSt.append(", ");
    tmpSt.append(cont->State);
    tmpSt.append(" ");
    tmpSt.append(cont->ZIP);
    p->drawText( 70,yPos, tmpSt);
    yPos += 10;



    p->setFont(QFont("courier", 10, QFont::Normal));
    rect.setCoords(350,80,450, 93);
    p->drawText(rect, AlignRight|AlignVCenter, "Customer ID:");
    rect.setCoords(460,80,600, 93);
    p->drawText(rect, AlignLeft|AlignVCenter, cust->getStr("CustomerID"));

    p->setFont(QFont("courier", 10, QFont::Normal));

    PhoneNumbersDB  PDB;

    rect.setCoords(350,100,450,113);
    p->drawText(rect, AlignRight|AlignVCenter, "Day Phone:");
    rect.setCoords(460,100,600,113);
    PDB.get(REF_CUSTOMER, CustID, "Daytime");
    p->drawText(rect, AlignLeft|AlignVCenter, PDB.PhoneNumber);
    rect.setCoords(350,110,450,123);
    p->drawText(rect, AlignRight|AlignVCenter, "Eve Phone:");
    PDB.get(REF_CUSTOMER, CustID, "Evening");
    rect.setCoords(460,110,600,123);
    p->drawText(rect, AlignLeft|AlignVCenter, PDB.PhoneNumber);

}

/*
** printFooter - Puts a footer on the page containing the page number
*/

void Tab_Notes::printFooter(QPainter *p, int PageNo)
{
    char    tmpSt[1024];
    
    sprintf(tmpSt, "Page %d", PageNo);

    p->drawLine(  0, 740, 692,740);
    p->drawText(290, 760, tmpSt);
}

/*
** noteHeader - Puts a column header on the page.
*/

void Tab_Notes::noteHeader(QPainter *p)
{
    QRect       rect;
    QString     tmpSt;
    QBrush      bbrush;

    p->setFont(QFont("helvetica", 10, QFont::Normal));
//    p->drawRect(40, 150, 525, 15);
    bbrush.setStyle(SolidPattern);
    bbrush.setColor(black);

    p->setBackgroundMode(OpaqueMode);
    p->setPen(white);

    rect.setCoords(20, 150, 80, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Date");

    rect.setCoords(80, 150, 140, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Time");
    p->drawLine(80, 150, 80, 165);
        
    rect.setCoords(140, 150, 200, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Login ID");
    p->drawLine(140, 150, 140, 165);
        
    rect.setCoords(200, 150, 460, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Description");
    p->drawLine(200, 150, 200, 165);

    rect.setCoords(460, 150, 520, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Type");
    p->drawLine(460, 150, 460, 165);

    rect.setCoords(520, 150, 580, 165);
    p->fillRect(rect, bbrush);
    p->drawText(rect, AlignCenter, "Entered By");
    p->drawLine(520, 150, 520, 165);

    // Reset our pen back to a transparent background and black letters.
    p->setBackgroundMode(TransparentMode);
    p->setPen(black);
}

/*
** refreshCustomer - Gets called automagically by the mainWin() when we
**                   need to refresh.  Our argument will be a customer
**                   ID, if it is the customer we have loaded, we refresh
**                   our notes.  If it is not, we ignore it.
*/

void Tab_Notes::refreshCustomer(long custID)
{
    if (custID == myCustID) refreshNotesList(1);
}

/*
** ticketButtonPressed - Checks to see if the currently selected item
**                       is a ticket, and if it is, it emits the signal
**                       to open it.
*/

void Tab_Notes::ticketButtonPressed()
{
    if (list->currentItem()) {
        QListViewItem *curItem = list->currentItem();
        if (atoi(curItem->key(NOTETYPECOL, 0)) == 1) {
            emit(openTicket(atol(curItem->key(NOTEIDCOL, 0))));
        }
    }
}

/*
** refreshTicket - This will get called automatically if a ticket is
**                 ever changed.  We want to check to see if the currently
**                 selected note ID is the same as the ticket being changed.
**                 If it is, refresh it.
*/

void Tab_Notes::refreshTicket(long ticketNo)
{
    if (list->currentItem()) {
        QListViewItem *curItem = list->currentItem();
        if (atoi(curItem->key(NOTETYPECOL, 0)) == 1) {
            if (ticketNo == atol(curItem->key(NOTEIDCOL, 0))) {
                showNoteDetail(curItem);
            }
        }
    }
}

/*
** noteDoubleClicked - Gets called when a user double clicks on a note.
**                     If it is a ticket, we open the ticket, otherwise
**                     we do nothing.
*/

void Tab_Notes::noteDoubleClicked(QListViewItem *curItem)
{
    if (curItem) {
        if (atoi(curItem->key(NOTETYPECOL, 0)) == 1) {
            emit(openTicket(atol(curItem->key(NOTEIDCOL, 0))));
        }
    }
}

