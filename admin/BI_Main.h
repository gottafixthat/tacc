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


#ifndef BI_Main_included
#define BI_Main_included

#include <QtGui/QPushButton>
#include <QtGui/QTabBar>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include "BI_General.h"
#include "BI_Prices.h"
#include "BI_Measurement.h"

class BI_Main : public TAAWidget
{
    Q_OBJECT

public:

    BI_Main
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~BI_Main();


    void    refreshBillables(void);

protected:
    Q3ListView       *billableList;
    QPushButton     *newButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;
    QTabBar         *theTabBar;
    Q3WidgetStack    *qws;

protected slots:
    virtual void    billableSelected(Q3ListViewItem *curItem);
    virtual void    newBillable();
    virtual void    deleteBillable();
    virtual void    updateBillable();

private:
    void            loadBillables();

    BI_General      *tgeneral;
    BI_Prices       *tprices;
    BI_Measurement  *tmeasurement;
};
#endif // BI_Main_included

// vim: expandtab
