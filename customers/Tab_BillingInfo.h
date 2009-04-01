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

#ifndef Tab_BillingInfo_included
#define Tab_BillingInfo_included

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class Tab_BillingInfo : public TAAWidget
{
    Q_OBJECT

public:

    Tab_BillingInfo
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_BillingInfo();

	virtual void fillStatementList(void);

public slots:
    virtual void    showCustRegister();
	virtual void    takeCCard();
	virtual void    extendGracePeriod();
    virtual void    changeRatePlan();
    virtual void    changeBillingCycle();
    virtual void    generateStatement();
    virtual void    runSubscriptions();
	void            refreshBillingInfo(int);
	void            viewStatement(Q3ListViewItem *curItem);

    void            cancelPendingCharge();
    void            removeAutoPaymentCard();
    void            issueCardRefund();
	
	virtual void    togglePrintedStatements();
	virtual void    toggleFreePrintedStatements();

    virtual void    refreshCustomer(long);

signals:
    void            customerChanged(long);

protected:
    QLabel      *ratePlan;
    QLabel      *billingCycle;
    QLabel      *lastStatementNo;
    QLabel      *ratePlanDate;
    QLabel      *terms;
    QLabel      *accountOpened;
    QLabel      *accountClosed;
    QLabel      *accountReOpened;
    QLabel      *accountExpires;
    QLabel      *active;
    QLabel      *graceEnds;
    QLabel      *statementType;
    QLabel      *lastBilled;
    QLabel      *lastDueDate;
    QLabel      *autoPayment;
    QLabel      *pendingCharge;
    Q3ListView   *stList;
    QLabel      *currentBalance;
    QLabel      *mailboxTally;
    
private:
	long	myCustID;
	QWidget *myParent;

signals:
    void refreshCust(int);
};
#endif // Tab_BillingInfo_included

// vim: expandtab

