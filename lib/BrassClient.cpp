/*
** $Id$
**
***************************************************************************
**
** BrassClient - An object interface into the BRASS daemon.  The BRASS
**               daemon allows for adding, deleting, wiping, and otherwise
**               altering a login record on a shell server.
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
** $Log: BrassClient.cpp,v $
** Revision 1.2  2004/01/21 22:33:56  marc
** Added a member function to return the results of the BRASS Dump command.
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <crypt.h>

#include <QtCore/QString>

#include <BlargDB.h>
#include <ADB.h>
#include <Cfg.h>

#include "BrassClient.h"

#ifdef USEDES
#include "bdes.h"
#endif

#define RECV_BUFFER_SIZE  65536
#define SEND_BUFFER_SIZE  65536


BrassClient::BrassClient()

{
    char    tmpstr[1024];
    
    strcpy(myServerName, cfgVal("BrassHost"));
    strcpy(tmpstr, cfgVal("BrassPort"));
    myPort = atoi(tmpstr);
	strcpy(myUserName, cfgVal("BrassUser"));
	strcpy(myPassword, cfgVal("BrassPass"));
	strcpy(sResponse, "");
    strcpy(myActiveUser, myUserName);
}

BrassClient::~BrassClient()
{
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "quit\r\n");
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
}

/*
** Connect    - Connects to the specified BRASS server using the specified
**              user name, password.
**
**              Returns 1 on success, 0 on failure.
*/

int BrassClient::Connect(void)
{
	int RetVal = 1;
    
    if (Open(myServerName, myPort) != '+') RetVal = 0;
	
	return(RetVal);
}

/*
** Authenticate - Authenticates the user with the Brass Server.
**
**              Returns 1 on success, 0 on failure.
*/

int BrassClient::Authenticate(void)
{
    int     RetVal  = 0;
    //char    *dst    = (char *) calloc(65536, sizeof(char));
    //char    *src    = (char *) calloc(65536, sizeof(char));
    
    //sprintf(src, "%ld %s %s", time(NULL), myUserName, myPassword);
    
    //encrypt_string2((unsigned char *) src, (unsigned char *) dst);
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    //strcpy(mSendBuffer, "auth ");
    //strncat(mSendBuffer, dst, SEND_BUFFER_SIZE-32);
    //strcat(mSendBuffer, "\r\n");
    strcpy(mSendBuffer, "user ");
    strcat(mSendBuffer, myUserName);
    strcat(mSendBuffer, "\r\n");
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    
    //free(src);
    //free(dst);
    
    if (mStatusCode == '+') {
        // Now send the password.
        strcpy(mSendBuffer, "pass ");
        strcat(mSendBuffer, myPassword);
        strcat(mSendBuffer, "\r\n");
        int bufferLen = strlen(mSendBuffer);
        int numSent = Send(mSendBuffer, bufferLen);
        if (numSent == bufferLen) {
            PGetSingleLineResponse();
        }

        if (mStatusCode == '+') RetVal = 1;
    }

	return RetVal;
}

/*
** SetUser  -  Sets the user that the BRASS daemon will be working on for other
**             commands.
**
**             Returns 1 if successful, 0 if not.
**
*/ 

int BrassClient::SetUser(const char *LoginID)
{
	int RetVal = 0;
    
    if (strcmp(LoginID, myActiveUser)) {
        mStatusCode = 0;
        mSingleLineResponse = mMultiLineResponse = "";
        strcpy(mSendBuffer, "SETUSER ");
        strcat(mSendBuffer, LoginID);
        strcat(mSendBuffer, "\r\n");
        
        int bufferLen = strlen(mSendBuffer);
        int numSent = Send(mSendBuffer, bufferLen);
        if (numSent == bufferLen) {
            PGetSingleLineResponse();
            if (mStatusCode == '+') RetVal = 1;
            strcpy(myActiveUser, LoginID);
        }
    } else {
        // The user is already the active user.
        RetVal = 1;
    }
	
	return(RetVal);
}


/*
** ValidateLogin - Checks a login ID for invalid characters, etc.
**                 The only things allowed are [a-z][0-9][-][_]
**
**               Returns 1 if it is good.
**               0 if it is not
**
**               It does not talk to the Daemon at all.
*/ 

int BrassClient::ValidateLogin(const char *LoginID)
{
	int RetVal = 1;
	unsigned int  i;
	char	c;
	
	if (strlen(LoginID)> 0 && strlen(LoginID) <= atoi(cfgVal("MaxLoginIDLength"))) {
		for (i = 0; i < strlen(LoginID); i++) {
			c = LoginID[i];
			if (!(((c >= 'a') && (c <= 'z')) ||
				  ((c >= '0') && (c <= '9')) ||
				   (c == '-') || (c == '_'))) {
		    	RetVal = 0;
		    }
		}
	} else {
		RetVal = 0;
	}
	
	return(RetVal);
}

/*
** LoginExists - Verifies that a login exists on the server.
**
**               Returns the UID if it does (i.e. > 0)
**               0 if it does not
**               -1 the account exists and locked.
*/ 

int BrassClient::LoginExists(const char *LoginID)
{
	int RetVal = 0;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "VRFY ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}



/*
** AddLogin   - Adds the specified login ID to the system.
**              The server assigns the UID, home directory, sets the shell
**              to /bin/false, and assigns a * password.
**              Other functions are needed to change other things...
**
**               Returns the UID if it succeeds (i.e. > 0)
**               0 if it does not
**               < 0 if an error occurred.
*/ 

int BrassClient::AddLogin(const char *LoginID)
{
	int RetVal = 0;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "ADDUSER ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}



/*
** SetPassword - Encrypts the specified password, and sets the users
**               password to this.
**
**               Returns 0 if it was successful.
*/ 

int BrassClient::SetPassword(const char *LoginID, const char *Password)
{
	int RetVal = 0;
    
    if (!SetUser(LoginID)) return RetVal;
    
    // Create the encrypted password
	char salt[8];
	char *cpass;
	QString qs1;
	QString qs2;

	qs1 = LoginID;
	qs2 = qs1.left(2);
	if (qs2.length() < 2) qs2.append("1");
	strcpy(salt, qs2);
	cpass = crypt(Password, salt);
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "CHPWC ");
    strcat(mSendBuffer, cpass);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') {
            // Okay, we were able to set the default user.  Now, change thier
            // password.
            RetVal = 1;
        }
    }
    
	return(RetVal);
}



/*
** SetFingerName - Sets the finger name for a user.
**
**               Returns 0 if it was successful.
*/ 

int BrassClient::SetFingerName(const char *LoginID, const char *FingerName)
{
	int RetVal = 0;

    // Set the user to work on.
    if (!SetUser(LoginID)) return RetVal;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "CHFN ");
    strcat(mSendBuffer, FingerName);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}

/*
** SetFlag - Sets a Flag/Dictionary Item for a user.
**           This modifies the /usr/local/etc/passwd.extend file.
**
**           Returns 0 if it was successful.
*/ 

int BrassClient::SetFlag(const char *LoginID, const char *Flag, const char *Value)
{
	int RetVal = 0;
    int Action = 0;     // Add the boolean flag
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";

    // Set the user to work on.
    if (!SetUser(LoginID)) return RetVal;
    
    // Find out what type of flag it is, and what we want to do with it.
    ADB     DB;
    
    DB.query("select IsBool from LoginFlags where LoginFlag = '%s'", Flag);
    if (DB.rowCount) {
        DB.getrow();
        if (atoi(DB.curRow["IsBool"])) {
            // Its a boolean flag.
            if (!atoi(Value)) {
                // Remove the flag
                Action = 1;
            }
        } else {
            Action = 2;
            // Its not a boolean flag.  If, however, the Value is "0"
            // we want to remove it.
            if (!strcmp(Value, "0")) Action = 3;
        }
    }
    
    // We have one other special case, and that is Shell.  If we see Shell
    // we want to issue the chsh command instead.
    if (!strcasecmp("Shell", Flag)) Action = 4;
           
    switch (Action) {
        case    1:              // Remove a boolean flag.
            strcpy(mSendBuffer, "RMFLAG ");
            strcat(mSendBuffer, Flag);
            break;
            
       case     2:              // Add a variable flag
            strcpy(mSendBuffer, "ADDVFLAG ");
            strcat(mSendBuffer, Flag);
            strcat(mSendBuffer, " ");
            strcat(mSendBuffer, Value);
            break;
            
       case     3:              // Remove a variable flag
            strcpy(mSendBuffer, "RMVFLAG ");
            strcat(mSendBuffer, Flag);
            break;
       
       case     4:              // chsh
            strcpy(mSendBuffer, "CHSH ");
            strcat(mSendBuffer, Value);
            break;
            
       default:                 // Add a boolean flag
            strcpy(mSendBuffer, "ADDFLAG ");
            strcat(mSendBuffer, Flag);
            break;

    }
    // Add our CR/LF to the end.
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	return(RetVal);
}

/*
** ZapFlags - Removes all flags for the specified user.
**
**            Returns 0 if it was successful.
*/ 

int BrassClient::ZapFlags(const char *LoginID)
{
	int RetVal = 0;

    // Set the user to work on.
    if (!SetUser(LoginID)) return RetVal;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "ZAPFLAGS ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}


/*
** LockUser - Locks the specified login ID.
**            This modifies the /usr/local/etc/passwd.lock and /etc/shadow
**            files.
**
**           Returns 0 if it was successful.
*/ 

int BrassClient::LockUser(const char *LoginID)
{
	int RetVal = 0;
	
    // Set the user to work on.
    if (!LoginExists(LoginID)) return RetVal;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "LOCKUSER ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}

/*
** UnLockUser - UnLocks the specified login ID.
**              This modifies the /usr/local/etc/passwd.lock and /etc/shadow
**              files.
**
**           Returns 0 if it was successful.
*/ 

int BrassClient::UnLockUser(const char *LoginID)
{
	int RetVal = 0;
	
    // Set the user to work on.
    if (!LoginExists(LoginID)) return RetVal;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "UNLOCKUSER ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}

/*
** WipeUser - Wipes the specified login ID off of the system.
**            This modifies the /etc/passwd, /etc/shadow, /etc/group,
**            /usr/local/etc/passwd.lock (maybe), and 
**            /usr/local/etc/passwd.extend files.
**
**           Returns 0 if it was successful.
*/ 

int BrassClient::WipeUser(const char *LoginID)
{
	int RetVal = 0;
	
    // Set the user to work on.
    if (!LoginExists(LoginID)) return RetVal;
    
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "WIPEUSER ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");
    
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mStatusCode == '+') RetVal = 1;
    }
	
	return(RetVal);
}

/*
** DumpUser - Does a "dump login" and gets the login ID, UID, GID, gecos,
**            home directory, shell and password.
*/

int BrassClient::DumpUser(const char *LoginID, LoginDumpStruct &dump)
{
    int RetVal = 0;

    // Make sure the login exists.
    if (!LoginExists(LoginID)) return RetVal;

    // Clear out our variables.
    memset(&dump, 0, sizeof(dump));
    dump.uid = -1;
    dump.gid = -1;

    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    strcpy(mSendBuffer, "DUMP ");
    strcat(mSendBuffer, LoginID);
    strcat(mSendBuffer, "\r\n");

    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetMultiLineResponse();
        if (mStatusCode == '+') RetVal = 1;
        //fprintf(stderr, "Response Code: %s\n", mSingleLineResponse.data());
        //fprintf(stderr, "Dump returned:\n%s\n", mMultiLineResponse.data());
        // Get the parts.
        char *dumpStr = new char[mMultiLineResponse.length()+1024];
        char *dumpLine;
        char *token   = new char[mMultiLineResponse.length()+1024];
        char *value   = new char[mMultiLineResponse.length()+1024];
        strcpy(dumpStr, mMultiLineResponse.data());
        dumpLine = strsep(&dumpStr, "\n");
        while(dumpLine) {
            char *lineStr = new char[mMultiLineResponse.length()+1024];
            char *tmpLine;
            // Get rid of any trailing CR's
            //fprintf(stderr, "strcpy(lineStr, dumpLine '%s')\n", dumpLine);
            strcpy(lineStr, dumpLine);
            if (lineStr[strlen(lineStr)-1] == '\r') {
                lineStr[strlen(lineStr)-1] = '\0';
            }
            //fprintf(stderr, "Dump Line = '%s'\n", lineStr);

            // Now, split the line, but only if it has a tab in it.
            //fprintf(stderr, "strcpy(token, '')\n");
            strcpy(token, "");
            //fprintf(stderr, "strcpy(value, '')\n");
            strcpy(value, "");
            if (index(lineStr, '\t')) {
                tmpLine = strsep(&lineStr, "\t");
                if (tmpLine && strlen(tmpLine)) {
                    //fprintf(stderr, "strcpy(token, tmpLine)\n");
                    strcpy(token, tmpLine);
                    tmpLine = strsep(&lineStr, "\t");
                    if (tmpLine && strlen(tmpLine)) {
                        //fprintf(stderr, "strcpy(value, tmpLine)\n");
                        strcpy(value, tmpLine);
                    }
                }
            }

            //fprintf(stderr, "Token = '%s', Value = '%s'\n", token, value);
            // Check the token for one of the values we care about.
            if (!strcasecmp("UID", token)) dump.uid = atoi(value);
            else if (!strcasecmp("GID", token)) dump.gid = atoi(value);
            else if (!strcasecmp("GCOS", token)) strcpy(dump.gecos, value);
            else if (!strcasecmp("GECOS", token)) strcpy(dump.gecos, value);
            else if (!strcasecmp("HomeDir", token)) strcpy(dump.home, value);
            else if (!strcasecmp("Shell", token)) strcpy(dump.shell, value);
            else if (!strcasecmp("Passwd", token)) strcpy(dump.passwd, value);

            dumpLine = strsep(&dumpStr, "\n");
        }
    }
	
	return(RetVal);
}


/*
** ResponseStr - Returns string that the server returned to us.
*/

char *BrassClient::ResponseStr(char *Response)
{
    strcpy(Response, mSingleLineResponse.data());
    return Response;
}

