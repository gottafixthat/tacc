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


#ifndef LoginEdit_included
#define LoginEdit_included

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include <TAAWidget.h>

class LoginEdit : public TAAWidget
{
    Q_OBJECT

public:

    LoginEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0,
        const char * LoginID = NULL
    );

    virtual ~LoginEdit();
    
signals:
	void refreshLoginList(int);
	void refreshCust(int);

private slots:
	virtual void saveLogin();
	virtual void cancelLogin();

protected:
    QLineEdit   *loginID;
    QComboBox   *loginType;
    QLineEdit   *passwordEntry;
    QLineEdit   *fingerEntry;
    QLineEdit   *contactName;

private:
	long	myCustID;
	char	myLoginID[64];
	
	void	updateSubscriptions(int oldType, int newType);
	void	addSubscriptions(int loginType);

    void    mailPrimaryLogin(void);
    void    mailAddOnLogin(void);
    void    mailParent();
    void    mailAdmins(void);

};
#endif // LoginEdit_included

// vim: expandtab
