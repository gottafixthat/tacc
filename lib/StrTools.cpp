/*
** $Id$
**
** StrTools.cpp - A few handy string manipulation functions.
**
*/

#include "StrTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
// #include <defalloc.h>
// #include <stl.h>
// #include <string>


/*
** StripWhiteSpace - Strips all leading and trailing white space from
**                   an STL string.
*/

void StripWhiteSpace(string &src)
{
    bool    finished = false;
    int     endpos   = 0;

    while (!finished) {
        finished = true;

        if (src.size()) {
            if (src[0] == ' ' || src[0] == '\n' || src[0] == '\t') {
                src.erase(0,1);
                finished = false;
            }
            endpos = src.size() - 1;

            if (endpos >= 0) {
                if (src[endpos] == ' ' || src[endpos] == '\n' || src[endpos] == '\t') {
                    src.erase(endpos,1);
                    finished = false;
                }
            }
        }
    }
}

/*
** SimplifyWhiteSpace - Converts tabs to spaces, and removes any double spaces.
*/

void SimplifyWhiteSpace(string &src)
{
    string::size_type   curpos;

    while((curpos = src.find("\t")) != string::npos) src.replace(curpos, 1, " ");
    while((curpos = src.find("  ")) != string::npos) src.replace(curpos, 1, "");
}

/*
** StrSplit  - Splits a string into a hash_map, keyed on an int.
**             This will allow us to use a for() loop to walk through
**             a split string.
**             It will return the number of split items.
*/

int StrSplit(const char *src, const char *delim, __gnu_cxx::StrSplitMap &map, int maxParts)
{

    string  workStr;
    workStr = src;
    return StringSplit(workStr, delim, map, maxParts);
#if 0
    int                 entryCount = 0;

    char                *workStr;
    
    char                *tmpStr;
    char                *part;
    

    if (!strlen(src)) return 0;

    workStr = new char[strlen(src)+1024];
    strcpy(workStr, src);

    part = strsep(&workStr, delim);

    while (part != NULL) {
        if ((maxParts) && (entryCount >= maxParts)) {
            // The user specified a maximum number of parts, append
            // this entry to the previous part, and make sure we
            // put the delimiter back in.
            entryCount--;
            char *tmpStr2 = new char[strlen(part) + strlen(delim) + strlen(map[entryCount].c_str()) + 16];
            strcpy(tmpStr2, map[entryCount].c_str());
            strcat(tmpStr2, delim);
            strcat(tmpStr2, part);
            map[entryCount] = tmpStr2;
            part = strsep(&workStr, delim);
            entryCount++;
        } else {
            tmpStr = new char[strlen(part)+16];
            strcpy(tmpStr, part);
            map[entryCount++] = tmpStr;
            part = strsep(&workStr, delim);
        }
    }

    return entryCount;
#endif
}

/*
** StringSplit  - Splits a string into a hash_map, keyed on an int.
**                This will allow us to use a for() loop to walk through
**                a split string.
**                It will return the number of split items.
*/

int StringSplit(string &src, const char *delim, __gnu_cxx::StrSplitMap &map, int maxParts)
{
    int                 entryCount = 0;
    bool                finished = false;
    string              workStr;
    string              partStr;
    string::size_type   curPos;

    map.clear();

    if (!src.length()) return 0;

    workStr = src;

    //fprintf(stderr, "\n\n----------\nDelimiter = '%s'\n", delim);
    //fprintf(stderr, "Source = '%s'\n", src.c_str());
    while (!finished) {
        entryCount++;
        if ((maxParts) && (entryCount >= maxParts)) {
            // We just hit our maximum number of parts.  Take whatever
            // is left in our workStr, add it to the map, and exit.
            //char *tmpStr2 = new char[workStr.size() + 16];
            //memset(tmpStr2, 0, workStr.size()+15);
            //workStr.copy(tmpStr2, workStr.size(), 0);
            map[entryCount-1] = workStr;
            finished = true;
        } else {
            curPos = workStr.find(delim);
            partStr = "";
            if (curPos != string::npos) {
                // Found another part.
                //fprintf(stderr, "Found part at position %d\n", curPos);
                //fprintf(stderr, "workStr = '%s'\n", workStr.c_str());
                partStr = workStr.substr(0, curPos);
                workStr.erase(0, curPos + strlen(delim));
                // StripWhiteSpace(partStr);
                // StripWhiteSpace(workStr);
                //fprintf(stderr, "Part %d is '%s'\n", entryCount-1, partStr.c_str());
                //fprintf(stderr, "workStr = '%s'\n", workStr.c_str());
                if (!workStr.length()) finished = true;
            } else {
                // No more parts were found.  Check to see if we have anything
                // left in our source string and add it to the map if necessary
                //fprintf(stderr, "Did not find part.  curPos = %d\n", curPos);
                //fprintf(stderr, "workStr = '%s'\n", workStr.c_str());
                partStr  = workStr;
                workStr  = "";
                finished = true;
            }

            //fprintf(stderr, "partStr.length() = '%d'\n", partStr.length());
            if (partStr.length()) {
                //char *tmpStr2 = new char[partStr.size() + 16];
                //memset(tmpStr2, 0, partStr.size()+15);
                //partStr.copy(tmpStr2, partStr.size(), 0);
                map[entryCount-1] = partStr; // tmpStr2;
            }
        }

        // fprintf(stderr, "Part %d is %d bytes long, workStr has %d bytes remaining\n", entryCount-1, map[entryCount-1].size(), workStr.size());
    }

    //fprintf(stderr, "Returning.  EntryCount = %d\n", entryCount);

    return entryCount;

#if 0
    int                 entryCount = 0;

    char                *workStr;
    
    char                *tmpStr;
    char                *part;
    

    if (!strlen(src)) return 0;

    workStr = new char[strlen(src)+1024];
    strcpy(workStr, src);

    part = strsep(&workStr, delim);

    while (part != NULL) {
        if ((maxParts) && (entryCount >= maxParts)) {
            // The user specified a maximum number of parts, append
            // this entry to the previous part, and make sure we
            // put the delimiter back in.
            entryCount--;
            char *tmpStr2 = new char[strlen(part) + strlen(delim) + strlen(map[entryCount].c_str()) + 16];
            strcpy(tmpStr2, map[entryCount].c_str());
            strcat(tmpStr2, delim);
            strcat(tmpStr2, part);
            map[entryCount] = tmpStr2;
            part = strsep(&workStr, delim);
            entryCount++;
        } else {
            tmpStr = new char[strlen(part)+16];
            strcpy(tmpStr, part);
            map[entryCount++] = tmpStr;
            part = strsep(&workStr, delim);
        }
    }

    return entryCount;
#endif
}


/*
** WordWrap  - Wordwraps a string to the specified number of columns.
*/

const char *WordWrap(const char *src, size_t cols)
{
    __gnu_cxx::StrSplitMap         lines;
    __gnu_cxx::StrSplitMap         parts;
    string::size_type   partcount;
    string::size_type   linecount;
    string              startstr;
    string              curline;
    string::size_type   curpos;
    char               *tmpstr;
    char               *retStr;

    retStr = new char[strlen(src) * 2 + 512];
    memset(retStr, 0, strlen(src) * 2 + 512);
    
    linecount = StrSplit(src, "\n", lines);
    for (unsigned int j = 0; j < linecount; j++) {
        startstr.erase();
        curline.erase();
        if (strlen(lines[j].c_str())) {
            tmpstr = new char[strlen(lines[j].c_str())+1024];
            memset(tmpstr, 0, strlen(lines[j].c_str())+1024);
            strcpy(tmpstr, lines[j].c_str());
            startstr.assign(tmpstr);

            // Replace all tabs with spaces.
            while ((curpos = startstr.find("\t")) != string::npos) startstr.replace(curpos, 1, " ");
            while ((curpos = startstr.find("\r")) != string::npos) startstr.replace(curpos, 1, " ");

            // Now, simplify all the white space.
            while ((curpos = startstr.find("  ")) != string::npos) startstr.replace(curpos, 2, " ");
            StripWhiteSpace(startstr);

            partcount = StrSplit(startstr.c_str(), " ", parts);
            for (unsigned int i = 0; i < partcount; i++) {
                if (curline.size() + parts[i].size() > cols) {
                    StripWhiteSpace(curline);
                    curline += "\n";
                    strcat(retStr, curline.c_str());
                    curline.assign(parts[i].c_str());
                    curline += " ";
                } else {
                    curline += parts[i].c_str();
                    curline += " ";
                }
            }
            curline += "\n";
            strcat(retStr, curline.c_str());
        } else {
            strcat(retStr, "\n");
        }
    }

    return retStr;
}

/*
** StringMapAdd - Allocates space for a variable and inserts it into
**                the passed in String Map.
*/

void StringMapAdd(__gnu_cxx::StringMap &map, const char *key, const char *val)
{
    // A test.  If we have this key in the map already, delete it.
    __gnu_cxx::StringMap::iterator    it = map.find(key);
    if (it != map.end()) {
        map.erase(key);
    }
    
    if (val && strlen(val)) {
        char *tmpKey = new char[strlen(key)+16];
        char *tmpVal = new char[strlen(val)+16];
        strcpy(tmpKey, key);
        strcpy(tmpVal, val);
        map[tmpKey] = tmpVal;
    } else {
        char *tmpKey = new char[strlen(key)+16];
        strcpy(tmpKey, key);
        map[tmpKey] = "";
    }
}


/*
** normalizeDate  - Converts a user date in the form m/d/y into a different
**                  date style.  The default form is YYYY-MM-DD.
**
**                  Returns true if it was successful.
*/


bool normalizeDate(const char *src, char *tgt, const char *fmt)
{
    regex_t             preg;
    char                sval[1024];
    size_t              nmatch = 1;
    regmatch_t          pmatch[999];
    bool                isvalid = false;
    string              workStr;
    int                 m,d,y;
    regex_t             subreg;
    size_t              maxsubmatch = 1;
    regmatch_t          submatch[50];

    strcpy(sval, "^[[:digit:]]{1,2}[^[:digit:]][[:digit:]]{1,2}[^[:digit:]][[:digit:]]{1,4}");
    if (!regcomp(&preg, sval, REG_ICASE | REG_EXTENDED)) {
        if (!regexec(&preg, src, nmatch, pmatch, 0)) {

            workStr = src;
            regcomp(&subreg, "^[[:digit:]]{1,2}.", REG_ICASE | REG_EXTENDED);
            // If it is a raw email address, we want to leave intact.
            if (!regexec(&subreg, src, maxsubmatch, submatch, 0)) {
                m = atoi(workStr.substr(submatch[0].rm_so, submatch[0].rm_eo - 1).c_str());
                workStr.erase(submatch[0].rm_so, submatch[0].rm_eo - submatch[0].rm_so);
            }
            if (!regexec(&subreg, workStr.c_str(), maxsubmatch, submatch, 0)) {
                d = atoi(workStr.substr(submatch[0].rm_so, submatch[0].rm_eo - 1).c_str());
                workStr.erase(submatch[0].rm_so, submatch[0].rm_eo - submatch[0].rm_so);
            }
            // The year is all that is left...
            y = atoi(workStr.c_str());
            // Now, validate the values themselves.
            isvalid = true;
            while (y > 100) y -= 100;
            if (y >= 70) y += 1900;
            else y += 2000;
            if (m < 1 || m > 12) isvalid = false;
            if (d < 1) isvalid = false;
            if (d > 31) isvalid = false;
            if (d > 28 && m == 2) {
                if (y % 4) isvalid = false;
            }
            if (d > 30 && m != 1 && m != 3 && m != 5 && m != 8 && m != 10 && m != 12) isvalid = false;

        } else {
            // Try it with a mySQL date format
            strcpy(sval, "^[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2}");
            if (!regcomp(&preg, sval, REG_ICASE | REG_EXTENDED)) {
                if (!regexec(&preg, src, nmatch, pmatch, 0)) {
                    workStr = src;
                    regcomp(&subreg, "^[[:digit:]]{1,4}-", REG_ICASE | REG_EXTENDED);
                    // If it is a raw email address, we want to leave intact.
                    if (!regexec(&subreg, src, maxsubmatch, submatch, 0)) {
                        y = atoi(workStr.substr(submatch[0].rm_so, submatch[0].rm_eo - 1).c_str());
                        workStr.erase(submatch[0].rm_so, submatch[0].rm_eo - submatch[0].rm_so);
                    }
                    if (!regexec(&subreg, workStr.c_str(), maxsubmatch, submatch, 0)) {
                        m = atoi(workStr.substr(submatch[0].rm_so, submatch[0].rm_eo - 1).c_str());
                        workStr.erase(submatch[0].rm_so, submatch[0].rm_eo - submatch[0].rm_so);
                    }
                    // The year is all that is left...
                    d = atoi(workStr.c_str());
                    // Now, validate the values themselves.
                    isvalid = true;
                    while (y > 100) y -= 100;
                    if (y >= 70) y += 1900;
                    else y += 2000;
                    if (m < 1 || m > 12) isvalid = false;
                    if (d < 1) isvalid = false;
                    if (d > 31) isvalid = false;
                    if (d > 28 && m == 2) {
                        if (y % 4) isvalid = false;
                    }
                    if (d > 30 && m != 1 && m != 3 && m != 5 && m != 8 && m != 10 && m != 12) isvalid = false;
                }
            }

        }
        // Now, if the date is valid, we need to format it for whatever
        // format was passed into us.
        if (isvalid) {
            // Take the format and for each character, append
            // something to the target
            char tmpStr[1024];
            int  tmpY = y;
            strcpy(tgt, "");
            for (unsigned int i = 0; i < strlen(fmt); i++) {
                switch (fmt[i]) {
                    case    'M':        // A two digit month
                        sprintf(tmpStr, "%02d", m);
                        break;
                    case    'm':        // The month
                        sprintf(tmpStr, "%d", m);
                        break;
                    case    'D':        // Two digit day
                        sprintf(tmpStr, "%02d", d);
                        break;
                    case    'd':        // The day
                        sprintf(tmpStr, "%d", d);
                        break;
                    case    'Y':        // Four digit year
                        sprintf(tmpStr, "%04d", y);
                        break;
                    case    'y':        // Two digit year
                        while (tmpY > 100) tmpY -= 100;
                        sprintf(tmpStr, "%02d", tmpY);
                        break;
                    default:            // Unkown, pass it through
                        sprintf(tmpStr, "%c", fmt[i]);
                        break;
                }
                strcat(tgt, tmpStr);
            }
        }
    } else {
        fprintf(stderr, "Compilation of regular expression failed.");
    }

    return isvalid;
}


