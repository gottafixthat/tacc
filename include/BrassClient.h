/* $Id: BrassClient.h,v 1.3 2004/01/21 22:33:56 marc Exp $
**
** BrassClient.h  - A unified, objectified interface into the BRASS daemon.
**                  The BRASS daemon allows for the adding, deleting, wiping,
**                  and otherwise altering a login record on a shell server.
*/

#ifndef BRASSCLIENT_H
#define BRASSCLIENT_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "IClient.h"

typedef struct LoginDumpStruct {
    char    loginID[64];
    int     uid;
    int     gid;
    char    gecos[128];
    char    home[128];
    char    shell[128];
    char    passwd[64];
};

class BrassClient : public IClient
{

public:
    BrassClient ();
    ~BrassClient();
    
    int Connect(void);
    int Authenticate(void);
    
    int  SetUser(const char *LoginID);
    int  ValidateLogin(const char *LoginID);
    int  LoginExists(const char *LoginID);
    int  AddLogin(const char *LoginID);
    int  SetPassword(const char *LoginID, const char *Password);
    int  SetFingerName(const char *LoginID, const char *FingerName);
    int  SetFlag(const char *LoginID, const char *Flag, const char *Value);
    int  ZapFlags(const char *LoginID);
    int  LockUser(const char *LoginID);
    int  UnLockUser(const char *LoginID);
    int  WipeUser(const char *LoginID);
    int  DumpUser(const char *LoginID, LoginDumpStruct &dump);
    
    char * ResponseStr(char *Response);
    
private:
	char myServerName[256];
	char myUserName[256];
	char myPassword[256];
	char sResponse[1024];
	int  myPort;
    char myActiveUser[256];
};


#endif // BRASSCLIENT_H
