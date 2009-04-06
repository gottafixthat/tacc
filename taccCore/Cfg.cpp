/*
** $Id$
**
** Cfg.cpp - A _very_ simple interface to config files for programs.
**
**           A config file consists of multiple lines, with keywords
**           on the left, an equals sign, and the values on the right.
**           Whitespace will be stripped from the beginning and end
**           of the lines.
*/

#include <stdio.h>

#include "Cfg.h"
#include "StrTools.h"

using namespace std;

// We must be able to handle a significant number of config entries.
// And be able to access them very quickly.
__gnu_cxx::StringMap       cfgDict;

char    cfgDelim[1024] = "=";

/*
 * setCfgDelimiter - Changes the default delimiter used to parse config
 *                   files.
*/

void setCfgDelimiter(const char *newDelim)
{
    if (strlen(newDelim) > sizeof(cfgDelim)) return;
    strcpy(cfgDelim, newDelim);
}


/*
** loadConfig   - Loads the specified configuration file into memory.
**
**                Returns 1 on success, 0 on failure.
*/

int    loadCfg(const char *cfgFile)
{
    int         aSize = 32768;
	FILE	    *fp;
	char 	    *tmpstr  = new char[aSize];
	char	    *tmpstr2 = new char[aSize];
    char        *keyStr;
    char        *varStr;
    __gnu_cxx::StrSplitMap tmpList;
    string      tmpStr1;
    string      tmpStr2;
    int         splitCount = 0;
	
    int     RetVal = 1;
	
	
    // Open the specified file
	fp = fopen(cfgFile, "r");
	if (fp != NULL) {
        // File opened, walk through the file a line at a time.
		while ((fgets(tmpstr, aSize, fp) != NULL)) {
			tmpList.clear();
            tmpStr1  = tmpstr;
            StripWhiteSpace(tmpStr1);
			strcpy(tmpstr2, tmpStr1.c_str());
			// Check for comments and blank lines
			if (tmpstr2[0] != '#' && tmpstr2[0] != '%' && tmpstr2[0] != '\n' && tmpstr2[0] != '\0') {
				splitCount = StrSplit(tmpstr2, "=", tmpList, 2);
				if (splitCount) {
					
					// Extract the token
                    tmpStr1 = tmpList[0].c_str();
                    StripWhiteSpace(tmpStr1);
					strcpy(tmpstr2, tmpStr1.c_str());

                    // Extract the value and strip the white space from the
                    // variable value.
                    tmpStr2 = tmpList[1].c_str();
                    StripWhiteSpace(tmpStr2);

                    // Allocate space for the variable to store in our map.
                    keyStr = new char[strlen(tmpStr1.c_str()) + 16];
                    varStr = new char[strlen(tmpStr2.c_str()) + 16];
                    strcpy(keyStr, tmpStr1.c_str());
                    strcpy(varStr, tmpStr2.c_str());
                    
                    // Finally, store it.
                    cfgDict[keyStr] = varStr;
				}
			}
		}
		fclose(fp);
	} else {
	    // No file.
	    // fprintf(stderr, "Unable to load config file '%s'\n", cfgFile);
	    RetVal = 0;
	}
	
	return RetVal;
}

/*
** cfgVal - Returns the value of the passed in configuration token.
*/

const char *cfgVal(const char *Token)
{
    return cfgDict[Token].c_str();
}

/*
** setCfgVal - Makes a copy of the passed in dataVal and adds it to 
**             our configuration dictionary.
*/

void setCfgVal(const char *Token, const char *dataVal)
{
    if (strlen(Token)) {
        char    *tmpKey = new char[strlen(Token)+2];
        char    *tmpStr = new char[strlen(dataVal)+2];
        strcpy(tmpKey, Token);
        strcpy(tmpStr, dataVal);
        cfgDict[tmpKey] = tmpStr;
    }
}

