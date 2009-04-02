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

#ifndef TAAWIDGET_H
#define TAAWIDGET_H

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>

/*!
 * \brief TACCWindow is a replacement for the Qt 3 main window functions.
 * TAA GUI applications should create *ONE* TACCWindow.
 *
 * TACCWindow is the main application window for the system.  The TAAWidget
 * class will connect its relay slots to TACCWindow objects, this includes
 * relay messages, such as status bar updates, progress bars, opening customers
 * or tickets, etc.
 */
/*
class TACCWindow : public QWidget
{
    Q_OBJECT

public:
    TACCWindow(QWidget *parent Qt::WFlags f = Qt::Window);
    virtual ~TACCWIndow();

public slots:
    virtual void updateStatusBar(QString)           {};
    virtual void updateStatusBar(QString, int)      {};
    virtual void updateProgressBar(int, int)        {};
    virtual void updateProgressBarRT(int, int)      {};
    virtual void openCustomerWindow(long)           {};
    virtual void openTicketWindow(long)             {};

};
*/

class TAAWidget : public QWidget
{
    Q_OBJECT

public:
    TAAWidget(QWidget *parent = NULL, Qt::WFlags f = Qt::Widget);
    TAAWidget(QWidget *parent = NULL, const char *name = NULL, Qt::WFlags f = Qt::Widget);
    virtual ~TAAWidget();

    void        setUserPref(const char *key, const char *subkey, const char *val);
    const char *getUserPref(const char *key, const char *subkey);

signals:
    // Status signals.
    void    setStatus(const char *);
    void    setStatus(const char *, int);
    void    setProgress(int, int);
    void    setProgressRT(int, int);

    // Customer related signals
    void    customerUpdated(long);
    void    openCustomer(long);

    // Ticketing system related signals
    void    ticketUpdated(long);
    void    openTicket(long);

public slots:
    // Any widget that loads customer information should override these
    // member function.  They will be called automatically whenever
    // another customer function updates a customers data.
    virtual void    refreshCustomer(long)           {};
    virtual void    refreshTicket(long)             {};

};


// A horizontal line class.

class HorizLine : public QLabel
{
    Q_OBJECT

public:
    HorizLine(QWidget *parent = NULL, const char *name = NULL, Qt::WFlags f = 0);
    virtual ~HorizLine();
};

// A vertical line class.

class VertLine : public QLabel
{
    Q_OBJECT

public:
    VertLine(QWidget *parent = NULL, const char *name = NULL, Qt::WFlags f = 0);
    virtual ~VertLine();
};

#endif // TAAWIDGET_H



// vim: expandtab

