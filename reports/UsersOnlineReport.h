/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef UsersOnlineReport_included
#define UsersOnlineReport_included

#include "Report.h"
#include <qdatetm.h>
#include <qintdict.h>

class UsersOnlineReport : public Report
{
    Q_OBJECT

public:

    UsersOnlineReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~UsersOnlineReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    virtual void    editFilters();    
    virtual void    applyFilters(QDateTime newDateTime);
    
private:
    
    void        loadDict();
    
    QDateTime   displayTime;
    int         myShowActive;
    
    QIntDict<char>  strxlt;
};
#endif // UsersOnlineReport_included
