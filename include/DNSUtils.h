/*
** $Id: DNSUtils.h,v 1.2 2004/01/02 23:56:14 marc Exp $
**
***************************************************************************
**
** DNSUtils - SQL based DNS Utility functions.  These are all functions
**            that don't have widgets associated with them.  Functions
**            such as creating a DNS entry from a template, creating a
**            "blank" entry, etc.
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
** $Log: DNSUtils.h,v $
** Revision 1.2  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.1  2004/01/02 14:59:39  marc
** Added DNSUtils.h, removed some functions from DNSManager.h and put them
** into DNSUtils
**
**
*/

#ifndef DNSUtils_included
#define DNSUtils_included

#define DNS_TMPL        0
#define DNS_SLAVE       1
#define DNS_SOHO_SLAVE  2
#define DNS_SOHO        3
#define DNS_STD_TMPL    4
#define DNS_STD         5
#define DNS_REV         6

// Utility functions
int hasDNSRecord(const char *domain);
int createDNSRecord(const char *domain, long customerID, const char *loginID, int propogate = 1, int userEdit = 0);
int setupDNSFromTemplate(int templateID, const char *domain, long customerID, const char *loginID);

#endif // DNSUtils_included

