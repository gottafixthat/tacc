/*
** $Id: VHostUtils.h,v 1.1 2004/01/22 00:02:10 marc Exp $
**
***************************************************************************
**
** VHostUtils - SQL based Virtual Hosting Utility functions.  These are all 
**              functions that don't have widgets associated with them.  
**              Functions such as creating a new VHost entry, determining
**              if there is a VHost entry for a domain, removing an entry,
**              etc.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2003, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: VHostUtils.h,v $
** Revision 1.1  2004/01/22 00:02:10  marc
** Virtual Hosting database access is now complete in a basic form.
**
**
*/

#ifndef VHostUtils_included
#define VHostUtils_included

// Utility functions
int hasVHostRecord(const char *domain);
int createVHostRecord(const char *domain, long customerID, const char *loginID);
int removeVHostRecord(const char *domain);

#endif // DNSUtils_included

