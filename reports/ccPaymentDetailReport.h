/**
 * ccPaymentDetailReport.h - Report class for credit card payment details
 * needed for Isomedia.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CCPAYMENTDETAILREPORT_H
#define CCPAYMENTDETAILREPORT_H

#include "Report.h"
#include <qcheckbox.h>
#include <q3listbox.h>

// Our "Filter" widget
class ccPaymentDetailOptions : public TAAWidget
{
    Q_OBJECT

public:
    ccPaymentDetailOptions(QWidget* parent = NULL, const char *name = NULL);
    ~ccPaymentDetailOptions();

    const QString   cardTypeSet();
    const QString   dispositionSet();

signals:
    void            optionsUpdated();

protected slots:
    void    updateClicked();
    void    closeClicked();
    void    allCardTypesChanged(int);
    void    allDispTypesChanged(int);

protected:
    QCheckBox       *allCardTypes;
    QCheckBox       *allDispTypes;
    Q3ListBox        *cardTypeList;
    Q3ListBox        *dispList;

};

class ccPaymentDetailReport : public Report
{
    Q_OBJECT

public:

    ccPaymentDetailReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ccPaymentDetailReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);
    virtual void    editFilters();

private:
    ccPaymentDetailOptions  *opts;
};

#endif
