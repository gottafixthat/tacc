/**********************************************************************

	--- Dlgedit generated file ---

	File: CallBackVerify.h
	Last generated: Thu Nov 6 18:22:47 1997

 *********************************************************************/

#ifndef CallBackVerify_included
#define CallBackVerify_included

#include "CallBackVerifyData.h"

class CallBackVerify : public CallBackVerifyData
{
    Q_OBJECT

public:

    CallBackVerify
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CallBackVerify();
    
	void setDayPhoneText(const char * newText);
	void setEvePhoneText(const char * newText);
	void setFaxPhoneText(const char * newText);
	void setAltPhoneText(const char * newText);

	const char * getSelected(void);

protected slots:
    virtual void QCloseEvent(QEvent *)              { delete this; }
};
#endif // CallBackVerify_included
