/*
** $Id: $
**
***************************************************************************
**
** BillingCycleEdit - The widget responsible for editing a billing
**                    cycle definition.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: $
**
*/


#ifndef BillingCycleEdit_included
#define BillingCycleEdit_included

#include "BillingCycleEditData.h"

class BillingCycleEdit : public BillingCycleEditData
{
    Q_OBJECT

public:

    BillingCycleEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        const char* CycleID = NULL
    );

    virtual ~BillingCycleEdit();

public slots:
    void refreshCycleList(int);
    
signals:
    void refresh(int);
    
private slots:
    virtual void saveCycle();
    virtual void cancelCycle();

private:    
    char myCycleID[20];
    int  EditingCycle;
};
#endif // BillingCycleEdit_included
