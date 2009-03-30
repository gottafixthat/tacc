/*
** $Id: LogCall.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** LogCall - An interface for the operator to log a call with.
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
** $Log: LogCall.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef LogCall_included
#define LogCall_included

#include <TAAWidget.h>

#include <qdatetm.h>
#include <qtimer.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qmultilinedit.h>
#include <qlcdnumber.h>
#include <qlineedit.h>

class LogCall : public TAAWidget
{
    Q_OBJECT

public:

    LogCall
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~LogCall();

    void    setLoginID(const char * newLoginID);

protected slots:
    virtual void callTypeChanged(int newType);
    virtual void loadCustomer();
    virtual void loginIDChanged(const char * newLoginID);
    virtual void loginIDChanged(const QString &newLoginID);
    virtual void hangupCall();
    
    virtual void toggleTimer();

protected:
    QLabel          *loginIDLabel;
    QLineEdit       *loginID;
    QPushButton     *searchButton;
    QLabel          *typeLabel;
    QComboBox       *callType;
    QLabel          *reasonLabel;
    QListView       *reasonList;
    QLabel          *solutionLabel;
    QMultiLineEdit  *solutionNotes;
    QPushButton     *appendButton;
    QPushButton     *clockButton;
    QPushButton     *viewLogButton;
    QPushButton     *doneButton;
    QPushButton     *custWinButton;
    QLabel          *statusLabel;
    QLCDNumber      *clockArea;

private:

    virtual     void QCloseEvent(QEvent *)          { delete this; }

    QListView   *supportList;
    QListView   *accountingList;
    QListView   *salesList;

    QDateTime   callStart;
    QDateTime   callEnd;
    
    QTimer      *callTimer;
    
    char        myLoginID[64];
    char        custName[256];
    long        myCustID;
    
    int         timerRunning;
    
    long        myLinkedCall;

private slots:
    void        updateClock();

};



#endif // LogCall_included
