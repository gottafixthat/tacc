/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangePasswordData.h
	Last generated: Fri May 22 18:34:36 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef ChangePasswordData_included
#define ChangePasswordData_included

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbt.h>
#include <qlined.h>

class ChangePasswordData : public QWidget
{
    Q_OBJECT

public:

    ChangePasswordData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ChangePasswordData();

public slots:


protected slots:

    virtual void generatePassword();
    virtual void cancelPassword();
    virtual void savePassword();

protected:
    QLabel* customerIDLabel;
    QLabel* customerNameLabel;
    QLabel* contactNameLabel;
    QLabel* loginIDLabel;
    QLabel* loginTypeLabel;
    QLineEdit* newPassword;
    QPushButton* generateButton;

};

#endif // ChangePasswordData_included
