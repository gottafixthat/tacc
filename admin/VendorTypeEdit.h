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

#ifndef VendorTypeEdit_included
#define VendorTypeEdit_included

#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

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

// vim: expandtab
