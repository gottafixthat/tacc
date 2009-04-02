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

#ifndef LoginTypeBillablesAdd_included
#define LoginTypeBillablesAdd_included

#include <QtGui/QPushButton>
#include <QtGui/QDialog>
#include <Qt3Support/q3listbox.h>

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
    Q3ListBox        *itemList;
    QPushButton     *addButton;
    QPushButton     *cancelButton;

private:
	int				myLoginTypeID;

	int				*billableIDX;
	
protected slots:
    void            listDoubleClicked(Q3ListBoxItem *);
    virtual void    cancelBillableItem();
	virtual void	addBillableItem();
};

#endif // LoginTypeBillablesAdd_included

// vim: expandtab

