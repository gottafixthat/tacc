/**********************************************************************

	--- Qt Architect generated file ---

	File: Tab_AccountsData.h
	Last generated: Thu May 20 16:45:55 1999

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef Tab_AccountsData_included
#define Tab_AccountsData_included

#include <qwidget.h>
#include <qlabel.h>
#include <qchkbox.h>
#include <qlistview.h>
#include <qpushbt.h>

class Tab_AccountsData : public QWidget
{
    Q_OBJECT

public:

    Tab_AccountsData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~Tab_AccountsData();

public slots:


protected slots:

    virtual void changePassword();
    virtual void hideWipedChanged(bool);
    virtual void editLogin();
    virtual void createNewLogin();
    virtual void modemUsageReport();
    virtual void unlockLogin();

protected:
    QPushButton* newButton;
    QPushButton* editButton;
    QPushButton* modemUsageButton;
    QPushButton* openCloseButton;
    QCheckBox* hideWiped;
    QPushButton* passwdButton;
    QLabel* adminMenuArea;
    QListView* list;

};

#endif // Tab_AccountsData_included
