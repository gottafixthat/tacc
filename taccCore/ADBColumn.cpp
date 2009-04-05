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

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <ADB.h>
#include <mysql/mysql.h>
#ifdef USEDES
#include "bdes.h"
#endif

#include <QtCore/QString>


/*
** ADBColumn::ADBColumn  - The constructor for a query row.
*/

ADBColumn::ADBColumn()
{
    // We have not yet defined our column information, so set everything
    // to blank.
    intData     = (char *) calloc(16, sizeof(char));
    intOldData  = (char *) calloc(16, sizeof(char));
    workStr     = (char *) calloc(16, sizeof(char));
    clear();
    
    debugLevel     = 0;
    intIsEncrypted = 0;
    isPrimaryKey   = 0;
}

/*
** ADBColumn::~ADBColumn  - The destructor for a query row.  Clears out all of 
**                  our memory and cleans itself up.
*/

ADBColumn::~ADBColumn()
{
    ADBDebugMsg(7, "ADBColumn: Freeing column number %d, defined as '%s'", intColumnNo, intColumnName);
    clear();
    free(intData);
    free(intOldData);
    free(workStr);
}

/*
** ADBColumn::clear()   - Clears the entire column, everything, including
**                        its definition.
*/

void ADBColumn::clear()
{
    clearData();
    intColumnName[0] = '\0';
    intTableName[0]  = '\0';
    intDataType      = FIELD_TYPE_NULL;
    intColumnNo      = -1;
    isPrimaryKey     = 0;
    intIsEncrypted   = 0;
    intUseDefKey     = 1;
}

/*
** ADBColumn::clearData()  - Clears only the data for the column.
*/

void ADBColumn::clearData()
{
    free(intData);
    free(intOldData);
    intData          = (char *) calloc(16, sizeof(char));
    intOldData       = (char *) calloc(16, sizeof(char));
}

/*
** ADBColumn::setDebugLevel - Sets our debug level.
*/

void ADBColumn::setDebugLevel(int newDebugLevel)
{
    debugLevel = newDebugLevel;
}


/*
** ADBColumn::loadRow - Loads a row from the database into memory, filling in
**                  the column names and definitions if necessary.
*/

int ADBColumn::define(uint columnNo, MYSQL_FIELD *mField, const char *newData)
{
    int     ret = 0;
    
    if (mField) {
        ADBDebugMsg(7, "ADBColumn::define() clearing currently loaded data for column %d...", columnNo);
        clear();

        // Now, fill in the column name, its type, etc.
        strncpy(intColumnName, mField->name, ADB_MAXCOLWIDTH - 1);
        strncpy(intTableName, mField->table, ADB_MAXCOLWIDTH - 1);

        intDataType     = mField->type;
        intColumnNo     = columnNo;
        isPrimaryKey    = IS_PRI_KEY(mField->flags);
        
        free(intData);
        free(intOldData);
        if (newData) {
            intData = (char *) calloc(strlen(newData)+16, sizeof(char));
            intOldData = (char *) calloc(strlen(newData)+16, sizeof(char));
            strcpy(intData, newData);
            strcpy(intOldData, newData);
        } else {
            intData = (char *) calloc(16, sizeof(char));
            intOldData = (char *) calloc(16, sizeof(char));
        }

        ret = 1;
    }

    return ret; 
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(const char *newData, int setBackupAlso, int isEncrypted, int useDefKey)
{
    int     ret = 1;
    int     newDataLen = 16;
    intUseDefKey = useDefKey;
    free(intOldData);
    intOldData = intData;
    if (newData) newDataLen += strlen(newData);
    intData = (char *) calloc(newDataLen, sizeof(char));
    if (newData) strcpy(intData, newData);
    
    // Was the data passed into us encrypted?  If so, decrypt it before
    // we copy to the backup data.
    if (isEncrypted) {
        decryptData();
    }
    
    // Did the caller request that we set the backup data as well?
    if (setBackupAlso) {
        free(intOldData);
        intOldData = (char *) calloc(newDataLen, sizeof(char));
        if (newData) strcpy(intOldData, newData);
    }
    
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(int newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(32, sizeof(char));
    sprintf(intData, "%d", newValue);
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(long newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%ld", newValue);
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(llong newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%qd", newValue);
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(float newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%f", newValue);
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(double newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%f", newValue);
    return ret;
}

/*
** append - Appends a string value to the current data
*/

int ADBColumn::append(const char *appVal)
{
    int     ret = 1;
    if (intOldData) free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(strlen(intOldData)+strlen(appVal)+64, sizeof(char));
    if (intOldData) strcpy(intData, intOldData);
    strcat(intData, appVal);
    return ret;
}


/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(const QDate newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%04d-%02d-%02d", newValue.year(), newValue.month(), newValue.day());
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(const QTime newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%02d:%02d:%02d", newValue.hour(), newValue.minute(), newValue.second());
    return ret;
}

/*
** set    - Sets a columns data to the passed in value.
*/

int ADBColumn::set(const QDateTime newValue)
{
    int     ret = 1;
    free(intOldData);
    intOldData = intData;
    intData = (char *) calloc(64, sizeof(char));
    sprintf(intData, "%04d%02d%02d%02d%02d%02d", 
      newValue.date().year(), newValue.date().month(), newValue.date().day(),
      newValue.time().hour(), newValue.time().minute(), newValue.time().second());
    return ret;
}


/*
** setColumnName  - Allows us to set the name of the column manually.
*/

int ADBColumn::setColumnName(const char *newName)
{
    ADBDebugMsg(7, "ADBColumn::setColumnName() - changing column name to '%s'", newName);
    int retVal = 1;
    if (newName) strcpy(intColumnName, newName);
    else retVal = 0;
    return retVal;
}

/*
** setColumnNumber - Allows us to set the number of the column manually.
*/

void ADBColumn::setColumnNumber(uint newNum)
{
    ADBDebugMsg(7, "ADBColumn::setColumnNumber() - changing column number to '%d'", newNum);
    intColumnNo = newNum;
}

/*
** setType - Allows us to set the data type of the column manually.
*/

void ADBColumn::setType(enum_field_types newType)
{
    ADBDebugMsg(7, "ADBColumn::setType() - changing column type to '%d'", newType);
    intDataType = newType;
}


/*
** setPrimaryKey - Allows us to set the column's primary key status manually.
*/

void ADBColumn::setPrimaryKey(int newIsPrimary)
{
    ADBDebugMsg(7, "ADBColumn::setPrimaryKey() - changing key status to '%d'", newIsPrimary);
    isPrimaryKey = newIsPrimary;
}



/*
** ColumnName   - Returns the column name as defined by define().
*/

const char *ADBColumn::ColumnName()
{
    return (const char *) intColumnName;
}

/*
** TableName - Returns the table name as defined by define().
*/

const char *ADBColumn::TableName()
{
    return (const char *) intTableName;
}

/*
** DataType - Returns the type of data as defined by define().
*/

enum_field_types ADBColumn::DataType()
{
    return intDataType;
}

/*
** ColumnNo - Returns the column number as defined by define().
*/

uint ADBColumn::ColumnNo()
{
    return intColumnNo;
}

/*
** PrimrayKey - Returns 0/1 if we are not the primary key.
*/

int ADBColumn::PrimaryKey()
{
    return isPrimaryKey;
}

/*
** ColumnChanged  - Tells the caller if Data == intOldData.
*/

int ADBColumn::ColumnChanged()
{
    return strcmp(intData, intOldData);
}

/*
** Data - Returns a reference to the data.
**        This is probably dangerous.
*/

const char *ADBColumn::Data(int useBackup)
{
    if (useBackup) return intOldData;
    else return intData;
}

/*
** setEncrypted  - Tells the column routines whether or not to encrypt/decrypt
**                 data as it goes.
**
**                 Returns true if we were compiled with BDES libraries.
**                 False otherwise.
*/

int ADBColumn::setEncrypted(int isEncrypted, int useDefKey)
{
    int retVal = 0;
    intIsEncrypted = isEncrypted;
    intUseDefKey   = useDefKey;
    #ifdef USEDES
    retVal = 1;
    #endif

    return retVal;
}

/*
** Encrypted - Returns 0/1 if this field is encrypted.
*/

int ADBColumn::Encrypted()
{
    return intIsEncrypted;
}


/*
** toInt()  - Converts data to an integer and returns it.
*/

int ADBColumn::toInt(int useBackup)
{
    if (intDataType != FIELD_TYPE_TINY &&
        intDataType != FIELD_TYPE_SHORT &&
        intDataType != FIELD_TYPE_LONG
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toInt(%d:%s) - Warning! Column is not an integer (type = %d)", intColumnNo, intColumnName, intDataType);

    if (useBackup) return atoi(intOldData);
    else return atoi(intData);
}

/*
** toLong()  - Converts data to a long and returns it.
*/

long ADBColumn::toLong(int useBackup)
{
    if (intDataType != FIELD_TYPE_TINY &&
        intDataType != FIELD_TYPE_SHORT &&
        intDataType != FIELD_TYPE_LONG &&
        intDataType != FIELD_TYPE_LONGLONG &&
        intDataType != FIELD_TYPE_INT24
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toLong(%d) - Warning! Column is not a long", intColumnNo);

    if (useBackup) return atol(intOldData);
    else return atol(intData);
}

/*
** toLLong()  - Converts data to a long long and returns it.
*/

llong ADBColumn::toLLong(int useBackup)
{
    if (intDataType != FIELD_TYPE_TINY &&
        intDataType != FIELD_TYPE_SHORT &&
        intDataType != FIELD_TYPE_LONG &&
        intDataType != FIELD_TYPE_LONGLONG &&
        intDataType != FIELD_TYPE_INT24
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toLLong(%d) - Warning! Column is not a long long", intColumnNo);

    if (useBackup) return atoll(intOldData);
    else return atoll(intData);
}


/*
** toFloat()  - Converts data to a float and returns it.
*/

float ADBColumn::toFloat(int useBackup)
{
    if (intDataType != FIELD_TYPE_FLOAT &&
        intDataType != FIELD_TYPE_DOUBLE
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toFloat(%d:%s) - Warning! Column is not a float (type = %d)", intColumnNo, intColumnName, intDataType);

    if (useBackup) return atof(intOldData);
    else return atof(intData);
}

/*
** toTime_t()  - Converts data to a time_t and returns it.
*/

time_t ADBColumn::toTime_t(int useBackup)
{
    if (intDataType != FIELD_TYPE_DATETIME &&
        intDataType != FIELD_TYPE_TIMESTAMP
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toTime_t(%d) - Warning! Column is not a date", intColumnNo);

    time_t  retVal = 0;
    tm      t;
    char    tmpSt1[256];
    char    tmpSt2[256];

    // This is a nasty, nasty, nasty, nasty way to do this, but my brain 
    // has melted down for the day, and I can't think of a better way
    // to do this right now, but I need the routine before I can continue on
    // with other things.  Brute force, but it will work.

    if (useBackup) { 
        if (intOldData) strcpy(tmpSt1, intOldData);
    } else {
        if (intData) strcpy(tmpSt1, intData);
    }

    if (strlen(tmpSt1) == 14) {
        // Its a time stamp in the form YYYYMMDDHHMMSS
        if (!strcmp("00000000000000", tmpSt1)) {
            retVal = (time_t) 0;
        } else {
            // Oh, god, this is ugly.
            sprintf(tmpSt2, "%c%c%c%c", tmpSt1[0], tmpSt1[1], tmpSt1[2], tmpSt1[3]);
            t.tm_year = atoi(tmpSt2) - 1900;

            sprintf(tmpSt2, "%c%c", tmpSt1[4], tmpSt1[5]);
            t.tm_mon = atoi(tmpSt2) - 1;

            sprintf(tmpSt2, "%c%c", tmpSt1[6], tmpSt1[7]);
            t.tm_mday = atoi(tmpSt2);

            // Oh, nasty, nasty, nasty, I must kill myself before letting 
            // anyone see this code.

            sprintf(tmpSt2, "%c%c", tmpSt1[8], tmpSt1[9]);
            t.tm_hour = atoi(tmpSt2);

            sprintf(tmpSt2, "%c%c", tmpSt1[10], tmpSt1[11]);
            t.tm_min = atoi(tmpSt2);

            sprintf(tmpSt2, "%c%c", tmpSt1[12], tmpSt1[13]);
            t.tm_sec = atoi(tmpSt2);

            retVal = mktime(&t);
        }
    } else {
        // Its a datetime in the form YYYY-MM-DD HH:MM:SS
        // I should really be ashamed to call myself a programmer for
        // writing shit this bad.
        if (!strcmp("0000-00-00 00:00:00", tmpSt1)) {
            retVal = (time_t) 0;
        } else {
            sprintf(tmpSt2, "%c%c%c%c", tmpSt1[0], tmpSt1[1], tmpSt1[2], tmpSt1[3]);
            t.tm_year = atoi(tmpSt2) - 1900;

            sprintf(tmpSt2, "%c%c", tmpSt1[5], tmpSt1[6]);
            t.tm_mon = atoi(tmpSt2) - 1;

            sprintf(tmpSt2, "%c%c", tmpSt1[8], tmpSt1[9]);
            t.tm_mday = atoi(tmpSt2);

            // Oh, this is bad.
            sprintf(tmpSt2, "%c%c", tmpSt1[11], tmpSt1[12]);
            t.tm_hour = atoi(tmpSt2);

            sprintf(tmpSt2, "%c%c", tmpSt1[14], tmpSt1[15]);
            t.tm_min = atoi(tmpSt2);

            sprintf(tmpSt2, "%c%c", tmpSt1[17], tmpSt1[18]);
            t.tm_sec = atoi(tmpSt2);

            retVal = mktime(&t);
        }
    }

    
    return retVal;
}


/*
** toQDate()  - Converts data to a QDate and returns it.
*/

const QDate ADBColumn::toQDate(int useBackup)
{
    if (intDataType != FIELD_TYPE_DATE &&
        intDataType != FIELD_TYPE_DATETIME &&
        intDataType != FIELD_TYPE_TIMESTAMP
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toQDate(%d) - Warning! Column is not a date", intColumnNo);

    QDate   retVal;
    QString tmpQStr;

    if (useBackup) tmpQStr = intOldData;
    else tmpQStr = intData;

    retVal.setYMD(
      tmpQStr.mid(0,4).toInt(),
      tmpQStr.mid(5,2).toInt(),
      tmpQStr.mid(8,2).toInt()
    );
    
    return (const QDate) retVal;
}

/*
** toQTime()  - Converts data to a QTime and returns it.
*/

const QTime ADBColumn::toQTime(int useBackup)
{
    if (intDataType != FIELD_TYPE_TIME
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toQDate(%d) - Warning! Column is not a date", intColumnNo);

    QTime   retVal;
    QString tmpQStr;

    if (useBackup) tmpQStr = intOldData;
    else tmpQStr = intData;

    retVal.setHMS(
      tmpQStr.mid(0,2).toInt(),
      tmpQStr.mid(3,2).toInt(),
      tmpQStr.mid(6,2).toInt()
    );
    
    return (const QTime) retVal;
}

/*
** toQDateTime()  - Converts data to a QDateTime and returns it.
*/

const QDateTime ADBColumn::toQDateTime(int useBackup)
{
    if (intDataType != FIELD_TYPE_DATETIME &&
        intDataType != FIELD_TYPE_TIMESTAMP
       ) ADBLogMsg(LOG_WARNING, "ADBColumn::toQDate(%d) - Warning! Column is not a date", intColumnNo);

    QDateTime retVal;
    QDate     tmpDate;
    QTime     tmpTime;
    QString   tmpQStr;

    if (useBackup) tmpQStr = intOldData;
    else tmpQStr = intData;

    if (tmpQStr.length() == 14) {
        // Its a time stamp in the form YYYYMMDDHHMMSS
        tmpDate.setYMD(
          tmpQStr.mid(0,4).toInt(),
          tmpQStr.mid(4,2).toInt(),
          tmpQStr.mid(6,2).toInt()
        );
        tmpTime.setHMS(
          tmpQStr.mid( 8,2).toInt(),
          tmpQStr.mid(10,2).toInt(),
          tmpQStr.mid(12,2).toInt()
        );
    } else {
        // Its a datetime in the form YYYY-MM-DD HH:MM:SS
        tmpDate.setYMD(
          tmpQStr.mid(0,4).toInt(),
          tmpQStr.mid(5,2).toInt(),
          tmpQStr.mid(8,2).toInt()
        );
        tmpTime.setHMS(
          tmpQStr.mid(11,2).toInt(),
          tmpQStr.mid(14,2).toInt(),
          tmpQStr.mid(17,2).toInt()
        );
    }
    retVal.setDate(tmpDate);
    retVal.setTime(tmpTime);
    
    return (const QDateTime) retVal;
}


/*
** insStr   - Creates an insert string suitable for use in an insert or
**            update query, including any necessary quotes.
*/

const char *ADBColumn::insStr()
{
    if (workStr) free(workStr);
    switch (intDataType) {
        case FIELD_TYPE_TINY:
        case FIELD_TYPE_SHORT:
            workStr = (char *) calloc(32, sizeof(char));
            sprintf(workStr, "%d", atoi(intData));
        break;
        
        case FIELD_TYPE_LONG:
            workStr = (char *) calloc(32, sizeof(char));
            sprintf(workStr, "%ld", atol(intData));
        break;
        
        case FIELD_TYPE_LONGLONG:
            workStr = (char *) calloc(64, sizeof(char));
            sprintf(workStr, "%qd", atoll(intData));
        break;

        case FIELD_TYPE_DOUBLE:
        case FIELD_TYPE_FLOAT:
            workStr = (char *) calloc(64, sizeof(char));
            sprintf(workStr, "%f", atof(intData));
        break;

        // Everything else is a string, so escape it and wrap it in quotes.
        default:
            if (intIsEncrypted) {
                workStr = (char *) calloc(16, sizeof(char));
                encryptData();
                int  tmpLen  = strlen(workStr);
                char *tmpStr = (char *) calloc(tmpLen*2+32, sizeof(char));
                mysql_escape_string(tmpStr, workStr, tmpLen);
                free(workStr);
                workStr = (char *) calloc(strlen(tmpStr)+64, sizeof(char));
                strcpy(workStr, "'");
                strcat(workStr, tmpStr);
                strcat(workStr, "'");
                free(tmpStr);
            } else {
                int tmpLen = 0;
                if (intData) tmpLen = strlen(intData);
                workStr = (char *) calloc(tmpLen * 2 + 16, sizeof(char));
                char *tmpStr = (char *) calloc(tmpLen * 2 + 16, sizeof(char));
                if (intData) mysql_escape_string(tmpStr, intData, tmpLen);
                strcpy(workStr, "'");
                strcat(workStr, tmpStr);
                strcat(workStr, "'");
                free(tmpStr);
            }
        break;
    }
    return workStr;
}

/*
** decryptData()   - Decrypts the stuff in intData.
*/

void ADBColumn::decryptData(void)
{
    #ifdef USEDES
    ADBDebugMsg(7, "ADBColumn: Decrypting source string '%s'", intData);
    char    *dst = (char *) calloc(strlen(intData)+128, sizeof(char));
    decrypt_string((unsigned char *)intData, (unsigned char *) dst, intUseDefKey);
    free (intData);
    intData = dst;
    ADBDebugMsg(7, "ADBColumn: Decrypted string '%s'", intData);
    #else
    ADBLogMsg(LOG_ERR, "ADBColumn::decryptData() - This version of the ADB libraries does not support encryption.");
    #endif
}


/*
** encryptData()   - Encrypts intData and stores it in workStr.
*/

void ADBColumn::encryptData(void)
{
    #ifdef USEDES
    free(workStr);
    ADBDebugMsg(7, "ADBColumn: Encrypting source string '%s'", intData);
    workStr = (char *) calloc((strlen(intData)*4)+128, sizeof(char));
    encrypt_string((unsigned char *)intData, (unsigned char *) workStr, intUseDefKey);
    ADBDebugMsg(7, "ADBColumn: Encrypted string into '%s'", workStr);
    #else
    ADBLogMsg(LOG_ERR, "ADBColumn::encryptData() - This version of the ADB libraries does not support encryption.");
    #endif
}



// vim: expandtab
