/*
** $Id: EditCustomer.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** EditCustomer.h - Edit Customer controls the main customer window.
**                  It controls all of the tabs, and the menu.
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
** $Log: EditCustomer.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef EditCustomer_included
#define EditCustomer_included

#include <qwidgetstack.h>
#include <qtabbar.h>
#include <qpushbutton.h>
#include <qmenubar.h>

#include "Tab_ContactInfo.h"
#include "Tab_BillingInfo.h"
#include "CustSubscrList.h"
#include "Tab_Logins.h"
#include "Tab_Domains.h"
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

protected:
    QTabBar             *tabBar;
    QWidgetStack        *qws;
    QPushButton         *saveButton;
    QPushButton         *cancelButton;
    QMenuBar            *customerMenu;


private:
	Tab_ContactInfo		*contactInfoTab;
	Tab_BillingInfo		*billingInfoTab;
	CustSubscrList      *subscrsTab;
//	Tab_Logins			*loginsTab;
	Tab_Domains			*domainsTab;
	Tab_Notes			*notesTab;
//	Tab_Accounts        *accountsTab;
	long				myCustID;

    QPopupMenu          *stMenu;
    int                 printedStatementID;
    int                 freePrintedStatementID;
    
};
#endif // EditCustomer_included
