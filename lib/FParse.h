/*
** $Id: FParse.h,v 1.1.1.1 2002/09/29 18:45:14 marc Exp $
**
***************************************************************************
**
** FParse.h  - Generic file parsing routines.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: FParse.h,v $
** Revision 1.1.1.1  2002/09/29 18:45:14  marc
**
**
**
*/

#ifndef FPARSE_H
#define FPARSE_H

// Standard includes
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

// STL includes
using namespace std;
#include <map>
#include <ext/hash_map>
#include <list>
#include <string>


namespace __gnu_cxx
{
struct fparsedeq 
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    };
};

// StringDict is our basic lookup hash.
//typedef hash_map<const char *, string, hash<const char *>, fparsedeq, single_client_alloc> FPStringDict;
typedef hash_map<const char *, string, hash<const char *>, fparsedeq> __gnu_cxx::FPStringDict;
typedef list<FPStringDict> FPStringList;
typedef hash_map<const char *, FPStringList, hash<const char *>, fparsedeq> FPStringListList;
typedef hash_map<int, string, hash<int> >  FPIntDict;
}

class FParser {
public:
    FParser();
    ~FParser();

    // Utility functions
    void        reset();
    void        setVariableMarkers(const char *newOpen, const char *newClose);
    void        setStripComments(bool newVal);
    
    // Parsing functions
    void        setPath(const char *newPath);
    int         dumpFile(const char *fName, FILE *outfp = stdout);
    void        parseFile(const char *fName, FILE *outfp = stdout);
    char       *parseFileToMem(const char *fName);
    void        set(const char *, const char *, bool escQuotes = true);
    const char *get(const char *);

    // List Functions
    // newList is not needed as insertion is automatic.
    void        addRow(const char *listName);
    void        addColumn(const char *key, const char *val);

    void        _dumpDebugInfo();

protected:
    void        parseBlock(string &block);
    void        parseListBlock(string &sourceblock, const char *listName);
    int         loadFile(const char *fName, string &dest);

    int         _FPSplitString(const char *src, const char *delim, __gnu_cxx::FPIntDict &map);

    void        storeVar(__gnu_cxx::FPStringDict &dict, const char *key, const char *val);

    char                fPath[1024];
    char                openStr[1024];
    char                closeStr[1024];
    int                 openStrLen;
    int                 closeStrLen;
    bool                stripComments;
    pid_t               myPID;
    __gnu_cxx::FPStringDict        simpleVars;
    __gnu_cxx::FPStringListList    lists;
    char                curListName[1024];
};


#endif  // FPARSE_H


