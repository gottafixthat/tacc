/*
** $Id: CCEntry.h,v 1.2 2004/01/13 01:15:58 marc Exp $
**
***************************************************************************
**
** CCEntry - Gets a credit card payment from the user.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CCEntry.h,v $
** Revision 1.2  2004/01/13 01:15:58  marc
** Added CV/Security Code processing and storage.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef CCEntry_included
#define CCEntry_included

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>

#include <TAAWidget.h>

class CCEntry : public TAAWidget
{
    Q_OBJECT

public:

    CCEntry
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~CCEntry();
    
protected slots:
    void        saveCCard();
    void        cancelCCard();

protected:
    QLineEdit   *custName;
    QLineEdit   *addr1;
    QLineEdit   *addr2;
    QLineEdit   *city;
    QLineEdit   *state;
    QLineEdit   *zip;
    QLineEdit   *phone;
    QLineEdit   *chargeAmount;
    QCheckBox   *autoPay;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QLabel      *dateLabel;
    QLabel      *balance;
    QComboBox   *cardType;
    QLineEdit   *cardNumber;
    QLineEdit   *expDate;
    QLineEdit   *cvInfo;
    QRadioButton *chargeDate1;
    QRadioButton *chargeDate2;
    
private:
    long    myCustID;

signals:
    void    customerChanged(long);
};
#endif // CCEntry_included
