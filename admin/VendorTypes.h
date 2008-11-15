/***************************************************************************
** VendorTypes.h - Class definition for the VendorTypes widget.
****************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2008, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
****************************************************************************/

#ifndef VendorTypes_included
#define VendorTypes_included

#include <TAAWidget.h>
#include <qlistview.h>
#include <qmenubar.h>

class VendorTypes : public TAAWidget
{
    Q_OBJECT

public:

    VendorTypes
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~VendorTypes();

signals:
    void refreshVendTypeList(int);
    
public slots:
    virtual void Hide();
    virtual void refreshList(int);

private slots:
    virtual void newVendorType();
    virtual void editVendorType();
    virtual void editVendorTypeL(int msg = 0);
    virtual void deleteVendorType();

protected:
    void        addToList(long, QListViewItem *);
    QListView   *list;
    QMenuBar    *menu;
};
#endif // VendorTypes_included
