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
#include <qspinbox.h>

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

protected:
    QListView       *vstList;
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
    virtual void    rmFlagClicked();

protected:
    long            voipServiceID;
    
    QLineEdit       *serviceName;
    QComboBox       *serviceType;
    QComboBox       *billableItem;
    QSpinBox        *baseChannels;
    QComboBox       *channelBillable;
    QListView       *flagsAvailable;
    QListView       *flagsAssigned;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;

};

#endif // VoIPServiceTypes_included
