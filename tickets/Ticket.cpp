/*
** $Id: Ticket.cpp,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** Ticket  - The class for maintaining a ticket.
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
** $Log: Ticket.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include "Ticket.h"

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <ADB.h>
#include <TAA.h>
#include <FParse.h>
#include <Cfg.h>
#include <StrTools.h>

Ticket::Ticket(long sTicketNo)
{
    clearVars();
    myTicketNo      = sTicketNo;
}

Ticket::~Ticket()
{
    if (needWrite) writeTicket();
}

/*
** clearVars   - Resets all of the internal variables to their defaults.
*/

void Ticket::clearVars()
{
    myTicketNo      = 0;
    myCustomerID    = 0;
    myType          = Support;
    myStatus        = Closed;
    myOpened        = 0;
    myClosed        = 0;
    myModified      = 0;
    memset(myName,       0, sizeof(myName));
    memset(myOwner,      0, sizeof(myOwner));
    memset(mySummary,    0, sizeof(mySummary));

    needWrite       = false;
}

/*
** ticketNo    - Returns the ticket number.  Mostly just useful when
**               a new ticket has been created.
*/

long Ticket::ticketNo()
{
    return myTicketNo;
}

/*
** setTicketNo - Sets the internal ticket number and loads the ticket
**               header from the database.  Returns a postive value
**               on success, zero on failure.
*/

int Ticket::setTicketNo(long newTicketNo)
{
    clearVars();
    int retVal = 1;
    myTicketNo = newTicketNo;
    if (newTicketNo) {
        ADBTable    tickDB("Tickets");
        tickDB.get(newTicketNo);

        if (newTicketNo != tickDB.getLong("TicketNo")) {
            myTicketNo  = 0;
            retVal      = 0;
        } else {
            myCustomerID    = tickDB.getLong("CustomerID");
            myOpened        = tickDB.getTime_t("Opened");
            myClosed        = tickDB.getTime_t("Closed");
            myModified      = tickDB.getTime_t("Modified");
            myStatus        = (t_Status) tickDB.getInt("Status");
            myType          = (t_Type)   tickDB.getInt("TicketType");
            strcpy(myName,          tickDB.getStr("Name"));
            strcpy(myOwner,         tickDB.getStr("Owner"));
            strcpy(mySummary,       tickDB.getStr("Summary"));
        }
    }
    return retVal;
}

/*
** customerID  - Returns the customer ID for this ticket.
*/

long Ticket::customerID()
{
    return myCustomerID;
}

/*
** setCustomerID  - Sets the customer ID for this ticket.
*/

void Ticket::setCustomerID(long newCustID)
{
    myCustomerID = newCustID;
    setModified(rightNow());
    needWrite = true;
    writeTicket();
}

/*
** name - Returns the contact name on the ticket.
*/

const char *Ticket::name()
{
    return myName;
}

/*
** setName - Sets the contact name for this ticket.
*/

void Ticket::setName(const char *newName)
{
    memset(myName, 0, sizeof(myName));
    strncpy(myName, newName, sizeof(myName)-1);
    needWrite = true;
    writeTicket();
}

/*
** owner - Returns the owner of the ticket.
*/

const char *Ticket::owner()
{
    return myOwner;
}

/*
** setOwner - Sets the owner of this ticket.
*/

void Ticket::setOwner(const char *newOwner)
{
    char    *oldOwner = new char[strlen(myOwner)+64];
    strcpy(oldOwner, myOwner);
    memset(myOwner, 0, sizeof(myOwner));
    strncpy(myOwner, newOwner, sizeof(myOwner)-1);
    setModified(rightNow());
    needWrite = true;
    writeTicket();
    addLogEntry(Audit, "Ticket taken from %s by %s at %s", oldOwner, myOwner, timeToStr(myModified, YYYY_MM_DD_HH_MM_SS));
}

/*
** summary - Returns the summary for this ticket
*/

const char *Ticket::summary()
{
    return mySummary;
}

/*
** setSummary - Sets the summary for this ticket
*/

void Ticket::setSummary(const char *newSummary)
{
    memset(mySummary, 0, sizeof(mySummary));
    strncpy(mySummary, newSummary, sizeof(mySummary)-1);
    needWrite = true;
    writeTicket();
}


/*
** opened  - Returns the time_t value that this ticket was opened.
*/

time_t Ticket::opened()
{
    return myOpened;
}

/*
** openedStr - Returns one of a few different formats for the ticket
**             open time.
*/

const char *Ticket::openedStr(t_TimeFormat fmt)
{
    return timeToStr(myOpened, fmt);
}

/*
** setOpened - Sets the time_t value for the ticket open date.
**             If the passwd in time is 0, the current date/time is used.
*/

void Ticket::setOpened(time_t newTime)
{
    if (newTime) myOpened = newTime;
    else myOpened = rightNow();
    needWrite = true;
    writeTicket();
}

/*
** setOpened - Sets the time_t value given a year, month, day, hour, minute
**             and optional second.
*/

void Ticket::setOpened(int y, int m, int d, int h, int min, int s)
{
    myOpened = ymdhmToTime(y, m, d, h, min, s);
    needWrite = true;
    writeTicket();
}

/*
** closed  - Returns the time_t value that this ticket was closed.
*/

time_t Ticket::closed()
{
    return myClosed;
}

/*
** closedStr - Returns one of a few different formats for the ticket
**             open time.
*/

const char *Ticket::closedStr(t_TimeFormat fmt)
{
    return timeToStr(myClosed, fmt);
}

/*
** setClosed - Sets the time_t value for the ticket open date.
**             If the passwd in time is 0, the current date/time is used.
*/

void Ticket::setClosed(time_t newTime)
{
    if (newTime) myClosed = newTime;
    else myClosed = rightNow();
    needWrite = true;
    writeTicket();
}

/*
** setClosed - Sets the time_t value given a year, month, day, hour, minute
**             and optional second.
*/

void Ticket::setClosed(int y, int m, int d, int h, int min, int s)
{
    myClosed = ymdhmToTime(y, m, d, h, min, s);
    needWrite = true;
    writeTicket();
}

/*
** modified  - Returns the time_t value that this ticket was modified.
*/

time_t Ticket::modified()
{
    return myModified;
}

/*
** modifiedStr - Returns one of a few different formats for the ticket
**             open time.
*/

const char *Ticket::modifiedStr(t_TimeFormat fmt)
{
    return timeToStr(myModified, fmt);
}

/*
** setModified - Sets the time_t value for the ticket open date.
**             If the passwd in time is 0, the current date/time is used.
*/

void Ticket::setModified(time_t newTime)
{
    if (newTime) myModified = newTime;
    else myModified = rightNow();
    needWrite = true;
    writeTicket();
}

/*
** setModified - Sets the time_t value given a year, month, day, hour, minute
**             and optional second.
*/

void Ticket::setModified(int y, int m, int d, int h, int min, int s)
{
    myModified = ymdhmToTime(y, m, d, h, min, s);
    needWrite = true;
    writeTicket();
}

/*
** type  - Returns the ticket type to the caller.
*/

Ticket::t_Type Ticket::type()
{
    return myType;
}

/*
** typeStr - Returns the textual type of ticket.
*/

const char *Ticket::typeStr()
{
    char    *retStr = new char[128];
    strcpy(retStr, "");
    switch (myType) {
        case    Support:
            strcpy(retStr, "Support");
            break;
        case    Accounting:
            strcpy(retStr, "Accounting");
            break;
        case    PreSales:
            strcpy(retStr, "Pre-Sales");
            break;
        case    Sales:
            strcpy(retStr, "Sales");
            break;
        case    Misc:
            strcpy(retStr, "Misc");
            break;
        case    Personal:
            strcpy(retStr, "Personal");
            break;
        default:
            sprintf(retStr, "Uknown type %d", myType);
            break;
    }

    return retStr;
}

/*
** setType  - Sets the type of ticket.  This change happens immediatly.
*/

void Ticket::setType(t_Type newType)
{
    myType = newType;
    needWrite = true;
    writeTicket();
}

/*
** status  - Returns the ticket type to the caller.
*/

Ticket::t_Status Ticket::status()
{
    return myStatus;
}

/*
** statusStr - Returns the textual type of ticket.
*/

const char *Ticket::statusStr()
{
    char    *retStr = new char[128];
    strcpy(retStr, "");
    switch (myStatus) {
        case    Closed:
            strcpy(retStr, "Closed");
            break;
        case    Unexamined:
            strcpy(retStr, "Unexamined");
            break;
        case    Unassigned:
            strcpy(retStr, "Unassigned");
            break;
        case    StaffPending:
            strcpy(retStr, "Staff Pending");
            break;
        case    CustPending:
            strcpy(retStr, "Cust Pending");
            break;
        case    TelcoPending:
            strcpy(retStr, "Telco Pending");
            break;
        case    VendorPending:
            strcpy(retStr, "Vendor Pending");
            break;
        default:
            sprintf(retStr, "Uknown status %d", myStatus);
            break;
    }

    return retStr;
}

/*
** setStatus  - Sets the type of ticket.  This change happens immediatly.
*/

void Ticket::setStatus(t_Status newStatus)
{
    if (myStatus != newStatus) {
        char    tmpStr[1024];
        strcpy(tmpStr, statusStr());
        myStatus = newStatus;
        // Changing the status gets an automatic log entry
        addLogEntry(Automatic, "Status changed from %s to %s by %s", tmpStr, statusStr(), curUser().userName);
        
        // Check to see if this ticket is now closed.
        if (newStatus == Closed) {
            setClosed(rightNow());
        }
        needWrite = true;
        writeTicket();
    }
}





/*
** addLogEntry - Adds a log entry of the specfied type into the TicketLog.
*/

int Ticket::addLogEntry(t_LogType theType, const char *fmt, ... )
{
    int     retVal = 0;
    char    *logStr = new char[65536];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(logStr, fmt, ap);

    ADBTable    tlDB("TicketLog");
    tlDB.setValue("TicketNo",   myTicketNo);
    tlDB.setValue("EntryBy",    curUser().userName);
    tlDB.setValue("EntryTime",  timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
    tlDB.setValue("EntryType",  (int) theType);
    tlDB.setValue("Urgent",     0);
    tlDB.setValue("LogNotes",   logStr);
    tlDB.ins();

    if (theType == Normal) sendStaffEmail();

    setModified(rightNow());
    return retVal;
}

/*
** addUrgentLogEntry - Adds an "Urgent" log entry of the specfied type into
**                     the TicketLog.
*/

int Ticket::addUrgentLogEntry(t_LogType theType, const char *fmt, ... )
{
    int     retVal = 0;
    char    *logStr = new char[65536];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(logStr, fmt, ap);

    ADBTable    tlDB("TicketLog");
    tlDB.setValue("TicketNo",   myTicketNo);
    tlDB.setValue("EntryBy",    curUser().userName);
    tlDB.setValue("EntryTime",  timeToStr(rightNow(), YYYY_MM_DD_HH_MM_SS));
    tlDB.setValue("EntryType",  (int) theType);
    tlDB.setValue("Urgent",     1);
    tlDB.setValue("LogNotes",   logStr);
    tlDB.ins();

    if (theType == Normal) sendStaffEmail();

    setModified(rightNow());
    return retVal;
}

/*
** writeTicket - Stores the ticket to the database.
*/

void Ticket::writeTicket()
{
    if (!myTicketNo) return;

    ADBTable    tickDB("Tickets");
    tickDB.get(myTicketNo);

    tickDB.setValue("CustomerID",       myCustomerID);
    tickDB.setValue("Opened",           timeToStr(myOpened,   YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Closed",           timeToStr(myClosed,   YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Modified",         timeToStr(myModified, YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Status",           (int) myStatus);
    tickDB.setValue("TicketType",       (int) myType);
    tickDB.setValue("Name",             myName);
    tickDB.setValue("Owner",            myOwner);
    tickDB.setValue("Summary",          mySummary);

    tickDB.upd();

    needWrite = false;
}

/*
** create - Creates a new ticket.
*/

long Ticket::create()
{

    ADBTable    tickDB("Tickets");
    //tickDB.get(myTicketNo);

    tickDB.setValue("CustomerID",       myCustomerID);
    tickDB.setValue("Opened",           timeToStr(myOpened,   YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Closed",           timeToStr(myClosed,   YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Modified",         timeToStr(myModified, YYYY_MM_DD_HH_MM_SS));
    tickDB.setValue("Status",           (int) myStatus);
    tickDB.setValue("TicketType",       (int) myType);
    tickDB.setValue("Name",             myName);
    tickDB.setValue("Owner",            myOwner);
    tickDB.setValue("Summary",          mySummary);

    tickDB.ins();
    myTicketNo = tickDB.getLong("TicketNo");

    needWrite = false;
    return myTicketNo;
}

/*
** fullLog - Returns a formatted log of all of the ticket entries for
**           the specified level.
*/

const char *Ticket::fullLog(t_LogType level)
{
    if (!myTicketNo) return "";

    ADB         DB;
    FParser     fparse;
    QString     tmpText;
    fparse.setPath(cfgVal("TemplatePath"));

    char        orderStr[1024];
    
    strcpy(orderStr, "order by Urgent desc, EntryTime desc");

    tmpText.append("<HTML>");
    DB.query("select * from TicketLog where TicketNo = %d and EntryType <= %d %s", myTicketNo, level, orderStr);
    if (DB.rowCount) {
        while (DB.getrow()) {
            fparse.addRow("TicketLog");
            fparse.addColumn("EntryTime",     timeToStr(DB.curRow.col("EntryTime")->toTime_t(), Mmm_DD_YYYY_HH_MMa));
            fparse.addColumn("EntryBy",       DB.curRow["EntryBy"]);
            fparse.addColumn("LogNotes",      DB.curRow["LogNotes"]);
            if (atoi(DB.curRow["Urgent"])) {
                fparse.addColumn("Urgent", cfgVal("UrgentTicketTag"));
            }
            switch((Ticket::t_LogType) atoi(DB.curRow["EntryType"])) {
                case    Ticket::Normal:
                    fparse.addColumn("EntryType", "");
                    break;

                case    Ticket::Automatic:
                    fparse.addColumn("EntryType", "Automatic: ");
                    break;

                case    Ticket::Audit:
                    fparse.addColumn("EntryType", "Audit: ");
                    break;

                default:
                    fparse.addColumn("EntryType", DB.curRow["EntryType"]);
                    break;
            }

        }
        tmpText.append(fparse.parseFileToMem("tickets/History.bhtml"));
    } else {
        tmpText.append("No log entries were found for this ticket.");
    }
    tmpText.append("</HTML>");
    char    *retStr = new char[tmpText.length()+64];
    strcpy(retStr, (const char *) tmpText);
    return  retStr;

}

/*
** sendStaffEmail - When a tickets has a "normal" log entry added, 
**                  this is called.  It will send email to all staff
**                  members, in the case of a new ticket, or the owner
**                  of a ticket if the status of a ticket has changed,
**                  or a new Non-Audit type note has been added to it.
*/

void Ticket::sendStaffEmail()
{
    ADB     DB;
    ADB     DB2;
    char    queryMod[1024];
    char    fName[1024];
    char    pFile[1024];
    char    tmpStr[1024];

    strcpy(queryMod, "");
    // If the ticket is owned by someone, we only need to send to them.
    if (strlen(myOwner)) {
        sprintf(queryMod, "and LoginID = '%s'", myOwner);
    }
    
    // Get the list of people we need to send tickets to.
    DB.query("select * from Staff where TicketNotifications > 0 %s", queryMod);
    if (DB.rowCount) {
        while (DB.getrow()) {
            FParser     VFP;
            VFP.setPath(cfgVal("TemplatePath"));
            VFP.set("LoginID",  DB.curRow["LoginID"]);

            sprintf(tmpStr, "%ld", myTicketNo);
            VFP.set("TicketNo",         tmpStr);
            VFP.set("TicketSummary",    summary());
            VFP.set("TicketStatus",     statusStr());
            VFP.set("TicketOpened",     openedStr());
            VFP.set("TicketModified",   modifiedStr());
            VFP.set("TicketType",       typeStr());
            if (strlen(owner())) {
                VFP.set("TicketOwner",      owner());
            } else {
                VFP.set("TicketOwner",  "[Unassigned]");
            }
            if (myCustomerID) {
                sprintf(tmpStr, "%ld", myCustomerID);
                VFP.set("CustomerID", tmpStr);
                DB2.query("select FullName from Customers where CustomerID = %ld", myCustomerID);
                DB2.getrow();
                VFP.set("CustomerName", DB2.curRow["FullName"]);
            } else {
                VFP.set("CustomerID",   "N/A");
                VFP.set("CustomerName", "N/A");
            }

            // Now, get the detail information for this ticket.
            DB2.query("select * from TicketLog where TicketNo = %d and EntryType <= 0 order by Urgent desc, EntryTime desc", myTicketNo);
            if (DB2.rowCount) {
                while (DB2.getrow()) {
                    VFP.addRow("TicketLog");
                    VFP.addColumn("EntryTime",     timeToStr(DB2.curRow.col("EntryTime")->toTime_t(), Mmm_DD_YYYY_HH_MMa));
                    VFP.addColumn("EntryBy",       DB2.curRow["EntryBy"]);
                    VFP.addColumn("LogNotes",      WordWrap(DB2.curRow["LogNotes"]));
                    if (atoi(DB2.curRow["Urgent"])) {
                        VFP.addColumn("Urgent", cfgVal("UrgentTicketTag"));
                    }
                    switch((Ticket::t_LogType) atoi(DB2.curRow["EntryType"])) {
                        case    Ticket::Normal:
                            VFP.addColumn("EntryType", "");
                            break;

                        case    Ticket::Automatic:
                            VFP.addColumn("EntryType", "Automatic: ");
                            break;

                        case    Ticket::Audit:
                            VFP.addColumn("EntryType", "Audit: ");
                            break;

                        default:
                            VFP.addColumn("EntryType", DB2.curRow["EntryType"]);
                            break;
                    }

                }
                // tmpText.append(VFP.parseFileToMem("tickets/History.bhtml"));
            } else {
                // tmpText.append("No log entries were found for this ticket.");
            }


            // Now, parse the file and send the email.
            sprintf(fName, "/var/spool/taamail/%s-%ld", DB.curRow["LoginID"], myTicketNo);
            FILE    *ofile = fopen(fName, "w");
            VFP.parseFile("tickets/ticketstatus.tmpl", ofile);
            fclose(ofile);
        }
    }
}


