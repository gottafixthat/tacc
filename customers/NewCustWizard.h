/*
** $Id: NewCustWizard.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** NewCustWizard - A wizard for creating new customer records.
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
** $Log: NewCustWizard.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "NCWContact.h"
#include "NCWAccount.h"
#include "NCWBilling.h"

#ifndef NewCustWizard_included
#define NewCustWizard_included

#include "NewCustWizardData.h"

class NewCustWizard : public NewCustWizardData
{
    Q_OBJECT

public:

    NewCustWizard(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~NewCustWizard();

protected slots:
    virtual void accept();
    virtual void QCloseEvent(QEvent *)              { delete this; }

private:
    NCWContact      *contact;
    NCWAccount      *account;
    NCWBilling      *billing;
};
#endif // NewCustWizard_included
