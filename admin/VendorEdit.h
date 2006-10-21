/**********************************************************************

	--- Dlgedit generated file ---

	File: VendorEdit.h
	Last generated: Sun Oct 12 19:54:47 1997

 *********************************************************************/

#ifndef VendorEdit_included
#define VendorEdit_included

#include "VendorEditData.h"
#include "BlargDB.h"

class VendorEdit : public VendorEditData
{
    Q_OBJECT

public:

    VendorEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int vendorID = 0
    );

    virtual ~VendorEdit();

private slots:
    virtual void saveVendor();
    virtual void cancelVendor();

private:
	VendorsDB		*vendor;
	int				myVendorID;

signals:
	void			refresh(int);
};
#endif // VendorEdit_included
