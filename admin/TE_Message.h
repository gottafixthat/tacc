/**********************************************************************

	--- Qt Architect generated file ---

	File: TE_Message.h
	Last generated: Tue Jul 27 11:55:52 1999

 *********************************************************************/

#ifndef TE_Message_included
#define TE_Message_included

#include "TE_MessageData.h"

class TE_Message : public TE_MessageData
{
    Q_OBJECT

public:

    TE_Message
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TE_Message();
    
    char *createMessage();
    
    int messageOk();

protected slots:
    virtual void    closeEvent(QCloseEvent *)       { delete this; }


};
#endif // TE_Message_included
