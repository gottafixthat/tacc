/*
** $Id: PKG_Main.h,v 1.2 2003/12/29 22:40:25 marc Exp $
**
***************************************************************************
**
** PKG_Main.h - Package Editor main widget definitions.
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
** $Log: PKG_Main.h,v $
** Revision 1.2  2003/12/29 22:40:25  marc
** Removed old QTArch created widgets, done manually now.
**
**
*/


#ifndef PKG_Main_included
#define PKG_Main_included

#include <qwidget.h>
#include <q3widgetstack.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qtabbar.h>
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
