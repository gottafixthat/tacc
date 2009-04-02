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

#ifndef PKG_Pricing_included
#define PKG_Pricing_included

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class PKG_Pricing : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Pricing
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Pricing();

    void    setPackageID(long newPackageID);
    
protected:
    Q3ListView   *pricingList;
    QComboBox   *setupItemList;
    QLineEdit   *itemPrice;
    QPushButton *saveButton;

protected slots:
    void         checkForSave();
    virtual void listItemSelected(Q3ListViewItem *curItem);
    virtual void setupItemChanged(int)                  { checkForSave(); }
    virtual void priceChanged(const QString &)          { checkForSave(); }
    
    virtual void save();
    
private:
    long    myPackageID;

    long    *setupItemIDX;

};
#endif // PKG_Pricing_included

// vim: expandtab
