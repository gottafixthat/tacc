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


#ifndef Customers_included
#define Customers_included

#include <QtCore/QMap>
#include <QtGui/QAction>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTreeWidget>

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
    QMenu      *menu()     { return optionsMenu; };

public slots:
    virtual void refreshList(long);
    void         customerSearch(const char *);
    
protected slots:
    void         refreshListV();
    virtual void listQueryS();
    virtual void custSelected(QTreeWidgetItem *sitem, int);
    
    // These are inhereted from TAAWidget
    virtual void refreshCustomer(long custID);

private slots:
    virtual void newCustomer();
    virtual void editCustomer();
    virtual void addCustNote();
    virtual void receivePayment();
    virtual void clearQuery();
    virtual void takeCall();
    virtual void globalAliases();

protected:
    QLabel      *queryLabel;
    QLineEdit   *listQuery;
    QCheckBox   *autoOpenCustomer;
    QTreeWidget  *list;
    QMenu       *optionsMenu;

    QAction     *actNew;
    QAction     *actEdit;
    QAction     *actAddNote;
    QAction     *actReceivePayment;
    QAction     *actDomainAliases;

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

// vim: expandtab

