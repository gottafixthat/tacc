/*
** $Id: LoginEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** LoginEdit - Brings up a small dialog box that the user can use to edit
**             a login type.
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
** $Log: LoginEdit.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef LoginEdit_included
#define LoginEdit_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlined.h>
#include <qcombo.h>

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
