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

#ifndef Ticket_H
#define Ticket_H

#include <TAATools.h>
#include <sys/types.h>


class Ticket {
public:
    Ticket(long sTicketNo = 0);
    ~Ticket();

    // Some data types that define the type and status of a ticket
    enum t_Type {
        Support         = 0, Accounting      = 1, PreSales        = 2,
        Sales           = 3, Misc            = 4, Personal        = 5
    };

    enum t_Status {
        Closed          = 0, Unexamined      = 1, Unassigned      = 2,
        StaffPending    = 3, CustPending     = 4, TelcoPending    = 5, 
        VendorPending   = 6 
    };

    enum t_LogType {
        Normal          = 0, Automatic       = 1, Audit           = 2
    };

    // Gets and Sets the ticket number and loads the ticket into memory
    long        ticketNo();
    int         setTicketNo(long);
    long        create();

    long        customerID();
    void        setCustomerID(long);

    const char *name();
    void        setName(const char *);
    
    const char *owner();
    void        setOwner(const char *);
    
    const char *summary();
    void        setSummary(const char *);

    time_t      opened();
    const char *openedStr(t_TimeFormat fmt = YYYY_MM_DD_HH_MM);
    void        setOpened(time_t = 0);
    void        setOpened(int y, int m, int d, int h, int min, int s = 0);
    
    time_t      closed();
    const char *closedStr(t_TimeFormat fmt = YYYY_MM_DD_HH_MM);
    void        setClosed(time_t = 0);
    void        setClosed(int y, int m, int d, int h, int min, int s = 0);
    
    time_t      modified();
    const char *modifiedStr(t_TimeFormat fmt = YYYY_MM_DD_HH_MM);
    void        setModified(time_t = 0);
    void        setModified(int y, int m, int d, int h, int min, int s = 0);
    
    t_Type      type();
    const char *typeStr();
    void        setType(t_Type);

    t_Status    status();
    const char *statusStr();
    void        setStatus(t_Status);

    const char *fullLog(t_LogType level = Normal);

    int         addLogEntry(t_LogType, const char *fmt, ... );
    int         addUrgentLogEntry(t_LogType, const char *fmt, ... );
    
protected:
    long        myTicketNo;
    long        myCustomerID;
    char        myName[80];
    char        myOwner[64];
    char        mySummary[128];
    time_t      myOpened;
    time_t      myClosed;
    time_t      myModified;
    t_Status    myStatus;
    t_Type      myType;
    bool        needWrite;

    void        clearVars();

    void        sendStaffEmail();
    void        writeTicket();
};


#endif // Ticket_H

// vim: expandtab
