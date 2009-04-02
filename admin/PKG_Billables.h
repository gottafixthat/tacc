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

#ifndef PKG_Billables_included
#define PKG_Billables_included

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class PKG_Billables : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Billables
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Billables();

    void    setPackageID(long newPackageID);

protected:
    Q3ListView   *billableList;
    QComboBox   *itemList;
    QLineEdit   *description;
    QSpinBox    *qtySpin;
    QSpinBox    *groupSpin;
    QPushButton *removeButton;
    QPushButton *addButton;
    QPushButton *saveButton;

protected slots:
    void         checkForSave();
    virtual void billableSelected(Q3ListViewItem *curItem);
    virtual void save();
    virtual void addBillable();
    virtual void removeBillable();
    virtual void billableItemChanged(int)               { checkForSave(); }
    virtual void descriptionChanged(const QString &)    { checkForSave(); }
    virtual void quantityChanged(int)                   { checkForSave(); }
    virtual void groupChanged(int)                      { checkForSave(); }

private:
    long    myPackageID;
    long    *billItemIDX;
    
    long    intBillable;
    char    *intDescr;
    int     intQty;
    int     intGroup;
};
#endif // PKG_Billables_included

// vim: expandtab
