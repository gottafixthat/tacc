/*
** $Id$
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
** $Log: VHostUtils.cpp,v $
** Revision 1.1  2004/01/22 00:02:10  marc
** Virtual Hosting database access is now complete in a basic form.
**
**
*/

#include <TAATools.h>
#include <ADB.h>
#include <Cfg.h>
#include <BrassClient.h>
#include <qstring.h>

// Utility functions

/*
** hasVHostRecord - Checks the main Apache VHosts SQL database and returns 0
**                  if the domain is not present in the database,
**                  1 if it is.
*/

int hasVHostRecord(const char *domain)
{
    ADB vhostDB(cfgVal("VHostSQLDB"), cfgVal("VHostSQLUser"), cfgVal("VHostSQLPass"), cfgVal("VHostSQLHost"));

    vhostDB.query("select VHostID from VHosts where HostName = '%s'", vhostDB.escapeString(domain));
    return (vhostDB.rowCount);
}

/*
** createVHostRecord - Creates a new domain entry in the Apache VHost
**                     database.  It will connect to the BRASS daemon to
**                     get the user information.
*/

int createVHostRecord(const char *domain, long customerID, const char *loginID)
{
    int retVal = 1;     // Assume success
    BrassClient *BC;
    LoginDumpStruct dump;

    BC = new BrassClient();
    if (!BC->Connect()) return 0;

    if (!BC->Authenticate()) return 0;

    if (!BC->DumpUser(loginID, dump)) return 0;

    // We have the dump info.  Insert the hosting entry
    ADBTable    vhostDB("VHosts", cfgVal("VHostSQLDB"), cfgVal("VHostSQLUser"), cfgVal("VHostSQLPass"), cfgVal("VHostSQLHost"));
    // First, check to make sure we don't already have a
    // VHost entry for this domain.
    vhostDB.query("select VHostID from VHosts where HostName = '%s'", domain);
    if (vhostDB.rowCount) return 0;

    char    docRoot[1024];
    char    adminEmail[1024];
    strcpy(docRoot, dump.home);
    strcat(docRoot, "/web_docs");
    strcpy(adminEmail, "webmaster@");
    strcat(adminEmail, domain);

    vhostDB.setValue("VHostID",     0);
    vhostDB.setValue("CustomerID",  customerID);
    vhostDB.setValue("LoginID",     loginID);
    vhostDB.setValue("HostName",    domain);
    vhostDB.setValue("DocumentRoot",docRoot);
    vhostDB.setValue("UID",         dump.uid);
    vhostDB.setValue("GID",         dump.gid);
    vhostDB.setValue("AdminEmail",  adminEmail);
    vhostDB.setValue("Active",      1);
    retVal = vhostDB.ins();

    delete BC;

    return retVal;
}


/*
** removeVHostRecord - Deletes any VHost records for the domain.
*/

int removeVHostRecord(const char *domain)
{
    ADB vhostDB(cfgVal("VHostSQLDB"), cfgVal("VHostSQLUser"), cfgVal("VHostSQLPass"), cfgVal("VHostSQLHost"));

    vhostDB.dbcmd("delete from VHosts where HostName = '%s'", vhostDB.escapeString(domain));
    return 1;
}


