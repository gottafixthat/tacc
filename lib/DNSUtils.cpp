/*
** $Id$
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
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNSUtils.cpp,v $
** Revision 1.8  2004/03/25 00:56:28  marc
** Brown bag bug with the regex replacement of the domain.
**
** Revision 1.7  2004/03/15 22:30:47  marc
** Updated QString replace calls with regexps
**
** Revision 1.6  2004/01/04 20:00:30  marc
** Added the Active column to the ZoneInfo table
**
** Revision 1.5  2004/01/03 18:11:26  marc
** *** empty log message ***
**
** Revision 1.4  2004/01/03 00:10:05  marc
** More minor changes to the qstring::replace
**
** Revision 1.3  2004/01/03 00:00:22  marc
** Minor tweaks to the domain name parsing.
**
** Revision 1.2  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.1  2004/01/02 14:59:01  marc
** DNSTemplatePicker complete, added DNSUtils
**
**
*/

#include <TAATools.h>
#include <ADB.h>
#include <Cfg.h>
#include <qstring.h>
#include <qregexp.h>

// Utility functions

/*
** hasDNSRecord - Checks the main MyDNS SQL database and returns 0
**                if the domain is not present in the database,
**                1 if it is.
*/
int hasDNSRecord(const char *domain)
{
    ADB     dnsDB(cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));

    dnsDB.query("select id from soa where origin = '%s.'", dnsDB.escapeString(domain));
    return (dnsDB.rowCount);
}

/*
** createDNSRecord - Creates a new domain entry in the MyDNS database, it is
**                   "blank" and will contain no records other than the
**                   base SOA information.
*/
int createDNSRecord(const char *domain, long customerID, const char *loginID, int propogate, int custEdit)
{
    ADBTable    dnsDB("soa", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    ADBTable    rrDB("rr", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    ADBTable    zoneDB("ZoneInfo", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    char        tmpStr[1024];
    char        serial[1024];
    QDate       tmpDate(QDate::currentDate());


    sprintf(tmpStr, "%s.", domain);
    sprintf(serial, "%04d%02d%02d01", tmpDate.year(), tmpDate.month(), tmpDate.day());
    dnsDB.setValue("origin",    tmpStr);
    dnsDB.setValue("ns",        "ns1.avvanta.com.");
    dnsDB.setValue("mbox",      "hostmaster.avvanta.com.");
    dnsDB.setValue("serial",    atol(serial));
    dnsDB.setValue("refresh",   28800);
    dnsDB.setValue("retry",     7200);
    dnsDB.setValue("expire",    604800);
    dnsDB.setValue("minimum",   86400);
    dnsDB.setValue("ttl",       86400);
    dnsDB.ins();
    
    // Now create the two NS records by default.
    rrDB.setValue("zone",       dnsDB.getLong("id"));
    rrDB.setValue("name",       "");
    rrDB.setValue("type",       "NS");
    rrDB.setValue("data",       "ns1.avvanta.com.");
    rrDB.setValue("ttl",        "86400");
    rrDB.ins();
    rrDB.setValue("id",         0);
    rrDB.setValue("data",       "ns2.avvanta.com.");
    rrDB.ins();

    zoneDB.setValue("zoneid",       dnsDB.getLong("id"));
    zoneDB.setValue("CustomerID",   customerID);
    zoneDB.setValue("LoginID",      loginID);
    zoneDB.setValue("Propogate",    propogate);
    zoneDB.setValue("AllowCustEdit",custEdit);
    zoneDB.setValue("TemplateID",   0);
    zoneDB.setValue("Active",       1);
    zoneDB.ins();

    return dnsDB.getLong("id");
}


/*
** setupDNSFromTemplate - Creates a new DNS Zone on based on a template.
**                         all occurances of {DOMAIN} in the host or IP
**                         columns will be replaced with the domain name.
*/
int setupDNSFromTemplate(int templateID, const char *domain, long customerID, const char *loginID)
{
    ADBTable    dnsDB("soa", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    ADBTable    rrDB("rr", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    ADBTable    zoneDB("ZoneInfo", cfgVal("DNSSQLDB"), cfgVal("DNSSQLUser"), cfgVal("DNSSQLPass"), cfgVal("DNSSQLHost"));
    ADB         DB;
    char        tmpStr[1024];
    char        serial[1024];
    QDate       tmpDate(QDate::currentDate());
    QString     name;
    QString     data;
    int         propogate;
    int         custEdit;
    QString     domainStr;

    domainStr = domain;

    // Load the template SOA 
    DB.query("select * from DNS_Templates where TemplateID = %d", templateID);
    if (!DB.rowCount) return 0;
    DB.getrow();

    propogate = atoi(DB.curRow["Propogate"]);
    custEdit  = atoi(DB.curRow["AllowCustEdit"]);

    sprintf(tmpStr, "%s.", domain);
    sprintf(serial, "%04d%02d%02d01", tmpDate.year(), tmpDate.month(), tmpDate.day());
    dnsDB.setValue("origin",    tmpStr);
    dnsDB.setValue("ns",        "ns1.avvanta.com.");
    dnsDB.setValue("mbox",      "hostmaster.avvanta.com.");
    dnsDB.setValue("serial",    atol(serial));
    dnsDB.setValue("refresh",   DB.curRow["Refresh"]);
    dnsDB.setValue("retry",     DB.curRow["Retry"]);
    dnsDB.setValue("expire",    DB.curRow["Expire"]);
    dnsDB.setValue("minimum",   DB.curRow["Minimum"]);
    dnsDB.setValue("ttl",       DB.curRow["TTL"]);
    dnsDB.ins();
    
    // Now, get each of the resource records in the template and
    // add them into the remote rr database after replacing {DOMAIN} with
    // the domain name.
    DB.query("select * from DNS_Templates_RR where TemplateID = %d", templateID);
    if (DB.rowCount) while (DB.getrow()) {
        name = DB.curRow["Name"];
        data = DB.curRow["Data"];
        // Now create the two NS records by default.
        rrDB.setValue("id",         0);
        rrDB.setValue("zone",       dnsDB.getLong("id"));
        rrDB.setValue("name",       (const char *)name.replace("{DOMAIN}", domainStr));
        rrDB.setValue("type",       DB.curRow["RRType"]);
        rrDB.setValue("data",       (const char *)data.replace("{DOMAIN}", domainStr));
        rrDB.setValue("aux",        DB.curRow["Aux"]);
        rrDB.setValue("ttl",        DB.curRow["TTL"]);
        rrDB.ins();
    }

    zoneDB.setValue("zoneid",       dnsDB.getLong("id"));
    zoneDB.setValue("CustomerID",   customerID);
    zoneDB.setValue("LoginID",      loginID);
    zoneDB.setValue("Propogate",    propogate);
    zoneDB.setValue("AllowCustEdit",custEdit);
    zoneDB.setValue("TemplateID",   templateID);
    zoneDB.setValue("Active",       1);
    zoneDB.ins();

    return dnsDB.getLong("id");
}




