/*
** $Id: BI_Main.h,v 1.2 2003/12/24 01:08:54 marc Exp $
**
***************************************************************************
**
** BI_Main.h - Billable Items Editor main window.
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
** $Log: BI_Main.h,v $
** Revision 1.2  2003/12/24 01:08:54  marc
** Removed all of the QTArch generated dialogs and did the layouts manually.
**
**
*/


#ifndef BI_Main_included
#define BI_Main_included

#include <qwidget.h>
#include <q3widgetstack.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qtabbar.h>
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
