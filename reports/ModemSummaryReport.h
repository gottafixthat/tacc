/**********************************************************************

	--- Qt Architect generated file ---

	File: Report.h
	Last generated: Wed Mar 31 18:33:32 1999

 *********************************************************************/

#ifndef ModemSummaryReport_included
#define ModemSummaryReport_included

#include "Report.h"

#include <qintdict.h>
#include <qdatetm.h>

#include "ModemSummaryFilters.h"

class ModemSummaryReport : public Report
{
    Q_OBJECT

public:

    ModemSummaryReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ModemSummaryReport();
    
    virtual void refreshReport();
#ifdef USEQWT
    virtual void plotGraph(QwtPlot *plot);
#endif
    
public slots:
    virtual void editFilters();
    virtual void applyFilters();
    
private:
    long            *timeDict;
    void            setupCounters();
    void            addClicks(QDateTime sessStart, QDateTime sessStop);
    
    QDateTime       dictStartDate;
    QDateTime       dictEndDate;
    
    int             timeRes;
    int             timeSteps;
    
    ModemSummaryFilters *myFilters;

};
#endif // ModemSummaryReport_included

