/**
 * VendorTypeEdit.h - Class definition for the vendor type editor.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef VendorTypeEdit_included
#define VendorTypeEdit_included

#include <qlineedit.h>
#include <qcombobox.h>

#include <TAAWidget.h>

class VendorTypeEdit : public TAAWidget
{
    Q_OBJECT

public:

    VendorTypeEdit(QWidget* parent = NULL, const char* name = NULL, int InternalID = 0);
    virtual ~VendorTypeEdit();

public slots:
    void refreshVendList(int);
    
signals:
    void refresh(int);
    
private slots:
    virtual void saveVendType();
    virtual void cancelVendType();

protected:
    QLineEdit   *vendorType;
    QComboBox   *subTypeList;

private:
    int *intIDIndex;      // for our combo box.
    
    int IntID;

};
#endif // VendorTypeEdit_included
