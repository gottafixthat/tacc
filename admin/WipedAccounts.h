/**********************************************************************

	--- Qt Architect generated file ---

	File: WipedAccounts.h
	Last generated: Thu Nov 12 14:32:29 1998

 *********************************************************************/

#ifndef WipedAccounts_included
#define WipedAccounts_included

#include "WipedAccountsData.h"

#include <BlargDB.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qfont.h>
#include <qdatetm.h>
#include <qrect.h>
#include <qcolor.h>
#include <qstring.h>


class WipedAccounts : public WipedAccountsData
{
    Q_OBJECT

public:

    WipedAccounts
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~WipedAccounts();
    void    fillList(void);

private:
    long    totAccts;
    long    selAccts;
    float   totOverdue;
    float   totSelected;
    
    int     myAccountTypes;
    
protected slots:
    virtual void processSelections();
    virtual void cancelOverdue();
    virtual void selectNone();
    virtual void selectAll();
    virtual void editCustomer(QListViewItem *curItem);
    virtual void updateTotals();
    virtual void sendToCollections(void);
    
};

class CollReport
{
public:
    CollReport();
    ~CollReport();

    void printReport(long CustID);
    
protected:
    void printHeader(QPainter *p, CustomersDB *cust, AddressesDB *cont, float Balance);
    void printFooter(QPainter *p, int PageNo);
    void registerHeader(QPainter *p);
    

};


#endif // WipedAccounts_included
