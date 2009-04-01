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

#ifndef ELT_Billables_included
#define ELT_Billables_included

#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class ELT_Billables : public TAAWidget
{
    Q_OBJECT

public:

    ELT_Billables
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ELT_Billables();

    void    setLoginTypeID(long newLoginTypeID);

protected:
    Q3ListView   *billableList;
    QPushButton *addButton;
    QPushButton *removeButton;

protected slots:
    virtual void itemSelected(Q3ListViewItem *curItem);
    virtual void removeBillable();
    virtual void addBillable();
    
private:
    long    myLoginTypeID;

};
#endif // ELT_Billables_included

// vim: expandtab
