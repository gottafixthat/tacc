/*
** Config.cpp - A _very_ simple interface to config files for programs.
**
**              A config file consists of multiple lines, with keywords
**              on the left, an equals sign, and the values on the right.
**              Whitespace will be stripped from the beginning and end
**              of the lines.
*/

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <Cfg.h>
#include <TAAConfig.h>
#include <qstrlist.h>
#include <qstring.h>
#include <BString.h>
#include <qdict.h>
#include <BlargDB.h>
#include <ADB.h>


/*
** cfgInit  - Initializes the dictionary and loads the configuration values
**            into memory.
*/

void cfgInit(void)
{
    // We want the entries in the dictionary to be auto-deleted when
    // the cfgDict is taken out of scope.
    uid_t   myUID;
    passwd  *pent;
    char    homeCfg[1024];
    
    // Get the uid of this user and thier passwd entry
    myUID = getuid();
    pent = getpwuid(myUID);
    if (pent == NULL) {
        fprintf(stderr, "\nUnable to get user information.\n\n");
        exit(-1);
    }

    // Create the file name for the users "local" configuration file.  This is
    // mostly just used for development.  Keeps things local to the user only and not
    // system wide.
    strcpy(homeCfg, pent->pw_dir);
    strcat(homeCfg, "/.taa/taa.cf");
    
    // load the file configuration first.  These settings can be
    // overridden by the database settings.
    if (!loadCfg(homeCfg)) {
        if (!loadCfg("/etc/taa.cf")) {
            if (!loadCfg("/usr/local/etc/taa.cf")) {
                if (!loadCfg("/usr/local/lib/taa.cf")) {
                    exit(-1);
                }
            }
        }
    }

    // Now, load all of the configuration data into memory.
    ADB DB;
    DB.query("select * from Settings order by InternalID");
    if (DB.rowCount) while (DB.getrow()) {
        char    *tmpStr = new char[strlen(DB.curRow["Setting"])+2];
        strcpy(tmpStr, DB.curRow["Setting"]);
        setCfgVal(DB.curRow["Token"], tmpStr);
    }
}

