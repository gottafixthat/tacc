/*
** $Id: BI_Prices.h,v 1.2 2003/12/24 01:08:54 marc Exp $
**
***************************************************************************
**
** BI_Prices.h - Billable Items pricing editor page.
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
** $Log: BI_Prices.h,v $
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
**
*/


#ifndef BI_Prices_included
#define BI_Prices_included

#include <qwidget.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <q3multilineedit.h>
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
