/*
***************************************************************************
**
** OriginationProviders - An interface that allows an admin to maintain
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


#ifndef OriginationProviders_included
#define OriginationProviders_included

#include "TAATools.h"
#include "TAAWidget.h"
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <calendar.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>
#include <QtSql/QSqlDatabase>

class OriginationProviders : public TAAWidget
{
    Q_OBJECT

public:

    OriginationProviders(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~OriginationProviders();

public slots:
    virtual void    refreshList();
    virtual void    refreshProvider(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void addClicked();
    virtual void editClicked();
    virtual void deleteClicked();
    virtual void closeClicked();
    virtual void itemDoubleClicked(Q3ListViewItem *);

protected:
    Q3ListView       *opList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;
    QSqlDatabase    *myDB1;
    QSqlDatabase    *myDB2;

    // Things to mark the columns in the list view
    int             activeColumn;
    int             availColumn;
    int             totalColumn;
    int             idColumn;

};


class OriginationProviderEditor : public TAAWidget
{
    Q_OBJECT

public:

    OriginationProviderEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~OriginationProviderEditor();

    int             setProviderID(long);

signals:
    void        providerSaved(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void saveClicked();
    virtual void cancelClicked();

protected:
    long            originationID;
    QComboBox       *vendorList;
    QLineEdit       *tagLine;
    QComboBox       *deliveryMethod;
    QComboBox       *serverGroup;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;


};

#endif // OriginationProviders_included
