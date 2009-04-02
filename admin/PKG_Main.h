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


#ifndef PKG_Main_included
#define PKG_Main_included

#include <QtGui/QPushButton>
#include <QtGui/QTabBar>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>
#include "PKG_General.h"
#include "PKG_Conversion.h"
#include "PKG_Pricing.h"
#include "PKG_Billables.h"

class PKG_Main : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Main
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Main();
    
    void    refreshPackages(void);

protected:
    QPushButton     *deleteButton;
    Q3ListView       *packageList;
    QTabBar         *theTabBar;
    Q3WidgetStack    *qws;

protected slots:
    virtual void    packageSelected(Q3ListViewItem *curItem);
    virtual void    newPackage();
    virtual void    deletePackage();
    void            updatePackage();

private:
    PKG_General     *tgeneral;
    PKG_Conversion  *tconversion;
    PKG_Pricing     *tpricing;
    PKG_Billables   *tbillables;

    void            loadPackages();

};
#endif // PKG_Main_included

// vim: expandtab
