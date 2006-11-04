/*
***************************************************************************
**
** ServerGroups - An interface that allows an admin to maintain
** the list of origination providers.
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


#ifndef ServerGroups_included
#define ServerGroups_included

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
#include <qsqldatabase.h>

class ServerGroups : public TAAWidget
{
    Q_OBJECT

public:

    ServerGroups(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~ServerGroups();

public slots:
    virtual void    refreshList();
    virtual void    refreshServerGroup(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void addClicked();
    virtual void editClicked();
    virtual void deleteClicked();
    virtual void closeClicked();
    virtual void itemDoubleClicked(QListViewItem *);

protected:
    QListView       *sgList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;
    QSqlDatabase    *myDB1;
    QSqlDatabase    *myDB2;

    // Things to mark the columns in the list view
    int             idColumn;

};


class ServerGroupEditor : public TAAWidget
{
    Q_OBJECT

public:

    ServerGroupEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~ServerGroupEditor();

    int             setServerGroupID(long);

signals:
    void        serverGroupSaved(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void saveClicked();
    virtual void cancelClicked();

protected:
    long            serverGroupID;
    QLineEdit       *serverGroup;
    QMultiLineEdit  *description;
    QLineEdit       *databaseHost;
    QLineEdit       *databaseName;
    QLineEdit       *databaseUser;
    QLineEdit       *databasePass1;
    QLineEdit       *databasePass2;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;


};

/**
  * ServerGroupSelector is a widget group that displays two columns of
  * server groups, available and assigned.
  */
class ServerGroupSelector : public TAAWidget
{
    Q_OBJECT

public:

    ServerGroupSelector (
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~ServerGroupSelector();

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
 
#endif // ServerGroups_included
