/*
** $Id: TicketsCommon.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** TicketsCommon.h - Some global definitions for tickets.  This is where
**                   we will define our different types of tickets, and
**                   such.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: TicketsCommon.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef TicketsCommon_H
#define TicketsCommon_H

enum TicketTypes {
    Support         = 0,
    Accounting      = 1,
    PreSales        = 2,
    Sales           = 3,
    Misc            = 4,
    Personal        = 5
};

const char TicketTypeStrings[7][40] = {
    "Support",
    "Accounting",
    "Pre-Sales",
    "Sales",
    "Misc",
    "Peronsal",
    ""
};


enum TicketStatusTypes {
    Closed          = 0,
    BlargPending    = 1,
    CustPending     = 2,
    TelcoPending    = 3,
    VendorPending   = 4
};

const char TicketStatusStrings[6][40] = {
    "Closed",
    "In Progress",
    "Customer Pending",
    "Telco Pending",
    "Vendor Pending",
    ""
};

// Some common functions

#endif // TicketsCommon_H
