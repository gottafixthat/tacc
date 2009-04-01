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


#ifndef Tab_Logins_included
#define Tab_Logins_included

#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3table.h>
#include <Qt3Support/q3strlist.h>
#include <Qt3Support/q3popupmenu.h>

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
	void refreshLoginList(long);


public slots:
	void refreshLoginListV(long);
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
    virtual void refreshCustomer(long);

protected:
    QPushButton *newButton;
    QPushButton *openCloseButton;
    QPushButton *editButton;
    QPushButton *modemUsageButton;
    QCheckBox   *hideWiped;
    QPushButton *passwdButton;
    QLabel      *adminMenuArea;
    Q3ListView   *list;
    QPushButton *bandwidthButton;
	
private:
	void updateARForLock(const char * LoginID);

	long myCustID;
	Q3StrList *loginList;
	
	Q3PopupMenu  *adminMenu;
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

    Q3Table  *flagTable;
    char    myLoginID[64];
    long    myCustID;
};


#endif // Tab_Logins_included

// vim: expandtab

