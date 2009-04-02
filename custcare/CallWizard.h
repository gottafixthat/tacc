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

#ifndef CALLWIZARD_H 
#define CALLWIZARD_H

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QLCDNumber>
#include <Qt3Support/q3widgetstack.h>

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
    Q3WidgetStack        *mainArea;
    CallTypeSelector    *typeSel;
    int                 timerRunning;

};

int main(int argc, char **argv);

#endif // CALLWIZARD_H

// vim: expandtab
