/* $Id: ParseFile.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** ParseFile.h  - A _very_ simple interface to parse a file and replace
**                keywords with program data for the purposes of mailing
**                them to staff members.
**
*/

#ifndef PARSEFILE_H
#define PARSEFILE_H

#include <qlist.h>
#include <qstrlist.h>
#include <qstring.h>

// The base of all of the elements, the Single Element.  All other lists
// and variables are designed around this data type.  It consists of a name
// and a value (simple, no?).

struct SElement {
    char    *Name;
    char    *Value;
};

void    parseFile(const char *SrcFile, const char *DstFile, long CustomerID, const char *LoginID, const char *DomainName);
void    parseEmail(const char *tmplName, long CustomerID, const char *LoginID,
        const char *DomainName, int holdForSync = 0, const char *emailFrom =
        "support@blarg.net", const char *emailTo = "", const char
        *emailSubject = "", SElement *extraVars = NULL);


// VParseFile - Variable File Parsing (aka weblib/bhtml)

// Define our data elements


// The Element List, which contains a single "row" of data for a list.
// Each item in an EList contains another list with SElements in it.
struct EList {
    QList<SElement> columns;
};

struct LList {
    char            *Name;
    QList<EList>    rows;
};

class VParser
{
public:
    VParser();
    ~VParser();

    void    set(const char *name, const char *value);
    char    *get(const char *name);
    
    void    newList(const char *listName);
    void    addRow(const char *listName);
    void    addColumn(const char *colName, const char *value);

    int     parseFile(const char *fileName);
    int     writeParsedFile(const char *fileName);

    void    _showDebugInfo(void);

    
protected:
    SElement    *find(const char *name);
    SElement    *findColumn(const char *name, EList *row);
    LList       *findList(const char *listName);
    void        doParsing(void);
    void        parseBlock(QString block, QList<SElement> *data);
    void        showParsedFile(void);
    

private:
    QList<SElement>     elements;
    QList<LList>        lists;
    
    LList               *currentList;
    EList               *currentListRow;
    
    QStrList            fcontents;
    QStrList            pcontents;
    
    int                 bufferOutput;
};



#endif // PARSEFILE_H
