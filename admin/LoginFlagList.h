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

#ifndef LoginFlagList_included
#define LoginFlagList_included

#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QCloseEvent>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3listview.h>

#include <calendar.h>
#include <TAATools.h>
#include <TAAWidget.h>

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
    virtual void itemDoubleClicked(Q3ListViewItem *);

protected:
    Q3ListView       *flagList;
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
    Q3MultiLineEdit  *description;
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
    virtual void availableDoubleClicked(Q3ListViewItem *);
    virtual void assignedDoubleClicked(Q3ListViewItem *);

protected:
    long            idColumn;
    Q3ListView       *available;
    Q3ListView       *assigned;
    QPushButton     *addButton;
    QPushButton     *rmButton;
};
 
#endif // LoginFlagList_included

// vim: expandtab
