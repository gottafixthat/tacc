/*
** $Id: ADBList.cpp,v 1.3 1999/10/27 00:55:41 marc Exp $
**
**  ADBList.cpp - An ADBTable hybrid that simulates a linked list.
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
** $Log: ADBList.cpp,v $
** Revision 1.3  1999/10/27 00:55:41  marc
** Added some comments to the headers.
** Added appendStr and getLLong to ADBTable.
**
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ADB.h>
#ifdef USEDES
#include "bdes.h"
#endif




ADBList::ADBList(
  const char *Table,
  const char *Name,
  const char *User,
  const char *Pass,
  const char *Host
) : ADBTable (Table, Name, User, Pass, Host)
{
    keyList  = NULL;
    totKeys  = 0;
    curKeyNo = 0;
}

ADBList::~ADBList()
{
    if (keyList) {
        free(keyList);
        keyList = NULL;
    }
}

/*
** ADBList::getList()  - Performs a query on the specified table and gets the
**                       list of primary keys for it.
*/

long ADBList::getList(const char *listQuery)
{
    long    retVal = 0;
    
    if (keyList) {
        free(keyList);
        keyList = NULL;
    }
    
    totKeys  = 0;
    curKeyNo = 0;

    if (numColumns) {
        char *myListQuery = NULL;
        if (listQuery) {
            myListQuery = (char *) calloc(strlen(listQuery)+512, sizeof(char));
            strcpy(myListQuery, listQuery);
        } else {
            myListQuery = (char *) calloc(256, sizeof(char));
        }
        query("SELECT %s FROM %s %s", 
          columnDefs[primaryKeyColumn]->ColumnName(),
          TableName,
          myListQuery
        );
        
        if (rowCount) {
            totKeys = rowCount;
            retVal  = rowCount;

            keyList = (long *) calloc(totKeys+1, sizeof(long));
            while (getrow()) {
                keyList[curKeyNo] = atol(curRow[0]);
                curKeyNo++;
            }
            first();
        }

        free(myListQuery);
    }
    
    return retVal;
}

/*
** ADBList::first() - Loads the first row from the database and adjusts 
**                    our pointer.
**
**                    Returns the primary key value for the row if 
**                    successful, or 0 if we weren't.
*/

long ADBList::first(void)
{
    long    retVal = 0;
    if (totKeys) {
        curKeyNo = 0;
        retVal   = keyList[curKeyNo];
        get(keyList[retVal]);
    }
    return retVal;
}

/*
** ADBList::last() - Loads the the last row from the database and adjusts 
**                   our pointer.
**
**                   Returns the primary key value for the row if 
**                   successful, or 0 if we weren't.
*/

long ADBList::last(void)
{
    long    retVal = 0;
    if (totKeys) {
        curKeyNo = totKeys - 1;
        retVal   = keyList[curKeyNo];
        get(keyList[retVal]);
    }
    return retVal;
}

/*
** ADBList::prev() - Loads the the previous row from the database and adjusts 
**                   our pointer.
**
**                   Returns the primary key value for the row if 
**                   successful, or 0 if we weren't.
*/

long ADBList::prev(void)
{
    long    retVal = 0;
    if (totKeys) {
        if (curKeyNo > 0) {
            curKeyNo--;
            retVal   = keyList[curKeyNo];
            get(keyList[retVal]);
        }
    }
    return retVal;
}

/*
** ADBList::next() - Loads the the next row from the database and adjusts 
**                   our pointer.
**
**                   Returns the primary key value for the row if 
**                   successful, or 0 if we weren't.
*/

long ADBList::next(void)
{
    long    retVal = 0;
    if (totKeys) {
        if (curKeyNo < totKeys - 1) {
            curKeyNo++;
            retVal   = keyList[curKeyNo];
            get(keyList[retVal]);
        }
    }
    return retVal;
}



