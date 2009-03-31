/*
** $Id: ADB.h,v 1.21 2002/09/29 18:43:47 marc Exp $
**
**  ADB.h    - Object Database Routines.
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
** The ADB classes comprise three classes which create simple access methods
** for one or more MySQL databases.  The databases can be local or remote.
** The Blarg! triple-DES libraries are supported for data protection if
** they are defined and compiled in.
**
** The three classes are:  ADB, ADBTable and ADBList.
**
**    The ADB class provides simple and direct access to a database, 
**    supporting both queries and commands.
**
**    The ADBTable class provides access to a single table within a class.
**    When given the Table name in a database, it automatically determines
**    all data types and the primary key for the table (provided it is a
**    single column).  All operations perform on a single row of the table
**    after a get() or ins() command has been issued.
**
**    ADBList is derived from the ADBTable class and can be used to work
**    with a query that returns multiple rows (such as reports).  Access
**    to individual rows is supported through the inherited ADBTable class.
**
**
** Requres:  - Blarg DES libraries are required if encrypted columns are 
**             to be supported.  Available from Blarg! Online Services, Inc.
**           - MySQL libraries avalable from TCX DataKonsult AB
**
*****************************************************************************
**
** $Log: ADB.h,v $
** Revision 1.21  2002/09/29 18:43:47  marc
** *** empty log message ***
**
** Revision 1.20  2000/06/12 21:48:07  marc
** Added a new class - ADBLogin.  This is a generic login dialog that can
** be used in any Qt based program that uses ADB for database access.
** Altered the defines for using the Qt exentions to ADB.  To use them, the
** user must now define ADBQT=1 in their Makefiles.
**
** Revision 1.19  2000/05/02 20:12:49  marc
** Added the option to return zero dates as empty strings instead of '0000-00-00'.
**
** Revision 1.18  2000/01/03 18:22:58  marc
** Made the order of the fields in the constructors consistant.
**
** Revision 1.17  1999/12/17 18:04:10  marc
** Added the ability for ADB to log to stderr as well as syslog (the default).
**
** Revision 1.16  1999/10/27 00:56:35  marc
** A test for the CVS log function.
**
** Revision 1.15  1999/10/27 00:55:41  marc
** Added some comments to the headers.
** Added appendStr and getLLong to ADBTable.
**
**
*/

#ifndef ADB_H
#define ADB_H

#include <mysql/mysql.h>

#ifdef ADBQT
#include <QtCore/QDateTime>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <Qt3Support/q3textview.h>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#endif

// As MySQL defines bigint as a long long, we need to define it so we don't
// have data overflows or corrupted data.
#ifndef llong
#define llong long long
#endif
#ifndef ullong
#define ullong unsigned long long
#endif


// For speed we pre-allocate enough space to hold column definitions
// up to this number of definitions.
#define ADB_MAXCOLS     128
#define ADB_MAXCOLWIDTH 128


// Logging/Debugging functions
void    ADBLogMsg(int priority, const char *format, ... );
void    ADBDebugMsg(int level,  const char *format, ... );

// Internal definitions for a MySQL column definition
class ADBColumn 
{
public:
    ADBColumn();
    ~ADBColumn();

    void                clear();
    void                clearData();
    
    int                 define(uint columnNo, MYSQL_FIELD *mField, const char *newData = NULL);

    int                 set(const char *newData, int setBackupAlso = 0, int isEncrypted = 0, int useDefKey = 1);
    int                 set(int newValue);
    int                 set(long newValue);
    int                 set(llong newValue);
    int                 set(float newValue);
    int                 set(double newValue);
    int                 append(const char *appVal);
    #ifdef ADBQT
    int                 set(const QDate newValue);
    int                 set(const QTime newValue);
    int                 set(const QDateTime newValue);
    #endif
    
    int                 setColumnName(const char *newName);
    void                setColumnNumber(uint newNum);
    void                setType(enum_field_types newType);
    void                setPrimaryKey(int newIsPrimary);

    const char          *ColumnName();
    const char          *TableName();
    enum_field_types    DataType();
    uint                ColumnNo();
    int                 PrimaryKey();
    int                 ColumnChanged();
    const char          *Data(int useBackup = 0);
    
    // Data conversion functions.
    int                 toInt(int useBackup = 0);
    long                toLong(int useBackup = 0);
    llong               toLLong(int useBackup = 0);
    float               toFloat(int useBackup = 0);
    
    time_t              toTime_t(int useBackup = 0);
    
    #ifdef ADBQT
    // QDateTime functions.
    const QDateTime     toQDateTime(int useBackup = 0);
    const QDate         toQDate(int useBackup = 0);
    const QTime         toQTime(int useBackup = 0);
    #endif
    
    const char          *insStr();
    
    int                 setEncrypted(int isEncrypted, int useDefKey = 1);
    int                 Encrypted();
    
    void                setDebugLevel(int newDebugLevel);

private:
    void                decryptData(void);
    void                encryptData(void);
    
    char                intColumnName[ADB_MAXCOLWIDTH];
    char                intTableName[ADB_MAXCOLWIDTH];
    enum_field_types    intDataType;
    int                 intColumnNo;

    int                 isPrimaryKey;
    int                 intIsEncrypted;
    int                 intUseDefKey;

    int                 debugLevel;
    
    char                *intData;
    char                *intOldData;
    
    char                *workStr;
};


/*
** DBRow   - A class that is the basic construct for retreiving the data
**           from a query.
**
**           This class has no way of performing a query, just loading
**           a row from the database after a query has been performed.
**           when loadRow is called, it loads it from the current database
**           cursor position.
*/

class ADBRow
{
public:
    ADBRow();
    ~ADBRow();
    
    void    setDebugLevel(int newDebugLevel);
    void    setZeroDatesAsNULL(bool newValue);
    void    clearRow();
    int     loadRow(MYSQL_RES *queryRes);
    
    uint    numColumns();
    
    // Magic time.  Here are the operators that will allow access by either
    // the column number or by column name.
    const char *operator[](unsigned int colNo);
    const char *operator[](int colNo);
    const char *operator[](const char *colName);
    
    ADBColumn  *col(unsigned int colNo);
    ADBColumn  *col(int colNo);
    ADBColumn  *col(const char *colName);
    
    ADBColumn  *primaryKey();
    
protected:
    ADBColumn  *findColumn(unsigned int colNo);
    ADBColumn  *findColumn(const char *colName);
    
private:
    ADBColumn   *columns[ADB_MAXCOLS];

    int         intRowDefined;
    uint        numFields;
    
    int         debugLevel;
    bool        zeroDatesAsNULL;
};


/*
** ADB - Object Database Access class.
*/

class ADB
{

public:
    ADB(
      const char *Name  = NULL,
      const char *User  = NULL,
      const char *Pass  = NULL,
      const char *Host  = NULL
    );
    virtual ~ADB();
    
    static const char *defaultHost();
    static  void       setDefaultHost(const char *GlobHost);
    static const char *defaultDBase();
    static  void       setDefaultDBase(const char *GlobDBase);
    static const char *defaultUser();
    static  void       setDefaultUser(const char *GlobUser);
    static const char *defaultPass();
    static  void       setDefaultPass(const char *GlobPass);

    static  void setDebugLevel(int newDebugLevel);
    static  void useSyslog(bool newVal);
    static  void recordUpdates(bool newVal);
    static  void returnEmptyDatesAsNULL(bool newVal);

    int     Connected(void);

    int     query(const char *format, ... );
    float   sumFloat(const char *format, ... );
    int     getrow(void);
    int     getfield(void);
    
    long    dbcmd(const char *format, ... );
    
    // Note that escapeString uses an internal buffer, so multiple calls
    // will trash the return pointer.
    const char  *escapeString(const char *src, int truncLen = 4096);

    ADBRow      curRow;
    ulong       rowCount;
    
protected:
    MYSQL       MyConn;
    MYSQL       *MySock;
    MYSQL_RES   *queryRes;
    
    int         debugLevel;

    // Once the connection is established, these will hold our connect info.
    char        *DBHost;
    char        *DBUser;
    char        *DBPass;
    char        *DBName;
    
    char        *escWorkStr;
    
    int         connected;
};


class ADBTable  : public ADB
{

public:
    ADBTable(
      const char *Table = NULL,
      const char *Name  = NULL,
      const char *User  = NULL,
      const char *Pass  = NULL,
      const char *Host  = NULL
    );
    virtual ~ADBTable();

    // Unless it is specified in the declaration, this can be used to
    // to set the table name that we will be operating on.
    void    setTableName(const char * tabName);
    
    // Clear out all of the data currently in memory.
    void    clearData(void);
    
    // Data assignment members
    int     setValue(uint colNo,          int val);
    int     setValue(const char *colName, int val);
    int     setValue(uint colNo,          long val);
    int     setValue(const char *colName, long val);
    int     setValue(uint colNo,          llong val);
    int     setValue(const char *colName, llong val);
    int     setValue(uint colNo,          float val);
    int     setValue(const char *colName, float val);
    int     setValue(uint colNo,          double val);
    int     setValue(const char *colName, double val);
    int     setValue(uint colNo,          const char *val);
    int     setValue(const char *colName, const char *val);
    int     appendStr(uint colNo,         const char *val);
    int     appendStr(const char *colName,const char *val);
    #ifdef ADBQT
    int     setValue(uint colNo,          const QDate val);
    int     setValue(const char *colName, const QDate val);
    int     setValue(uint colNo,          const QTime val);
    int     setValue(const char *colName, const QTime val);
    int     setValue(uint colNo,          const QDateTime val);
    int     setValue(const char *colName, const QDateTime val);
    #endif
    
    // Data retrieval members
    int     getInt(uint colNo, int useBackup = 0);
    int     getInt(const char *colName, int useBackup = 0);
    long    getLong(uint colNo, int useBackup = 0);
    long    getLong(const char *colName, int useBackup = 0);
    llong   getLLong(uint colNo, int useBackup = 0);
    llong   getLLong(const char *colName, int useBackup = 0);
    float   getFloat(uint colNo, int useBackup = 0);
    float   getFloat(const char *colName, int useBackup = 0);

    const char *getStr(uint colNo, int useBackup = 0);
    const char *getStr(const char *colName, int useBackup = 0);
    
    time_t              getTime_t(uint colNo, int useBackup = 0);
    time_t              getTime_t(const char *colName, int useBackup = 0);
    #ifdef ADBQT
    const   QDate       getDate(uint colNo, int useBackup = 0);
    const   QDate       getDate(const char *colName, int useBackup = 0);
    const   QTime       getTime(uint colNo, int useBackup = 0);
    const   QTime       getTime(const char *colName, int useBackup = 0);
    const   QDateTime   getDateTime(uint colNo, int useBackup = 0);
    const   QDateTime   getDateTime(const char *colName, int useBackup = 0);
    #endif
    
    // Row retrieval access
    long    get(long keyVal);
    int     get(int  keyVal);
    
    // Row insert/update/delete members.
    long            ins(int autoGet = 1);
    virtual void    postIns(void)                   {};
    
    long            upd(int autoGet = 1);
    virtual void    postUpd(void)                   {};

    int             del(long keyVal = 0);
    virtual void    postDel(void)                   {};
    
    // Misc functions.
    int             setEncryptedColumn(uint colNo, int useDefKey = 1);
    int             setEncryptedColumn(const char *colName, int useDefKey = 1);

protected:
    ADBColumn   *columnDefs[ADB_MAXCOLS];
    uint        numColumns;
    uint        primaryKeyColumn;
    
    uint        getColumnNumber(const char *colName);
    
    char        TableName[256];
};


// ADBList simulates a doubly linked list for accessing the columns from 
// a single table returned by a query.  It provides a convenient method 
// to access a full query on a single table.  If joins are used, the ADB 
// class should be used and the results should be obtained manually.
class ADBList : public ADBTable
{
public:
    ADBList(
      const char *Table,
      const char *Name  = NULL,
      const char *User  = NULL,
      const char *Pass  = NULL,
      const char *Host  = NULL
    );
    ~ADBList();

    // The main entry point.  This loads the selected keys into memory and 
    // loads the first row.
    // Only the where portion of the query should be used when giving the
    // query.
    // 
    // Example.  If the Table were "Customers", and contained a column
    // CustomerID (defined as the primary key), and a column "City", and
    // a list of customers based on a certain city is required, the following
    // query would be used:    "where City = 'Seattle'"
    // Which would populate the linked list with all CustomerID's that matched
    // which could then be traversed with first, last, next, prev.
    //
    // If NULL is specified, or no argument is given, it loads all of them.
    long    getList(const char * listQuery = NULL);
    
    // List traversal functions.  Returns zero ((long) 0) if the row was
    // not found.
    // A simple loop would look like this:
    //
    // ADBList  LDB("Customers");
    // LDB.getList("where City = 'Seattle'");
    // for (long liNo == LDB.first(); liNo != 0; liNo = LDB.next()) {
    //    cout << LDB.getLong("CustomerID") << " " << LDB.getStr("City") << endl;
    // }
    long    first(void);
    long    last(void);
    long    prev(void);
    long    next(void);

private:
    long    *keyList;
    long    totKeys;
    long    curKeyNo;
};


#endif // ifdef ADB_H

