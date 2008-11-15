/*
** $Id: ADBRow.cpp,v 1.5 2002/09/29 18:43:47 marc Exp $
**
**  ADBRow.cpp - Row definitions used by other ADB Classes.
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
** $Log: ADBRow.cpp,v $
** Revision 1.5  2002/09/29 18:43:47  marc
** *** empty log message ***
**
** Revision 1.4  2000/05/02 20:12:49  marc
** Added the option to return zero dates as empty strings instead of '0000-00-00'.
**
** Revision 1.3  1999/12/17 18:04:10  marc
** Added the ability for ADB to log to stderr as well as syslog (the default).
**
** Revision 1.2  1999/10/27 00:55:41  marc
** Added some comments to the headers.
** Added appendStr and getLLong to ADBTable.
**
**
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ADB.h>
#include <mysql/mysql.h>
#ifdef USEDES
#include "bdes.h"
#endif


/*
** ADBRow::ADBRow  - The constructor for a query row.
*/

ADBRow::ADBRow()
{
    // We have not yet defined our row definitions.
    intRowDefined   = 0;
    numFields       = 0;
    debugLevel      = 0;
    zeroDatesAsNULL = false;
}

/*
** ADBRow::~ADBRow  - The destructor for a query row.  Clears out all of 
**                  our memory and cleans itself up.
*/

ADBRow::~ADBRow()
{
    clearRow();
}

/*
** ADBRow::setDebugLevel - Sets our debug level.
*/

void ADBRow::setDebugLevel(int newDebugLevel)
{
    debugLevel = newDebugLevel;
}

/*
** ADBRow::setZeroDatesAsNULL - Determines how we return empty dates.
*/

void ADBRow::setZeroDatesAsNULL(bool newValue)
{
    zeroDatesAsNULL = newValue;
}

/*
** ADBRow::clearRow() - Clears all of the variables we currently have defined
**                  in preparation for the retrieval of a new row.
*/

void ADBRow::clearRow()
{
    if (intRowDefined && numFields) {
        for (uint i = 0; i < numFields; i++) delete columns[i];
    }
    numFields       = 0;
    intRowDefined   = 0;

}

/*
** ADBRow::loadRow - Loads a row from the database into memory, filling in
**                  the column names and definitions if necessary.
*/

int ADBRow::loadRow(MYSQL_RES *queryRes)
{
    int             ret = 0;
    MYSQL_ROW       rawRow;
    unsigned long   *rowLengths;
    MYSQL_FIELD     *curField;

    ADBDebugMsg(7, "ADBRow::loadRow clearing currently loaded data...");
    // clearRow();

    ADBDebugMsg(7, "ADBRow::loadRow determining field count...");
    numFields = mysql_num_fields(queryRes);
    
    // Now that we have the number of fields, we can proceed.
    if (numFields > 0) {
        rawRow = mysql_fetch_row(queryRes);
        if (rawRow) {
            rowLengths = (unsigned long *) mysql_fetch_lengths(queryRes);
            ADBDebugMsg(7, "ADBRow::loadRow fethcing %d fields...", numFields);
            mysql_field_seek(queryRes, 0);
            for (uint i = 0; i < numFields; i++) {
                // Instantiate the column class for this column
                if (!intRowDefined) columns[i] = new ADBColumn();

                // Get the field by number
                curField = mysql_fetch_field(queryRes);

                // Now, fill in the column name, its type, etc.
                columns[i]->setDebugLevel(debugLevel);
                columns[i]->define(i, curField, rawRow[i]);
            }
            intRowDefined = 1;
            ret = 1;
        }
    }
    
    if (debugLevel > 7 && intRowDefined && ret) {
        ADBDebugMsg(7, "ADBRow:  Loaded row with the following structure");
        for (uint i = 0; i < numFields ; i++) {
            ADBDebugMsg(7, "   Column[%d]->Name  = '%s'", i, columns[i]->ColumnName());
            ADBDebugMsg(7, "   Column[%d]->Table = '%s'", i, columns[i]->TableName());
            ADBDebugMsg(7, "   Column[%d]->Data  = '%s'", i, columns[i]->Data());
            ADBDebugMsg(7, "   Column[%d]->Type  = '%d'", i, columns[i]->DataType());
        }
    }

    return ret; 
}

/*
** numColumns   - Returns the number of columns that we have loaded.
*/

uint ADBRow::numColumns()
{
    return numFields;
}

/*
** ADBRow::[unsigned int colNo] - Returns a column by row.
*/

const char *ADBRow::operator[](unsigned int colNo)
{
    const char  *retVal;
    ADBColumn   *tmpCol;

    tmpCol = findColumn(colNo);
    if (tmpCol) {
        retVal = tmpCol->Data();
        // Check for empty dates
        if (zeroDatesAsNULL && tmpCol->DataType() == FIELD_TYPE_DATE) {
            if (!strcmp("0000-00-00", tmpCol->Data())) {
                retVal = "";
            }
        }
    } else {
        retVal = NULL;
    }

    return retVal;
}

/*
** ADBRow::[int colNo] - Returns a column by row.
*/

const char *ADBRow::operator[](int colNo)
{
    const char  *retVal;
    ADBColumn   *tmpCol;

    tmpCol = findColumn((unsigned int) colNo);
    if (tmpCol) {
        retVal = tmpCol->Data();
        // Check for empty dates
        if (zeroDatesAsNULL && tmpCol->DataType() == FIELD_TYPE_DATE) {
            if (!strcmp("0000-00-00", tmpCol->Data())) {
                retVal = "";
            }
        }
    } else {
        retVal = NULL;
    }

    return retVal;
}

/*
** ADBRow::[const char *colName] - Returns a column by name.
*/

const char *ADBRow::operator[](const char *colName)
{
    const char  *retVal;
    ADBColumn   *tmpCol;
    tmpCol = findColumn(colName);
    if (tmpCol) {
        retVal = tmpCol->Data();
        // Check for empty dates
        if (zeroDatesAsNULL && tmpCol->DataType() == FIELD_TYPE_DATE) {
            if (!strcmp("0000-00-00", tmpCol->Data())) {
                retVal = "";
            }
        }
    } else {
        retVal = NULL;
    }
    return retVal;
}

/*
** ADBRow::col    -  Returns a pointer to the requested column.
*/

ADBColumn *ADBRow::col(unsigned int colNo)
{
    return findColumn(colNo);
}

/*
** ADBRow::col    -  Returns a pointer to the requested column.
*/

ADBColumn *ADBRow::col(int colNo)
{
    return findColumn((unsigned int) colNo);
}

/*
** ADBRow::col    -  Returns a pointer to the requested column.
*/

ADBColumn *ADBRow::col(const char *colName)
{
    return findColumn(colName);
}

/*
** primaryKey   - Returns the column defined as the primary key or NULL if
**                there is no primary key defined.
*/

ADBColumn *ADBRow::primaryKey()
{
    ADBColumn    *retVal = NULL;
    
    if (intRowDefined && numFields) {
        for (uint i = 0; i < numFields; i++) {
            if (columns[i]->PrimaryKey()) {
                // We have a winner.
                retVal = columns[i];
            }
        }
    }
    
    return retVal;
}

/*
** findColumn   - Gets a column by number.
*/

ADBColumn *ADBRow::findColumn(unsigned int colNo)
{
    ADBColumn   *retVal = NULL;
    if (intRowDefined && numFields && (colNo <= numFields)) {
        retVal = columns[colNo];
    }
    return retVal;
}

/*
** findColumn   - Gets a column by name.
*/

ADBColumn *ADBRow::findColumn(const char *colName)
{
    ADBColumn   *retVal = NULL;
    if (intRowDefined && numFields) {
        for (uint i = 0; i < numFields; i++) {
            if (!strcmp(colName, columns[i]->ColumnName())) {
                retVal = columns[i];
            }
        }
    }
    return retVal;
}





