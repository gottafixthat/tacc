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

#ifndef LogCall_included
#define LogCall_included

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QLCDNumber>
#include <QtGui/QLineEdit>
#include <QtGui/QCloseEvent>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3multilineedit.h>

#include <TAAWidget.h>

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
    Q3ListView       *reasonList;
    QLabel          *solutionLabel;
    Q3MultiLineEdit  *solutionNotes;
    QPushButton     *appendButton;
    QPushButton     *clockButton;
    QPushButton     *viewLogButton;
    QPushButton     *doneButton;
    QPushButton     *custWinButton;
    QLabel          *statusLabel;
    QLCDNumber      *clockArea;

private:

    virtual     void QCloseEvent(QEvent *)          { delete this; }

    Q3ListView   *supportList;
    Q3ListView   *accountingList;
    Q3ListView   *salesList;

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

// vim: expandtab
