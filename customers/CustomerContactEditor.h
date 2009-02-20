/**
 * CustomerContactEditor.h - Class definition for the customer contact editor.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CUSTOMERCONTACTEDITOR_H
#define CUSTOMERCONTACTEDITOR_H

#include <TAAWidget.h>
#include <TAAStructures.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlabel.h>

class CustomerContactEditor : public TAAWidget
{
    Q_OBJECT

public:
    CustomerContactEditor(QWidget *parent = NULL, const char *name = NULL);
    ~CustomerContactEditor();

    void        setContactID(uint id);
    void        setCustomerID(uint id);

protected:
    customerContactRecord   contactRec;
    QLabel                  *custName;
    QLabel                  *custID;
    QComboBox               *Tag;
    QLineEdit               *Name;
    QComboBox               *Access;
    QCheckBox               *International;
    QLineEdit               *PhoneNumber;
    QLineEdit               *EmailAddress;
    QCheckBox               *BillingEmail;
    QCheckBox               *Active;
    void                    updateCustomerFields();
    uint                    myCustID;

protected slots:
    void                    saveClicked();
    void                    cancelClicked();
};


#endif

