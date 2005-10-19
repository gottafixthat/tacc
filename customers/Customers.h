/*
** $Id: Customers.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Customers  - Handles the list of customers.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/


#ifndef Customers_included
#define Customers_included

#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qlineedit.h>
#include <qstrlist.h>
#include <qmap.h>

#include <TAAWidget.h>

typedef QMap<long, long>    loadedCustMap;

class Customers : public TAAWidget
{
    Q_OBJECT

public:

    Customers
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~Customers();
    QPopupMenu  *menu();

public slots:
    virtual void refreshList(long);
    
protected slots:
    void         refreshListV();
    virtual void listQueryS();
    virtual void custSelected(QListViewItem * sitem);
    
    // These are inhereted from TAAWidget
    virtual void refreshCustomer(long custID);

private slots:
    virtual void newCustomer();
    virtual void editCustomer();
    virtual void addCustNote();
    virtual void receivePayment();
    virtual void clearQuery();
    virtual void takeCall();
    virtual void newCustWizard();
    virtual void globalAliases();

protected:
    QLabel      *queryLabel;
    QLineEdit   *listQuery;
    QCheckBox   *autoOpenCustomer;
    QListView   *list;
    QPopupMenu  *optionsMenu;

    QLabel      *phoneNumberLabel;
    QLabel      *phoneNumber;
    QLabel      *telcoNameLabel;
    QLabel      *telcoName;
    QLabel      *LATALabel;
    QLabel      *LATA;
    QLabel      *serviceAreaLabel;
    QLabel      *serviceArea;
    QLabel      *dslQualLabel;
    QLabel      *dslQual;
    QLabel      *iHostLabel;
    QLabel      *iHost;
    QLabel      *locationLabel;
    QLabel      *location;

signals:
    void    refreshTabs(int);
    
private:
    loadedCustMap   loadedCusts;

};
#endif // Customers_included
