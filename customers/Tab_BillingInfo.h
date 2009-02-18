/*
** $Id: Tab_BillingInfo.h,v 1.3 2004/04/15 18:11:44 marc Exp $
**
***************************************************************************
**
** Tab_BillingInfo - Displays billing information about the customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2002, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Tab_BillingInfo.h,v $
** Revision 1.3  2004/04/15 18:11:44  marc
** Changed the way that automatic credit cards and pending payments display in the
** Billing tab of the Edit Customer window.  It now shows what day automatic
** payments go through on, and the last four digits of pending payments.  Added
** options to remove pending charges and automatic payment cards in the Payments
** menu option of the Edit Customer window.  Taking payments now checks for
** pending payments and asks the user if they want to remove the pending payment
** before taking the new one.
**
** Revision 1.2  2004/02/27 01:33:33  marc
** The Customers table now tracks how many mailboxes are allowed.  LoginTypes
** and Packages were both updated to include how many mailboxes are allowed with
** each package or login type.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef Tab_BillingInfo_included
#define Tab_BillingInfo_included

#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qlineedit.h>
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
    virtual void showCustRegister();
	virtual void takeCCard();
	virtual void extendGracePeriod();
    virtual void changeRatePlan();
    virtual void changeBillingCycle();
    virtual void generateStatement();
    virtual void runSubscriptions();
	void refreshBillingInfo(int);
	void viewStatement(QListViewItem *curItem);

    void         cancelPendingCharge();
    void         removeAutoPaymentCard();
	
	virtual void togglePrintedStatements();
	virtual void toggleFreePrintedStatements();

    virtual void refreshCustomer(long);

signals:
    void         customerChanged(long);

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
    QListView   *stList;
    QLabel      *currentBalance;
    QLabel      *mailboxTally;
    
private:
	long	myCustID;
	QWidget *myParent;

signals:
    void refreshCust(int);
};
#endif // Tab_BillingInfo_included
