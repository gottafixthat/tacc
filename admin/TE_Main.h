/**********************************************************************

	--- Qt Architect generated file ---

	File: TE_Main.h
	Last generated: Mon Jul 26 21:21:34 1999

 *********************************************************************/

#ifndef TE_Main_included
#define TE_Main_included

#include "TE_MainData.h"
#include "TE_Message.h"
#include "TE_LoginTypes.h"
#include "TE_Cities.h"

class TE_Main : public TE_MainData
{
    Q_OBJECT

public:

    TE_Main
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TE_Main();
    
    virtual void sendMessage();

protected slots:
    virtual void    closeEvent(QCloseEvent *)       { delete this; }
    
private:
    TE_Message      *tmessage;
    TE_LoginTypes   *tlogins;
    TE_Cities       *tcities;

};
#endif // TE_Main_included
