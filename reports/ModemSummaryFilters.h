/**********************************************************************

	--- Qt Architect generated file ---

	File: ModemSummaryFilters.h
	Last generated: Thu Oct 7 15:13:53 1999

 *********************************************************************/

#ifndef ModemSummaryFilters_included
#define ModemSummaryFilters_included

#include "ModemSummaryFiltersData.h"

#include <qintdict.h>
#include <qlist.h>

typedef struct HostInfoStruct {
    int     HostID;
    char    HostName[128];
};

class ModemSummaryFilters : public ModemSummaryFiltersData
{
    Q_OBJECT

public:

    ModemSummaryFilters
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ModemSummaryFilters();
    
    int     showHost(int hostID);
    
protected slots:
    virtual void allServersSelected();
    virtual void selectedServersSelected();
    virtual void listSelectionChanged();
    virtual void cancelFilters()                    { hide(); }
    virtual void applyFilters();
    virtual void applyAndExit();
    
private:
    int                     showAllHosts;
    QIntDict<long>          hostList;
    QList<HostInfoStruct>   allHosts;
    
    void    loadHosts();
    
signals:
    void    filtersChanged();

};
#endif // ModemSummaryFilters_included
