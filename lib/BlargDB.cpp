/*
** $Id$
**
** BlargDB - A class library for doing MySQL queries.
**
** BlargDB encompases more than just generic queries, it also defines many
** of the relationships between different files used in Total Accountability.
*/

// #include <qobject.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <mysql/mysql.h>
#include <qapplication.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qregexp.h>
#include <qdatetm.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qrect.h>
#include "BlargDB.h"
#include <time.h>
#include <sys/timeb.h>
#include "AcctsRecv.h"

#include <ADB.h>
#include "BString.h"
#include <TAATools.h>
#include <Cfg.h>
#include <CCValidate.h>

#ifdef USEDES
#include "bdes.h"
#endif

typedef struct PackageItem {
    long    InternalID;
    long    ItemNumber;
    int     LoginGroup;
    char    LoginID[20];
};


// Initialize the connection.  Does nothing, really.  Just sets up our
// internal socket so we can do queries on it.

BlargDB::BlargDB()
{
	//extern int	OpenConns;

	connected = 0;
    #ifdef DBDEBUG    
	// fprintf(stderr, "BlargDB: Connecting to %s as %s, pw %s...\n", DBHost, DBUser, DBPass);
	#endif
    mysql_init(&MyConn);
    if (!(MySock = mysql_real_connect(&MyConn, cfgVal("TAAMySQLHost"), cfgVal("TAAMySQLUser"), cfgVal("TAAMySQLPass"), NULL, 0, NULL, 0))) {
        fprintf(stderr, "BlargDB: Unable to connect to the database.\n");
        exit(-1);
    } else {
        // Connect to the requested datase now.
		#ifdef DBDEBUG
		// fprintf(stderr, "BlargDB: Connecting to the database...\n");
		#endif
        if (mysql_select_db(MySock, cfgVal("TAAMySQLDB")) == -1) {
            fprintf(stderr, "BlargDB: Unable to connect to database '%s'\n", cfgVal("TAAMySQLDB"));
            exit(-1);
        }
    }
    
    //OpenConns++;
    //Instance = OpenConns;
    connected  = 1;
    // Set our initial pointers to NULL
    queryRes   = NULL;
    curField   = NULL;
    // rowLengths = NULL;

    #ifdef DBDEBUG    
    // fprintf(stderr, "BlargDB[%d].connect - returning.  %d open connections.\n", Instance, OpenConns);
    #endif
}

// Destrcutor.  Doesn't need to do anything right now, as it should delete
// itself...

BlargDB::~BlargDB()
{
	//extern int OpenConns;
	//OpenConns--;
    #ifdef DBDEBUG    
	// fprintf(stderr, "BlargDB[%d].destruct - closing connections.  %d Open connections.\n", Instance, OpenConns);
	#endif
    // Free the last query result if it exists...
    if (queryRes != NULL) { 
       mysql_free_result(queryRes);
       queryRes = NULL;
    }
    
    // And disconnect from the database.
    mysql_close(MySock);
}

// query - Do a query on the database we're connected to.

void BlargDB::query(const char *format, ... )
{
    // Make the query string from the variable arguments.
    va_list ap;
    va_start(ap, format);
    char    *querystr = new char[65536];    // Should be big enough for most...
    
    vsprintf(querystr, format, ap);

    #ifdef DBDEBUG
    fprintf(stderr, "BlargDB[%d]: query = %s\n", Instance, querystr);
    #endif // DBDEBUG


    // Free the last query result.
    if (queryRes != NULL) { 
        mysql_free_result(queryRes);
        queryRes = NULL;
    }

    // Do the query.
    if (mysql_query(MySock, querystr) || !(queryRes = mysql_store_result(MySock))) {
        fprintf(stderr, "BlargDB: MySQL error on query.\nQuery: %s\nError: %s\n", querystr, mysql_error(MySock));
        exit(-1);
    } else {
        numFields = mysql_num_fields(queryRes);
        rowCount  = mysql_num_rows(queryRes);
        #ifdef DBDEBUG
        fprintf (stderr, "BlargDB[%d]: query returned %ld rows and %d fields.\n", Instance, rowCount, numFields);
        #endif
    }
    delete querystr;
}

/*
** fSum  - Works exactly like query, but gets the first variable.
**         This is needed because in v3.22.xx of MySQL, they decided to
**         have SUM return NULL if no records matched.
*/

float BlargDB::fSum(const char *format, ... )
{
    // Make the query string from the variable arguments.
    va_list ap;
    va_start(ap, format);
    char    *querystr = new char[65536];    // Should be big enough for most...
    
    vsprintf(querystr, format, ap);

    #ifdef DBDEBUG
    fprintf(stderr, "BlargDB[%d]: query = %s\n", Instance, querystr);
    #endif // DBDEBUG


    // Free the last query result.
    if (queryRes != NULL) { 
        mysql_free_result(queryRes);
        queryRes = NULL;
    }

    // Do the query.
    if (mysql_query(MySock, querystr) || !(queryRes = mysql_store_result(MySock))) {
        fprintf(stderr, "BlargDB: MySQL error on query.\nQuery: %s\nError: %s\n", querystr, mysql_error(MySock));
        exit(-1);
    } else {
        numFields = mysql_num_fields(queryRes);
        rowCount  = mysql_num_rows(queryRes);
        #ifdef DBDEBUG
        fprintf (stderr, "BlargDB[%d]: query returned %ld rows and %d fields.\n", Instance, rowCount, numFields);
        #endif
    }
    delete querystr;
    
    float   RetVal = 0.00;
    getrow();
    if (strlen(curRow[0])) {
        RetVal = atof(curRow[0]);
    }
    return RetVal;
}

// getrow - Loads a row from the database into memory.

int BlargDB::getrow(void)
{

    int ret = 0;
    
    if ((curRow = mysql_fetch_row(queryRes))) {
        rowLengths = (unsigned long *) mysql_fetch_lengths(queryRes);

        // Seek to the first field.
        mysql_field_seek(queryRes, 0);
        
        for (uint i = 0; i < numFields; i++) {
            curField = mysql_fetch_field(queryRes);
        }
        
        // Update our return value letting the caller know it worked.
        ret = 1;
    }
   
    return(ret); 
}

// dbcmd - Execute a command on the database we're connected to.

long BlargDB::dbcmd(const char *format, ... )
{
    // Make the query string from the variable arguments.
    va_list ap;
    va_start(ap, format);
    char    *cmdstr= new char[65536];    // Should be big enough for most...
    
    vsprintf(cmdstr, format, ap);

#ifdef DBDEBUG
    fprintf(stderr, "BlargDB[%d]: command = %s\n", Instance, cmdstr);
    fflush(stderr);
#endif // DBDEBUG

//	MYSQL	res;
	long	Ret = 0;

    // Do the command
    mysql_query(MySock, cmdstr);
    Ret = mysql_insert_id(MySock);
    if (Ret < 0) {
        fprintf(stdout, "BlargDB: MySQL error on command.\nCommand: %s\nError: %s\n", cmdstr, mysql_error(MySock));
        // exit(-1);
    }
    
    delete(cmdstr);
    
    return(Ret);
}

/*
** escapeString - A public function which will escape the passed in string
**				  for database inserts/updates.
**
*/

QString BlargDB::escapeString(QString s, int truncLen)
{
	QString	retStr;
    // Escape the strings
    retStr = s.stripWhiteSpace();
    retStr.truncate(truncLen);
    retStr.replace(QRegExp("\\"), "\\\\");
    retStr.replace(QRegExp("'"),  "\\'");
	return(retStr);
}

/*
** GLDB - Generic routeins for updating & reading the General Ledger tables
**
*/

GLDB::GLDB(void)
{
	// from the GL table
    InternalID      = 0;
    TransID			= "";
    AccountNo		= "";
    Amount			= "";
    LinkedTrans		= "";
    TransType		= "";
    TransTypeLink	= "";
    TransDate		= "";
    BilledDate		= "";
    DueDate			= "";
    Cleared			= "";
    Number			= "";
    NumberStr		= "";
    ItemID			= "";
    Quantity		= "";
    Price			= "";
    Memo			= "";
    
    // from the GLIndex
    IDXTransID		= "";
    SplitCount		= "";
    BaseDesc		= "";
    
}

GLDB::~GLDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified Internal ID.
//
// Args        - int IntID - the internal ID of the record
//
// Returns     - 0 on failure, 1 on success
//

int GLDB::get(long IntID)
{
    int		Ret = 0;
    int		tmpID = 0;
    ADB	    DB;

    DB.query("select * from GL where InternalID = %ld", IntID);
    if (DB.rowCount) {
        DB.getrow();
        tmpID = atoi(DB.curRow["InternalID"]);
    };

    if (IntID == tmpID) {
    
        InternalID		= IntID;
        TransID			= DB.curRow["TransID"];
        AccountNo		= DB.curRow["AccountNo"];
        Amount			= DB.curRow["Amount"];
        LinkedTrans		= DB.curRow["LinkedTrans"];
        TransType		= DB.curRow["TransType"];
        TransTypeLink	= DB.curRow["TransTypeLink"];
        TransDate		= DB.curRow["TransDate"];
        BilledDate		= DB.curRow["BilledDate"];
        DueDate			= DB.curRow["DueDate"];
        Cleared			= DB.curRow["Cleared"];
        Number			= DB.curRow["Number"];
        NumberStr		= DB.curRow["NumberStr"];
        ItemID			= DB.curRow["ItemID"];
        Quantity		= DB.curRow["Quantity"];
        Price			= DB.curRow["Price"];
        Memo			= DB.curRow["Memo"];
        
        // And now, load the base record for it
        DB.query("select * from GLIndex where TransID = %ld", TransID.toLong());
        DB.getrow();
        
        IDXTransID		= DB.curRow["TransID"];
        SplitCount		= DB.curRow["SplitCount"];
        BaseDesc		= DB.curRow["BaseDesc"];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts ONLY an Index record into the database.
//
// Args        - None.
//
// Returns     - the new Trans ID
//

long GLDB::ins(void)
{
    long    Ret = 0;
    QString qquery;
    char    desc[4096];
	ADB	    DB;
    
    // Escape the strings
    strcpy(desc, DB.escapeString(BaseDesc));

    Ret = DB.dbcmd("insert into GLIndex values (0, 0, '%s')",
      desc
    );
    
    IDXTransID.setNum(Ret);
    
    return(Ret);
}

//
// insTrans()       - Inserts ONLY a Transaction record into the database.
//
// Args        - None.
//
// Returns     - the new InternalID
//

long GLDB::insTrans(void)
{
    long    Ret = 0;
    float	tmpAmt;
    char	tmpOp[16];
	ADB	    DB;
    ADBTable    glDB("GL");
    
    // Escape the strings

    glDB.setValue("TransID",        TransID.toLong());
    glDB.setValue("AccountNo",      AccountNo.toInt());
    glDB.setValue("Amount",         Amount.toFloat());
    glDB.setValue("LinkedTrans",    LinkedTrans.toLong());
    glDB.setValue("TransType",      TransType.toInt());
    glDB.setValue("TransTypeLink",  TransTypeLink.toLong());
    glDB.setValue("TransDate",      (const char *) TransDate);
    glDB.setValue("BilledDate",     (const char *) BilledDate);
    glDB.setValue("DueDate",        (const char *) DueDate);
    glDB.setValue("Cleared",        Cleared.toInt());
    glDB.setValue("Number",         Number.toInt());
    glDB.setValue("NumberStr",      (const char *) NumberStr);
    glDB.setValue("ItemID",         ItemID.toLong());
    glDB.setValue("Quantity",       Quantity.toFloat());
    glDB.setValue("Price",          Price.toFloat());
    glDB.setValue("Memo",           (const char *) Memo);

    Ret = glDB.ins();
    TransID.setNum(Ret);
    
    tmpAmt = Amount.toFloat();
    if (tmpAmt < 0.00) {
    	strcpy(tmpOp, "-");
    	tmpAmt = tmpAmt * -1;
    } else {
    	strcpy(tmpOp, "+");
    }
   	DB.dbcmd("update Accounts set Balance = Balance %s %12.2f, TransCount = TransCount + 1 where AccountNo = %d", tmpOp, tmpAmt, AccountNo.toInt());

    return(Ret);
}

//
// upd()       - Updates a GLIndex record ONLY in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int GLDB::upd(void)
{
    int Ret = 0;

    ADBTable    glDB("GLIndex");

    glDB.get(TransID.toLong());
    glDB.setValue("SplitCount", SplitCount.toInt());
    glDB.setValue("BaseDesc",   (const char *) BaseDesc);
    glDB.upd();
    
    return(Ret);
}

//
// del()       - Deletes a GLIndex record and all of its associated GL records.
//
// Args        - The TransID to delete.
//
// Returns     - -1 on failure, 0 on success
//

int GLDB::del(long TransID)
{
    int     Ret = 0;
    long    *intIDIdx;
    
    int	    tmpAccountNo;
    float   tmpAmount;
	ADB     DB;
    
    // We need to add some checks in here to make sure that this transaction
    // isn't required by some other transaction.
    
    DB.dbcmd("delete from GLIndex where TransID = %ld", TransID);
    
    // We now need to load all of the splits and adjust the balances of the
    // Accounts that they refer to.
    DB.query("select InternalID from GL where TransID = %ld", TransID);
    if (DB.rowCount) {
    	intIDIdx = new(long[DB.rowCount + 1]);
    	int tmpCount = DB.rowCount;
    	for (int i = 0; i < tmpCount; i++) {
    		DB.getrow();
    		intIDIdx[i] = atol(DB.curRow["InternalID"]);
    	}
    	
    	// Now that we have the individual row counts, update the accounts.
    	for (int i = 0; i < tmpCount; i++) {
    	    // Load the record from the GL to get the amounts
	    	DB.query("select * from GL where InternalID = %ld", intIDIdx[i]);
	    	DB.getrow();
	    	tmpAccountNo = atoi(DB.curRow["AccountNo"]);
	    	tmpAmount = atof(DB.curRow["Amount"]);
	    	tmpAmount = tmpAmount * -1.0;
	    	
	    	// Now, update the account it refers to.
		    DB.dbcmd("update Accounts set TransCount = TransCount - 1, Balance = Balance + %f where AccountNo = %d", tmpAmount, tmpAccountNo);
		    
		    // Now that we've updated the balance, delete the split record
		    DB.dbcmd("delete from GL where InternalID = %ld", intIDIdx[i]);
    	}
    	
    	// Free up our memory...
    	delete(intIDIdx);
    }
    
    
    return(Ret);
}


//
// PaymentTermsDB - generic routines for reading & updating the 
//                  PaymentTerms table.
//

PaymentTermsDB::PaymentTermsDB(void)
{
    setTableName("PaymentTerms");
}

PaymentTermsDB::~PaymentTermsDB()
{
}

//
// VendorTypesDB - generic routines for reading & updating the 
//                  PaymentTerms table.
//

VendorTypesDB::VendorTypesDB(void)
{
    InternalID      = 0;
    VendorType      = "";
    SubTypeOf       = "";
    HasSubTypes     = "";
}

VendorTypesDB::~VendorTypesDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified Internal ID.
//
// Args        - int IntID - the internal ID of the record
//
// Returns     - 0 on failure, 1 on success
//

int VendorTypesDB::get(int IntID)
{
    int     Ret = 0;
    int     tmpID;
	ADB     DB;

    DB.query("select * from VendorTypes where InternalID = %d", IntID);
    DB.getrow();
    
    tmpID = atoi(DB.curRow["InternalID"]);
    if (IntID == tmpID) {
    
        InternalID  = IntID;
        VendorType  = DB.curRow["VendorType"];
        HasSubTypes = DB.curRow["HasSubTypes"];
        SubTypeOf   = DB.curRow["SubTypeOf"];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Internal ID
//

int VendorTypesDB::ins(void)
{
    int     Ret = 0;
	ADB	        DB;
    ADBTable    vtDB("VendorTypes");

    vtDB.setValue("VendorType",     (const char *) VendorType);
    vtDB.setValue("HasSubTypes",    HasSubTypes.toInt());
    vtDB.setValue("SubTypeOf",      SubTypeOf.toInt());
    vtDB.ins();
    
    // Now, check to see if it is a sub type of something.  If so, update
    // the parent info.
    if (SubTypeOf.toInt() > 0) {
        DB.dbcmd("update VendorTypes set HasSubTypes = HasSubTypes + 1 where InternalID = %d", SubTypeOf.toInt());
    }
    
    return(Ret);
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int VendorTypesDB::upd(void)
{
    int Ret = 0;

    QString tmpSubTypeOf;
	ADB	        DB;
    ADBTable    vtDB("VendorTypes");

    // We need to get the old record to see if any sub/parent info
    // is changing so we can update things properly.
    vtDB.get(InternalID);
    tmpSubTypeOf = vtDB.getStr("SubTypeOf");
    vtDB.setValue("VendorType",     (const char *) VendorType);
    vtDB.setValue("HasSubTypes",    HasSubTypes.toInt());
    vtDB.setValue("SubTypeOf",      SubTypeOf.toInt());
    vtDB.upd();

    // Now, check the previous data to see if we need to adjust anything.
    if (tmpSubTypeOf.toInt() != SubTypeOf.toInt()) {
        // SubTypeOf has changed, so first, update the old one.
        if (tmpSubTypeOf.toInt()) {
            // It wasn't zero, so we have to update one.
            DB.dbcmd("update VendorTypes set HasSubTypes = HasSubTypes - 1 where InternalID = %d", tmpSubTypeOf.toInt());
        }
        
        if (SubTypeOf.toInt()) {
            // We have a different sub-type now.
            DB.dbcmd("update VendorTypes set HasSubTypes = HasSubTypes + 1 where InternalID = %d", SubTypeOf.toInt());
        }
    }
    
    return(Ret);
}

//
// del()       - Deletes a record in the database.
//
// Args        - The InternalID to delete.
//
// Returns     - -1 on failure, 0 on success
//

int VendorTypesDB::del(int IntID)
{
    int     Ret = 0;
    
    int     tmpIntID;
    QString tmpVendorType;
    QString tmpHasSubTypes;
    QString tmpSubTypeOf;
	ADB	    DB;
    
    // First, we need to make sure that no Vendors have been assigned this
    // ID.  If they have, we can't delete it.
    DB.query("select VendorType from Vendors where VendorType = %d", IntID);
    if (DB.rowCount) {
        return(-1);
    }
    
    // We need to get the old record to see if any vendor types are using
    // this one as a parent, or if this one has any sub-types.
    DB.query("select * from VendorTypes where InternalID = %d", IntID);
    DB.getrow();
    tmpIntID       = atoi(DB.curRow["InternalID"]);
    tmpVendorType  = DB.curRow["VendorType"];
    tmpHasSubTypes = DB.curRow["HasSubTypes"];
    tmpSubTypeOf   = DB.curRow["SubTypeOf"];
    
    // We've got the old info, so we can safely delete the record.
    DB.dbcmd("delete from VendorTypes where InternalID = %d", IntID);
    
    // First, take care of any accounts using this one as a parent.
    if (tmpHasSubTypes.toInt() > 0) {
        DB.dbcmd("update VendorTypes set SubTypeOf = 0 where SubTypeOf = %d", tmpIntID);
    }

    // Now, check to see if we were a sub-account of anything, and if so,
    // update the parent so he has one less.
    if (tmpSubTypeOf.toInt() > 0) {
        DB.dbcmd("update VendorTypes set HasSubTypes = HasSubTypes - 1 where InternalID = %d", tmpSubTypeOf.toInt());
    }
    
    return(Ret);
}

//
// AccountsDB - generic routines for reading & updating the 
//                  PaymentTerms table.
//

AccountsDB::AccountsDB(void)
{
    AccountNo     = 0;
    AcctName      = "";
    HasSubAccts   = "";
    SubAcctOf     = "";
    AcctType      = "";
    Reimbursable  = "";
    AcctNumber    = "";
    TaxLine       = "";
    Balance       = "";
    TransCount    = "";
}

AccountsDB::~AccountsDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified AccountNO.
//
// Args        - int AcctNo - the AccountNo of the record
//
// Returns     - 0 on failure, 1 on success
//

int AccountsDB::get(int AcctNo)
{
    int     Ret = 0;
	ADB     DB;

    DB.query("select * from Accounts where AccountNo = %d", AcctNo);
    if (DB.rowCount) {
        DB.getrow();
        
        AccountNo    = AcctNo;
        AcctName     = DB.curRow["AcctName"];
        HasSubAccts  = DB.curRow["HasSubAccts"];
        SubAcctOf    = DB.curRow["SubAcctOf"];
        AcctType     = DB.curRow["AcctType"];
        Reimbursable = DB.curRow["Reimbursable"];
        AcctNumber   = DB.curRow["AcctNumber"];
        TaxLine      = DB.curRow["TaxLine"];
        Balance      = DB.curRow["Balance"];
        TransCount   = DB.curRow["TransCount"];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Internal ID
//

int AccountsDB::ins(void)
{
    int         Ret = 0;
    int         startingAcctNo = 0;
	ADB	        DB;
    ADBTable    acctsDB("Accounts");
    
    AccountNo = 0;
    
    switch(AcctType.toInt()) {
        case 0:  startingAcctNo = 9000; break;
        case 1:  startingAcctNo = 9100; break;
        case 2:  startingAcctNo = 9200; break;
        case 3:  startingAcctNo = 1400; break;
        case 4:  startingAcctNo = 0; break;
        case 5:  startingAcctNo = 2000; break;
        case 6:  startingAcctNo = 2050; break;
        case 7:  startingAcctNo = 2100; break; 
        case 8:  startingAcctNo = 2300; break; 
        case 9:  startingAcctNo = 3000; break; 
        case 10: startingAcctNo = 4000; break; 
        case 11: startingAcctNo = 5000; break; 
        case 12: startingAcctNo = 6000; break; 
        case 13: startingAcctNo = 7000; break; 
        case 14: startingAcctNo = 8000; break; 
        default: startingAcctNo = 0;
    }
    
    while (!AccountNo) {
        DB.query("select AccountNo from Accounts where AccountNo = %d", startingAcctNo);
        if (DB.rowCount) {
            startingAcctNo++;
        } else {
            AccountNo = startingAcctNo;
        }
    }
    
    #ifdef DBDEBUG
    fprintf(stderr, "AccountsDB: next account number is %d\n", AccountNo);
    #endif    
    
    acctsDB.setValue("AccountNo",       AccountNo);
    acctsDB.setValue("AcctName",        (const char *) AcctName);
    acctsDB.setValue("HasSubAccts",     HasSubAccts.toInt());
    acctsDB.setValue("SubAcctOf",       SubAcctOf.toInt());
    acctsDB.setValue("AcctType",        AcctType.toInt());
    acctsDB.setValue("Reimbursable",    Reimbursable.toInt());
    acctsDB.setValue("AcctNumber",      (const char *) AcctNumber);
    acctsDB.setValue("TaxLine",         (const char *) TaxLine);
    acctsDB.setValue("Balance",         Balance.toFloat());
    acctsDB.setValue("TransCount",      TransCount.toInt());
    acctsDB.ins();
    
    // Now, check to see if it is a sub Account of something.  If so, update
    // the parent info.
    if (SubAcctOf.toInt() > 0) {
        DB.dbcmd("update Accounts set HasSubAccts = HasSubAccts + 1 where AccountNo = %d", SubAcctOf.toInt());
    }

    #ifdef DBDEBUG
    fprintf(stderr, "AccountsDB: returning from ins()\n");
    #endif    
    return Ret;
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int AccountsDB::upd(void)
{
    int Ret = 0;
	BlargDB	DB;
    ADBTable    acctsDB("Accounts");

    acctsDB.get(AccountNo);
    acctsDB.setValue("AcctName",        (const char *) AcctName);
    acctsDB.setValue("HasSubAccts",     HasSubAccts.toInt());
    acctsDB.setValue("SubAcctOf",       SubAcctOf.toInt());
    acctsDB.setValue("AcctType",        AcctType.toInt());
    acctsDB.setValue("Reimbursable",    Reimbursable.toInt());
    acctsDB.setValue("AcctNumber",      (const char *) AcctNumber);
    acctsDB.setValue("TaxLine",         (const char *) TaxLine);
    acctsDB.setValue("Balance",         Balance.toFloat());
    acctsDB.setValue("TransCount",      TransCount.toInt());
    acctsDB.upd();

    // Now, check the previous data to see if we need to adjust anything.
    if (acctsDB.getInt("SubAcctOf", 1) != SubAcctOf.toInt()) {
        // SubAcctOf has changed, so first, update the old one.
        if (acctsDB.getInt("SubAcctOf", 1)) {
            // It wasn't zero, so we have to update one.
            DB.dbcmd("update Accounts set HasSubAccts = HasSubAccts - 1 where AccountNo = %d", acctsDB.getInt("SubAcctOf", 1));
        }
        
        if (SubAcctOf.toInt()) {
            // We have a different sub-type now.
            DB.dbcmd("update Accounts set HasSubAccts = HasSubAccts + 1 where AccountNo = %d", SubAcctOf.toInt());
        }
    }
    
    return Ret;
}

//
// del()       - Deletes a record in the database.
//
// Args        - The AccountNo to delete.
//
// Returns     - -1 on failure, 0 on success
//

int AccountsDB::del(int AcctNo)
{
    int     Ret = 0;
    
    int     tmpAcctNo;
    QString tmpHasSubAccts;
    QString tmpSubAcctOf;
	BlargDB	DB;

    // First things first.  We need to check to see if this account can
    // be deleted.  It can't if there are transactions referencing it in
    // either the Register or RegisterSplits tables.
    DB.query("select TransID from Register where AccountNo = %d", AcctNo);
    if (DB.rowCount) {
        Ret = 1;
    } else {
        // Check RegisterSplits
        DB.query("select TransID from RegisterSplits where AccountNo = %d", AcctNo);
        if (DB.rowCount) {
            Ret = 1;
        }
    }
    
    if (!Ret) {
        // We need to get some data from old record to see if any sub/parent info
        // is changing so we can update things properly.
        DB.query("select AccountNo, HasSubAccts, SubAcctOf from Accounts where AccountNo = %d", AcctNo);
        DB.getrow();
        tmpAcctNo      = atoi(DB.curRow[0]);
        tmpHasSubAccts = DB.curRow[1];
        tmpSubAcctOf   = DB.curRow[2];
    
        // We've got the old info, so we can safely delete the record.
        DB.dbcmd("delete from Accounts where AccountNo = %d", tmpAcctNo);
    
        // First, take care of any accounts using this one as a parent.
        if (tmpHasSubAccts.toInt() > 0) {
            DB.dbcmd("update Accounts set SubAcctOf = 0 where SubAcctOf = %d", tmpAcctNo);
        }

        // Now, check to see if we were a sub-account of anything, and if so,
        // update the parent so he has one less.
        if (tmpSubAcctOf.toInt() > 0) {
            DB.dbcmd("update Accounts set HasSubAccts = HasSubAccts - 1 where AccountNo = %d", tmpSubAcctOf.toInt());
        }
    }
    return(Ret);
}


//
// BillingCyclesDB - generic routines for reading & updating the 
//                   BillingCycles table.
//

BillingCyclesDB::BillingCyclesDB(void)
{
	InternalID	= 0;
    CycleID     = "";
    Description = "";
    Jan			= "";
    Feb			= "";
    Mar			= "";
    Apr			= "";
    May			= "";
    Jun			= "";
    Jul			= "";
    Aug			= "";
    Sep			= "";
    Oct			= "";
    Nov			= "";
    Dece		= "";
    Day			= "";
    DefaultCycle= "";
}

BillingCyclesDB::~BillingCyclesDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified cycleID.
//
// Args        - int AcctNo - the AccountNo of the record
//
// Returns     - 0 on failure, 1 on success
//

int BillingCyclesDB::get(long internalID)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.query("select * from BillingCycles where InternalID = %ld", internalID);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID	= atol(DB.curRow["InternalID"]);
        CycleID		= DB.curRow["CycleID"];
        Description	= DB.curRow["Description"];
        Jan			= DB.curRow["Jan"];
        Feb			= DB.curRow["Feb"];
        Mar			= DB.curRow["Mar"];
        Apr			= DB.curRow["Apr"];
        May			= DB.curRow["May"];
        Jun			= DB.curRow["Jun"];
        Jul			= DB.curRow["Jul"];
        Aug			= DB.curRow["Aug"];
        Sep			= DB.curRow["Sep"];
        Oct			= DB.curRow["Oct"];
        Nov			= DB.curRow["Nov"];
        Dece		= DB.curRow["Dece"];
        Day			= DB.curRow["Day"];
        DefaultCycle= DB.curRow["DefaultCycle"];
        Ret = 1;
    }
    return(Ret);
}

//
// getByCycleID() - Gets a record from the database using the
//                  specified cycleID.
//
// Args        - char CycleID - the Billing Cycle ID
//
// Returns     - 0 on failure, 1 on success
//

int BillingCyclesDB::getByCycleID(const char * cycleID)
{
    int     Ret = 0;
    char	tmpCycleID[1024];
	ADB	    DB;

	strcpy(tmpCycleID, DB.escapeString(cycleID, 16));
	    
    DB.query("select * from BillingCycles where CycleID = '%s'", tmpCycleID);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID	= atol(DB.curRow["InternalID"]);
        CycleID		= DB.curRow["CycleID"];
        Description	= DB.curRow["Description"];
        Jan			= DB.curRow["Jan"];
        Feb			= DB.curRow["Feb"];
        Mar			= DB.curRow["Mar"];
        Apr			= DB.curRow["Apr"];
        May			= DB.curRow["May"];
        Jun			= DB.curRow["Jun"];
        Jul			= DB.curRow["Jul"];
        Aug			= DB.curRow["Aug"];
        Sep			= DB.curRow["Sep"];
        Oct			= DB.curRow["Oct"];
        Nov			= DB.curRow["Nov"];
        Dece		= DB.curRow["Dece"];
        Day			= DB.curRow["Day"];
        DefaultCycle= DB.curRow["DefaultCycle"];
        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Internal ID
//

long BillingCyclesDB::ins(void)
{
    long        Ret = 0;
    ADBTable    bcDB("BillingCycles");
    
	if (DefaultCycle.toInt()) {
	    ADB     DB;
	    DB.dbcmd("update BillingCycles set DefaultCycle = 0");
	}

    bcDB.setValue("CycleID",        (const char *) CycleID);
    bcDB.setValue("Description",    (const char *) Description);
    bcDB.setValue("Jan",            Jan.toInt());
    bcDB.setValue("Feb",            Feb.toInt());
    bcDB.setValue("Mar",            Mar.toInt());
    bcDB.setValue("Apr",            Apr.toInt());
    bcDB.setValue("May",            May.toInt());
    bcDB.setValue("Jun",            Jun.toInt());
    bcDB.setValue("Jul",            Jul.toInt());
    bcDB.setValue("Aug",            Aug.toInt());
    bcDB.setValue("Sep",            Sep.toInt());
    bcDB.setValue("Oct",            Oct.toInt());
    bcDB.setValue("Nov",            Nov.toInt());
    bcDB.setValue("Dece",           Dece.toInt());
    bcDB.setValue("DefaultCycle",   DefaultCycle.toInt());
    Ret = bcDB.ins();

    InternalID = Ret;
    
    return(Ret);
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int BillingCyclesDB::upd(void)
{
    int     Ret = 0;
    ADBTable    bcDB("BillingCycles");

    if (DefaultCycle.toInt()) {
	    ADB     DB;
        DB.dbcmd("update BillingCycles set DefaultCycle = 0");
    }

    bcDB.get(InternalID);
    bcDB.setValue("CycleID",        (const char *) CycleID);
    bcDB.setValue("Description",    (const char *) Description);
    bcDB.setValue("Jan",            Jan.toInt());
    bcDB.setValue("Feb",            Feb.toInt());
    bcDB.setValue("Mar",            Mar.toInt());
    bcDB.setValue("Apr",            Apr.toInt());
    bcDB.setValue("May",            May.toInt());
    bcDB.setValue("Jun",            Jun.toInt());
    bcDB.setValue("Jul",            Jul.toInt());
    bcDB.setValue("Aug",            Aug.toInt());
    bcDB.setValue("Sep",            Sep.toInt());
    bcDB.setValue("Oct",            Oct.toInt());
    bcDB.setValue("Nov",            Nov.toInt());
    bcDB.setValue("Dece",           Dece.toInt());
    bcDB.setValue("DefaultCycle",   DefaultCycle.toInt());
    Ret = bcDB.upd();

    return(Ret);
}

//
// del()       - Deletes a record in the database.
//
// Args        - The AccountNo to delete.
//
// Returns     - -1 on failure, 0 on success
//

int BillingCyclesDB::del(long internalID)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.query("select CustomerID from Customers where BillingCycle = %ld", internalID);
    if (DB.rowCount) {
        return -1;
    }
    
    DB.dbcmd("delete from BillingCycles where InternalID = %ld", internalID);
    
    // Make sure we still have a default billing cycle.
    DB.query("select InternalID from BillingCycles where DefaultCycle = 1");
    if (!DB.rowCount) {
        // We don't have one, assign the default to the first one we find.
        DB.query("select InternalID from BillingCycles order by InternalID");
        if (DB.rowCount) {
            DB.getrow();
            DB.dbcmd("update BillingCycles set DefaultCycle = 1 where InternalID = %d", atoi(DB.curRow["InternalID"]));
        } else {
            fprintf(stderr, "Warning! No billing cycles defined, unable to assign default cycle\n");
        }
    }
    
    return(Ret);
}

/*
** getCycleDates - Calculates the dates to be used for the supplied billing 
**                 cycle.  This includes the CycleStart date, the CycleEnd
**                 date, the and the number of days in the current cycle.
*/

void BillingCyclesDB::getCycleDates(QDate *StartDate, QDate *EndDate, int *TotalDays, int *DaysLeft, char *asOf)
{
	int	Y1, Y2, M1, M2, D1, D2 = 0;
	int				looping = 0;
	QDate			tmpDate;
	int				Months[13];

	Months[1]  = Jan.toInt();
	Months[2]  = Feb.toInt();
	Months[3]  = Mar.toInt();
	Months[4]  = Apr.toInt();
	Months[5]  = May.toInt();
	Months[6]  = Jun.toInt();
	Months[7]  = Jul.toInt();
	Months[8]  = Aug.toInt();
	Months[9]  = Sep.toInt();
	Months[10] = Oct.toInt();
	Months[11] = Nov.toInt();
	Months[12] = Dece.toInt();

	// The start date will be the "Day" from the Billing Cycle record.
	// The end date will be the Next month and then Day - 1.
	// Start out with the current date.
    if (asOf == NULL) {
    	tmpDate	= QDate::currentDate();
    } else {
        myDatetoQDate(asOf, &tmpDate);
    }
	Y1 = Y2 = tmpDate.year();
	M1 = M2 = tmpDate.month();
	D1 = D2 = tmpDate.day();

	// If the day for the cycle is greater than the current day, but we
	// are still in the same month, we need to back up the month for the
	// cycle since the cycle isn't on the first of the month.
	if (D1 > Day.toInt() && Months[M1] == 0) {
		M1--;
		if (M1 < 1) {
			M1 = 12;
			Y1--;
		}
	}

    // Get the month for the start of the current cycle.
	while(!looping) {
		if (Months[M1]) {
			looping = 1;
		} else {
			M1--;
			if (M1 < 1) {
				M1 = 12;
				Y1--;
			}
		}
	}
	
	// Now, set the Day and we should have the starting day of this customer's
	// billing cycle.
	D1 = Day.toInt();
	StartDate->setYMD(Y1, M1, D1);
	
	Y2 = Y1;
	M2 = M1;
	D2 = D1;
	
	// Now, get the ending date.  This one is a bit easier.  We subtract one
	// from the Day, and add months to it until we come across the next month
	// in the cycle.
	D2--;
	if (D2 < 1) {
		// Since the date was on the first, we need to do a little more
		// checking to make sure we get the cycle right...
		looping = 0;
		D2 = 31;
		while (!looping) {
			if (M2 == 12) {
				if (Months[1]) {
					// It appears to be a monthly cycle, so decrement the 
					// month before entering into the loop.
					looping = 1;
				} else {
				    M2 = 1;
				    Y2++;
				}
			} else {
				if (Months[M2+1]) {
					// It appears to be a monthly cycle, so decrement the
					// month before entering into the loop...
					looping = 1;
				} else {
					M2++;
					if (M2 > 12) {
						M2 = 1;
						Y2++;
					}
				}
			}
		}
	} else {
		looping = 0;
		while(!looping) {
			M2++;
			if (M2 > 12) {
				M2 = 1;
				Y2++;
			}
			if (Months[M2]) {
				// We're done.
				looping = 1;
			}
		}
	}
	// Now, make sure that our day is okay for the number of days in the
	// ending month.
	EndDate->setYMD(Y2, M2, 1);
	if (EndDate->daysInMonth() < D2) {
		D2 = EndDate->daysInMonth();
	}
	EndDate->setYMD(Y2, M2, D2);
	
	tmpDate = QDate::currentDate();
	*TotalDays = StartDate->daysTo(*EndDate) + 1;
	*DaysLeft  = tmpDate.daysTo(*EndDate) + 1;
}


//
// getDefaultCycle() - Gets the default billing cycle to be assigned to 
//                     new users.
//
// Args        - None.
//
// Returns     - 0 on failure, 1 on success
//

int BillingCyclesDB::getDefaultCycle(void)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.query("select * from BillingCycles where DefaultCycle <> 0");
    
    if (DB.rowCount) {
        DB.getrow();
        
        InternalID	= atol(DB.curRow["InternalID"]);
        CycleID		= DB.curRow["CycleID"];
        Description	= DB.curRow["Description"];
        Jan			= DB.curRow["Jan"];
        Feb			= DB.curRow["Feb"];
        Mar			= DB.curRow["Mar"];
        Apr			= DB.curRow["Apr"];
        May			= DB.curRow["May"];
        Jun			= DB.curRow["Jun"];
        Jul			= DB.curRow["Jul"];
        Aug			= DB.curRow["Aug"];
        Sep			= DB.curRow["Sep"];
        Oct			= DB.curRow["Oct"];
        Nov			= DB.curRow["Nov"];
        Dece		= DB.curRow["Dece"];
        Day			= DB.curRow["Day"];
        DefaultCycle= DB.curRow["DefaultCycle"];
        Ret = 1;
    } else {
        fprintf(stderr, "Warning! No default billing cycle found.");
    }
    return(Ret);
}



/*
*************************************************************************
**
** BillablesDataDB - generic routines for reading & updating the
**                   BillablesData table.
**
************************************************************************
*/

BillablesDataDB::BillablesDataDB(void)
{
    setTableName("BillablesData");
}

BillablesDataDB::~BillablesDataDB()
{
}

//
// getItem()   - Gets a record from the database using the
//               specified ItemNumber, RatePlanID and CycleID.
//
// Args        - int ItemNumber, RatePlanID, and CycleID
//
// Returns     - 0 on failure, 1 on success
//

long BillablesDataDB::getItem(long itemNo, long ratePlanID, long cycleID)
{
    long    Ret = 0;
	ADB     DB;
    

    DB.query("select InternalID from BillablesData where ItemNumber = %ld and RatePlanID = %ld and CycleID = %ld",
      itemNo,
      ratePlanID,
      cycleID
    );
    if (DB.rowCount) {
    	DB.getrow();
    	get(atol(DB.curRow["InternalID"]));
    	Ret = atoi(DB.curRow["InternalID"]);
    }
    return(Ret);
}


/*
*************************************************************************
**
** BillablesDB - generic routines for reading & updating the
**               Billables table.
**
************************************************************************
*/

BillablesDB::BillablesDB(void)
{
    setTableName("Billables");
}

BillablesDB::~BillablesDB()
{
}

/*
*************************************************************************
**
** PackagesDB - generic routines for reading & updating the
**              Packages table.
**
************************************************************************
*/

PackagesDB::PackagesDB(void)
{
    setTableName("Packages");
}

PackagesDB::~PackagesDB()
{
}


//
// del()       - Deletes a record in the database.
//
// Args        - The ItemNo to delete.
//
// Returns     - -1 on failure, 0 on success
//

void PackagesDB::postDel(void)
{
    if (!getLong("InternalID")) return;
    
	ADB     DB;
    long    ItemNo = getLong("InternalID");
    
    if (ItemNo) {
	    DB.dbcmd("delete from PackageContents where PackageTag = %ld", ItemNo);
	    DB.dbcmd("delete from PackagesData where PackageTag = %ld", ItemNo);
    }
    return;
}


/*
*************************************************************************
**
** PackagesDataDB - generic routines for reading & updating the
**                  PackagesData table.
**
************************************************************************
*/

PackagesDataDB::PackagesDataDB(void)
{
    setTableName("PackagesData");
}

PackagesDataDB::~PackagesDataDB()
{
}


/*
*************************************************************************
**
** PackageContentsDB - generic routines for reading & updating the
**                     PackageContents table.
**
************************************************************************
*/

PackageContentsDB::PackageContentsDB(void)
{
    setTableName("PackageContents");
}

PackageContentsDB::~PackageContentsDB()
{
}


/*
*************************************************************************
**
** CustomersDB - generic routines for reading & updating the
**               Customers table.
**
************************************************************************
*/

CustomersDB::CustomersDB(void)
{
    setTableName("Customers");
}

CustomersDB::~CustomersDB()
{
}

/*
** postUpd  - This gets called after the ADBTable 'updates' the Customers
**            file.  This is where we want to check to see if we have changed
**            the customer's billing cycle, rate plan, etc for changes.
**            If the rate plan or billing cycle has changed, we will
**            re-charge them for any charges on or after the date specified
**            in RatePlanDate and/or BillingCycleDate, leaving the customer
**            with the appropriate amount due....Very nifty.
*/

void CustomersDB::postUpd(void)
{
    // First things first, check for a RatePlan change...
    if (getLong("RatePlan") != getLong("RatePlan", 1)) {
        if (getInt("RatePlan", 1) != 0) {
            rerateRatePlan(getStr("RatePlanDate"));
        }
    }
    
    // Now, check for a BillingCycle change...
    if (getLong("BillingCycle") != getLong("BillingCycle", 1)) {
        rerateBillingCycle(getStr("BillingCycleDate"));
    }
}

/*
** rerateRatePlan - Scans through the AcctsRecv table and re-rates any charges
**                  on or after the specified date.  It uses the current
**                  rate plan information.
**
*/

void CustomersDB::rerateRatePlan(const char * effDate)
{
    ADB             DB;
    BillablesDB     BDB;
    BillablesDataDB BDDB;
    
    // A re-rate has to be done very carefully.  We want to update the data
    // in the database without actually deleting records and such, but...
    
    // The easiest way to do this is to load each AcctsRecv record one by
    // one, adjust the necessary values, delete the old AcctsRecv entry and
    // their GL entries, and then insert a new record.
    
    // With this path in mind, lets get our list of AcctsRecv records that
    // we want to change.
    DB.query("select InternalID from AcctsRecv where CustomerID = %ld and RatePlan <> %ld and RatePlan <> 0 and TransDate >= '%s'", getLong("CustomerID"), getLong("RatePlan"), effDate);
    if (!DB.rowCount) return;

    // If we made it here, there are transactions waiting for us....
    while (DB.getrow()) {
        printf("Found transaction %ld to rerate...\n", atol(DB.curRow["InternalID"]));
        AcctsRecv       AR;
        AcctsRecvDB     ARDB;
        ADBTable        ADBAR("AcctsRecv");
        
        // Now, load the transaction information.
        ADBAR.get(atol(DB.curRow["InternalID"]));
        
        // Set up the new transaction with the new amounts.
        // A few things to note.  This next block can only do RatePlans.
        // When doing billing cycle changes, we need to check the start
        // and end dates, and also any transactions between the new start
        // and end dates and make sure that the customer isn't going to
        // be overcharged for anything.
        AR.ARDB->setValue("CustomerID", ADBAR.getLong("CustomerID"));
        AR.ARDB->setValue("LoginID", ADBAR.getStr("LoginID"));
        
        AR.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
        AR.ARDB->setValue("RatePlan", getLong("RatePlan"));
        AR.ARDB->setValue("ItemID", ADBAR.getLong("ItemID"));
        AR.ARDB->setValue("TransDate", ADBAR.getStr("TransDate"));
        AR.ARDB->setValue("StartDate", ADBAR.getStr("StartDate"));
        AR.ARDB->setValue("EndDate", ADBAR.getStr("EndDate"));
        AR.ARDB->setValue("Quantity", ADBAR.getFloat("Quantity"));
        AR.ARDB->setValue("Price", ADBAR.getFloat("Price"));
        AR.ARDB->setValue("Amount", ADBAR.getFloat("Amount"));
        AR.ARDB->setValue("Memo", (const char *) ADBAR.getStr("Memo"));
        AR.ARDB->appendStr("Memo", " (Rerated for Rate Plan change)");

        // Now that we've got the basics in there, lets see what we need to
        // change...
        BDB.get(ADBAR.getLong("ItemID"));
        int isPrimary = 1;
        
        // Check to see if the charge was for a primary or secondary price.
        // We'll do the check on the secondary price and if it doesn't match
        // we charge them the primary price.  This way, if we re-rate someone
        // who was being grandfathered at old rates, then we charge them
        // the new rates.
        BDDB.getItem(ADBAR.getLong("ItemID"), ADBAR.getLong("RatePlan"), ADBAR.getLong("BillingCycle"));
        if (ADBAR.getFloat("Price") == BDDB.getFloat("SecondaryPrice")) {
            isPrimary = 0;
        }
            
        BDDB.getItem(ADBAR.getLong("ItemID"), getLong("RatePlan"), getLong("BillingCycle"));
        // Now, check to see if they were charged the secondary price or
        // the primary price.
        if (isPrimary) {
            AR.ARDB->setValue("Price", BDDB.getFloat("Price"));
            AR.ARDB->setValue("Amount", (float) (BDDB.getFloat("Price") * ADBAR.getFloat("Quantity")));
        } else {
            AR.ARDB->setValue("Price", BDDB.getFloat("SecondaryPrice"));
            AR.ARDB->setValue("Amount", (float) (BDDB.getFloat("SecondaryPrice") * ADBAR.getFloat("Quantity")));
        }
        
        AR.SaveTrans();
        
        // Now, lets do something with the old charge, like delete it.
        ARDB.del(atol(DB.curRow["InternalID"]));
    }
}

/*
** rerateBillingCycle - Scans through the AcctsRecv table and re-rates any 
**                      charges on or after the specified date.  It uses the 
**                      current rate plan information.
**
*/

void CustomersDB::rerateBillingCycle(const char * effDate)
{
    ADB             DB;
    BillablesDB     BDB;
    BillablesDataDB BDDB;
    
    // A re-rate has to be done very carefully.  We want to update the data
    // in the database without actually deleting records and such, but...
    
    // The easiest way to do this is to load each AcctsRecv record one by
    // one, adjust the necessary values, delete the old AcctsRecv entry and
    // their GL entries, and then insert a new record.
    
    // Step 1.  Find any transactions that meet the following criteria:
    //              - StartDate < effDate
    //              - EndDate   > effDate
    //              - BillingCycle = OldBillingCycle
    //              - It is linked to a subscription item.


    // With this path in mind, lets get our list of AcctsRecv records that
    // we want to change.
    DB.query("select InternalID from AcctsRecv where CustomerID = %ld and RatePlan <> %ld and RatePlan <> 0 and TransDate >= '%s'", getLong("CustomerID"), getLong("RatePlan"), effDate);
    if (!DB.rowCount) return;

    // If we made it here, there are transactions waiting for us....
    while (DB.getrow()) {
        printf("Found transaction %ld to rerate...\n", atol(DB.curRow["InternalID"]));
        AcctsRecv       AR;
        AcctsRecvDB     ARDB;
        ADBTable        ADBAR("AcctsRecv");

        // Now, load the transaction information.
        ADBAR.get(atol(DB.curRow["InternalID"]));
        
    }
}




/*
** doSubscription - Takes the current customer, loads the subscriptions 
**                  for them and processes them.
**
**        Returns - 1 on error, 0 on success.
*/

int CustomersDB::doSubscriptions(void)
{

	if (!getLong("CustomerID")) return 1;

	int				RetVal = 0;
	ADB			    DB;
	ADB             DB2;
	BillingCyclesDB	BCDB;
	SubscriptionsDB	SDB;
	PackagesDB		PDB;
	BillablesDB		BDB;
	BillablesDataDB	BDDB;
	RatePlansDB		RPDB;
	char			Today[64];
	char            Yesterday[64];
	QDate			CycleStart;		// The day that their billing cycle starts
	QDate			CycleEnd;		// The day that their billing cycle ends
	QDate			CurrentDate;    // The current date.
	QDate           YesterdaysDate;
	QDate           ChargeDate;     // The date to charge them on, which is EndsOnDate + 1 day.
	QDate           TmpDate;
	char            sChargeDate[64];
	char			sCycleEnd[64];
	int				CycleDays;      // The number of days in the billing cycle for pro-rating
	int				DaysLeft;		// The remaining days in this cycle.
	float			Quantity;
	int				SubscrPriority = 9999;
	float			thePrice = 0.0;
	long            PSetupItem = 0;
	int             isSecondary = 0;
	
	// First off, lets load some preliminary information about this user.
	// Things such as their billing cycle, rate plan, etc.
	#ifdef DBDEBUG
	printf("Getting the rate plan...\n");
	fflush(stdout);
    #endif
	RPDB.get(getLong("RatePlan"));
	#ifdef DBDEBUG
	printf("Getting the Billing Cycle...\n");
	fflush(stdout);
    #endif
	BCDB.get(getLong("BillingCycle"));
		
	#ifdef DBDEBUG
	printf("Getting the Billing Cycle Dates...\n");
	fflush(stdout);
    #endif
	// Find out the start and stop dates of the customers billing cycle.
	BCDB.getCycleDates(&CycleStart, &CycleEnd, &CycleDays, &DaysLeft);
	#ifdef DBDEBUG
	printf("Getting the Current Date and Yesterdays date...\n");
	fflush(stdout);
    #endif
	CurrentDate = QDate::currentDate();
	QDatetomyDate(Today, CurrentDate);
	QDatetomyDate(sCycleEnd, CycleEnd);
	YesterdaysDate = CurrentDate.addDays(-1);
    QDatetomyDate(Yesterday, YesterdaysDate);
	
	
	#ifdef DBDEBUG
	printf("\nCycle is %d, there are a total of %d days in the cycle.\n", getLong("BillingCycle"), CycleDays);
	printf("The date is %04d-%02d-%02d and there %d billable days left in the cycle.\n", CurrentDate.year(), CurrentDate.month(), CurrentDate.day(), DaysLeft);
	printf("Cycle Start Date is %04d-%02d-%02d,", CycleStart.year(), CycleStart.month(), CycleStart.day());
	printf("the Cycle End Date is %04d-%02d-%02d\n", CycleEnd.year(), CycleEnd.month(), CycleEnd.day());
	fflush(stdout);
	#endif
	
	// Load the internal ID's of all of the active subscriptions for the 
	// customer.
	// This is done in two parts.  First, we load any packages the user may
	// have set up into memory.  We then scan through those packages and
	// charge the user for them.  With the scanSubscriptions() function 
	// maintaining the links between packages and subscriptions, we can do
	// this in a much simpler fashion.  We still put in statement charges for
	// all of the subscription items, but the ones incorporated into packages
	// have a zero dollar charge amount, and have the text "(Included as 
	// part of {Package Name})" appended to it.


    // Find out what the lowest priority item is and plug that value into
    // SubscrPriority.
    DB.query("select InternalID, ItemNumber, EndsOn from Subscriptions where CustomerID = %ld and Active <> 0 order by InternalID", getLong("CustomerID"));
    while (DB.getrow()) {
        BDB.get(atol(DB.curRow["ItemNumber"]));

        if (BDB.getInt("Priority") < SubscrPriority) {
            SubscrPriority = BDB.getInt("Priority");
            
            // Check to see if we have already charged the user for this
            // subscription in this billing cycle.  If so, lower the
            // SubscrPriority again.
            if (!strcmp("0000-00-00", DB.curRow["EndsOn"])) TmpDate = YesterdaysDate;
            else myDatetoQDate(DB.curRow["EndsOn"], &TmpDate);
	        if (TmpDate >= CurrentDate) SubscrPriority--;

        }

    }
    
    
    // So, that being the case, load all packages from the Subscriptions for
	// this customer.
	DB.query("select InternalID from Subscriptions where CustomerID = %ld and Active = 1 and PackageNo <> 0 and EndsOn < '%s'", getLong("CustomerID"), Today);
	#ifdef DBDEBUG
	printf("%ld Package Items found.\n", DB.rowCount);
	#endif
	if (DB.rowCount) while (DB.getrow()) {
		SDB.get(atol(DB.curRow["InternalID"]));
		// We have some packages.  Load them into memory and charge as we go.
		PDB.get(SDB.getLong("PackageNo"));
		
		// Make sure this package has some active children.  If it doesn't
		// don't process it, set the package to inactive and skip it.
		DB2.query("select InternalID from Subscriptions where CustomerID = %ld and Active <> 0 and ParentID = %ld",
		  getLong("CustomerID"),
		  SDB.getLong("InternalID")
		);
		if (!DB2.rowCount) {
		    // There are no children in this package.
		    // Set the package to inactive and don't process it.
		    SDB.setValue("Active", 0);
		    SDB.upd();
		} else {
		    // The package has active children.  Process it and them.
			    
	        // Now, save the transaction into the AR.
			AcctsRecv		AR;
			#ifdef DBDEBUG
			printf("Doing Package ID %ld", atol(DB.curRow["InternalID"]));
			#endif
			
			// Get the charge date.
			if (!strcmp("0000-00-00", SDB.getStr("EndsOn"))) SDB.setValue("EndsOn", Yesterday);
	        myDatetoQDate((const char *)SDB.getStr("EndsOn"), &TmpDate);
	        ChargeDate = TmpDate.addDays(1);
	        QDatetomyDate(sChargeDate, ChargeDate);
	        DaysLeft   = ChargeDate.daysTo(CycleEnd);
	        
			AR.ARDB->setValue("CustomerID", getLong("CustomerID"));
			AR.ARDB->setValue("LoginID", "Package");
			AR.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
			AR.ARDB->setValue("RatePlan", getLong("RatePlan"));
			AR.ARDB->setValue("ItemID", (long) 0);
			AR.ARDB->setValue("TransDate", sChargeDate);
			AR.ARDB->setValue("StartDate", sChargeDate);
			AR.ARDB->setValue("EndDate", sCycleEnd);
			// Get the quantity
			if (SDB.getFloat("Quantity") == 0.00) {
			    SDB.setValue("Quantity", (float) 1.00);
			}
			if (ChargeDate.day() == CycleStart.day()) {
				Quantity = 1.00 * SDB.getFloat("Quantity");
			} else {
				Quantity = (float)(((float) DaysLeft / (float) CycleDays) * SDB.getFloat("Quantity"));
			}
			#ifdef DBDEBUG
			printf("Quantity = %0.2f, DaysLeft = %d, CycleDays = %d, SDB.Quantity = %f\n", Quantity, DaysLeft, CycleDays, SDB.getFloat("Quantity"));
			#endif
			AR.ARDB->setValue("Memo", PDB.getStr("Description"));
			// A package price always follows the package.
			if (Quantity < 1.00) {
				AR.ARDB->appendStr("Memo", " (Prorated)");
			}
			// Get the price for the package, based on the customers billing
			// cycle and rate plan.
			float   PPrice = 0.00;
			PSetupItem  = 0;
			DB2.query("select Price, SetupItem from PackagesData where RatePlanID = %ld and CycleID = %ld and PackageID = %ld", getLong("RatePlan"), getLong("BillingCycle"), PDB.getLong("InternalID"));
			if (DB2.rowCount) {
			    DB2.getrow();
			    PPrice = atof(DB2.curRow["Price"]);
			    PSetupItem = atol(DB2.curRow["SetupItem"]);
			}
			AR.ARDB->setValue("Price", PPrice);
			AR.ARDB->setValue("Quantity", Quantity);
			AR.ARDB->setValue("Amount", Quantity * PPrice);

			AR.SaveTrans();
			// GrandTotal += AR.ARDB->Amount.toFloat();
			
			SDB.setValue("LastDate", Today);
			SDB.setValue("EndsOn", sCycleEnd);
			
			// See if we've ever charged a setup charge for this package, and
			// if not, make sure there is a setup charge for this.
			if (!SDB.getInt("SetupCharged") && PSetupItem) {
			    // Nope.  Lets charge them now...
			    // We've got the setup Item to charge them already in memory...
                BillablesDB         tmpBDB;
	            BillablesDataDB     BDDB;
                tmpBDB.get(PSetupItem);
	            BDDB.getItem(PSetupItem, getLong("RatePlan"), getLong("BillingCycle"));
	            // Okay, we've got everything we need, setup the AR entry.
	            AcctsRecv       AR2;
				AR2.ARDB->setValue("CustomerID", getLong("CustomerID"));
				AR2.ARDB->setValue("LoginID", "Package");
				AR2.ARDB->setValue("ItemID", PSetupItem);
	    		AR2.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
		    	AR2.ARDB->setValue("RatePlan", getLong("RatePlan"));
				AR2.ARDB->setValue("TransDate", sChargeDate);
				AR2.ARDB->setValue("StartDate", sChargeDate);
				AR2.ARDB->setValue("EndDate", sChargeDate);
				AR2.ARDB->setValue("Memo", tmpBDB.getStr("Description"));
				AR2.ARDB->setValue("Price", BDDB.getFloat("Price"));
				AR2.ARDB->setValue("Quantity", (float) 1.00);
				AR2.ARDB->setValue("Amount", BDDB.getFloat("Price"));

				AR2.SaveTrans();
                SDB.setValue("SetupCharged", 1);
			} else {
			    // Since they were either already charged, or there is no
			    // setup item, it won't hurt to set the SetupCharged value
			    // to 1.  This way, should we add or change the setup charge
			    // on this package in the future, they won't get charged for it
			    // 6 months down the road...
			    SDB.setValue("SetupCharged", 1);
			}
			
			// Don't update while we're testing...
			SDB.upd();
			
			// Now, go through each of the billables we have for this item, 
			// and put in statement charges for them as well with a zero charge
			// and reference the package.
			DB2.query("select InternalID from Subscriptions where CustomerID = %ld and ParentID = %ld", getLong("CustomerID"), atol(DB.curRow[0]));
			if (DB2.rowCount) while (DB2.getrow()) {
				AcctsRecv		AR2;
				#ifdef DBDEBUG
				printf("Doing Subscription ID %ld", atol(DB2.curRow["InternalID"]));
				#endif
				SDB.get(atol(DB2.curRow["InternalID"]));

	    		// Get the charge date.
                // We don't want to recalculate any start or end dates if
                // it is a package item.  We want it to match the package.
                /*
	    		if (!strcmp("0000-00-00", SDB.getStr("EndsOn"))) SDB.setValue("EndsOn", Yesterday);
	            myDatetoQDate(SDB.getStr("EndsOn"), &TmpDate);
	            ChargeDate = TmpDate.addDays(1);
	            QDatetomyDate(sChargeDate, ChargeDate);
	            DaysLeft   = ChargeDate.daysTo(CycleEnd);
                */

				AR2.ARDB->setValue("CustomerID", getLong("CustomerID"));
				AR2.ARDB->setValue("LoginID", SDB.getStr("LoginID"));
				AR2.ARDB->setValue("ItemID", SDB.getLong("ItemNumber"));
	    		AR2.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
		    	AR2.ARDB->setValue("RatePlan", getLong("RatePlan"));
				AR2.ARDB->setValue("TransDate", sChargeDate);
				AR2.ARDB->setValue("StartDate", sChargeDate);
				AR2.ARDB->setValue("EndDate", sCycleEnd);
				// Get the quantity
				if (SDB.getFloat("Quantity") == (float) 0.00) {
				    SDB.setValue("Quantity", (float) 1.00);
				}
				if (ChargeDate.day() == CycleStart.day()) {
					Quantity = 1.00 * SDB.getFloat("Quantity");
				} else {
	    			Quantity = (float)(((float) DaysLeft / (float) CycleDays) * SDB.getFloat("Quantity"));
				}
				#ifdef DBDEBUG
	    		printf("Quantity = %0.2f, DaysLeft = %d, CycleDays = %d, SDB.Quantity = %f\n", Quantity, DaysLeft, CycleDays, SDB.getFloat("Quantity"));
				#endif
				// The price is zero because it is part of the package.
				AR2.ARDB->setValue("Memo", SDB.getStr("ItemDesc"));
				if (Quantity < 1.00) {
					AR2.ARDB->appendStr("Memo", " (Prorated)");
				}
				AR2.ARDB->appendStr("Memo", " (Included with ");
				AR2.ARDB->appendStr("Memo", PDB.getStr("Description"));
				AR2.ARDB->appendStr("Memo", ")");
				AR2.ARDB->setValue("Price", (float) 0.00);
				AR2.ARDB->setValue("Quantity", Quantity);
				AR2.ARDB->setValue("Amount", (float) 0.00);

                // Only insert the item if we are set up to show the
                // included items.
                if (PDB.getInt("ShowIncluded")) {
				    AR2.SaveTrans();
                }
				// GrandTotal += AR2.ARDB->Amount.toFloat();
				
				SDB.setValue("LastDate", Today);
				SDB.setValue("EndsOn", sCycleEnd);

	            // Check for setup charges...
				if (!SDB.getInt("SetupCharged")) {
				    // Nope.  Lets charge them now...
				    // We've got the setup Item to charge them already in memory...
		            BillablesDataDB     BDDB;
		            BDB.get(SDB.getLong("ItemNumber"));
		            // Now that we've got the subscription item in memory, check for
		            // a setup item.
		            if (BDB.getLong("SetupItem")) {
		                // Yes, there is a setup item.  Now load that one.
			            BDDB.getItem(BDB.getLong("SetupItem"), getLong("RatePlan"), getLong("BillingCycle"));
			            
			            // Okay, we've got everything we need, setup the AR entry.
			            AcctsRecv       AR2;
						AR2.ARDB->setValue("CustomerID", getLong("CustomerID"));
						AR2.ARDB->setValue("LoginID", SDB.getStr("LoginID"));
						AR2.ARDB->setValue("ItemID", BDDB.getLong("ItemNumber"));
	        	    	AR2.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
		        	    AR2.ARDB->setValue("RatePlan", getLong("RatePlan"));
						AR2.ARDB->setValue("TransDate", sChargeDate);
						AR2.ARDB->setValue("StartDate", sChargeDate);
						AR2.ARDB->setValue("EndDate", sChargeDate);
						AR2.ARDB->setValue("Memo", BDB.getStr("Description"));
						AR2.ARDB->appendStr("Memo", " (Included with ");
						AR2.ARDB->appendStr("Memo", PDB.getStr("Description"));
						AR2.ARDB->appendStr("Memo", ")");
						AR2.ARDB->setValue("Price", (float) 0.00);
						AR2.ARDB->setValue("Quantity", Quantity);
						AR2.ARDB->setValue("Amount", (float) 0.00);
                        if (PDB.getInt("ShowIncluded")) {
                            AR2.SaveTrans();
                        }
					}
					// Whether or not there was a setup charge, we want
					// to update the subscription item saying that we
					// charged it.
					SDB.setValue("SetupCharged", (int) 1);
	            }
				// Don't update while we're testing...
				SDB.upd();
			}
        }
	}


    // Now, do the non-package subscription items.
	DB.query("select Subscriptions.InternalID, Billables.Priority from Subscriptions, Billables where Subscriptions.ItemNumber = Billables.ItemNumber and Subscriptions.CustomerID = %ld and Subscriptions.Active = 1 and Subscriptions.PackageNo = 0 and Subscriptions.ParentID = 0 and Subscriptions.EndsOn < '%s' order by Billables.Priority", getLong("CustomerID"), Today);
	#ifdef DBDEBUG
	printf("%ld Subscription Items found.\n", DB.rowCount);
	#endif
	if (DB.rowCount) {
		#ifdef DBDEBUG
		printf("***Found One***\n");
		#endif
		// We have some, start processing them one by one.
		while (DB.getrow()) {
			AcctsRecv		AR;
			#ifdef DBDEBUG
			printf("Doing Subscription ID %ld", atol(DB.curRow["InternalID"]));
			#endif
			SDB.get(atol(DB.curRow["InternalID"]));
			
			// Check the ends on date, and make sure that it is not in the
			// future.  If it is, then continue;
            if (!strcmp("0000-00-00", SDB.getStr("EndsOn"))) SDB.setValue("EndsOn", Yesterday);
            myDatetoQDate(SDB.getStr("EndsOn"), &TmpDate);
			if (TmpDate >= CurrentDate) continue; 

    		// Get the charge date.
    		// if (!strcmp("0000-00-00", SDB.getStr("EndsOn"))) SDB.setValue("EndsOn", Yesterday);
            // myDatetoQDate(SDB.getStr("EndsOn"), &TmpDate);
            ChargeDate = TmpDate.addDays(1);
            QDatetomyDate(sChargeDate, ChargeDate);
            DaysLeft   = ChargeDate.daysTo(CycleEnd);

			AR.ARDB->setValue("CustomerID", getLong("CustomerID"));
			AR.ARDB->setValue("LoginID", SDB.getStr("LoginID"));
			AR.ARDB->setValue("ItemID", (long) 0);
   	    	AR.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
       	    AR.ARDB->setValue("RatePlan", getLong("RatePlan"));
			AR.ARDB->setValue("TransDate", sChargeDate);
			AR.ARDB->setValue("StartDate", sChargeDate);
			AR.ARDB->setValue("EndDate", sCycleEnd);
			// Get the quantity
			if (SDB.getFloat("Quantity") == (float) 0.00) {
			    SDB.setValue("Quantity", (float) 1.00);
			}
			if (ChargeDate.day() == CycleStart.day()) {
				Quantity = 1.00 * SDB.getFloat("Quantity");
			} else {
    			Quantity = (float)(((float) DaysLeft / (float) CycleDays) * SDB.getFloat("Quantity"));
			}
			#ifdef DBDEBUG
    		printf("Quantity = %0.2f, DaysLeft = %d, CycleDays = %d, SDB.Quantity = %f\n", Quantity, DaysLeft, CycleDays, SDB.getFloat("Quantity"));
			#endif
			if (SDB.getInt("AutoPrice")) {
				// The price follows the Billable, so load the Billable
				BDB.get(SDB.getLong("ItemNumber"));
				AR.ARDB->setValue("Memo", BDB.getStr("Description"));
				BDDB.getItem(SDB.getLong("ItemNumber"), getLong("RatePlan"), getLong("BillingCycle"));
				if (strlen(BDDB.getStr("Description"))) {
					AR.ARDB->appendStr("Memo", BDDB.getStr("Description"));
				}
				if (Quantity < 1.00) {
					AR.ARDB->appendStr("Memo", " (Prorated)");
				}
				AR.ARDB->setValue("ItemID", SDB.getLong("ItemNumber"));
				// Check to see if the priority of this item is lower, if 
				// it is not, then use the Secondary Price.  If the
				// priority is equal, we charge the same.
				if (atoi(DB.curRow["Priority"]) <= SubscrPriority) {
					thePrice = BDDB.getFloat("Price");
					isSecondary = 0;
					SubscrPriority = atoi(DB.curRow[1]) - 1;
				} else {
					thePrice = BDDB.getFloat("SecondaryPrice");
					isSecondary = 1;
				}
				AR.ARDB->setValue("Price", thePrice);
				AR.ARDB->setValue("Quantity", Quantity);
				AR.ARDB->setValue("Amount", (float) Quantity * thePrice);
			} else {
				// The price does not follow the billable, use the
				// subscription information.
				AR.ARDB->setValue("Memo", SDB.getStr("ItemDesc"));
				if (Quantity < 1.00) {
					AR.ARDB->appendStr("Memo", " (Prorated)");
				}
				AR.ARDB->setValue("Price", SDB.getFloat("Price"));
				AR.ARDB->setValue("Quantity", Quantity);
				AR.ARDB->setValue("Amount", Quantity * SDB.getFloat("Price"));
			}
			AR.SaveTrans();
			// GrandTotal += AR.ARDB->Amount.toFloat();
			
			SDB.setValue("LastDate", Today);
			SDB.setValue("EndsOn", sCycleEnd);

            // Check for setup charges...
			if (!SDB.getInt("SetupCharged")) {
			    // Nope.  Lets charge them now...
			    // We've got the setup Item to charge them already in memory...
	            BillablesDataDB     BDDB;
	            BDB.get(SDB.getLong("ItemNumber"));
	            // Now that we've got the subscription item in memory, check for
	            // a setup item.
	            if (BDB.getLong("SetupItem")) {
	                // Yes, there is a setup item.  Now load that one.
	                BillablesDB BDB2;
	                BDB2.get(BDB.getLong("SetupItem"));
		            BDDB.getItem(BDB.getLong("SetupItem"), getLong("RatePlan"), getLong("BillingCycle"));
		            
		            // float   prevSetup = 0.00;
		            
		            // Check for the setup charge type...
		            switch (BDB2.getInt("ItemType")) {
		                case 2:         // Merged setup
		                    #if 0
		                    BlargDB tmpDB;
		                    char    tmpSt[1024];
		                    char    dayGrace[64];
		                    QDate   tmpDate;
		                    QDate   setDate;
		                    tmpDate = QDate::currentDate();
		                    setDate = tmpDate.addDays(-90);
		                    QDatetomyDate(dayGrace, setDate);

                            // Okay, lets generate a list of Item ID's that
                            // are "Merged" setup charges.
                            QStringList    tmpList;
                            tmpList.setAutoDelete(TRUE);
                            tmpDB.query("select ItemNumber from Billables where ItemType = 2");
                            if (DB.rowCount) {
                                while (DB.getrow()) {
                                    tmpList.append(DB.curRow[0]);
                                }
                                // Now, put this list into something we can use.
                                
                            }
                            #endif
		                    break;
		                    
		                default:
		                    break;
		            }
		            
		            // Check for the secondary price on the setup charge.
					if (!isSecondary) {
						thePrice = BDDB.getFloat("Price");
					} else {
						thePrice = BDDB.getFloat("SecondaryPrice");
					}
		            // Okay, we've got everything we need, setup the AR entry.
		            AcctsRecv       AR2;
					AR2.ARDB->setValue("CustomerID", getLong("CustomerID"));
					AR2.ARDB->setValue("LoginID", SDB.getStr("LoginID"));
					AR2.ARDB->setValue("ItemID", BDDB.getLong("ItemNumber"));
           	    	AR2.ARDB->setValue("BillingCycle", getLong("BillingCycle"));
       	            AR2.ARDB->setValue("RatePlan", getLong("RatePlan"));
					AR2.ARDB->setValue("TransDate", sChargeDate);
					AR2.ARDB->setValue("StartDate", sChargeDate);
					AR2.ARDB->setValue("EndDate", sChargeDate);
					AR2.ARDB->setValue("Memo", BDB2.getStr("Description"));
					AR2.ARDB->appendStr("Memo", " ");
					AR2.ARDB->appendStr("Memo", BDDB.getStr("Description"));
					AR2.ARDB->setValue("Price", thePrice);
					AR2.ARDB->setValue("Quantity", (float) 1.00);
					AR2.ARDB->setValue("Amount", thePrice);
					AR2.SaveTrans();
				}
				// Whether or not there was a setup charge, we want
				// to update the subscription item saying that we
				// charged it.
				SDB.setValue("SetupCharged", 1);
            }

			// Don't update while we're testing...
			SDB.upd();
		}
	}

	#ifdef DBDEBUG
	printf("\n");
	#endif
	return(RetVal);
}



/*
*************************************************************************
**
** AddressesDB - generic routines for reading & updating the
**               Customers table.
**
************************************************************************
*/

AddressesDB::AddressesDB(void)
{
	InternalID			= 0;
	RefFrom				= "";
	RefID				= "";
	Tag					= "";
	International		= "";
	Address1			= "";
	Address2			= "";
	City				= "";
	State				= "";
	ZIP					= "";
	Country				= "";
	PostalCode			= "";
	Active				= "";
	LastModifiedBy		= "";
	LastModified		= "";
}

AddressesDB::~AddressesDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified CustomerID.
//
// Args        - int CustID - the CustomerID of the record
//
// Returns     - 0 on failure, 1 on success
//

int AddressesDB::get(int qRefFrom, long qRefID, const char * qTag)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.query("select * from Addresses where RefFrom = %d and RefID = %ld and Tag = '%s'", qRefFrom, qRefID, qTag);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID			= atoi(DB.curRow["InternalID"]);
        RefFrom				= DB.curRow["RefFrom"];
        RefID				= DB.curRow["RefID"];
        Tag					= DB.curRow["Tag"];
        International		= DB.curRow["International"];
        Address1			= DB.curRow["Address1"];
        Address2			= DB.curRow["Address2"];
        City				= DB.curRow["City"];
        State				= DB.curRow["State"];
        ZIP					= DB.curRow["ZIP"];
        Country				= DB.curRow["Country"];
        PostalCode			= DB.curRow["PostalCode"];
        Active				= DB.curRow["Active"];
        LastModifiedBy		= DB.curRow["LastModifiedBy"];
        LastModified		= DB.curRow["LastModified"];

        Ret = 1;
    }
    return(Ret);
}

//
// getByID()   - Gets a record from the database using the
//               specified InternalID.
//
// Args        - int CustID - the CustomerID of the record
//
// Returns     - 0 on failure, 1 on success
//

int AddressesDB::getByID(long intID)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.query("select * from Addresses where InternalID = %ld", intID);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID			= atoi(DB.curRow["InternalID"]);
        RefFrom				= DB.curRow["RefFrom"];
        RefID				= DB.curRow["RefID"];
        Tag					= DB.curRow["Tag"];
        International		= DB.curRow["International"];
        Address1			= DB.curRow["Address1"];
        Address2			= DB.curRow["Address2"];
        City				= DB.curRow["City"];
        State				= DB.curRow["State"];
        ZIP					= DB.curRow["ZIP"];
        Country				= DB.curRow["Country"];
        PostalCode			= DB.curRow["PostalCode"];
        Active				= DB.curRow["Active"];
        LastModifiedBy		= DB.curRow["LastModifiedBy"];
        LastModified		= DB.curRow["LastModified"];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Customer ID
//

int AddressesDB::ins(void)
{
	int 	Ret = 0;
	char 	tag[128];
	char	address1[128];
	char	address2[128];
	char	city[128];
	char	state[128];
	char	zip[128];
	char	country[128];
	char	postalCode[128];
	char	lastModifiedBy[128];
	
	char	query[1024];
	BlargDB	DB;
	
	strcpy(tag, DB.escapeString(Tag));
	strcpy(address1, DB.escapeString(Address1));
	strcpy(address2, DB.escapeString(Address2));
	strcpy(city, DB.escapeString(City));
	strcpy(state, DB.escapeString(State));
	strcpy(zip, DB.escapeString(ZIP));
	strcpy(country, DB.escapeString(Country));
	strcpy(postalCode, DB.escapeString(PostalCode));
	strcpy(lastModifiedBy, DB.escapeString(LastModifiedBy));

	sprintf(query, "insert into Addresses values (0, %d, %ld, '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', NULL)",
	  RefFrom.toInt(),
	  RefID.toLong(),
	  tag,
	  International.toInt(),
	  address1,
	  address2,
	  city,
	  state,
	  zip,
	  country,
	  postalCode,
	  Active.toInt(),
	  lastModifiedBy
	);
	
	Ret = DB.dbcmd(query);

    return(Ret);
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int AddressesDB::upd(void)
{
    int         Ret = 0;
	ADB	        DB;
    ADBTable    addrDB("Addresses");
	
	// Load the old record from the database so we can do a more 
	// intelligent update than all of the fields.
	DB.query("select * from Addresses where InternalID = %ld", InternalID);
	if (DB.rowCount != 1) return 1;

    addrDB.get(InternalID);
    addrDB.setValue("Tag",              (const char *) Tag);
    addrDB.setValue("International",    International.toInt());
    addrDB.setValue("Address1",         (const char *) Address1);
    addrDB.setValue("Address2",         (const char *) Address2);
    addrDB.setValue("City",             (const char *) City);
    addrDB.setValue("State",            (const char *) State);
    addrDB.setValue("ZIP",              (const char *) ZIP);
    addrDB.setValue("Country",          (const char *) Country);
    addrDB.setValue("PostalCode",       (const char *) PostalCode);
    addrDB.setValue("Active",           Active.toInt());
    addrDB.setValue("LastModifiedBy",   curUser().userName);
    addrDB.upd();

	return Ret;
}

//
// del()       - Deletes a record in the database.
//
// Args        - The CustomerID to delete.
//
// Returns     - -1 on failure, 0 on success
//

int AddressesDB::del(int qRefFrom, long qRefID)
{
    int     Ret = 0;
	ADB     DB;
    
    DB.dbcmd("delete from Addresses where RefFrom = %d and RefID = %ld", qRefFrom, qRefID);
    
    return Ret;
}

/*
*************************************************************************
**
** PhoneNumbersDB - generic routines for reading & updating the
**               Customers table.
**
************************************************************************
*/

PhoneNumbersDB::PhoneNumbersDB(void)
{
	InternalID			= 0;
	RefFrom				= "";
	RefID				= "";
	Tag					= "";
	International		= "";
	PhoneNumber			= "";
	Active				= "";
	LastModifiedBy		= "";
	LastModified		= "";
}

PhoneNumbersDB::~PhoneNumbersDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified CustomerID.
//
// Args        - int CustID - the CustomerID of the record
//
// Returns     - 0 on failure, 1 on success
//

int PhoneNumbersDB::get(int qRefFrom, long qRefID, const char * qTag)
{
    int     Ret = 0;
	BlargDB	DB;
    
    DB.query("select * from PhoneNumbers where RefFrom = %d and RefID = %ld and Tag = '%s'", qRefFrom, qRefID, qTag);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID			= atoi(DB.curRow[0]);
        RefFrom				= DB.curRow[1];
        RefID				= DB.curRow[2];
        Tag					= DB.curRow[3];
        International		= DB.curRow[4];
        PhoneNumber			= DB.curRow[5];
        Active				= DB.curRow[6];
        LastModifiedBy		= DB.curRow[7];
        LastModified		= DB.curRow[8];

        Ret = 1;
    }
    return(Ret);
}

//
// getByID()   - Gets a record from the database using the
//               specified InternalID.
//
// Args        - int InternalID - the InternalID of the record
//
// Returns     - 0 on failure, 1 on success
//

int PhoneNumbersDB::getByID(long intID)
{
    int     Ret = 0;
	BlargDB	DB;
    
    DB.query("select * from PhoneNumbers where InternalID = %ld", intID);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        
        InternalID			= atoi(DB.curRow[0]);
        RefFrom				= DB.curRow[1];
        RefID				= DB.curRow[2];
        Tag					= DB.curRow[3];
        International		= DB.curRow[4];
        PhoneNumber			= DB.curRow[5];
        Active				= DB.curRow[6];
        LastModifiedBy		= DB.curRow[7];
        LastModified		= DB.curRow[8];

        Ret = 1;
    }
    return(Ret);
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Customer ID
//

int PhoneNumbersDB::ins(void)
{
	int 	Ret = 0;
	char 	tag[128];
	char	phoneNumber[128];
	char	lastModifiedBy[128];
	
	char	query[1024];
	BlargDB	DB;
	
	strcpy(tag, DB.escapeString(Tag));
	strcpy(phoneNumber, DB.escapeString(PhoneNumber));
	strcpy(lastModifiedBy, DB.escapeString(LastModifiedBy));

	sprintf(query, "insert into PhoneNumbers values (0, %d, %ld, '%s', %d, '%s', %d, '%s', NULL)",
	  RefFrom.toInt(),
	  RefID.toLong(),
	  tag,
	  International.toInt(),
	  phoneNumber,
	  Active.toInt(),
	  lastModifiedBy
	);
	
	Ret = DB.dbcmd(query);
	InternalID = Ret;

    return(Ret);
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

int PhoneNumbersDB::upd(void)
{
    int     Ret = 0;

	QStrList Updates(TRUE);
	QStrList Audits(TRUE);
	QString qquery;
	char    query[2048];
	BlargDB	DB;
	
	// Load the old record from the database so we can do a more 
	// intelligent update than all of the fields.
	DB.query("select * from PhoneNumbers where InternalID = %ld", InternalID);
	if (DB.rowCount != 1) return(1);
	
	DB.getrow();
	
	qquery = "";
	
	int curCol = 3;
	// Now, go through through each column, one by one and compare them.
	if (strcmp(DB.curRow[curCol], Tag)) {
		sprintf(query, "Tag = '%s'", (const char *) DB.escapeString(Tag));
		Updates.append(query);
	}
	curCol++;

	if (atoi(DB.curRow[curCol]) != International.toInt()) {
		sprintf(query, "International = %d", International.toInt());
		Updates.append(query);
	}
	curCol++;

	if (strcmp(DB.curRow[curCol], PhoneNumber)) {
		sprintf(query, "PhoneNumber = '%s'", (const char *) DB.escapeString(PhoneNumber));
		Updates.append(query);
	}
	curCol++;

	if (atoi(DB.curRow[curCol]) != Active.toInt()) {
		sprintf(query, "Active = %d", Active.toInt());
		Updates.append(query);
	}
	curCol++;

	// Check our list to see if there are any updates.
	if (!Updates.count()) return(Ret);

	// Since we've updated things, get the user name and set them to be
	// The last one that modified it.	
	sprintf(query, "LastModifiedBy = '%s'", curUser().userName);
	Updates.append(query);

	// Now that we've got all the fields that need updating, put them all
	// into one giant string.
	
	qquery = "update PhoneNumbers set ";
	for (uint i = 1; i < Updates.count(); i++) {
		qquery.append(Updates.at(i));
		qquery.append(", ");
	}
	qquery.append(Updates.at(0));

	sprintf(query, " where InternalID = %ld", InternalID);
	qquery.append(query);
	strcpy(query, qquery);
	DB.dbcmd(query);
	
	return(Ret);
}

//
// del()       - Deletes a record in the database.
//
// Args        - The CustomerID to delete.
//
// Returns     - -1 on failure, 0 on success
//

int PhoneNumbersDB::del(void)
{
    int  Ret = 0;
	BlargDB	DB;

	if (InternalID) {
	    DB.dbcmd("delete from PhoneNumbers where InternalID = %ld", InternalID);
    }
    
    return(Ret);
}



/*
*************************************************************************
**
** VendorsDB - generic routines for reading & updating the
**               Vendors table.
**
************************************************************************
*/

VendorsDB::VendorsDB(void)
{
    setTableName("Vendors");
}

VendorsDB::~VendorsDB()
{
}

/*
*************************************************************************
**
** SubscriptionsDB - generic routines for reading & updating the
**               Subscriptions table.
**
************************************************************************
*/

SubscriptionsDB::SubscriptionsDB(void)
{
    setTableName("Subscriptions");
}

SubscriptionsDB::~SubscriptionsDB()
{
}

//
// postIns()   - Takes care of things after a row has been inserted.
//
// Args        - None.
//
// Returns     - the new Internal ID
//

void SubscriptionsDB::postIns(void)
{
    packageScan();
}

//
// postUpd()   - Takes care of things after a row has been updated.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

void SubscriptionsDB::postUpd(void)
{
    packageScan();
}

//
// postDel()   - Takes care of things after a row has been deleted.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

void SubscriptionsDB::postDel(void)
{
    packageScan();
}

/*
** packageScan   - When adding or deleting a subscription item of any kind,
**                 we will want to scan through the list and "link"
**                 normal subscription items with packages, or unlink them
**                 depending...
*/

void SubscriptionsDB::packageScan(void)
{
    // A simple structure for maintaining the list of linked package info.
    
    QList<PackageItem> PItems;
    PItems.setAutoDelete(TRUE);
    PackageItem *curItem;
    
    BlargDB     DB;
    BlargDB     DB2;
    
    // First, do a simple query to update all non-package items so they
    // are not associated with a package.
    DB.dbcmd("update Subscriptions set ParentID = 0 where CustomerID = %ld", getLong("CustomerID"));

    // Now, we want to get a list of the customer's packages (if any)
    DB.query("select InternalID, PackageNo, LoginID from Subscriptions where CustomerID = %ld and Active <> 0 and PackageNo <> 0", getLong("CustomerID"));
    if (DB.rowCount) {
        // Okay, we have a list of subscribed-to packages.  Load all of the
        // individual items that this package covers into our list.
        while (DB.getrow()) {
            DB2.query("select Quantity, ItemNumber, LoginGroup from PackageContents where PackageID = %d", atoi(DB.curRow[1]));
            if (DB2.rowCount) while (DB2.getrow()) {
                // Insert one item into the list for each quantity.
                for (int i = 1; i <= atoi(DB2.curRow[0]); i++) {
                    curItem = new(PackageItem);
                    curItem->InternalID = atol(DB.curRow[0]);
                    curItem->ItemNumber = atol(DB2.curRow[1]);
                    curItem->LoginGroup = atoi(DB2.curRow[2]);
                    strcpy(curItem->LoginID, DB.curRow[2]);
                    PItems.append(curItem);
                }
            }
        }
        
        // Now that we have our list of package items, go through the
        // subscription list and associate them with packages.
        DB2.query("select InternalID, ItemNumber, Quantity, LoginID from Subscriptions where CustomerID = %ld and Active <> 0 and PackageNo = 0", getLong("CustomerID"));
        if (DB2.rowCount) while (DB2.getrow()) {
            // Scan through our package list and see this item matches
            // one of the included package items.
            int looping = 1;
            int doUpdate = 0;
            unsigned int tmpCur  = 0;
            while (looping) {
                doUpdate = 0;
                if (!PItems.count()) looping = 0;
                else {
                    curItem = PItems.at(tmpCur);
                    if (curItem->ItemNumber == atol(DB2.curRow[1])) {
                        // Found a match.  
                        // Check the login group to make sure that we can
                        // include this one...
                        
                        // Check for the primary login group first.
                        if (curItem->LoginGroup == 1) {
                            if (!strcmp(DB2.curRow[3], curItem->LoginID)) {
                                // We have a matching login ID.  Update the
                                // subscription record.
                                doUpdate = 1;
                            }
                        } else {
                            // Fall through.
                            // If the LoginGroup == 0, any login ID will do.
                            doUpdate = 1;
                        }
                        
                        if (doUpdate) {
	                        // Update it in the database.
	                        DB.dbcmd("update Subscriptions set ParentID = %ld where InternalID = %ld", curItem->InternalID, atol(DB2.curRow[0]));
	                        PItems.remove(tmpCur);
	                        looping = 0;
	                        doUpdate = 0;
                        } else {
                            // Wrong Login ID for this group, keep
                            // searching...
	                        tmpCur++;
	                        if (tmpCur == PItems.count()) looping = 0;
                        }
                        
                    } else {
                        // No match, check to see if we're done looping.
                        tmpCur++;
                        if (tmpCur == PItems.count()) looping = 0;
                    }
                }
            }
        }
        
        
        // Finally, free up whatever remaining memory is used by the list.
        while (PItems.count()) PItems.removeLast();
    }

}

/*
*************************************************************************
**
** NotesDB - generic routines for reading & updating the
**           Notes table.
**
************************************************************************
*/

NotesDB::NotesDB(void)
{
    setTableName("Notes");
	QDateTime	tmpDate;
	char		tmpstr[512];

    // Set the DateStamp to be today by default.
    tmpDate = QDateTime::currentDateTime();
    sprintf(tmpstr, "%04d-%02d-%02d %02d:%02d:%02d", 
      tmpDate.date().year(), tmpDate.date().month(), tmpDate.date().day(),
      tmpDate.time().hour(), tmpDate.time().minute(),tmpDate.time().second()
    );
    setValue("NoteDate", tmpstr);
    setValue("AddedBy", curUser().userName);
}

NotesDB::~NotesDB()
{
}



/*
*************************************************************************
**
** LoginsDB - generic routines for reading & updating the
**            Logins table.
**
************************************************************************
*/

LoginsDB::LoginsDB(void)
{
    setTableName("Logins");
}

LoginsDB::~LoginsDB()
{
}

//
// get()       - Gets a record from the database using the
//               specified InternalID.
//
// Args        - int IntID - the InternalID of the record
//
// Returns     - 0 on failure, 1 on success
//

long LoginsDB::get(long CustID, const char * loginID)
{
    int     Ret = 0;
	BlargDB	DB;
    
    DB.query("select InternalID from Logins where CustomerID = %ld and LoginID = '%s'", CustID, loginID);
    
    if (DB.rowCount) {
        DB.getrow();
        
        Ret = ADBTable::get(atol(DB.curRow[0]));
    }

    //fprintf(stderr, "LoginsDB::get(long, char *), Current Login Type = %d, Old Login Type = %d\n", getInt("LoginType"), getInt("LoginType", 1));
    return(Ret);
}


void LoginsDB::postUpd(void)
{

    //fprintf(stderr, "LoginsDB::postUpd() called.\n");
    //fprintf(stderr, "LoginsDB::postUpd(), Current Login Type = %d, Old Login Type = %d\n", getInt("LoginType"), getInt("LoginType", 1));
    if (getInt("LoginType", 0) != getInt("LoginType", 1)) {
    	updateARForTypeChg();
    	updateSubscriptions(getLong("LoginType", 1), getLong("LoginType", 0));
    }
    
    // Update the customer active flags.
    ADB tmpDB;
    tmpDB.query("select LoginID from Logins where CustomerID = %ld and Active > 0", getLong("CustomerID")); 
    int ActCount = tmpDB.rowCount;
    tmpDB.dbcmd("update Customers set Active = %d where CustomerID = %ld", ActCount, getLong("CustomerID"));
}

void LoginsDB::postIns(void)
{
	addSubscriptions();

    // Update the customer active flags.
    ADB tmpDB;
    tmpDB.query("select LoginID from Logins where CustomerID = %ld and Active > 0", getLong("CustomerID")); 
    int ActCount = tmpDB.rowCount;
    tmpDB.dbcmd("update Customers set Active = %d where CustomerID = %ld", ActCount, getLong("CustomerID"));
}

void LoginsDB::del(long CustID, const char * loginID)
{
	BlargDB	DB;
    
    DB.query("select InternalID from Logins where CustomerID = %ld and LoginID = '%s'", CustID, loginID);
    
    if (DB.rowCount) {
        DB.getrow();
        
        ADBTable::del(atol(DB.curRow[0]));
    }
}

/*
** addSubscriptions - A function that gets called when a login record
**                    is inserted.  This is done automatically.  If 
**                    it needs to be called manually, the record should
**                    be loaded first, so the CustomerID, LoginID and 
**                    account type are in place.
*/

void LoginsDB::addSubscriptions(void)
{
	BlargDB			DB1;
	BlargDB			DB2;
	SubscriptionsDB	SDB;
	BillablesDB		BDB;
	BillablesDataDB BDDB;
	CustomersDB     CDB;
    QDate           today;
    char            stoday[64];
    QDate           yesterday;
	char            lastDate[64];
	char            endsOn[64];
	
    today   = QDate::currentDate();
    yesterday = today.addDays(-1);
    QDatetomyDate(stoday, today);
    QDatetomyDate(lastDate, yesterday);
    QDatetomyDate(endsOn, yesterday);
	
	// Now, get the associated subscription items for the new LoginType.
	DB1.query("select ItemNumber from LoginTypeBillables where LoginTypeID = %d", getLong("LoginType"));
	while (DB1.getrow()) {
		// Load the Billable into memory
		BDB.get(atol(DB1.curRow[0]));
		// If it is a subscription item, add it into the subscriptions.
		if (BDB.getInt("ItemType") == 1) {
		    // Get the LastDate and EndsOn dates to check for trial periods.
    	    CDB.get(getLong("CustomerID"));
    	    BDDB.getItem(BDB.getLong("ItemNumber"), CDB.getLong("RatePlan"), CDB.getLong("BillingCycle"));
		    if (BDDB.getInt("TrialPeriod")) {
                // There is a trial period involved.  Check to see if they
                // are still eligible.
                QDate      tmpDate1;
                QDate      tmpDate2;
                
                if (!strcmp("0000-00-00", CDB.getStr("AccountOpened"))) CDB.setValue("AccountOpened", stoday);
                myDatetoQDate((const char *) CDB.getStr("AccountOpened"), &tmpDate1);
                tmpDate2 = tmpDate1.addDays(BDDB.getInt("TrialPeriod"));
                if (tmpDate2 >= today) {
                    // They are still eligible for a trial.
                    QDatetomyDate(lastDate, today);
                    QDatetomyDate(endsOn, tmpDate2);
                }
	    	}
		    
		    
			// Setup the subscription data
			SDB.setValue("CustomerID", getLong("CustomerID"));
			SDB.setValue("LoginID", getStr("LoginID"));
			SDB.setValue("Active", getInt("Active"));
			SDB.setValue("PackageNo", (long) 0);
			SDB.setValue("ItemNumber", BDB.getLong("ItemNumber"));
			SDB.setValue("Quantity", (float) 1.0);
			SDB.setValue("ItemDesc", BDB.getStr("Description"));
			SDB.setValue("Price", (float) 0.0);
			SDB.setValue("LastDate", lastDate);
			SDB.setValue("EndsOn", endsOn);
			SDB.setValue("AutoRenew", (int) 1);
			SDB.setValue("AutoPrice", (int) 1);
			SDB.ins();
		}
	}
}

/*
** updateSubscriptions - A private function that gets called when a login
**                       type has changed.  It handles the updates for the
**                       subscriptions by removing the old subscriptions
**                       and putting new ones in its place.  This is done
**                       automatically when upd() is called.
*/

void LoginsDB::updateSubscriptions(int oldType, int newType)
{
	ADB			    DB1;
	ADB	    		DB2;
	SubscriptionsDB	SDB;
	BillablesDB		BDB;
	char            yesterday[32];
	QDate           tmpDate;
	QDate           tmpDate2;
	
	tmpDate2 = QDate::currentDate();
	tmpDate  = tmpDate.addDays(-1);
	QDatetomyDate(yesterday, tmpDate);
	
	// The first thing we need to do is get the associated subscription
	// billables from the LoginTypeBillables table.
	DB1.query("select ItemNumber from LoginTypeBillables where LoginTypeID = %d", oldType);
	while (DB1.getrow()) {
		// Now, get any matches from the subscriptions database.
		DB2.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s' and ItemNumber = %ld",
		  getLong("CustomerID"),
		  (const char *) getStr("LoginID"),
		  atol(DB1.curRow[0])
		);
		if (DB2.rowCount) {
			DB2.getrow();
			// We have a match in the subscriptions database.  Delete it.
			SDB.del(atoi(DB2.curRow[0]));
		}
	}
	
	// Now, get the associated subscription items for the new LoginType.
	DB1.query("select ItemNumber from LoginTypeBillables where LoginTypeID = %d", newType);
	while (DB1.getrow()) {
		// Load the Billable into memory
		BDB.get(atol(DB1.curRow[0]));
		// If it is a subscription item, add it into the subscriptions.
		if (BDB.getInt("ItemType") == 1) {
			// Setup the subscription data
			SDB.setValue("CustomerID", getLong("CustomerID"));
			SDB.setValue("LoginID", getStr("LoginID"));
			SDB.setValue("Active", getInt("Active"));
			SDB.setValue("PackageNo", (long) 0);
			SDB.setValue("ItemNumber", BDB.getLong("ItemNumber"));
			SDB.setValue("Quantity", (float) 1.0);
			SDB.setValue("ItemDesc", BDB.getStr("Description"));
			SDB.setValue("Price", (float) 0.0);
			SDB.setValue("LastDate", yesterday);
			SDB.setValue("EndsOn", yesterday);
			SDB.setValue("AutoRenew", (int) 1);
			SDB.setValue("AutoPrice", (int) 1);
			SDB.setValue("SetupCharged", (int) 1);
			SDB.ins();
		}
	}	
}

/*
** updateARForTypeChg - This function searches through the AcctsRecv table
**                      for any billed items to the specified login ID that
**                      have a date range.  The EndDate must be > Today.
**                      Any transactions found will be copied (ItemID, etc)
**                      and a negative amount adjusting entry will be inserted
**                      into the customer register.  This will quickly pro-rate
**                      a balance for a customer that is changing their account
**                      type.
*/

void LoginsDB::updateARForTypeChg(void)
{
	BlargDB			DB;
	BlargDB			DB2;
	CustomersDB		CDB;
	QDate			tmpDate;
	QDate			CycleStart;
	QDate			CycleEnd;
	QDate			yesterday;
	char			sYesterday[64];
	int				cycleDays;
	int				daysLeft;
	char			today[64];
	char			sCycleEnd[64];
	float			Qty;
	BillingCyclesDB	BCDB;
	
	tmpDate = QDate::currentDate();
	sprintf(today, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
	
	CDB.get(getLong("CustomerID"));
	BCDB.get(CDB.getLong("BillingCycle"));
	BCDB.getCycleDates(&CycleStart, &CycleEnd, &cycleDays, &daysLeft);
	sprintf(sCycleEnd, "%04d-%02d-%02d", CycleEnd.year(), CycleEnd.month(), CycleEnd.day());
	
	// Okay, the first thing we want/need to do is load up the entries
	// in the AcctsRecv table that have an EndDate > Today.
	DB.query("select ItemID, Price, Amount, Memo, InternalID from AcctsRecv where CustomerID = %ld and LoginID = '%s' and EndDate > '%s'", 
	  getLong("CustomerID"),
	  (const char *) getStr("LoginID"),
	  today
	);
	if (DB.rowCount) {
		while (DB.getrow()) {
			// It looks as if we need to add to the balance of the user.
			AcctsRecv	AR;
			AR.ARDB->setValue("CustomerID", getLong("CustomerID"));
			AR.ARDB->setValue("LoginID", getStr("LoginID"));
			AR.ARDB->setValue("ItemID", atol(DB.curRow[0]));
			AR.ARDB->setValue("TransDate", today);
			AR.ARDB->setValue("StartDate", today);
			AR.ARDB->setValue("EndDate", today);  // End date has to be today or we'll pro-rate it again.
			Qty = (float) (daysLeft-1) / cycleDays;
			if ((float)atof(DB.curRow[2]) > 0) {
				Qty = Qty * -1.0;
			}
			AR.ARDB->setValue("Quantity", Qty);
			AR.ARDB->setValue("Price", DB.curRow[1]);
			AR.ARDB->setValue("Amount", (float)atof(DB.curRow[1]) * Qty);
			AR.ARDB->setValue("Memo", DB.curRow[3]);
			QString     tString;
			tString = AR.ARDB->getStr("Memo");
			if (tString.find('(')) {
			    tString.truncate(tString.find('(')-1);
			}
			AR.ARDB->setValue("Memo", (const char *) tString);
			AR.ARDB->appendStr("Memo", " (Adjustment for account type change)");
			AR.SaveTrans();
				
			DB2.dbcmd("update AcctsRecv set EndDate = '%s' where InternalID = %ld", today, atol(DB.curRow[4]));
		}
	
	} else {
		// There were no register entries.  So make sure there are
		// Subscriptions for the user.  If there aren't, add one.  If there
		// are, update all of the subscriptions for this user so the ends on
		// date are yesterday, and then do a subscription run for the 
		// customer.
		DB.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s'", getLong("CustomerID"), (const char *) getStr("LoginID"));
		if (DB.rowCount) {
			// They have subscriptions, so lets do a quick and dirty update
			// of them.
			tmpDate = QDate::currentDate();
			yesterday = tmpDate.addDays(-1);
			sprintf(sYesterday, "%04d-%02d-%02d", yesterday.year(), yesterday.month(), yesterday.day());
			DB.dbcmd("update Subscriptions set EndsOn = '%s' where CustomerID = %ld and LoginID = '%s'",
			  sYesterday,
			  getLong("CustomerID"),
			  (const char *) getStr("LoginID")
			);
			
		} else {
			// No subscriptions.  Add them.
			// LDB.get(myCustID, LoginID);
			addSubscriptions();
		}
	}
}

/*
** changeLoginID    - Takes the currently loaded LoginDB record, and changes
**                    the LoginID for it in all associated tables.  It should
**                    be called when wiping or transferring a login, or 
**                    changing a login ID, etc.
**
*/

void LoginsDB::changeLoginID(char * newLoginID)
{
    if (!getLong("CustomerID")) return;
    if (!strlen(getStr("LoginID"))) return;
    if (!strlen(newLoginID)) return;
    
    BlargDB     DB;
    char        tmpLogin[64];
    long        tmpCustID;
    
    strcpy(tmpLogin, getStr("LoginID"));
    tmpCustID   = getLong("CustomerID");
    
	// Now that we've "validated" the newLogin, update
	// _ALL_ of the tables with it...
	DB.dbcmd("update AcctsRecv set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update CCTrans set EnteredBy = '%s' where CustomerID = %ld and EnteredBy = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update Demographics set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update Domains set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update Logins set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update ModemUsage set LoginID = '%s' where LoginID = '%s'", newLoginID, tmpLogin);
	DB.dbcmd("update StatementsData set LoginID = '%s' where LoginID = '%s'", newLoginID, tmpLogin);
	DB.dbcmd("update Subscriptions set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update UserActivities set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	// DB.dbcmd("update UserNotes set LoginID = '%s' where CustomerID = %ld and LoginID = '%s'", newLoginID, tmpCustID, tmpLogin);
	DB.dbcmd("update ModemUsage set LoginID = '%s' where LoginID = '%s'", newLoginID, tmpLogin);
	DB.dbcmd("update Customers set PrimaryLogin = '%s' where CustomerID = %ld and PrimaryLogin = '%s'", newLoginID, tmpCustID, tmpLogin);

    // Now, update our own object to have the new Login ID in it.
    setValue("LoginID", newLoginID);	
}



/*
*************************************************************************
**
** DomainsDB - generic routines for reading & updating the
**             Domains table.
**
************************************************************************
*/

DomainsDB::DomainsDB(void)
{
    setTableName("Domains");
}

DomainsDB::~DomainsDB()
{
}

/*
** DomainsDB::postIns  - ADBTable calls the virtual function 'postIns' after
**                       inserting a row into the database.  This is where
**                       we call the subscription additions...
*/

void DomainsDB::postIns(void)
{
    // addSubscriptions();
}

/*
** DomainsDB::postUpd  - ADBTable calls the virtual function 'postUpd' after
**                       updating a row into the database.  This is where
**                       we call the subscription updates...
**
** This checks to see if a domain has just been released.  If it has, 
** we add the subscription items for it.
*/

void DomainsDB::postUpd(void)
{
    char    tmpst1[64];
    char    tmpst2[64];
    
    strcpy(tmpst1, (const char *) getStr("Released", 0));
    strcpy(tmpst2, (const char *) getStr("Released", 1));
    
    if (strlen(tmpst1) && !strlen(tmpst2)) {
        addSubscriptions();
    } else {
        updateSubscriptions(getInt("DomainType", 0), getInt("DomainType", 1));
    }
    
    //if (getInt("Active", 0) != getInt("Active", 1)) updateSubscriptions(getInt("DomainType", 0), getInt("DomainType", 1));
}


/*
** addSubscriptions - A function that gets called when a login record
**                    is inserted.  This is done automatically.  If 
**                    it needs to be called manually, the record should
**                    be loaded first, so the CustomerID, LoginID and 
**                    account type are in place.
*/

void DomainsDB::addSubscriptions()
{
	BlargDB			DB1;
	BlargDB			DB2;
	SubscriptionsDB	SDB;
	BillablesDB		BDB;
	BillablesDataDB BDDB;
	CustomersDB     CDB;
	char            lastDate[64];
	char            endsOn[64];
	
	strcpy(lastDate, "");
	strcpy(endsOn, "");
	
	// Now, get the associated subscription items for the new DomainType.
	DB1.query("select ItemNumber from DomainTypeBillables where DomainTypeID = %d", getInt("DomainType"));
	while (DB1.getrow()) {
		// Load the Billable into memory
		BDB.get(atol(DB1.curRow[0]));
		// If it is a subscription item, add it into the subscriptions.
		if (BDB.getInt("ItemType") == 1) {
		    // Get the LastDate and EndsOn dates to check for trial periods.
    	    CDB.get(getLong("CustomerID"));
    	    BDDB.getItem(BDB.getLong("ItemNumber"), CDB.getLong("RatePlan"), CDB.getLong("BillingCycle"));
		    if (BDDB.getInt("TrialPeriod")) {
                // There is a trial period involved.  Check to see if they
                // are still eligible.
                QDate      tmpDate1;
                QDate      tmpDate2;
                QDate      today;
                today   = QDate::currentDate();
                
                myDatetoQDate((const char *) CDB.getStr("AccountOpened"), &tmpDate1);
                tmpDate2 = tmpDate1.addDays(BDDB.getInt("TrialPeriod"));
                if (tmpDate2 >= today) {
                    // They are still eligible for a trial.
                    QDatetomyDate(lastDate, today);
                    QDatetomyDate(endsOn,   tmpDate2);
                } else {
                    QDatetomyDate(lastDate, today);
                    QDatetomyDate(endsOn,   today);
                }
	    	}
		    
		    
			// Setup the subscription data
			SDB.setValue("CustomerID", getLong("CustomerID"));
			SDB.setValue("LoginID", getStr("LoginID"));
			SDB.setValue("Active", (int) 1);
			SDB.setValue("PackageNo", (long) 0);
			SDB.setValue("ItemNumber", BDB.getLong("ItemNumber"));
			SDB.setValue("Quantity", (float) 1.0);
			SDB.setValue("ItemDesc", BDB.getStr("Description"));
			SDB.setValue("Price", (float) 0.0);
			SDB.setValue("LastDate", lastDate);
			SDB.setValue("EndsOn", endsOn);
			SDB.setValue("AutoRenew", (int) 1);
			SDB.setValue("AutoPrice", (int) 1);
			SDB.setValue("LinkedTo",  SUBSCRLINK_DOMAINS);     // Domain Link
			SDB.setValue("LinkID", getLong("InternalID"));
			SDB.setValue("LinkStr", getStr("DomainName"));
			SDB.ins();
		}
	}	
}

/*
** updateSubscriptions - A private function that gets called when a login
**                       type has changed.  It handles the updates for the
**                       subscriptions by removing the old subscriptions
**                       and putting new ones in its place.  This is done
**                       automatically when upd() is called.
*/

void DomainsDB::updateSubscriptions(int oldType, int newType)
{
	BlargDB			DB1;
	BlargDB			DB2;
	SubscriptionsDB	SDB;
	BillablesDB		BDB;
	int             keepGoing = 1;
	
	// The first thing we need to do is get the associated subscription
	// billables from the DomainTypeBillables table.
	DB1.query("select ItemNumber from DomainTypeBillables where DomainTypeID = %d", oldType);
	while (DB1.getrow()) {
		// Now, get any matches from the subscriptions database.
		DB2.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s' and ItemNumber = %ld and LinkedTo = %d and LinkID = %ld",
		  getLong("CustomerID"),
		  (const char *) getStr("LoginID"),
		  atol(DB1.curRow[0]),
		  SUBSCRLINK_DOMAINS,
		  getLong("InternalID")
		);
		if (DB2.rowCount) {
			DB2.getrow();
			// We have a match in the subscriptions database.  Delete it.
			SDB.get(atol(DB2.curRow[0]));
			SDB.setValue("Active", getInt("Active")); // (int) 0);
			SDB.upd();
			keepGoing = 0;
			// SDB.del(atoi(DB2.curRow[0]));
		} else {
            // There were no matches found.  See if this domain is active,
            // if so, add subscriptions instead.
            if (getInt("Active")) {
                addSubscriptions();
                keepGoing = 0;
            }
        }
	}
	
	if (keepGoing) {
		// Now, get the associated subscription items for the new DomainType.
		DB1.query("select ItemNumber from DomainTypeBillables where DomainTypeID = %d", newType);
		while (DB1.getrow()) {
			// Load the Billable into memory
			BDB.get(atol(DB1.curRow[0]));
			// If it is a subscription item, add it into the subscriptions.
			if (BDB.getInt("ItemType") == 1) {
				// Setup the subscription data
				SDB.setValue("CustomerID", getLong("CustomerID"));
				SDB.setValue("LoginID", getStr("LoginID"));
				SDB.setValue("Active", getInt("Active"));
				SDB.setValue("PackageNo", (long) 0);
				SDB.setValue("ItemNumber", BDB.getLong("ItemNumber"));
				SDB.setValue("Quantity", (float) 1.0);
				SDB.setValue("ItemDesc", BDB.getStr("Description"));
				SDB.setValue("Price", (float) 0.0);
				SDB.setValue("LastDate", "");
				SDB.setValue("EndsOn", "");
				SDB.setValue("AutoRenew", (int) 1);
				SDB.setValue("AutoPrice", (int) 1);
				SDB.setValue("LinkedTo",  SUBSCRLINK_DOMAINS);     // Domain Link
				SDB.setValue("LinkID", getLong("InternalID"));
				SDB.setValue("LinkStr", getStr("DomainName"));
				SDB.ins();
			}
		}	
    }
}

/*
** updateARForTypeChg - This function searches through the AcctsRecv table
**                      for any billed items to the specified login ID that
**                      have a date range.  The EndDate must be > Today.
**                      Any transactions found will be copied (ItemID, etc)
**                      and a negative amount adjusting entry will be inserted
**                      into the customer register.  This will quickly pro-rate
**                      a balance for a customer that is changing their account
**                      type.
*/

void DomainsDB::updateARForTypeChg(void)
{
	BlargDB			DB;
	BlargDB			DB2;
	CustomersDB		CDB;
	QDate			tmpDate;
	QDate			CycleStart;
	QDate			CycleEnd;
	QDate			yesterday;
	char			sYesterday[64];
	int				cycleDays;
	int				daysLeft;
	char			today[64];
	char			sCycleEnd[64];
	float			Qty;
	BillingCyclesDB	BCDB;
	
	tmpDate = QDate::currentDate();
	sprintf(today, "%04d-%02d-%02d", tmpDate.year(), tmpDate.month(), tmpDate.day());
	
	CDB.get(getLong("CustomerID"));
	BCDB.get(CDB.getLong("BillingCycle"));
	BCDB.getCycleDates(&CycleStart, &CycleEnd, &cycleDays, &daysLeft);
	sprintf(sCycleEnd, "%04d-%02d-%02d", CycleEnd.year(), CycleEnd.month(), CycleEnd.day());
	
	// Okay, the first thing we want/need to do is load up the entries
	// in the AcctsRecv table that have an EndDate > Today.
	DB.query("select ItemID, Price, Amount, Memo, InternalID from AcctsRecv where CustomerID = %ld and LoginID = '%s' and EndDate > '%s'", 
	  getLong("CustomerID"),
	  (const char *) getStr("LoginID"),
	  today
	);
	if (DB.rowCount) {
		while (DB.getrow()) {
			// It looks as if we need to add to the balance of the user.
			AcctsRecv	AR;
			AR.ARDB->setValue("CustomerID", getLong("CustomerID"));
			AR.ARDB->setValue("LoginID", getStr("LoginID"));
			AR.ARDB->setValue("ItemID", atol(DB.curRow[0]));
			AR.ARDB->setValue("TransDate", today);
			AR.ARDB->setValue("StartDate", today);
			AR.ARDB->setValue("EndDate", today);  // End date has to be today or we'll pro-rate it again.
			Qty = (float) daysLeft / cycleDays;
			if ((float)atof(DB.curRow[2]) > 0) {
				Qty = Qty * -1.0;
			}
			AR.ARDB->setValue("Quantity", Qty);
			AR.ARDB->setValue("Price", DB.curRow[1]);
			AR.ARDB->setValue("Amount", (float)atof(DB.curRow[1]) * Qty);
			QString tStr;
			tStr = DB.curRow[3];
			if (tStr.find('(')) {
			    tStr.truncate(tStr.find('(') - 1);
			}
			AR.ARDB->setValue("Memo", (const char *) tStr);
			AR.ARDB->appendStr("Memo", " (Adjustment for account type change)");
			AR.SaveTrans();
				
			DB2.dbcmd("update AcctsRecv set EndDate = '%s' where InternalID = %ld", today, atol(DB.curRow[4]));
		}
	
	} else {
		// There were no register entries.  So make sure there are
		// Subscriptions for the user.  If there aren't, add one.  If there
		// are, update all of the subscriptions for this user so the ends on
		// date are yesterday, and then do a subscription run for the 
		// customer.
		DB.query("select InternalID from Subscriptions where CustomerID = %ld and LoginID = '%s'", getLong("CustomerID"), (const char *) getStr("LoginID"));
		if (DB.rowCount) {
			// They have subscriptions, so lets do a quick and dirty update
			// of them.
			tmpDate = QDate::currentDate();
			yesterday = tmpDate.addDays(-1);
			sprintf(sYesterday, "%04d-%02d-%02d", yesterday.year(), yesterday.month(), yesterday.day());
			DB.dbcmd("update Subscriptions set EndsOn = '%s' where CustomerID = %ld and LoginID = '%s'",
			  sYesterday,
			  getLong("CustomerID"),
			  (const char *) getStr("LoginID")
			);
		} else {
			// No subscriptions.  Add them.
			// LDB.get(myCustID, LoginID);
			addSubscriptions();
		}
	}
}



/*
*************************************************************************
**
** RatePlansDB - generic routines for reading & updating the
**            RatePlans table.
**
************************************************************************
*/

RatePlansDB::RatePlansDB(void)
{
    setTableName("RatePlans");
}

RatePlansDB::~RatePlansDB()
{
}

//
// getbytag()       - Gets a record from the database using the
//                    specified PlanTag.
//
// Args        - int IntID - the InternalID of the record
//
// Returns     - 0 on failure, 1 on success
//

long RatePlansDB::getbytag(const char * searchTag)
{
    long    Ret = 0;
    char    tmpTag[4096];
	BlargDB	DB;

    // Escape the strings
    strcpy(tmpTag, DB.escapeString(searchTag));
                
    DB.query("select InternalID from RatePlans where PlanTag = '%s'", tmpTag);
    
    if (DB.rowCount == 1) {
        DB.getrow();
        Ret = get(atol(DB.curRow[0]));
    }
    return(Ret);
}

//
// del()       - Deletes a record in the database.
//
// Args        - The InternalID to delete.
//
// Returns     - -1 on failure, 0 on success
//

void RatePlansDB::postDel(void)
{
	BlargDB	DB;
    
    if (getLong("InternalID")) {
    	DB.dbcmd("delete from BillablesData where RatePlanID = %ld", getLong("InternalID"));
    	DB.dbcmd("delete from PackagesData where RatePlanID = %ld", getLong("InternalID"));
    }
}


/*
*************************************************************************
**
** AcctsRecvDB - generic routines for reading & updating the
**               AcctsRecv table.
**
************************************************************************
*/

AcctsRecvDB::AcctsRecvDB(void)
{
    setTableName("AcctsRecv");
}

AcctsRecvDB::~AcctsRecvDB()
{
}

//
// ins()       - Inserts a record into the database.
//
// Args        - None.
//
// Returns     - the new Internal ID
//

void AcctsRecvDB::postIns(void)
{
    BlargDB DB;                 // This is cheating.
    	
	DB.dbcmd("update Customers set CurrentBalance = CurrentBalance + %f where CustomerID = %ld",
	  getFloat("Amount"),
	  getLong("CustomerID")
	);
}

//
// upd()       - Updates a record in the database.
//
// Args        - None.
//
// Returns     - -1 on failure, 0 on success
//

void AcctsRecvDB::postUpd(void)
{
	char    query[2048];
	BlargDB	DB;
	float   tmpAmt;
	
	if (!getLong("InternalID")) return;
	
	if (getFloat("Amount", 1) != getFloat("Amount", 0)) {
		sprintf(query, "Amount = %f", getFloat("Amount"));
		
		tmpAmt = getFloat("Amount", 1);
		tmpAmt = tmpAmt * -1;
		
		DB.dbcmd("update Customers set CurrentBalance = CurrentBalance + %f where CustomerID = %ld",
		  tmpAmt,
		  getLong("CustomerID")
		);
		
		DB.dbcmd("update Customers set CurrentBalance = CurrentBalance + %f where CustomerID = %ld",
		  getFloat("Amount"),
		  getLong("CustomerID")
		);
		
	}
}

void AcctsRecvDB::postDel(void)
{
    ADB	    DB;
    GLDB	GDB;
    float	tmpAmount;
    
    if (!getLong("InternalID")) return;

    DB.query("select SUM(Amount) from AcctsRecv where CustomerID = %ld", getLong("CustomerID"));
    DB.getrow();
    tmpAmount = atof(DB.curRow[0]);
    DB.dbcmd("update Customers set CurrentBalance = %f where CustomerID = %ld", tmpAmount, getLong("CustomerID"));

	//tmpAmount = getFloat("Amount") * -1.0;
	//DB.dbcmd("update Customers set CurrentBalance = CurrentBalance + %f where CustomerID = %ld", tmpAmount, getLong("CustomerID"));
	
    GDB.del(getLong("GLTransID"));
    
    DB.dbcmd("update AcctsRecv set Cleared = 0, ClearedAmount = 0.00 where CustomerID = %ld",  getLong("CustomerID"));
    applyCredits(getLong("CustomerID"));
}

CCTransDB::CCTransDB(void)
{
    setTableName("CCTrans");
    setEncryptedColumn("CardNo");
}

CCTransDB::~CCTransDB()
{
}

int CCTransDB::isValid(void)
{
    return(creditCardValid(getInt("CardType"), getStr("CardNo"), getStr("ExpDate")));
}



/*
*************************************************************************
**
** StatementsDB - generic routines for reading & updating the
**                Statements table.
**
************************************************************************
*/

StatementsDB::StatementsDB(void)
{
    setTableName("Statements");

    // Setup the printer offsets.
    transDateX1     = 20;
    transDateX2     = transDateX1 + 44;
    serviceDateX1   = transDateX2 +  1;
    serviceDateX2   = serviceDateX1 + 89;
    loginIDX1       = serviceDateX2 + 1;
    loginIDX2       = loginIDX1 + 59;
    descriptionX1   = loginIDX2 + 1;
    descriptionX2   = descriptionX1 + 279;
    amountX1        = descriptionX2 + 1;
    amountX2        = amountX1 + 39;
    balanceX1       = amountX2 + 1;
    balanceX2       = balanceX1 + 39;

    strcpy(headerFont, "b&h lucida");
    strcpy(bodyFont,   "b&h lucida");
    strcpy(footerFont, "b&h lucida");
}

StatementsDB::~StatementsDB()
{
}

//
// postDel()       - Deletes a record in the database.
//
//

void StatementsDB::postDel(void)
{
	BlargDB	DB;
    
    DB.dbcmd("delete from StatementsData where StatementNo = %ld", getLong("StatementNo"));
}

/*
** StatementsDB::print()  - Prints a statement to the specified QPainter
**                          device, or, if NULL is specified, a printer
**                          and painter device will be created with the
**                          defaults.
*/

void StatementsDB::print(QPainter *inP)
{
    QApplication::setOverrideCursor(Qt::waitCursor);
    QPrinter            prn(QPrinter::PrinterResolution);
    QPainter            *p;
    QBrush              bbrush;
    QRect               rect;
    QDate               tmpDate1;
    QDate               tmpDate2;
    char                tmpst[1024];
    char                tStr[1024];
    StatementsDataDB    SDDB;
    BlargDB             DB;
    int                 yPos = 165;
    int                 pOfs = 36;
    
    float               Balance = 0.0;
    
    int                 pageNo = 1;
    int                 totPages = 1;
    
    // prn = NULL;
    
    // First, check to see if we need to create a printer device.
    if (inP == NULL) {
        // We do.  Configure our printer
        // While testing, set the output to a file.
        sprintf(tmpst, "/tmp/statement-%09ld.ps", getLong("StatementNo"));
        // fprintf(stderr, "Printing statement %ld to '%s'...\n", getLong("StatementNo"), tmpst);
        //prn.setup();
        //prn.setPageSize(QPrinter::Letter);
        //prn.setOutputFileName(tmpst);
        //prn.setOutputToFile(TRUE);
        prn.setOutputToFile(false);
        prn.setFullPage(true);
        //prn.setFullPage(false);
        

        // Now, set up the document name, creator, etc.
        prn.setDocName("Statement");
        prn.setCreator("Total Accountability");
        
        p = new QPainter();
        p->begin(&prn);
        
    } else {
        // Use the paint device we were passed.
        p = inP;
    }
    
    
    // Get the font metrics to see exactly how many pages we'll have.
    // We space each line by two pixels.  The description is 250 points wide.
    // Anything that needs more than that, we adjust accordingly.
    p->setFont(QFont(bodyFont, 8, QFont::Normal));
    QFontMetrics fm(p->fontMetrics());
    // fprintf(stderr, "Font = '%s'\n", (const char *) p->fontInfo().family());
    
    // Now, load the statement data from the database and start printing
    // it, watching for page breaks, etc.
    yPos = 275 + pOfs;
    DB.query("select InternalID, Description from StatementsData where StatementNo = %ld order by InternalID", getLong("StatementNo"));
    while (DB.getrow()) {
        // int Lines = (int) (strlen(DB.curRow[1]) / 52) + 1;
        int Lines = (int) (fm.width(DB.curRow[1]) / (descriptionX2 - descriptionX1 - 2)) + 1;
        //int rowHeight = 15 * Lines;
        int rowHeight = (fm.height()+2) * Lines;
        //if (yPos + rowHeight >= 690) {
        if (yPos + rowHeight >= 730) {
            totPages++;
            yPos = 115 + pOfs + rowHeight;
        } else {
            yPos += rowHeight;
        }
    }
    

    // We're all set.  Put the header on the page.
    printHeader(p, pageNo);    
    printFooter(p, pageNo, totPages);
    registerHeader(p, pageNo);

    yPos = 295 + pOfs;
    
    // Re-run our query to get the data.
    DB.query("select InternalID from StatementsData where StatementNo = %ld order by InternalID", getLong("StatementNo"));
    while (DB.getrow()) {
        SDDB.get(atol(DB.curRow[0]));
        
        p->setFont(QFont(bodyFont, 8, QFont::Normal));

        //int Lines = (int) (strlen(SDDB.getStr("Description")) / 52) + 1;
        //int RowHeight = 15 * Lines;
        int Lines = (int) (fm.width(SDDB.getStr("Description")) / (descriptionX2 - descriptionX1 - 2)) + 1;
        int RowHeight = (fm.height()+2) * Lines;
        
        // Check to see if we have enough room on the page left for this
        // line.
        if (yPos+RowHeight >= 730) {
        //if (yPos+RowHeight >= 690) {
            prn.newPage();
            printHeader(p, ++pageNo);
            printFooter(p, pageNo, totPages);
            registerHeader(p, pageNo);
            yPos = 115+pOfs;
        } 
    
        // The transaction date.
        myDatetoQDate((const char *)SDDB.getStr("TransDate"), &tmpDate1);
        sprintf(tmpst, "%02d/%02d/%02d", tmpDate1.month(), tmpDate1.day(), tmpDate1.year()%100);
        rect.setCoords(transDateX1, yPos, transDateX2, yPos + RowHeight-1);
        p->drawRect(rect);
        p->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, tmpst);
        
        // The service period.
        strcpy(tmpst, "");      // Start with an empty string
        bool printDate = true;
        if (atoi(SDDB.getStr("Special"))) printDate = false;
        // If the starting and ending dates are the same, don't print a 
        // date range.
        if (!strcmp(SDDB.getStr("StartDate"), SDDB.getStr("EndDate"))) printDate = false;
        
        rect.setCoords(serviceDateX1, yPos, serviceDateX2, yPos + RowHeight-1);
        p->drawRect(rect);
        if (printDate) {
            // Put the date range in.
            myDatetoQDate((const char *)SDDB.getStr("StartDate"), &tmpDate1);
            myDatetoQDate((const char *)SDDB.getStr("EndDate"), &tmpDate2);
            sprintf(tmpst, "%02d/%02d/%02d - %02d/%02d/%02d",
                    tmpDate1.month(), tmpDate1.day(), tmpDate1.year() % 100,
                    tmpDate2.month(), tmpDate2.day(), tmpDate2.year() % 100
                   );

            p->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, tmpst);
        }
        
        // The Login ID
        rect.setCoords(loginIDX1, yPos, loginIDX2, yPos + RowHeight-1);
        p->drawRect(rect);
        p->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, SDDB.getStr("LoginID"));
        
        // The description...
        rect.setCoords(descriptionX1, yPos, descriptionX2, yPos + RowHeight-1);
        p->drawRect(rect);
        rect.setCoords(descriptionX1+2, yPos, descriptionX2, yPos + RowHeight);
        p->drawText(rect, Qt::WordBreak|Qt::AlignLeft|Qt::AlignVCenter, SDDB.getStr("Description"));
        
        // The amount.
        rect.setCoords(amountX1, yPos, amountX2, yPos + RowHeight-1);
        p->drawRect(rect);
        p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, SDDB.getStr("Amount"));
        
        // The balance.
        Balance += SDDB.getFloat("Amount");
        sprintf(tStr, "%.2f", Balance);
        if (Balance == 0.0) strcpy(tStr, "0.00");
        rect.setCoords(balanceX1, yPos, balanceX2, yPos + RowHeight-1);
        p->drawRect(rect);
        p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tStr);
        
        yPos += RowHeight;

    }
    
    
    
    // Now, check to see if we need to close the printer devices.
    if (inP == NULL) {
        p->end();
        delete (p);
    }

    QApplication::restoreOverrideCursor();
}

/*
** StatementsDB::printHeader - Puts a header on the paint device for printing.
**                             This should only be done on the first page.
**                             Subsequent pages have the footer and not the 
**                             header.
**
*/

void StatementsDB::printHeader(QPainter *p, int PageNo)
{
    QDate       theDate;
    QDate       stDate;
    QDate       tmpDate;
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    int         yPos;
    int         pOfs = 36;
    CustomersDB cust;
    char        tmpstr[1024];
    
    cust.get(getLong("CustomerID"));

    theDate = QDate::currentDate();
    
    // p->rotate(55);
    if (PageNo == 1) yPos = 76+pOfs;
    else yPos = 36+pOfs;
    //if (PageNo == 1) ypos = 102;
    //else ypos = 72;
    QFont   hbFont;
    hbFont.setFamily(headerFont);
    hbFont.setPointSize(10);
    hbFont.setWeight(QFont::Bold);
    hbFont.setStyleHint(QFont::SansSerif, QFont::PreferDevice);
    QFont   hnFont;
    hnFont.setFamily(headerFont);
    hnFont.setPointSize(10);
    hnFont.setWeight(QFont::Normal);
    hnFont.setStyleHint(QFont::SansSerif, QFont::PreferDevice);

    p->setFont(hbFont);
    p->drawText(34, yPos, "Blarg! Online Services, Inc.");
    p->setFont(hnFont);
    p->drawText(34, yPos+10, "PO Box 1827");
    p->drawText(34, yPos+20, "Bellevue, WA 98009-1827");
    //p->drawText(34, yPos+30, "Phone:  425.401-9821");
    //p->drawText(34, yPos+40, "Fax: 425/401-9741");

    myDatetoQDate((const char *)getStr("StatementDate"), &stDate);
    
    // 2003-01-30, Since there are many companies that seem to have problems
    // with the word "Statement", we'll simply remove it.
    /*
    rect.setCoords(450,36+pOfs,600, 65+pOfs);
    p->setFont(QFont("helvetica", 18, QFont::Bold));
    p->drawText(rect, Qt::AlignCenter, "Statement");
    */

    // 2003-01-30, The date is also now moving.
    /*
    rect.setCoords(450,65+pOfs,600, 80+pOfs);
    p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->drawText(rect, Qt::AlignCenter, stDate.toString());
    */
    
    if (PageNo == 1) {

	    p->setFont(QFont(headerFont, 10, QFont::Normal));
	    //yPos = 162;
	    yPos = 182 + pOfs;
        // Because of the variations in the windowed envelopes,
        // we'll center the customer address between 182 and 132 
        // pixels.
        if (!strlen(getStr("CustName")))  yPos += 5;
        if (!strlen(getStr("CustAddr1"))) yPos += 5;
        if (!strlen(getStr("CustAddr2"))) yPos += 5;
        if (!strlen(getStr("CustAddr3"))) yPos += 5;
        if (!strlen(getStr("CustAddr4"))) yPos += 5;

	    if (strlen(getStr("CustName"))) {
	        p->drawText( 60, yPos, getStr("CustName"));
	        yPos += 10;
	    }
	    if (strlen(getStr("CustAddr1"))) {
	        p->drawText( 60, yPos, getStr("CustAddr1"));
	        yPos += 10;
	    }
	    if (strlen(getStr("CustAddr2"))) {
	        p->drawText( 60, yPos, getStr("CustAddr2"));
	        yPos += 10;
	    }
	    if (strlen(getStr("CustAddr3"))) {
	        p->drawText( 60, yPos, getStr("CustAddr3"));
	        yPos += 10;
	    }
	    if (strlen(getStr("CustAddr4"))) {
	        p->drawText( 60, yPos, getStr("CustAddr4"));
	        yPos += 10;
	    }

	    // For the first page, it should have a black box for the title, and
	    // give a summary of all of the information (Customer ID, new charges,
	    // total due, etc.)

        int acctSummaryTop = pOfs+66;
        yPos = acctSummaryTop;
	    p->setFont(QFont(headerFont, 12, QFont::Bold));
	    bbrush.setStyle(Qt::SolidPattern);
	    bbrush.setColor(Qt::black);
	    //p->setBackgroundMode(Qt::OpaqueMode);
	    p->setPen(Qt::white);
	    //rect.setCoords(325, 100+pOfs, 575, 115+pOfs);
	    rect.setCoords(325, yPos, 575, yPos+15);
	    p->fillRect(rect, bbrush);
	    p->drawText(rect, Qt::AlignCenter, "Account Summary");
	    
	    p->setBackgroundMode(Qt::TransparentMode);
	    p->setPen(Qt::black);
	    
	    //yPos = 120+pOfs;
	    yPos+=20;
	    p->setFont(QFont(bodyFont, 8, QFont::Normal));
	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Customer ID:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "%ld", getLong("CustomerID"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;
	    
	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Primary User Name:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    CustomersDB CDB;
	    CDB.get(getLong("CustomerID"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, CDB.getStr("PrimaryLogin"));
	    yPos += 10;
	    
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Reference Number:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "%ld", getLong("StatementNo"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;
	    
        QDate   ctDate = stDate.addDays(-1);
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Charges Through:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "%s", (const char *) ctDate.toString());
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;

	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Payment Due By:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    myDatetoQDate((const char *) getStr("DueDate"), &tmpDate);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpDate.toString());
	    yPos += 10;
	    
	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Previous Balance:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "$%.2f", getFloat("PrevBalance"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;
	    
	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Credits/Payments:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "$%.2f", getFloat("Credits"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;
	    
	    //p->setFont(QFont("courier", 10, QFont::Normal));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "New Charges:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "$%.2f", getFloat("NewCharges"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 10;
	    
	    //p->setFont(QFont("courier", 10, QFont::Bold));
	    rect.setCoords(325,yPos,455,yPos+13);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Total Amount Due:");
	    rect.setCoords(465,yPos,565,yPos+13);
	    sprintf(tmpstr, "$%.2f", getFloat("TotalDue"));
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
	    yPos += 13;
	    
	    //p->setFont(QFont("courier", 10, QFont::Bold));
	    rect.setCoords(325,yPos,455,yPos+16);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Amount Enclosed:");
	    rect.setCoords(465,yPos,565,yPos+16);
	    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "$");
	    p->drawRect(rect);
	    yPos += 20;

        // Draw a box around the account summary.
	    //rect.setCoords(325,100+pOfs,575,225+pOfs);
	    rect.setCoords(325,acctSummaryTop,575,yPos);
	    p->drawRect(rect);

	    // Draw the line where the perforation is.  It is going to be
	    // 3.5" down from the top of the page.
	    //p->drawLine(  0,  242, 692, 242);
	    //rect.setCoords(100, 242, 674, 262);
	    p->drawLine(  0,  262+pOfs, 692, 262+pOfs);
	    rect.setCoords(0, 262+pOfs, 576, 282+pOfs);
	    p->setFont(QFont(headerFont, 6, QFont::Normal, TRUE));
	    p->drawText(rect, Qt::AlignTop|Qt::AlignCenter, "Please detach here and return the upper portion with your payment.  Keep the bottom portion for your records.");
	    p->setFont(QFont(headerFont,10, QFont::Normal));
    }
}

/*
** StatementsDB::printFooter - Puts a footer on the paint device for printing.
**                             This should be done for each page.
**
*/

void StatementsDB::printFooter(QPainter *p, int PageNo, int TotPages)
{
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    QDate       tmpDate;
    CustomersDB cust;
    int         yPos;
    int         pOfs = 36;
    int         footerStart = 740;
    char        tmpstr[1024];
    QDate       stDate;

    
    cust.get(getLong("CustomerID"));

    myDatetoQDate((const char *)getStr("StatementDate"), &stDate);

    // And now, the footer...The bottom of the page is at 720.
    // So, that being the case, we'll work backwards from the bottom.
    yPos = 770+pOfs;
    
    // 735 is where we want to start.  We'll actually start working from
    // top down now to keep things lined up.
    yPos = footerStart;
    
    // Draw a line above the bottom info...
    // About 3.5" down from the top of the page.
    p->drawLine(  0,yPos-3, 692, yPos-3);

    // Put in the Blarg address
    p->setFont(QFont(footerFont, 9, QFont::Bold));
    QFontMetrics fm(p->fontMetrics());
    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Blarg! Online Services, Inc.");
    yPos += fm.height();

    // Reset our font and metrics
    p->setFont(QFont(footerFont, 9, QFont::Normal));
    fm = p->fontMetrics();

    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "PO Box 1827");
    yPos += fm.height();

    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Bellevue, WA 98009-1827");
    yPos += fm.height();
    
    // Add a spacer
    yPos += fm.height();

    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Email: accounting@blarg.net");
    yPos += fm.height();

    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Phone: 425.818.6500");
    yPos += fm.height();
    
    rect.setCoords(36,yPos,192,yPos+fm.height());
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Toll Free: 888.662.5274");
    yPos += fm.height();
    
    // Reset our font and metrics.
    // This is the middle part, which is the account summary.
    p->setFont(QFont(headerFont, 8, QFont::Normal));
    fm = p->fontMetrics();
    yPos = footerStart;
    
    int XA1 = 200;
    int XA2 = 290;
    int XB1 = 290;
    int XB2 = 375;

    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Customer ID:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "%ld", getLong("CustomerID"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    
    //p->setFont(QFont("courier", 10, QFont::Normal));
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Primary User Name:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    CustomersDB CDB;
    CDB.get(getLong("CustomerID"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, CDB.getStr("PrimaryLogin"));
    yPos += fm.height();
    
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Reference Number:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "%ld", getLong("StatementNo"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    
    QDate   ctDate = stDate.addDays(-1);
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Charges Through:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "%s", (const char *) ctDate.toString());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();

    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Payment Due By:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    myDatetoQDate((const char *) getStr("DueDate"), &tmpDate);
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpDate.toString());
    yPos += fm.height();
    
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Previous Balance:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "$%.2f", getFloat("PrevBalance"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Credits/Payments:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "$%.2f", getFloat("Credits"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "New Charges:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "$%.2f", getFloat("NewCharges"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    
    rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, "Total Amount Due:");
    rect.setCoords(XB1,yPos,XB2,yPos+fm.height());
    sprintf(tmpstr, "$%.2f", getFloat("TotalDue"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpstr);
    yPos += fm.height();
    

    // And now, the middle page...The bottom of the page is at 720.
    // So, that being the case, we'll work backwards from the bottom.
    /*
    yPos = 770+pOfs;

    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Payment Due By:");
    rect.setCoords(XB1,yPos,XB2+30,yPos+16);
    myDatetoQDate((const char *) getStr("DueDate"), &tmpDate);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, tmpDate.toString());
    yPos -= 6;

    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Total Amount Due:");
    rect.setCoords(XB1,yPos,XB2,yPos+16);
    tmpSt.sprintf("$%.2f", getFloat("TotalDue"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpSt);
    yPos -= 6;
    
    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Total New Charges:");
    rect.setCoords(XB1,yPos,XB2,yPos+16);
    tmpSt.sprintf("$%.2f", getFloat("NewCharges"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpSt);
    yPos -= 6;

    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Credits/Payments:");
    rect.setCoords(XB1,yPos,XB2,yPos+16);
    tmpSt.sprintf("$%.2f", getFloat("Credits"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpSt);
    yPos -= 6;

    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Previous Balance:");
    rect.setCoords(XB1,yPos,XB2,yPos+16);
    tmpSt.sprintf("$%.2f", getFloat("PrevBalance"));
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpSt);
    yPos -= 6;


    // The Statement Number
    rect.setCoords(XA1,yPos,XA2,yPos+16);
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Reference Number:");
    rect.setCoords(XB1,yPos,XB2,yPos+16);
    tmpSt.sprintf("%ld", getLong("StatementNo"));
    p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, tmpSt);
    yPos -= 6;
    */

    // And now, the right part...The bottom of the page is at 720.
    // So, that being the case, we'll work backwards from the bottom.
    yPos = footerStart;

    // Reset our font and metrics
    p->setFont(QFont(footerFont, 8, QFont::Normal));
    fm = p->fontMetrics();

    rect.setCoords(350,yPos,565,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, getStr("CustName"));
    yPos += fm.height();

    rect.setCoords(350,yPos,565,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, getStr("CustAddr1"));
    yPos += fm.height();

    rect.setCoords(350,yPos,565,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, getStr("CustAddr2"));
    yPos += fm.height();

    rect.setCoords(350,yPos,565,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, getStr("CustAddr3"));
    yPos += fm.height();
    
    rect.setCoords(350,yPos,565,yPos+fm.height());
    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, getStr("CustAddr4"));
    yPos += fm.height();

    // A spacer
    yPos += fm.height();
    // The Page Number if applicable
    if (TotPages > 1) {
	    //rect.setCoords(XA1,yPos,XA2,yPos+fm.height());
	    //p->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, "Page Number:");
	    rect.setCoords(350,yPos,565,yPos+fm.height());
	    tmpSt.sprintf("Page number %d of %d", PageNo, TotPages);
	    p->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, tmpSt);
	    yPos += fm.height();
    }

    // Draw a line above the bottom info...
    // 3.5" down from the top of the page.
    //p->drawLine(  0,yPos+5, 692, yPos+5);


}

/*
** registerHeader - Puts a register header on the page.
*/

void StatementsDB::registerHeader(QPainter *p, int PageNo)
{
    QRect       rect;
    QString     tmpSt;
    QBrush      bbrush;
    int         topY;
    int         pOfs = 36;
    int         rowHeight;
    
    if (PageNo == 1) {
        //topY = 262;
        topY = 282+pOfs;
    } else {
        //topY = 100;
        topY = 100+pOfs;
    }

    //p->setFont(QFont("helvetica", 10, QFont::Normal));
    p->setFont(QFont(headerFont, 8, QFont::Normal));
    QFontMetrics fm(p->fontMetrics());
    rowHeight = fm.height()+3;
//    p->drawRect(40, 150, 525, 15);
    bbrush.setStyle(Qt::SolidPattern);
    bbrush.setColor(Qt::black);

    //p->setBackgroundMode(Qt::OpaqueMode);
    p->setPen(Qt::white);

    rect.setCoords(transDateX1, topY, transDateX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Date");

    rect.setCoords(serviceDateX1, topY, serviceDateX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Service Period");
    p->drawLine(serviceDateX1, topY, serviceDateX1, topY+rowHeight);
        
    rect.setCoords(loginIDX1, topY, loginIDX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "User Name");
    p->drawLine(loginIDX1, topY, loginIDX1, topY+rowHeight);
        
    rect.setCoords(descriptionX1, topY, descriptionX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Description");
    p->drawLine(descriptionX1, topY, descriptionX1, topY+rowHeight);

    rect.setCoords(amountX1, topY, amountX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Amount");
    p->drawLine(amountX1, topY, amountX1, topY+rowHeight);

    rect.setCoords(balanceX1, topY, balanceX2, topY+rowHeight);
    p->fillRect(rect, bbrush);
    p->drawText(rect, Qt::AlignCenter, "Balance");
    p->drawLine(balanceX1, topY, balanceX1, topY+rowHeight);

    // Reset our pen back to a transparent background and black letters.
    p->setBackgroundMode(Qt::TransparentMode);
    p->setPen(Qt::black);
}


/*
*************************************************************************
**
** StatementsDataDB - generic routines for reading & updating the
**                    StatementsData table that contains the individual
**                    line items for a statement.
**
************************************************************************
*/

StatementsDataDB::StatementsDataDB(void)
{
    setTableName("StatementsData");
}

StatementsDataDB::~StatementsDataDB()
{
}



/*
*************************************************************************
**
** AutoPaymentsDB - generic routines for reading & updating the
**                  AutoPayments table that contains the automatic payment
**                  information for customers.
**
************************************************************************
*/

AutoPaymentsDB::AutoPaymentsDB(void)
{
    setTableName("AutoPayments");
    setEncryptedColumn("Name");
    setEncryptedColumn("Address");
    setEncryptedColumn("CityState");
    setEncryptedColumn("ZIP");
    setEncryptedColumn("Phone");
    setEncryptedColumn("AuthName");
    setEncryptedColumn("BankName");
    setEncryptedColumn("BankCity");
    setEncryptedColumn("BankState");
    setEncryptedColumn("CardType");
    setEncryptedColumn("AcctNo");
}

AutoPaymentsDB::~AutoPaymentsDB()
{
}


/*
** createDomainAliases - Given a customer ID this will auto create the
**                       domain aliases for them for all of the domains
**                       that can have aliases.  This can also optionally
**                       do the "stock" aliases.  If a domain name is passed
**                       it will work only on that domain otherwise it will
**                       do all of the domains for a customer.
*/

void createDomainAliases(long custID, int doStock, const char *domainName)
{
    ADBTable mailDB("Virtual", cfgVal("MailSQLDB"), cfgVal("MailSQLUser"), cfgVal("MailSQLPass"), cfgVal("MailSQLHost"));
    ADB      DB;
    char    timedate[128];
    char    fullAddr[1024];
    char    addedBy[1024];
    QDateTime   cd = QDateTime::currentDateTime();
    sprintf(timedate, "%04d-%02d-%02d %02d:%02d:%02d",
            cd.date().year(),
            cd.date().month(),
            cd.date().day(),
            cd.time().hour(),
            cd.time().minute(),
            cd.time().second()
           );
    sprintf(addedBy, "%s (Auto)", curUser().userName);
    char    stockaliases[5][32] = {
        "webmaster",
        "sales",
        "info",
        "support",
        ""
    };

    // Check to see if we're doing email for them.
    // If they specified a domain name, check to see that we're doing mail 
    // for it before doing anything else.
    /*
    if (domainName) {
        mailDB.query("select VirtualID from Virtual where Mailbox = '%s'", domainName);
        if (!mailDB.rowCount) {
            // Nope.  We're not.  Return.
            return;
        }
    }
    */

    
    // Okay, this is an interesting query.  It will give us basically
    // what we need to compare against in the Virtual database
    if (domainName) DB.query("select Logins.LoginID, Domains.DomainName, DomainTypes.DomainType from Logins, Domains, DomainTypes where Logins.CustomerID = %ld and Domains.CustomerID = Logins.CustomerID and Logins.Wiped = '0000-00-00' and Domains.DomainName = '%s' and Domains.DomainType = DomainTypes.InternalID and Domains.Active > 0 and DomainTypes.AllowAliases > 0 order by Logins.LoginID", custID, domainName);
    else DB.query("select Logins.LoginID, Domains.DomainName, DomainTypes.DomainType from Logins, Domains, DomainTypes where Logins.CustomerID = %ld and Domains.CustomerID = Logins.CustomerID and Logins.Wiped = '0000-00-00' and Domains.DomainType = DomainTypes.InternalID and Domains.Active > 0 and DomainTypes.AllowAliases > 0 order by Logins.LoginID", custID);
    if (DB.rowCount) while (DB.getrow()) {
        // We now have three columns, we only care about the first two.
        // The first two columns are the username and the domain name.
        // We now do a query for each one on the virtual table
        mailDB.query("select VirtualID from Virtual where Mailbox = '%s'", DB.curRow["DomainName"]);
        if (mailDB.rowCount) {
            sprintf(fullAddr, "%s@%s", DB.curRow["LoginID"], DB.curRow["DomainName"]);
            mailDB.query("select * from Virtual where Address = '%s'", fullAddr);
            if (!mailDB.rowCount) {
                // Wasn't found.  Add the alias.
                mailDB.clearData();
                mailDB.setValue("CustomerID",   custID);
                mailDB.setValue("AliasDate",    timedate);
                mailDB.setValue("AddedBy",      addedBy);
                mailDB.setValue("Address",      fullAddr);
                mailDB.setValue("Mailbox",      DB.curRow["LoginID"]);
                mailDB.ins();
            }
        }
    }

    // Now, do the "stock" aliases.
    if (doStock) {
        if (domainName) DB.query("select Domains.DomainName, Domains.LoginID from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainName = '%s' and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", custID, domainName);
        else DB.query("select Domains.DomainName, Domains.LoginID from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", custID);
        if (DB.rowCount) while (DB.getrow()) {
            int i = 0;
            // Walk through the stock aliases.
            while (strlen(stockaliases[i])) {
                // Check to see if we're doing email for this domain.
                mailDB.query("select VirtualID from Virtual where Mailbox = '%s'", DB.curRow["DomainName"]);
                if (mailDB.rowCount) {
                    // We are, safe to go forward.
                    sprintf(fullAddr, "%s@%s", stockaliases[i], DB.curRow["DomainName"]);
                    mailDB.query("select * from Virtual where Address = '%s'", fullAddr);
                    if (!mailDB.rowCount) {
                        // Wasn't found.  Add the alias.
                        mailDB.clearData();
                        mailDB.setValue("CustomerID",   custID);
                        mailDB.setValue("AliasDate",    timedate);
                        mailDB.setValue("AddedBy",      addedBy);
                        mailDB.setValue("Address",      fullAddr);
                        mailDB.setValue("Mailbox",      DB.curRow["LoginID"]);
                        mailDB.ins();
                    }
                }
                i++;
            }
        }
    }

    // Finally, we need to have an entry in there for the domain by itself
    // so that Postfix knows that the domain is okay to accept email for.
    // We need to get all of the domains that we take email for and add
    // them into the system.
    /*
    // This code works, we just don't want to do it automatically.
    if (domainName) DB.query("select Domains.DomainName from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainName = '%s' and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", custID, domainName);
    else DB.query("select Domains.DomainName from Domains, DomainTypes where Domains.CustomerID = %ld and Domains.Active > 0 and Domains.DomainType = DomainTypes.InternalID and DomainTypes.AllowAliases > 0 order by DomainName", custID);
    if (DB.rowCount) while (DB.getrow()) {
        mailDB.query("select * from Virtual where Address = '%s' and Mailbox = '%s'", DB.curRow["DomainName"], DB.curRow["DomainName"]);
        if (!mailDB.rowCount) {
            // Wasn't found.  Add it.
            mailDB.clearData();
            mailDB.setValue("CustomerID",   custID);
            mailDB.setValue("AliasDate",    timedate);
            mailDB.setValue("AddedBy",      addedBy);
            mailDB.setValue("Address",      DB.curRow["DomainName"]);
            mailDB.setValue("Mailbox",      DB.curRow["DomainName"]);
            mailDB.ins();
        }
    }
    */
    
    
}

