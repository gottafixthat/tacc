/*
** $Id$
**
**  qwestconnect.cpp - A filter that takes a Qwest connect notice on
**                     stdin, searches for a matching customer phone
**                     number, then stores it in the Customer's notes.
**
**                     Ultimately, we want it to automatically create
**                     sales tickets for customers with no matching phone
**                     numbers so we can contact them.
**
*****************************************************************************
**
**  (C)opyright 1998-2002 by R. Marc Lewis and Blarg! Online Services, Inc.
**  All Rights Reserved
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
**
*****************************************************************************
** $Log: qwestconnect.cpp,v $
** Revision 1.1  2004/08/21 15:45:01  marc
** Added qwestconnect mail filter for connect notices.
**
**
*/

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

// Mime++ includes
#include <mimepp/config.h>
#include <mimepp/enum.h>
#include <mimepp/protocol.h>
#include <mimepp/string.h>
#include <mimepp/message.h>
#include <mimepp/headers.h>
#include <mimepp/text.h>
#include <mimepp/mboxlist.h>
#include <mimepp/mailbox.h>
#include <mimepp/body.h>
#include <mimepp/bodypart.h>
#include <mimepp/disptype.h>
#include <mimepp/addrlist.h>
#include <mimepp/field.h>
#include <mimepp/utility.h>
#include <mimepp/msgid.h>

#include <StrTools.h>
#include <ADB.h>
#include <Cfg.h>
#include <Ticket.h>

typedef struct ConnectNotice {
    long    noticeID;
    long    customerID;
    long    ticketNo;
    char    noticeDate[256];
    char    connType[128];
    char    custName[128];
    char    addr1[128];
    char    addr2[128];
    char    addr3[128];
    char    isSelect[10];
    char    cbr[128];
    char    dueDate[128];
    char    custCirc[128];
    char    hostCirc[128];
    char    speed[128];
    char    vpi[128];
    char    vci[128];
};

// Function prototypes
int  main(int, char **);
int  extractConnectInfo(FILE *fp, ConnectNotice &cn);
int  storeConnectNotice(ConnectNotice &cn);
int  createSalesTicket(ConnectNotice &cn);
void scanOrphans();


int main(int, char **)
{

    // Load our configuration file
    if (!loadCfg("/usr/local/etc/taa.cf")) {
        syslog(LOG_NOTICE, "Unable to load configuration file.  Aborting.\n");
        exit(-1);
    }
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));

    openlog("qwestconnect", LOG_PID, LOG_DAEMON);

    ConnectNotice   cn;
    int             retVal;
    retVal = extractConnectInfo(stdin, cn);

    if (retVal > 0) {
        //printf("Got connection info successfully.\n");
        //printf("Action = '%s', DSL Number = '%s'\n", cn.connType, cn.custCirc);
        retVal = storeConnectNotice(cn);
        if (retVal > 0) {
            syslog(LOG_NOTICE, "Successfully stored Qwest Connect info for DSL Number '%s'\n", cn.custCirc);
        } else {
            syslog(LOG_NOTICE, "No matching customers found for DSL Number '%s'\n", cn.custCirc);
            createSalesTicket(cn);
        }
    } else {
        // printf("Unable to get connection info (retcode = %d).\n", retVal);
    }

    // Regardless of anything else we have done, we now check for
    // to see if any orphaned records (those without a customer ID)
    // have now been added into the system.  So, each time we get a 
    // connect notice, it will "fix" the database.  Since we get connect
    // notices often, this will keep things pretty well up-to-date.
    scanOrphans();
}

/*
** extractConnectInfo - Extracts a single connect notice from the passed
**                      in file.
**
**                      Returns 1 if successful, or < 1 on failure.
*/

int extractConnectInfo(FILE *fp, ConnectNotice &cn)
{
    // First, clear out the connect notice.
    cn.noticeID = 0;
    cn.customerID = 0;
    cn.ticketNo = 0;
    strcpy(cn.noticeDate, "");
    strcpy(cn.connType, "");
    strcpy(cn.custName, "");
    strcpy(cn.addr1, "");
    strcpy(cn.addr2, "");
    strcpy(cn.addr3, "");
    strcpy(cn.isSelect, "");
    strcpy(cn.cbr, "");
    strcpy(cn.dueDate, "");
    strcpy(cn.custCirc, "");
    strcpy(cn.hostCirc, "");
    strcpy(cn.speed, "");
    strcpy(cn.vpi, "");
    strcpy(cn.vci, "");

    // Create our buffers and work variables.
    int     retVal = 0;
    int     bufsize = 65536;
    char    *buf = new char[bufsize];
    __gnu_cxx::StrSplitMap tmpList;
    __gnu_cxx::StrSplitMap bodyLines;
    int         lineCount;
    string      tmpStr1;
    string      tmpStr2;
    int         splitCount;
    size_t      byteCount;
    time_t      msgUnixTime;

    // Load the entire message into memory.
    byteCount = fread((void *)buf, sizeof(char), bufsize, fp);
    if (!byteCount) return(retVal);

    // Okay the whole file should be in memory now.  Parse it as a DwMessage
    DwMessage   msg(buf);
    msg.Parse();
    msgUnixTime = msg.Headers().Date().AsCalendarTime();
    tm *t = localtime(&msgUnixTime);
    int theYear = t->tm_year + 1900;
    sprintf(cn.noticeDate, "%04d-%02d-%02d %02d:%02d:%02d",
            theYear, t->tm_mon+1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);

    lineCount = StrSplit(msg.Body().AsString().c_str(), "\n", bodyLines);

    // Loop through the file until EOF.
    //while ((fgets(buf, bufsize, fp) != NULL)) {
    for (int i = 0; i < lineCount; i++) {
        strcpy(buf, bodyLines[i].c_str());
        // Strip off the whitespace.
        tmpList.clear();
        tmpStr1 = buf;
        StripWhiteSpace(tmpStr1);
        splitCount = StrSplit(tmpStr1.c_str(), " ", tmpList, 2);
        
        // Look for any of our values
        if (!strncasecmp(buf, "ACTION", strlen("ACTION"))) {
            strcpy(cn.connType, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "NAME", strlen("NAME"))) {
            strcpy(cn.custName, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "ADDR1", strlen("ADDR1"))) {
            strcpy(cn.addr1, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "ADDR2", strlen("ADDR2"))) {
            strcpy(cn.addr2, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "ADDR3", strlen("ADDR3"))) {
            strcpy(cn.addr3, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "SELECT256", strlen("SELECT256"))) {
            strcpy(cn.isSelect, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "CBR", strlen("CBR"))) {
            strcpy(cn.cbr, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "DUEDATE", strlen("DUEDATE"))) {
            strcpy(cn.dueDate, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "CKTTN", strlen("CKTTN"))) {
            strcpy(cn.custCirc, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "HOSTCKT", strlen("HOSTCKT"))) {
            strcpy(cn.hostCirc, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "SPEED", strlen("SPEED"))) {
            strcpy(cn.speed, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "VPI", strlen("VPI"))) {
            strcpy(cn.vpi, tmpList[1].c_str());
        }
        if (!strncasecmp(buf, "VCI", strlen("VCI"))) {
            strcpy(cn.vci, tmpList[1].c_str());
        }
    }

    // Now, if we got a CKKTN, and an ACTION, it is good.
    if (strlen(cn.connType) && strlen(cn.custCirc)) retVal = 1;

    return retVal;
}

/*
** storeConnectNotice - Searces the TAA phone number database for the DSL
**                      phone number, and if found, it adds a note into
**                      the customers notes with the connect information.
**
**                      Returns 1 if successful, or < 1 on failure.
*/

int storeConnectNotice(ConnectNotice &cn)
{
    int retVal = 0;

    // Before we do anything else, make sure that the Customer's DSL
    // number is a full 10 digit number.
    if (strlen(cn.custCirc) != 10) return retVal;
    
    ADB     DB;

    char    tmpPhone[128];
    string  workStr;
    char    noteText[1024];
    
    
    // Some time functions.
    time_t  curTime;
    char    curTimeDate[1024];
    struct  timeval tv;
    // Get GMT
    gettimeofday(&tv, 0);
    curTime = tv.tv_sec;
    // Now, convert GMT into local time
    tm *t = localtime(&curTime);
    int theYear = t->tm_year + 1900;

    sprintf(curTimeDate, "%04d-%02d-%02d %02d:%02d:%02d",
            theYear, t->tm_mon+1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);

    // We need to turn the DSL phone number into something we can find
    // in the database using any phone number delimiter.
    strcpy(tmpPhone, "");
    workStr = cn.custCirc;
    strcat(tmpPhone, workStr.substr(0, 3).c_str());
    strcat(tmpPhone, "%%");
    strcat(tmpPhone, workStr.substr(3, 3).c_str());
    strcat(tmpPhone, "%%");
    strcat(tmpPhone, workStr.substr(6, 4).c_str());
    strcat(tmpPhone, "%%");

    //printf("select * from PhoneNumbers where PhoneNumber LIKE '%s'\n", tmpPhone);
    DB.query("select * from PhoneNumbers where PhoneNumber LIKE '%s'", tmpPhone);
    if (DB.rowCount) {
        DB.getrow();
        syslog(LOG_NOTICE, "Found a phone number match (CustID %s)\n", DB.curRow["RefID"]);
        cn.customerID = atol(DB.curRow["RefID"]);
        ADBTable    notesDB("Notes");
        notesDB.setValue("AutoNote",    1);
        notesDB.setValue("AddedBy",     "Auto");
        notesDB.setValue("NoteDate",    cn.noticeDate);
        notesDB.setValue("CustomerID",  DB.curRow["RefID"]);
        notesDB.setValue("NoteType",    "Qwest DSL");
        notesDB.setValue("Category",    cn.connType);
        notesDB.setValue("SubCategory", "Connect Notice");
        notesDB.setValue("Subject",     cn.connType);
        strcpy(noteText, "");
        strcat(noteText, "Action ");
        strcat(noteText, cn.connType);
        strcat(noteText, "\n");

        strcat(noteText, "Name ");
        strcat(noteText, cn.custName);
        strcat(noteText, "\n");
        
        strcat(noteText, "Addr1 ");
        strcat(noteText, cn.addr1);
        strcat(noteText, "\n");

        strcat(noteText, "Addr2 ");
        strcat(noteText, cn.addr2);
        strcat(noteText, "\n");

        strcat(noteText, "Addr3 ");
        strcat(noteText, cn.addr3);
        strcat(noteText, "\n");

        strcat(noteText, "Select ");
        strcat(noteText, cn.isSelect);
        strcat(noteText, "\n");

        strcat(noteText, "CBR Number ");
        strcat(noteText, cn.cbr);
        strcat(noteText, "\n");

        strcat(noteText, "DSL Phone ");
        strcat(noteText, cn.custCirc);
        strcat(noteText, "\n");

        strcat(noteText, "Blarg Circuit ");
        strcat(noteText, cn.hostCirc);
        strcat(noteText, "\n");

        strcat(noteText, "Due Date ");
        strcat(noteText, cn.dueDate);
        strcat(noteText, "\n");

        strcat(noteText, "Speed ");
        strcat(noteText, cn.speed);
        strcat(noteText, "\n");

        strcat(noteText, "VPI ");
        strcat(noteText, cn.vpi);
        strcat(noteText, "\n");

        strcat(noteText, "VCI ");
        strcat(noteText, cn.vci);
        strcat(noteText, "\n");

        notesDB.setValue("NoteText", noteText);
        notesDB.ins();

        retVal = 1;
    } else {
        // fprintf(stderr, "No matching phone numbers for '%s'\n", tmpPhone);
    }

    // Store it in the QwestDSLNotices table now.
    ADBTable    QDB("QwestDSLNotices");
    QDB.setValue("CustomerID",  cn.customerID);
    QDB.setValue("NoticeDate",  cn.noticeDate);
    QDB.setValue("Action",      cn.connType);
    QDB.setValue("Name",        cn.custName);
    QDB.setValue("Addr1",       cn.addr1);
    QDB.setValue("Addr2",       cn.addr2);
    QDB.setValue("Addr3",       cn.addr3);
    QDB.setValue("Select256",   cn.isSelect);
    QDB.setValue("CBR",         cn.cbr);
    QDB.setValue("DueDate",     cn.dueDate);
    QDB.setValue("DSLNumber",   cn.custCirc);
    QDB.setValue("BlargCircuit",cn.hostCirc);
    QDB.setValue("LineSpeed",   cn.speed);
    QDB.setValue("VPI",         cn.vpi);
    QDB.setValue("VCI",         cn.vci);
    QDB.ins();
    cn.noticeID = QDB.getLong("NoticeID");
    
    return retVal;
}


/*
** createSalesTicket - Creates a sales ticket for DSL connect notices
**                     for whom we were unable to locate a matching 
**                     phone number in the database for.
**
**                     Returns 1 if successful, or < 1 on failure.
*/

int createSalesTicket(ConnectNotice &cn)
{
    char    noteText[4096];
    char    summary[4096];
    Ticket  newTick;
    sprintf(summary, "Qwest DSL - %s %s", cn.connType, cn.custCirc);
    newTick.setName(cn.custName);
    newTick.setSummary(summary);
    newTick.setOpened();
    newTick.setModified();
    newTick.setStatus(Ticket::Unexamined);
    newTick.setType(Ticket::Sales);
    newTick.create();


    strcpy(noteText, "The following Qwest connect notice has been recieved and no matching phone numbers could be found for it.<P>\n");
    strcat(noteText, "Action ");
    strcat(noteText, cn.connType);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Notice Date ");
    strcat(noteText, cn.noticeDate);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Name ");
    strcat(noteText, cn.custName);
    strcat(noteText, "<BR>\n");
    
    strcat(noteText, "Addr1 ");
    strcat(noteText, cn.addr1);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Addr2 ");
    strcat(noteText, cn.addr2);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Addr3 ");
    strcat(noteText, cn.addr3);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Select ");
    strcat(noteText, cn.isSelect);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "CBR Number ");
    strcat(noteText, cn.cbr);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "DSL Phone ");
    strcat(noteText, cn.custCirc);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Blarg Circuit ");
    strcat(noteText, cn.hostCirc);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Due Date ");
    strcat(noteText, cn.dueDate);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "Speed ");
    strcat(noteText, cn.speed);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "VPI ");
    strcat(noteText, cn.vpi);
    strcat(noteText, "<BR>\n");

    strcat(noteText, "VCI ");
    strcat(noteText, cn.vci);
    strcat(noteText, "<BR>\n");

    newTick.addLogEntry(Ticket::Normal, "%s", noteText);

    ADB tmpDB;
    cn.ticketNo = newTick.ticketNo();
    tmpDB.dbcmd("update QwestDSLNotices set TicketNo = %ld where NoticeID = %ld", cn.ticketNo, cn.noticeID);

    return 1;
}

/*
** scanOrphans - Scans through the database looking for connect notices
**               that have no customer ID.  It then searches for their
**               phone number again, and if it finds it, it will put a
**               copy of the connect notice into the customer's notes.
**               It will also look for an open sales ticket for them, and
**               if it finds one, it will auto-close it.
*/

void scanOrphans()
{
    ADB     connDB;
    ADB     phoneDB;
    ADB     workDB;
    char    tmpPhone[1024];
    string  workStr;
    long    custID;
    char    noteText[4096];

    connDB.query("select * from QwestDSLNotices where CustomerID = 0");
    if (!connDB.rowCount) return;
    while (connDB.getrow()) {
        strcpy(tmpPhone, "");
        workStr = connDB.curRow["DSLNumber"];
        strcat(tmpPhone, workStr.substr(0, 3).c_str());
        strcat(tmpPhone, "%%");
        strcat(tmpPhone, workStr.substr(3, 3).c_str());
        strcat(tmpPhone, "%%");
        strcat(tmpPhone, workStr.substr(6, 4).c_str());
        strcat(tmpPhone, "%%");

        //printf("select * from PhoneNumbers where PhoneNumber LIKE '%s'\n", tmpPhone);
        phoneDB.query("select * from PhoneNumbers where PhoneNumber LIKE '%s'", tmpPhone);
        if (phoneDB.rowCount) {
            // Found a matching phone number
            phoneDB.getrow();
            custID = atol(phoneDB.curRow["RefID"]);
            workDB.dbcmd("update QwestDSLNotices set CustomerID = %ld where NoticeID = %ld", custID, atol(connDB.curRow["NoticeID"]));

            // Now, add the notice into the customer notes.
            ADBTable    notesDB("Notes");
            notesDB.setValue("AutoNote",    1);
            notesDB.setValue("AddedBy",     "Auto");
            notesDB.setValue("NoteDate",    connDB.curRow["NoticeDate"]);
            notesDB.setValue("CustomerID",  custID);
            notesDB.setValue("NoteType",    "Qwest DSL");
            notesDB.setValue("Category",    connDB.curRow["Action"]);
            notesDB.setValue("SubCategory", "Connect Notice");
            notesDB.setValue("Subject",     connDB.curRow["Action"]);
            strcpy(noteText, "");
            strcat(noteText, "Action ");
            strcat(noteText, connDB.curRow["Action"]);
            strcat(noteText, "\n");

            strcat(noteText, "Name ");
            strcat(noteText, connDB.curRow["Name"]);
            strcat(noteText, "\n");
            
            strcat(noteText, "Addr1 ");
            strcat(noteText, connDB.curRow["Addr1"]);
            strcat(noteText, "\n");

            strcat(noteText, "Addr2 ");
            strcat(noteText, connDB.curRow["Addr2"]);
            strcat(noteText, "\n");

            strcat(noteText, "Addr3 ");
            strcat(noteText, connDB.curRow["Addr3"]);
            strcat(noteText, "\n");

            strcat(noteText, "Select ");
            strcat(noteText, connDB.curRow["Select256"]);
            strcat(noteText, "\n");

            strcat(noteText, "CBR Number ");
            strcat(noteText, connDB.curRow["CBR"]);
            strcat(noteText, "\n");

            strcat(noteText, "DSL Phone ");
            strcat(noteText, connDB.curRow["DSLNumber"]);
            strcat(noteText, "\n");

            strcat(noteText, "Blarg Circuit ");
            strcat(noteText, connDB.curRow["BlargCircuit"]);
            strcat(noteText, "\n");

            strcat(noteText, "Due Date ");
            strcat(noteText, connDB.curRow["DueDate"]);
            strcat(noteText, "\n");

            strcat(noteText, "Speed ");
            strcat(noteText, connDB.curRow["LineSpeed"]);
            strcat(noteText, "\n");

            strcat(noteText, "VPI ");
            strcat(noteText, connDB.curRow["VPI"]);
            strcat(noteText, "\n");

            strcat(noteText, "VCI ");
            strcat(noteText, connDB.curRow["VCI"]);
            strcat(noteText, "\n");

            notesDB.setValue("NoteText", noteText);
            notesDB.ins();

            // Now that the note has been inserted, check for any
            // tickets that may be opened on this line still.
            if (atol(connDB.curRow["TicketNo"])) {
                // We have a ticket.  Load it up.
                Ticket  tmpTick;
                tmpTick.setTicketNo(atol(connDB.curRow["TicketNo"]));
                if (tmpTick.status() != Ticket::Closed) {
                    // The ticket is open.  Set the customer ID and
                    // close it.
                    tmpTick.setCustomerID(custID);
                    tmpTick.setStatus(Ticket::Closed);
                    tmpTick.addLogEntry(Ticket::Normal, "DSL Number %s found for Customer ID %ld.  Auto-closing ticket.", connDB.curRow["DSLNumber"], custID);
                }
            }
        }
    }
}

