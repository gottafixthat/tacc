/**********************************************************************

	--- Qt Architect generated file ---

	File: TE_Cities.h
	Last generated: Tue Jul 27 12:48:22 1999

 *********************************************************************/

#ifndef TE_Cities_included
#define TE_Cities_included

#include "TE_CitiesData.h"

class TE_Cities : public TE_CitiesData
{
    Q_OBJECT

public:

    TE_Cities
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TE_Cities();
    
    void    loadCities();
    int     allCities();
    int     isIncluded(const char *City);
    
protected slots:
    virtual void allCitiesClicked();
    virtual void    closeEvent(QCloseEvent *)       { delete this; }


};
#endif // TE_Cities_included
