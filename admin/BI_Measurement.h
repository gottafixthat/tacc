/*
** $Id: BI_Measurement.h,v 1.2 2003/12/24 01:08:54 marc Exp $
**
***************************************************************************
**
** BI_Measurement.h - Billable Items Measurement tab.
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
** $Log: BI_Measurement.h,v $
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
**
*/


#ifndef BI_Measurement_included
#define BI_Measurement_included

#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qmultilineedit.h>
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
    QListView       *measureList;
    QCheckBox       *isMeasured;
    QComboBox       *measuredFrom;
    QLineEdit       *additionalPrice;
    QLineEdit       *baseQty;
    QCheckBox       *isIncremental;
    QLineEdit       *minIncrement;
    QMultiLineEdit  *additionalDescription;
    QPushButton     *saveButton;

protected slots:
    void            checkForSave();
    virtual void    listItemSelected(QListViewItem *curItem);
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
