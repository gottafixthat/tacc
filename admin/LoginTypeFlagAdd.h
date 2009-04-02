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

#ifndef LoginTypeFlagAdd_included
#define LoginTypeFlagAdd_included

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listbox.h>

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


// vim: expandtab
