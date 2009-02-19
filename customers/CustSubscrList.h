/*
** $Id: CustSubscrList.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CustSubscrList - A widget that allows viewing and editing subscriptions.
**                  It can be swallowed into other widgets, like tab
**                  dialogs, etc.
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
** $Log: CustSubscrList.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef CustSubscrList_included
#define CustSubscrList_included

#include <TAAWidget.h>
#include <SelectionList.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

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
    QListView   *list;

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
