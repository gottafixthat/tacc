/*
** $Id: PKG_Conversion.h,v 1.2 2003/12/30 00:32:53 marc Exp $
**
***************************************************************************
**
** PKG_Conversion.h - Package Editor, conversion tab.
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
** $Log: PKG_Conversion.h,v $
** Revision 1.2  2003/12/30 00:32:53  marc
** Removed QTArch created layouts.
**
**
*/


#ifndef PKG_Conversion_included
#define PKG_Conversion_included


#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qdatetm.h>
#include <calendar.h>
#include <TAAWidget.h>

class PKG_Conversion : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Conversion
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Conversion();
    

    void         setPackageID(long newPackageID);

protected:
    QPushButton     *saveButton;
    QCheckBox       *autoConvert;
    QRadioButton    *cvtDaysButton;
    QSpinBox        *daysSpinner;
    QRadioButton    *cvtOnDateButton;
    QLabel          *cvtOnDateParent;
    QComboBox       *cvtToList;
    QCheckBox       *isPromotional;
    QLabel          *promoDateParent;
    DateInput       *cvtDate;
    DateInput       *promoDate;


protected slots:
    virtual void autoConvertChanged(bool)               { checkForSave(); }
    virtual void cvtDaysButtonChanged(bool)             { checkForSave(); }
    virtual void cvtDaysChanged(int)                    { checkForSave(); }
    virtual void cvtOnButtonChanged(bool)               { checkForSave(); }
    virtual void convertToChanged(int)                  { checkForSave(); }
    virtual void promotionalChanged(bool)               { checkForSave(); }
    
    void         checkForSave();
    virtual void save();

private:
    long        myPackageID;

    long        *cvtToIDX;
    
    // Our internal variables for comparison when checking for saves.
    // They should contain the loaded info from the database.
    int         intAutoConvert;
    int         intConvertDays;
    QDate       intConvertDate;
    long        intConvertTo;
    int         intIsPromo;
    QDate       intPromoDate;

};
#endif // PKG_Conversion_included
