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


#endif // WipedAccounts_included
