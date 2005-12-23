/*
** $Id$
**
***************************************************************************
**
** TAAWidget - The TAAWidget is the base class for all of the windows and
**             widgets in Total AccountAbility.  It connects various
**             various signals and slots to the MainWin for automatic
**             inter-widget communication.
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
** $Log: TAAWidget.cpp,v $
** Revision 1.2  2003/12/30 18:44:56  marc
** Added a new real-time setProgress signal.
**
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <qapp.h>
#include <qwidget.h>

#include <TAATools.h>
#include "TAAWidget.h"



TAAWidget::TAAWidget(QWidget *parent, const char *name, WFlags f)
    : QWidget(parent, name, f)
{
    // Connect the relay slots to the main window.
    connect(this, SIGNAL(openCustomer(long)), mainWin(), SLOT(openCustomerMW(long)));
    connect(this, SIGNAL(openTicket(long)), mainWin(), SLOT(openTicketMW(long)));
    connect(this, SIGNAL(setStatus(const char *)), mainWin(), SLOT(setStatusMW(const char *)));
    connect(this, SIGNAL(setProgress(int, int)), mainWin(), SLOT(setProgressMW(int, int)));
    connect(this, SIGNAL(setProgressRT(int, int)), mainWin(), SLOT(setProgressMWRT(int, int)));

    // Relay slots.  Individual TAAWidgets will emit customerUpdated() whenever
    // they update customer information.  This, in turn, will call the 
    // mainWin() member function refreshCustomerMW().  refreshCustomerMW()
    // will then emit refreshCustomer(), which is a virtual function contained
    // by every TAAWidget instance.  This makes it so any window in TAA
    // can cause updates to every other open window in the system without
    // keeping a list of windows.
    connect(this, SIGNAL(customerUpdated(long)), mainWin(), SLOT(refreshCustomerMW(long)));
    connect(mainWin(), SIGNAL(refreshCustomer(long)), this, SLOT(refreshCustomer(long)));
    connect(this, SIGNAL(ticketUpdated(long)), mainWin(), SLOT(refreshTicketMW(long)));
    connect(mainWin(), SIGNAL(refreshTicket(long)), this, SLOT(refreshTicket(long)));
}

TAAWidget::~TAAWidget()
{
}


    
HorizLine::HorizLine(QWidget *parent, const char *name, WFlags f)
    : QLabel(parent, name, f)
{
    setFrameStyle(QFrame::HLine|QFrame::Sunken);
    setMinimumSize(0,4);
    setMaximumSize(32767, 4);
}

HorizLine::~HorizLine()
{
}

VertLine::VertLine(QWidget *parent, const char *name, WFlags f)
    : QLabel(parent, name, f)
{
    setFrameStyle(QFrame::VLine|QFrame::Sunken);
    setMinimumSize(4,0);
    setMaximumSize(4,32767);
}

VertLine::~VertLine()
{
}

