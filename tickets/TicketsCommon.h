/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
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

// vim: expandtab
