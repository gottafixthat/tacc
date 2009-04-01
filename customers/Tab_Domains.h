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

#ifndef Tab_Domains_included
#define Tab_Domains_included

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class Tab_Domains : public TAAWidget
{
    Q_OBJECT

public:

    Tab_Domains
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_Domains();

public slots:
	void refreshDomainList(int);

protected slots:
	virtual void newDomain();
	virtual void editChecklist();
	virtual void deActivateDomain();
    virtual void vhostSelected();
    virtual void emailSelected();
    virtual void aliasSelected();
    virtual void dnsSelected();
    virtual void createDNSFromTemplate();
    virtual void refreshCustomer(long);

protected:
    QPushButton *newButton;
    QPushButton *editButton;
    QPushButton *checklistButton;
    QPushButton *activeButton;
    QPushButton *vhostButton;
    QPushButton *emailButton;
    QPushButton *activitiesButton;
    QPushButton *trafficButton;
    QPushButton *aliasButton;
    QPushButton *dnsButton;
    Q3ListView   *list;
    
private:
	long myCustID;
};
#endif // Tab_Domains_included

// vim: expandtab

