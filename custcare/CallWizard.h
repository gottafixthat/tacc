/*
** $Id: CallWizard.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CallWizard - Starts an incoming call wizard.
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
** $Log: CallWizard.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef CALLWIZARD_H 
#define CALLWIZARD_H

#include <qwidget.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qlcdnumber.h>
#include <qdatetime.h>
#include <qwidgetstack.h>
#include <TAAWidget.h>

#include "CallTypeSelector.h"

class CallWizard : public TAAWidget
{
    Q_OBJECT

public:
    CallWizard( QWidget *parent = NULL, const char *name = NULL);
    virtual ~CallWizard();

protected slots:
    void            updateClock();
    void            stopTimer();

protected:
    QLCDNumber          *clockLCD;
    QLabel              *clockLabel;
    QTimer              *clockTimer;
    QStatusBar          *status;
    QStatusBar          *titleBar;
    QPushButton         *hangupButton;
    QPushButton         *proceedButton;
    QPushButton         *transferButton;
    QDateTime           callStart;
    QDateTime           callEnd;
    QWidgetStack        *mainArea;
    CallTypeSelector    *typeSel;
    int                 timerRunning;

};

int main(int argc, char **argv);

#endif // CALLWIZARD_H
