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

#ifndef RATEPLANEDIT_H
#define RATEPLANEDIT_H

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QComboBox>
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3buttongroup.h>

#include <TAAWidget.h>

/*!
 * \brief A Widget to edit Rate Plans
 *
 * This widget allows the user to edit a rate plan or create a new
 * one.  If no rate plan ID is specified, then a new rate plan
 * will be created when saving, otherwise it will edit an existing
 * rate plan.
 */
class RatePlanEdit : public TAAWidget
{
    Q_OBJECT

public:

    RatePlanEdit(QWidget* parent = NULL, const char* name = NULL, long IntID = 0);
    virtual ~RatePlanEdit();
    
public slots:
	virtual void toggleAutoConvert(bool newState);
	virtual void toggleConvertDays(bool newState);
	virtual void toggleConvertDate(bool newState);
	virtual void togglePromoPlan(bool newState);
	virtual void cancelRatePlan();
	virtual void saveRatePlan();

signals:
    void            ratePlanSaved(int);

protected:
    QLineEdit       *planTag;
    QLineEdit       *description;
    Q3ButtonGroup    *autoConvertGroup;
    QRadioButton    *noAutoConvert;
    QRadioButton    *autoConvertDays;
    QLabel          *convertDaysLabel;
    QRadioButton    *autoConvertDate;
    QLineEdit       *convertDays;
    Q3DateEdit       *convertDate;
    QLabel          *convertToLabel;
    QComboBox       *convertToList;

    QCheckBox       *promoPlan;
    QLabel          *promoEndsOnLabel;
    Q3DateEdit       *promoEndsOn;

private:
	long	        myIntID;
	long            *planIndex;
};
#endif // RatePlanEdit_included

// vim: expandtab

