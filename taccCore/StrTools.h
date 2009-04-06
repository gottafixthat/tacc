/*
** $Id: StrTools.h,v 1.2 2001/05/19 19:34:22 marc Exp $
**
***************************************************************************
**
** StrTools.h - A few string tools.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
*/

#ifndef STRTOOLS_H
#define STRTOOLS_H

#include <stdio.h>
#include <stdlib.h>
// #include <defalloc.h>
// #include <stl.h>
//#include <complex>
// #include <strstream>
#include <iostream>
using namespace std;
#include <ext/hash_map>
#include <string>
#include <string.h>


namespace __gnu_cxx
{
    /*
    template<> struct hash< std::string >
    {
        size_t operator()( const std::string& x ) const
        {
            return hash< const char* >()( x.c_str() );
        }
    };
    */

// String Map Dictionary comparison
struct _StrDictEQ
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    };
};

typedef hash_map<const char *, string, hash<const char *>, _StrDictEQ> StringMap;

typedef hash_map<int, string, hash<int> > StrSplitMap;
} 

void        StripWhiteSpace(string &src);
void        SimplifyWhiteSpace(string &src);
int         StrSplit(const char *src, const char *delim, __gnu_cxx::StrSplitMap &map, int maxParts = 0);
int         StringSplit(string &src, const char *delim, __gnu_cxx::StrSplitMap &map, int maxParts = 0);
const char *WordWrap(const char *src, size_t cols = 72);

void    StringMapAdd(__gnu_cxx::StringMap &map, const char *key, const char *val);

bool        normalizeDate(const char *src, char *tgt, const char *fmt = "Y-M-D");

#endif // STRTOOLS_H

