/**********************************************************************

	--- Dlgedit generated file ---

	File: VendorTypes.h
	Last generated: Tue Sep 16 19:21:54 1997

 *********************************************************************/

#ifndef VendorTypes_included
#define VendorTypes_included

#include <qapp.h>
#include <qwidget.h>
#include "VendorTypesData.h"

class VendorTypes : public VendorTypesData
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
    virtual int IntIDfromList(int ListNum);

private:
    void addToList(int IntID, int Level);

    int *intIDIndex;
    int indexPtr;
};
#endif // VendorTypes_included
