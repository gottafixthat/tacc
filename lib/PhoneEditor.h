/**********************************************************************

	--- Qt Architect generated file ---

	File: PhoneEditor.h
	Last generated: Sat Mar 14 17:15:00 1998

 *********************************************************************/

#ifndef PhoneEditor_included
#define PhoneEditor_included

#include "PhoneEditorData.h"
#include "BlargDB.h"

class PhoneEditor : public PhoneEditorData
{
    Q_OBJECT

public:

    PhoneEditor
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PhoneEditor();
    
    void	editPhone(long refFrom, long refID, const char *tag);
    void	editPhoneByID(long intID);
    void	newPhone(long refFrom, long refID);
    
signals:
	void	phoneUpdated(int, long);
    
private:
	void	prepareEditForm(PhoneNumbersDB *PDB);
	void	updateInternational(void);
	
	virtual void internationalChanged(bool);
	virtual void savePhone();
	virtual void cancelPhone();

	long	myIntID;
	long 	myRefFrom;
	long	myRefID;
	char	myTag[64];
	int		myInternational;

};
#endif // PhoneEditor_included
