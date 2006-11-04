/*
***************************************************************************
**
** VoIPCommon - An interface that allows an admin to maintain DID's
** for our VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <VoIPCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* voipServiceFlagText[] = {
    "Voicemail",
    "E911",
    "Directory Assistance",
    "Call Forwarding",
    "International Calling",
    ""
};


/**
  * voipServiceFlagStr - Translates VoIP Service flags into strings.
  */
const char *voipServiceFlagStr(int svcFlag)
{
    char    *retStr = new char[64];
    switch(svcFlag) {
        case VSF_VOICEMAIL:
        case VSF_E911:
        case VSF_DIRECTORYASSIST:
        case VSF_CALLFORWARDING:
        case VSF_INTERNATIONAL:
            strcpy(retStr, voipServiceFlagText[svcFlag-1]);
            break;
        default:
            strcpy(retStr, "Unknown Service Flag");
            break;
    }

    return(retStr);
}


