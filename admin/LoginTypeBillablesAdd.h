/*
** $Id: LoginTypeBillablesAdd.h,v 1.2 2003/12/23 01:23:58 marc Exp $
**
***************************************************************************
**
** LoginTypeBillablesAdd - A dialog that allows the selection of a
**                         billable item.
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
** $Log: LoginTypeBillablesAdd.h,v $
** Revision 1.2  2003/12/23 01:23:58  marc
** Broke some time ago because of the QTArchitect dialogs.  Did the layout manually.
**
**
*/


#ifndef LoginTypeBillablesAdd_included
#define LoginTypeBillablesAdd_included

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlistbox.h>

class LoginTypeBillablesAdd : public QDialog
{
    Q_OBJECT

public:

    LoginTypeBillablesAdd
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int LoginTypeID = 0
    );

    virtual ~LoginTypeBillablesAdd();

protected:
    QListBox        *itemList;
    QPushButton     *addButton;
    QPushButton     *cancelButton;

private:
	int				myLoginTypeID;

	int				*billableIDX;
	
protected slots:
    void            listDoubleClicked(QListBoxItem *);
    virtual void    cancelBillableItem();
	virtual void	addBillableItem();
};

#endif // LoginTypeBillablesAdd_included
