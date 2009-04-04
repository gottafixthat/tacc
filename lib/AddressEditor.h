/*
** $Id: AddressEditor.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** AddressEditor - An address editor class.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2002, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: AddressEditor.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef AddressEditor_included
#define AddressEditor_included

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <TAAWidget.h>
#include <BlargDB.h>
#include <ADB.h>

class AddressEditor : public TAAWidget
{
    Q_OBJECT

public:

    AddressEditor(QWidget* parent = NULL);

    virtual ~AddressEditor();
    
    void	editAddress(long refFrom, long refID, const char *tag);
    void	editAddressByID(long intID);
    void	newAddress(long refFrom, long refID);
    
signals:
	void	addressUpdated(int, long);
    void    customerChanged(long);

protected slots:	
	virtual void internationalChanged(bool);
	virtual void saveAddress();
	virtual void cancelAddress();

protected:
    QLabel      *typeLabel;
    QLabel      *typeData;
    QCheckBox   *active;
    QLabel      *tagLabel;
    QComboBox   *tagList;
    QLabel      *tagData;
    QCheckBox   *international;
    QLabel      *cszLabel;
    QLabel      *countryLabel;
    QLabel      *postalCodeLabel;
    QLineEdit   *address1;
    QLineEdit   *address2;
    QLineEdit   *city;
    QLineEdit   *state;
    QLineEdit   *zip;
    QLineEdit   *postalCode;
    QLineEdit   *country;
    QPushButton *saveButton;
    QPushButton *cancelButton;

private:
	void	prepareEditForm(AddressesDB *ADB);
	void	updateInternational(void);
	long	myIntID;
	long 	myRefFrom;
	long	myRefID;
	char	myTag[64];
	int		myInternational;

};
#endif // AddressEditor_included
