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


#ifndef CompanyEditor_included
#define CompanyEditor_included

#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3popupmenu.h>

#include <calendar.h>
#include <TAATools.h>
#include <TAAWidget.h>

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
    virtual void itemDoubleClicked(Q3ListViewItem *);

protected:
    Q3ListView       *coList;
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


// vim: expandtab
