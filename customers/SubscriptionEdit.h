/*
** $Id: SubscriptionEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** SubscriptionEdit - Allows the user to edit a subscription item that has
**                    been assigned to a customer.
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
** $Log: SubscriptionEdit.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef SubscriptionEdit_included
#define SubscriptionEdit_included

#include <TAAWidget.h>
#include <q3strlist.h>
#include <calendar.h>
#include <qdatetm.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <q3multilineedit.h>
#include <qlineedit.h>
#include <qcombobox.h>

class SubscriptionEdit : public TAAWidget
{
    Q_OBJECT

public:

    SubscriptionEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0,
        long SubscrID = 0
    );

    virtual ~SubscriptionEdit();

signals:
    void refresh(int);

private slots:
    void cancelSubscription();
    void saveSubscription();
    void itemIDChanged(int);
    void loginIDChanged(const QString &);
    void updateFollowItem(bool);
    
protected:
    QLabel          *priceLabel;
    QLabel          *customerName;
    QLabel          *primaryLogin;
    QLabel          *ratePlan;
    QLabel          *billingCycle;
    QComboBox       *itemID;
    QComboBox       *loginID;
    QLineEdit       *quantity;
    QLineEdit       *price;
    QCheckBox       *autoRenew;
    QCheckBox       *active;
    QCheckBox       *followItem;
    QCheckBox       *setupCharged;
    QLabel          *descriptionLabel;
    Q3MultiLineEdit  *description;
    DateInput       *lastDate;
    DateInput       *endDate;

private:
    long            myCustID;
    long            mySubscrID;
    char            myCycleID[64];
    char            myParentID[64];
    long            *itemIndex;
    Q3StrList        *loginIndex;
    
    Q3StrList        *myItemID;
    Q3StrList        *myItemDescr;
    float           *myItemPrice;
    float           *myItemChildPrice;
    
    QDate           myLastDate;
    QDate           myEndDate;
    
    int             isParent;
};
#endif // SubscriptionEdit_included
