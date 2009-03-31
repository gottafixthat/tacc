/*
** $Id: adbtest.cpp,v 1.3 2000/06/12 21:48:07 marc Exp $
**
**  adbtest.cpp - A test program that calls many ADB functions with full
**                debugging.
**
**                Warning!  Full debugging spits out a _lot_ of data.
**
*****************************************************************************
**
**  (C)opyright 1998, 1999 by R. Marc Lewis and Blarg! Online Services, Inc.
**  All Rights Reserved
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
**
*****************************************************************************
**
** $Log: adbtest.cpp,v $
** Revision 1.3  2000/06/12 21:48:07  marc
** Added a new class - ADBLogin.  This is a generic login dialog that can
** be used in any Qt based program that uses ADB for database access.
** Altered the defines for using the Qt exentions to ADB.  To use them, the
** user must now define ADBQT=1 in their Makefiles.
**
** Revision 1.2  1999/10/27 00:55:41  marc
** Added some comments to the headers.
** Added appendStr and getLLong to ADBTable.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <ADB.h>

#define DBHost  "localhost"
#define DBName  "adbtest"
#define DBUser  "marc"
#define DBPass  "9!!8110"
#define DBTable "adbtest"

void test1(void);
void test2(void);
void test3(void);

main(int argc, char **argv)
{
    printf("Turning up debugging...\n");
    ADB::setDebugLevel(9);
    test1();
    test2();
    test3();
}


void test1(void)
{
    char    tmpStr[65536];

    printf("Connecting with no ADB defaults...\n");
    ADB     DB1(DBName, DBUser, DBPass, DBHost);
    
    printf("Inserting a record...\n");
    sprintf(tmpStr, "INSERT INTO %s VALUES (0, 'Char Test', 'varchar test', 123456789012345, 54321, 'Big Blob Test', '1999-10-22', '11:26:34', '19990130123456', NULL)", DBTable);
    DB1.dbcmd(tmpStr);

    printf("Performing a query...\n");
    DB1.query("select * from %s", DBTable);
    
    printf("The query returned %ld rows.  Getting the first one.\n", DB1.rowCount);
    if (DB1.rowCount) DB1.getrow();

    printf("Performing another query...\n");
    DB1.query("select * from %s order by InternalID desc", DBTable);
    
    printf("The query returned %ld rows.  Getting 2 of them...\n", DB1.rowCount);
    if (DB1.rowCount) {
        int tmpCount = 0;
        while (DB1.getrow()) {
            tmpCount++;
            if (tmpCount > 1) break;
        }
    }
    
    printf("Performing another query...\n");
    DB1.query("select varcharfield, InternalID, blobfield from %s order by InternalID desc", DBTable);
    
    printf("The query returned %ld rows.  Getting them...\n", DB1.rowCount);
    if (DB1.rowCount) {
        int tmpCount = 0;
        while (DB1.getrow()) {
            printf("Row %d: Column 0 -> '%s', Column 1 -> '%s'\n", tmpCount,
              DB1.curRow[0],
              DB1.curRow[1]
            );
            printf("Row %d: Column 'InternalID' -> '%s', Column 'blobfield' -> '%s'\n", tmpCount,
              DB1.curRow["InternalID"],
              DB1.curRow["blobfield"]
            );
            tmpCount++;
            // if (tmpCount > 1) break;
        }
    }
    
    
    printf("Escaping a string...\n");
    sprintf(tmpStr, "Test quotes = ' ' ''', test backslashes \\, test percents %%");
    printf("Source: '%s'\nEscaped: '%s'\n", tmpStr, DB1.escapeString(tmpStr));

    printf("Deleting all rows...\n");
}

void test2(void)
{
    printf("\nCreating an ADBTable...\n");
    ADBTable     DB1(DBTable, DBName, DBUser, DBPass, DBHost);
    DB1.setEncryptedColumn("blobfield");
    DB1.get(1);
    DB1.clearData();
    DB1.setValue("blobfield", "This is a test blob field with 'quoted' text");
    DB1.ins();
    DB1.clearData();
    DB1.setValue("blobfield", "This is a test blob field with 'quoted' text");
    DB1.ins();
    DB1.setValue("blobfield", "This is an update test");
    DB1.upd();

    DB1.del();
}

void test3(void)
{
    printf("\nCreating an ADBList...\n");
    ADBList DB1(DBTable, DBName, DBUser, DBPass, DBHost);
    ADBList DB2(DBTable, DBName, DBUser, DBPass, DBHost);
    DB1.setEncryptedColumn("blobfield");
    DB2.setEncryptedColumn("blobfield");
    DB1.getList();
    for (long liNo = DB1.first(); liNo != 0; liNo = DB1.next()) {
        printf("Got row %ld from the list...blobfield = '%s'\n", liNo, DB1.getStr("blobfield"));
    }
    DB2.getList("order by InternalID desc");
    for (long liNo = DB1.first(); liNo != 0; liNo = DB1.next()) {
        printf("Descending - Got row %ld from the list...blobfield = '%s'\n", liNo, DB1.getStr("blobfield"));
    }
}

