/*
** $Id: ADBTable.cpp,v 1.7 2002/09/29 18:43:47 marc Exp $
**
**  ADBTable.cpp - Class definitions for working with entire rows within a 
**                 table, including insertions, smart updates and deletions.
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
** $Log: ADBTable.cpp,v $
** Revision 1.7  2002/09/29 18:43:47  marc
** *** empty log message ***
**
** Revision 1.6  2000/06/12 21:48:07  marc
** Added a new class - ADBLogin.  This is a generic login dialog that can
** be used in any Qt based program that uses ADB for database access.
** Altered the defines for using the Qt exentions to ADB.  To use them, the
** user must now define ADBQT=1 in their Makefiles.
**
** Revision 1.5  2000/05/02 20:12:49  marc
** Added the option to return zero dates as empty strings instead of '0000-00-00'.
**
** Revision 1.4  1999/12/17 18:04:10  marc
** Added the ability for ADB to log to stderr as well as syslog (the default).
**
** Revision 1.3  1999/10/27 00:55:41  marc
** Added some comments to the headers.
** Added appendStr and getLLong to ADBTable.
**
**
*/


#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <ADB.h>
#ifdef USEDES
#include "bdes.h"
#endif




ADBTable::ADBTable(
  const char *Table,
  const char *Name,
  const char *User,
  const char *Pass,
  const char *Host
) : ADB (Name, User, Pass, Host)
{
    strcpy(TableName, "UNDEFINED");

    numColumns = 0;
    primaryKeyColumn = ADB_MAXCOLS + 1;

    if (Table && strlen(Table)) {
        setTableName(Table);
    }
    
}

ADBTable::~ADBTable()
{
    if (numColumns) {
        for (uint i = 0; i < numColumns; i++) {
            delete columnDefs[i];
        }
        numColumns = 0;
    }
}

/*
** ADBTable::setTableName - Sets the table name and loads the definitions for
**                          the table.
**
** This one is a bit annoying compared to the DBRow class.  To get the names
** of the columns we actually have to parse the query and do things manually.
*/

void ADBTable::setTableName(const char * tabName)
{
    query("SHOW COLUMNS FROM %s", tabName);
    numColumns = 0;
    enum_field_types    tmpType;
    while (getrow()) {
        ADBDebugMsg(7, "ADBTable::setTableName column 0 = '%s'", (const char *) curRow[0]);
        columnDefs[numColumns] = new ADBColumn();
        columnDefs[numColumns]->setDebugLevel(debugLevel);
        columnDefs[numColumns]->setColumnNumber(numColumns);
        columnDefs[numColumns]->setColumnName(curRow["Field"]);
        
        // Now, we have some fun.  We need to parse column 1, which is our
        // Type, this is ugly and I hate it, but it seems there is no other
        // way to get the column information from the MySQL sever without
        // actually loading a row, which won't work if the table is empty.
        // So, we parse.
        ADBDebugMsg(7, "ADBTable::setTableName - determining type ('%s')", curRow[1]);
        tmpType = FIELD_TYPE_NULL;
        if (!strncasecmp(curRow[1], "int",        strlen("int")))        tmpType = FIELD_TYPE_SHORT;
        if (!strncasecmp(curRow[1], "bigint",     strlen("bigint")))     tmpType = FIELD_TYPE_LONGLONG;
        if (!strncasecmp(curRow[1], "char",       strlen("char")))       tmpType = FIELD_TYPE_STRING;
        if (!strncasecmp(curRow[1], "varchar",    strlen("varchar")))    tmpType = FIELD_TYPE_VAR_STRING;
        if (!strncasecmp(curRow[1], "float",      strlen("float")))      tmpType = FIELD_TYPE_FLOAT;
        if (!strncasecmp(curRow[1], "blob",       strlen("blob")))       tmpType = FIELD_TYPE_BLOB;
        if (!strncasecmp(curRow[1], "tinyblob",   strlen("tinyblob")))   tmpType = FIELD_TYPE_TINY_BLOB;
        if (!strncasecmp(curRow[1], "mediumblob", strlen("mediumblob"))) tmpType = FIELD_TYPE_MEDIUM_BLOB;
        if (!strncasecmp(curRow[1], "longblob",   strlen("longblob")))   tmpType = FIELD_TYPE_LONG_BLOB;
        if (!strncasecmp(curRow[1], "year",       strlen("year")))       tmpType = FIELD_TYPE_YEAR;
        if (!strncasecmp(curRow[1], "date",       strlen("date")))       tmpType = FIELD_TYPE_DATE;
        if (!strncasecmp(curRow[1], "time",       strlen("time")))       tmpType = FIELD_TYPE_TIME;
        if (!strncasecmp(curRow[1], "datetime",   strlen("datetime")))   tmpType = FIELD_TYPE_DATETIME;
        if (!strncasecmp(curRow[1], "timestamp",  strlen("timestamp")))  tmpType = FIELD_TYPE_TIMESTAMP;
        if (!strncasecmp(curRow[1], "set",        strlen("set")))        tmpType = FIELD_TYPE_SET;
        if (!strncasecmp(curRow[1], "enum",       strlen("enum")))       tmpType = FIELD_TYPE_ENUM;

        columnDefs[numColumns]->setType(tmpType);
        
        // Now, check for the primary key.  It is column 3.
        if (!strcasecmp(curRow[3], "PRI")) {
            columnDefs[numColumns]->setPrimaryKey(1);
            primaryKeyColumn = numColumns;
        }


        numColumns++;
    }
    
    if (numColumns) strcpy(TableName, tabName);
    
    // Spit out a warning if there is no primary key.
    if (primaryKeyColumn > numColumns) {
        ADBLogMsg(LOG_WARNING, "ADBTable::setTableName() - No primary key defined for table '%s'!", tabName);
    }
}

/*
** ADBTable::clearData - Clears the data values for everything loaded.
*/

void ADBTable::clearData(void)
{
    curRow.clearRow();
    if (numColumns) {
        for (uint i = 0; i < numColumns; i++) {
            columnDefs[i]->clearData();
        }
    }
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, int val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, int val)
{
    return setValue(getColumnNumber(colName), val);
}


/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, long val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, long val)
{
    return setValue(getColumnNumber(colName), val);
}


/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, llong val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, llong val)
{
    return setValue(getColumnNumber(colName), val);
}


/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, float val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, float val)
{
    return setValue(getColumnNumber(colName), val);
}


/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, double val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, double val)
{
    return setValue(getColumnNumber(colName), val);
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, const char *val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, const char *val)
{
    return setValue(getColumnNumber(colName), val);
}

#ifdef ADBQT
/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, const QString val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val.toAscii().data());
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, const QString val)
{
    return setValue(getColumnNumber(colName), val);
}

#endif

/*
** ADBTable::appendStr() - Appends the string to the specified column.
*/

int ADBTable::appendStr(uint colNo, const char *val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->append(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::appendStr() - Appends the string to the specified column name.
*/

int ADBTable::appendStr(const char *colName, const char *val)
{
    return appendStr(getColumnNumber(colName), val);
}


#ifdef ADBQT

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, const QDate val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, const QDate val)
{
    return setValue(getColumnNumber(colName), val);
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, const QTime val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, const QTime val)
{
    return setValue(getColumnNumber(colName), val);
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(uint colNo, const QDateTime val)
{
    int retVal = 0;
    if (colNo < numColumns) {
        columnDefs[colNo]->set(val);
        retVal = 1;
    }
    return retVal;
}

/*
** ADBTable::setValue()   - Sets the column data to the passed in value.
*/

int ADBTable::setValue(const char *colName, const QDateTime val)
{
    return setValue(getColumnNumber(colName), val);
}

#endif   // ADBQT


/*
** ADBTable::getInt() - Gets the column data and returns an int.
*/

int ADBTable::getInt(uint colNo, int useBackup)
{
    int retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toInt(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getInt()  - Gets the column data and returns it as an int.
*/

int ADBTable::getInt(const char *colName, int useBackup)
{
    return getInt(getColumnNumber(colName), useBackup);
}


/*
** ADBTable::getLong() - Gets the column data and returns a long.
*/

long ADBTable::getLong(uint colNo, int useBackup)
{
    long retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toLong(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getLong()  - Gets the column data and returns it as an int.
*/

long ADBTable::getLong(const char *colName, int useBackup)
{
    return getLong(getColumnNumber(colName), useBackup);
}

/*
** ADBTable::getLLong() - Gets the column data and returns an llong.
*/

llong ADBTable::getLLong(uint colNo, int useBackup)
{
    llong retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toLLong(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getLLong()  - Gets the column data and returns an llong
*/

llong ADBTable::getLLong(const char *colName, int useBackup)
{
    return getLLong(getColumnNumber(colName), useBackup);
}


/*
** ADBTable::getFloat() - Gets the column data and returns a float.
*/

float ADBTable::getFloat(uint colNo, int useBackup)
{
    float retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toFloat(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getFloat()  - Gets the column data and returns it as a float.
*/

float ADBTable::getFloat(const char *colName, int useBackup)
{
    return getFloat(getColumnNumber(colName), useBackup);
}

/*
** ADBTable::getStr() - Gets the column data and returns it.
*/

const char *ADBTable::getStr(uint colNo, int useBackup)
{
    const char *retVal = NULL;
    if (colNo < numColumns) {
        return columnDefs[colNo]->Data(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getStr()  - Gets the column data and returns it.
*/

const char *ADBTable::getStr(const char *colName, int useBackup)
{
    return getStr(getColumnNumber(colName), useBackup);
}

/*
** ADBTable::getTime_t() - Gets the column data and returns a time_t.
*/

time_t ADBTable::getTime_t(uint colNo, int useBackup)
{
    time_t retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toTime_t(useBackup);
    }
    return retVal;
}

/*
** ADBTable::getTime_t()  - Gets the column data and returns it as a time_t.
*/

time_t ADBTable::getTime_t(const char *colName, int useBackup)
{
    return getTime_t(getColumnNumber(colName), useBackup);
}

#ifdef ADBQT

/*
** ADBTable::getDate() - Gets the column data and returns a QDate.
*/

const QDate ADBTable::getDate(uint colNo, int useBackup)
{
    QDate retVal;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toQDate(useBackup);
    }
    return (const QDate) retVal;
}

/*
** ADBTable::getDate()  - Gets the column data and returns it as a float.
*/

const QDate ADBTable::getDate(const char *colName, int useBackup)
{
    return getDate(getColumnNumber(colName), useBackup);
}

/*
** ADBTable::getTime() - Gets the column data and returns a QTime.
*/

const QTime ADBTable::getTime(uint colNo, int useBackup)
{
    QTime retVal;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toQTime(useBackup);
    }
    return (const QTime) retVal;
}

/*
** ADBTable::getTime()  - Gets the column data and returns it as a QTime.
*/

const QTime ADBTable::getTime(const char *colName, int useBackup)
{
    return getTime(getColumnNumber(colName), useBackup);
}

/*
** ADBTable::getDateTime() - Gets the column data and returns a QDateTime.
*/

const QDateTime ADBTable::getDateTime(uint colNo, int useBackup)
{
    QDateTime retVal;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->toQDateTime(useBackup);
    }
    return (const QDateTime) retVal;
}

/*
** ADBTable::getDateTime()  - Gets the column data and returns it as a QDateTime.
*/

const QDateTime ADBTable::getDateTime(const char *colName, int useBackup)
{
    return getDateTime(getColumnNumber(colName), useBackup);
}


#endif  // NOADBQT

/*
** get   - Gets a data row based on a long key.
*/

long  ADBTable::get(long keyVal)
{
    long    retVal = 0;
    
    if (primaryKeyColumn < numColumns) {
        query("SELECT * FROM %s where %s = %ld", 
          TableName,  
          columnDefs[primaryKeyColumn]->ColumnName(),
          keyVal
        );
        if (rowCount) {
            getrow();
            // Now, copy the row contents into our internal values.
            for (uint i = 0; i < numColumns; i++) {
                columnDefs[i]->clearData();
                columnDefs[i]->set(curRow[i], 1, columnDefs[i]->Encrypted());
            }
            retVal = keyVal;
        }

    } else {
        ADBLogMsg(LOG_WARNING, "ADBTable::get(long) - No primary key defined for table '%s'", TableName);
    }
    
    return retVal;
}


/*
** get   - Gets a data row based on an int key.
*/

int ADBTable::get(int keyVal)
{
    int retVal = 0;
    
    if (primaryKeyColumn < numColumns) {
        query("SELECT * FROM %s where %s = %d", 
          TableName,  
          columnDefs[primaryKeyColumn]->ColumnName(),
          keyVal
        );
        if (rowCount) {
            getrow();
            // Now, copy the row contents into our internal values.
            for (uint i = 0; i < numColumns; i++) {
                columnDefs[i]->clearData();
                columnDefs[i]->set(curRow[i], 1);
            }
            retVal = keyVal;
        }

    } else {
        ADBLogMsg(LOG_WARNING, "ADBTable::get(int) - No primary key defined for table '%s'\n", TableName);
    }
    
    return retVal;
}

/*
** ins   - Takes the current column and inserts it.  After inserting,
**         it performs a get() on the data.
**
**         It returns the new primary key value if successful.
*/

long ADBTable::ins(int autoGet)
{
    long        retVal = 0;
    ADBDebugMsg(7, "ADBTable::ins() Generating insert string...");
    if (numColumns) {
        // Create an initial buffer to work with.
        uint       sSize = 4096;
        char       *insStr = (char *) calloc(sSize + ADB_MAXCOLWIDTH + 32, sizeof(char));
        const char *tmpStr;
        uint       tmpLen = 0;
        
        sprintf(insStr, "INSERT INTO %s VALUES (", TableName);
        
        // Loop through all of our columns and get their values.
        for (uint i = 0; i < numColumns; i++) {
            tmpStr = columnDefs[i]->insStr();
            tmpLen = strlen(tmpStr);
            // Check to see if we need to grow our insert string.
            if (tmpLen + strlen(insStr) > sSize) {
                ADBDebugMsg(7, "ADBTable::ins() - growing buffer...");
                // We need to grow it.
                char *tmpStr2 = (char *) calloc(sSize + 4096 + ADB_MAXCOLWIDTH + 32 + tmpLen, sizeof(char));
                sSize += 4096 + tmpLen;
                strcpy(tmpStr2, insStr);
                free(insStr);
                insStr = tmpStr2;
                ADBDebugMsg(7, "ADBTable::ins() - buffer is now %d bytes long....", sSize);
            }
            strcat(insStr, tmpStr);

            // If we're not at the last column, add a field seperator
            if (i < numColumns - 1) strcat(insStr, ",");
        }
        
        // Append the closing paren to complete the command.
        strcat(insStr, ")");

        // Insert the row and get the primary key value back.
        retVal = dbcmd("%s", insStr);
        
        // free our work string
        free(insStr);

        // If requested, re-load the row so both row references are complete.
        if (autoGet) get(retVal);
        
        // Now, call the virtual post-insert routine.
        postIns();
    }
    return retVal;
}

/*
** upd   - Takes the currently loaded row and creates an update query for it
**         using only the modified columns.  If no columns have been modified,
**         this function does nothing.  After updating, it performs an
**         optional get() on the data.
**
**         It returns the primary key value if successful.
*/

long ADBTable::upd(int autoGet)
{
    long        retVal = 0;
    ADBDebugMsg(7, "ADBTable::upd() Generating update string...");
    if (numColumns) {
        // Create an initial buffer to work with.
        uint       sSize = 4096;
        char       *updStr = (char *) calloc(sSize + ADB_MAXCOLWIDTH + 32, sizeof(char));
        const char *tmpStr;
        uint       tmpLen = 0;
        int        changedCols = 0;
        
        sprintf(updStr, "UPDATE %s SET ", TableName);
        
        // Loop through all of our columns and get their values.
        for (uint i = 0; i < numColumns; i++) {
            // Only do something if the column data has changed.
            if (columnDefs[i]->ColumnChanged()) {
                changedCols++;
                tmpStr = columnDefs[i]->insStr();
                tmpLen = strlen(tmpStr);
                // Check to see if we need to grow our insert string.
                if (tmpLen + strlen(updStr) > sSize) {
                    ADBDebugMsg(7, "ADBTable::upd() - growing buffer...");
                    // We need to grow it.
                    char *tmpStr2 = (char *) calloc(sSize + 4096 + ADB_MAXCOLWIDTH + 32 + tmpLen, sizeof(char));
                    sSize += 4096 + tmpLen;
                    strcpy(tmpStr2, updStr);
                    free(updStr);
                    updStr = tmpStr2;
                    ADBDebugMsg(7, "ADBTable::ins() - buffer is now %d bytes long....", sSize);
                }
                if (changedCols > 1) strcat(updStr, ", ");
                strcat(updStr, columnDefs[i]->ColumnName());
                strcat(updStr, " = ");
                strcat(updStr, tmpStr);
            }
        }

        // Update the row and get the primary key value back.
        if (changedCols) {
            strcat(updStr, " where ");
            strcat(updStr, columnDefs[primaryKeyColumn]->ColumnName());
            strcat(updStr, " = ");
            strcat(updStr, columnDefs[primaryKeyColumn]->insStr());
            retVal = dbcmd("%s", updStr);
            
            if (!retVal) {
                // Success.
                if (autoGet) retVal = get(columnDefs[primaryKeyColumn]->toLong());
                else retVal = columnDefs[primaryKeyColumn]->toLong();
            }

            // Now, call the virtual post-insert routine.
            postUpd();
        }
        
        // free our work string
        free(updStr);
    }
    return retVal;
}

/*
** del   - Deletes the row with the specified key.  If no key is specified
**         it deletes the currently loaded row.
**
**         It returns 1 if successful.
*/

int ADBTable::del(long keyVal)
{
    int     retVal  = 0;
    long    pKeyVal = keyVal;
    char    *delStr = (char *) calloc(4096, sizeof(char));
    
    // Check to see if we were given a key value.
    if (!pKeyVal) {
        // We were not.  Check to see if we have a row loaded.
        if (numColumns) {
            // So far so good.  Assign the key value.
            pKeyVal = columnDefs[primaryKeyColumn]->toLong();
        }
    }
    
    if (pKeyVal) {
        sprintf(delStr, "DELETE FROM %s WHERE %s = %ld",
          TableName,
          columnDefs[primaryKeyColumn]->ColumnName(),
          pKeyVal
        );
        if (!dbcmd("%s", delStr)) {
            // postDel();
            retVal = 1;
        }
        postDel();
        
        // Was the key loaded the one we just deleted?  If so
        // clear our data.
        if (pKeyVal == columnDefs[primaryKeyColumn]->toLong()) {
            clearData();
        }
    }
    
    free(delStr);
    return retVal;
} 

/*
** ADBTable::setEncryptedColumn() - Tells the ADBColumn that this column
**                                  is stored in encrypted form.
*/

int ADBTable::setEncryptedColumn(uint colNo, int useDefKey)
{
    int retVal = 0;
    if (colNo < numColumns) {
        retVal = columnDefs[colNo]->setEncrypted(1, useDefKey);
    }
    return retVal;
}

/*
** ADBTable::getInt()  - Gets the column data and returns it as an int.
*/

int ADBTable::setEncryptedColumn(const char *colName, int useDefKey)
{
    return setEncryptedColumn(getColumnNumber(colName), useDefKey);
}


/*
** getColumnNumber  - Turns a column name into a column number.
**                    If it is unable to find the column name, it returns
**                    ADB_MAXCOLS.
*/

uint ADBTable::getColumnNumber(const char *colName)
{
    uint    retVal = ADB_MAXCOLS;
    if (numColumns) for (uint i = 0; i < numColumns; i++) {
        if (!strcmp(columnDefs[i]->ColumnName(), colName)) {
            // Found it.  Short circuit our loop and exit.
            retVal = i;
            i = ADB_MAXCOLS;
        }
    }
    return retVal;
}


