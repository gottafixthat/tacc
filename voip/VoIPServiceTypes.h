/*
***************************************************************************
**
** VoIPServiceTypes - An interface that allows an admin to maintain
** the VoIP Service Types.
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


#ifndef VoIPServiceTypes_included
#define VoIPServiceTypes_included

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
#include <QtGui/QSpinBox>

#include <ServerGroups.h>

class VoIPServiceTypes : public TAAWidget
{
    Q_OBJECT

public:

    VoIPServiceTypes(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~VoIPServiceTypes();

public slots:
    virtual void    refreshList();
    virtual void    refreshServiceItem(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void addClicked();
    virtual void editClicked();
    virtual void deleteClicked();
    virtual void closeClicked();
    virtual void itemDoubleClicked(Q3ListViewItem *);

protected:
    Q3ListView       *vstList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;

    // Things to mark the columns in the list view
    int             idColumn;

};


class VoIPServiceTypeEditor : public TAAWidget
{
    Q_OBJECT

public:

    VoIPServiceTypeEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual         ~VoIPServiceTypeEditor();

    int             setServiceID(long);

signals:
    void            serviceItemSaved(long);

protected slots:
    virtual void    QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void    saveClicked();
    virtual void    cancelClicked();
    virtual void    addFlagClicked();
    virtual void    availableFlagDoubleClicked(Q3ListViewItem *);
    virtual void    assignedFlagDoubleClicked(Q3ListViewItem *);
    virtual void    rmFlagClicked();

protected:
    long                voipServiceID;
    
    QLineEdit           *serviceName;
    QComboBox           *serviceType;
    QComboBox           *billableItem;
    QSpinBox            *baseChannels;
    QComboBox           *channelBillable;
    Q3ListView           *flagsAvailable;
    Q3ListView           *flagsAssigned;
    ServerGroupSelector *serverGroups;
    QPushButton         *saveButton;
    QPushButton         *cancelButton;

};

#endif // VoIPServiceTypes_included
