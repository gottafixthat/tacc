/*
** ParseFile.cpp - A _very_ simple interface to parse a file and replace
**                 keywords/tokes with program data for the purpose of
**                 mailing information to staff members.
**
*/

#include "ParseFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Cfg.h>
#include <qstrlist.h>
#include <qstring.h>
#include <qregexp.h>
#include <BlargDB.h>
#include <ADB.h>
#include <FParse.h>

/*
** parseFile   - The only function for it.  It reads SrcFile, and spits
**               out DstFile.
*/

void    parseFile(const char *SrcFile, const char *DstFile, long CustomerID, const char *LoginID, const char *DomainName)
{
    FParser  parser;
	FILE     *dfp;
	char 	 tmpstr[1024];
    char     tmpDst[1024];
	QStrList tmplist(TRUE);
	QString	 qst;
	QString	 tmpqstr2;
	char     contactName[256];
	char     phoneNumber[32];
	
	char     theDate[64];
	QDate    theQDate;
	
	theQDate    = QDate::currentDate();
	strcpy(theDate, theQDate.toString());
	
	
	ADB             DB;
	CustomersDB     CDB;
	LoginsDB        LDB;
	AddressesDB     addrDB;
	ADBTable        LTDB;
	
	CDB.get(CustomerID);
	LDB.get(CustomerID, LoginID);
	LTDB.setTableName("LoginTypes");
	LTDB.get(LDB.getLong("LoginType"));
	strcpy(tmpstr, CDB.getStr("BillingAddress"));
	addrDB.get(REF_CUSTOMER, CustomerID, tmpstr);
	
	// Since there is no default phone number, grab the first one from
	// the database.
	strcpy(phoneNumber, "");
	DB.query("select PhoneNumber from PhoneNumbers where Active <> 0 and RefFrom = %d and RefID = %ld", REF_CUSTOMER, CustomerID);
	if (DB.rowCount) {
	    DB.getrow();
	    strcpy(phoneNumber, DB.curRow["PhoneNumber"]);
	}
	
	// fprintf(stderr, "parseFile: Reading File '%s'\n", SrcFile);
	
	// If strlen(DstFile) == 0, then generate a temp file name for it.
	if (!strlen(DstFile)) {
	    tmpnam(tmpDst);
	} else { 
        strcpy(tmpDst, DstFile);
    }

	
	// Check to see if we have a contact name.
	if (strlen((const char *)CDB.getStr("ContactName"))) {
	    strcpy(contactName, CDB.getStr("ContactName"));
	} else {
	    strcpy(contactName, CDB.getStr("FullName"));
	}
	
    parser.set("DomainName",     DomainName);
    parser.set("CompanyName",    CDB.getStr("FullName"));
    parser.set("ContactName",    contactName);
    parser.set("LoginID",        LoginID);
    parser.set("NICName",        contactName);
    parser.set("Addr1",          addrDB.Address1);
    parser.set("Addr2",          addrDB.Address2);
    parser.set("City",           addrDB.City);
    parser.set("State",          addrDB.State);
    parser.set("ZIP",            addrDB.ZIP);
    parser.set("DayPhone",       phoneNumber);
    parser.set("EvePhone",       phoneNumber);
    parser.set("CurrentDate",    theDate);
    parser.set("LoginType",      LTDB.getStr("LoginType"));
    parser.set("LoginTypeD",     LTDB.getStr("Description"));

    // Read in the entire source file.
	dfp = fopen(tmpDst, "w");
    parser.parseFile(SrcFile, dfp);
    fclose(dfp);
}

/*
** parseEmail  - Given a template name and customer information, this will
**               read a template out of the database and create an email
**               record for it.
*/ 

void    parseEmail(const char *tmplName, long CustomerID, const char *LoginID, const char *DomainName, int holdForSync, const char *emailFrom, const char *emailTo, const char *emailSubject, SElement *extraVars)
{
    FParser  parser;
	char 	 tmpstr[1024];
    char     tmpDst[1024];
    char     srcTextFile[1024];
    FILE     *srcTextFP;
    char     srcHTMLFile[1024];
    FILE     *srcHTMLFP;
    int      tmpFP;
    bool     doHTML = false;
	QStrList tmplist(TRUE);
	QString	 qst;
	QString	 tmpqstr2;
	long     lineCount = 0;
	char     contactName[256];
	char     phoneNumber[32];

    // Default fields
    char     msgFrom[1024];
    char     msgTo[1024];
    char     msgSubj[1024];
	
	char     theDate[64];
	QDate    theQDate;
    QTime    theQTime = QTime::currentTime();
    char     curDateTime[1024];
	
	theQDate    = QDate::currentDate();
	strcpy(theDate, theQDate.toString());
	
    sprintf(curDateTime, "%04d%02d%02d%02d%02d%02d",
            theQDate.year(), theQDate.month(), theQDate.day(),
            theQTime.hour(), theQTime.minute(), theQTime.second());
	
	ADB             DB;
	CustomersDB     CDB;
	LoginsDB        LDB;
	AddressesDB     addrDB;
	ADBTable        LTDB;
    ADBTable        emailDB("EmailQueue");

    // Extract the template from the DB and store it in /tmp somewhere
    DB.query("select * from EmailTemplates where Name = '%s'", tmplName);
    if (!DB.rowCount) return;
    DB.getrow();
    strcpy(srcTextFile, "/tmp/taatextXXXXXX");
    strcpy(srcHTMLFile, "/tmp/taahtmlXXXXXX");
    tmpFP = mkstemp(srcTextFile);
    close(tmpFP);
    srcTextFP = fopen(srcTextFile, "w");
    fprintf(srcTextFP, "%s", DB.curRow["TextPart"]);
    fclose(srcTextFP);
    tmpFP = mkstemp(srcHTMLFile);
    close(tmpFP);
    srcHTMLFP = fopen(srcHTMLFile, "w");
    fprintf(srcHTMLFP, "%s", DB.curRow["HTMLPart"]);
    fclose(srcHTMLFP);

    if (strlen(DB.curRow["HTMLPart"])) doHTML = true;

    // Get our defaults from the passed in options.
    strcpy(msgFrom, emailFrom);
    if (strlen(emailTo)) strcpy(msgTo, emailTo);
    else sprintf(msgTo, "%s@blarg.net", LoginID);

    if (strlen(emailSubject)) strcpy(msgSubj, emailSubject);
    else strcpy(msgSubj, DB.curRow["DefaultSubject"]);

	
	CDB.get(CustomerID);
	LDB.get(CustomerID, LoginID);
	LTDB.setTableName("LoginTypes");
	LTDB.get(LDB.getLong("LoginType"));
	strcpy(tmpstr, CDB.getStr("BillingAddress"));
	addrDB.get(REF_CUSTOMER, CustomerID, tmpstr);
	
	// Since there is no default phone number, grab the first one from
	// the database.
	strcpy(phoneNumber, "");
	DB.query("select PhoneNumber from PhoneNumbers where Active <> 0 and RefFrom = %d and RefID = %ld", REF_CUSTOMER, CustomerID);
	if (DB.rowCount) {
	    DB.getrow();
	    strcpy(phoneNumber, DB.curRow["PhoneNumber"]);
	}
	
	// fprintf(stderr, "parseFile: Reading File '%s'\n", SrcFile);
	
	
	// Check to see if we have a contact name.
	if (strlen((const char *)CDB.getStr("ContactName"))) {
	    strcpy(contactName, CDB.getStr("ContactName"));
	} else {
	    strcpy(contactName, CDB.getStr("FullName"));
	}
	
    parser.set("DomainName",     DomainName);
    parser.set("CompanyName",    CDB.getStr("FullName"));
    parser.set("ContactName",    contactName);
    parser.set("LoginID",        LoginID);
    parser.set("NICName",        contactName);
    parser.set("Addr1",          addrDB.Address1);
    parser.set("Addr2",          addrDB.Address2);
    parser.set("City",           addrDB.City);
    parser.set("State",          addrDB.State);
    parser.set("ZIP",            addrDB.ZIP);
    parser.set("DayPhone",       phoneNumber);
    parser.set("EvePhone",       phoneNumber);
    parser.set("CurrentDate",    theDate);
    parser.set("LoginType",      LTDB.getStr("LoginType"));
    parser.set("LoginTypeD",     LTDB.getStr("Description"));

    //fprintf(stderr, "Checking for extra variables to parse....\n");
    if (extraVars != NULL) {
        int j = 0;
        while(strlen(extraVars[j].Name)) {
            //fprintf(stderr, "Adding '%s' as '%s'....\n", extraVars[j].Name, extraVars[j].Value);
            parser.set(extraVars[j].Name, extraVars[j].Value);
            j++;
        }
    }

    // Parse the files and put them into the database now.
    emailDB.setValue("ProcessID",       tmplName);
    emailDB.setValue("EmailTime",       curDateTime);
    emailDB.setValue("HoldForSync",     holdForSync);
    emailDB.setValue("EmailFrom",       msgFrom);
    emailDB.setValue("EmailTo",         msgTo);
    emailDB.setValue("EmailSubject",    msgSubj);
    emailDB.setValue("TextBody",        parser.parseFileToMem(srcTextFile));
    if (doHTML) {
        emailDB.setValue("HTMLBody",    parser.parseFileToMem(srcHTMLFile));
    }
    emailDB.ins();


    // Remove our temp files
    unlink(srcTextFile);
    unlink(srcHTMLFile);
}


/*
** VParser is a variable parser which will create a parsed file into
** memory and gives the ability to write it to a file multiple times.
*/

/*
**  VParser   - The constructor.
*/

VParser::VParser()
{
    // Because all of our lists have dynamically allocated data, we do
    // not want the Qt libraries to try deallocating the lists, instead
    // we'll do it manually in our destructor.
    elements.setAutoDelete(FALSE);
    lists.setAutoDelete(FALSE);
    
    currentList     = NULL;
    currentListRow = NULL;
}

/*
**  ~VParser   - The Destructor.
*/

VParser::~VParser()
{
}


/*
** set          - Sets the value for a single element.  If the element is
**                in the list, the value is replaced.  If the element is not
**                in the list, it is added automatically.
*/

void VParser::set(const char *name, const char *value)
{
    SElement    *curItem;
    
    curItem = find(name);
    if (curItem == NULL) {
        curItem = new(SElement);
        curItem->Name  = new(char[strlen(name)  + 1]);
        curItem->Value = new(char[strlen(value) + 1]);
        strcpy(curItem->Name,  name);
        strcpy(curItem->Value, value);
        elements.append(curItem);
    } else {
        delete(curItem->Value);
        curItem->Value = new(char[strlen(value) + 1]);
        strcpy(curItem->Value, value);
    }
}

/*
** get          - Gets the value for an element.
**                Returns NULL if the element is not found.
*/

char *VParser::get(const char *name)
{
    char    *RetVal = NULL;
    SElement *curItem;
    
    curItem = find(name);
    if (curItem != NULL) {
        RetVal = curItem->Value;
    }
    
    return(RetVal);
}

/*
** newList    - Adds a new list into memory.
*/

void VParser::newList(const char *listName)
{
    LList   *curList;
    
    curList = findList(listName);
    if (curList == NULL) {
        curList = new(LList);
        curList->Name  = new(char[strlen(listName)  + 1]);
        strcpy(curList->Name, listName);        
        lists.append(curList);
    }
    currentList = curList;
    currentListRow = currentList->rows.first();
}

/*
** addRow - Adds a new row to the specified list and sets it to be
**          the currently active list row.
*/

void VParser::addRow(const char *listName)
{
    LList   *curList;
    EList   *curRow;
    
    curList = findList(listName);
    if (curList == NULL) {
        curList = new(LList);
        curList->Name  = new(char[strlen(listName)  + 1]);
        strcpy(curList->Name, listName);        
        lists.append(curList);
    }
    currentList = curList;
    
    curRow = new(EList);
    currentList->rows.append(curRow);
    
    currentListRow = curRow;
}

/*
** addColumn    - Sets the value for a single element.  If the element is
**                in the list, the value is replaced.  If the element is not
**                in the list, it is added automatically.
*/

void VParser::addColumn(const char *name, const char *value)
{
    SElement    *curItem = NULL;
    
    curItem = findColumn(name, currentListRow);
    if (curItem == NULL) {
        curItem = new(SElement);
        curItem->Name  = new(char[strlen(name)  + 1]);
        curItem->Value = new(char[strlen(value) + 1]);
        strcpy(curItem->Name,  name);
        strcpy(curItem->Value, value);
        currentListRow->columns.append(curItem);
    } else {
        delete(curItem->Value);
        curItem->Value = new(char[strlen(value) + 1]);
        strcpy(curItem->Value, value);
    }
}



/*
** find         - Scans through the list of elements and finds the specified
**                element name.  If it is not found, it returns NULL, 
**                otherwise, it returns a pointer to it.
*/

SElement *VParser::find(const char *name)
{
    SElement    *RetVal = NULL;
    SElement    *curItem;
    
    if (elements.count()) {
        curItem = elements.first();
        while (curItem != NULL) {
            if (!strcmp(curItem->Name, name)) {
                RetVal = curItem;
                curItem = NULL;
            } else {
                curItem = elements.next();
            }
        }
    }
    
    return(RetVal);
}

/*
** findColumn   - Scans through the current row of elements and finds the 
**                specified name.  If it is not found, it returns NULL, 
**                otherwise, it returns a pointer to it.
*/

SElement *VParser::findColumn(const char *name, EList *row)
{
    SElement    *RetVal = NULL;
    SElement    *curItem;
    
    if (row->columns.count()) {
        curItem = row->columns.first();
        while (curItem != NULL) {
            if (!strcmp(curItem->Name, name)) {
                RetVal = curItem;
                curItem = NULL;
            } else {
                curItem = row->columns.next();
            }
        }
    }
    
    return(RetVal);
}



/*
** findList     - Scans through the list of lists and finds the specified
**                list name.  If it is not found, it returns NULL, 
**                otherwise, it returns a pointer to it.
*/

LList *VParser::findList(const char *listName)
{
    LList    *RetVal = NULL;
    LList    *curItem;
    
    if (lists.count()) {
        curItem = lists.first();
        while (curItem != NULL) {
            if (!strcmp(curItem->Name, listName)) {
                RetVal = curItem;
                curItem = NULL;
            } else {
                curItem = lists.next();
            }
        }
    }
    
    return(RetVal);
}


/*
** parseFile  - Loads a file and parses the peices of it.  It loads the
**              *entire* file into memory, one line at a time, and then
**              parses the individual elements of the file.
**
**              Returns 0 on success, -1 on failure (i.e. file did not
**              exist).
*/

int VParser::parseFile(const char *fileName)
{
    int     RetVal = 0;
    FILE    *fp;
    long    bufsize = 65536;
    char    *buffer;

    buffer = new(char[bufsize]);
        
    fp = fopen(fileName, "r");
    if (fp != NULL) {
        // Well, we were able to read the file...Start parsing it.
        while(fgets(buffer, bufsize - 1, fp) != NULL) {
            // Check for comments in the file.  Skip them.
            if (buffer[0] != '#') {
                fcontents.append(buffer);
            }
        }
        fclose(fp);

        doParsing();

    } else {
        RetVal = -1;
    }

    delete(buffer);

    return(RetVal);
}


/*
** doParsing  - This is the function that actually parses the contents.
**              It scans through the list we've got in memory and does
**              its replacments on things.
**
**              It also checks for the {List listname} token and if found,
**              creates a speial block of memory which encapsulates the
**              data between {List listname} and {EndList listname}
**              and calls the appropriate parsing routines.
*/

void VParser::doParsing(void)
{
    QString curItem;
    QString curListItem;
    QString tmpSt;
    QString workStr;
    QString listName;
    QString listBlock;
    QString endTag;
    QString token;
    QString strtoken;
    int     rpos;
    int     toklen;
    char    tmpStr[1024];
    
    for (curItem = fcontents.first(); curItem != 0; curItem = fcontents.next()) {
        if (curItem.find("{") >= 0) {
            // We have something to parse.
            // See if it is a list.  If it is not, then just do a replace on
            // it, if it is, a list, do different things.
            if (curItem.find("{List ") >= 0) {
                // Okay, we need to get everything between the open and
                // close list tags into a single block for parsing.
                // For right now, we'll require that the {List ...} and
                // {EndList ...} tags be on lines of their own.
                // We will then pass the block and the list to the
                // parser.
                
                // Set up our work string.
                workStr = curItem;
                
                // Get the token from the string.
                rpos = workStr.find("}");
                toklen = rpos - workStr.find("{") + 1;
                token  = workStr.mid(workStr.find("{"), toklen);
                strtoken = token.mid(1, toklen-2);
                
                // Now, get the list name from the strtoken
                listName = strtoken;
                listName.replace(QRegExp("List "), "");
                
                // Now set up our ending string so we can search for the
                // end tag.
                endTag = "{EndList ";
                endTag.append(listName);
                endTag.append("}");
                
                // Now, continue looping through our list and append
                // things to the block we'll be sending through the
                // parser.
                listBlock = "";
                for (curListItem = fcontents.next(); curListItem != 0; curListItem = fcontents.next()) {
                    if (curListItem.find(endTag) >= 0) {
                        // We've found our ending tag.  Abort the scan
                        break;
                        // curListItem = 0;
                    } else {
                        // We didn't find our tag, so append it to the string
                        // to the block to parse.
                        listBlock.append(curListItem);
                    }
                }
                
                // Reset our counter for the main loop
                fcontents.find(curListItem);
                // curItem = curListItem;
                
                // Since we're out of the loop, we have the list block to
                // parse.  First, strip out the list references to all of
                // the {ListName:...} so they just read as {...}
                workStr = "{";
                workStr.append(listName);
                workStr.append(":");
                listBlock.replace(QRegExp(workStr), "{");
                
                // Now, find the list.
                strcpy(tmpStr, listName);
                LList   *tmpList = findList(tmpStr);
                if (tmpList != NULL) {
                    // Got it.  Parse the block for each list element.
                    EList       *curRow;
                    
    		        for (curRow = tmpList->rows.first(); curRow != 0; curRow = tmpList->rows.next()) {
                        parseBlock(listBlock, &curRow->columns);
                    }
                } else {
                    fprintf(stderr, "List '%s' not found!\n", (const char *) listName);
                }
                
                
            } else {
                // Okay, no list items.  We can just parse the
                // string as-is.
                parseBlock(curItem, &elements);
            }
            
        } else {
            pcontents.append(curItem);
        }
    }
}


/*
** parseBlock  - This is the function that actually does the parsing.
**
**               The list of elements must be passed to it.
*/

void VParser::parseBlock(QString block, QList<SElement> *data)
{
    QString     destStr;
    QString     workStr;
    QString     token;
    QString     strtoken;
    int         rpos;
    int         toklen;
    QString     ReplVal;
    
    ReplVal = "";
    
    workStr = block;
    while(workStr.find("{") != -1) {
        // Get the token from the string.
        rpos = workStr.find("}");
        toklen = rpos - workStr.find("{") + 1;
        token  = workStr.mid(workStr.find("{"), toklen);
        strtoken = token.mid(1, toklen-2);
        ReplVal = "";
        
        // Now, get the token from the list.
	    SElement    *curItem;
	    
	    if (data->count()) {
	        for (unsigned int i = 0; i < data->count(); i++) {
    	        curItem = data->at(i);
                if (!strcmp(curItem->Name, strtoken)) {
                    ReplVal = curItem->Value;
	            }
	        }
	    }
	    
	    workStr.replace(QRegExp(token), ReplVal);
        
    }
    
    pcontents.append(workStr);    
}

/*
** writeParsedFile  - This spits out the parsed file's contents into a file.
**                    Returns 0 if no error.
*/

int VParser::writeParsedFile(const char *fileName)
{
    int     RetVal = 0;
    FILE    *fp;

    fp = fopen(fileName, "w");
    
    if (fp != NULL) {
        QString curItem;
    
        for (curItem = pcontents.first(); curItem != 0; curItem = pcontents.next()) {
            fprintf(fp, "%s", (const char *) curItem);
        }
        fflush(fp);
        fclose(fp);
    } else {
        RetVal = -1;
    }
    
    return(RetVal);
}



/*
** showParsedFile  - This spits out the parsed file's contents.
*/

void VParser::showParsedFile(void)
{
    QString curItem;
    
    for (curItem = pcontents.first(); curItem != 0; curItem = pcontents.next()) {
        fprintf(stdout, "%s", (const char *) curItem);
    }
    fflush(stdout);
}


/*
** showDebugInfo - Dumps all of our lists and lists of lists and the data
**                 that they contain.
*/

void VParser::_showDebugInfo(void)
{
    SElement    *curElement;
    LList       *curList;
    EList       *curRow;
    
    printf("\n\nDebugging info for VParser lists:\n\n");
    printf("--------------------------------------------------------\n");
    printf("Singular Elements: %d\n", elements.count());

    if (elements.count()) {
        for (curElement = elements.first(); curElement != NULL; curElement = elements.next()) {
            printf("   %s = %s\n", curElement->Name, curElement->Value);
        }
    }
    
    printf("--------------------------------------------------------\n");
    printf("Lists: %d\n", lists.count());

    if (lists.count()) {
        for (curList = lists.first(); curList != NULL; curList = lists.next()) {
            printf("-----\n");
            printf("  List Name = %s\n", curList->Name);
		    printf("       Rows = %d\n", curList->rows.count());

		    if (curList->rows.count()) {
		        for (curRow = curList->rows.first(); curRow != NULL; curRow = curList->rows.next()) {
		            for (curElement = curRow->columns.first(); curElement != NULL; curElement = curRow->columns.next()) {
    		            printf(" %s = %s    ", curElement->Name, curElement->Value);
    		        }
		            printf("\n");
		        }
		    }
		    printf("\n");
        }
    }
    
    printf("--------------------------------------------------------\n\n\n");
    
}

