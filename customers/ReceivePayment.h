/*
** $Id: ReceivePayment.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** RecievePayment - A class that brings up open charges for a customer and
**                  allows the user to enter a payment from them.
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
** $Log: ReceivePayment.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef ReceivePayment_included
#define ReceivePayment_included

#include <qdialog.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <TAAWidget.h>

class ReceivePayment : public TAAWidget
{
    Q_OBJECT

public:

    ReceivePayment
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~ReceivePayment();

public slots:
    void         loadCustInfo();
    virtual void refreshCustomer(long);
    
protected slots:
    virtual void cancelPressed();
    virtual void savePayment();

signals:
    void customerChanged(long);
	
protected:
    QLabel      *customerIDLabel;
    QLabel      *customerNameLabel;
    QLabel      *addressLabel1;
    QLabel      *addressLabel2;
    QLabel      *phoneLabel;
    QLabel      *dateLabel;
    QLabel      *balanceLabel;
    QLineEdit   *checkNumber;
    QLineEdit   *amount;
    QListView   *chargeList;
    QPushButton *saveButton;
    QPushButton *cancelButton;

private:
    void fillUncleared(void);
    
    long    myCustID;    
    float   balanceDue;

};
#endif // ReceivePayment_included
