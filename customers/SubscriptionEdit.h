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

#ifndef SubscriptionEdit_included
#define SubscriptionEdit_included

#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3strlist.h>

#include <TAAWidget.h>
#include <calendar.h>

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


// vim: expandtab
