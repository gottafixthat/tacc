/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef NEWCUSTOMER_H
#define NEWCUSTOMER_H

#include <qlineedit.h>
#include <qcombobox.h>
#include <qmultilineedit.h>

#include <TAAWidget.h>

class NewCustomer : public TAAWidget
{
    Q_OBJECT

public:

    NewCustomer(QWidget* parent = NULL, const char* name = NULL);
    virtual ~NewCustomer();

signals:
	void refreshCustList();
    
protected slots:
	virtual void addCustomer();

protected:
	int checkPhone(char * phone);

    QLineEdit       *fullName;
    QLineEdit       *contactName;
    QLineEdit       *altContact;
    QLineEdit       *address1;
    QLineEdit       *address2;
    QLineEdit       *city;
    QLineEdit       *state;
    QLineEdit       *zip;
    QLineEdit       *country;
    QLineEdit       *dayPhone;
    QLineEdit       *evePhone;
    QLineEdit       *faxPhone;
    QLineEdit       *altPhone;
    QComboBox       *ratePlan;
    QComboBox       *statementType;
    QComboBox       *package;
    QComboBox       *referredBy;
    QMultiLineEdit  *initialNotes;

};
#endif // NewCustomer_included

// vim: expandtab

