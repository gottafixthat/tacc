/*
** $Id: NCWBilling.cpp,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** NCWBilling - 
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
** $Log: NCWBilling.cpp,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#include "NCWBilling.h"

#define Inherited NCWBillingData

NCWBilling::NCWBilling
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
}

NCWBilling::~NCWBilling()
{
}
