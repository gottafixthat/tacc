/*
***************************************************************************
**
** LoginFlagList - An interface that allows an admin to maintain
** the list of flags that can be associated with login types.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
***************************************************************************
*/


#ifndef LoginFlagList_included
#define LoginFlagList_included

#include "TAATools.h"
#include "TAAWidget.h"
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qmultilineedit.h>
#include <calendar.h>
#include <qtextview.h>
#include <qpopupmenu.h>
#include <qsqldatabase.h>

class LoginFlagList : public TAAWidget
{
    Q_OBJECT

public:

    LoginFlagList(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~LoginFlagList();

public slots:
    virtual void    refreshList();
    virtual void    refreshLoginFlag(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void addClicked();
    virtual void editClicked();
    virtual void deleteClicked();
    virtual void closeClicked();
    virtual void itemDoubleClicked(QListViewItem *);

protected:
    QListView       *flagList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;
    QSqlDatabase    *myDB1;
    QSqlDatabase    *myDB2;

    // Things to mark the columns in the list view
    int             idColumn;

};


class LoginFlagEditor : public TAAWidget
{
    Q_OBJECT

public:

    LoginFlagEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~LoginFlagEditor();

    int         setLoginFlagID(long);

signals:
    void        loginFlagSaved(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void saveClicked();
    virtual void cancelClicked();

protected:
    long            loginFlagID;
    QLineEdit       *loginFlag;
    QMultiLineEdit  *description;
    QCheckBox       *isBool;
    QLineEdit       *defaultValue;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;


};

/**
  * ServerGroupSelector is a widget group that displays two columns of
  * server groups, available and assigned.
  */
class LoginFlagSelector : public TAAWidget
{
    Q_OBJECT

public:

    LoginFlagSelector (
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~LoginFlagSelector();

    void    setAddButtonText(const char *);
    void    setRemoveButtonText(const char *);
    void    reset();
    void    assign(long);
    void    unassign(long);
    long    *getAssigned();

protected slots:
    virtual void addClicked();
    virtual void rmClicked();
    virtual void availableDoubleClicked(QListViewItem *);
    virtual void assignedDoubleClicked(QListViewItem *);

protected:
    long            idColumn;
    QListView       *available;
    QListView       *assigned;
    QPushButton     *addButton;
    QPushButton     *rmButton;
};
 
#endif // LoginFlagList_included
