/*
** $Id: TAALogin.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** TAALogin.h - Database login functions.
**
***************************************************************************
** Written by R. Marc Lewis, 
**     Based on the ADB class ADBLogin, (C)opyright 1998-2001
**     R. Marc Lewis, All Rights Reserved
***************************************************************************
*/

#ifndef TAALOGIN_H
#define TAALOGIN_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <Qt3Support/q3textview.h>

class TAALogin : public QDialog
{
    Q_OBJECT

public:
    TAALogin(QWidget *parent = NULL, const char *name = NULL);
    virtual ~TAALogin();

    void        setDefaultUser(const char *user = NULL);
    void        setUserEditable(bool allowUserEdit = true);

    const char *user();
    const char *pass();

protected slots:
    void        checkLogin();
    void        inputChanged(const QString &);

protected:
    Q3TextView   *titleText;
    QLineEdit   *userName;
    QLineEdit   *password;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLabel      *status;

    int         currentAttempt;
    int         maxAttempts;

private:
    void        badLogin();
};


#endif // TAALOGIN_H

