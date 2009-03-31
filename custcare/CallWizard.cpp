/*
** $Id$
**
***************************************************************************
**
** CallWizard - Creates an assload of widgets in a QWidgetStack and brings
**              up the appropriate one based on user entry.
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
** $Log: CallWizard.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "CallWizard.h"
#include <CustomerSearch.h>

#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <QLabel>
#include <Q3Frame>

#include <stdio.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <q3header.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qrect.h>
#include <qstatusbar.h>
#include <qpushbutton.h>
#include <math.h>
#include <qlayout.h>
#include <q3widgetstack.h>

CallWizard::CallWizard
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption("Call Logger");
    titleBar = new QStatusBar(this);
    titleBar->setSizeGripEnabled(false);
    titleBar->message("Call Logger");

    QLabel *hline1 = new QLabel(this);
    hline1->setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    hline1->setMaximumHeight(3);

    hangupButton   = new QPushButton(this, "Hangup Button");
    hangupButton->setText("Han&g Up");
    connect(hangupButton, SIGNAL(clicked()), this, SLOT(stopTimer()));

    transferButton = new QPushButton(this, "Transfer Button");
    transferButton->setText("&Transfer");
    transferButton->hide();

    proceedButton = new QPushButton(this, "Proceed Button");
    proceedButton->setText("&Proceed");
    
    QLabel *hline2 = new QLabel(this);
    hline2->setFrameStyle(Q3Frame::HLine|Q3Frame::Sunken);
    hline2->setMaximumHeight(3);
    status = new QStatusBar(this);
    status->message("Status bar here.");

    mainArea = new Q3WidgetStack(this);
    typeSel = new CallTypeSelector(mainArea, "CallTypeSelector");
    mainArea->addWidget(typeSel, 0);
    mainArea->raiseWidget(0);

    // Now, do the layout for the widget.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 0, 3);
    ml->addWidget(titleBar, 0);
    ml->addWidget(hline1, 0);
    ml->addWidget(mainArea, 1);

    Q3BoxLayout  *butlay = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    butlay->addStretch(1);
    butlay->addWidget(hangupButton, 0);
    butlay->addWidget(transferButton, 0);
    butlay->addWidget(proceedButton, 0);
    butlay->addSpacing(3);

    ml->addLayout(butlay, 0);
    ml->addWidget(hline2, 0);
    ml->addWidget(status, 0);

    callStart = QDateTime::currentDateTime();
    callEnd   = QDateTime::currentDateTime();

    clockLCD = new QLCDNumber(status, "Clock Timer");
    clockLCD->setMode(QLCDNumber::DEC);
    clockLCD->setSegmentStyle(QLCDNumber::Flat);
    clockLCD->setNumDigits(7);
    clockLCD->display("0:00:00");

    status->addWidget(clockLCD, 0, true);

    clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(updateClock()));
    clockTimer->start(1000);
    timerRunning = 1;

    // Make it the same width as an 80 column wide RXVT terminal window.
    resize(499, 385);
}



CallWizard::~CallWizard()
{
}

/*
** updateClock - Updates the call timer.  Called automatically with
**               a QTimer.
*/

void CallWizard::updateClock()
{
    if (!timerRunning) return;

    char    tmpStr[1024];
    QTime   tmpTime(0,0,0,0);
    QTime   elapsed(0,0,0,0);
    int     tmpSecs;

    callEnd = QDateTime::currentDateTime();

    tmpSecs = callStart.secsTo(callEnd);
    elapsed = tmpTime.addSecs(tmpSecs);

    sprintf(tmpStr, "%2d:%02d:%02d", elapsed.hour(), elapsed.minute(), elapsed.second());
    clockLCD->display(tmpStr);

}

/*
** stopTimer - Stops the call timer.
*/

void CallWizard::stopTimer()
{
    timerRunning = false;
    hangupButton->setEnabled(false);
}


