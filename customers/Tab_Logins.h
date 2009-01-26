/*
** $Id: Tab_Logins.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Tab_Logins - The logins tab.
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
** $Log: Tab_Logins.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef Tab_Logins_included
#define Tab_Logins_included

#include <qstrlist.h>
#include <qpopupmenu.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qtable.h>
#include <TAAWidget.h>

class Tab_Logins : public TAAWidget
{
    Q_OBJECT

public:

    Tab_Logins
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_Logins();
	void refreshLoginList(int);

signals:
	void refreshCust(int);
    
public slots:
	void refreshLoginListV(int);
	virtual void setPrimaryLogin();
	virtual void transferLogin();
	virtual void wipeLogin();
	virtual void createNewLogin(void);
	virtual void editLogin(void);
	virtual void unlockLogin(void);
	virtual void hideWipedChanged(bool);
	void         updateDBForWipe(const char * LoginID);
	virtual void modemUsageReport();
	virtual void bandwidthUsageReport();
	virtual void changePassword();
    virtual void loginFlagsClicked();
    virtual void diskSpaceClicked();

protected:
    QPushButton *newButton;
    QPushButton *openCloseButton;
    QPushButton *editButton;
    QPushButton *modemUsageButton;
    QCheckBox   *hideWiped;
    QPushButton *passwdButton;
    QLabel      *adminMenuArea;
    QListView   *list;
    QPushButton *bandwidthButton;
	
private:
	void updateARForLock(const char * LoginID);

	long myCustID;
	QStrList *loginList;
	
	QPopupMenu  *adminMenu;
};

// Custom Flag Editor
class CustomLoginFlagEditor : public TAAWidget
{
    Q_OBJECT

public:

    CustomLoginFlagEditor
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0,
        const char *loginID = NULL
    );

    virtual ~CustomLoginFlagEditor();

signals:
	void refreshCust(int);
   
public slots:
    void    cancelClicked();
    void    saveClicked();

protected:
    void    fillTable();

    QTable  *flagTable;
    char    myLoginID[64];
    long    myCustID;
};


#endif // Tab_Logins_included
