/**********************************************************************

	--- Qt Architect generated file ---

	File: CustomerPicker.h
	Last generated: Wed May 12 17:46:11 1999

 *********************************************************************/

#ifndef CustomerPicker_included
#define CustomerPicker_included

#include "CustomerPickerData.h"

class CustomerPicker : public CustomerPickerData
{
    Q_OBJECT

public:

    CustomerPicker
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CustomerPicker();
    
    virtual void listQueryS();
    virtual void clearQuery();

protected:
    virtual void itemDoubleClicked(QListViewItem *selItem);
    virtual void itemEnterHit(QListViewItem *selItem);
    virtual void itemSelChanged(QListViewItem *selItem);

private:
    void    doQuery(void);
    
    int     allowEmptyQuery;

signals:
    void    customerSelected(long);
    void    customerHilighted(long);

};
#endif // CustomerPicker_included
