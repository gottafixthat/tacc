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

#ifndef EditCustomer_included
#define EditCustomer_included

#include <QtGui/QTabBar>
#include <QtGui/QPushButton>
#include <QtGui/QMenuBar>
#include <Qt3Support/Q3PopupMenu>
#include <Qt3Support/q3widgetstack.h>

#include "Tab_ContactInfo.h"
#include "Tab_BillingInfo.h"
#include "CustSubscrList.h"
#include "Tab_Logins.h"
#include "Tab_Domains.h"
#include "Tab_VoIP.h"
#include "Tab_Notes.h"
#include <TAAWidget.h>
// #include "Tab_Accounts.h"

class EditCustomer : public TAAWidget
{
    Q_OBJECT

public:

    EditCustomer
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~EditCustomer();

	Tab_Logins			*loginsTab;

signals:
	void refreshCustList();
	
public slots:
    virtual void        refreshCustomer(long);
	virtual void saveCustomer();
	void refreshAll(int);
    virtual void receivePayment();

protected slots:
    virtual void        loadCustInfo();
    virtual void        cancelChanges()         { close(); }    
    virtual void        tabSelected(int);
    void                createTicket();
    void                raiseTab1();
    void                raiseTab2();
    void                raiseTab3();
    void                raiseTab4();
    void                raiseTab5();
    void                raiseTab6();
    void                raiseTab7();

protected:
    QTabBar             *tabBar;
    Q3WidgetStack        *qws;
    QPushButton         *saveButton;
    QPushButton         *cancelButton;
    QMenuBar            *customerMenu;


private:
	Tab_ContactInfo		*contactInfoTab;
	Tab_BillingInfo		*billingInfoTab;
	CustSubscrList      *subscrsTab;
//	Tab_Logins			*loginsTab;
	Tab_Domains			*domainsTab;
	Tab_VoIP 			*voipTab;
	Tab_Notes			*notesTab;
//	Tab_Accounts        *accountsTab;
	long				myCustID;

    Q3PopupMenu          *stMenu;
    int                 printedStatementID;
    int                 freePrintedStatementID;
    
};
#endif // EditCustomer_included

// vim: expandtab

