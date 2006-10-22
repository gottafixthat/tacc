/*
***************************************************************************
**
** DIDManager - An interface that allows an admin to maintain DID's
** for our VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/


#ifndef DIDManager_included
#define DIDManager_included

#include "TAATools.h"
#include "TAAWidget.h"
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qmultilineedit.h>
#include <calendar.h>
#include <qtextview.h>
#include <qpopupmenu.h>

class DIDManager : public TAAWidget
{
    Q_OBJECT

public:

    DIDManager(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DIDManager();

/*
public slots:
    virtual void    refreshList();
    virtual void    listItemChanged(QListViewItem *);

protected slots:
    virtual void loginIDChanged(const QString &);
    virtual void QCloseEvent(QEvent *)      { delete this; }
    virtual void saveClicked();

protected:
    QListView       *vmList;
    QLabel          *msgDate;
    QLabel          *msgTime;
    QFrame          *msgTimeArea;
    QLabel          *loggedBy;
    QLabel          *loggedAt;
    QTextView       *message;
    QPushButton     *saveButton;
    QMultiLineEdit  *followUp;
    QComboBox       *disposition;
    QLineEdit       *loginID;
    QLabel          *customerID;
    QLabel          *customerName;
    QCheckBox       *processed;
    QPopupMenu      *vmMenu;
    long            myCustID;
    int             myVMCount;
*/
};


// A class for adding DID's
class DIDManagerAdd : public TAAWidget
{
    Q_OBJECT

public:

    DIDManagerAdd(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DIDManagerAdd();

//public slots:

protected slots:
    virtual void doneClicked();
    void         countrySelected(const QString &);
    void         stateSelected(const QString &);

protected:
    QLineEdit       *npa;
    QLineEdit       *nxx;
    QLineEdit       *numStart;
    QLineEdit       *numStop;
    QComboBox       *countryList;
    QComboBox       *stateList;
    QComboBox       *cityList;
    QLabel          *messageArea;

    QPushButton     *addButton;
    QPushButton     *doneButton;
    
};
#endif // DIDManager_included