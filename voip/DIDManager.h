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

#include <TAATools.h>
#include <TAAWidget.h>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>

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
    virtual void addClicked();
    virtual void doneClicked();
    void         vendorSelected(const QString &);
    void         countrySelected(const QString &);
    void         stateSelected(const QString &);
    void         reserveTypeChanged(int);

protected:
    QString         errorBoxText;
    QLineEdit       *npa;
    QLineEdit       *nxx;
    QLineEdit       *numStart;
    QLineEdit       *numStop;
    QComboBox       *vendorList;
    QComboBox       *groupList;
    QComboBox       *countryList;
    QComboBox       *stateList;
    QComboBox       *cityList;
    QComboBox       *reserved;
    Q3DateEdit       *reserveDate;
    QLabel          *messageArea;

    QPushButton     *addButton;
    QPushButton     *doneButton;
    
};
#endif // DIDManager_included
