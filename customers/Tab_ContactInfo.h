/*
** $Id: Tab_ContactInfo.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Tab_ContactInfo - Contains all of the contact information for a
**                   customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Tab_ContactInfo.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef Tab_ContactInfo_included
#define Tab_ContactInfo_included

#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qstrlist.h>
#include <TAAWidget.h>


class Tab_ContactInfo : public TAAWidget
{
    Q_OBJECT

public:

    Tab_ContactInfo
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_ContactInfo();
    
	bool saveContactInfo(void);
	
public slots:
    virtual void refreshCustomer(long);

signals:
    void         customerChanged(long);

protected slots:
	virtual void addNewAddress();
	virtual void editCurrentAddress();
	virtual void editCurrentAddressItem(QListViewItem *);
	virtual void deleteCurrentAddress();
    virtual void DeactiveAddress();
    virtual void loadCustInfo();
	virtual void addNewContact();
	virtual void editCurrentContact();
	virtual void deleteCurrentContact();

	virtual void editCurrentAddressL(const QString &tag);

protected:
    QLineEdit   *fullName;
    QLineEdit   *contactName;
    QLineEdit   *altContact;
    QLabel      *customerID;
    QLabel      *primaryLogin;
    QLabel      *lastModified;
    QPushButton *editAddrButton;
    QPushButton *deleteAddrButton;
    QPushButton *addContactButton;
    QPushButton *editContactButton;
    QPushButton *deleteContactButton;
    QListView   *addressList;
    QListView   *contactList;
    
private:
	long		myCustID;
    int         contactIDCol;
	char		myBillingTag[20];
	void		refreshAddrList(void);
	
private slots:
	void	addrRefreshSlot(int RefFrom, long RefID);
	
};
#endif // Tab_ContactInfo_included
