/*
** $Id$
**
***************************************************************************
**
** FParse  - Generic file parsing routines.
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
** $Log: FParse.cpp,v $
** Revision 1.1.1.1  2002/09/29 18:45:14  marc
**
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "FParse.h"


FParser::FParser()
{
    strcpy(openStr, "{");
    strcpy(closeStr, "}");
    openStrLen = 1;
    closeStrLen = 1;
    strcpy(fPath, "");
    stripComments = true;
    
    myPID = getpid();
}

FParser::~FParser()
{
    #ifdef DEBUG
    _dumpDebugInfo();
    #endif
}



/*
** set - Sets a simpleVar.
*/

void FParser::set(const char *key, const char *val, bool escQuotes)
{
    if (escQuotes) {
        string  tmpStr;
        tmpStr = val;
        string::size_type   startpos = 0;
        while ((startpos = tmpStr.find("\"", startpos)) != string::npos) {
            tmpStr.replace(startpos, 1, "&#34;");
        }
        storeVar(simpleVars, key, tmpStr.c_str());
    } else {
        storeVar(simpleVars, key, val);
    }
}

/*
** get - Gets a simpleVar.
*/

const char *FParser::get(const char *key)
{
    __gnu_cxx::FPStringDict::iterator    it = simpleVars.find(key);
    if (it != simpleVars.end()) return simpleVars[key].c_str();
    else return "";
}

/*
** addRow  - Adds a new list row and sets it to be our current one.
*/

void FParser::addRow(const char *listName)
{
    __gnu_cxx::FPStringDict  *tmpDict = new __gnu_cxx::FPStringDict;
    lists[listName].push_back(*tmpDict);
    strcpy(curListName, listName);
}

/*
** addColumn - Sets a column value for the current list.
*/

void FParser::addColumn(const char *key, const char *val)
{
    // Because the STL list.end() returns one past the end, i.e.
    // where the next push_back() call will insert the item, we need
    // to decrement our iterator in order to get the actual last item.
    // Kind of stupid that they would do it that way, but hey...
    __gnu_cxx::FPStringList::iterator    it = lists[curListName].end();
    it--;
    __gnu_cxx::FPStringDict& curMap = *it;
    storeVar(curMap, key, val);
}

/*
** reset - Clears all simple and list variables.
*/

void FParser::reset()
{
    simpleVars.clear();
    lists.clear();
}

/*
** setOpenCloseMarkers - Sets the seperators that identify variables and
**                       commands.
*/

void FParser::setVariableMarkers(const char *newOpen, const char *newClose)
{
    strcpy(openStr, newOpen);
    strcpy(closeStr, newClose);
    openStrLen = strlen(openStr);
    closeStrLen = strlen(closeStr);
}

/*
** setStripComments - Toggles whether or not we will strip comments
**                    out of the source file.
*/

void FParser::setStripComments(bool newVal)
{
    stripComments = newVal;
}


/*
** setPath  - Sets the default path for BHTML files.
*/

void FParser::setPath(const char *newPath)
{
    strcpy(fPath, newPath);
}

/*
** dumpFile - This dumps a .bhtml file located in 'fPath' and sends it
**            to the specified file (default = stdout).
**            It does this without parsing it.
*/

int FParser::dumpFile(const char *fName, FILE *outfp)
{
    FILE    *fp;
    int     retVal = 1;

    string  fullPath;
    if (strlen(fPath) && fName[0] != '/') {
        fullPath = fPath;
        fullPath += "/";
    }
    fullPath += fName;

    fp = fopen(fullPath.c_str(), "r");
    if (fp) {
        int     bufsize = 65536;
        char    *buffer = new char[65536];
        while(fgets(buffer, bufsize - 1, fp) != NULL) {
            // Check for comments in the file.  Skip them.
            if (buffer[0] != '#' || !stripComments) {
                fprintf(outfp, "%s", buffer);
            }
        }
        fclose(fp);
    } else {
        retVal = 0;
    }
    return retVal;
}


/*
** parseFile - This parses a .bhtml file located in 'fPath' and sends it
**             to the specified file (default = stdout).
**             This is the workhorse of the BCGI library.
*/

void FParser::parseFile(const char *fName, FILE *outfp)
{
    string              parsed;

    // Load the file into our string, or return.
    if (!loadFile(fName, parsed)) return;

    // Okay, start parsing it...
    parseBlock(parsed);

    fprintf(outfp, "%s", parsed.c_str());

}

/*
** parseFileToMem - This parses a .bhtml file located in 'fPath' and sends it
**                  to the specified file (default = stdout).
**                  This is the workhorse of the BCGI library.
*/

char *FParser::parseFileToMem(const char *fName)
{
    string              parsed;

    // Load the file into our string, or return.
    if (!loadFile(fName, parsed)) return "";

    // Okay, start parsing it...
    parseBlock(parsed);

    // Now, allocate the buffer for our return.
    char *retStr = new char[strlen(parsed.c_str())+1024];
    strcpy(retStr, parsed.c_str());
    return retStr;
}


/*
** parseBlock - This is the member function that does all of the actual
**              work.  It will call itself recursively until the entire
**              block has been parsed.
*/

void FParser::parseBlock(string &block)
{
    string::size_type   startpos = 0;
    string::size_type   endpos = 0;
    string::size_type   tmppos = 0;

    while ((startpos = block.find(openStr, startpos)) != string::npos) {
        // fprintf(stderr, "Star pos = '%d'\n", startpos);
        endpos = block.find(closeStr, startpos+openStrLen);
        // fprintf(stderr, "startpos = '%d', endpos = '%d'\n", startpos, endpos);
        if (endpos != string::npos) {
            char    tmpVarName[4096];
            string  varName;
            string  varVal;
            // Got the relative position of the item in the string.
            // Get the variable name, and delete it from the sting.
            varName = block.substr(startpos+openStrLen, endpos-startpos-closeStrLen);
            // fprintf(stderr, "Found Variable '%s'\n", varName.c_str());
            
            // Check for lists, if's, etc.
            tmppos = varName.find(" ");
            if (tmppos != string::npos) {
                string  action;
                string  modifier;
                action = varName.substr(0, tmppos);
                modifier = varName.substr(tmppos+openStrLen, varName.length() - tmppos);
                
                // We now have an action and a modifier.  Do something
                // with it.
                if (!action.compare("List")) { 
                    // Hey, its a list.  Extract the block between {List name}
                    // and {EndList name}
                    string::size_type   listendpos;
                    string              listBlock;
                    string              endListTag;
                    endListTag  = openStr;
                    endListTag += "EndList ";
                    endListTag += modifier.c_str();
                    endListTag += closeStr;

                    listendpos = block.find(endListTag.c_str(), endpos);
                    if (listendpos != string::npos) {
                        listBlock = block.substr(endpos+closeStrLen, listendpos-closeStrLen-endpos);

                        parseListBlock(listBlock, modifier.c_str());

                        // Now, replace the entire list block with our parsed
                        // list block.
                        block.replace(startpos, listendpos-startpos+endListTag.length(), listBlock);
                    } else {
                        // No EndList found.  Remove the reference
                        block.replace(startpos, endpos-startpos+1, "");
                    }
                } else if (!action.compare("if")) { 
                    // Hey, its an if statement.  Extract the block between
                    // {if Variable} and {endif Variable}
                    string::size_type   listendpos;
                    string              listBlock;
                    string              endListTag;
                    endListTag  = openStr;
                    endListTag += "endif ";
                    endListTag += modifier.c_str();
                    endListTag += closeStr;

                    listendpos = block.find(endListTag.c_str(), endpos);
                    if (listendpos != string::npos) {
                        listBlock = block.substr(endpos+closeStrLen, listendpos-closeStrLen-endpos);

                        // Now, check if the variable both exists and
                        // has a value.  If both are true, then we will
                        // parse this block.
                        __gnu_cxx::FPStringDict::iterator    it = simpleVars.find(modifier.c_str());
                        if (it != simpleVars.end()) {
                            varVal = simpleVars[modifier.c_str()];
                            if (strlen(varVal.c_str())) {
                                // It exists and has a value.  Parse this
                                // block
                                parseBlock(listBlock);
                            } else {
                                listBlock = "";
                            }
                        } else {
                            // If test failed, clear the listBlock
                            listBlock = "";
                        }

                        // Now, replace the entire list block with our parsed
                        // list block.
                        block.replace(startpos, listendpos-startpos+endListTag.length(), listBlock);
                    } else {
                        // No EndList found.  Remove the reference
                        block.replace(startpos, endpos-startpos+closeStrLen, "");
                    }
                } else {
                    // Bogus action.  Remove it.
                    block.replace(startpos, endpos-startpos+closeStrLen, "");
                }
            } else {
                strcpy(tmpVarName, varName.c_str());
                __gnu_cxx::FPStringDict::iterator    it = simpleVars.find(tmpVarName);
                if (it != simpleVars.end()) {
                    varVal = simpleVars[tmpVarName];
                    block.replace(startpos, endpos-startpos+closeStrLen, varVal.c_str());
                } else {
                    // Unknown variable, ignore it.
                    block.replace(startpos, endpos-startpos+closeStrLen, "");
                }
            }
        } else {
            // Bogus variable.  Lose the opening '{'
            block.replace(startpos, openStrLen, "");
        }
    }
}

/*
** parseListBlock - Takes a list name, and a block, and does replacments
**                  on each variable found for each element in a list.
*/

void FParser::parseListBlock(string &sourceblock, const char *listName)
{
    if (!lists[listName].empty()) {
        // This list is valid and has data in it.
        string::size_type   startpos = 0;
        string::size_type   endpos = 0;
        string::size_type   tmppos = 0;

        string  doneblock;
        string  block;
        __gnu_cxx::FPStringList::iterator it = lists[listName].begin();
        while (it != lists[listName].end()) {
            __gnu_cxx::FPStringDict& curMap = *it;
            block = sourceblock;

            while ((startpos = block.find(openStr, startpos)) != string::npos) {
                endpos = block.find(closeStr, startpos+openStrLen);
                if (endpos != string::npos) {
                    string  varName;
                    string  varVal;
                    // Got the relative position of the item in the string.
                    // Get the variable name, and delete it from the sting.
                    varName = block.substr(startpos+openStrLen, endpos-startpos-closeStrLen);
                    
                    // Check to make sure it is a list variable
                    tmppos = varName.find(":");
                    if (tmppos != string::npos) {
                        string  action;
                        string  modifier;
                        action = varName.substr(0, tmppos);
                        modifier = varName.substr(tmppos+1, varName.length() - tmppos);
                        
                        // We now have an action and a modifier.  Do something
                        // with it.
                        if (!action.compare(listName)) { 
                            // Hey, its in our list.  Replace it.
                            varVal = curMap[modifier.c_str()];
                            // if (!varVal.length()) log(CGI_WARNING, "Encountered unknown variable '%s'", modifier.c_str());
                            block.replace(startpos, endpos-startpos+closeStrLen, varVal.c_str());
                        }
                    } else {
                        tmppos = varName.find(" ");
                        if (tmppos != string::npos) {
                            string  action;
                            string  modifier;
                            action = varName.substr(0, tmppos);
                            modifier = varName.substr(tmppos+1, varName.length() - tmppos);
                            
                            // We now have an action and a modifier.  Do something
                            // with it.
                            if (!action.compare("List")) { 
                                // Hey, its a list.  Extract the block between {List name}
                                // and {EndList name}
                                string::size_type   listendpos;
                                string              listBlock;
                                string              endListTag;
                                endListTag  = openStr;
                                endListTag += "EndList ";
                                endListTag += modifier.c_str();
                                endListTag += closeStr;

                                listendpos = block.find(endListTag.c_str(), endpos);
                                if (listendpos != string::npos) {
                                    listBlock = block.substr(endpos+closeStrLen, listendpos-closeStrLen-endpos);

                                    parseListBlock(listBlock, modifier.c_str());

                                    // Now, replace the entire list block with our parsed
                                    // list block.
                                    block.replace(startpos, listendpos-startpos+endListTag.length(), listBlock);
                                } else {
                                    // No EndList found.  Remove the reference
                                    block.replace(startpos, endpos-startpos+closeStrLen, "");
                                }
                            } else if (!action.compare("if")) { 
                                // Hey, its an if statement.  Extract the block between
                                // {if Variable} and {endif Variable}
                                string::size_type   listendpos;
                                string              listBlock;
                                string              endListTag;
                                endListTag  = openStr;
                                endListTag += "endif ";
                                endListTag += modifier.c_str();
                                endListTag += closeStr;

                                listendpos = block.find(endListTag.c_str(), endpos);
                                if (listendpos != string::npos) {
                                    listBlock = block.substr(endpos+closeStrLen, listendpos-closeStrLen-endpos);

                                    // Now, check if the variable both exists and
                                    // has a value.  If both are true, then we will
                                    // parse this block.
                                    __gnu_cxx::FPStringDict::iterator    it = simpleVars.find(modifier.c_str());
                                    if (it != simpleVars.end()) {
                                        varVal = simpleVars[modifier.c_str()];
                                        if (strlen(varVal.c_str())) {
                                            // It exists and has a value.  Parse this
                                            // block
                                            parseBlock(listBlock);
                                        } else {
                                            listBlock = "";
                                        }
                                    } else {
                                        // If test failed, clear the listBlock
                                        listBlock = "";
                                    }

                                    // Now, replace the entire list block with our parsed
                                    // list block.
                                    block.replace(startpos, listendpos-startpos+endListTag.length(), listBlock);
                                } else {
                                    // No EndList found.  Remove the reference
                                    block.replace(startpos, endpos-startpos+closeStrLen, "");
                                }
                            } else {
                                // Bogus action.  Remove it.
                                block.replace(startpos, endpos-startpos+closeStrLen, "");
                            }
                        } else {
                            // No ':' or ' ' in it, global variable.
                            varVal = simpleVars[varName.c_str()];
                            // if (!varVal.length()) log(CGI_WARNING, "Encountered unknown variable '%s'", varName.c_str());
                            block.replace(startpos, endpos-startpos+closeStrLen, varVal.c_str());
                        }
                    }
                }
            }
            // parseblock = "List parsed.";

            doneblock += block;
            it++;
        }
        sourceblock = doneblock;
    } else {
        // log(CGI_WARNING, "List '%s' not found or empty", listName);
        sourceblock = "";
    }
}

/*
** loadFile - Loads a file into a string.  Returns 1 on success, 0 on
**            failure.  It appends the file to the end of the string so
**            this function can be called repeatedly on the same string,
**            causing the string to grow.
*/

int FParser::loadFile(const char *fName, string &dest)
{
    int     retVal = 1;
    FILE    *fp;

    string  fullPath;
    if (strlen(fPath) && fName[0] != '/') {
        fullPath = fPath;
        fullPath += "/";
    }
    fullPath += fName;


    fp = fopen(fullPath.c_str(), "r");
    if (fp) {
        int     bufsize = 65536;
        char    *buffer = new char[65536];
        while(fgets(buffer, bufsize - 1, fp) != NULL) {
            // Check for comments in the file.  Skip them.
            if (buffer[0] != '#') {
                dest += buffer;
            } else {
                // Check for an include file...
                if (!strncasecmp(buffer, "#include", 8)) {
                    string  tmpFName = buffer;
                    // Extract the file name and drop the trailing \n
                    tmpFName.erase(0, strlen("#include "));
                    tmpFName.erase(tmpFName.length() - 1, 1);
                    loadFile(tmpFName.c_str(), dest);
                } else if (!stripComments) {
                    dest += buffer;
                }
            }
        }
        fclose(fp);
    } else {
        fprintf(stderr, "FParser::loadFile - Unable to load file '%s'\n", fullPath.c_str());
        retVal = 0;
    }

    return retVal;
}



/*
** storeVar  - Allocates space for a variable and inserts it into
**             the passed in list.
*/

void FParser::storeVar(__gnu_cxx::FPStringDict &dict, const char *key, const char *val)
{
    // A test.  If we have this key in the map already, delete it.
    __gnu_cxx::FPStringDict::iterator    it = dict.find(key);
    if (it != dict.end()) {
        dict.erase(key);
    }
    
    if (val && strlen(val)) {
        char *tmpKey = new char[strlen(key)+16];
        char *tmpVal = new char[strlen(val)+16];
        strcpy(tmpKey, key);
        strcpy(tmpVal, val);
        dict[tmpKey] = tmpVal;
    } else {
        char *tmpKey = new char[strlen(key)+16];
        strcpy(tmpKey, key);
        dict[tmpKey] = "";
    }
}

/*
** FPSplitString - Splits a string into a hash_map, keyed on an int.
**                   This will allow us to use a for() loop to walk through
**                   a split string.
**                   It will return the number of split items.
*/

int FParser::_FPSplitString(const char *src, const char *delim, __gnu_cxx::FPIntDict &map)
{
    int                 entryCount = 0;

    char                *workStr;
    
    char                *tmpStr;
    char                *part;
    

    if (!strlen(src)) return 0;

    workStr = new char[strlen(src)+1024];
    strcpy(workStr, src);

    part = strsep(&workStr, delim);

    while (part != NULL) {
        tmpStr = new char[strlen(part)+16];
        strcpy(tmpStr, part);
        map[entryCount++] = tmpStr;
        part = strsep(&workStr, delim);
    }

    return entryCount;
}

/*
** dumpDebugInfo  - Dumps out all of our maps and lists to stderr.
*/

void FParser::_dumpDebugInfo()
{
    fprintf(stderr, "\nBCGI Debug Info\n\n");
    fprintf(stderr, "simpleVars:\t%d elements\n", simpleVars.size());
    /*
    StringDict::iterator    it = simpleVars.begin();
    while (it != simpleVars.end()) {
        fprintf(stderr, "\tData = %s\n", (*it).Data);
        it++;
    }
    */

    fprintf(stderr, "\n");
}


