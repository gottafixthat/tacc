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


#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QCloseEvent>

#include <TAAWidget.h>

/*!
 * \brief Brings up the a password change dialog for a login.
 */
class ChangePassword : public TAAWidget
{
    Q_OBJECT

public:

    ChangePassword
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0,
        const char* LoginID = NULL
    );

    virtual ~ChangePassword();
    
protected slots:
    virtual void generatePassword();
    virtual void savePassword();
    virtual void cancelPassword();
    virtual void QCloseEvent(QEvent *)              { delete this; }

protected:
    QLabel      *customerID;
    QLabel      *customerName;
    QLabel      *contactName;
    QLabel      *loginID;
    QLabel      *loginType;
    QLineEdit   *newPassword;

    long    myCustID;
    QString myLoginID;

};
#endif // ChangePassword_included

// vim: expandtab
