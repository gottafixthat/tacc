/**
 * VendorEdit.h - Class definition for the Vendor Edit widget.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef VendorEdit_included
#define VendorEdit_included

#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>

#include <TAAWidget.h>

class VendorEdit : public TAAWidget
{
    Q_OBJECT

public:

    VendorEdit(QWidget* parent = NULL, const char* name = NULL, int vID = 0);
    virtual ~VendorEdit();

private slots:
    virtual void saveVendor();
    virtual void cancelVendor();

signals:
	void			refresh(int);

protected:
	int				myVendorID;
    QLineEdit       *companyName;
    QLineEdit       *contactName;
    QLineEdit       *vendorID;
    QLineEdit       *altContact;
    QCheckBox       *isActive;
    QLineEdit       *address1;
    QLineEdit       *address2;
    QLineEdit       *city;
    QLineEdit       *state;
    QLineEdit       *zip;
    QLineEdit       *checkName;
    QLineEdit       *accountNo;
    QComboBox       *vendorType;
    QComboBox       *terms;
    QLineEdit       *taxID;
    QCheckBox       *use1099;
    QLineEdit       *creditLimit;
    QLineEdit       *phone;
    QLineEdit       *altPhone;
    QLineEdit       *fax;
    QLabel          *balance;

};
#endif // VendorEdit_included
