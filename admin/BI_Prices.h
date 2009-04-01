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


#ifndef BI_Prices_included
#define BI_Prices_included

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class BI_Prices : public TAAWidget
{
    Q_OBJECT

public:

    BI_Prices
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BI_Prices();

    void setItemNumber(long newItemNumber);

protected:
    Q3ListView       *pricingList;
    QLineEdit       *primaryPrice;
    QLineEdit       *secondaryPrice;
    QComboBox       *unitsList;
    QSpinBox        *trialPeriod;
    Q3MultiLineEdit  *additionalDescription;
    QPushButton     *saveButton;

protected slots:
    void            checkForSave();
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    virtual void    primaryPriceChanged(const QString &)    { checkForSave(); }
    virtual void    secondaryPriceChanged(const QString &)  { checkForSave(); }
    virtual void    unitsChanged(int)                       { checkForSave(); }
    virtual void    trialPeriodChanged(int)                 { checkForSave(); }
    virtual void    descriptionChanged()                    { checkForSave(); }
    virtual void    save();

private:
    long        myItemNumber;
    char        *myAddDesc;
};
#endif // BI_Prices_included

// vim: expandtab
