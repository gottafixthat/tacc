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

#ifndef CCREFUND_H
#define CCREFUND_H

#include <QtCore/QString>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include <TAAWidget.h>

struct CCRefundCardRecord {
    QString cardholder;
    QString addr;
    QString zip;
    int     cardType;
    QString cardTypeStr;
    QString cardNo;
    QString expDate;
    QString ccv;
};

class CCRefund : public TAAWidget
{
    Q_OBJECT

public:
    CCRefund(QWidget *parent = NULL, const char *name = NULL);
    virtual ~CCRefund();

    void        setCustomerID(long);

protected slots:
    void        processRefund();
    void        cancelClicked();

protected:
    QLabel      *customerID;
    QLabel      *customerName;
    QComboBox   *cardList;
    QLineEdit   *refundAmount;
    QPushButton *processButton;

    CCRefundCardRecord  *cardListIDX;
    int                 cardListCount;
    long                myCustID;
    QString             myMsgBoxTitle;
};

#endif

// vim: expandtab
