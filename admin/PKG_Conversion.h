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


#ifndef PKG_Conversion_included
#define PKG_Conversion_included


#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QRadioButton>
#include <QtGui/QComboBox>

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

// vim: expandtab
