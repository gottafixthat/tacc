/*
** $Id: Vendors.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Vendors.h - Class definition for the vendor list.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Vendors.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef VENDORS_H
#define VENDORS_H

#include <qwidget.h>
#include <q3listview.h>
#include <qmenubar.h>

#include <TAAWidget.h>

class Vendors : public TAAWidget
{
    Q_OBJECT

public:

    Vendors
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~Vendors();

public slots:
	virtual void refreshList(int);

private slots:
    virtual void newVendor();
    virtual void editVendor();
    virtual void deleteVendor()         {};
    
protected:
    QMenuBar    *menu;
    Q3ListView   *list;

};
#endif // Vendors_included
