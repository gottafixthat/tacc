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


#ifndef DOMAINTYPEBILLABLESADD_H
#define DOMAINTYPEBILLABLESADD_H

#include <qlistview.h>

#include <TAAWidget.h>

/*!
 * \brief Brings up a selection box for adding a billable item to a
 * domain type record.
 */
class DomainTypeBillablesAdd : public TAAWidget
{
    Q_OBJECT

public:

    DomainTypeBillablesAdd(QWidget* parent = NULL, const char* name = NULL, int DomainTypeID = 0);
    virtual ~DomainTypeBillablesAdd();

signals:
    void            domainTypeBillableAdded(int);

protected slots:
    virtual void    addBillableItem();
    virtual void    cancelBillableItemAdd();

protected:
    QListView       *itemList;

private:
    int             myDomainTypeID;
    int             itemNumCol;

};
#endif

// vim: expandtab
