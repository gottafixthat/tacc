/**********************************************************************

	--- Qt Architect generated file ---

	File: StatementView.h
	Last generated: Wed Sep 9 16:51:41 1998

 *********************************************************************/

#ifndef StatementView_included
#define StatementView_included

#include "StatementViewData.h"

class StatementView : public StatementViewData
{
    Q_OBJECT

public:

    StatementView
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~StatementView();
    
    void    loadStatement(long stNo);
    
public slots:
    virtual void closeView();
    virtual void reEmailStatement();
    virtual void rePrintStatement();
    
private:
    long myStNo;

};
#endif // StatementView_included
