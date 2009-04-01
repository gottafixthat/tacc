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


#ifndef Tab_ContactInfo_included
#define Tab_ContactInfo_included

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QLayout>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3strlist.h>

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
	virtual void editCurrentAddressItem(Q3ListViewItem *);
	virtual void deleteCurrentAddress();
    virtual void DeactiveAddress();
    virtual void loadCustInfo();
	virtual void addNewContact();
	virtual void editCurrentContact();
	virtual void deleteCurrentContact();

	virtual void editCurrentAddressL(const QString &tag);
    virtual void copyContactInfoToClipboard();

protected:
    QLineEdit   *fullName;
    QLineEdit   *contactName;
    QLineEdit   *altContact;
    QLabel      *customerID;
    QLabel      *regNum;
    QLabel      *primaryLogin;
    QLabel      *lastModified;
    QPushButton *editAddrButton;
    QPushButton *deleteAddrButton;
    QPushButton *addContactButton;
    QPushButton *editContactButton;
    QPushButton *deleteContactButton;
    Q3ListView   *addressList;
    Q3ListView   *contactList;
    
private:
	long		myCustID;
    int         contactIDCol;
	char		myBillingTag[20];
	void		refreshAddrList(void);
	
private slots:
	void	addrRefreshSlot(int RefFrom, long RefID);
	
};
#endif // Tab_ContactInfo_included

// vim: expandtab

