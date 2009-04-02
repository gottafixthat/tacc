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


#ifndef UserEditor_included
#define UserEditor_included

#include <QtCore/QEvent>
#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

#include <TAATools.h>
#include <TAAWidget.h>


class UserEditor : public TAAWidget
{
    Q_OBJECT

public:

    UserEditor(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~UserEditor();
    
public slots:
    virtual void    loadUser(long userID);

protected slots:
    void            saveUser();
    void            newUser();
    void            deleteUser();

signals:
    void            userUpdated(long);

protected:
    QLineEdit       *loginID;
    QComboBox       *accessLevel;
    QLineEdit       *password1;
    QLineEdit       *password2;
    QLineEdit       *telephone;
    QLineEdit       *agentID;
    QLineEdit       *extension;
    QLineEdit       *queueName;
    QPushButton     *delButton;
    QPushButton     *newButton;
    QPushButton     *addSave;
    QCheckBox       *sendTicketNotifications;
    QCheckBox       *userActive;

    long            myCurrentID;

};
#endif // UserEditor_included



// vim: expandtab
