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

#ifndef CustSubscrList_included
#define CustSubscrList_included

#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

#include <TAAWidget.h>
#include <SelectionList.h>

class CustSubscrList : public TAAWidget
{
    Q_OBJECT

public:

    CustSubscrList
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~CustSubscrList();
    
public slots:
	virtual void refreshCustomer(long);

private slots:
	virtual void newSubscription();
	virtual void editSubscription();
	virtual void deleteSubscription();
	virtual void hideInactiveChanged();
	virtual void addPackageItem();

protected:
    QPushButton *newButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QCheckBox   *hideInactiveButton;
    Q3ListView   *list;

private:
	long	myCustID;
    int     subscriptionIDCol;
	int     showInactive;

};


class PackageSelector : public SelectionList
{
    Q_OBJECT
    
public:
    PackageSelector 
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );
    virtual ~PackageSelector();
    
    virtual void setCustomerID(long CustID);
    const char *getLoginID(void);
 
//protected:
//    QComboBox   *loginList;   
};
    
#endif // CustSubscrList_included

// vim: expandtab
