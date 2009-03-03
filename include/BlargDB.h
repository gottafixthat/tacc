// $Id: BlargDB.h,v 1.2 2004/04/15 18:13:47 marc Exp $
//
// Definitions for BlargDB, the catch-all database access for TAA.
//

#ifndef BLARGDB_H
#define BLARGDB_H 1

#define MAXFIELDS    99

// Define references
#define REF_CUSTOMER 0
#define REF_LOGIN    1
#define REF_VENDOR   2
#define REF_DOMAIN   3

// Credit card response codes.
#define CCARD_UNK        0          // Unknown error
#define CCARD_OK         1          // All is fine
#define CCARD_INVPRE    -1          // Invalid card number (prefix error)
#define CCARD_INVNUM    -2          // Invalid card number (length)
#define CCARD_INVMOD    -3          // Failed the MOD10 test.
#define CCARD_EXP       -4          // Card has expired
#define CCARD_INVEXP    -5          // Card has an invalid expiration date.

// Credit card types
#define CCTYPE_MC       0           // MasterCard
#define CCTYPE_VISA     1           // Visa
#define CCTYPE_AMEX     2           // American Express
#define CCTYPE_DINERS   3           // Diners Club
#define CCTYPE_DISC     4           // Discover

#define CCSTATUS_PENDING    0
#define CCSTATUS_EXPORTED   1
#define CCSTATUS_APPROVED   2
#define CCSTATUS_DECLINED   3

// Subscription links
#define SUBSCRLINK_LOGINS       1
#define SUBSCRLINK_DOMAINS      2
#define SUBSCRLINK_LOGINDISK    3
#define SUBSCRLINK_LOGINDCHAN   4

#include <mysql/mysql.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qdatetm.h>
#include <qpainter.h>
#include "ADB.h"

class BlargDB
{

public:
    BlargDB();
    ~BlargDB();

    // void connect(char *Host, char *User, char *Pass, char *DBName);
    void query(const char *format, ... );
    float fSum(const char *format, ... );
    int  getrow(void);
    int  getfield(void);
    
    long dbcmd(const char *format, ... );

    QString	escapeString(QString s, int truncLen = 4096);

    // numFields will tell the calling routine how many times to call
    // getfield()
    uint          numFields;
    MYSQL_ROW     curRow;
    unsigned long rowCount;
    
    int    connected;
    
    MYSQL        *MySock;

//private slots:

private:
    MYSQL         MyConn;
    MYSQL_RES     *queryRes;
    unsigned long *rowLengths;
    MYSQL_FIELD   *curField;
    int			  Instance;
};

// PaymentTerms layout

class PaymentTermsDB : public ADBTable
{
public:
    PaymentTermsDB(void);
    ~PaymentTermsDB();
};

// AcctsRecv (AR) layout

class AcctsRecvDB : public ADBTable
{
public:
    AcctsRecvDB(void);
    ~AcctsRecvDB();

    virtual void postIns(void);
    virtual void postUpd(void);
    virtual void postDel(void);
};

// VendorTypes layout

class VendorTypesDB
{
public:
    VendorTypesDB(void);
    ~VendorTypesDB();

    int get(int IntID);
    int ins(void);
    int upd(void);
    int del(int IntID);

    int     InternalID;
    QString VendorType;
    QString HasSubTypes;
    QString SubTypeOf;
    
};

// Vendors layout

class VendorsDB : public ADBTable
{
public:
    VendorsDB(void);
    ~VendorsDB();
};

// Accounts layout

class AccountsDB
{
public:
    AccountsDB(void);
    ~AccountsDB();

    int get(int intAcctNo);
    int ins(void);
    int upd(void);
    int del(int intAcctNo);

    int     IntAccountNo;
    QString AccountNo;
    QString AcctName;
    int     ParentID;
    int     AccountType;
    QString ProviderAccountNo;
    QString TaxLine;
    double  Balance;
    int     TransCount;

};


// Billing Cycles

class BillingCyclesDB 
{
public:
    BillingCyclesDB(void);
    ~BillingCyclesDB();

    int get(long internalID);
    int getByCycleID(const char * cycleID);
    long ins(void);
    int upd(void);
    int del(long internalID);
    
    void getCycleDates(QDate *StartDate, QDate *EndDate, int *TotalDays, int *DaysLeft, char *asOf = NULL, uint custID = 0);
    
    int getDefaultCycle(void);

	long	InternalID;
    QString CycleID;
    QString CycleType;
    QString Description;
    QString Jan;
    QString Feb;
    QString Mar;
    QString Apr;
    QString May;
    QString Jun;
    QString Jul;
    QString Aug;
    QString Sep;
    QString Oct;
    QString Nov;
    QString Dece;
    QString Day;
    QString DefaultCycle;
    QString AnniversaryPeriod;
};

// Billables

class BillablesDataDB : public ADBTable
{
public:
    BillablesDataDB(void);
    virtual ~BillablesDataDB();

    virtual long getItem(long itemNo, long ratePlanID, long cycleID);
	
};

// Billables

class BillablesDB : public ADBTable
{
public:
    BillablesDB(void);
    ~BillablesDB();
};


// Packages

class PackagesDB : public ADBTable
{
public:
    PackagesDB(void);
    ~PackagesDB();

    virtual void postDel(void);
};


// PackagesDataDB

class PackagesDataDB : public ADBTable
{
public:
    PackagesDataDB(void);
    ~PackagesDataDB();
};


// PackageContentsDB 

class PackageContentsDB : public ADBTable
{
public:
    PackageContentsDB(void);
    ~PackageContentsDB();

};


// Customers

class CustomersDB : public ADBTable
{
public:
    CustomersDB(void);
    ~CustomersDB();

    virtual void postUpd(void);
    
    void rerateRatePlan(const char *effDate);
    void rerateBillingCycle(const char *effDate);

    int	doSubscriptions(void);
};

// Addresses

class AddressesDB
{
public:
    AddressesDB(void);
    ~AddressesDB();

    int get(int qRefFrom, long qRefID, const char * qTag);
    int getByID(long intID);
    int ins(void);
    int upd(void);
    int del(int qRefFrom, long qRefID);

    long    InternalID;
    QString RefFrom;
    QString RefID;
    QString Tag;
    QString	International;
    QString	Address1;
    QString	Address2;
    QString	City;
    QString	State;
    QString	ZIP;
    QString	Country;
    QString	PostalCode;
    QString	Active;
    QString	LastModifiedBy;
    QString	LastModified;

};

// PhoneNumbers

class PhoneNumbersDB
{
public:
    PhoneNumbersDB(void);
    ~PhoneNumbersDB();

    int get(int qRefFrom, long qRefID, const char * qTag);
	int getByID(long intID);
    int ins(void);
    int upd(void);
    int del(void);

    long    InternalID;
    QString RefFrom;
    QString RefID;
    QString Tag;
    QString	International;
    QString	PhoneNumber;
    QString	Active;
    QString	LastModifiedBy;
    QString	LastModified;

};


// SubscriptionsDB

class SubscriptionsDB : public ADBTable
{
public:
    SubscriptionsDB(void);
    ~SubscriptionsDB();
    
    virtual void postUpd(void);
    virtual void postIns(void);
    virtual void postDel(void);

    void packageScan(void);
};

// NotesDB

class NotesDB : public ADBTable
{
public:
    NotesDB(void);
    ~NotesDB();
};

// LoginsDB

class LoginsDB : public ADBTable
{
public:
    LoginsDB(void);
    ~LoginsDB();

    long get(long CustID, const char * loginID);
    void del(long CustID, const char * loginID);

    virtual void postUpd(void);
    virtual void postIns(void);
    
    void addSubscriptions(void);
    void updateSubscriptions(int oldType, int newType);
    void updateARForTypeChg(void);
    void changeLoginID(char * newLoginID);

};

// DomainsDB

class DomainsDB : public ADBTable
{
public:
    DomainsDB(void);
    virtual ~DomainsDB();

    void    updateSubscriptions(int oldType, int newType);
    void    updateARForTypeChg(void);
    
    virtual void postIns(void);
    virtual void postUpd(void);
    
    void    addSubscriptions(void);
};

// RatePlansDB

class RatePlansDB : public ADBTable
{
public:
    RatePlansDB(void);
    ~RatePlansDB();

    long    getbytag(const char * searchTag);
    virtual void postDel(void);

};


// CCTransDB
class CCTransDB : public ADBTable
{
public:
    CCTransDB(void);
    ~CCTransDB();

    int  isValid(void);
};


// StatementsDB

class StatementsDB : public ADBTable
{
public:
    StatementsDB(void);
    ~StatementsDB();


    virtual void postDel(void);
    
    void print(QPainter *p);
    void printHeader(QPainter *p, int PageNo);
    void printFooter(QPainter *p, int PageNo, int TotPages);
    void registerHeader(QPainter *p, int PageNo);

private:
    // These are the offsets on the page for printed statements.
    // X1 = Starting X Offset, X2 = Ending X Offset.
    int     transDateX1;
    int     transDateX2;
    int     serviceDateX1;
    int     serviceDateX2;
    int     loginIDX1;
    int     loginIDX2;
    int     descriptionX1;
    int     descriptionX2;
    int     amountX1;
    int     amountX2;
    int     balanceX1;
    int     balanceX2;

    char    headerFont[1024];
    char    bodyFont[1024];
    char    footerFont[1024];
};

// StatementsDataDB - The statement line items...

class StatementsDataDB : public ADBTable
{
public:
    StatementsDataDB(void);
    ~StatementsDataDB();

};

// AutoPaymentsDB - The table that holds automatic payment information for
//                  customers.

class AutoPaymentsDB : public ADBTable
{
public:
    AutoPaymentsDB(void);
    ~AutoPaymentsDB();
};


// Some common functions that can be used all over the place.
void createDomainAliases(long custID, int doStock = 0, const char *domainName = NULL);




#endif // BLARGDB_H
