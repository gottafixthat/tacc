/*
** $Id: Tab_Domains.h,v 1.5 2004/01/22 00:02:10 marc Exp $
**
***************************************************************************
**
** Tab_Domains - Handles domain information.
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
** $Log: Tab_Domains.h,v $
** Revision 1.5  2004/01/22 00:02:10  marc
** Virtual Hosting database access is now complete in a basic form.
**
** Revision 1.4  2004/01/09 01:55:19  marc
** Minor changes here and there
**
** Revision 1.3  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.2  2004/01/02 15:00:20  marc
** Added more DNS Management functionality
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef Tab_Domains_included
#define Tab_Domains_included

#include <qwidget.h>
#include <qlistview.h>
#include <qpushbutton.h>
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
    QListView   *list;
    
private:
	long myCustID;
};
#endif // Tab_Domains_included
