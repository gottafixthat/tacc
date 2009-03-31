/*
** $Id: PKG_Pricing.h,v 1.2 2003/12/30 01:07:54 marc Exp $
**
***************************************************************************
**
** PKG_Pricing - Package pricing tab.
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
** $Log: PKG_Pricing.h,v $
** Revision 1.2  2003/12/30 01:07:54  marc
** Removed QTArch generated dialogs.
**
**
*/


#ifndef PKG_Pricing_included
#define PKG_Pricing_included

#include <qwidget.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <TAAWidget.h>

class PKG_Pricing : public TAAWidget
{
    Q_OBJECT

public:

    PKG_Pricing
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PKG_Pricing();

    void    setPackageID(long newPackageID);
    
protected:
    Q3ListView   *pricingList;
    QComboBox   *setupItemList;
    QLineEdit   *itemPrice;
    QPushButton *saveButton;

protected slots:
    void         checkForSave();
    virtual void listItemSelected(Q3ListViewItem *curItem);
    virtual void setupItemChanged(int)                  { checkForSave(); }
    virtual void priceChanged(const QString &)          { checkForSave(); }
    
    virtual void save();
    
private:
    long    myPackageID;

    long    *setupItemIDX;

};
#endif // PKG_Pricing_included
