/*
** $Id$
**
***************************************************************************
**
** NewCustWizard - A wizard for creating new customers records.
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
** $Log: NewCustWizard.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "NewCustWizard.h"

#define Inherited NewCustWizardData

NewCustWizard::NewCustWizard
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption("New Customer Wizard");

    contact     = new NCWContact();
    account     = new NCWAccount();
    billing     = new NCWBilling();

    addPage(contact,    "Contact Information");
    addPage(account,    "Account Information");
    addPage(billing,    "Billing/Payment Information");

    setHelpEnabled(contact,     false);
    setHelpEnabled(account,     false);
    setHelpEnabled(billing,     false);
}

NewCustWizard::~NewCustWizard()
{
}

/*
** accept   - Saves the entry and closes the window.
*/

void NewCustWizard::accept()
{
    close();
}



