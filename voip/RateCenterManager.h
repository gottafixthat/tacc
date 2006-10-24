/*
***************************************************************************
**
** RateCenterManager - An interface that allows an admin to maintain
** the list of rate centers that we provide DID's in.
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


#ifndef RateCenterManager_included
#define RateCenterManager_included

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

class RateCenterManager : public TAAWidget
{
    Q_OBJECT

public:

    RateCenterManager(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~RateCenterManager();

public slots:
    virtual void    refreshRateCenter(long);

protected slots:
    virtual void    refreshRateCenters();
    virtual void    QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void    addClicked();
    virtual void    editClicked();
    virtual void    deleteClicked();
    virtual void    closeClicked();

protected:
    QListView       *rcList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;

    // Things to mark the columns in the list view
    int             activeColumn;
    int             availColumn;
    int             totalColumn;
    int             idColumn;

};


class RateCenterEditor : public TAAWidget
{
    Q_OBJECT

public:

    RateCenterEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~RateCenterEditor();

    int         setRateCenterID(long);

signals:
    void        rateCenterSaved(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void saveClicked();
    virtual void cancelClicked();
    void         countrySelected(const QString &);

protected:
    long            rateCenterID;
    QComboBox       *countryList;
    QComboBox       *stateList;
    QLineEdit       *city;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;
};

#endif // RateCenterManager_included
