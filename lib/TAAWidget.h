/*
** $Id: TAAWidget.h,v 1.3 2003/12/30 18:44:56 marc Exp $
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
** $Log: TAAWidget.h,v $
** Revision 1.3  2003/12/30 18:44:56  marc
** Added a new real-time setProgress signal.
**
** Revision 1.2  2003/12/15 06:26:13  marc
** Minor change to cause it to delete the widget when closing.
**
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef TAAWIDGET_H
#define TAAWIDGET_H

#include <qapp.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qlabel.h>

class TAAWidget : public QWidget
{
    Q_OBJECT

public:
    TAAWidget(QWidget *parent = NULL, const char *name = NULL, WFlags f = WDestructiveClose);
    virtual ~TAAWidget();

    void        setUserPref(const char *key, const char *subkey, const char *val);
    const char *getUserPref(const char *key, const char *subkey);

signals:
    // Status signals.
    void    setStatus(const char *);
    void    setProgress(int, int);
    void    setProgressRT(int, int);

    // Customer related signals
    void    customerUpdated(long);
    void    openCustomer(long);

    // Ticketing system related signals
    void    ticketUpdated(long);
    void    openTicket(long);

public slots:
    // Any widget that loads customer information should override this
    // member function.  It will be called automatically whenever
    // another customer function updates a customers data.
    virtual void    refreshCustomer(long)           {};
    virtual void    refreshTicket(long)             {};

};


// A horizontal line class.

class HorizLine : public QLabel
{
    Q_OBJECT

public:
    HorizLine(QWidget *parent = NULL, const char *name = NULL, WFlags f = 0);
    virtual ~HorizLine();
};

// A vertical line class.

class VertLine : public QLabel
{
    Q_OBJECT

public:
    VertLine(QWidget *parent = NULL, const char *name = NULL, WFlags f = 0);
    virtual ~VertLine();
};

#endif // TAAWIDGET_H


