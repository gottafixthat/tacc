/**********************************************************************

	--- Qt Architect generated file ---

	File: UsersOnlineFilters.h
	Last generated: Thu Sep 30 14:14:20 1999

 *********************************************************************/

#ifndef UsersOnlineFilters_included
#define UsersOnlineFilters_included

#include "UsersOnlineFiltersData.h"
#include <qdatetm.h>
#include <calendar.h>

#include <qdatetimeedit.h>

class UsersOnlineFilters : public UsersOnlineFiltersData
{
    Q_OBJECT

public:

    UsersOnlineFilters
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~UsersOnlineFilters();
    
    void        setDateTime(QDateTime newDateTime);
    
    QDateTime   selectedDateTime();


signals:
    void    filtersChanged(QDateTime);
    
public slots:
    virtual void cancelFilters();
    virtual void applyFilters();
    virtual void applyAndExit();

private:
    int         dateTimeSet;
    void        loadHosts();
    QDateTime   myDateTime;
    
    DateInput       *dateCal;
    QTimeEdit       *timeSpin;
    
};
#endif // UsersOnlineFilters_included
