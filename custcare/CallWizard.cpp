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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QDateTime>
#include <QtCore/QRect>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <QtGui/QStatusBar>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3header.h>

#include <CustomerSearch.h>

#include "CallWizard.h"

using namespace Qt;

CallWizard::CallWizard
(
	QWidget* parent,
	const char* name
) : TAAWidget(parent)
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



// vim: expandtab
