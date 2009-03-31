/*
** $Id: LoginTypeFlagAdd.h,v 1.2 2003/12/23 02:20:41 marc Exp $
**
***************************************************************************
**
** LoginTypeFlagAdd.h - Allows flags to be added to login type definitions.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: LoginTypeFlagAdd.h,v $
** Revision 1.2  2003/12/23 02:20:41  marc
** Fixed the non-working dialog.  Removed the old QTArch created dialog.
**
**
*/


#ifndef LoginTypeFlagAdd_included
#define LoginTypeFlagAdd_included

#include <qdialog.h>
#include <qpushbutton.h>
#include <q3listbox.h>

class LoginTypeFlagAdd : public QDialog
{
    Q_OBJECT

public:

    LoginTypeFlagAdd
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int LoginTypeID = 0
    );

    virtual ~LoginTypeFlagAdd();

protected:
	int				myLoginTypeID;
    Q3ListBox        *flagList;
    QPushButton     *addButton;
    QPushButton     *cancelButton;

protected slots:
    void            flagDoubleClicked(Q3ListBoxItem *);
	virtual void	addFlag();
    virtual void    cancelFlag();

};
#endif // LoginTypeFlagAdd_included
