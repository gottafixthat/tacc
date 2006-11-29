/*
***************************************************************************
**
** CompanyEditor - An interface that allows an admin to maintain
** the list of companies that TACC manages.
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


#ifndef CompanyEditor_included
#define CompanyEditor_included

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

class CompanyList : public TAAWidget
{
    Q_OBJECT

public:

    CompanyList (
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~CompanyList();

public slots:
    virtual void    refreshList();
    virtual void    refreshCompany(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { closeClicked(); }
    virtual void addClicked();
    virtual void editClicked();
    virtual void deleteClicked();
    virtual void closeClicked();
    virtual void itemDoubleClicked(QListViewItem *);

protected:
    QListView       *coList;
    QPushButton     *addButton;
    QPushButton     *editButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;

    // Things to mark the columns in the list view
    int             idColumn;

};


class CompanyEditor : public TAAWidget
{
    Q_OBJECT

public:

    CompanyEditor (
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~CompanyEditor();

    int             setCompanyID(long);

signals:
    void            companySaved(long);

protected slots:
    virtual void QCloseEvent(QEvent *)      { cancelClicked(); }
    virtual void saveClicked();
    virtual void cancelClicked();

protected:
    long            companyID;
    QLineEdit       *companyTag;
    QLineEdit       *companyName;
    QLineEdit       *address1;
    QLineEdit       *address2;
    QLineEdit       *city;
    QLineEdit       *state;
    QLineEdit       *zip;
    QLineEdit       *mainPhone;
    QLineEdit       *altPhone;
    QLineEdit       *faxPhone;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;

};

#endif // CompanyEditor_included

