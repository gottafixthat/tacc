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

#ifndef VENDORS_H
#define VENDORS_H

#include <QtGui/QMenuBar>
#include <Qt3Support/q3listview.h>

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

// vim: expandtab
