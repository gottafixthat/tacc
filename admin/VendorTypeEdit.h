/**********************************************************************

	--- Dlgedit generated file ---

	File: VendorTypeEdit.h
	Last generated: Tue Sep 16 20:36:05 1997

 *********************************************************************/

#ifndef VendorTypeEdit_included
#define VendorTypeEdit_included

#include "VendorTypeEditData.h"

class VendorTypeEdit : public VendorTypeEditData
{
    Q_OBJECT

public:

    VendorTypeEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        int InternalID = 0
    );

    virtual ~VendorTypeEdit();

public slots:
    void refreshVendList(int);
    
signals:
    void refresh(int);
    
private slots:
    virtual void saveVendType();
    virtual void cancelVendType();

private:
    int *intIDIndex;      // for our combo box.
    
    int IntID;

};
#endif // VendorTypeEdit_included
