/*
** $Id: UserEditor.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** UserEditor.h  - UserEditor allows the user to view and change various
**                 information about a user.
**
***************************************************************************
** Written by R. Marc Lewis, 
**     under contract to The Public Defender Association
***************************************************************************
*/


#ifndef UserEditor_included
#define UserEditor_included

#include <qevent.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
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
    QPushButton     *delButton;
    QPushButton     *newButton;
    QPushButton     *addSave;
    QCheckBox       *sendTicketNotifications;

    long            myCurrentID;

};
#endif // UserEditor_included


