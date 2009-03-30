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


#ifndef EXTENDGRACEDATE_H
#define EXTENDGRACEDATE_H

#include <qdatetime.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <TAAWidget.h>

class ExtendGraceDate : public TAAWidget
{
    Q_OBJECT

public:

    ExtendGraceDate(QWidget* parent = NULL, const char* name = NULL, long CustomerID = 0);
    virtual ~ExtendGraceDate();

protected slots:
    virtual void daysChanged(int newDays);
    virtual void cancelGrace();
    virtual void saveGrace();

protected:
    long        myCustID;
    QDate       origGrace;
    QDate       newGrace;

    QLabel      *titleLabel;
    QSpinBox    *extendDays;
    QLabel      *currentGraceDate;
    QLabel      *newGraceDate;

};
#endif

// vim: expandtab
