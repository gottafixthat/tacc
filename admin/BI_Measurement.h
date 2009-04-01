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


#ifndef BI_Measurement_included
#define BI_Measurement_included

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include <ADB.h>

class BI_Measurement : public TAAWidget
{
    Q_OBJECT

public:

    BI_Measurement
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BI_Measurement();
    void    setItemNumber(long newItemNumber);

protected:
    Q3ListView       *measureList;
    QCheckBox       *isMeasured;
    QComboBox       *measuredFrom;
    QLineEdit       *additionalPrice;
    QLineEdit       *baseQty;
    QCheckBox       *isIncremental;
    QLineEdit       *minIncrement;
    Q3MultiLineEdit  *additionalDescription;
    QPushButton     *saveButton;

protected slots:
    void            checkForSave();
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    virtual void    measuredChanged()                       { checkForSave(); }
    virtual void    measuredFromChanged(int)                { checkForSave(); }
    virtual void    priceChanged(const QString &)           { checkForSave(); }
    virtual void    baseQtyChanged(const QString &)         { checkForSave(); }
    virtual void    isIncrementalChanged()                  { checkForSave(); }
    virtual void    minIncrementChanged(const QString &)    { checkForSave(); }
    virtual void    additionalDescriptionChanged()          { checkForSave(); }
    virtual void    save();


private:
    long        myItemNumber;
    int         myIsMeasured;
    int         myMeasuredFrom;
    float       myAdditionalPrice;
    llong       myBaseQuantity;
    int         myIsIncremental;
    llong       myMinIncrement;
    char        *myAddDesc;
};
#endif // BI_Measurement_included

// vim: expandtab
